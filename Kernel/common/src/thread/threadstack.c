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


#include <pharos/kernel/thread/stack.h>
#include <pharos/thread.h>
#include <pharos/kernel/thread/thread.h>
#include <pharos/kernel/core/core.h>


/**
 * Fills the stack with the pre-defined pattern (if any selected)
 * 
 * @param stack stack to initialize
 */
static void fillStack(ptrStack stack);


ptrStack pharosIStackSharedMemory(ptrStackShared stack)
{
    /* result of the method */
    ptrStack result;

    /* if there is no shared stack */
    if(stack == NULL)
    {
        /* then return invalid */
        result = NULL;
    }
    else
    {
        /* return the stack itself (it will be the same for all environments */
        result = &stack->stack;
    }

    /* return the shared stack or invalid */
    return result;
}


void pharosIThreadStackInit(ptrThreadInfo thread)
{
    /* initialize the user stack */
    fillStack(&thread->applicationStack);

    /* initialize the kernel stack */
    fillStack(&thread->kernelStack);

    /* if there is a shared stack */
    if(thread->sharedAccessStack != NULL)
    {
        /* initialize the shared stack */
        fillStack(&thread->sharedAccessStack->stack);
    }
}


PharosThreadStackR pharosSThreadStack(ptrThreadId id , bool exactMethod ,
                                      uintptr_t *userStackUsage ,
                                      uintptr_t *kernelStackUsage ,
                                      uintptr_t *sharedStackUsage)
{
    /* return value of this method */
    PharosThreadStackR result;

    /* thread with the specified name (if correct name) */
    ptrThreadInfo thread;


    /* check if the id address is valid */
    if(pharosCpuBufferIsInsideProtection((uint8_t *) id , sizeof (ThreadId) , pharosIPartitionRunningMemProt()) == FALSE)
    {
        /* address of id is invalid */
        result = PHAROS_THREAD_STACK_INVALID_ID_ADDRESS;
    }
    else if(pharosCpuBufferIsInsideProtection((uint8_t *) userStackUsage , sizeof (uintptr_t) , pharosIPartitionRunningMemProt()) == FALSE)
    {
        /* address of userStackUsage is invalid */
        result = PHAROS_THREAD_STACK_INVALID_USER_STACK;
    }
    else if(pharosCpuBufferIsInsideProtection((uint8_t *) kernelStackUsage , sizeof (uintptr_t) , pharosIPartitionRunningMemProt()) == FALSE)
    {
        /* address of kernelStackUsage is invalid */
        result = PHAROS_THREAD_STACK_INVALID_KERNEL_STACK;
    }
    else if(pharosCpuBufferIsInsideProtection((uint8_t *) sharedStackUsage , sizeof (uintptr_t) , pharosIPartitionRunningMemProt()) == FALSE)
    {
        /* address of sharedStackUsage is invalid */
        result = PHAROS_THREAD_STACK_INVALID_SHARED_STACK;
    }
    else
    {
        /* try to get the thread */
        thread = pharosIThreadGet(id);

        /* if there is a thread with such an id on the running partition */
        if(thread != NULL)
        {
            /* calculate the user stack usage */
            *userStackUsage = pharosIStackUsage(&thread->applicationStack , exactMethod);

            /* calculate the kernel stack usage */
            *kernelStackUsage = pharosIStackUsage(&thread->kernelStack , exactMethod);

            /* if there is a shared stack */
            if(thread->sharedAccessStack != NULL)
            {
                /* then calculate it */
                *sharedStackUsage = pharosIStackUsage(&thread->sharedAccessStack->stack , exactMethod);
            }
            else
            {
                /* otherwise it is not used */
                *sharedStackUsage = 0U;
            }

            /* return success */
            result = PHAROS_THREAD_STACK_SUCCESS;
        }
        else
        {
            /* could not find a thread with the corresponding id */
            result = PHAROS_THREAD_STACK_INVALID_ID;
        }
    }

    /* return the method result */
    return result;
}


void fillStack(ptrStack stack)
{
    /* if a pattern is set */
    if(stack->pattern != 0U)
    {
        /* fill the stack with the pattern */
        pharosISet64((uint64_t *) stack->stack , stack->pattern , stack->size / 8U);
    }
}


bool pharosIStackIsBufferInside(ptrStack stack , const uint8_t *buffer , size_t size)
{
    /* result of the method */
    bool result;

    /* end address of the buffer */
    const uint8_t *end = buffer + size;

    /* end address of the stack */
    uint8_t *stackEnd = stack->stack + stack->size;


    /* check if the buffer is inside the start and end of the stack */
    if((stack->stack <= buffer) && (stackEnd >= end))
    {
        /* then the buffer is inside */
        result = TRUE;
    }
        /* otherwise */
    else
    {
        /* buffer is not inside the stack */
        result = FALSE;
    }

    /* return the result */
    return result;
}


bool pharosIStackIsAddressInside(ptrStack stack , uint8_t *address)
{
    /* return value */
    bool result;

    /* end position of the stack */
    uint8_t *end = stack->stack + stack->size;


    /* check if it has access */
    if((stack->stack <= address) && (address <= end))
    {
        /* then the buffer is inside protection env */
        result = TRUE;
    }
        /* otherwise */
    else
    {
        /* address is not inside the stack */
        result = FALSE;
    }

    /* return the result */
    return result;
}


bool pharosIStackIsAddressInsideRunningStack(ptrThreadInfo thread , uint8_t *address)
{
    /* return value */
    bool result = FALSE;


    /* check if the thread is running inside another partition */
    if(pharosIThreadStateIsInterPartition(thread) == TRUE)
    {
        /* if the buffer is inside the thread shared stack */
        if((pharosIStackIsAddressInside(&thread->sharedAccessStack->stack , address) == TRUE))
        {
            /* then it has permissions */
            result = TRUE;
        }
    }
    else
    {
        /* if the buffer is inside the thread shared stack */
        if((pharosIStackIsAddressInside(&thread->applicationStack , address) == TRUE))
        {
            /* then it has permissions */
            result = TRUE;
        }
    }

    return result;
}
