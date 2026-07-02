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


#include <pharos/hal/boardApi.h>
#include <pharos/kernel/object/object.h>
#include <pharos/kernel/partition/partition.h>
#include <pharos/kernel/thread/rtthread.h>
#include <pharos/kernel/timer/timer.h>
#include <pharos/hal/boardApi.h>
#include <pharos/kernel/bulk/bulk.h>
#include <pharos/kernel/channel/channel.h>
#include <pharos/kernel/resource/resource.h>
#include <pharos/kernel/thread/thread.h>
#include <pharos/error.h>
#include <pharos/kernel/semaphore/counting.h>


ptrObjectTable pharosIPartitionRunObjectTable(void)
{
    /* get the partition */
    return &pharosIPartitionGetRunning()->objects;
}


ptrObjectTable pharosIPartitionGetObjectsTable(PartitionNumber number)
{
    /* return value */
    ptrObjectTable result;

    /* pointer to the partition */
    ptrPartition partition;


    /* get the partition */
    partition = pharosIPartitionGet(number);

    /* if the partition exists */
    if(partition != NULL)
    {
        /* get the driver and its object table */
        result = &partition->objects;
    }
    else
    {
        /* partition does not exist */
        result = NULL;
    }

    /* return the object table */
    return result;
}


ptrObjectTable pharosIGetObjectsTable(PartitionNumber number)
{
    /* result of the method */
    ptrObjectTable result;

    /* get the partition */
    ptrPartition partition = pharosIPartitionGet(number);


    /* if the partition is correct */
    if(partition != NULL)
    {
        /* return its object table */
        result = &partition->objects;
    }
        /* if the partition is incorrect */
    else
    {

        /* return NULL */
        result = NULL;
    }

    /* return the object table or NULL if the partition number is invalid */
    return result;
}


void pharosIObjectTableReinitialize(ptrObjectTable table , ptrPartition partition)
{
    /* initialize all semaphores */
    pharosISemsCeilInitialize(partition , &table->ceilingSemaphoreTable);

    /* initialize all mutex semaphores */
    pharosISemsMutexInitialize(&table->mutexSemaphoreTable);

    /* initialize all counting semaphores */
    pharosISemsCountInitialize(&table->countingSemaphoreTable);

    /* initialize all light weight queues */
    pharosILwQueuesInitialize(&table->lwQueueTable);

    /* initialize hw queues */
    pharosIHwQueuesInitialize(partition , &table->hwQueueTable);

    /* initialize the bulks */
    pharosIBulksInitialize(&table->bulkTable);

    /* initialize resources */
    pharosIResourcesInitialize(partition , &table->resourceTable);

    /* initialize channels */
    pharosIChannelsInitialize(partition , &table->channelTable);

    /* initialize all timers */
    pharosITimersInitialize(&table->timerTable , partition , TRUE);

    /* re-initialize all threads (restart = TRUE) */
    pharosIThreadsInitialize(&table->threadTable , partition , TRUE);
}


void pharosIObjectTableInitialize(ptrObjectTable table , ptrPartition partition)
{
    /* initialize all semaphores */
    pharosISemsCeilInitialize(partition , &table->ceilingSemaphoreTable);

    /* initialize all mutex semaphores */
    pharosISemsMutexInitialize(&table->mutexSemaphoreTable);

    /* initialize all counting semaphores */
    pharosISemsCountInitialize(&table->countingSemaphoreTable);

    /* initialize all light weight queues */
    pharosILwQueuesInitialize(&table->lwQueueTable);

    /* initialize hw queues */
    pharosIHwQueuesInitialize(partition , &table->hwQueueTable);

    /* initialize the bulks */
    pharosIBulksInitialize(&table->bulkTable);

    /* initialize resources */
    pharosIResourcesInitialize(partition , &table->resourceTable);

    /* initialize channels */
    pharosIChannelsInitialize(partition , &table->channelTable);

    /* initialize the channel bulks */
    pharosIChannelBulkQueuesInit(partition , &table->channelBulkTable);

    /* initialize all timers */
    pharosITimersInitialize(&table->timerTable , partition , FALSE);

    /* initialize all threads (must be last for the lazy load feature since the threads will be made ready and may start 
     * to execute, so all other objects should have been created already) */
    pharosIThreadsInitialize(&table->threadTable , partition , FALSE);
}


/**
 * Initializes an object id of a partition
 * 
 * @param id object where to write to
 * @param partitionNumber
 * @param objectNumber
 */
bool pharosIObjectIdInitialize(const ptrObjectId id , PartitionNumber partitionNumber , ObjectNumber objectNumber)
{
    /* return of the method */
    bool result;

    /* get the partition */
    ptrPartition p = pharosIPartitionGet(partitionNumber);

    /* if found the partition */
    if(p != NULL)
    {
        /* initialize the partition */
        id->partition = p;

        /* initialize the object number */
        id->objectNumber = objectNumber;

        /* found the partition */
        result = TRUE;
    }
        /* if not found the partition */
    else
    {
        /* initialize with invalid partition */
        id->partition = NULL;

        /* and invalid number */
        id->objectNumber = 0xffffffff;

        /* object not found */
        result = FALSE;
    }

    /* returns TRUE of the object was found */
    return result;
}


/**
 * determines if the object id is valid 
 * 
 * @param id id to determine
 * @param maximum maximum number of objects of the id type
 * 
 * @return returns TRUE if the object is valid and FALSE otherwise 
 */
bool pharosIObjectIdIsValid(const ptrObjectId id , ObjectNumber maximum)
{
    /* if the number is below the maximum then its valid */
    return id->objectNumber < maximum ? TRUE : FALSE;
}


/**
 * Get the memory protection information of the environment (partition/driver)
 * 
 * Protection: none
 * Constraints: Assumes the environment id is valid
 * 
 * @param environment environment to get the memory protection information from 
 * 
 * @return returns the memory protection address of the specified environment
 */
ptrEnvMemoryProtection pharosIPartitionMemoryProt(ptrPartition partition)
{
    /* return value of the function */
    ptrEnvMemoryProtection result;


    /* if the environment is a driver */
    if(partition == NULL)
    {
        /* return the kernel memory protection */
        result = (ptrEnvMemoryProtection) & pharosVKernelMemProtection;
    }
        /* else, is a partition (kernel is not called in this method) */
    else
    {
        /* get the start address of the partition */
        result = &(partition->memProtection);
    }

    /* return the start address of the environment */
    return result;
}


/**
 * Get the MMU context of a specified environment
 * 
 * @contraint call only with a driver or partition environment (not a kernel)
 * 
 * @param environment environment to analyze
 * 
 * @return returns the MMU context of the specified environment
 */
ptrPartitionCpuMmuContext pharosIPartitionMemoryContext(const ptrPartition partition)
{
    /* get the start address of the partition */
    return &partition->context;
}


ptrPartitionCpuMmuContext pharosIPartitionRunningMemCont(void)
{
    /* return the start address of the environment */
    return &pharosIPartitionGetRunning()->context;
}


/**
 * Get the memory protection information of the running thread environment (partition/driver)
 * 
 * Protection: none
 *  
 * @return returns the memory protection address of the running thread environment
 */
ptrEnvMemoryProtection pharosIPartitionRunningMemProt(void)
{
    /* get the running thread environment protection */
    return pharosIPartitionMemoryProt(pharosIPartitionGetRunning());
}
