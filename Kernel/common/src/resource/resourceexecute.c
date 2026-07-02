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


PharosResourceFilterR pharosIResourceFilterExecute(ptrThreadInfo runningThread , ptrResource resource ,
                                                   bool *wasInside , uint64_t input , ptrStack sharedStack ,
                                                   ptrPartition original ,
                                                   ptrPharosCpuInterPartitionContext originalInterPartitionContext)
{
    /* this function was removed from inside the pharosIResourceFilter function to avoid a similar problem to the
     * function pharosIChannelFilterSendExecute, in which GCC (8.2.0) optimizations for the versatilepb-release target 
     * would mess the order around macro pharosIThreadInterPartitionSwitch which is very delicate to work with */

    /* return value */
    PharosResourceFilterR result;

    /* context of the resource owner */
    ptrPartitionCpuMmuContext context = pharosIPartitionMemoryContext(resource->owner);

    /* running partition that is sending the message (no need to call pharosIPartitionGetRunning since the resource cannot be accessed inside an interrupt) */
    ptrPartition callerId = runningThread->partition;


    /* change the stack to the queue owner memory space */
    pharosCpuAddSharedStack(runningThread , context);

    /* execute the filter on the shared stack and with user permissions */
    result = pharosCpuResourceExecuteFilter(resource->owner->data , resource->id , input , callerId->id , resource->filter ,
                                            pharosIPartitionMemoryContext(resource->owner) ,
                                            pharosCpuInitializeStackPointer(sharedStack->stack , sharedStack->size) , resource->owner);

    /* change the stack back to the kernel space */
    pharosCpuRemoveSharedStack(runningThread , context);

    /* restore the thread running partition */
    pharosIThreadInterPartitionRestore(runningThread , original , wasInside , originalInterPartitionContext);

    /* return the result of the application filter */
    return result;
}


void pharosIResourceAccessExecute(ptrThreadInfo runningThread , ptrResource resource , uint64_t input , uint64_t *aux ,
                                  Timeout timeout , bool *wasInside , ptrPartition original ,
                                  ptrPharosCpuInterPartitionContext originalInterPartitionContext)
{
    /* this function was removed from inside the pharosIResourceAccess function to avoid a similar problem to the
     * function pharosIChannelFilterSendExecute, in which GCC (8.2.0) optimizations for the versatilepb-release target 
     * would mess the order around macro pharosIThreadInterPartitionSwitch which is very delicate to work with */


    /* context of the resource owner */
    ptrPartitionCpuMmuContext context = pharosIPartitionMemoryContext(resource->owner);

    /* shared stack to use when accessing the resource method */
    ptrStack sharedStack = pharosIStackSharedMemory(runningThread->sharedAccessStack);


    /* change the stack to the queue owner memory space */
    pharosCpuAddSharedStack(runningThread , context);

    /* access the method in the context of the resource stack */
    *aux = pharosCpuResourceExecuteAccess(resource->owner->data , input , resource->access , context ,
                                          pharosCpuInitializeStackPointer(sharedStack->stack , sharedStack->size) , resource->owner);
 
    /* remove shared stack permissions */
    pharosCpuRemoveSharedStack(runningThread , context);

    /* restore the thread running partition */
    pharosIThreadInterPartitionRestore(runningThread , original , wasInside , originalInterPartitionContext);
}
