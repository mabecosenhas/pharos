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


#include <pharos/kernel/thread/state.h>
#include <pharos/kernel/thread/thread.h>
#include <pharos/kernel/partition/partition.h>
#include <pharos/hal/atomic.h>
#include <pharos/kernel/thread/dispatcher.h>


void pharosIThreadStateInitialize(const ptrThreadState state)
{
    /* not blocked */
    state->flags.blocked = PHAROS_THREAD_STATE_UNBLOCKED;

    /* thread is initially not suspended */
    state->flags.isSuspended = FALSE;

    /* not waiting any timeout */
    state->flags.wait = PHAROS_THREAD_STATE_NOT_WAITING;

    /* has not been created */
    state->flags.isCreated = FALSE;

    /* flag not started */
    state->flags.isStarted = FALSE;

    /* not deleted */
    state->flags.isDeleted = FALSE;

    /* not waiting for activation */
    state->flags.isWaitingActivation = FALSE;

    /* not waiting to be stopped */
    state->flags.isWaitingStop = FALSE;

    /* not waiting to be suspended */
    state->flags.isWaitingSuspend = FALSE;
    
    /* reset the inter-partition nesting level */
    state->interPartitionNest = 0;
}


bool pharosIThreadIsWaiting(ThreadState state)
{
    /* check if state says its not waiting */
    return state.flags.wait != PHAROS_THREAD_STATE_NOT_WAITING;
}


bool pharosIThreadStateIsBlockedFuture(ThreadState state)
{
    return state.flags.blocked == PHAROS_THREAD_STATE_BLOCKED_FUTURE;
}


bool pharosIThreadStateIsBlockedFutureQueue(ThreadState state)
{
    return state.flags.blocked == PHAROS_THREAD_STATE_BLOCKED_FUTURE_QUEUE;
}


void pharosIThreadStateDelete(const ptrThreadState state)
{
    state->flags.isDeleted = TRUE;
}


void pharosIThreadStateBlockMutex(const ptrThreadState state)
{
    /* set to block by mutex state */
    state->flags.blocked = PHAROS_THREAD_STATE_BLOCKED_SEMAPHORE_MUTEX;
}


void pharosIThreadStateBlockCount(const ptrThreadState state)
{
    /* set to block by semaphore counting state */
    state->flags.blocked = PHAROS_THREAD_STATE_BLOCKED_SEMAPHORE_COUNTING;
}


void pharosIThreadStateBlockBulk(const ptrThreadState state)
{
    /* set to block by a bulk state */
    state->flags.blocked = PHAROS_THREAD_STATE_BLOCKED_BULK;
}


void pharosIThreadStateBlockChannelBulk(const ptrThreadState state)
{
    /* set to block by a bulk state */
    state->flags.blocked = PHAROS_THREAD_STATE_BLOCKED_CHANNEL_BULK;
}


void pharosIThreadStateBlockCeil(const ptrThreadState state)
{
    /* set to block by semaphore ceiling state */
    state->flags.blocked = PHAROS_THREAD_STATE_BLOCKED_SEMAPHORE_CEILING;
}


void pharosIThreadStateBlockFutureQueue(const ptrThreadState state)
{
    /* set to block by future queue state */
    state->flags.blocked = PHAROS_THREAD_STATE_BLOCKED_FUTURE_QUEUE;
}


void pharosIThreadStateBlockFuture(const ptrThreadState state)
{
    /* set to block by future state */
    state->flags.blocked = PHAROS_THREAD_STATE_BLOCKED_FUTURE;
}


void pharosIThreadStateBlockEvent(const ptrThreadState state)
{
    /* set to block by event state */
    state->flags.blocked = PHAROS_THREAD_STATE_BLOCKED_EVENT;
}


void pharosIThreadStateSetResume(const ptrThreadState state)
{
    /* state is now not suspended */
    state->flags.isSuspended = FALSE;

    /* state is also not waiting to be suspended */
    state->flags.isWaitingSuspend = FALSE;
}


void pharosIThreadStateSetSuspend(const ptrThreadState state)
{
    /* state is now suspended */
    state->flags.isSuspended = TRUE;
}


bool pharosIThreadStateIsSuspended(const ptrThreadState state)
{
    /* return if the thread state is suspended or not */
    return state->flags.isSuspended == TRUE ? TRUE : FALSE;
}


void pharosIThreadStateWaitActivation(const ptrThreadState state)
{
    state->flags.isWaitingActivation = TRUE;
}


void pharosIThreadStateClearWaitActivation(const ptrThreadState state)
{
    state->flags.isWaitingActivation = FALSE;
}


void pharosIThreadStateBlockLwQueue(const ptrThreadState state)
{
    state->flags.blocked = PHAROS_THREAD_STATE_BLOCKED_LIGHTWEIGHT_QUEUE;
}


