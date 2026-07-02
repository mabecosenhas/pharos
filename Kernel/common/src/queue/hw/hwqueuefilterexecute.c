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


PharosHwQueueFilterR pharosIHwQueueFilterExecute(ptrThreadInfo runningThread , ptrHeavyWeightQueue queue ,
                                                 uint32_t size , PharosPriority priority , ptrStack sharedStack ,
                                                 bool *wasInside , ptrPartition caller , ptrPartition original ,
                                                 ptrPharosCpuInterPartitionContext originalInterPartitionContext)
{
    /* this function was removed from inside the pharosIHwQueueFilter function to avoid a similar problem to the
     * function pharosIChannelFilterSendExecute, in which GCC (8.2.0) optimizations for the versatilepb-release target 
     * would mess the order around macro pharosIThreadInterPartitionSwitch which is very delicate to work with */

    /* return value */
    PharosHwQueueFilterR result;

    /* get the memory context of the partition that owns the queue */
    ptrPartitionCpuMmuContext context = pharosIPartitionMemoryContext(queue->owner);


    /* change the stack to the queue owner memory space */
    pharosCpuAddSharedStack(runningThread , context);

    /* if the switch went OK execute the filter in the separate stack and memory area */
    result = pharosCpuHwQueueExecuteFilter(queue->owner->data , queue->id , caller->id , priority , size ,
                                           queue->filter , context ,
                                           pharosCpuInitializeStackPointer(sharedStack->stack , sharedStack->size) , queue->owner);

    /* change the stack back to the kernel space */
    pharosCpuRemoveSharedStack(runningThread , context);

    /* restore the thread running partition context */
    pharosIThreadInterPartitionRestore(runningThread , original , wasInside , originalInterPartitionContext);

    /* return the result of the application filter */
    return result;
}
