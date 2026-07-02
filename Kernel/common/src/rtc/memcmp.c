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


#include <pharos/hal/hal.h>
#include <pharos/kernel/object/declarations.h>
#include <pharos/hal/cpu/conf.h>
#include <pharos/kernel/memory/memory.h>


/**
 * Number of bytes than can be copied at each time by the CPU 
 * (assume that anything less than 8 has to be copied byte by byte)
 */
#define MEMCPY_THRESHOLD                    (8U)


int memcmp(const void *a , const void *b , size_t size)
{
    /* unsigned 8 bit integer pointer to address a */
    const uint8_t *first = a;

    /* unsigned 8 bit integer pointer to address b */
    const uint8_t *second = b;


    /* if the addresses are aligned to the minimum required by the CPU for 8 bytes */
    if((size > MEMCPY_THRESHOLD) && (pharosIIsAligned(first , PHAROS_CPU_COPY8_ALIGN) == TRUE) && (pharosIIsAligned(second , PHAROS_CPU_COPY8_ALIGN) == TRUE))
    {
        /* MISRA: we already checked that the pointer is aligned for 8 bytes */
        const uint64_t *u64f = (const uint64_t *) first;

        /* MISRA: we already checked that the pointer is aligned for 8 bytes */
        const uint64_t *u64s = (const uint64_t *) second;


        /* while there are many bytes to compare */
        while(size > MEMCPY_THRESHOLD)
        {
            /* if the 8-bytes are not equal */
            if(*u64f != *u64s)
            {
                /* stop searching, */

                /* update the first pointer */
                first = (const uint8_t *) u64f;

                /* update the second pointer */
                second = (const uint8_t *) u64s;

                /* now we will search byte by byte for which is bigger/smaller */
                break;
            }

            /* update the number of bytes searched */
            size -= 8U;
        }
    }

    /* loop for the size number of bytes */
    for(; size > 0; size-- , first++ , second++)
    {
        /* check if the bytes are equal */
        if(*first != *second)
        {
            /* return the difference */
            return *first - *second;
        }
    }

    /* both arrays are equal */
    return 0;
}