void pharosIThreadStateBlockHwQueueReceive(const ptrThreadState state)
{
    state->flags.blocked = PHAROS_THREAD_STATE_BLOCKED_HW_QUEUE_RECEIVE;
}


void pharosIThreadStateBlockHwQueueSend(const ptrThreadState state)
{
    state->flags.blocked = PHAROS_THREAD_STATE_BLOCKED_HW_QUEUE_SEND;
}


void pharosIThreadStateBlockChannel(const ptrThreadState state)
{
    state->flags.blocked = PHAROS_THREAD_STATE_BLOCKED_CHANNEL;
}


void pharosIThreadStateBlockChannelResponse(const ptrThreadState state)
{
    state->flags.blocked = PHAROS_THREAD_STATE_BLOCKED_CHANNEL_RESPONSE;
}


bool pharosIThreadStateIsReady(const ptrThreadState state)
{
    /* return value */
    bool result;


    /* if the thread is not blocked 
     * and is not waiting on anything (including sleep) 
     * and is not suspended 
     * and is not deleted 
     * and has started 
     * then the thread is ready */
    if((state->flags.blocked == PHAROS_THREAD_STATE_UNBLOCKED) &&
       (state->flags.wait == PHAROS_THREAD_STATE_NOT_WAITING) &&
       (state->flags.isSuspended == FALSE) &&
       (state->flags.isDeleted == FALSE) &&
       (state->flags.isStarted == TRUE))
    {
        /* state is ready */
        result = TRUE;
    }
        /* otherwise */
    else
    {
        /* state is not ready */
        result = FALSE;
    }

    /* return state is ready or not */
    return result;
}


void pharosIThreadStateUnblock(const ptrThreadState state)
{
    state->flags.blocked = PHAROS_THREAD_STATE_UNBLOCKED;
}


void pharosIThreadStateWaitCount(const ptrThreadState state)
{
    state->flags.wait = PHAROS_THREAD_STATE_WAITING_TIMEOUT_SEMAPHORE_COUNTING;
}


void pharosIThreadStateWaitBulk(const ptrThreadState state)
{
    state->flags.wait = PHAROS_THREAD_STATE_WAITING_TIMEOUT_BULK;
}


void pharosIThreadStateWaitChannelBulk(const ptrThreadState state)
{
    state->flags.wait = PHAROS_THREAD_STATE_WAITING_TIMEOUT_CHANNEL_BULK;
}


void pharosIThreadStateWaitChannelResponse(const ptrThreadState state)
{
    state->flags.wait = PHAROS_THREAD_STATE_WAITING_TIMEOUT_CHANNEL_BULK_RESPONSE;
}


void pharosIThreadStateWaitCeil(const ptrThreadState state)
{
    state->flags.wait = PHAROS_THREAD_STATE_WAITING_TIMEOUT_SEMAPHORE_CEILING;
}


void pharosIThreadStateWaitMutex(const ptrThreadState state)
{
    state->flags.wait = PHAROS_THREAD_STATE_WAITING_TIMEOUT_SEMAPHORE_MUTEX;
}


void pharosIThreadStateWaitEvent(const ptrThreadState state)
{
    state->flags.wait = PHAROS_THREAD_STATE_WAITING_TIMEOUT_EVENT;
}


void pharosIThreadStateWaitLwQueue(const ptrThreadState state)
{
    state->flags.wait = PHAROS_THREAD_STATE_WAITING_TIMEOUT_LIGHTWEIGHT_QUEUE;
}


void pharosIThreadStateWaitHwQueue(const ptrThreadState state)
{
    state->flags.wait = PHAROS_THREAD_STATE_WAITING_TIMEOUT_HW_QUEUE_RECEIVE;
}


void pharosIThreadStateWaitChannel(const ptrThreadState state)
{
    state->flags.wait = PHAROS_THREAD_STATE_WAITING_TIMEOUT_CHANNEL;
}


void pharosIThreadStateWaitFutureQueue(const ptrThreadState state)
{
    state->flags.wait = PHAROS_THREAD_STATE_WAITING_TIMEOUT_FUTURE_QUEUE;
}


void pharosIThreadStateWaitFuture(const ptrThreadState state)
{
    state->flags.wait = PHAROS_THREAD_STATE_WAITING_TIMEOUT_FUTURE;
}


void pharosIThreadStateWaitSleep(const ptrThreadState state)
{
    state->flags.wait = PHAROS_THREAD_STATE_WAITING_SLEEP;
}


void pharosIThreadStateUnwait(const ptrThreadState state)
{
    state->flags.wait = PHAROS_THREAD_STATE_NOT_WAITING;
}


bool pharosIThreadStateIsWaitEvent(const ptrThreadState state)
{
    return state->flags.wait == PHAROS_THREAD_STATE_WAITING_TIMEOUT_EVENT ? TRUE : FALSE;
}


bool pharosIThreadStateIsWaiting(const ptrThreadState state)
{
    return state->flags.wait == PHAROS_THREAD_STATE_NOT_WAITING ? FALSE : TRUE;
}


