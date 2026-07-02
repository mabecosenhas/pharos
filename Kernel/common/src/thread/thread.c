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


#include <pharos/kernel/thread/periodicthread.h>
#include <pharos/kernel/thread/sporadicthread.h>
#include <pharos/kernel/thread/aperiodicthread.h>
#include <pharos/kernel/thread/thread.h>
#include <pharos/kernel/thread/idle.h>
#include <pharos/kernel/scheduler/scheduler.h>
#include <pharos/thread.h>
#include <pharos/kernel/thread/dispatcher.h>
#include <pharos/error.h>
#include <pharos/kernel/partition/partition.h>
#include <pharos/kernel/thread/state.h>
#include <pharos/hal/boardApi.h>
#include <pharos/hal/cpu/cpu.h>
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/hal/cpu/sections.h>
#include <pharos/kernel/scheduler/schedulerinline.h>


void pharosIThreadRtInitialize(ptrRtThread thread , ptrPartition partition , bool isPeriodic , bool isSporadic ,
                               ThreadNumber number , uintptr_t kernelEntryPoint , uintptr_t userEntryPoint ,
                               bool restart)
{
    /* initialize the thread information */
    pharosIThreadInitialize(&thread->thread , partition , partition->scheduler , isPeriodic , isSporadic , FALSE ,
                            number , kernelEntryPoint , userEntryPoint , restart);

    /* reset the deadline handler (not yet placed) */
    thread->currentDeadlineHandler = NULL;

    /* if starting the thread for the first time */
    if(restart == FALSE)
    {
        /* add the thread time tick to be used when a thread/timers will wait for a specific clock tick */
        pharosIFlatSchedulerTimeTickInit(partition->scheduler , &thread->deadlineTickHandler);
    }

    /* unset the deadline */
    thread->nextDeadline = PHAROS_CLOCK_TICK_MAXIMUM;
}


void pharosIThreadInitialize(ptrThreadInfo thread , ptrPartition partition , ptrFlatScheduler scheduler ,
                             bool isPeriodic , bool isSporadic , bool isAperiodic , ThreadNumber number ,
                             uintptr_t kernelEntryPoint , uintptr_t userEntryPoint , bool restart)
{
    /* set the current priority equal to the base priority */
    thread->currentPriority = thread->priority;

    /* initialize the blocked priority (this initial value is not used, but we initialize it just in case) */
    thread->blockedPriority = thread->priority;

    /* initialize the execution time */
    thread->executionTime = 0U;

    /* initialize the last dispatch instant (unused) */
    thread->lastDispatchTime = 0U;

    /* initialize field */
    thread->threadFlags.unblockDueToTimeout = FALSE;

    /* set the is aperiodic flag */
    thread->threadId.flags.isAperiodic = isAperiodic;

    /* set the is periodic flag */
    thread->threadId.flags.isPeriodic = isPeriodic;

    /* set the is sporadic flag */
    thread->threadId.flags.isSporadic = isSporadic;

    /* initialize the thread has not on the ready list */
    thread->threadFlags.isOnReadyList = FALSE;

    /* set the thread number */
    thread->threadId.number = number;

    /* initialize the suspended awake time with an invalid number */
    thread->suspendedAwakeTime = PHAROS_CLOCK_TICK_MAXIMUM;

    /* initialize the state */
    pharosIThreadStateInitialize(&thread->state);

    /* reset the time tick handler */
    thread->timeTickHandler = NULL;

    /* is not currently activated by a time tick */
    thread->threadFlags.hasTimeTickElapsed = FALSE;

    /* thread not on ready list */
    thread->threadFlags.isOnReadyList = FALSE;

    /* set the scheduler */
    thread->scheduler = scheduler;

    /* initialize the partition */
    thread->partition = partition;

    /* initialize the running partition as the partition */
    thread->runningPartition = partition;

    /* not blocked on a future */
    thread->blockedFuture = NULL;

    /* initialize the suspended state blocking queue */
    thread->suspendedBlockedQueue = NULL;

    /* reset type */
    thread->type = 0U;

    /* initialize the list of resources hold */
    linkedListInitialize(&thread->resourcesHold , OFFSETOF(Resource , threadNode));

    /* initialize the future list */
    linkedListInitialize(&thread->futureReadyQueue , OFFSETOF(Future , queueNode));

    /* initialize the future list */
    linkedListInitialize(&thread->futureBlockedQueue , OFFSETOF(Future , queueNode));

    /* if not restarting the thread (we don't want to do this twice) */
    if(restart == FALSE)
    {
        /* initialize the list of threads with the same priority that are blocked on the same object */
        pharosIThreadQueueInitThreadBlocked(&thread->blockedThreadQueueNode);

        /* initialize the list of threads with the same priority that will awake at the same instant */
        pharosIPqInitThreadWait(&thread->waitThreadQueueNode);

        /* if not in idle thread */
        if(partition != NULL)
        {

            /* add the thread time tick to be used when a thread/timers will wait for a specific clock tick */
            pharosIFlatSchedulerTimeTickInit(scheduler , &thread->tickHandler);
        }
    }

    /* reset the blocked queue (thread is not blocked on any queue) */
    thread->blockedQueue = NULL;

    /* reset the wait queue (thread is not waiting on any time instant queue */
    thread->waitQueue = NULL;

    /* set the user entry point */
    thread->userEntryPoint = userEntryPoint;

    /* set the kernel entry point */
    thread->kernelEntryPoint = kernelEntryPoint;

    /* initialize the queue of ceiling semaphores that this thread will hold (always by priority order) */
    pharosIThreadQueueInitialize(&thread->ceilingSemaphoresHold , FALSE);

    /* initialize the thread stack according to the pre-fill pattern (if any) */
    pharosIThreadStackInit(thread);

    /* if the thread stack did not yet been initialized */
    if(restart == FALSE)
    {
        /* allocate the stack for the user space (TLS or future ids for sporadic threads) */
        thread->userStackAllocateAddress = pharosIStackAllocate(&thread->applicationStack , thread->userStackAllocateSize);
    }

    /* initialize the application thread to access the environment space */
    pharosCpuThreadContextInit(partition , thread , pharosIPartitionMemoryProt(partition));

    /* initialize the RMP information */
    pharosIThreadInitRmp(thread);

    /* set the thread as been created */
    pharosIThreadStateSetCreated(&thread->state);

    /* if the thread has a name */
    if(thread->name != NULL)
    {
        /* copy the name (including the '/0') */
        memcpy(thread->nameInternal , thread->name , strnlen(thread->name , PHAROS_THREAD_NAME_MAX - 1U) + 1U);

        /* if the thread with the same name has not been placed yet */
        if(treeMapContains(&partition->threadsByName , thread->nameInternal) == FALSE)
        {
            /* then put it on the map */
            treeMapPut(&partition->threadsByName , thread->nameInternal , thread);
        }
        /* else, ignore the second thread with the same name, don't place it on the map of names */
    }
}


