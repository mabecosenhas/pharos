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


#ifndef PHAROS_PARTITION_DECLARATIONS_H
#define PHAROS_PARTITION_DECLARATIONS_H


#include <pharos/hal/hal.h>
#include <pharos/kernel/core/declarations.h>


#define MAXIMUM_NUMBER_PARTITIONS   (64U)
#define PHAROS_PARTITION_INVALID    (0xffU)

typedef struct Partition Partition , *ptrPartition;

typedef struct PartitionTable PartitionTable , *ptrPartitionTable;

typedef struct IoMemAreaTable IoMemAreaTable , *ptrIoMemAreaTable;

typedef struct IoMemAreaDirect IoMemAreaDirect , *ptrIoMemAreaDirect;

typedef struct PharosCpuPartitionInfo PharosCpuPartitionInfo , *ptrPharosCpuPartitionInfo;

/**
 * Index of a partition
 */
typedef uint32_t PartitionNumber , *ptrPartitionNumber;

typedef struct IpCall IpCall , *ptrIpCall;

typedef struct IpCallTable IpCallTable , *ptrIpCallTable;

typedef struct IpCallMinimum IpCallMinimum , *ptrIpCallMinimum;


/**
 * Function called to determine if the running partition has enough permissions to restart the restarting partition
 * 
 * @param runningPartition running partition that attempts to restart another partition
 * @param restartingPartition partition to be restarted
 * 
 * @return returns TRUE if the running partition is allowed to restart the restartingPartition and FALSE otherwise
 */
typedef bool (*PartitionRestartGrantHandler)(PartitionNumber runningPartition , PartitionNumber restartingPartition);

/**
 * Number of partition restarts
 */
typedef uint64_t PartitionRestartNumber;

typedef enum
{
    /**
     * Partition is initializing but thread scheduling is enabled. That is, has new threads are 
     * being created they can be scheduled
     */
    PHAROS_PARTITION_STATE_INITIALIZING = 0 ,

    /**
     * Partition in normal mode. All initialization has been done.
     * 
     */
    PHAROS_PARTITION_STATE_NOMINAL = 1 ,

    /**
     * Partition is restarting. Its threads will not execute while the partition is under this mode 
     * (except for the restarting thread)
     */
    PHAROS_PARTITION_STATE_RESTARTING = 2 ,

    /**
     * Partition has been stopped and no threads will be scheduled.
     */
    PHAROS_PARTITION_STATE_STOPPED = 3 ,
} PharosPartitionState;


#endif /* DECLARATIONS_H */
