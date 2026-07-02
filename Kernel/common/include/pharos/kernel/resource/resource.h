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


#ifndef PHAROS_KERNELAPI_RESOURCE_H
#define PHAROS_KERNELAPI_RESOURCE_H

#include <pharos/kernel/resource/declarations.h>
#include <pharos/kernel/thread/declarations.h>
#include <pharos/resource.h>
#include <pharos/kernel/thread/declarations.h>
#include <pharos/kernel/semaphore/ceiling.h>
#include <pharos/kernel/partition/declarations.h>
#include <pharos/kernel/clock/declarations.h>
#include <pharos/kernel/object/object.h>

/**
 * A resource is used for inter-partitions synchronization protected through a filter (executed in the context of 
 * calling thread but with access to the memory context of the resource owner) and a semaphore ceiling.
 * 
 * Useful when partition need to access a shared resource and avoid priority inversion scenarios (prevented through the 
 * use of the semaphore ceiling).
 * 
 * The filter function is useful for protecting the resource from unbounded use from a low criticality erroneous 
 * partition. Since it is invoked in the context of the calling thread, then it will use up the time of the calling 
 * thread and not of the resource or the partition that owns the resource. Also, the filter is invoked with access to 
 * the memory area of the resource owner (and without access to the calling thread memory area), so it is protected 
 * from unwanted memory accesses.
 * 
 * The filter also allows the application to provide a fast method to allow or not accesses to a resource. For example, 
 * it can only allow accesses from partitions 0 and 1 and filter all else.
 */
struct Resource
{
    /**
     * Id of the resource
     */
    ResourceNumber id;

    /**
     * Method to filter (or not) the access to the resource
     */
    ResourceFilterMethod filter;

    /**
     * @brief access method to the resource
     */
    ResourceAccessMethod access;

    /**
     * @brief semaphore used by the resource
     */
    SemaphoreCeiling semaphore;

    /**
     * @brief WCET that the access method can take
     */
    Wcet wcet;

    /**
     * owner of the resource (when accessing the resource the access method will have MMU access to the owner and 
     * caller partition/driver space
     */
    ptrPartition owner;

    /**
     * Node on the thread list of resources hold (a resource can only be hold by one thread at a time) 
     */
    LinkedListNode threadNode;

    struct
    {
        /**
         * Flag indicating that the resource has been initialized
         * 
         * Note: initialized by PHAROS
         */
        uint32_t isInitialized : 1;
    } flags;
};


/**
 * Minimal initialization of the resources of the partition 
 * 
 * @param p partition to initialize
 */
void pharosIResourcesInitializeMinimal(ptrPartition p);


/**
 * initializes all resources inside the table
 *
 * Protection:
 */
void pharosIResourcesInitialize(ptrPartition p , const ptrResourceTable table);



/**
 * Internal syscall to execute the resource access
 * 
 * @param partitionNumber
 * @param resourceNumber
 * @param output
 * @param input
 * @param timeout
 * @return 
 */
PharosResourceAccessR pharosSResourceAccess(PartitionNumber partitionNumber , ResourceNumber resourceNumber , 
                                            uint64_t input , uint64_t *output , Timeout timeout);


/**
 * executes the resource access method
 * 
 * @param resource resource to execute
 * @param input input to the resource access method
 * @param result address where to place the result of the access method
 * @param timeout timeout to wait for the resource
 * 
 * @return returns the resource access method return value
 */
uint64_t pharosIResourceAccess(ptrResource resource , uint64_t input , PharosResourceAccessR *result , Timeout timeout);


/**
 * Executes the resource access method in the separate stack and with the queue owner memory permissions
 * 
 * @param runningThread running thread
 * @param resource resource to execute the filter of
 * @param input input to the resource
 * @param aux address where to place the return value of the resource
 * @param timeout timeout to obtain the resource (ceiling semaphore)
 * @param wasInside flag indicating if the thread is already inside the running partition list of external threads 
 * @param original original partition of the running thread
 * @param originalInterPartitionContext context of this thread before this call was made
 */
void pharosIResourceAccessExecute(ptrThreadInfo runningThread , ptrResource resource , uint64_t input , uint64_t *aux ,
                                  Timeout timeout , bool *wasInside , ptrPartition original ,
                                  ptrPharosCpuInterPartitionContext originalInterPartitionContext);


/**
 * executes the filter of the resource in the stack context of the resource
 * 
 * @param resource resource to execute the filter of
 * @param input filter input
 * 
 * @return returns the result of the filter execution
 */
PharosResourceFilterR pharosIResourceFilter(ptrResource resource , uint64_t input);


/**
 * Executes the filter in the separate stack and with the queue owner memory permissions
 * 
 * @param runningThread running thread
 * @param resource resource to execute the filter of
 * @param wasInside flag indicating if the thread is already inside the running partition list of external threads 
 * @param input input to the resource
 * @param sharedStack shared stack in which to place the thread stack
 * @param original original partition of the running thread
 * @param originalInterPartitionContext context of this thread before this call was made
 * 
 * @return returns the filter result
 */
PharosResourceFilterR pharosIResourceFilterExecute(ptrThreadInfo runningThread , ptrResource resource ,
                                                   bool *wasInside , uint64_t input , ptrStack sharedStack ,
                                                   ptrPartition original ,
                                                   ptrPharosCpuInterPartitionContext originalInterPartitionContext);


 
#endif /* RESOURCE_H */
