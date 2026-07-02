/****************************************************************************
 * Pharos - A Real-Time Secure Operating System                             *
 * Copyright 2020 Pedro Macara and Filipe Monteiro                          *
 *                                                                          *
 * Licensed under the Apache License, Version 2.0 (the "License");          *
 * you may not use this file except in compliance with the License.         *
 * You may obtain a copy of the License at                                  *
 *                                                                          *
 *     http://www.apache.org/licenses/LICENSE-2.0                           *
 *                                                                          *
 * Unless required by applicable law or agreed to in writing, software      *
 * distributed under the License is distributed on an "AS IS" BASIS,        *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. *
 * See the License for the specific language governing permissions and      *
 * limitations under the License.                                           *
 *                                                                          *
 ****************************************************************************/


#include <pharos/kernel/thread/thread.h>
#include <pharos/kernel/thread/sporadicthread.h>
#include <pharos/event.h>
#include <pharos/queue.h>
#include <pharos/semaphore.h>
#include <pharos/kernel/thread/dispatcher.h>
#include <pharos/kernel/partition/partition.h>
#include <pharos/hal/boardApi.h>
#include <pharos/thread.h>
#include <pharos/kernel/core/syscall.h>
#include <pharos/kernel/channel/channel.h>
#include <pharos/hal/cpu/cpu.h>
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/kernel/scheduler/schedulerinline.h>
#include <pharos/kernel/future/futureinline.h>


static void sporadicThreadBody(void);
static void activateEvent(ptrSporadicThread thread);
static void activateMutex(ptrSporadicThread thread);
static void activateLwQueue(ptrSporadicThread thread);
static void activateHwQueue(ptrSporadicThread thread);
static void activateChannel(ptrSporadicThread thread);
void pharosISporadicJobEnd(ptrSporadicThread sporadic);
static void *sporadicEntryPoint(ptrSporadicThread thread);


typedef void (*SporadicActivationMethod)(ptrSporadicThread thread);

extern uint8_t pharosSporadicThreadEndMutex;
extern uint8_t pharosSporadicThreadEndEvent;
extern uint8_t pharosSporadicThreadEndLwQueue;
extern uint8_t pharosSporadicThreadEndHwQueue;
extern uint8_t pharosSporadicThreadEndChannel;


/**
 * Activates the deadline handler for the sporadic thread
 * 
 * @param thread thread to set the deadline
 */
static void pharosISporadicThreadActivateDeadline(ptrSporadicThread thread);

static size_t calculateUserStackSpace(SporadicThreadActivationType properties);

/**
 * pointer to the methods associated with each sporadic thread activation type
 */
static const SporadicActivationMethod methods[] = {
    activateMutex ,
    activateEvent ,
    activateLwQueue ,
    activateHwQueue ,
    activateChannel
};


/* global variable labels to where the function _pharosSporadicThreadEnded will go to once the syscall ends */
static const uint8_t * labels[] = {
    /* address where the mutex ends */
    &pharosSporadicThreadEndMutex ,

    /* address where the event ends */
    &pharosSporadicThreadEndEvent ,

    /* address where the lw queue ends */
    &pharosSporadicThreadEndLwQueue ,

    /* address where the hw queue ends */
    &pharosSporadicThreadEndHwQueue ,

    /* address where the channel ends */
    &pharosSporadicThreadEndChannel
};


void activateEvent(ptrSporadicThread thread)
{
    /* event to receive */
    Event event;

    /* interrupt level */
    PharosInterruptLevel interrupt;

    /* get the running partition */
    ptrPartition partition = thread->rtThread.thread.partition;


    /* loop forever */
    for(;;)
    {
        /* disable interrupts so we safely update the state of the thread */
        pharosCpuDisableInterrupts(&interrupt);

        /* set that the thread is waiting for the activation */
        pharosIThreadStateWaitActivation(&thread->rtThread.thread.state);

        /* re-enable interrupts */
        pharosCpuEnableInterrupts(&interrupt);

        /* wait for the event */
        /* MISRA2004 16.10: function will always return PHAROS_EVENT_RECEIVE_SUCCESS */
        pharosIEventReceiveUnprotected(PHAROS_TIMEOUT_INFINITE , &event);

        /* check if the partition should be run with supervisor permissions */
        if(partition->partitionFlags.isSupervisor == TRUE)
        {
            /* run directly the function without switching to user mode */
            thread->activation.entry.eventBody(partition->data , event);

            /* end the execution of the job */
            pharosISporadicJobEnd(thread);
        }
        else
        {
            /* now switch to user mode */
            pharosCpuExecuteUserModeEvent(partition->data , event , &thread->rtThread.thread);
        }
    }
}


