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
#include <pharos/kernel/partition/partition.h>
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/kernel/assert/assert.h>
#include <pharos/kernel/channel/channelbulkkernel.h>


void pharosIChannelReceiveReturn(ptrThreadInfo thread , ptrChannel channel , ptrChannelBulkKernel bulkKernel ,
                                 PharosChannelReceiveR returnValue)
{
    /* interrupt level */
    PharosInterruptLevel level;


    /* check that the channel is valid */
    PHAROS_ASSERT(channel != NULL , "Channel is NULL");

    /* check that the thread is valid */
    PHAROS_ASSERT(thread != NULL , "Thread is NULL");

    /* check that the bulk is valid */
    PHAROS_ASSERT(bulkKernel != NULL , "Bulk is NULL");

    /* check that dispatch is disabled */
    PHAROS_ASSERT(pharosIIsDispatchingEnabled() == FALSE , "Dispatch is enabled");

    /* disable interrupts so nothing can change the value of the channel bulks held */
    pharosCpuDisableInterrupts(&level);

    /* at this point the bulk belongs to this channel, we must remove it */
    pharosIChannelBulkRemoveFromHolderOnPartitionCore(bulkKernel);

    /* add the bulk to the receiver partition owner */
    pharosIChannelBulkAdd2PartitionOnPartitionCore(thread->partition , bulkKernel);

    /* re-enable interrupts */
    pharosCpuEnableInterrupts(&level);

    /* use the same core function to receive the message */
    pharosIChannelReceiveReturnOnThreadCore(thread , bulkKernel , returnValue);

    /* we can now re-enable the dispatch */
    pharosIDispatchEnable();
}


PharosChannelReceiveR pharosIChannelReceive(ptrChannel channel , ptrChannelBulk bulk ,
                                            ptrPharosPriority priority , Timeout timeout ,
                                            ptrFutureId future)
{
    /* just call the function on the local core */
    return pharosIChannelReceiveOnThreadAndChannelCore(channel , bulk , priority , timeout , future);
}


PharosChannelSendR pharosIChannelSend(ptrChannel channel , ptrChannelBulkKernel bulkKernel)
{
    /* interrupt level */
    PharosInterruptLevel level;


    /* check that the channel is valid */
    PHAROS_ASSERT(channel != NULL , "Channel is NULL");

    /* check that the channel owner is valid */
    PHAROS_ASSERT(channel->owner != NULL , "Channel is NULL");

    /* check that the bulk is valid */
    PHAROS_ASSERT(bulkKernel != NULL , "Bulk is NULL");

    /* we have to allow this thread to remove the bulk holder, add the holder and give to the thread without interrupt 
     * it, otherwise, if a partition restart is place to this thread, the bulk could be lost */
    pharosIDispatchDisable();

    /* disable interrupts so nothing can change the value of the channel bulks held */
    pharosCpuDisableInterrupts(&level);

    /* remove any external object (future or bulk) from the running partition */
    pharosIChannelBulkRemoveFromHolderOnPartitionCore(bulkKernel);

    /* temporarily add the bulk to the channel owner */
    pharosIChannelBulkAdd2PartitionOnPartitionCore(channel->owner , bulkKernel);

    /* re-enable interrupts */
    pharosCpuEnableInterrupts(&level);

    /* send the message to the local core */
    return pharosIChannelSendOnChannelCore(channel , bulkKernel);
}


void pharosIChannelBulkFree(ptrChannelBulkKernel bulk)
{
    /* interrupt level */
    PharosInterruptLevel level;


    /* check if there is a bulk to free */
    if(bulk != NULL)
    {
        /* we have to force the remove of the bulk and the free to be atomic */
        pharosIDispatchDisable();

        /* disable interrupts so nothing can change the value of the channel bulks held */
        pharosCpuDisableInterrupts(&level);

        /* remove any external object (future or bulk) from the running partition */
        pharosIChannelBulkRemoveFromHolderOnPartitionCore(bulk);

        /* re-enable interrupts */
        pharosCpuEnableInterrupts(&level);

        /* free on the local core */
        pharosIChannelBulkFreeOnQueueCore(bulk);

        /* we can now re-enable the dispatch */
        pharosIDispatchEnable();
    }
}
