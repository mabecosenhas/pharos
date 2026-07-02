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


#include <pharos/kernel/core/multi.h>
#include <pharos/kernel/thread/dispatcher.h>
#include <pharos/kernel/channel/channel.h>
#include <pharos/kernel/channel/channelbulk.h>
#include <pharos/kernel/channel/channelbulkkernel.h>
#include <pharos/kernel/core/mpsc.h>
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/kernel/core/rmpqueue.h>
#include <pharos/kernel/thread/threadinline.h>
#include <pharos/kernel/assert/assert.h>
#include <pharos/kernel/scheduler/schedulerinline.h>
#include <pharos/kernel/time/timeinline.h>
#include <pharos/kernel/thread/sporadicthread.h>


PharosChannelReceiveR pharosIChannelReceive(ptrChannel channel , ptrChannelBulk bulk , ptrPharosPriority priority ,
                                            Timeout timeout , ptrFutureId future)
{
    /* return value */
    PharosChannelReceiveR result;

    /* get the running core */
    CoreNumber core = pharosICoreRunning();

    /* get the core that owns the channel */
    CoreNumber channelOwner = channel->owner->core->number;

    /* node to send the message to the other core with */
    ptrPharosMpscNode node;

    /* message to send */
    ptrPharosRmpMessageChannelReceive message;

    /* queue on the other core to send */
    ptrPharosMpscQueue queue;

    /* interrupt level */
    PharosInterruptLevel level;

    /* get the running thread (if any, we could be running inside an interrupt) */
    ptrThreadInfo runningThread = pharosISchedGetRunningThread();

    /* get the running partition */
    ptrPartition runningPartition = pharosIPartitionGetRunning();


    /* check if we are sending the message to the same core */
    if(channelOwner == core)
    {
        /* then use the same core function to receive the message */
        result = pharosIChannelReceiveOnThreadAndChannelCore(channel , bulk , priority , timeout , future);
    }
    else
    {
        /* check if an interrupt is running */
        if(pharosIInterruptIsRunning() == TRUE)
        {
            /* cannot receive bulks from a remote core inside an interrupt, even with a zero timeout */
            result = PHAROS_CHANNEL_RECEIVE_REMOTE_INSIDE_INTERRUPT;
        }
        else
        {
            /* if we are here then must not be inside an interrupt. 
             * This is a constraint of the caller */

            /* message to send to the other core */
            node = &runningThread->rmp.mpscChannelReceiveNode[channelOwner];

            /* get the MPSC queue to send to on the channel owner core */
            queue = &channel->owner->core->rmp.queue;

            /* get the correct node to use to send the message */
            node = pharosIMpscQueueGetMessage(queue , node);

            /* get the message */
            message = &runningThread->rmp.channelReceiveMessage;

            /* set the message */
            node->message.channelReceive = message;

            /* create the multi-core message */
            node->type = PHAROS_RMP_MESSAGE_TYPE_CHANNEL_RECEIVE;
            message->channel = channel;
            message->thread = runningThread;
            message->timeout = timeout;
            message->priority = priority;

            /* reset the flag */
            runningThread->threadFlags.unblockDueToTimeout = FALSE;

            /* disable interrupts so we can atomically remove the external bulks/futures and send the IPI interrupt */
            pharosCpuDisableInterrupts(&level);

            /* check if should wait for a timeout */
            if(pharosITimeoutIsFinite(timeout) == TRUE)
            {
                /* update state to waiting on queue */
                pharosIThreadStateWaitChannel(&runningThread->state);

                /* reset the flag */
                runningThread->threadFlags.unblockDueToTimeout = FALSE;

                /* add the time handler using the channel partition scheduler */
                pharosITimeTickAddThread(runningThread , timeout + pharosIClockTicksGet(runningThread->scheduler) ,
                                         PHAROS_TIME_TICK_AWAKEN_THREAD_CHANNEL_RECEIVE_TIMEOUT ,
                                         runningThread->scheduler);
            }

            /* update the core of the running thread */
            pharosIThreadUpdateCore(runningThread , channel->owner->core);

            /* make sure that every memory store is done before sending the message */
            __atomic_thread_fence(__ATOMIC_RELEASE);

            /* push the message to the MPSC queue */
            pharosIMpscQueuePush(queue , node);

            /* remove this thread from the running thread list (before the message is sent so the other core won't 
             * try to remove it again) */
            pharosISchedRemoveReadyThread(runningThread);

            /* make sure that the message has been placed in the queue before triggering the interrupt */
            __atomic_thread_fence(__ATOMIC_RELEASE);

            /* send an IPI call to the core queue owner */
            pharosCpuMulticoreTriggerInterrupt(channelOwner);

            /* re-enable interrupts */
            pharosCpuEnableInterrupts(&level);

            /* dispatch to any other thread */
            pharosIDispatchThread();

            /* restore the core of the running thread */
            pharosIThreadRestoreCore(runningThread);

            /* when we get back here that means a timeout or a channel was received */
            if(runningThread->threadFlags.unblockDueToTimeout == TRUE)
            {
                /* return timeout */
                result = PHAROS_CHANNEL_RECEIVE_TIMEOUT;
            }
            else
            {
                /* get the return value */
                result = runningThread->unblockInfo.channelReceive.returnValue;

                /* if success, get the bulk */
                if(result == PHAROS_CHANNEL_RECEIVE_SUCCESS)
                {
                    /* copy the unblock info */
                    bulk->kernelBulk = runningThread->unblockInfo.channelReceive.channelBulk;

                    /* copy the channel bulk and future */
                    result = pharosIChannelBulkCopy(future , bulk , priority , runningPartition);
                }
            }
        }
    }

    /* return value */
    return result;
}


