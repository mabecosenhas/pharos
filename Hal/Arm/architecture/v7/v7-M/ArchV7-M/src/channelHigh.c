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


#include <pharos/kernel/scheduler/scheduler.h>
#include <pharos/kernel/partition/partition.h>
#include <pharos/kernel/core/coreconfinline.h>

#include "pharos/hal/cpu/context.h"


PharosChannelFilterReceiveR pharosCpuChannelExecuteFilterReceiveLow(void *data , PartitionNumber receiverId ,
                                                                    ChannelId id , ChannelFilterReceiveMethod filter ,
                                                                    ptrPartitionCpuMmuContext context ,
                                                                    StackPointer sharedStack);


PharosChannelFilterSendR pharosCpuChannelExecuteFilterSendLow(void *data , ChannelNumber id , PartitionNumber sender ,
                                                              PharosPriority priority , ptrChannelBulk bulk ,
                                                              ChannelFilterSendMethod filter ,
                                                              ptrPartitionCpuMmuContext context ,
                                                              StackPointer sharedStack);


PharosChannelFilterSendR pharosCpuChannelSendExecuteFilter(void *data , ChannelNumber id , PartitionNumber sender ,
                                                           PharosPriority priority , ptrChannelBulk bulk ,
                                                           ChannelFilterSendMethod filter ,
                                                           ptrPartitionCpuMmuContext context , StackPointer sharedStack , ptrPartition channelOwner)
{
    /* result of the filter method */
    PharosChannelFilterSendR result;

    /* get the thread partition */
    ptrPartition partition = pharosISchedGetRunningThread()->partition;


    /* need to change the MPU to the specified context */
    pharosCpuMpuSwitchPartition(&channelOwner->cpuInfo.partitionMpuContext);

    /* execute the low level filter on a different stack and with user permissions */
    result = pharosCpuChannelExecuteFilterSendLow(data , id , sender , priority , bulk , filter , context , sharedStack);

    /* restore the previous partition MPU regions */
    pharosCpuMpuSwitchPartition(&partition->cpuInfo.partitionMpuContext);

    /* return the result of the resource filter */
    return result;
}


PharosChannelFilterReceiveR pharosCpuChannelReceiveExecuteFilter(void *data , PartitionNumber receiverId ,
                                                                 ChannelId id , ChannelFilterReceiveMethod filter ,
                                                                 ptrPartitionCpuMmuContext context ,
                                                                 StackPointer sharedStack , ptrPartition channelOwner)
{
    /* result of the filter method */
    PharosChannelFilterReceiveR result;

    /* get the thread partition */
    ptrPartition partition = pharosISchedGetRunningThread()->partition;


    /* need to change the MPU to the specified context */
    pharosCpuMpuSwitchPartition(&channelOwner->cpuInfo.partitionMpuContext);

    /* execute the low level filter on a different stack and with user permissions */
    result = pharosCpuChannelExecuteFilterReceiveLow(data , receiverId , id , filter , context , sharedStack);

    /* restore the previous partition MPU regions */
    pharosCpuMpuSwitchPartition(&partition->cpuInfo.partitionMpuContext);

    /* return the result of the resource filter */
    return result;
}