void activateMutex(ptrSporadicThread thread)
{
    /* semaphore id to wait for */
    SemMutexNumber id;

    /* semaphore pointer to wait for */
    ptrSemaphoreMutex semaphore;

    /* interrupt level */
    PharosInterruptLevel interrupt;

    /* get the running partition */
    ptrPartition partition = thread->rtThread.thread.partition;


    /* get the semaphore id to where to wait */
    id = thread->activation.entry.mutex.mutexNumber;

    /* get the semaphore pointer */
    semaphore = pharosIGetSemaphoreMutex(id);

    /* loop forever */
    for(;;)
    {
        /* disable interrupts so we safely update the state of the thread */
        pharosCpuDisableInterrupts(&interrupt);

        /* set that the thread is waiting for the activation */
        pharosIThreadStateWaitActivation(&thread->rtThread.thread.state);

        /* re-enable interrupts */
        pharosCpuEnableInterrupts(&interrupt);

        /* wait for the semaphore */
        /* MISRA2004 16.10: function will always return PHAROS_SEMAPHORE_MUTEX_OBTAIN_TIMEOUT_SUCCESS */
        pharosISemMutexObtainTimeout(&thread->rtThread.thread , semaphore , PHAROS_TIMEOUT_INFINITE);

        /* check if the partition should be run with supervisor permissions */
        if(partition->partitionFlags.isSupervisor == TRUE)
        {
            /* run directly the function without switching to user mode */
            thread->activation.entry.mutex.mutexBody(partition->data , id);

            /* end the execution of the job */
            pharosISporadicJobEnd(thread);
        }
        else
        {
            /* now switch to user mode */
            pharosCpuExecuteUserModeMutex(partition->data , id , &thread->rtThread.thread);
        }
    }
}


void activateLwQueue(ptrSporadicThread thread)
{
    /* queue to wait for */
    LwQueueNumber number;

    /* queue pointer to wait for */
    ptrLightWeightQueue queue;

    /* message received from the queue */
    ptrBulk message;

    /* interrupt level */
    PharosInterruptLevel interrupt;

    /* get the running partition */
    ptrPartition partition = thread->rtThread.thread.partition;

    /* buffer */
    Buffer buffer;


    /* get the semaphore id to where to wait */
    number = thread->activation.entry.lwQueue.lwQueueId;

    /* get the queue pointer */
    queue = pharosILwQueueGet(number);

    /* loop forever */
    for(;;)
    {
        /* disable interrupts so we safely update the state of the thread */
        pharosCpuDisableInterrupts(&interrupt);

        /* set that the thread is waiting for the activation */
        pharosIThreadStateWaitActivation(&thread->rtThread.thread.state);

        /* re-enable interrupts */
        pharosCpuEnableInterrupts(&interrupt);

        /* wait for a message */
        /* MISRA2004 16.10: function will always return PHAROS_LIGHTWEIGHT_QUEUE_RECEIVE_TIMEOUT_SUCCESS */
        pharosILwQueueReceiveTimeout(&thread->rtThread.thread , queue , &message , &buffer , PHAROS_TIMEOUT_INFINITE);

        /* check if the partition should be run with supervisor permissions */
        if(partition->partitionFlags.isSupervisor == TRUE)
        {
            /* run directly the function without switching to user mode */
            thread->activation.entry.lwQueue.lwBody(partition->data , number , message , buffer.buffer , buffer.size);

            /* end the execution of the job */
            pharosISporadicJobEnd(thread);
        }
        else
        {
            /* now switch to user mode */
            pharosCpuExecuteUserModeLwQueue(partition->data , number , message , buffer.buffer , buffer.size , &thread->rtThread.thread);
        }
    }
}


