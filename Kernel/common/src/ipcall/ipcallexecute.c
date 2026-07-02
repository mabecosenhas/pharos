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


#include <pharos/kernel/partition/ipcall.h>
#include <pharos/kernel/partition/partitioninline.h>


PharosIpCallR pharosIIpCall(ptrIpCall call , void *input , void *output , ptrThreadInfo runningThread)
{
    /* this function is placed in a separate file from ipcall.c on purpose since test 1900 in 
     * versatilepb-release would fails in a similar code for pharosIHwQueueFilterFunction function. 
     * This is suspected because GCC optimizations would place this function inside the caller function */

    /* return value of this method */
    PharosIpCallR result;

    /* shared stack of the running thread */
    ptrStack sharedStack;
 
    /* MMU context of the callee partition */
    ptrPartitionCpuMmuContext context = pharosIPartitionMemoryContext(call->partition);

    /* output pointer at the different stack */
    void *newOutput = output;

    /* input pointer at the different stack */
    uint8_t *newInput = input;

    /* variable to store the partition */
    ptrPartition original;

    bool wasInside;

    /* flag that indicates if this thread was interrupted while inside the other partition (due to a partition restart) */
    bool interrupted;

    /* inter-partition context */
    PharosCpuInterPartitionContext interPartitionContext;

    /* previous inter-partition context */
    ptrPharosCpuInterPartitionContext originalInterPartitionContext;


    /* if there is no input */
    if(call->min->hasInput == FALSE)
    {
        /* then the output is actually in the place of the input */
        output = input;

        /* update the output pointer at the different stack */
        newOutput = input;
    }
 
    /* get the shared stack */
    sharedStack = pharosIStackSharedMemory(runningThread->sharedAccessStack);

    /* change the stack to the calling partition memory space */
    pharosCpuAddSharedStack(runningThread , context);

    /* if the function has input */
    if(call->min->inputSize != 0U)
    {
        /* update the pointer of the input */
        newInput = pharosCpuInitializeStackPointer(sharedStack->stack , sharedStack->size) - call->min->inputSize;

        /* copies the input to the alternate stack location */
        memcpy(newInput , input , call->min->inputSize);

        /* decreases the alternate stack */
        pharosStackReserve(sharedStack , call->min->inputSize);
    }

    /* if has an output */
    if(call->min->outputSize != 0U)
    {
        /* update the pointer of the output */
        newOutput = pharosCpuInitializeStackPointer(sharedStack->stack , sharedStack->size) - call->min->outputSize;

        /* decreases the alternate stack */
        pharosStackReserve(sharedStack , call->min->outputSize);
    }
    
    
    /* switch partition */
    pharosIThreadInterPartitionSwitch(runningThread , call->partition , &original , &wasInside ,
                                      &interPartitionContext , &originalInterPartitionContext , &interrupted);

    /* check if the partition switch was interrupted */
    if(interrupted == FALSE)
    {
        /* execute the call (if this thread is restarted, this thread context will jump to the macro 
         * pharosIThreadInterPartitionSwitch but interrupted will be FALSE */
        result = pharosIIpCallFunctionExecute(call , runningThread , newInput , newOutput , output , sharedStack ,
                                              &wasInside , original , originalInterPartitionContext);
    }
        /* if this thread was interrupted it should return an error (the other partition is restarting */
    else
    {
        /* return that the other partition is not initialized */
        result = PHAROS_IPC_RESULT_UNINITIALIZED;
    }

    /* change the stack back to the kernel space */
    pharosCpuRemoveSharedStack(runningThread , context);

    /* return the error raised or success */
    return result;
}


