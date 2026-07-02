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
#include <pharos/kernel/partition/partitioninline.h>


PharosChannelFilterReceiveR pharosIChannelFilterReceiveExecute(ptrThreadInfo runningThread , ptrChannel channel ,
                                                               PartitionNumber partitionId , ptrStack sharedStack ,
                                                               ptrPartition original , bool *wasInside ,
                                                               ptrPharosCpuInterPartitionContext originalInterPartitionContext)
{
    /* this function was removed from inside the pharosIChannelFilterReceive function to avoid a similar problem to the
     * function pharosIChannelFilterSendExecute, in which GCC (8.2.0) optimizations for the versatilepb-release target 
     * would mess the order around macro pharosIThreadInterPartitionSwitch which is very delicate to work with */

    /* return value */
    PharosChannelFilterReceiveR result;

    /* MMU context of the channel owner */
    ptrPartitionCpuMmuContext context = pharosIPartitionMemoryContext(channel->owner);


    /* change the stack to the queue owner memory space */
    pharosCpuAddSharedStack(runningThread , context);

    /* if the switch went OK execute the filter */
    result = pharosCpuChannelReceiveExecuteFilter(channel->owner->data , partitionId , channel->id ,
                                                  channel->receiveFilter ,
                                                  pharosIPartitionMemoryContext(channel->owner) ,
                                                  pharosCpuInitializeStackPointer(sharedStack->stack ,
                                                                                  sharedStack->size) , channel->owner);
    
    /* change the stack back to the kernel space */
    pharosCpuRemoveSharedStack(runningThread , context);

    /* restore the thread running partition */
    pharosIThreadInterPartitionRestore(runningThread , original , wasInside , originalInterPartitionContext);

    /* return the result of the application filter */
    return result;
}


PharosChannelFilterSendR pharosIChannelFilterSendExecute(ptrThreadInfo runningThread , ptrChannel channel , PartitionNumber partitionId , ptrChannelBulk bulk , ptrStack sharedStack , ptrPartition original , bool *wasInside , ptrPharosCpuInterPartitionContext originalInterPartitionContext , PharosPriority priority)
{
    /* this part of the code is placed in a separate function/file from channelfilter.c on purpose since test 1890 in 
     * versatilepb-release would fail. 
     * This is suspected because GCC (8.2.0) optimizations would mess the order around macro 
     * pharosIThreadInterPartitionSwitch which is very delicate to work with */

    /* return value */
    PharosChannelFilterSendR result;

    /* MMU context of the channel owner */
    ptrPartitionCpuMmuContext context = pharosIPartitionMemoryContext(channel->owner);


    /* change the stack to the queue owner memory space */
    pharosCpuAddSharedStack(runningThread , context);

    /* if the switch went OK execute the filter */
    result = pharosCpuChannelSendExecuteFilter(channel->owner->data , channel->id.channel , partitionId , priority , bulk ,
                                               channel->sendFilter , pharosIPartitionMemoryContext(channel->owner) ,
                                               pharosCpuInitializeStackPointer(sharedStack->stack , sharedStack->size) , channel->owner);

    /* change the stack back to the kernel space */
    pharosCpuRemoveSharedStack(runningThread , context);

    /* restore the thread running partition */
    pharosIThreadInterPartitionRestore(runningThread , original , wasInside , originalInterPartitionContext);

    /* return the result of the application filter */
    return result;
}