void pharosIChannelReceiveReturn(ptrThreadInfo thread , ptrChannel channel , ptrChannelBulkKernel bulkKernel ,
                                 PharosChannelReceiveR returnValue)
{
    /* get the core that owns the thread */
    CoreNumber threadCore = thread->partition->core->number;

    /* node to send the message to the other core with */
    ptrPharosMpscNode node;

    /* get the running core */
    CoreNumber core = pharosICoreRunning();

    /* message to send */
    ptrPharosRmpMessageChannelReceiveReturn message;

    /* queue on the other core to send */
    ptrPharosMpscQueue queue;

    /* interrupt level */
    PharosInterruptLevel level;


    /* check that dispatch is disabled */
    PHAROS_ASSERT(pharosIIsDispatchingEnabled() == FALSE , "Dispatch is enabled");

    /* check if we are sending the message to the same core */
    if(threadCore == core)
    {
        /* check if there is a bulk to return 
         * (this function could be called with bulk == NULL in case a timeout or partition restart occurs)*/
        if(bulkKernel != NULL)
        {
            /* disable interrupts so nothing can change the value of the channel bulks held */
            pharosCpuDisableInterrupts(&level);

            /* at this point the bulk belongs to this channel, we must remove it */
            pharosIChannelBulkRemoveFromHolderOnPartitionCore(bulkKernel);

            /* add the bulk to the receiver partition owner */
            pharosIChannelBulkAdd2PartitionOnPartitionCore(thread->partition , bulkKernel);

            /* re-enable interrupts */
            pharosCpuEnableInterrupts(&level);
        }

        /* then use the same core function to receive the message */
        pharosIChannelReceiveReturnOnThreadCore(thread , bulkKernel , returnValue);

        /* we can now re-enable the dispatch 
         * (we must finish the pharosIChannelReceiveReturnOnThreadCore function and not allow it to stop otherwise 
         * the system may stop, see test1956) */
        pharosIDispatchEnable();
    }
    else
    {
        /* if we are here then must not be inside an interrupt. 
         * This is a constraint of the caller */

        /* message to send to the other core */
        node = &thread->rmp.mpscChannelReceiveReturnNode[threadCore];

        /* get the MPSC queue to send to on the thread owner core */
        queue = &thread->partition->core->rmp.queue;

        /* get the correct node to use to send the message */
        node = pharosIMpscQueueGetMessage(queue , node);

        /* get the message */
        message = &thread->rmp.channelReceiveReturnMessage;

        /* set the message */
        node->message.channelReceiveReturn = message;

        /* create the multi-core message */
        node->type = PHAROS_RMP_MESSAGE_TYPE_CHANNEL_RECEIVE_RETURN;

        message->bulk = bulkKernel;
        message->thread = thread;
        message->returnValue = returnValue;

        /* disable interrupts so we can atomically remove the external bulks/futures and send the IPI interrupt */
        pharosCpuDisableInterrupts(&level);

        /* check if there is a bulk to return 
         * (this function could be called with bulk == NULL in case a timeout or partition restart occurs)*/
        if(bulkKernel != NULL)
        {
            /* at this point bulk belongs to this channel, we must remove it */
            pharosIChannelBulkRemoveFromHolderOnPartitionCore(bulkKernel);
        }

        /* first thing that will be done when the receiver core get the IPI message is to add the bulk to the receiver
         * partition */

        /* make sure that every memory store is done before sending the message */
        __atomic_thread_fence(__ATOMIC_RELEASE);

        /* push the message to the MPSC queue */
        pharosIMpscQueuePush(queue , node);

        /* make sure that the message has been placed in the queue before triggering the interrupt */
        __atomic_thread_fence(__ATOMIC_RELEASE);

        /* send an IPI call to the core thread owner */
        pharosCpuMulticoreTriggerInterrupt(threadCore);

        /* we can now re-enable the dispatch */
        pharosIDispatchEnable();
    }
}