void pharosIThreadsInitialize(ptrThreadTable threads , ptrPartition partition , bool restart)
{
    /* iterator of the thread number */
    ObjectNumber i;


    /* initialize sporadic threads */
    for(i = 0U; i < threads->sizeSporadicThreads; i++)
    {
        /* initialize the sporadic thread */
        pharosISporadicThreadInitialize(&threads->sporadicThreads[i] , partition , i , restart);
    }

    /* update the sporadic thread number iterator */
    partition->sporadicIterator = i;

    /* initialize aperiodic threads */
    for(i = 0U; i < threads->sizeAperiodicThreads; i++)
    {
        /* initialize the aperiodic thread */
        pharosIAperiodicThreadInit(&threads->aperiodicThreads[i] , partition , i , restart);
    }

    /* update the aperiodic thread number iterator */
    partition->aperiodicIterator = i;

    /* initialize periodic threads */
    for(i = 0U; i < threads->sizePeriodicThreads; i++)
    {
        /* initialize the periodic thread */
        pharosIPeriodicThreadInitialize(&threads->periodicThreads[i] , partition , i , restart);
    }

    /* update the periodic thread number iterator */
    partition->periodicIterator = i;
}


PharosWaitR pharosSWait(ClockTick clockTicks)
{
    /* return value of this method */
    PharosWaitR result;

    /* interrupt level */
    PharosInterruptLevel interrupt;

    /* running thread pointer */
    ptrThreadInfo thread;


    /* check if is running inside an interrupt */
    if(pharosIInterruptIsRunning() == TRUE)
    {
        /* inside an interrupt cannot wait */
        result = PHAROS_WAIT_INSIDE_INTERRUPT;
    }
    else
    {
        /* get the running thread */
        thread = pharosISchedGetRunningThread();

        /* disable interrupts so nothing can change the value of the global variables */
        pharosCpuDisableInterrupts(&interrupt);

        /* add a time tick handler to the thread */
        pharosITimeTickAddThread(thread , clockTicks + pharosIClockTicksGet(thread->scheduler) ,
                                 PHAROS_TIME_TICK_AWAKEN_SLEEP , thread->scheduler);

        /* update state to waiting on sleep */
        pharosIThreadStateWaitSleep(&thread->state);

        /* remove the thread from the list of ready threads */
        pharosISchedRemoveReadyThread(thread);

        /* re-enable interrupts*/
        pharosCpuEnableInterrupts(&interrupt);

        /* dispatch to next higher priority ready thread */
        pharosIDispatchThread();

        /* return success */
        result = PHAROS_WAIT_SUCCESS;
    }

    /* return the method result (wait always returns success unless inside interrupt) */
    return result;
}