void activateHwQueue(ptrSporadicThread thread)
{
    /* queue to wait for */
    HwQueueNumber queueNumber;

    /* queue pointer to wait for */
    ptrHeavyWeightQueue queue;

    /* buffer pointer where to place the message */
    uint8_t *buffer;

    /* size of the message received */
    uint32_t size;

    /* priority of the message received */
    PharosPriority priority;

    /* future on the user stack */
    ptrFutureId future;

    /* future pointer to use (or not) */
    ptrFutureId f;

    /* allocated user stack */
    ptrThreadSporadicStackUserSpace stack;

    /* interrupt level */
    PharosInterruptLevel interrupt;

    /* get the running partition */
    ptrPartition partition = thread->rtThread.thread.partition;


    /* get the allocated user stack */
    stack = (ptrThreadSporadicStackUserSpace) thread->rtThread.thread.userStackAllocateAddress;


    /* get the future on the allocated stack */
    future = &stack->future;

    /* get the semaphore id to where to wait */
    queueNumber = thread->activation.entry.hwQueue.hwQueueNumber;

    /* get the buffer address */
    buffer = thread->activation.entry.hwQueue.buffer;

    /* get the queue pointer */
    queue = pharosIHwQueueGet(queueNumber);

    /* loop forever */
    for(;;)
    {
        /* disable interrupts so we safely update the state of the thread */
        pharosCpuDisableInterrupts(&interrupt);

        /* set that the thread is waiting for the activation */
        pharosIThreadStateWaitActivation(&thread->rtThread.thread.state);

        /* re-enable interrupts */
        pharosCpuEnableInterrupts(&interrupt);

        /* wait for a message */
        /* MISRA2004 16.10: function will always return PHAROS_HEAVY_WEIGHT_RECEIVE_SUCCESS */
        pharosIHwQueueReceive(queue , buffer , &size , &priority , PHAROS_TIMEOUT_INFINITE , future);

        /* check if the future is valid */
        if(pharosIFutureIdIsValid(future) == FALSE)
        {
            f = NULL;
        }
        else
        {
            f = future;
        }

        /* check if the partition should be run with supervisor permissions */
        if(partition->partitionFlags.isSupervisor == TRUE)
        {
            /* run directly the function without switching to user mode */
            thread->activation.entry.hwQueue.hwBody(partition->data , queueNumber , buffer , size , priority , f);

            /* end the execution of the job */
            pharosISporadicJobEnd(thread);
        }
        else
        {
            /* now switch to user mode */
            pharosCpuExecuteUserModeHwQueue(partition->data , queueNumber , buffer , size , priority , f , &thread->rtThread.thread);
        }
    }
}


