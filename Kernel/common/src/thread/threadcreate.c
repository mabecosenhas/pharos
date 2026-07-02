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


#include <pharos/kernel/thread/thread.h>
#include <pharos/hal/boardApi.h>
#include <pharos/kernel/interrupt/interrupt.h>
#include <pharos/kernel/object/protection.h>
#include <pharos/kernel/object/object.h>
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/kernel/scheduler/schedulerinline.h>
#include <pharos/kernel/assert/assert.h>


PharosThreadPeriodicCreateR pharosIThreadCreate(ptrThreadInfo *thread , uint32_t sizeofTcb ,
                                                const ptrPharosThreadProperties properties , size_t userStackAllocateSize)
{
    /* result of the method, start with successfully allocated all memory (will be placed with the error if not able) */
    PharosThreadPeriodicCreateR result = PHAROS_THREAD_PERIODIC_CREATE_SUCCESS;

    /* get the running partition */
    ptrPartition partition = pharosIPartitionGetRunning();

    /* kernel stack pointer of the newly created thread */
    uint8_t *kernelStack;

    /* user stack pointer of the newly created thread */
    uint8_t *userStack;

    /* shared stack pointer of the newly created thread */
    uint8_t *sharedStack = NULL;

    /* shared stack of the newly created thread */
    ptrStackShared shared;

    /* calculate the size of the kernel area */
    uint32_t kernelArea = properties->kernelStackSize + sizeofTcb + 2 * PHAROS_STACK_ALIGNMENT + 2 * STRUCTURE_ALIGNMENT;

    /* kernel area pointer */
    uint8_t *kernel;

    /* alignment for the stack */
    intptr_t alignment;

    /* interrupt level */
    PharosInterruptLevel level;


    /* if has shared stack */
    if(properties->sharedStackSize != 0)
    {
        /* then also allocate the structure StackShared */
        kernelArea += sizeof (StackShared);
    }

    /* allocate the whole kernel memory area */
    kernel = pharosIMallocKernel(kernelArea);

    /* if attempting to create with an invalid kernel stack memory */
    if(properties->kernelStackSize < PHAROS_STACK_KERNEL_MIN_SIZE)
    {
        /* return error */
        result = PHAROS_THREAD_PERIODIC_CREATE_INVALID_KERNEL_STACK;
    }
        /* otherwise try to allocate all the necessary memory */
    else
    {
        /* if able to allocate the full kernel memory area */
        if(kernel != NULL)
        {
            /* allocate the user stack */
            userStack = pharosSMalloc(properties->userStackSize + PHAROS_DYNAMIC_STACK_ALIGN(properties->userStackSize));

            /* if the user stack was allocated */
            if(userStack != NULL)
            {
                /* continue to allocate the shared stack if needed */
                if(properties->sharedStackSize != 0)
                {
                    /* try to allocate memory for shared stack, correct the stack size (to account for 4 KiB page alignments, 
                     * MPU regions aligned to power of 2, etc */
                    sharedStack = pharosIMallocSharedStack(PHAROS_SHARED_STACK_CORRECT_SIZE(properties->sharedStackSize) +
                                                           PHAROS_DYNAMIC_STACK_ALIGN(properties->sharedStackSize));

                    /* if could not allocate memory */
                    if(sharedStack == NULL)
                    {
                        /* free previously allocated TCB memory */
                        pharosIFreeKernel(kernel);

                        /* free the user stack */
                        pharosFreeS(userStack);

                        /* no shared stack memory */
                        result = PHAROS_THREAD_PERIODIC_CREATE_NO_SHARED_STACK_MEMORY;
                    }
                    /* else, every memory required was allocated */
                }
            }
            else
            {
                /* free previously allocated kernel memory */
                pharosIFreeKernel(kernel);

                /* not enough user memory */
                result = PHAROS_THREAD_PERIODIC_CREATE_NO_USER_MEMORY;
            }
        }
        else
        {
            /* not enough kernel memory */
            result = PHAROS_THREAD_PERIODIC_CREATE_NO_KERNEL_MEMORY;
        }
    }

    /* if was able to create all the memory */
    if(result == PHAROS_THREAD_PERIODIC_CREATE_SUCCESS)
    {
        /* disable interrupts: cannot allow an interrupt or another thread to change the partition->threadsByName at
         * the same time */
        pharosCpuDisableInterrupts(&level);

        /* check if there is a thread with the same name */
        if(treeMapContains(&partition->threadsByName , properties->name) == TRUE)
        {
            /* re-enable interrupts */
            pharosCpuEnableInterrupts(&level);

            /* free any allocated memory */

            /* free previously allocated TCB memory */
            pharosIFreeKernel(kernel);

            /* free the user stack */
            pharosFreeS(userStack);

            /* if there was shared stack */
            if(properties->sharedStackSize != 0)
            {
                /* free the shared stack */
                pharosIFreeSharedStack(sharedStack);
            }

            /* return error */
            result = PHAROS_THREAD_PERIODIC_CREATE_ALREADY_CREATED;
        }
        else
        {
            /* split the kernel memory area into the TCB, kernel stack and shared (if needed) */

            /* set the TCB pointer (correctly aligned) */
            *thread = (ptrThreadInfo) (kernel + pharosIAlignmentNextUnbound(kernel , STRUCTURE_ALIGNMENT));

            /* set the size of the user stack to allocate */
            (*thread)->userStackAllocateSize = userStackAllocateSize;

            /* increase by the size of the TCB */
            kernel = ((uint8_t *) (*thread)) + sizeofTcb;

            /* copy the name (including the '/0') */
            memcpy((*thread)->nameInternal , properties->name , strnlen(properties->name , PHAROS_THREAD_NAME_MAX - 1U) + 1U);

            /* set the name address */
            (*thread)->name = (*thread)->nameInternal;

            /* then put it on the map */
            treeMapPut(&partition->threadsByName , (*thread)->nameInternal , *thread);

            /* set the shared pointer (even if not used, there is no harm in placing it) */
            shared = (ptrStackShared) (kernel + pharosIAlignmentNextUnbound(kernel , STRUCTURE_ALIGNMENT));

            /* update the kernel pointer */
            kernel = ((uint8_t *) (shared)) + sizeof (StackShared);

            /* set the kernel stack */
            kernelStack = kernel;

            /* initialize the kernel stack pointer */
            alignment = pharosIAlignmentNextUnbound(kernelStack , PHAROS_STACK_ALIGNMENT);

            (*thread)->kernelStack.stack = kernelStack + alignment;

            /* initialize the kernel stack size */
            (*thread)->kernelStack.size = properties->kernelStackSize +
                    pharosIAlignmentNextUnbound((*thread)->kernelStack.stack + properties->kernelStackSize , PHAROS_STACK_ALIGNMENT);

            /* copy the stack pattern */
            (*thread)->kernelStack.pattern = properties->stackPattern;

            /* re-enable interrupts */
            pharosCpuEnableInterrupts(&level);



            /* initialize the user stack pointer */
            alignment = pharosIAlignmentNextUnbound(userStack , PHAROS_DYNAMIC_STACK_ALIGN(properties->userStackSize));

            /* align the start of the stack */
            (*thread)->applicationStack.stack = userStack + alignment;

            /* calculate the correct size of the stack */
            (*thread)->applicationStack.size = properties->userStackSize +
                    pharosIAlignmentNextUnbound((*thread)->applicationStack.stack + properties->userStackSize , PHAROS_DYNAMIC_STACK_ALIGN(properties->userStackSize));

            /* copy the stack pattern */
            (*thread)->applicationStack.pattern = properties->stackPattern;

            /* initialize the thread priority */
            (*thread)->priority = properties->basePriority;

            /* initialize the thread WCET */
            (*thread)->wcet = properties->budget;

            /* initialize the thread replenishment interval */
            (*thread)->replenishBudgetInterval = properties->replenishmentPeriod;

            /* if the shared stack exists */
            if(properties->sharedStackSize != 0)
            {
                /* set the pointer to the shared stack */
                (*thread)->sharedAccessStack = shared;

                /* calculate the alignment */
                alignment = pharosIAlignmentNextUnbound(sharedStack , PHAROS_DYNAMIC_STACK_ALIGN(properties->sharedStackSize));

                shared->stack.stack = sharedStack + alignment;

                shared->stack.size = properties->sharedStackSize +
                        pharosIAlignmentNextUnbound(shared->stack.stack + properties->sharedStackSize , PHAROS_DYNAMIC_STACK_ALIGN(properties->sharedStackSize));

                /* copy the stack pattern */
                shared->stack.pattern = properties->stackPattern;
            }
            else
            {
                /* reset the shared access stack */
                (*thread)->sharedAccessStack = NULL;
            }
        }
    }

    /* return success or the error */
    return result;
}


