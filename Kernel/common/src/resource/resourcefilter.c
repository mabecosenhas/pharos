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


PharosResourceFilterR pharosIResourceFilter(ptrResource resource , uint64_t input)
{
    /* this function is placed in a separate file from resource.c on purpose since test 1900 in 
     * versatilepb-release would fails in a similar code for pharosIHwQueueFilterFunction function. 
     * This is suspected because GCC optimizations would place this function inside the caller function */

    /* return value */
    PharosResourceFilterR result;

    /* running thread */
    ptrThreadInfo runningThread = pharosISchedGetRunningThread();

    /* get the shared stack */
    ptrStack sharedStack = pharosIStackSharedMemory(runningThread->sharedAccessStack);

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


    /* if there is no shared resource stack */
    if((sharedStack == NULL) || (sharedStack->stack == NULL))
    {
        /* return invalid shared stack */
        result = PHAROS_RESOURCE_ACCESS_RUNNING_THREAD_INVALID_SHARED_STACK;
    }
        /* if there is a filter to execute */
    else if(resource->filter != NULL)
    {

        /* switch partition */
        pharosIThreadInterPartitionSwitch(runningThread , resource->owner , &original , &wasInside ,
                                          &interPartitionContext , &originalInterPartitionContext , &interrupted);

        /* check if the partition switch was interrupted */
        if(interrupted == FALSE)
        {
            /* it was not, continue with switch */

            /* execute the filter (if this thread is restarted, this thread context will jump to the macro 
             * pharosIThreadInterPartitionSwitch but interrupted will be FALSE */
            result = pharosIResourceFilterExecute(runningThread , resource , &wasInside , input , sharedStack ,
                                                  original , originalInterPartitionContext);

        }
            /* if this thread was interrupted it should return an error (the other partition is restarting */
        else
        {
            /* return that the other partition is not initialized */
            result = PHAROS_RESOURCE_ACCESS_UNINITIALIZED;
        }
    }
    else
    {
        /* no filter to execute */
        result = PHAROS_RESOURCE_FILTER_SUCCESS;
    }

    /* return the result of the filter */
    return result;
}
