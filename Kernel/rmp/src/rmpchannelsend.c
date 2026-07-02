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
#include <pharos/kernel/assert/assert.h>


PharosChannelSendR pharosIChannelSend(ptrChannel channel , ptrChannelBulkKernel bulkKernel)
{
    /* return value */
    PharosChannelSendR result;

    /* get the core that owns the channel */
    CoreNumber channelOwner = channel->owner->core->number;

    /* node to send the message to the other core with */
    ptrPharosMpscNode node;

    /* message to send */
    ptrPharosRmpMessageChannelSend message;

    /* queue on the other core to send */
    ptrPharosMpscQueue queue;

    /* interrupt level */
    PharosInterruptLevel level;


    /* check that the bulk is valid */
    PHAROS_ASSERT(bulkKernel != NULL , "Bulk is NULL");

    /* check if we are sending the message to the same core */
    if(channelOwner == pharosICoreRunning())
    {
        /* we have to force the remove of the bulk and the free to be atomic (this thread cannot be stopped)  */
        pharosIDispatchDisable();

        /* disable interrupts so nothing can change the value of the channel bulks held */
        pharosCpuDisableInterrupts(&level);

        /* remove any external object (future or bulk) from the running partition */
        pharosIChannelBulkRemoveFromHolderOnPartitionCore(bulkKernel);

        /* temporarily add the bulk to the channel owner */
        pharosIChannelBulkAdd2PartitionOnPartitionCore(channel->owner , bulkKernel);

        /* re-enable interrupts */
        pharosCpuEnableInterrupts(&level);

        /* then use the same core function to send the message */
        result = pharosIChannelSendOnChannelCore(channel , bulkKernel);
    }
    else
    {
        /* message to send to the other core */
        node = &bulkKernel->rmp.mpscSendNode[channelOwner];

        /* get the MPSC queue to send to on the channel owner core */
        queue = &pharosICoreGet(channelOwner)->rmp.queue;

        /* get the correct node to use to send the message */
        node = pharosIMpscQueueGetMessage(queue , node);

        /* get the message */
        message = &bulkKernel->rmp.sendMessage;

        /* set the message */
        node->message.channelSend = message;

        /* create the multi-core message */
        node->type = PHAROS_RMP_MESSAGE_TYPE_CHANNEL_SEND;
        message->channel = channel;
        message->bulk = bulkKernel;

        /* disable interrupts so we can atomically remove the external bulks/futures and send the IPI interrupt */
        pharosCpuDisableInterrupts(&level);

        /* remove the bulk and future from the running partition external lists */
        pharosIChannelBulkRemoveFromHolderOnPartitionCore(bulkKernel);

        /* cannot add the bulk the channel owner since we are not running on its core, 
         * but it will be the first thing that it does */

        /* make sure that every memory store is done before sending the message */
        __atomic_thread_fence(__ATOMIC_RELEASE);

        /* push the message to the MPSC queue */
        pharosIMpscQueuePush(queue , node);

        /* make sure that the message has been placed in the queue before triggering the interrupt */
        __atomic_thread_fence(__ATOMIC_RELEASE);

        /* send an IPI call to the core queue owner */
        pharosCpuMulticoreTriggerInterrupt(channelOwner);

        /* re-enable interrupts */
        pharosCpuEnableInterrupts(&level);

        /* return success */
        result = PHAROS_CHANNEL_SEND_SUCCESS;
    }

    /* return value */
    return result;
}


void pharosIChannelSendRmp(ptrChannel channel , ptrChannelBulkKernel bulk)
{
    /* interrupt level */
    PharosInterruptLevel level;


    /* make sure to disable dispatch */
    pharosIDispatchDisable();

    /* disable interrupts */
    pharosCpuDisableInterrupts(&level);

    /* temporarily add the bulk to the channel owner */
    pharosIChannelBulkAdd2PartitionOnPartitionCore(channel->owner , bulk);

    /* re-enable interrupts */
    pharosCpuEnableInterrupts(&level);

    /* process the channel bulk on the core of the channel owner */
    pharosIChannelSendOnChannelCore(channel , bulk);
}
