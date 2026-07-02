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
#include <pharos/kernel/memory/memory.h>
#include <pharos/kernel/assert/assert.h>
#include <pharos/hal/boardApi.h>


void *pharosIStackAllocate(ptrStack stack , size_t size)
{
    /* address that will be allocated on the stack */
    void *result;

    /* stack address */
    uint8_t *s = stack->stack;

    /* calculate the top of the stack */
    uint8_t *end = stack->stack + stack->size;


    /* reserve the space (stack grows down) */
    s -= size;

    /* mark the address that was reserved */
    result = end - size;

    /* align the stack */
    end = result - pharosIAlignmentPreviousUnbound(result , PHAROS_STACK_ALIGNMENT);

    /* set the thread to start at this address */
    stack->size = end - stack->stack - PHAROS_STACK_ALIGNMENT;

    /* return the new stack pointer which is the address of the allocated space */
    return end;
}


void pharosStackReserve(const ptrStack stack , uint32_t size)
{
    /* stack is descending so decrease the stack pointer when reserving */
    stack->stack -= size;

    /* and decrease the size */
    stack->size -= size;
}


void pharosStackFree(const ptrStack stack , uint32_t size)
{
    /* stack is descending so increase the stack pointer when freeing the stack */
    stack->stack += size;

    /* and increase the size */
    stack->size += size;
}


uintptr_t pharosIStackUsage(ptrStack stack , bool exactMethod)
{
    /* stack top */
    uintptr_t top = (uintptr_t) pharosCpuInitializeStackPointer(stack->stack , stack->size);

    /* result of the method*/
    uintptr_t result;


    if(exactMethod == TRUE)
    {
        /* iterator for the exact method */
        uintptr_t i;

        /* start from the base of the stack until the top */
        for(i = (uintptr_t) stack->stack; i < top; i += 8U)
        {
            /* check if the pattern is there */
            if(*(uint64_t *) i != stack->pattern)
            {
                /* if not, then stop looking */
                break;
            }
        }

        /* we found at i the last place that as the pattern */
        result = top - i;
    }
    else
    {
        /* top iterator */
        uintptr_t topIterator = top;

        /* get the beginning of the stack */
        uintptr_t bottomIterator = (uintptr_t) stack->stack;

        /* get the middle searching point */
        uintptr_t middle;


        /* while we have not reached the end of the search. We have to leave always 8 bytes */
        while(bottomIterator + 8U < topIterator)
        {
            /* search again */
            middle = (topIterator + bottomIterator) / 2U;

            /* if middle - bottom is not aligned to 8 bytes (size of the pattern) */
            if((middle - bottomIterator) % 8U != 0U)
            {
                /* make sure middle - bottom is aligned to 8 bytes */
                middle = middle - (middle - bottomIterator) % 8U;
            }

            /* if the middle iterator we did not find the pattern */
            if(*(uint64_t *) middle != stack->pattern)
            {
                /* then lower the top to the middle */
                topIterator = middle;
            }
                /* if we did find the pattern */
            else
            {
                /* raise the bottom */
                bottomIterator = middle;
            }
        }

        /* calculate the stack usage */
        result = top - bottomIterator - 8U;
    }


    /* return the stack usage */
    return result;
}
