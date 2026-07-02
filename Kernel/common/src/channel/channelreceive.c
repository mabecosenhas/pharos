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


#include <pharos/kernel/channel/channel.h>
#include <pharos/kernel/channel/channelbulkkernel.h>
#include <pharos/channel.h>
#include <pharos/kernel/scheduler/scheduler.h>
#include <pharos/kernel/thread/dispatcher.h>
#include <pharos/kernel/thread/sporadicthread.h>
#include <pharos/kernel/object/object.h>
#include <pharos/kernel/channel/channelbulk.h>
#include <pharos/kernel/object/protection.h>
#include <pharos/kernel/interrupt/interrupt.h>
#include <pharos/hal/boardApi.h>
#include <pharos/kernel/partition/partition.h>
#include <pharos/kernel/future/declarations.h>
#include <pharos/kernel/future/futureinline.h>
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/kernel/core/coreinlines.h>
#include <pharos/kernel/partition/partitioninline.h>
#include <pharos/kernel/thread/threadinline.h>
#include <pharos/buffer.h>
#include <pharos/kernel/assert/assert.h>
#include <pharos/kernel/scheduler/schedulerinline.h>


/**
 * Receive a channel bulk
 * 
 * @param channel channel to receive from
 * @param bulk address where to place the received bulk
 * @param priority address where to place the priority
 * @param future address where to place the future id
 * @param timeout timeout of the receive
 * 
 * @return returns success or the error
 */
static PharosChannelReceiveR receive(ptrChannel channel , ptrChannelBulk bulk ,
                                     ptrPharosPriority priority , ptrFutureId future , ptrPartitionNumber sender ,
                                     Timeout timeout);


/**
 * Revert the copy of the bulk kernel 
 * Frees the channel bulk and readies the future with NULL
 * 
 * @param bulkKernel bulk kernel to revert
 */
static void pharosIChannelBulkCopyRevert(ptrChannelBulkKernel bulkKernel);


/**
 * Copy the internal buffers of the channel bulk from its "full buffer"
 * 
 * @param bulk channel bulk to initialize the buffers
 */
static void pharosIChannelBulkCopyBuffers(ptrChannelBulk bulk);


void pharosIChannelBulkCopyRevert(ptrChannelBulkKernel bulkKernel)
{
    /* return the bulk since it cannot be used */
    pharosIChannelBulkFree(bulkKernel);

    /* if there is a future */
    if(bulkKernel->future != NULL)
    {
        /* announce to the future that the bulk was not sent due to an error */
        pharosIFutureReset(bulkKernel->future);
    }
}


PharosChannelReceiveR pharosIChannelBulkCopy(ptrFutureId future , ptrChannelBulk bulk , ptrPharosPriority priority ,
                                             ptrPartition partition)
{
    /* return value of this method */
    PharosChannelReceiveR result = PHAROS_CHANNEL_RECEIVE_SUCCESS;

    /* queue where the bulk is */
    ptrChannelBulkQueue queue;

    /* interrupt level */
    PharosInterruptLevel level;


    /* get the queue */
    queue = bulk->kernelBulk->queue;

    /* disable interrupts so we can call pharosCpuChannelBulkAdd safely */
    pharosCpuDisableInterrupts(&level);

    /* if running partition is executing is normal mode  */
    if(pharosIPartitionIsSupervisor(partition) == FALSE)
    {
        /* then add the channel bulk, checking if it was possible to do so */
        if(pharosCpuChannelBulkAdd(queue , bulk->kernelBulk , partition) == 0U)
        {
            /* re-enable interrupts*/
            pharosCpuEnableInterrupts(&level);

            /* revert the copy (free the bulk and ready the future) */
            pharosIChannelBulkCopyRevert(bulk->kernelBulk);

            /* return invalid MMU tables */
            result = PHAROS_CHANNEL_RECEIVE_INVALID_MMU;
        }
    }

    /* if we are in supervisor mode or normal mode and just added the channel bulk to the partition MMU tables */
    if(result == PHAROS_CHANNEL_RECEIVE_SUCCESS)
    {
        /* re-enable interrupts*/
        pharosCpuEnableInterrupts(&level);

        /* set the new holder of the bulk */
        bulk->kernelBulk->holder = partition;

        /* if there is a future number (that is, the receiver wants to know the future if any) */
        if(future != NULL)
        {
            /* if the sender did not send a future */
            if(bulk->kernelBulk->future == NULL)
            {
                /* then place invalid number on the receiver future id */
                future->number = PHAROS_FUTURE_NUMBER_INVALID;

                /* and invalid partition */
                future->partition = PHAROS_PARTITION_INVALID;
            }
                /* if the sender sent a future and the receiver wants to know it */
            else
            {
                /* then copy it */
                *future = bulk->kernelBulk->future->index;
            }
        }

        /* initialize the bulk */
        pharosIChannelBulkCopyBuffers(bulk);

        /* reset the future of the bulk: no longer needed and may cause confusion 
         * because another future could be made ready with this bulk */
        bulk->kernelBulk->future = NULL;

        /* copy the priority */
        *priority = bulk->kernelBulk->priority;

        /* return success */
        result = PHAROS_CHANNEL_RECEIVE_SUCCESS;
    }

    /* return success or invalid data buffer or invalid MMU tables */
    return result;
}


