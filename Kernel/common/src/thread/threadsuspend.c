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
#include <pharos/kernel/scheduler/scheduler.h>
#include <pharos/kernel/thread/dispatcher.h>
#include <pharos/kernel/core/coreconfinline.h>

/**
 * Suspends the thread
 * 
 * @param thread thread to suspend
 * @param rightNow flag indicating if should suspend right now 
 * 
 * @return returns success or the associated error
 */
static PharosThreadSuspendR suspend(ptrThreadInfo thread , bool rightNow);


PharosThreadSuspendSelfR pharosSThreadSuspendSelf(bool rightNow)
{
    /* return value of this function */
    PharosThreadSuspendSelfR result;


    /* check if is running inside an interrupt */
    if(pharosIInterruptIsRunning() == TRUE)
    {
        /* inside an interrupt cannot wait */
        result = PHAROS_THREAD_SUSPEND_SELF_INSIDE_INTERRUPT;
    }
    else
    {
        /* suspend the running thread */
        result = suspend(pharosISchedGetRunningThread() , rightNow);
    }

    /* return the inside interrupt or the suspension return value */
    return result;
}


PharosThreadSuspendR pharosSThreadSuspend(ptrThreadId id , bool rightNow)
{
    /* return value of this method */
    PharosThreadSuspendR result;

    /* get the thread from its id */
    ptrThreadInfo thread;


    /* check if the id is at a valid address */
    if(pharosCpuBufferIsInsideProtection((uint8_t *) id , sizeof (ThreadId) , pharosIPartitionRunningMemProt()) == FALSE)
    {
        /* return invalid */
        result = PHAROS_THREAD_SUSPEND_INVALID_ID_ADDRESS;
    }
    else
    {
        /* try to get the thread on the running partition */
        thread = pharosIThreadGet(id);

        /* if the id is invalid */
        if(thread == NULL)
        {
            /* return invalid */
            result = PHAROS_THREAD_SUSPEND_INVALID_ID;
        }
        else
        {
            /* suspend the thread */
            result = suspend(thread , rightNow);
        }
    }

    /* return the method result */
    return result;
}


PharosThreadSuspendR suspend(ptrThreadInfo thread , bool rightNow)
{
    /* return value of this method */
    PharosThreadSuspendR result;

    /* interrupt level */
    PharosInterruptLevel level;


    /* disable interrupts so nothing can change the value of the global variables */
    pharosCpuDisableInterrupts(&level);

    /* check if the thread is already suspended */
    if(pharosIThreadStateIsSuspended(&thread->state) == TRUE)
    {
        /* re-enable interrupts */
        pharosCpuEnableInterrupts(&level);

        /* return that it is already suspended */
        result = PHAROS_THREAD_SUSPEND_ALREADY_SUSPENDED;
    }
        /* if thread can't be stopped right now */
    else if(pharosIThreadStateIsInterPartition(thread) == TRUE)
    {
        /* if we don't have to stop the thread right away */
        if(rightNow == FALSE)
        {
            /* set the thread to suspend once it can */
            pharosIThreadStateSuspendWaitingSet(&thread->state);

            /* re-enable interrupts*/
            pharosCpuEnableInterrupts(&level);

            /* return delayed */
            result = PHAROS_THREAD_SUSPEND_DELAY;
        }
        else
        {
            /* re-enable interrupts*/
            pharosCpuEnableInterrupts(&level);

            /* cannot suspend the thread now */
            result = PHAROS_THREAD_SUSPEND_INTER_PARTITION;
        }
    }
    else
    {
        /* update state to suspended */
        pharosIThreadStateSetSuspend(&thread->state);

        /* re-enable interrupts*/
        pharosCpuEnableInterrupts(&level);

        /* continue suspending the thread (on the correct core) */
        pharosIThreadSuspend(thread);

        /* dispatch to another thread (we might be suspending ourselves) */
        pharosIDispatchThread();

        /* thread is now suspended */
        result = PHAROS_THREAD_SUSPEND_SUCCESS;
    }

    /* return the method result */
    return result;
}


void pharosIThreadSuspendOnSchedulerCore(ptrThreadInfo thread)
{
    /* interrupt level */
    PharosInterruptLevel interrupt;

    
    /* disable interrupts so nothing can change the value of the global variables */
    pharosCpuDisableInterrupts(&interrupt);

    /* remove the thread from the blocked queue (if any) */
    thread->suspendedBlockedQueue = pharosIThreadQueueRemoveBlocked(thread);

    /* if the thread is waiting for a specific clock tick */
    if(thread->timeTickHandler != NULL)
    {
        /* copy the awake time */
        thread->suspendedAwakeTime = thread->timeTickHandler->awakeTime;

        /* remove the thread from the tick handler */
        pharosITimeTickRemoveThread(thread);
    }
    else
    {
        /* reset the awake time (invalid value) */
        thread->suspendedAwakeTime = PHAROS_CLOCK_TICK_MAXIMUM;
    }

    /* re-enable interrupts (thread is already in the suspended state so it cannot be 
     * dispatched to - see pharosIContextSwitchPossible) */
    pharosCpuEnableInterrupts(&interrupt);

    /* remove the thread from its current scheduler */
    pharosISchedRemoveReadyThread(thread);
}