void activateChannel(ptrSporadicThread thread)
{
    /* result of the channel receive */
    PharosChannelReceiveR result;

    /* channel to wait for */
    ChannelId channelId;

    /* pointer to channel to wait for */
    ptrChannel channel;

    /* bulk */
    ptrChannelBulk bulk;

    /* priority of the message received */
    PharosPriority priority;

    /* future pointer to use (or not) */
    ptrFutureId f;

    /* future on the user stack */
    ptrFutureId future;

    /* get the running partition */
    ptrPartition partition = thread->rtThread.thread.partition;

    /* allocated user stack */
    ptrThreadSporadicStackUserSpace stack;

    /* interrupt level */
    PharosInterruptLevel interrupt;


    /* partition is invalid */
    PHAROS_ASSERT(partition != NULL , "Invalid partition");

    /* get the allocated user stack */
    stack = (ptrThreadSporadicStackUserSpace) thread->rtThread.thread.userStackAllocateAddress;

    /* get the future on the allocated stack */
    future = &stack->future;

    /* get the bulk on the allocated stack */
    bulk = &stack->bulk;

    /* get the semaphore id to where to wait */
    channelId = thread->activation.entry.channel.channelId;

    /* get the queue pointer */
    channel = pharosIChannelGet(&channelId);

    /* loop forever */
    for(;;)
    {
        /* disable interrupts so we safely update the state of the thread */
        pharosCpuDisableInterrupts(&interrupt);

        /* set that the thread is waiting for the activation */
        pharosIThreadStateWaitActivation(&thread->rtThread.thread.state);

        /* re-enable interrupts */
        pharosCpuEnableInterrupts(&interrupt);

        /* wait for a message */
        result = pharosIChannelReceive(channel , bulk , &priority , PHAROS_TIMEOUT_INFINITE , future);

        /* check if successful reception of bulk */
        if(result == PHAROS_CHANNEL_RECEIVE_SUCCESS)
        {
            /* check if the future is valid */
            if(pharosIFutureIdIsValid(future) == FALSE)
            {
                /* no future */
                f = NULL;
            }
            else
            {
                /* with the future received */
                f = future;
            }

            /* check if the partition should be run with supervisor permissions */
            if(partition->partitionFlags.isSupervisor == TRUE)
            {
                /* run directly the function without switching to user mode */
                thread->activation.entry.channel.channelBody(partition->data , bulk , channelId , priority , f);

                /* end the execution of the job */
                pharosISporadicJobEnd(thread);
            }
            else
            {
                /* now switch to user mode */
                pharosCpuExecuteUserModeChannel(partition->data , bulk , channelId , priority , f ,
                                                &thread->rtThread.thread);
            }
        }
            /* an error occurred */
        else
        {
            /* check for invalid MMU configuration
             * Application should give more MMU pages to the partition if this error occurs */
            PHAROS_ASSERT(result != PHAROS_CHANNEL_RECEIVE_INVALID_MMU , "Sporadic thread receiving with not enough MMU pages");

            /* check for invalid shared stack
             * Application should set a sufficient shared stack for this thread is this error occurs */
            PHAROS_ASSERT(result != PHAROS_CHANNEL_RECEIVE_INVALID_SHARED_STACK , "Sporadic thread receiving with invalid shared stack");

            /* end the execution of the job */
            pharosISporadicJobEnd(thread);
        }
    }
}


void *sporadicEntryPoint(ptrSporadicThread thread)
{
    /* entry point to the thread */
    void *entryPoint;

    /* get the entry point depending on the type of activation of the sporadic thread */
    switch(thread->activation.type)
    {
        /* case the thread is activated by mutex */
    case PHAROS_SPORADIC_THREAD_ACTIVATION_BY_MUTEX:

        /* set the entry point to the mutex body */
        entryPoint = thread->activation.entry.mutex.mutexBody;
        break;

        /* case the thread is activated by event */
    case PHAROS_SPORADIC_THREAD_ACTIVATION_BY_EVENT:

        /* set the entry point to the event body */
        entryPoint = thread->activation.entry.eventBody;
        break;

        /* case the thread is activated by lw queue */
    case PHAROS_SPORADIC_THREAD_ACTIVATION_BY_LIGHTWEIGHT_QUEUE:

        /* set the entry point to the mutex lw queue body */
        entryPoint = thread->activation.entry.lwQueue.lwBody;
        break;

        /* case the thread is activated by hw queue */
    case PHAROS_SPORADIC_THREAD_ACTIVATION_BY_HEAVYWEIGHT_QUEUE:

        /* set the entry point to the hw queue body */
        entryPoint = thread->activation.entry.hwQueue.hwBody;
        break;

        /* otherwise */
    default:

        /* assume it is a channel */
        entryPoint = thread->activation.entry.channel.channelBody;

        break;
    }

    return entryPoint;
}


void pharosISporadicThreadInitialize(ptrSporadicThread thread , ptrPartition partition , ThreadNumber number , bool restart)
{
    /* set the user allocated stack size */
    thread->rtThread.thread.userStackAllocateSize = calculateUserStackSpace(thread->activation.type);

    /* initialize common thread data */
    pharosIThreadRtInitialize(&thread->rtThread , partition , FALSE , TRUE , number ,
                              (uintptr_t) sporadicThreadBody , (uintptr_t) sporadicEntryPoint(thread) , restart);

    /* thread has not yet been activated */
    thread->rtThread.thread.threadFlags.hasActivated = FALSE;

    /* set the thread as started */
    pharosIThreadStateSetStarted(&thread->rtThread.thread.state);

    /* make the thread ready (it will block in the beginning) */
    pharosISchedAddReadyThread(&thread->rtThread.thread);

    /* set the thread is waiting for an activation */
    pharosIThreadStateWaitActivation(&thread->rtThread.thread.state);
}


