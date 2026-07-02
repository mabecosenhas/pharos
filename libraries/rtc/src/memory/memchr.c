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


#include <pharos/rtc/rtc.h>
#include <pharos/kernel/memory/memory.h>
#include <pharos/hal/cpu/conf.h>
#include <pharos/rtc/rtcinline.h>


static void *searchUnalignedByte(const uint8_t *array , uint8_t c , size_t length)
{
    /* result of the method */
    void *result = NULL;

    /* calculate the last byte to search for */
    const uint8_t *end = array + length;


    /* while we did not reach the end of the array */
    while(array < end)
    {
        /* if we found the char */
        if(*array == c)
        {
            /* return it */
            result = (void *) array;

            /* stop looking */
            break;
        }
        else
        {
            /* increase the pointer */
            array++;
        }
    }

    /* return the array or NULL if did not find it */
    return result;
}


void *rtcMemSearch8(const void *array , uint8_t c , size_t length)
{
    void *result;

    /* calculate the mask to search 8 bytes at a time */
    uint64_t mask;

    /* calculate number of unaligned bytes */
    intptr_t alignment = pharosIAlignmentNextBound(array , PHAROS_CPU_COPY8_ALIGN , length);

    /* have to convert to an signed integer to decrease it safely */
    intptr_t missing = length;


    /* if the bytes can be aligned (within n) */
    if(alignment >= 0)
    {
        /* MISRA: we know that now d has the required alignment for 8 bytes */
        uint64_t *d = (uint64_t *) (array + alignment);

        /* search for the byte in the unaligned array */
        result = searchUnalignedByte(array , c , alignment);

        /* if found the byte */
        if(result != NULL)
        {
            /* then return it right now */
            return result;
        }

        /* build the mask to search for */
        mask = pharosU64BuildMask(c);

        /* update number of bytes left to search */
        missing -= alignment;

        /* loop 8 bytes at a time */
        while(missing >= 8)
        {
            /* if the char is inside the 8 byte variable */
            if(pharosU64ContainsMask(*d , mask) != 0)
            {
                /* stop searching (don't update the u64 since the byte is here) */
                break;
            }
            else
            {
                /* search the next 8 bytes */
                d++;

                /* decrease the number of bytes left to search */
                missing -= 8U;
            }
        }

        /* update the pointer */
        array = (uint8_t *) d;
    }

    /* search for the byte in the unaligned array */
    return searchUnalignedByte(array , c , missing);
}