bool pharosIThreadHasEnoughTime(ptrThreadInfo thread , Wcet wcet)
{
    /* return of this method */
    bool result;

    /* current time */
    uint64_t currentTime;


    /* if the running thread has a WCET */
    if(thread->wcet != PHAROS_WCET_UNLIMITED)
    {
        /* check the last context switch */

        /* current time */
        currentTime = pharosSClockGetSinceBoot();

        /* executing = current time - last context switch + previous executing */
        thread->executionTime += (uint32_t) (currentTime - thread->lastDispatchTime);

        /* if thread WCET - executing < WCET then does NOT have enough time */
        if(thread->wcet > wcet + thread->executionTime)
        {
            /* thread has enough time to execute before the WCET reaches */
            result = TRUE;
        }
        else
        {
            /* then thread cannot guarantee it will finish before the WCET */
            result = FALSE;
        }
    }
    else
    {
        /* running thread does not have WCET */
        result = TRUE;
    }


    /* return the result */
    return result;
}


ptrThreadInfo pharosIThreadGet(ptrThreadId threadId)
{
    /* result of the function */
    ptrThreadInfo result;

    /* get the running partition */
    ptrPartition partition = pharosIPartitionGetRunning();

    /* get the thread table of the running partition */
    const ptrThreadTable table = &(partition->objects.threadTable);

    /* interrupt level */
    PharosInterruptLevel interrupt;


    /* if the thread is periodic */
    if(threadId->flags.isPeriodic == TRUE)
    {
        /* check if the number is valid */
        if(threadId->number < table->sizePeriodicThreads)
        {
            /* return the corresponding periodic thread */
            result = &table->periodicThreads[threadId->number].rtThread.thread;
        }
            /* if the number is invalid */
        else
        {
            /* disable interrupts so nothing can change the tree map */
            pharosCpuDisableInterrupts(&interrupt);

            /* it isn't valid (is too big). search on the periodic thread maps of the running partition */
            result = treeMapGet(&partition->periodicById , &threadId->number);

            /* re-enable interrupts*/
            pharosCpuEnableInterrupts(&interrupt);
        }
    }
        /* if the thread is sporadic */
    else if(threadId->flags.isSporadic == TRUE)
    {
        /* check if the number is valid */
        if(threadId->number < table->sizeSporadicThreads)
        {
            /* return the corresponding sporadic thread */
            result = &table->sporadicThreads[threadId->number].rtThread.thread;
        }
            /* if the number is invalid */
        else
        {
            /* disable interrupts so nothing can change the tree map */
            pharosCpuDisableInterrupts(&interrupt);

            /* it isn't valid (is too big). search on the sporadic thread maps of the running partition */
            result = treeMapGet(&partition->sporadicById , &threadId->number);

            /* re-enable interrupts*/
            pharosCpuEnableInterrupts(&interrupt);
        }
    }
        /* else is aperiodic thread */
    else
    {
        /* check if the number is valid */
        if(threadId->number < table->sizeAperiodicThreads)
        {
            /* return the corresponding aperiodic thread */
            result = &table->aperiodicThreads[threadId->number].info;
        }
            /* if the number is invalid */
        else
        {
            /* disable interrupts so nothing can change the tree map */
            pharosCpuDisableInterrupts(&interrupt);

            /* it isn't valid (is too big). search on the aperiodic thread maps of the running partition */
            result = treeMapGet(&partition->aperiodicById , &threadId->number);

            /* re-enable interrupts*/
            pharosCpuEnableInterrupts(&interrupt);
        }
    }

    /* return the thread (or NULL if number is invalid) */
    return result;
}


bool pharosIThreadIsAwake(ptrThreadInfo thread)
{
    /* method result */
    bool result;


    /* if the thread is currently waiting */
    if(pharosIThreadStateIsWaiting(&thread->state) == TRUE)
    {
        /* if the thread is not waiting on an event */
        if(thread->timeTickHandler == NULL)
        {
            /* then it is waiting with an infinite timeout */
            result = FALSE;
        }
            /* if the awake time did not elapsed yet */
        else if(thread->timeTickHandler->awakeTime > pharosIClockTicksGet(thread->scheduler))
        {
            /* then it should not be awake */
            result = FALSE;
        }
        else
        {
            /* if the awake time already elapsed (could occur when multiple thread are awaken at the same time and only
             * the highest priority state is updated) */
            result = TRUE;
        }
    }
    else
    {
        /* thread is not waiting so it is awake */
        result = TRUE;
    }

    /* return if the thread should be awake*/
    return result;
}
