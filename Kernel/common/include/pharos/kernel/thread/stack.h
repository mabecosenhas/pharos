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


#ifndef PHAROS_KERNEL_THREAD_STACK_H
#define PHAROS_KERNEL_THREAD_STACK_H

#include <pharos/kernel/thread/declarations.h>
#include <pharos/hal/hal.h>
#include <pharos/kernel/channel/channelbulk.h>
#include <pharos/kernel/future/future.h>

struct Stack
{
    /**
     * pointer to the stack used when accessing this resource
     * 
     * Note: application must define this in the section of the Resource owner (e.g. Partition 0 section)
     */
    StackPointer stack;

    /**
     * size of the stack
     */
    uint32_t size;

    /**
     * Initialization pattern to fill the stack with
     */
    uint64_t pattern;
};

struct StackShared
{
    /**
     * when using an MMU the a shared stack must be aligned to the MMU page and its size must be a multiple of the MMU page size
     */
    Stack stack;
};

struct ThreadLocalStorage
{
    /* nothing for now */
};

struct ThreadRegularStackUserSpace
{
    /* local storage */
    ThreadLocalStorage tls;
};

struct ThreadSporadicStackUserSpace
{
    /* local storage */
    ThreadLocalStorage tls;

    /* additional future id */
    FutureId future;

    /* additional channel bulk */
    ChannelBulk bulk;
};


/**
 * Get the stack pointer of the shared stack 
 * 
 * @constraint assumes the shared stack pointer is valid or is NULL
 * 
 * @param stack shared stack
 * 
 * @return returns the stack pointer
 */
ptrStack pharosIStackSharedMemory(ptrStackShared stack);


/**
 * Initialize the stacks of the thread with a pre-defined pattern (if not 0)
 * 
 * @param thread thread to initialize
 */
void pharosIThreadStackInit(ptrThreadInfo thread);


/**
 * Determines if the buffer is inside the stack
 * 
 * @param stack stack 
 * @param buffer start of the buffer
 * @param size size (in bytes) of the buffer
 * 
 * @return returns TRUE if the buffer is inside completely inside the stack and FALSE otherwise
 */
bool pharosIStackIsBufferInside(ptrStack stack , const uint8_t *buffer , size_t size);


/**
 * Check if the specified address is inside the stack
 * 
 * @param stack stack to check
 * @param address address to check
 * 
 * @return returns TRUE if the address is inside the stack and FALSE otherwise
 */
bool pharosIStackIsAddressInside(ptrStack stack , uint8_t *address);


/**
 * Determine the stack usage 
 * 
 * @param stack stack to determine
 * @param exactMethod flag to indicate if the method is exact (slow) or fast (quick but inaccurate) 
 * 
 * @return returns the stack usage (in bytes(
 */
uintptr_t pharosIStackUsage(ptrStack stack , bool exactMethod);


/**
 * Determines if the address is inside the running thread stack (or shared stack)
 * 
 * @param thread thread to analyze the stack
 * @param address address to check
 * 
 * @return returns TRUE if the address is inside the thread running stack or FALSE otherwise
 */
bool pharosIStackIsAddressInsideRunningStack(ptrThreadInfo thread , uint8_t *address);


#endif /* PHAROS_KERNEL_THREAD_STACK_H */
