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
#include <pharos/hal/boardApi.h>
#include <pharos/kernel/interrupt/interrupt.h>
#include <pharos/kernel/object/protection.h>
#include <pharos/kernel/object/object.h>
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/kernel/clock/clock.h>
#include <pharos/kernel/thread/rtthread.h>
#include <pharos/kernel/thread/dispatcher.h>
#include <pharos/kernel/thread/state.h>
#include <pharos/kernel/thread/threadinline.h>
#include <pharos/kernel/assert/assert.h>


void pharosIThreadStopOnSchedulerCore(ptrThreadInfo thread)
{
    /* interrupt level */
    PharosInterruptLevel level;


    /* check that the thread is valid */
    PHAROS_ASSERT(thread != NULL , "Thread is NULL");

    /* disable interrupts so nothing can change the value of the thread state */
    pharosCpuDisableInterrupts(&level);

    /* remove the thread from the blocking queue (if any) - even on the remote core if needed */
    pharosIThreadQueueRemoveBlocked(thread);

    /* remove the thread timeout time tick (if needed) */
    pharosITimeTickRemoveThread(thread);

    /* check if the thread is real-time */
    if(pharosIThreadIsRt(thread) == TRUE)
    {
        /* remove any deadline handler */
        pharosITimeTickRemoveDeadline((ptrRtThread) thread);
    }

    /* re-enable interrupts */
    pharosCpuEnableInterrupts(&level);

    /* restore the base priority */
    thread->currentPriority = thread->priority;

    /* initialize the execution time */
    thread->executionTime = 0U;

    /* initialize the last dispatch instant (unused) */
    thread->lastDispatchTime = 0U;

    /* initialize field */
    thread->threadFlags.unblockDueToTimeout = FALSE;

    /* reset the time tick handler */
    thread->timeTickHandler = NULL;

    /* is not currently activated by a time tick */
    thread->threadFlags.hasTimeTickElapsed = FALSE;

    /* reset the time tick handler */
    thread->timeTickHandler = NULL;

    /* not blocked on a future */
    thread->blockedFuture = NULL;

    /* reset type */
    thread->type = 0U;
}


PharosThreadStopR stop(ptrThreadInfo thread , bool rightNow)
{
    /* return value of this method */
    PharosThreadStopR result;

    /* interrupt level */
    PharosInterruptLevel level;


    /* disable interrupts so nothing can change the value of the thread state */
    pharosCpuDisableInterrupts(&level);

    /* if the thread did not yet start */
    if(pharosIThreadStateIsStarted(&thread->state) == FALSE)
    {
        /* since the thread will no longer be re-scheduled, we can enable interrupts now */
        pharosCpuEnableInterrupts(&level);

        /* thread was not yet started */
        result = PHAROS_THREAD_STOP_NOT_STARTED;
    }
        /* if thread can't be stopped right now */
    else if(pharosIThreadStateIsInterPartition(thread) == TRUE)
    {
        /* if we don't have to stop the thread right away */
        if(rightNow == FALSE)
        {
            /* set the thread to stop once it can */
            pharosIThreadStateStopWaitingSet(&thread->state);

            /* re-enable interrupts*/
            pharosCpuEnableInterrupts(&level);

            /* return delayed */
            result = PHAROS_THREAD_STOP_SELF_DELAY;
        }
        else
        {
            /* re-enable interrupts*/
            pharosCpuEnableInterrupts(&level);

            /* cannot stop the thread right away */
            result = PHAROS_THREAD_STOP_INTER_PARTITION;
        }
    }
    else
    {
        /* update the state to stop the thread. After this point the thread will no longer be able to be scheduled */
        pharosIThreadStateSetStopped(&thread->state);

        /* then remove the thread from the ready list (if on the list) */
        pharosISchedRemoveReadyThread(thread);

        /* since the thread will no longer be re-scheduled, we can enable interrupts now */
        pharosCpuEnableInterrupts(&level);

        /* stop the thread (state is re-initialized) */
        pharosIThreadStop(thread);

        /* dispatch to a possible heir thread */
        pharosIDispatchThread();

        /* return success */
        result = PHAROS_THREAD_STOP_SUCESS;
    }

    /* return success or the error */
    return result;
}


PharosThreadStopSelfR pharosSThreadStopSelf(bool rightNow)
{
    PharosThreadStopSelfR result;

    /* check if is running inside an interrupt */
    if(pharosIInterruptIsRunning() == TRUE)
    {
        /* inside an interrupt cannot stop self */
        result = PHAROS_THREAD_STOP_SELF_INSIDE_INTERRUPT;
    }
        /* else, we can try to get the valid semaphore call */
    else
    {
        /* stop the running thread */
        result = stop(pharosISchedGetRunningThread() , rightNow);
    }

    /* return success or the error */
    return result;
}


PharosThreadStopR pharosSThreadStop(ptrThreadId id , bool rightNow)
{
    /* return value of this method */
    PharosThreadRestartR result;

    /* thread with the specified id from the partition thread map */
    ptrThreadInfo thread;


    /* check that the id is at a valid address */
    if(pharosCpuBufferIsInsideProtection((uint8_t *) id , sizeof (ThreadId) , pharosIPartitionRunningMemProt()) == FALSE)
    {
        /* return invalid id */
        result = PHAROS_THREAD_STOP_INVALID_ID_ADDRESS;
    }
    else
    {
        /* get the thread with the specified id from the partition thread map */
        thread = pharosIThreadGet(id);

        if(thread != NULL)
        {
            /* stop the thread */
            result = stop(thread , rightNow);
        }
        else
        {
            /* return holding resources */
            result = PHAROS_THREAD_STOP_INVALID_ID;
        }
    }

    /* return success or the error */
    return result;
}
