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
#include <pharos/kernel/core/core.h>
#include <pharos/hal/cpu/conf.h>


/**
 * Copy bytes unaligned 
 * 
 * @param d destination
 * @param s source
 * @param size number of bytes to copy
 */
static void copyUnalignedBytes(uint8_t *d , const uint8_t *s , size_t size);


void *memcpy(void *dest , const void *src , size_t size)
{
    /* iterator of the number of bytes left to copy */
    size_t i;

    /* copy to a uint8_t pointer */
    uint8_t *d = dest;

    /* copy to a uint8_t pointer */
    const uint8_t *s = src;

    /* check if both pointers can be aligned */
    intptr_t alignment = pharosIAlignmentNextBothBound(dest , src , PHAROS_CPU_COPY8_ALIGN , size);


    /* if completely aligned and enough to copy at least one 8-byte word */
    if((size >= 8U) && (alignment == 0))
    {
        /* get number of 8-bytes to copy */
        i = size / 8U;

        /* copy 8 bytes at a time */
        /* MISRA: we know that now s has the required alignment for 8 bytes */
        /* MISRA: we know that now d has the required alignment for 8 bytes */
        pharosICopy64((uint64_t *) (d) , (uint64_t *) (s) , i);

        /* update the number of bytes copied */
        i *= 8U;

        /* copy all the bytes left to copy */
        copyUnalignedBytes(d + i , s + i , size - i);
    }
        /* If copying and if the addresses are aligned to the minimum required by the CPU for 8 bytes copy and 
         * the size is large enough to justify using at least one 8-copy */
    else if((size >= 2 * 8U) && (alignment >= 0))
    {
        /* first copy unaligned bytes */
        copyUnalignedBytes(d , s , alignment);

        /* decrease the number of bytes to copy */
        size -= alignment;

        /* get the number of 8-bytes to copy */
        i = size / 8U;

        /* copy 8 bytes at a time */
        /* MISRA: we know that now s has the required alignment for 8 bytes */
        /* MISRA: we know that now d has the required alignment for 8 bytes */
        pharosICopy64((uint64_t *) (d + alignment) , (uint64_t *) (s + alignment) , i);

        /* get the number of bytes copied */
        i *= 8U;

        /* update the number of bytes missing to be copied */
        size -= i;

        d += i + alignment;
        s += i + alignment;

        /* copy all the bytes left to copy */
        copyUnalignedBytes(d , s , size);
    }
        /* addresses are unaligned (to 8-byte copy) or there are too few bytes to copy */
    else
    {
        /* copy all the bytes unaligned byte by byte */
        copyUnalignedBytes(d , s , size);
    }

    /* return the destination address */
    return dest;
}


void pharosICopy64(uint64_t *dst , const uint64_t *src , uintptr_t size)
{
    /**
     * This function must be on a separate file because if compiled with -O2, gcc (version 6.3.0) and ld (version 2.27) 
     * optimize this incorrectly and with a size = 0 it still enters in the "for" loop (same happens with a "while" loop). 
     */

    /* index on the table */
    uintptr_t i;


    /* while there are bytes left to copy */
    for(i = 0U; i < size; i++)
    {
        /* copy 8 bytes at a time */
        dst[i] = src[i];
    }
}


void pharosIZeros64(uint64_t *start , uintptr_t size)
{
    /**
     * This function must be on a separate file because if compiled with -O2, gcc (version 6.3.0) and ld (version 2.27) 
     * optimize this incorrectly and with a size = 0 it still enters in the "for" loop (same happens with a "while" loop). 
     */

    /* index on the table */
    uintptr_t i;


    /* while there are bytes left to zero-out */
    for(i = 0U; i < size; i++)
    {
        /* zero 8 bytes at a time */
        start[i] = 0U;
    }
}


void pharosISet64(uint64_t *dst , uint64_t src , uintptr_t size)
{
    /* index on the table */
    uintptr_t i;


    /* while there are bytes left to copy */
    for(i = 0U; i < size; i++)
    {
        /* copy 8 bytes at a time */
        dst[i] = src;
    }
}


void copyUnalignedBytes(uint8_t *d , const uint8_t *s , size_t size)
{
    /* iterator of the number of bytes left to copy */
    size_t i = 0;


    /* for all the bytes left to copy */
    while(i < size)
    {
        /* copy byte by byte */
        d[i] = s[i];

        /* increment the byte index to copy */
        i++;
    }
}