bool pharosIThreadStateIsWaitAct(const ptrThreadState state)
{
    return state->flags.isWaitingActivation;
}


void pharosIThreadStateSetCreated(const ptrThreadState state)
{
    state->flags.isCreated = TRUE;
}


bool pharosIThreadStateIsCreated(const ptrThreadState state)
{
    return state->flags.isCreated;
}


void pharosIThreadStateSetStarted(const ptrThreadState state)
{
    state->flags.isStarted = TRUE;
    state->flags.isWaitingStop = FALSE;
}


bool pharosIThreadStateIsStarted(const ptrThreadState state)
{
    return state->flags.isStarted;
}


void pharosIThreadStateSetStopped(const ptrThreadState state)
{
    state->flags.isStarted = FALSE;
}


void pharosIThreadStateStopWaitingSet(const ptrThreadState state)
{
    state->flags.isWaitingStop = TRUE;
}


void pharosIThreadStateStopWaitingClear(const ptrThreadState state)
{
    state->flags.isWaitingStop = FALSE;
}


bool pharosIThreadStateIsStopWaiting(const ptrThreadState state)
{
    return state->flags.isWaitingStop;
}


void pharosIThreadStateSuspendWaitingSet(const ptrThreadState state)
{
    state->flags.isWaitingSuspend = TRUE;
}


bool pharosIThreadStateIsSuspendWaiting(const ptrThreadState state)
{
    return state->flags.isWaitingSuspend;
}


void pharosIThreadStateSuspendWaitingClear(const ptrThreadState state)
{
    state->flags.isWaitingSuspend = FALSE;
}


void pharosIThreadStateInterPartitionInc(ptrThreadInfo thread)
{
    /* check if running inside a thread (could be an interrupt) */
    if(thread != NULL)
    {
        /* increment the thread number inter partition calls being made nested */
        thread->state.interPartitionNest++;

        /* increment the thread partition number of threads using the inter-partition */
        pharosIUint32AtomicInc(&thread->partition->interPartitionNestLevel , 1);
    }
    /* else, we are running inside an interrupt so no need to increment since it will be decremented right afterwards */

}


void pharosIThreadStateInterPartitionDec(ptrThreadInfo thread)
{
    /* check if running inside a thread (could be an interrupt) */
    if(thread != NULL)
    {
        /* decrement the thread number inter partition calls being made nested */
        thread->state.interPartitionNest--;

        /* if the thread just left all inter-partitions */
        if(thread->state.interPartitionNest == 0U)
        {
            /* if the thread is waiting to be stopped */
            if(pharosIThreadStateIsStopWaiting(&thread->state) == TRUE)
            {
                /* clear the waiting to be stopped flag */
                pharosIThreadStateStopWaitingClear(&thread->state);

                /* set the thread state to stopped */
                pharosIThreadStateSetStopped(&thread->state);

                /* then remove the thread from the ready list (if on the list) */
                pharosISchedRemoveReadyThread(thread);

                /* stop the thread */
                pharosIThreadStop(thread);
            }

            /* if the thread is waiting to be suspended */
            if(pharosIThreadStateIsSuspendWaiting(&thread->state) == TRUE)
            {
                /* clear the waiting to be stopped flag */
                pharosIThreadStateSuspendWaitingClear(&thread->state);

                /* set the thread state to suspended */
                pharosIThreadStateSetSuspend(&thread->state);

                /* then remove the thread from the ready list (if on the list) */
                pharosISchedRemoveReadyThread(thread);

                /* suspend the thread */
                pharosIThreadSuspend(thread);
            }

            /* a dispatch may be required if the thread stopped is the running thread */
            pharosIDispatchThread();
        }

        /* decrement the thread partition number of threads using the inter-partition and
         * if the partition has no more threads accessing other partitions and is restarting */
        if((pharosIUint32AtomicDec(&thread->partition->interPartitionNestLevel , 1) == 0U) &&
           (thread->partition->state == PHAROS_PARTITION_STATE_RESTARTING))
        {
            /* do another check to see if the partition can restart */
            pharosIPartitionRestartCheck(thread->partition);
        }
    }
}


bool pharosIThreadStateIsInterPartition(ptrThreadInfo thread)
{
    /* if the nest level is 0 then thread is not inside an inter-partition call */
    return thread->state.interPartitionNest == 0U ? FALSE : TRUE;
}


void pharosIThreadStateClear(const ptrThreadState state)
{
    /* clear the blocked state */
    state->flags.blocked = PHAROS_THREAD_STATE_UNBLOCKED;

    /* reset the suspension */
    state->flags.isSuspended = 0;

    /* is not waiting for activation */
    state->flags.isWaitingActivation = 0;

    /* is not waiting */
    state->flags.wait = PHAROS_THREAD_STATE_NOT_WAITING;
}
