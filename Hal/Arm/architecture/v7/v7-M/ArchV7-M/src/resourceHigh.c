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
#include <pharos/hal/v7m/misc.h>

#include "pharos/hal/cpu/context.h"


PharosResourceFilterR pharosCpuResourceExecuteFilterLow(void *data , ResourceNumber id , uint64_t input , PartitionNumber callerId ,
                                                        ResourceFilterMethod filter , ptrPartitionCpuMmuContext context ,
                                                        StackPointer sharedStack);

uint64_t pharosCpuResourceExecuteAccessLow(void *data , uint64_t input , ResourceAccessMethod method , ptrPartitionCpuMmuContext context ,
                                           StackPointer sharedStack);


PharosResourceFilterR pharosCpuResourceExecuteFilter(void *data , ResourceNumber id , uint64_t input , PartitionNumber callerId ,
                                                     ResourceFilterMethod filter , ptrPartitionCpuMmuContext context ,
                                                     StackPointer sharedStack , ptrPartition resourceOwner)
{
    /* result of the filter method */
    PharosResourceFilterR result;

    /* get the thread partition */
    ptrPartition partition = pharosISchedGetRunningThread()->partition;


    PHAROS_ASSERT(pharosCpuControlIsUnpriv() == FALSE , "Running in unprivileged mode");

    /* need to change the MPU to the specified context */
    pharosCpuMpuSwitchPartition(&resourceOwner->cpuInfo.partitionMpuContext);

    /* execute the low level filter on a different stack and with user permissions */
    result = pharosCpuResourceExecuteFilterLow(data , id , input , callerId , filter , context , sharedStack);

    /* restore the previous partition MPU regions */
    pharosCpuMpuSwitchPartition(&partition->cpuInfo.partitionMpuContext);

    /* return the result of the resource filter */
    return result;
}


uint64_t pharosCpuResourceExecuteAccess(void *data , uint64_t input , ResourceAccessMethod method , ptrPartitionCpuMmuContext context ,
                                        StackPointer sharedStack , ptrPartition resourceOwner)
{
    /* result of the access method */
    uint64_t result;

    /* get the thread partition */
    ptrPartition partition = pharosISchedGetRunningThread()->partition;


    /* need to change the MPU to the specified context */
    pharosCpuMpuSwitchPartition(&resourceOwner->cpuInfo.partitionMpuContext);

    /* execute the low level filter on a different stack and with user permissions */
    result = pharosCpuResourceExecuteAccessLow(data , input , method , context , sharedStack);

    /* restore the previous partition MPU regions */
    pharosCpuMpuSwitchPartition(&partition->cpuInfo.partitionMpuContext);

    /* return the result of the resource access method */
    return result;
}