void pharosISporadicJobEnd(ptrSporadicThread sporadic)
{
    /* interrupt level */
    PharosInterruptLevel interrupt;

    ClockTick nextReplenishment = sporadic->rtThread.thread.lastReplenishmentInstant + sporadic->rtThread.thread.replenishBudgetInterval;


    /* remove any deadline handler of the thread */
    pharosITimeTickRemoveDeadline(&sporadic->rtThread);

    /* disable interrupts so we check if the deadline was meet and remove the handler if needed */
    pharosCpuDisableInterrupts(&interrupt);

    /* if the MIT has not yet elapsed */
    if(nextReplenishment > pharosIClockTicksGet(sporadic->rtThread.thread.scheduler))
    {
        /* sleep for at least MIT */
        pharosITimeTickAddThread(&sporadic->rtThread.thread , nextReplenishment , PHAROS_TIME_TICK_AWAKEN_SPORADIC_THREAD_MIT ,
                                 sporadic->rtThread.thread.scheduler);

        /* update state to waiting on sleep */
        pharosIThreadStateWaitSleep(&sporadic->rtThread.thread.state);

        /* remove thread from ready list to force it to wait until next MIT */
        pharosISchedRemoveReadyThread(&sporadic->rtThread.thread);

        /* re-enable interrupts */
        pharosCpuEnableInterrupts(&interrupt);

        /* dispatch to other thread */
        pharosIDispatchThread();

        /* reset the execution time for next job */
        sporadic->rtThread.thread.executionTime = 0U;
    }
        /* else, thread only finished after MIT (but did not took more than WCET). Let it continue to run on the next MIT */
    else
    {
        /* reset the execution time for next job */
        sporadic->rtThread.thread.executionTime = 0U;

        /* re-enable interrupts */
        pharosCpuEnableInterrupts(&interrupt);
    }
}


CpuModeChangeContext pharosISporadicThreadEnded(const uint8_t *returnAddress , CpuModeChangeContext context)
{
    ptrThreadInfo thread;

    /* running sporadic thread */
    ptrSporadicThread sporadic;


    /* get the running sporadic thread */
    thread = pharosISchedGetRunningThread();

    /* if the running thread is sporadic */
    if(pharosIThreadIsSporadic(thread) == TRUE)
    {
        /* then we can end it */

        /* MISRA2004 11.4: pharosIThreadIsSporadic return true so the thread must be a sporadic thread */
        sporadic = (ptrSporadicThread) thread;

        /* check if the return address is correct */
        if(returnAddress == labels[sporadic->activation.type])
        {
            /* end the sporadic thread */
            pharosISporadicJobEnd(sporadic);

            /* when leaving switch to supervisor mode */
            context = pharosCpuSwitchSupervisorMode(context);
        }
        /* else, address is not correct and we won't give supervisor permissions and do anything */
    }
    /* else, do nothing since this is an invalid syscall (performed from an invalid address) */

    /* return the context */
    return context;
}


void sporadicThreadBody(void)
{
    /* when we reach here we are at supervisor mode and with full memory access and interrupts disabled */

    /* running sporadic thread */
    ptrSporadicThread thread;

    /* type of activation for the sporadic thread (e.g. mutex, event, message queue) */
    SporadicThreadActivationType activationType;


    /* enable the interrupts right away */
    pharosCpuInterruptForceEnable();

    /* get the running sporadic thread */
    /* MISRA2004 11.4: this method is only invoked inside a sporadic thread */
    thread = (ptrSporadicThread) pharosISchedGetRunningThread();

    /* update the last dispatch time to not consider the time taken until here */
    thread->rtThread.thread.lastDispatchTime = pharosIClockGetSinceBoot();

    /* get the activation type */
    activationType = thread->activation.type;

    /* execute the thread job */
    methods[activationType](thread);
}