void pharosIChannelReceiveOnChannelCore(ptrThreadInfo thread , ptrChannel channel , ptrPharosPriority priority ,
                                        Timeout timeout)
{
    /* received bulk */
    ptrChannelBulkKernel bulkKernel;

    /* interrupt level */
    PharosInterruptLevel level;


    /* this method is running on the core of the channel owner (and not on the core of the thread) */
    PHAROS_ASSERT_CORE(channel->owner->core->number , "Incorrect core");

    /* disable interrupts so nothing can change the value of the global variables */
    pharosCpuDisableInterrupts(&level);

    /* add the thread extra nodes to the running core */
    pharosIThreadQueueThreadRemoteAdd(thread);

    /* if the channel partition is not initialized */
    if(pharosIPartitionIsStarted(channel->owner) == FALSE)
    {
        /* special case of sporadic threads */
        if(pharosIThreadIsSporadic(thread) == TRUE)
        {
            /* special case of sporadic threads from another core blocking on this channel. We have to add them to the 
             * blocked threads so their state is not lost */

            /* update the state of the thread */
            pharosIThreadStateBlockChannel(&thread->state);

            /* add the calling thread to the list of blocked threads on the queue */
            pharosIThreadQueueAddBlocked(&channel->blockedThreads , thread);

            /* re-enable interrupts*/
            pharosCpuEnableInterrupts(&level);
        }
        else
        {
            /* re-enable interrupts*/
            pharosCpuEnableInterrupts(&level);

            /* disable dispatch since the pharosIChannelReceiveReturn expects that */
            pharosIDispatchDisable();

            /* send message back with error */
            pharosIChannelReceiveReturn(thread , channel , NULL , PHAROS_CHANNEL_RECEIVE_UNINITIALIZED);
        }
    }
        /* check if there is any message on the channel */
    else if(pharosIChannelIsEmpty(channel , priority) == FALSE)
    {
        /* get the highest priority message on the queue */
        bulkKernel = pharosIChannelGetHighestPriorityBulk(channel , priority);

        /* re-enable interrupts*/
        pharosCpuEnableInterrupts(&level);

        /* disable dispatch since the pharosIChannelReceiveReturn expects that */
        pharosIDispatchDisable();

        /* send the message to the thread core with the received bulk/priority */
        pharosIChannelReceiveReturn(thread , channel , bulkKernel , PHAROS_CHANNEL_RECEIVE_SUCCESS);
    }
        /* there is no message available */
    else if(timeout == PHAROS_TIMEOUT_ZERO)
    {
        /* re-enable interrupts*/
        pharosCpuEnableInterrupts(&level);

        /* disable dispatch since the pharosIChannelReceiveReturn expects that */
        pharosIDispatchDisable();

        /* received nothing, return empty */
        pharosIChannelReceiveReturn(thread , channel , NULL , PHAROS_CHANNEL_RECEIVE_EMPTY);
    }
        /* check if the channel has a thread waiting with a minimum priority and there are other threads waiting */
    else if((channel->flags.isWaitWithMinimumPriority == TRUE) &&
            (pharosIThreadQueueIsEmpty(&channel->blockedThreads) == FALSE))
    {
        /* disable dispatch since the pharosIChannelReceiveReturn expects that */
        pharosIDispatchDisable();

        /* return error */
        pharosIChannelReceiveReturn(thread , channel , NULL ,
                                    PHAROS_CHANNEL_RECEIVE_QUEUE_THREAD_WAITING_WITH_MIN_PRIORITY);
    }
    else
    {
        /* there are no bulks on the channel and we have to wait */

        /* if the thread waits with a minimum priority */
        if((channel->flags.isWaitWithMinimumPriority == TRUE))
        {
            /* if there is a minimum priority */
            if(priority != NULL)
            {
                /* set the defined priority */
                channel->minimumPriority = *priority;
            }
            else
            {
                /* set to receive all priorities */
                channel->minimumPriority = PHAROS_CHANNEL_PRIORITY_MINIMUM_ALL;
            }
        }

        /* update the state of the thread */
        pharosIThreadStateBlockChannel(&thread->state);

        /* add the calling thread to the list of blocked threads on the queue */
        pharosIThreadQueueAddBlocked(&channel->blockedThreads , thread);

        /* re-enable interrupts*/
        pharosCpuEnableInterrupts(&level);
    }
}


