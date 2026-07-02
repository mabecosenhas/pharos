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


#include <pharos/kernel/resource/resource.h>
#include <pharos/kernel/partition/partitioninline.h>


uint64_t pharosIResourceAccess(ptrResource resource , uint64_t input , PharosResourceAccessR *result , Timeout timeout)
{
    /* this function is placed in a separate file from resource.c on purpose since test 1900 in 
     * versatilepb-release would fails in a similar code for pharosIHwQueueFilterFunction function. 
     * This is suspected because GCC optimizations would place this function inside the caller function */

    /* return value of the resource access method */
    uint64_t aux = 0;

    /* running thread */
    ptrThreadInfo runningThread = pharosISchedGetRunningThread();

    /* save the original partition */
    ptrPartition original;

    /* flag indicating if the this thread is already running an inter-partition call */
    bool wasInside;

    /* flag that indicates if this thread was interrupted while inside the other partition (due to a partition restart) */
    bool interrupted;

    /* inter-partition context */
    PharosCpuInterPartitionContext interPartitionContext;

    /* previous inter-partition context */
    ptrPharosCpuInterPartitionContext originalInterPartitionContext;


    /* obtain the semaphore (and block if needed) */
    *result = pharosISemCeilObtain(&resource->semaphore , timeout , resource);

    /* if obtained the semaphore */
    if(*result == (PharosResourceAccessR) PHAROS_SEMAPHORE_CEILING_OBTAIN_SUCCESS)
    {
        /* switch partition */
        pharosIThreadInterPartitionSwitch(runningThread , resource->owner , &original , &wasInside ,
                                          &interPartitionContext , &originalInterPartitionContext , &interrupted);

        /* check if the partition switch was interrupted */
        if(interrupted == FALSE)
        {
            /* it was not, continue with switch */

            /* execute the access method (if this thread is restarted, this thread context will jump to the macro 
             * pharosIThreadInterPartitionSwitch but interrupted will be FALSE */
            pharosIResourceAccessExecute(runningThread , resource , input , &aux , timeout , &wasInside , original , originalInterPartitionContext);

            /* return success */
            *result = PHAROS_RESOURCE_ACCESS_SUCCESS;
        }
            /* if this thread was interrupted it should return an error (the other partition is restarting */
        else
        {
            /* return that the other partition is not initialized */
            *result = PHAROS_RESOURCE_ACCESS_UNINITIALIZED;
        }
        
        /* release the semaphore. Method will always return PHAROS_SEMAPHORE_CEILING_RELEASE_SUCCESS */
        pharosISemCeilRelease(&resource->semaphore , runningThread , resource);
    }
    /* else, if did not obtain the semaphore then the result is already stored in "result". 
     * There is a mapping between PharosResourceAccessR and PharosSemCeilObtainR */

    /* return the access method output */
    return aux;
}

