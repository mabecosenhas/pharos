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


#ifndef PHAROS_PARTITION_IPCALL_H
#define PHAROS_PARTITION_IPCALL_H

#include <pharos/kernel/partition/declarations.h>
#include <pharos/kernel/thread/declarations.h>
#include <pharos/declarations.h>
#include <pharos/hal/cpu/cpu.h>

/**
 * Prototype for the IP-call filter
 */
typedef PharosIpCallR(*IpCallFilter)(void *partitionData , PartitionNumber number , void *input);

/**
 * One possible prototype for the IP-call method (with input and output)
 */
typedef PharosIpCallR(*IpCallIo)(void *data , void *input , void *output);

/**
 * Another possible prototype for the IP-call method (with only output)
 */
typedef PharosIpCallR(*IpCallO)(void *data , void *output);

/**
 * Pack the to occupy the minimum space
 */
#pragma pack(push ,4)

/**
 * Each function must obey to the following structure
 */
struct IpCallMinimum
{
    /**
     * Pointer to the IP-call function
     */
    void *ipcFunctionPointer;

    /**
     * Pointer to the IP-call filter function
     */
    IpCallFilter ipcFilterPointer;

    /**
     * Input (bytes) size
     */
    uint32_t inputSize;

    /**
     * Output (bytes) size
     */
    uint32_t outputSize;

    /**
     * Minimum stack size required to make the call
     */
    uint32_t mininumStackSize;

    /**
     * Flag indicating if the call has input
     */
    uint8_t hasInput;

    /**
     * Preamble of the call (written in assembly code)
     * This code will trigger a syscall (pharosSIpCall) to switch to kernel mode
     */
    IpCallPreamble preamble;
};

struct IpCall
{
    /**
     * original pointer to the function 
     */
    void *functionPointer;

    /**
     * pointer to the minimum IP call. The function pointer is changed to the preamble and the original function is stored in this structure
     */
    ptrIpCallMinimum min;


    /**
     * Partition where the IP call belongs to
     */
    ptrPartition partition;
};

struct IpCallTable
{
    /**
     * Array of inter-partition calls. Must have the same size as the number of functions defined in ipCall field
     */
    ptrIpCall calls;

    uint32_t number;

    /**
     * This is a pointer to the partition defined Inter-partition calls. It must follow the pattern defined as:
     * 
     * #pragma pack(push ,4)
     * 
     * typedef struct 
     * {
     *   // definition of one function 
     *   myFuncion1 function1;
     *   myFilter1 filter1;
     *   int32_t function1InputSize;
     *   int32_t function1OutputSize;
     *   uint32_t function1MinimumStackSize;
     *   uint8_t function1HasInput : 1;
     * 
     *   // definition of a second function 
     *   myFuncion2 function2;
     *   myFilter2 filter2;
     *   int32_t function2InputSize;
     *   int32_t function2OutputSize;
     *   uint32_t function2MinimumStackSize;
     *   uint8_t function1HasInput : 1;
     *   .
     *   .
     *   .
     * } IpCallPartition1 , *ptrIpCallPartition1;
     * 
     * #pragma pack(pop)
     * 
     * The application must configure this variable in the const since this is a read-only
     * section that the partition can use to call each others IP-calls (but cannot change them). The advantage of this
     * method is that the caller knows exactly the arguments it must pass to the caller and if they are incorrect the compiler
     * will issue a warning (or error) letting the programmer know immediately that the call is wrongly performed.
     */
    const ptrIpCallMinimum ipCall;
};

/**
 * restore the pack attribute to its previous value
 */
#pragma pack(pop)


/**
 * Initialize the minimal information for the IP-calls of the specified partition
 * 
 * @param partition partition to initialize
 */
void pharosIIpCallsInitializeMinimal(ptrPartition partition);


/**
 * Syscall for an Inter-partition call 
 * Performs the filter and if it allows the call to continue, performs the call
 * 
 * @param input input argument
 * @param output output argument
 * @param number number of the call to perform
 * 
 * @return returns the result of the inter-partition call.
 */
PharosIpCallR pharosSIpCall(void *input , void *output , uint32_t number , PartitionNumber partition);


/**
 * Internal function to call the IP call, including filters
 * 
 * @param call Inter-partition call to make
 * @param input pointer to the input argument
 * @param output pointer to the output argument
 * @param runningThread calling thread
 * 
 * @return returns the result of the filter+function
 */
PharosIpCallR pharosIIpCall(ptrIpCall call , void *input , void *output , ptrThreadInfo runningThread);


/**
 * Internal function to call the IP call, including filters
 * 
 * @param call IP-call to execute
 * @param runningThread running thread
 * @param newInput input in the shared stack
 * @param newOutput output on the shared stack
 * @param output original output on the user stack of the running thread
 * @param sharedStack shared stack in which to place the thread stack
 * @param wasInside flag indicating if the thread is already inside the running partition list of external threads 
 * @param original original partition of the running thread
 * @param originalInterPartitionContext context of this thread before this call was made
 * 
 * @return returns the ip-call result
 */
PharosIpCallR pharosIIpCallFunctionExecute(ptrIpCall call , ptrThreadInfo runningThread , uint8_t *newInput ,
                                           void *newOutput , void *output , ptrStack sharedStack ,
                                           bool *wasInside , ptrPartition original ,
                                           ptrPharosCpuInterPartitionContext originalInterPartitionContext);

#endif /* IPCALL_H */
