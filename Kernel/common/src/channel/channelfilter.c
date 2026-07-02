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


#include <pharos/kernel/channel/channel.h>
#include <pharos/kernel/channel/channelbulkkernel.h>
#include <pharos/kernel/partition/partitioninline.h>


PharosChannelFilterReceiveR pharosIChannelFilterReceive(ptrChannel channel)
{
    /* this function is placed in a separate file from channelreceive.c on purpose since test 1900 in 
     * versatilepb-release would fails in a similar code for pharosIHwQueueFilterFunction function. 
     * This is suspected because GCC optimizations would place this function inside the caller function */

    /* return value */
    PharosChannelFilterReceiveR result;

    /* shared stack of the running thread */
    ptrStack sharedStack;

    /* running thread */
    ptrThreadInfo runningThread;

    /* get the running partition */
    ptrPartition partition = pharosIPartitionGetRunning();

    /* get the running partition (before we change it) */
    PartitionNumber partitionId = partition->id;


    /* original partition */
    ptrPartition original;

    bool wasInside;

    /* flag that indicates if this thread was interrupted while inside the other partition (due to a partition restart) */
    bool interrupted;

    /* inter-partition context */
    PharosCpuInterPartitionContext interPartitionContext;

    /* previous inter-partition context */
    ptrPharosCpuInterPartitionContext originalInterPartitionContext;


    /* if there is no filter function or the running partition is the channel owner */
    if((channel->receiveFilter == NULL) || (channel->owner == partition))
    {
        /* send the message (no need to execute the filter if receiving on the owner of the channel) */
        result = PHAROS_CHANNEL_SEND_FILTER_SUCCESS;
    }
    else
    {
        /* if an interrupt is running (not a thread) */
        if(pharosIInterruptIsRunning() == TRUE)
        {
            /* use the current interrupt stack to call the filter */
            result = channel->receiveFilter(channel->owner->data , partitionId , channel->id);
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
                result = PHAROS_CHANNEL_RECEIVE_INVALID_SHARED_STACK;
            }
            else
            {
                /* switch partition */
                pharosIThreadInterPartitionSwitch(runningThread , channel->owner , &original , &wasInside ,
                                                  &interPartitionContext , &originalInterPartitionContext ,
                                                  &interrupted);

                /* check if the partition switch was interrupted */
                if(interrupted == FALSE)
                {
                    /* it was not, continue with switch */

                    /* execute the filter (if this thread is restarted, this thread context will jump to the macro 
                     * pharosIThreadInterPartitionSwitch but interrupted will be FALSE */
                    result = pharosIChannelFilterReceiveExecute(runningThread , channel , partitionId , sharedStack ,
                                                                original , &wasInside , originalInterPartitionContext);
                }
                    /* if this thread was interrupted it should return an error (the other partition is restarting */
                else
                {
                    /* return that the other partition is not initialized */
                    result = PHAROS_CHANNEL_RECEIVE_UNINITIALIZED;
                }
            }
        }
    }

    return result;
}


PharosChannelFilterSendR pharosIChannelFilterSend(ptrChannel channel , PharosPriority priority ,
                                                  ptrChannelBulk bulk)
{
    /* this function is placed in a separate file from channelsend.c on purpose since test 1900 in 
     * versatilepb-release would fails in a similar code for pharosIHwQueueFilterFunction function. 
     * This is suspected because GCC optimizations would place this function inside the caller function */

    /* return value */
    PharosChannelFilterSendR result;

    /* shared stack of the running thread */
    ptrStack sharedStack;

    /* running thread */
    ptrThreadInfo runningThread;

    /* get the running partition */
    ptrPartition partition = pharosIPartitionGetRunning();

    /* get the running partition (before we change it) */
    PartitionNumber partitionId = partition->id;

    /* original partition */
    ptrPartition original;

    /* auxiliary variable needed when switching partition */
    bool wasInside;

    /* flag that indicates if this thread was interrupted while inside the other partition (due to a partition restart) */
    bool interrupted;

    /* inter-partition context */
    PharosCpuInterPartitionContext interPartitionContext;

    /* previous inter-partition context */
    ptrPharosCpuInterPartitionContext originalInterPartitionContext;


    /* if there is no filter function */
    if(channel->sendFilter == NULL)
    {
        /* send the message */
        result = PHAROS_CHANNEL_SEND_FILTER_SUCCESS;
    }
    else
    {
        /* if an interrupt is running (not a thread) */
        if(pharosIInterruptIsRunning() == TRUE)
        {
            /* use the current interrupt stack to call the filter */
            result = channel->sendFilter(channel->owner->data , channel->id.channel , partitionId , priority , bulk);
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
                result = PHAROS_CHANNEL_SEND_INVALID_SHARED_STACK;
            }
            else
            {
                /* switch partition */
                pharosIThreadInterPartitionSwitch(runningThread , channel->owner , &original , &wasInside ,
                                                  &interPartitionContext , &originalInterPartitionContext , &interrupted);

                /* check if the partition switch was interrupted */
                if(interrupted == FALSE)
                {
                    /* it was not, continue with switch */
                    result = pharosIChannelFilterSendExecute(runningThread , channel , partitionId , bulk , sharedStack , original , &wasInside ,
                                                             originalInterPartitionContext , priority);
                }
                    /* if this thread was interrupted it should return an error (the other partition is restarting */
                else
                {
                    /* return that the other partition is not initialized */
                    result = PHAROS_CHANNEL_SEND_UNINITIALIZED;
                }
            }
        }
    }

    /* return the result */
    return result;
}