void pharosIChannelBulkCopyBuffers(ptrChannelBulk bulk)
{
    /* initialize the buffer to the start of the bulk */
    bulk->bufferFull = bulk->kernelBulk->bulkFullBuffer;

    /* fill the message buffer to occupy the full bulk buffer */
    bulk->bufferMessage = bulk->kernelBulk->bufferMessage;

    /* copy the extra buffer */
    bulk->bufferExtra = bulk->kernelBulk->bufferExtra;
}


bool pharosIChannelIsEmpty(ptrChannel channel , ptrPharosPriority priority)
{
    /* return value of this method */
    bool result;


    /* if waiting with a minimum priority and the caller specified a minimum priority */
    if((channel->flags.isWaitWithMinimumPriority == TRUE) && (priority != NULL))
    {
        /* check if the tree map contains a lower (in value) priority */
        if(treeMapGetLowerOrEqual(&channel->messages , priority) != NULL)
        {
            /* it contains so it is not empty to the specified priority level */
            result = FALSE;
        }
        else
        {
            result = TRUE;
        }
    }
        /* if no priority is defined */
    else
    {
        /* just check if the tree is empty */
        result = treeMapIsEmpty(&channel->messages);
    }

    return result;
}


PharosChannelReceiveR pharosIChannelReceiveOnThreadAndChannelCore(ptrChannel channel , ptrChannelBulk bulk ,
                                                                  ptrPharosPriority priority , Timeout timeout ,
                                                                  ptrFutureId future)
{
    /* interrupt level */
    PharosInterruptLevel level;

    /* return value of the method */
    PharosChannelReceiveR result;

    /* calling thread */
    ptrThreadInfo thread;

    /* get the running partition */
    ptrPartition partition = pharosIPartitionGetRunning();


    /* check that we are running in the correct core */
    PHAROS_ASSERT_CORE(channel->owner->core->number , "Incorrect core");

    /* function runs on the core of the channel */
    /* get the calling thread */
    thread = pharosISchedGetRunningThread();

    /* disable interrupts so nothing can change the value of the global variables */
    pharosCpuDisableInterrupts(&level);

    /* check if there is any message on any list through the priority bit map */
    if(pharosIChannelIsEmpty(channel , priority) == FALSE)
    {
        /* get the highest priority message on the queue (and update the priority map) */
        bulk->kernelBulk = pharosIChannelGetHighestPriorityBulk(channel , priority);

        /* re-enable interrupts*/
        pharosCpuEnableInterrupts(&level);

        /* update the activation of the sporadic thread */
        pharosIThreadSporadicActivate(thread);

        /* update the bulk future with the receiver information */
        pharosIFutureUpdateHolder(bulk->kernelBulk->future , partition);

        /* copy the future */
        result = pharosIChannelBulkCopy(future , bulk , priority , partition);
    }
        /* there is no message available */
    else if(timeout == PHAROS_TIMEOUT_ZERO)
    {
        /* re-enable interrupts*/
        pharosCpuEnableInterrupts(&level);

        /* no timeout selected, return unavailable */
        result = PHAROS_CHANNEL_RECEIVE_EMPTY;
    }
        /* check if the channel has a thread waiting with a minimum priority and there are threads waiting */
    else if((channel->flags.isWaitWithMinimumPriority == TRUE) &&
            (pharosIThreadQueueIsEmpty(&channel->blockedThreads) == FALSE))
    {
        /* re-enable interrupts*/
        pharosCpuEnableInterrupts(&level);

        /* return error */
        result = PHAROS_CHANNEL_RECEIVE_QUEUE_THREAD_WAITING_WITH_MIN_PRIORITY;
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

        /* if wait without timeout */
        if(timeout == PHAROS_TIMEOUT_INFINITE)
        {
            /* update the state of the thread */
            pharosIThreadStateBlockChannel(&thread->state);

            /* add the calling thread to the list of blocked threads on the queue */
            pharosIThreadQueueAddBlocked(&channel->blockedThreads , thread);

            /* remove the thread from the list of ready threads */
            pharosISchedRemoveReadyThread(thread);

            /* re-enable interrupts*/
            pharosCpuEnableInterrupts(&level);

            /* dispatch to highest priority ready thread */
            pharosIDispatchThread();

            /* if we got here then a message was sent and we got it */

            /* copy the message pointer */
            bulk->kernelBulk = thread->unblockInfo.channelReceive.channelBulk;

            /* check that the bulk received is valid */
            PHAROS_ASSERT(bulk->kernelBulk != NULL , "Bulk is not valid");

            /* copy the result of the sending */
            result = thread->unblockInfo.channelReceive.returnValue;

            if(result == PHAROS_CHANNEL_RECEIVE_SUCCESS)
            {
                /* update the bulk with the receiver information */
                pharosIFutureUpdateHolder(bulk->kernelBulk->future , partition);

                /* copy the future */
                result = pharosIChannelBulkCopy(future , bulk , priority , partition);
            }
        }
            /* else is wait with timeout */
        else
        {

            /* update the state of the thread to blocked */
            pharosIThreadStateBlockChannel(&thread->state);

            /* update state to waiting on queue */
            pharosIThreadStateWaitChannel(&thread->state);

            /* add the calling thread to the list of blocked threads on the queue */
            pharosIThreadQueueAddBlocked(&channel->blockedThreads , thread);

            /* remove the thread from the list of ready threads */
            pharosISchedRemoveReadyThread(thread);

            /* reset the flag */
            thread->threadFlags.unblockDueToTimeout = FALSE;

            /* add the time handler */
            pharosITimeTickAddThread(thread , timeout + pharosIClockTicksGet(thread->scheduler) ,
                                     PHAROS_TIME_TICK_AWAKEN_THREAD_CHANNEL_BULK_RECEIVE_TIMEOUT ,
                                     thread->scheduler);

            /* re-enable interrupts*/
            pharosCpuEnableInterrupts(&level);

            /* dispatch to highest priority ready thread */
            pharosIDispatchThread();

            /* disable interrupts so nothing can change the value of the global variables */
            pharosCpuDisableInterrupts(&level);

            /* remove the thread from the list of blocked threads whether a timeout occurred or not */
            pharosIThreadQueueRemoveBlocked(thread);

            /* get the result of the timeout (message was received or timeout occurred?) */
            if(thread->threadFlags.unblockDueToTimeout == TRUE)
            {
                /* re-enable interrupts*/
                pharosCpuEnableInterrupts(&level);

                /* there was a timeout */
                result = PHAROS_CHANNEL_RECEIVE_TIMEOUT;
            }
            else
            {
                /* re-enable interrupts*/
                pharosCpuEnableInterrupts(&level);

                /* copy the message pointer */
                bulk->kernelBulk = thread->unblockInfo.channelReceive.channelBulk;

                result = thread->unblockInfo.channelReceive.returnValue;

                /* copy the result of the sending */
                result = thread->unblockInfo.channelReceive.returnValue;

                if(result == PHAROS_CHANNEL_RECEIVE_SUCCESS)
                {
                    /* update the bulk with the receiver information */
                    pharosIFutureUpdateHolder(bulk->kernelBulk->future , partition);

                    /* copy the future */
                    result = pharosIChannelBulkCopy(future , bulk , priority , partition);
                }
            }
        }
    }

    /* return success or the error */
    return result;
}


