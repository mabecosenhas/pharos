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
#include <pharos/kernel/core/mpsc.h>
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/kernel/channel/channelbulkkernel.h>


void pharosIChannelBulkFree(ptrChannelBulkKernel bulk)
{
    /* interrupt level */
    PharosInterruptLevel level;

    /* node to send the message to the other core with */
    ptrPharosMpscNode node;

    /* message to send */
    ptrPharosRmpMessageChannelBulkFree message;

    /* queue on the other core to send */
    ptrPharosMpscQueue queue;

    /* get the running core */
    CoreNumber core = pharosICoreRunning();

    /* core that owns the bulk */
    CoreNumber bulkOwner;


    /* check if there is a bulk to free */
    if(bulk != NULL)
    {
        /* get the core that owns the bulk */
        bulkOwner = bulk->queue->owner->core->number;

        /* we have to force the remove of the bulk and the free to be atomic (this thread cannot be stopped)  */
        pharosIDispatchDisable();

        /* disable interrupts so nothing can change the value of the channel bulks held */
        pharosCpuDisableInterrupts(&level);

        /* remove any external object (future or bulk) from the running partition */
        pharosIChannelBulkRemoveFromHolderOnPartitionCore(bulk);

        /* re-enable interrupts */
        pharosCpuEnableInterrupts(&level);

        /* check if we are sending the message to the same core */
        if(bulkOwner == core)
        {
            /* then use the same core function to free the bulk */
            pharosIChannelBulkFreeOnQueueCore(bulk);
        }
        else
        {
            /* message to send to the other core */
            node = &bulk->rmp.mpscFreeNode[bulkOwner];

            /* get the MPSC queue to send to on the channel owner core */
            queue = &pharosICoreGet(bulkOwner)->rmp.queue;

            /* get the correct node to use to send the message */
            node = pharosIMpscQueueGetMessage(queue , node);

            /* get the message */
            message = &bulk->rmp.freeMessage;

            /* set the message */
            node->message.channelFree = message;

            /* create the multi-core message */
            node->type = PHAROS_RMP_MESSAGE_TYPE_CHANNEL_BULK_FREE;
            message->bulk = bulk;

            /* make sure that every memory store is done before sending the message */
            __atomic_thread_fence(__ATOMIC_RELEASE);

            /* push the message to the MPSC queue */
            pharosIMpscQueuePush(queue , node);

            /* make sure that the message has been placed in the queue before triggering the interrupt */
            __atomic_thread_fence(__ATOMIC_RELEASE);

            /* send an IPI call to the core queue owner */
            pharosCpuMulticoreTriggerInterrupt(bulkOwner);
        }

        /* we can now re-enable the dispatch */
        pharosIDispatchEnable();
    }
}
