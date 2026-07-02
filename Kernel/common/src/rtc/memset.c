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


#include <pharos/kernel/memory/memory.h>
#include <pharos/hal/cpu/conf.h>


/**
 * Sets the array s with n bytes
 * 
 * @note assumes the array is unaligned and the size is small enough that an alignment is not needed/performant
 * 
 * @param s array to set
 * @param c byte to set with
 * @param n number of bytes to set
 */
static void setUnalignedByte(uint8_t *s , uint8_t c , size_t n);


void *memset(void *s , int c , size_t n)
{
    /* aux variable to store the number of bytes to copy */
    intptr_t aux;

    /* calculate number of unaligned bytes */
    intptr_t alignment = pharosIAlignmentNextBound(s , PHAROS_CPU_COPY8_ALIGN , n);


    /* if the bytes can be aligned (within n) */
    if(alignment >= 0)
    {
        /* copy first unaligned bytes */
        setUnalignedByte(s , c , alignment);

        /* update s */
        s += alignment;

        /* update number of bytes left to copy */
        n -= alignment;

        /* calculate the number of 8 bytes to set */
        aux = n / 8U;

        /* set 8 bytes at a time */
        /* MISRA: we know that now s has the required alignment for 8 bytes */
        pharosISet64((uint64_t *) s , pharosU64BuildMask(c) , aux);

        aux *= 8U;

        /* update the pointer */
        s += aux;

        /* update the number of bytes left to set */
        n -= aux;
    }

    /* copy last unaligned bytes */
    setUnalignedByte(s , c , n);

    /* returns the same pointer */
    return s;
}


void setUnalignedByte(uint8_t *s , uint8_t c , size_t n)
{
    /* calculate end of the array */
    uint8_t *end = s + n;


    /* copy remaining bytes left */
    while(s < end)
    {
        /* copy one byte at a time */
        *s = c;

        /* increment number of bytes copied */
        s++;
    }
}