PharosChannelReceiveR receive(ptrChannel channel , ptrChannelBulk bulk ,
                              ptrPharosPriority priority , ptrFutureId future , ptrPartitionNumber sender ,
                              Timeout timeout)
{
    /* return value of the method */
    PharosChannelReceiveR result;

    /* running partition environment */
    ptrEnvMemoryProtection env = pharosIPartitionRunningMemProt();


    /* if queue is invalid */
    if(channel == NULL)
    {
        /* queue is invalid */
        result = PHAROS_CHANNEL_RECEIVE_INVALID_CHANNEL;
    }
        /* check if we are inside an interrupt with a timeout */
    else if((pharosIInterruptIsRunning() == TRUE) && (timeout != PHAROS_TIMEOUT_ZERO))
    {
        /* called blocking call inside interrupt */
        result = PHAROS_CHANNEL_RECEIVE_INSIDE_INTERRUPT;
    }
        /* if the bulk is not inside the calling thread partition */
    else if(pharosCpuBufferIsInsideProtection((uint8_t *) bulk , sizeof (ChannelBulk) , env) == FALSE)
    {
        /* return invalid bulk */
        result = PHAROS_CHANNEL_RECEIVE_INVALID_BULK_ADDRESS;
    }
        /* if the sender is not inside the calling thread partition */
    else if((sender != NULL) &&
            (pharosCpuBufferIsInsideProtection((uint8_t *) sender , sizeof (PartitionNumber) , env) == FALSE))
    {
        /* return invalid sender */
        result = PHAROS_CHANNEL_RECEIVE_INVALID_SENDER_ADDRESS;
    }
        /* check if the future is outside the protection */
    else if((future != NULL) &&
            (pharosCpuBufferIsInsideProtection((uint8_t *) future , sizeof (FutureId) , env) == FALSE))
    {
        /* return invalid future */
        result = PHAROS_CHANNEL_RECEIVE_INVALID_FUTURE_ADDRESS;
    }
        /* check if the priority is at a valid address */
    else if((priority != NULL) &&
            (pharosCpuBufferIsInsideProtection((uint8_t *) priority , sizeof (PharosPriority) , env) == FALSE))
    {
        /* return invalid priority address */
        result = PHAROS_CHANNEL_RECEIVE_INVALID_PRIORITY_ADDRESS;
    }
    else
    {
        /* now we are sure that the arguments are in the partition memory area */

        /* execute the filter */
        result = pharosIChannelFilterReceive(channel);

        /* if allowed to receive the message */
        if(result == PHAROS_CHANNEL_RECEIVE_SUCCESS)
        {
            /* receive the message */
            result = pharosIChannelReceive(channel , bulk , priority , timeout , future);

            /* if the caller specified a sender */
            if(sender != NULL)
            {
                /* then write the sender */
                *sender = bulk->kernelBulk->sender->id;
            }
        }
    }

    /* return the result (receive success or an error occurred) */
    return result;
}