void pharosISporadicThreadActivateDeadline(ptrSporadicThread thread)
{
    /* if there is a deadline to set */
    if(thread->rtThread.deadline != PHAROS_DEADLINE_IGNORED)
    {
        /* set the deadline */
        pharosITimeTickAddDeadline(&thread->rtThread ,
                                   thread->rtThread.thread.lastReplenishmentInstant + thread->rtThread.deadline);
    }
}


void pharosIThreadSporadicActivate(ptrThreadInfo thread)
{
    /* interrupt level */
    PharosInterruptLevel interrupt;

    /* sporadic thread pointer */
    ptrSporadicThread sporadic;


    /* check if the thread is sporadic */
    if(pharosIThreadIsSporadic(thread) == TRUE)
    {
        /* MISRA2004 11.4: if pharosIThreadIsSporadic returns TRUE then the thread is a sporadic thread */
        sporadic = (ptrSporadicThread) thread;

        /* disable interrupts so nothing can change the value of the global variables */
        pharosCpuDisableInterrupts(&interrupt);

        /* if the sporadic thread is waiting for an activation */
        if(pharosIThreadStateIsWaitAct(&thread->state) == TRUE)
        {
            /* check if the thread has not been released yet */
            if(thread->threadFlags.hasActivated == FALSE)
            {
                thread->threadFlags.hasActivated = TRUE;

                /* set the first release instant */
                thread->lastReplenishmentInstant = pharosIClockTicksGet(thread->scheduler);
            }
                /* if the thread has had several releases */
            else
            {
                /* initialize the release instant to the current clock tick but after the MIT */
                thread->lastReplenishmentInstant = MAX(pharosIClockTicksGet(thread->scheduler) ,
                                                       thread->lastReplenishmentInstant + thread->replenishBudgetInterval);
            }

            /* activate the deadline handler for the sporadic thread */
            pharosISporadicThreadActivateDeadline(sporadic);

            /* clear the activating state */
            pharosIThreadStateClearWaitActivation(&thread->state);
        }

        /* re-enable interrupts*/
        pharosCpuEnableInterrupts(&interrupt);
    }
    /* else, if the thread is not sporadic then this method does not do anything */
}


PharosThreadSporadicCreateR pharosIThreadSporadicActivationValidate(ptrPharosSporadicThreadActivation activation)
{
    /* return value */
    PharosThreadSporadicCreateR result = PHAROS_THREAD_SPORADIC_CREATE_SUCCESS;


    /* check the type is OK (cannot be lower than PHAROS_SPORADIC_THREAD_ACTIVATION_BY_MUTEX) */
    if(activation->type > PHAROS_SPORADIC_THREAD_ACTIVATION_BY_CHANNEL)
    {
        /* type is invalid */
        result = PHAROS_THREAD_SPORADIC_CREATE_INVALID_ACTIVATION;
    }
        /* if activated by a mutex */
    else if(activation->type == PHAROS_SPORADIC_THREAD_ACTIVATION_BY_MUTEX)
    {
        /* check that the mutex exists on the running partition */
        if(pharosIGetSemaphoreMutex(activation->entry.mutex.mutexNumber) == NULL)
        {
            /* does not exist */
            result = PHAROS_THREAD_SPORADIC_CREATE_INVALID_ACTIVATION;
        }
    }
        /* if activated by a lw queue */
    else if(activation->type == PHAROS_SPORADIC_THREAD_ACTIVATION_BY_LIGHTWEIGHT_QUEUE)
    {
        /* check that the lw queue exists on the running partition */
        if(pharosILwQueueGet(activation->entry.lwQueue.lwQueueId) == NULL)
        {
            /* does not exist */
            result = PHAROS_THREAD_SPORADIC_CREATE_INVALID_ACTIVATION;
        }
    }
        /* if activated by a hw queue */
    else if(activation->type == PHAROS_SPORADIC_THREAD_ACTIVATION_BY_HEAVYWEIGHT_QUEUE)
    {
        /* check that the hw queue exists on the running partition */
        if(pharosIHwQueueGet(activation->entry.hwQueue.hwQueueNumber) == NULL)
        {
            /* does not exist */
            result = PHAROS_THREAD_SPORADIC_CREATE_INVALID_ACTIVATION;
        }
    }
        /* if activated by a channel */
    else if(activation->type == PHAROS_SPORADIC_THREAD_ACTIVATION_BY_CHANNEL)
    {
        ptrChannel channel = pharosIChannelGet(&activation->entry.channel.channelId);

        /* check that the channel exists on the running partition */
        if(channel == NULL)
        {
            /* does not exist */
            result = PHAROS_THREAD_SPORADIC_CREATE_INVALID_ACTIVATION;
        }
        else
        {
            /* success */
            result = PHAROS_THREAD_SPORADIC_CREATE_SUCCESS;
        }
    }
        /* else if by an event there is no problem */
    else
    {
        /* success */
        result = PHAROS_THREAD_SPORADIC_CREATE_SUCCESS;
    }

    /* return success or the error */
    return result;
}