PharosThreadPeriodicCreateR pharosIThreadPropertiesValidate(ptrPharosThreadProperties properties , ptrThreadId id , uint32_t sizeofTcb)
{
    PharosThreadPeriodicCreateR result;

    /* get the running core */
    ptrCore core = pharosICore();


    /* check if the properties are at a valid address */
    if(pharosCpuBufferIsInsideProtection((uint8_t *) properties , sizeofTcb , pharosIPartitionRunningMemProt()) == FALSE)
    {
        /* return invalid properties */
        result = PHAROS_THREAD_PERIODIC_CREATE_INVALID_PROPERTIES;
    }
        /* if using a flat scheduler then check the priority of thread to be created (must use a "<=" instead of a
         *  "<" in case the application did not define the maxDynamicPriority and its default is 0) */
    else if(pharosISchedulerIsHierarchical() == FALSE && properties->basePriority <= core->maxDynamicPriority)
    {
        /* return invalid scheduler */
        result = PHAROS_THREAD_PERIODIC_CREATE_INVALID_PRIORITY;
    }
        /* if the id is not a valid address */
    else if(pharosCpuBufferIsInsideProtection((uint8_t *) id , sizeof (ThreadId) , pharosIPartitionRunningMemProt()) == FALSE)
    {
        /* return invalid id */
        result = PHAROS_THREAD_PERIODIC_CREATE_INVALID_ID;
    }
        /* if the kernel stack size is too little */
    else if(properties->kernelStackSize < PHAROS_STACK_KERNEL_MIN_SIZE)
    {
        /* return invalid kernel stack */
        result = PHAROS_THREAD_PERIODIC_CREATE_INVALID_KERNEL_STACK;
    }
        /* if the user stack is too small */
    else if(properties->userStackSize < PHAROS_STACK_MIN_SIZE)
    {
        /* return invalid user stack */
        result = PHAROS_THREAD_PERIODIC_CREATE_INVALID_USER_STACK;
    }
        /* if there is a shared stack and if it is too small */
    else if(properties->sharedStackSize != 0 && properties->sharedStackSize < PHAROS_SHARED_STACK_SIZE_MINIMUM_SIZE)
    {
        /* return invalid shared stack */
        result = PHAROS_THREAD_PERIODIC_CREATE_INVALID_SHARED_STACK;
    }
        /* check if the name belongs to the running partition and if it has less than PHAROS_THREAD_NAME_MAX chars */
    else if(pharosCpuStringIsInsideProtection(properties->name , PHAROS_THREAD_NAME_MAX , pharosIPartitionRunningMemProt()) == FALSE)
    {
        /* return invalid name */
        result = PHAROS_THREAD_PERIODIC_CREATE_INVALID_NAME;
    }
        /* if setting a WCET and the replenishment is 0 */
    else if(properties->budget |= PHAROS_WCET_UNLIMITED && properties->replenishmentPeriod == 0U)
    {
        /* return invalid period */
        result = PHAROS_THREAD_PERIODIC_CREATE_INVALID_PERIOD;
    }
        /* if every check was successful */
    else
    {
        /* return success */
        result = PHAROS_THREAD_PERIODIC_CREATE_SUCCESS;
    }

    /* return the result */
    return result;
}