void pharosIChannelReceiveReturnOnReceiverCore(ptrThreadInfo unblocked , ptrChannelBulkKernel bulkKernel ,
                                               PharosChannelReceiveR returnValue)
{
    /* interrupt level */
    PharosInterruptLevel level;

    ptrPartition receiverPartition = unblocked->partition;


    /* this method runs in the core of the unblocked thread */
    PHAROS_ASSERT_CORE(unblocked->partition->core->number , "Incorrect core");

    /* disable interrupts so nothing can change the value of the global variables */
    pharosCpuDisableInterrupts(&level);

    /* check if the unblocked thread is started partition is restarting */
    if(pharosIThreadIsStarted(unblocked) == FALSE)
    {
        /* re-enable interrupts */
        pharosCpuEnableInterrupts(&level);

        /* check if there is a bulk */
        if(bulkKernel != NULL)
        {
            /* set the holder so that it can be free */
            bulkKernel->holder = unblocked->partition;

            /* then we cannot give the bulk to the thread and must return it */
            pharosIChannelBulkFree(bulkKernel);
        }
        /* else, a timeout occurred, or the channel partition was restarted. 
         * In which case we don't need to do anything */
    }
    else
    {
        /* check if there is a bulk */
        if(bulkKernel != NULL)
        {
            /* update the activation of the sporadic thread (if the thread is sporadic and currently waiting to be 
             * activated) */
            pharosIThreadSporadicActivate(unblocked);

            /* add the bulk to the receiver partition owner */
            pharosIChannelBulkAdd2PartitionOnPartitionCore(receiverPartition , bulkKernel);

            /* copy the message pointer */
            unblocked->unblockInfo.channelReceive.channelBulk = bulkKernel;
        }

        /* set the return value */
        unblocked->unblockInfo.channelReceive.returnValue = returnValue;

        /* restore the scheduler */
        pharosIThreadRestoreCore(unblocked);

        /* remove any time tick (if needed) */
        pharosITimeTickRemoveThread(unblocked);

        /* update the state of the thread to ready */
        pharosIThreadStateUnblock(&unblocked->state);

        /* remove the wait state (if was placed) */
        pharosIThreadStateUnwait(&unblocked->state);

        /* re-enable interrupts */
        pharosCpuEnableInterrupts(&level);

        /* add the thread to ready thread list */
        pharosISchedAddReadyThread(unblocked);

        /* maybe necessary to dispatch to new thread */
        pharosIDispatchThread();
    }
}