size_t calculateUserStackSpace(SporadicThreadActivationType activation)
{
    /* return value */
    size_t result;


    /* hw queue or channel have different stack space for the future id */
    if(activation < PHAROS_SPORADIC_THREAD_ACTIVATION_BY_HEAVYWEIGHT_QUEUE)
    {
        /* not a hw queue or channel so return the normal amount of stack space */
        result = sizeof (ThreadRegularStackUserSpace);
    }
    else
    {
        /* a hw queue or channel so return the extra space for the future id (and other stuff) */
        result = sizeof (ThreadSporadicStackUserSpace);
    }

    /* return the space of the user stack to allocate */
    return result;
}


PharosThreadSporadicCreateR pharosSThreadSporadicCreate(const ptrPharosThreadSporadicProperties properties , ptrThreadId id)
{
    /* return value */
    PharosThreadSporadicCreateR result;

    /* thread being created */
    ptrThreadInfo thread;

    /* sporadic thread being created (cast to thread) */
    ptrSporadicThread sporadic;

    /* running partition */
    ptrPartition partition = pharosIPartitionGetRunning();

    /* thread number that will be created */
    ThreadNumber number;

    /* interrupt level */
    PharosInterruptLevel level;


    /* validate the basic properties */
    result = pharosIThreadPropertiesValidate(&properties->basic , id , sizeof (PharosThreadSporadicProperties));

    /* check if the properties are correct */
    if(result == PHAROS_THREAD_SPORADIC_CREATE_SUCCESS)
    {
        /* validate the activation */
        result = pharosIThreadSporadicActivationValidate(&properties->activation);

        /* if activation is OK */
        if(result == PHAROS_THREAD_SPORADIC_CREATE_SUCCESS)
        {
            /* create memory for all the threads stacks and the TCB */
            result = pharosIThreadCreate(&thread , sizeof (SporadicThread) , &properties->basic ,
                                         calculateUserStackSpace(properties->activation.type));

            /* if successfully created (not given out of memory error) */
            if(result == PHAROS_THREAD_SPORADIC_CREATE_SUCCESS)
            {
                /* cast to an aperiodic thread (we created with sizeof(AperiodicThread) */
                sporadic = (ptrSporadicThread) thread;

                /* place the entry point (copy the activation) */
                sporadic->activation = properties->activation;

                /* copy the deadline */
                sporadic->rtThread.deadline = properties->deadline;

                /* disable interrupts: cannot allow an interrupt or another thread to change the _threadsReady at the same time */
                pharosCpuDisableInterrupts(&level);

                /* get the thread number to be used */
                number = partition->sporadicIterator;

                /* increment the aperiodic iterator for the next thread to be created */
                partition->sporadicIterator++;

                /* re-enable interrupts */
                pharosCpuEnableInterrupts(&level);

                /* initialize the thread */
                pharosIThreadRtInitialize(&sporadic->rtThread , partition , FALSE , TRUE , number ,
                                          (uintptr_t) sporadicThreadBody , (uintptr_t) sporadicEntryPoint(sporadic) ,
                                          FALSE);

                /* thread has not yet been activated */
                thread->threadFlags.hasActivated = FALSE;

                /* add the thread to the partition */
                partitionAddSporadicThread(partition , sporadic);

                /* copy the id */
                *id = sporadic->rtThread.thread.threadId;

                /* but do not place it in the ready queue */
            }
        }
        /* else, just return the error */
    }
    /* else, just return the error */

    /* return the result */
    return result;
}
