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
#include <pharos/kernel/object/declarations.h>


static void copyUnalignedBytes(uint8_t *d , const uint8_t *s , intptr_t size)
{
    /* decrease the size (we start at minus 1 and go to 0) */
    size--;

    /* until there are no more bytes left to copy */
    while(size >= 0)
    {
        /* copy byte by byte (in reverse order) */
        d[size] = s[size];

        /* decrement the byte index to copy */
        size--;
    }
}


void rtcMemCopyReverse8(void *dest , const void *src , size_t size)
{
    /* iterator of the number of bytes left to copy */
    intptr_t i = size;

    /* copy to a uint8_t pointer */
    uint8_t *d = (uint8_t *) dest;

    /* copy to a uint8_t pointer */
    const uint8_t *s = (uint8_t *) src;

    /* index on the table */
    intptr_t j;

    /* check if both pointers can be aligned */
    intptr_t alignment = pharosIAlignmentPreviousBothSizeBound(d + size , s + size , PHAROS_CPU_COPY8_ALIGN , size);


    /* and if the addresses are aligned to the minimum required by the CPU for 8 bytes copy */
    if(alignment >= 0)
    {
        /* MISRA: we already checked that the address is aligned */
        uint64_t *u64dst = (uint64_t *) (d + size - alignment);

        /* MISRA: we already checked that the address is aligned */
        const uint64_t *u64src = (const uint64_t *) (s + size - alignment);

        /* copy all the bytes left */
        copyUnalignedBytes(&d[size - alignment] , &s[size - alignment] , alignment);

        /* while there are bytes left to copy. Note: have to start copy with negative indexes since the pointers point
         * to the end of the array to copy (and must start at -1 since the address at index 0 is outside) */
        for(j = -1; j > -1 - (intptr_t) (size / 8U); j--)
        {
            /* copy 8 bytes at a time (in reverse order) */
            u64dst[j] = u64src[j];
        }

        /* get the number of bytes copied */
        i = size - (size / 8U)*8U - alignment;

        d += i;
        s += i;
    }

    /* copy all the bytes left */
    copyUnalignedBytes(d , s , i);
}
