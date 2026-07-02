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


#include <pharos/kernel/partition/partition.h>
#include <pharos/kernel/core/coreconfinline.h>


PharosIpCallR pharosCpuExecuteIpCallFilterLow(void *partitionData , PartitionNumber partition , uint8_t *input , uint8_t *stack , void *filterFunction);

PharosIpCallR pharosCpuExecuteIpCallIoLow(void *partitionData , uint8_t *input , uint8_t *output , uint8_t *stack , void *functionPointer);

PharosIpCallR pharosCpuExecuteIpCallOLow(void *partitionData , uint8_t *output , uint8_t *stack , void *functionPointer);


PharosIpCallR pharosCpuExecuteIpCallFilter(void *partitionData , PartitionNumber partitionNumber , uint8_t *input , uint8_t *stack ,
                                           ptrPartitionCpuMmuContext context , void *filterFunction , ptrPartition ipcallOwner)
{
    /* result of the filter method */
    PharosResourceFilterR result;

    /* get the thread partition */
    ptrPartition partition = pharosISchedGetRunningThread()->partition;


    /* need to change the MPU to the specified context */
    pharosCpuMpuSwitchPartition(&ipcallOwner->cpuInfo.partitionMpuContext);

    /* execute the low level filter on a different stack and with user permissions */
    result = pharosCpuExecuteIpCallFilterLow(partitionData , partitionNumber , input , stack , filterFunction);

    /* restore the previous partition MPU regions */
    pharosCpuMpuSwitchPartition(&partition->cpuInfo.partitionMpuContext);

    /* return the result of the resource filter */
    return result;
}


PharosIpCallR pharosCpuExecuteIpCallIo(void *partitionData , uint8_t *input , uint8_t *output , uint8_t *stack ,
                                       ptrPartitionCpuMmuContext context , void *functionPointer , ptrPartition caller , ptrPartition ipcallOwner)
{
    /* result of the filter method */
    PharosIpCallR result;

    /* get the thread partition */
    ptrPartition partition = pharosISchedGetRunningThread()->partition;


    /* need to change the MPU to the specified context */
    pharosCpuMpuSwitchPartition(&ipcallOwner->cpuInfo.partitionMpuContext);

    /* execute the low level filter on a different stack and with user permissions */
    result = pharosCpuExecuteIpCallIoLow(partitionData , input , output , stack , functionPointer);

    /* restore the previous partition MPU regions */
    pharosCpuMpuSwitchPartition(&partition->cpuInfo.partitionMpuContext);

    /* return the result of the resource filter */
    return result;
}


PharosIpCallR pharosCpuExecuteIpCallO(void *partitionData , uint8_t *output , uint8_t *stack , ptrPartitionCpuMmuContext context ,
                                      void *functionPointer , ptrPartition caller , ptrPartition ipcallOwner)
{
    /* result of the filter method */
    PharosIpCallR result;

    /* get the thread partition */
    ptrPartition partition = pharosISchedGetRunningThread()->partition;


    /* need to change the MPU to the specified context */
    pharosCpuMpuSwitchPartition(&ipcallOwner->cpuInfo.partitionMpuContext);

    /* execute the low level filter on a different stack and with user permissions */
    result = pharosCpuExecuteIpCallOLow(partitionData , output , stack , functionPointer);

    /* restore the previous partition MPU regions */
    pharosCpuMpuSwitchPartition(&partition->cpuInfo.partitionMpuContext);

    /* return the result of the resource filter */
    return result;
}
