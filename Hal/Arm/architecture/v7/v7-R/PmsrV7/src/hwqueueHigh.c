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


PharosHwQueueFilterR pharosCpuHwQueueExecuteFilterLow(void *data , HwQueueNumber id , PartitionNumber sender , PharosPriority priority ,
                                                      uint32_t messageSize , HeavyWeightFilterMethod filter ,
                                                      ptrPartitionCpuMmuContext context , StackPointer sharedStack);


PharosHwQueueFilterR pharosCpuHwQueueExecuteFilter(void *data , HwQueueNumber id , PartitionNumber sender , PharosPriority priority ,
                                                   uint32_t messageSize , HeavyWeightFilterMethod filter ,
                                                   ptrPartitionCpuMmuContext context , StackPointer sharedStack , ptrPartition hwQueueOwner)

{
    /* result of the filter method */
    PharosHwQueueFilterR result;

    /* get the thread partition */
    ptrPartition partition = pharosISchedGetRunningThread()->partition;


    /* need to change the MPU to the specified context */
    pharosCpuMpuSwitchPartition(&hwQueueOwner->cpuInfo.partitionMpuContext);

    /* execute the low level filter on a different stack and with user permissions */
    result = pharosCpuHwQueueExecuteFilterLow(data , id , sender , priority , messageSize , filter , context , sharedStack);

    /* restore the previous partition MPU regions */
    pharosCpuMpuSwitchPartition(&partition->cpuInfo.partitionMpuContext);

    /* return the result of the resource filter */
    return result;
}
