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


#include <pharos/kernel/thread/thread.h>
#include <pharos/kernel/resource/resource.h>
#include <pharos/kernel/scheduler/scheduler.h>
#include <pharos/kernel/object/protection.h>
#include <pharos/hal/boardApi.h>
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/kernel/partition/partitioninline.h>


/**
 * Get the resource on the local core
 * 
 * @param partitionNumber partition number
 * @param resourceNumber resource index
 * 
 * @return returns the resource or NULL if invalid
 */
static ptrResource pharosIResourceGet(PartitionNumber partitionNumber , ResourceNumber resourceNumber);


void pharosIResourcesInitializeMinimal(ptrPartition p)
{
    /* resource index */
    ResourceNumber i;

    /* get the table */
    ptrResourceTable table = &p->objects.resourceTable;

    /* pointer to the resource */
    ptrResource resource;


    /* for each resource */
    for(i = 0U; i < table->size; i++)
    {
        /* get the resource */
        resource = &table->resources[i];

        /* initialize the owner */
        resource->owner = p;

        /* set the resource as not initialized */
        resource->flags.isInitialized = FALSE;
    }
}


void pharosIResourcesInitialize(ptrPartition p , const ptrResourceTable table)
{
    /* resource index */
    ResourceNumber i;

    /* pointer to the resource */
    ptrResource resource;


    /* for each resource */
    for(i = 0U; i < table->size; i++)
    {
        /* get the resource */
        resource = &table->resources[i];

        /* initialize the resource id */
        resource->id = i;

        /* initialize the semaphore ceiling */
        pharosISemCeilInitialize(p , &resource->semaphore , TRUE);

        /* set the resource as initialized */
        resource->flags.isInitialized = TRUE;
    }
}


PharosResourceAccessR pharosSResourceAccess(PartitionNumber partitionNumber , ResourceNumber resourceNumber , uint64_t input , uint64_t *output , Timeout timeout)
{
    /* method return value */
    PharosResourceAccessR result;

    /* running thread */
    ptrThreadInfo runningThread;

    /* resource pointer */
    ptrResource resource;


    /* get the running thread */
    runningThread = pharosISchedGetRunningThread();

    /* get the resource */
    resource = pharosIResourceGet(partitionNumber , resourceNumber);

    /* if there is no resource */
    if(resource == NULL)
    {
        /* then return invalid */
        result = PHAROS_RESOURCE_ACCESS_INVALID;
    }
        /* if running inside an interrupt*/
    else if(pharosIInterruptIsRunning() == TRUE)
    {
        /* cannot obtain a resource inside an interrupt */
        result = PHAROS_RESOURCE_ACCESS_INSIDE_INTERRUPT;
    }
        /* check if there is sufficient time left to obtain the resource */
    else if(pharosIThreadHasEnoughTime(runningThread , resource->wcet) == FALSE)
    {
        /* not enough time */
        result = PHAROS_RESOURCE_ACCESS_NOT_GRANTED_WCET;
    }
        /* if output is invalid */
    else if(pharosCpuBufferIsInsideProtection((uint8_t *) output , sizeof (uint32_t) , pharosIPartitionRunningMemProt()) == FALSE)
    {
        /* return invalid buffer */
        result = PHAROS_RESOURCE_ACCESS_INVALID_OUTPUT;
    }
        /* check if the both the resource and partition that owns the resource have been initialized */
    else if((resource->flags.isInitialized == FALSE) || (pharosIPartitionIsInitialized(resource->owner) == FALSE))
    {
        /* then return uninitialized */
        result = PHAROS_RESOURCE_ACCESS_UNINITIALIZED;
    }
    else
    {

        /* execute the filter */
        result = pharosIResourceFilter(resource , input);

        /* check if the both the resource and partition that owns the resource have been initialized */
        if((resource->flags.isInitialized == FALSE) || (pharosIPartitionIsInitialized(resource->owner) == FALSE))
        {
            /* then return uninitialized */
            result = PHAROS_RESOURCE_ACCESS_UNINITIALIZED;
        }
        else
        {
            /* if the filter gave success */
            if(result == PHAROS_RESOURCE_ACCESS_SUCCESS)
            {
                /* return the result of the access to the resource */
                *output = pharosIResourceAccess(resource , input , &result , timeout);
            }
        }
    }

    /* return the method result */
    return result;
}


ptrResource pharosIResourceGet(PartitionNumber partitionNumber , ResourceNumber resourceNumber)
{
    /* result of the method */
    ptrResource resource;

    /* get the desired partition */
    ptrPartition p = pharosIPartitionGetOnRunCore(partitionNumber);


    /* check if the partition is valid */
    if(p != NULL)
    {
        /* get the resource table of the partition */
        ptrResourceTable table = &p->objects.resourceTable;

        /* if the resource id number is too high */
        if((table == NULL) || (resourceNumber >= table->size))
        {
            /* return invalid */
            resource = NULL;
        }
            /* if the id is OK */
        else
        {
            /* return the pointer to the resource */
            resource = &table->resources[resourceNumber];
        }
    }
        /* the partition is invalid */
    else
    {
        /* return invalid */
        resource = NULL;
    }

    /* return the result */
    return resource;
}
