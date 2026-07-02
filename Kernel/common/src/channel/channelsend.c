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
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/kernel/core/coreinlines.h>
#include <pharos/kernel/partition/partitioninline.h>
#include <pharos/kernel/future/futureinline.h>
#include <pharos/buffer.h>
#include <pharos/kernel/assert/assert.h>
#include <pharos/future.h>


/**
 * Sends the bulk to the channel (in RMP and RSP)
 * 
 * @constraint assumes the arguments are all valid and have been checked previously 
 * 
 * @param channel channel where to send the bulk
 * @param bulkKernel bulk kernel to send
 * @param bulk bulk to send
 * @param priority priority of the bulk
 * @param future future (optional) to send with the bulk
 * @param runningPartition running partition
 * 
 * @return returns success or the error in sending
 */
static PharosChannelSendR pharosIChannelSendAux(ptrChannel channel , ptrChannelBulkKernel bulkKernel ,
                                                ptrChannelBulk bulk , PharosPriority priority ,
                                                ptrFuture future ,
                                                ptrPartition runningPartition);


PharosChannelSendR pharosIChannelSendOnChannelCore(ptrChannel channel , ptrChannelBulkKernel bulk)
{
    /* return value */
    PharosChannelSendR result;

    /* interrupt level */
    PharosInterruptLevel level;

    /* thread unblocked by this message (if any) */
    ptrThreadInfo unblocked;


    /* check that we are running in the correct core */
    PHAROS_ASSERT_CORE(channel->owner->core->number , "Incorrect core");

    /* check that dispatch is disabled */
    PHAROS_ASSERT(pharosIIsDispatchingEnabled() == FALSE , "Dispatch is enabled");

    /* disable interrupts so nothing can change the value of the global variables */
    pharosCpuDisableInterrupts(&level);

    /* check if the channel partition is restarting (if it is, then we can't send the bulk (the channel 
     * blockedThreads could be incoherent).
     * The sender can only make sure that the message was processed by waiting on the future and checking that it 
     * was successful */
    if(channel->owner->state == PHAROS_PARTITION_STATE_RESTARTING)
    {
        /* re-enable interrupts*/
        pharosCpuEnableInterrupts(&level);

        /* we can now re-enable the dispatch */
        pharosIDispatchEnable();

        /* free the bulk */
        pharosIChannelBulkFree(bulk);

        /* if there is a future */
        if(bulk->future != NULL)
        {
            /* reset the future so it will return "partition restarting" when the sender tries to wait for it */
            pharosIFutureReset(bulk->future);
        }

        /* return partition restart. This will only be used if the sender is on the same core */
        result = PHAROS_CHANNEL_SEND_PARTITION_RESTART;
    }
        /* if the queue owner is not restarting */
    else
    {
        /* remove the highest priority blocked thread on the queue */
        unblocked = pharosIThreadQueueGetHighest(&channel->blockedThreads);

        /* if there was a thread blocked on the queue that thread will receive the message */
        if(unblocked != NULL)
        {
            /* if waiting for with a minimum channel bulk priority */
            if((channel->flags.isWaitWithMinimumPriority == TRUE) && (bulk->priority > channel->minimumPriority))
            {
                /* place the message in the queue */
                pharosIChannelBulkPut(channel , bulk);

                /* re-enable interrupts*/
                pharosCpuEnableInterrupts(&level);

                /* we can now re-enable the dispatch */
                pharosIDispatchEnable();

                /* the thread that is waiting for the bulk is waiting for a bulk with a higher priority so we can't 
                 * awake it. We have to continue to block it until a message with the right priority comes or a 
                 * timeout occurs */
            }
            else
            {
                /* remove the thread from the map of blocked threads on this queue */
                pharosIThreadQueueRemoveBlocked(unblocked);

                /* update the bulk with the receiver information */
                pharosIFutureUpdateHolder(bulk->future , unblocked->partition);

                /* update the state of the thread to ready */
                pharosIThreadStateUnblock(&unblocked->state);

                /* remove the wait state (if was placed) */
                pharosIThreadStateUnwait(&unblocked->state);

                /* re-enable interrupts*/
                pharosCpuEnableInterrupts(&level);

                /* send the channel to the remote core or the local core -> this will re-enable thread dispatch */
                pharosIChannelReceiveReturn(unblocked , channel , bulk , PHAROS_CHANNEL_RECEIVE_SUCCESS);
            }
        }
            /* there is no thread being blocked */
        else
        {
            /* place the message in the queue */
            pharosIChannelBulkPut(channel , bulk);

            /* re-enable interrupts*/
            pharosCpuEnableInterrupts(&level);

            /* we can now re-enable the dispatch */
            pharosIDispatchEnable();
        }

        /* return success */
        result = PHAROS_CHANNEL_SEND_SUCCESS;
    }

    /* return success or the partition restart */
    return result;
}


