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
#include <pharos/kernel/partition/partition.h>
#include <pharos/hal/boardApi.h>
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/kernel/core/core.h>
#include <pharos/kernel/partition/partitioninline.h>


/**
 * Get the ip-call for the specified partition 
 * 
 * @param partition partition to get the ip-call from
 * @param number number of the ip-call
 * 
 * @return returns the ip-call or NULL if none was found
 */
static ptrIpCall getIpCall(PartitionNumber partition , uint32_t number);


void pharosIIpCallsInitializeMinimal(ptrPartition partition)
{
    /* iterator on the table of inter-partition calls */
    uint32_t i;

    /* inter-partition call iterator */
    ptrIpCall call;

    /* call iteration */
    ptrIpCallMinimum minCall;

    /* get the array of IPC calls */
    const ptrIpCallMinimum array = partition->ipCallTable.ipCall;


    /* for every inter-partition call */
    for(i = 0U; i < partition->ipCallTable.number; i++)
    {
        /* get the call for the "i" iteration */
        minCall = &array[i];

        /* get the inter-partition call */
        call = &partition->ipCallTable.calls[i];

        /* copy to our internal representation of the call */
        call->functionPointer = minCall->ipcFunctionPointer;

        /* set the pointer to the minimum IP call */
        call->min = minCall;

        /* setup the call partition */
        call->partition = partition;

        /* initialize the inter-partition call */
        pharosCpuIpCallPreambleInit(call , minCall , i , partition->id);
    }
}


uint8_t *pharosCpuPspGet();


PharosIpCallR pharosSIpCall(void *input , void *output , uint32_t number , PartitionNumber partition)
{
    PharosIpCallR result;

    ptrIpCall call;

    /* shared stack of the running thread */
    ptrStack sharedStack;

    /* running thread */
    ptrThreadInfo runningThread;

    /* auxiliary pointer to the output */
    void *auxOutput = output;

    /* get the partition to call */
    ptrPartition p = pharosIPartitionGet(partition);


    /* get the running thread */
    runningThread = pharosISchedGetRunningThread();

    /* get the call */
    call = getIpCall(partition , number);

    /* if there is no such call */
    if(call == NULL)
    {
        /* return invalid call */
        result = PHAROS_IPC_RESULT_INVALID_CALL;
    }
        /* check if the partition has completely started (if call != NULL then p has a valid partition) */
    else if(pharosIPartitionIsStarted(p) == FALSE)
    {
        /* it has not started, return un-initialized */
        result = PHAROS_IPC_RESULT_UNINITIALIZED;
    }
        /* if running inside an interrupt */
    else if(pharosIInterruptIsRunning() == TRUE)
    {
        /* use the calling interrupt stack to call the filter */
        result = call->min->ipcFilterPointer(p->data , pharosIPartitionGetRunning()->id , input);

        /* check again if the partition has restarted */
        if(pharosIPartitionIsStarted(p) == FALSE)
        {
            /* then return uninitialized */
            result = PHAROS_IPC_RESULT_UNINITIALIZED;
        }
            /* if the filter give success */
        else if(result == PHAROS_IPC_RESULT_SUCCESS)
        {
            /* use the calling interrupt stack to call the function */

            /* if has an input */
            if(call->min->hasInput == TRUE)
            {
                /* call the function under a different stack and different MMU context and both input and output */
                result = ((IpCallIo) call->functionPointer)(call->partition->data , input , output);
            }
                /* if no input is defined */
            else
            {
                /* call the function with only the input */
                result = ((IpCallO) call->functionPointer)(call->partition->data , input);
            }
        }
    }
        /* the call exists */
    else
    {
        /* get the running thread */
        runningThread = pharosISchedGetRunningThread();

        /* get the shared stack */
        sharedStack = pharosIStackSharedMemory(runningThread->sharedAccessStack);

        /* if there is no input */
        if(call->min->hasInput == FALSE)
        {
            /* then the output is now the input */
            auxOutput = input;
        }

        /* check if the input is completely since the caller memory space */
        if((call->min->inputSize != 0U) &&
           (pharosCpuBufferIsInsideProtection(input , call->min->inputSize , pharosIPartitionRunningMemProt()) == FALSE))
        {
            /* return invalid input */
            result = PHAROS_IPC_RESULT_INVALID_INPUT;
        }
            /* check if the output is completely inside the caller memory space */
        else if((call->min->outputSize != 0U) &&
                (pharosCpuBufferIsInsideProtection(auxOutput , call->min->outputSize , pharosIPartitionRunningMemProt()) == FALSE))
        {
            /* return invalid output */
            result = PHAROS_IPC_RESULT_INVALID_OUTPUT;
        }
        else if((sharedStack == NULL) || (sharedStack->stack == NULL))
        {
            /* return invalid stack */
            result = PHAROS_IPC_RESULT_INVALID_SHARED_STACK;
        }
            /* if the stack size is not enough */
        else if(sharedStack->size < call->min->inputSize + call->min->outputSize + call->min->mininumStackSize)
        {
            /* return stack too small */
            result = PHAROS_IPC_RESULT_SHARED_STACK_SMALL;
        }
        else
        {
            /* execute the IP call */
            result = pharosIIpCall(call , input , output , runningThread);
        }
    }

    /* return the IP call return value or the error that was raised in this function */
    return result;
}


ptrIpCall getIpCall(PartitionNumber partition , uint32_t number)
{
    /* ip-call table of the partition */
    ptrIpCallTable table;

    /* result of the method */
    ptrIpCall result = NULL;

    /* get the desired partition */
    ptrPartition p = pharosIPartitionGetOnRunCore(partition);


    /* if could not find the partition on the running core */
    if(p != NULL)
    {
        /* get the IPC table for the specified partition */
        table = &p->ipCallTable;

        /* if the call number is too high */
        if(number >= table->number)
        {
            /* return invalid */
            result = NULL;
        }
            /* if the number is OK */
        else
        {
            /* return the pointer to the queue */
            result = &table->calls[number];
        }
    }

    /* return the result */
    return result;
}
