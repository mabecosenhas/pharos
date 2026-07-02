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


#include <pharos/kernel/queue/queue.h>
#include <pharos/kernel/partition/partitioninline.h>


PharosHwQueueFilterR pharosIHwQueueFilter(ptrHeavyWeightQueue queue , uint32_t size , PharosPriority priority)
{
    /* this function is placed in a separate file from hwqueue.c on purpose since test 1900 in versatilepb-release 
     * fails. This is suspected because GCC optimizations would place this function inside the caller function 
     * (pharosSHwQueueSend) which would mix up the stacks when calling the macro pharosIThreadInterPartitionSwitch */

    /* result of the function */
    PharosHwQueueFilterR result;

    /* shared stack of the running thread */
    ptrStack sharedStack;

    /* running thread */
    ptrThreadInfo runningThread;

    /* save the original partition */
    ptrPartition original;

    /* flag indicating if the thread is already inside the running partition list of external threads */
    bool wasInside;

    /* flag that indicates if this thread was interrupted while inside the other partition (due to a partition restart) */
    bool interrupted;

    /* inter-partition context */
    PharosCpuInterPartitionContext interPartitionContext;

    /* previous inter-partition context */
    ptrPharosCpuInterPartitionContext originalInterPartitionContext;

    /* get the partition that is calling the filter function */
    ptrPartition caller = pharosIPartitionGetRunning();


    /* if filter does not exist */
    if(queue->filter == NULL)
    {
        /* send the message */
        result = PHAROS_HEAVY_WEIGHT_FILTER_SUCCESS;
    }
    else
    {
        /* if an interrupt is running (not a thread) */
        if(pharosIInterruptIsRunning() == TRUE)
        {
            /* use the current interrupt stack and MMU context to call the filter */
            result = queue->filter(queue->owner->data , queue->id , pharosIPartitionGetRunning()->id , priority , size);
        }
        else
        {
            /* get the running thread */
            runningThread = pharosISchedGetRunningThread();

            /* get the shared stack */
            sharedStack = pharosIStackSharedMemory(runningThread->sharedAccessStack);

            /* if the calling thread does not have any shared stack */
            if((sharedStack == NULL) || (sharedStack->stack == NULL))
            {
                /* then we cannot invoke the filter */
                result = PHAROS_HEAVY_WEIGHT_SEND_INVALID_SHARED_STACK;
            }
            else
            {
                /* switch partition */
                pharosIThreadInterPartitionSwitch(runningThread , queue->owner , &original , &wasInside ,
                                                  &interPartitionContext , &originalInterPartitionContext ,
                                                  &interrupted);

                /* check if the partition switch was interrupted */
                if(interrupted == FALSE)
                {
                    /* it was not, continue with switch */

                    /* execute the filter (if this thread is restarted, this thread context will jump to the macro 
                     * pharosIThreadInterPartitionSwitch but interrupted will be FALSE */
                    result = pharosIHwQueueFilterExecute(runningThread , queue , size , priority , sharedStack ,
                                                         &wasInside , caller , original ,
                                                         originalInterPartitionContext);

                }
                    /* if this thread was interrupted it should return an error (the other partition is restarting */
                else
                {
                    /* return that the other partition is not initialized */
                    result = PHAROS_HEAVY_WEIGHT_SEND_UNINITIALIZED;
                }
            }
        }
    }

    /* return the result of the filter */
    return result;
}