PharosChannelSendR pharosSChannelSend(ptrChannelId id , ptrChannelBulk bulk , PharosPriority priority ,
                                      ptrFutureId future)
{
    /* kernel copy of the bulk to avoid the caller partition from changing it */
    ChannelBulk myBulk;

    /* channel where to send the message */
    ptrChannel channel;

    /* result of the function */
    PharosChannelSendR result;

    /* future pointer */
    ptrFuture f;

    /* get the running partition */
    ptrPartition runningPartition = pharosIPartitionGetRunning();

    /* partition memory protection */
    ptrEnvMemoryProtection env = pharosIPartitionRunningMemProt();


    /* check if the address of the channel id is valid */
    if(pharosCpuBufferIsInsideProtection((uint8_t *) id , sizeof (ChannelId) , env) == FALSE)
    {
        /* it is invalid, there is nothing more we can do */
        result = PHAROS_CHANNEL_SEND_INVALID_CHANNEL_ADDRESS;
    }
        /* check if the address of the bulk is valid */
    else if(pharosCpuBufferIsInsideProtection((uint8_t *) bulk , sizeof (ChannelBulk) , env) == FALSE)
    {
        /* return invalid bulk buffer address */
        result = PHAROS_CHANNEL_SEND_INVALID_BULK_ADDRESS;
    }
        /* check if the future is outside the protection */
    else if((future != NULL) && (pharosCpuBufferIsInsideProtection((uint8_t *) future , sizeof (FutureId) , env) == FALSE))
    {
        /* return invalid future */
        result = PHAROS_CHANNEL_SEND_INVALID_FUTURE_ADDRESS;
    }
    else
    {
        /* now all arguments have been made sure they are on the caller partition memory area */

        /* read the bulk (prevents any preemption to change the bulk) */
        myBulk = *bulk;

        /* if the bulk is not valid */
        if(pharosIChannelBulkKernelValid(runningPartition , myBulk.kernelBulk) == FALSE)
        {
            /* return invalid bulk */
            result = PHAROS_CHANNEL_SEND_INVALID_BULK;
        }
            /* check if the bulk message buffer is valid (inside the bulk page) */
        else if(pharosIChannelBulkBufferValid(myBulk.kernelBulk , &myBulk.bufferMessage) == FALSE)
        {
            /* return invalid bulk buffer */
            result = PHAROS_CHANNEL_SEND_INVALID_BULK_BUFFER;
        }
            /* check if the bulk extra buffer is valid (inside the bulk page) */
        else if(pharosIChannelBulkBufferValid(myBulk.kernelBulk , &myBulk.bufferExtra) == FALSE)
        {
            /* return invalid bulk buffer */
            result = PHAROS_CHANNEL_SEND_INVALID_BULK_EXTRA_BUFFER;
        }
        else
        {
            /* get the channel */
            channel = pharosIChannelGet(id);

            /* if could not find the queue */
            if(channel == NULL)
            {
                /* return invalid queue */
                result = PHAROS_CHANNEL_SEND_INVALID_CHANNEL;
            }
                /* check if the channel owner is already initialized */
            else if(pharosIPartitionIsStarted(channel->owner) == FALSE)
            {
                /* partition is not yet initialized (maybe restarted or is lazy load) */
                result = PHAROS_CHANNEL_SEND_UNINITIALIZED;
            }
            else
            {
                /* if there is a future and it is valid (if invalid then just don't send it) */
                if((future != NULL) && pharosFutureIsValid(future) == TRUE)
                {
                    /* try to get the future */
                    f = pharosIFutureGetGlobal(future);

                    /* if the future is invalid */
                    if(f == NULL)
                    {
                        /* return invalid future */
                        result = PHAROS_CHANNEL_SEND_INVALID_FUTURE_ADDRESS;
                    }
                        /* if the restart number is incorrect */
                    else if(f->index.partitionRestartNumber != f->owner->restartNumber)
                    {
                        /* return that the partition restarted, so nothing needs to be done */
                        result = PHAROS_CHANNEL_SEND_PARTITION_RESTART;
                    }
                        /* check if the running partition is the holder */
                    else if(pharosIFutureIsHolder(f , runningPartition) == FALSE)
                    {
                        /* return invalid future */
                        result = PHAROS_CHANNEL_SEND_INVALID_FUTURE_HOLDER;
                    }
                        /* if the future is ready or in the progress of making it ready */
                    else if((f->flags.isReady == TRUE) || (f->flags.makingReady == TRUE))
                    {
                        /* cannot send this future since it is already ready */
                        result = PHAROS_CHANNEL_SEND_FUTURE_IS_READY;
                    }
                        /* else, the future is valid */
                    else
                    {
                        /* send the bulk to the channel (execute the filter) */
                        result = pharosIChannelSendAux(channel , myBulk.kernelBulk , &myBulk , priority ,
                                                       f , runningPartition);
                    }
                }
                    /* if there is no future */
                else
                {
                    /* send the bulk to the channel (execute the filter) */
                    result = pharosIChannelSendAux(channel , myBulk.kernelBulk , &myBulk , priority ,
                                                   NULL , runningPartition);
                }
            }
        }
    }

    /* return the send result */
    return result;
}


PharosChannelSendR pharosIChannelSendAux(ptrChannel channel , ptrChannelBulkKernel bulkKernel , ptrChannelBulk bulk ,
                                         PharosPriority priority , ptrFuture future ,
                                         ptrPartition runningPartition)
{
    /* result of the function */
    PharosChannelSendR result;


    /* execute the filter */
    result = pharosIChannelFilterSend(channel , priority , bulk);

    /* if the filter gave go-ahead */
    if(result == (PharosChannelSendR) PHAROS_CHANNEL_SEND_FILTER_SUCCESS)
    {
        /* if not running on a partition with supervisor permissions */
        if(pharosIPartitionHasSupervisor() == FALSE)
        {
            /* remove permissions on the bulk to the running thread partition */
            pharosCpuChannelBulkRemove(bulkKernel , bulkKernel->holder);
        }

        /* copy the future */
        bulkKernel->future = future;

        /* set the priority */
        bulkKernel->priority = priority;

        /* copy the buffer data (start and size) */
        bulkKernel->bufferMessage = bulk->bufferMessage;

        /* copy the buffer extra data (start and size) */
        bulkKernel->bufferExtra = bulk->bufferExtra;

        /* set the sender partition */
        bulkKernel->sender = runningPartition;

        /* send the channel (via RMP or RSP libraries) */
        result = pharosIChannelSend(channel , bulkKernel);
    }

    /* return the result */
    return result;
}