PharosChannelReceiveR pharosSChannelReceive(ptrChannelId id , ptrChannelBulk bulk , ptrPharosPriority priority ,
                                            ptrFutureId future , ptrPartitionNumber sender ,
                                            Timeout timeout)
{
    /* return value of the method */
    PharosChannelReceiveR result;


    /* check if the address of the channel id is valid */
    if(pharosCpuBufferIsInsideProtection((uint8_t *) id , sizeof (ChannelId) , pharosIPartitionRunningMemProt()) == FALSE)
    {
        /* it is invalid, there is nothing more we can do */
        result = PHAROS_CHANNEL_RECEIVE_INVALID_CHANNEL_ADDRESS;
    }
    else
    {
        /* get the queue and receive */
        result = receive(pharosIChannelGet(id) , bulk , priority , future , sender , timeout);
    }

    /* return the method result */
    return result;
}


PharosChannelReceiveLocalR pharosSChannelReceiveLocal(ChannelNumber id , ptrChannelBulk bulk ,
                                                      ptrPharosPriority priority ,
                                                      ptrFutureId future , ptrPartitionNumber sender , Timeout timeout)
{
    /* get the channel on the running partition and receive the bulk */
    return receive(pharosIChannelGetLocal(id) , bulk , priority , future , sender , timeout);
}


void pharosIChannelReceiveReturnOnThreadCore(ptrThreadInfo unblocked , ptrChannelBulkKernel bulkKernel ,
                                             PharosChannelReceiveR returnValue)
{
    /* interrupt level */
    PharosInterruptLevel level;

    /* this method is ONLY invoked in case the channel is on the same core as the unblocked thread */

    /* this method must run in the core of the unblocked thread */

    /* check that we are running in the correct core */
    PHAROS_ASSERT_CORE(unblocked->partition->core->number , "Incorrect core");

    /* disable interrupts so nothing can change the value of the global variables */
    pharosCpuDisableInterrupts(&level);

    /* if the thread has started */
    if(pharosIThreadIsStarted(unblocked) == FALSE)
    {
        /* thread/partition may be restarting */

        /* then free the bulk */
        pharosIChannelBulkFree(bulkKernel);

        /* re-enable interrupts */
        pharosCpuEnableInterrupts(&level);
    }
    else
    {
        /* update the activation of the sporadic thread (if the thread is sporadic and currently waiting to be 
         * activated) */
        pharosIThreadSporadicActivate(unblocked);

        /* update the thread queue receive result */
        unblocked->threadFlags.unblockDueToTimeout = FALSE;

        /* remove the thread timeout time tick (if needed). */
        pharosITimeTickRemoveThread(unblocked);

        /* copy the message pointer */
        unblocked->unblockInfo.channelReceive.channelBulk = bulkKernel;

        /* set the return value */
        unblocked->unblockInfo.channelReceive.returnValue = returnValue;

        /* update the state of the thread to ready */
        pharosIThreadStateUnblock(&unblocked->state);

        /* remove the wait state (if was placed) */
        pharosIThreadStateUnwait(&unblocked->state);

        /* add the thread to ready thread list */
        pharosISchedAddReadyThread(unblocked);

        /* re-enable interrupts */
        pharosCpuEnableInterrupts(&level);

        /* maybe necessary to dispatch to new thread */
        pharosIDispatchThread();
    }
}
