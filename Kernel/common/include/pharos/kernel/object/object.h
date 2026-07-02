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


#ifndef PHAROS_OBJECT_H
#define PHAROS_OBJECT_H


#include <pharos/hal/boardApi.h>
#include <pharos/kernel/object/declarations.h>
#include <pharos/kernel/resource/resourcetable.h>
#include <pharos/kernel/channel/channeltable.h>
#include <pharos/kernel/channel/channelbulktable.h>
#include <pharos/kernel/timer/timertable.h>
#include <pharos/kernel/semaphore/ceilingTable.h>
#include <pharos/kernel/semaphore/countingTable.h>
#include <pharos/kernel/semaphore/mutexTable.h>
#include <pharos/kernel/thread/threadtable.h>
#include <pharos/kernel/queue/hwtable.h>
#include <pharos/kernel/queue/lwtable.h>
#include <pharos/kernel/future/futureTable.h>
#include <pharos/kernel/bulk/bulktable.h>
#include <pharos/hal/memory/declarations.h>

struct ObjectId
{
    /**
     * Pointer to the partition
     */
    ptrPartition partition;

    /**
     * number of the object on the corresponding partition table
     */
    ObjectNumber objectNumber;
};

/**
 * @brief Table of objects 
 */
struct ObjectTable
{
    /**
     * @brief table of threads
     */
    ThreadTable threadTable;

    /**
     * @brief table of ceiling semaphores
     */
    SemaphoreCeilingTable ceilingSemaphoreTable;

    /**
     * @brief table of mutex semaphores
     */
    SemaphoreMutexTable mutexSemaphoreTable;

    /**
     * @brief table of counting semaphores
     */
    SemaphoreCountingTable countingSemaphoreTable;

    /**
     * @brief table of queues
     */
    LightWeightQueueTable lwQueueTable;

    /**
     * @brief table of bulks
     */
    BulkQueueTable bulkTable;

    /**
     * @brief table of resources
     */
    ResourceTable resourceTable;

    /**
     * @brief table of heavy weight queues
     */
    HeavyWeightQueueTable hwQueueTable;

    /**
     * @brief table of channels
     */
    ChannelTable channelTable;

    /**
     * @brief table of bulk queues
     */
    ChannelBulkQueueTable channelBulkTable;

    /**
     * @brief table of software timers
     */
    TimerTable timerTable;

    /**
     * @brief table of futures
     */
    FutureTable futureTable;

} STRUCTURE_FIELD_ALIGNMENT_8;


extern const EnvMemoryProtection pharosVKernelMemProtection;


/**
 * get the object table from the specified object id
 * 
 * @param number partition number
 * 
 * @return returns the pointer to the objects table or NULL if the partition number is invalid
 */
ptrObjectTable pharosIGetObjectsTable(PartitionNumber number);


/**
 * get the object table from the running partition 
 * 
 * @return returns the pointer to the objects table
 */
ptrObjectTable pharosIPartitionRunObjectTable(void);


/**
 * Get the object table of the specified partition (even if on a remote core)
 * 
 * @param number partition number
 * 
 * @return returns the object table if the partition number is valid (partition exists) and NULL otherwise
 */
ptrObjectTable pharosIPartitionGetObjectsTable(PartitionNumber number);


/**
 * Initialize the objects on the object table 
 * 
 * @param table object table 
 * @param id id of the environment where the object table is in
 */
void pharosIObjectTableInitialize(ptrObjectTable table , ptrPartition partition);


/**
 * Re-initialize the objects on the object table (don't initialize the channel bulks since they could belong to 
 * another partition right now)
 * 
 * @param table object table 
 * @param id id of the environment where the object table is in
 */
void pharosIObjectTableReinitialize(ptrObjectTable table , ptrPartition partition);


/**
 * Checks if the priority is valid 
 * 
 * @param priority priority to check
 * 
 * @return returns TRUE if the priority is valid and FALSE otherwise
 */
bool pharosICheckPriority(PharosPriority priority);


/**
 * Executes the deadline handler of the thread
 * 
 * @param thread thread that reached its deadline
 */
void pharosIExecuteDeadline(ptrRtThread thread);


/**
 * Initializes an object id of a partition
 * 
 * @param id object where to write to
 * @param partitionNumber
 * @param objectNumber
 */
bool pharosIObjectIdInitialize(const ptrObjectId id , PartitionNumber partitionNumber , ObjectNumber objectNumber);


/**
 * determines if the object id is valid 
 * 
 * @param id id to determine
 * @param maximum maximum number of objects of the id type
 * 
 * @return returns TRUE if the object is valid and FALSE otherwise 
 */
bool pharosIObjectIdIsValid(const ptrObjectId id , ObjectNumber maximum);


/**
 * Get the memory protection information of the partition
 * 
 * Protection: none
 * Constraints: Assumes the environment id is valid
 * 
 * @param environment environment to get the memory protection information from 
 * 
 * @return returns the memory protection address of the specified environment
 */
ptrEnvMemoryProtection pharosIPartitionMemoryProt(ptrPartition partition);


/**
 * Get the MMU context of a specified environment
 * 
 * @contraint call only with a partition environment
 * 
 * @param environment environment to analyze
 * 
 * @return returns the MMU context of the specified environment
 */
ptrPartitionCpuMmuContext pharosIPartitionMemoryContext(const ptrPartition partition);


/**
 * Get the MMU context of the running partition
 * 
 * @return returns the MMU context of the running partition
 */
ptrPartitionCpuMmuContext pharosIPartitionRunningMemCont(void);


/**
 * Get the memory protection information of the running thread partition
 * 
 * Protection: none
 *  
 * @return returns the memory protection address of the running thread environment
 */
ptrEnvMemoryProtection pharosIPartitionRunningMemProt(void);


#endif /* OBJECT_H */
