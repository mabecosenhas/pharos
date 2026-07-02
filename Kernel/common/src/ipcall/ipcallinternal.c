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


static PharosIpCallR pharosIExecuteIpCallFunction(ptrIpCall call , void *input , void *output , ptrStack sharedStack ,
                                                  ptrPartition caller);


PharosIpCallR pharosIIpCallFunctionExecute(ptrIpCall call , ptrThreadInfo runningThread , uint8_t *newInput ,
                                           void *newOutput , void *output , ptrStack sharedStack ,
                                           bool *wasInside , ptrPartition original ,
                                           ptrPharosCpuInterPartitionContext originalInterPartitionContext)
{
    /* this function was removed from inside the pharosIIpCall function to avoid a similar problem to the
     * function pharosIChannelFilterSendExecute, in which GCC (8.2.0) optimizations for the versatilepb-release target 
     * would mess the order around macro pharosIThreadInterPartitionSwitch which is very delicate to work with */

    /* return value */
    PharosIpCallR result;

    /* it was not, continue with switch */

    /* if the switch went OK execute the filter */
    if(call->min->ipcFilterPointer != NULL)
    {
        /* executes the filter function */
        result = pharosCpuExecuteIpCallFilter(call->partition->data , runningThread->partition->id , newInput ,
                                              pharosCpuInitializeStackPointer(sharedStack->stack , sharedStack->size) ,
                                              pharosIPartitionMemoryContext(call->partition) ,
                                              call->min->ipcFilterPointer , call->partition);

        /* if the filter give success */
        if(result == PHAROS_IPC_RESULT_SUCCESS)
        {
            /* then execute the function */
            result = pharosIExecuteIpCallFunction(call , newInput , newOutput , sharedStack , runningThread->partition);
        }
    }
        /* if there is no filter function */
    else
    {
        /* then execute the function */
        result = pharosIExecuteIpCallFunction(call , newInput , newOutput , sharedStack , runningThread->partition);
    }

    /* if the output needs to be copied */
    if(call->min->outputSize != 0U)
    {
        /* copies the output from the shared stack */
        memcpy(output , newOutput , call->min->outputSize);

        /* increases the shared stack */
        pharosStackFree(sharedStack , call->min->outputSize);
    }

    /* if we previously allocated space for the input */
    if(call->min->inputSize != 0U)
    {
        /* increases the shared stack */
        pharosStackFree(sharedStack , call->min->inputSize);
    }

    /* restore the thread running partition */
    pharosIThreadInterPartitionRestore(runningThread , original , wasInside , originalInterPartitionContext);

    /* return the result of the ip-call */
    return result;
}


PharosIpCallR pharosIExecuteIpCallFunction(ptrIpCall call , void *input , void *output , ptrStack sharedStack , ptrPartition caller)
{
    /* return value */
    PharosIpCallR result;

    /* MMU context of the callee partition */
    ptrPartitionCpuMmuContext context = pharosIPartitionMemoryContext(call->partition);


    /* if has an input */
    if(call->min->hasInput == TRUE)
    {
        /* call the function under a different stack and different MMU context and both input and output */
        result = pharosCpuExecuteIpCallIo(call->partition->data , input , output ,
                                          pharosCpuInitializeStackPointer(sharedStack->stack , sharedStack->size) ,
                                          context , call->functionPointer , caller , call->partition);
    }
        /* if no input is defined */
    else
    {
        /* call the function under a different stack and different MMU context and only output */
        result = pharosCpuExecuteIpCallO(call->partition->data , output ,
                                         pharosCpuInitializeStackPointer(sharedStack->stack , sharedStack->size) ,
                                         context , call->functionPointer , caller , call->partition);
    }

    /* return the result of the call */
    return result;
}
