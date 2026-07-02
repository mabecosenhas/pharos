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


static void *searchBackwardsUnalignedByte(const uint8_t *start , const uint8_t *end , uint8_t c)
{
    /* result of the method */
    void *result = NULL;


    /* while we did not reach the start of the array */
    while(start <= end)
    {
        /* if we found the char */
        if(*end == c)
        {
            /* return it */
            result = (void *) end;

            /* stop looking */
            break;
        }
        else
        {
            /* decrease the pointer */
            end--;
        }
    }

    /* return the array or NULL if did not find it */
    return result;
}


void *rtcMemSearchReverse8(const void *array , uint8_t c , size_t length)
{
    void * result;

    /* cast the array to an array of chars */
    uint8_t *end = (uint8_t *) array;

    /* calculate the last byte to search for */
    uint8_t *start = end;

    /* cast the array to an array of uint64_t */
    uint64_t *u64;

    /* calculate the mask to search 8 bytes at a time */
    uint64_t mask;

    /* alignment required to align array */
    intptr_t alignment;


    /* start at the last byte to search for */
    end = end + length - 1;

    /* calculate number of unaligned bytes */
    alignment = pharosIAlignmentPreviousBound(end , PHAROS_CPU_COPY8_ALIGN , length);


    /* if the bytes can be aligned (within n) */
    if(alignment >= 0)
    {
        /* search unaligned backwards */
        result = searchBackwardsUnalignedByte(start , end , c);

        /* if found something */
        if(result != NULL)
        {
            /* return it */
            return result;
        }

        /* MISRA: we know that now u64 has the required alignment for 8 bytes */
        u64 = (uint64_t *) (end - alignment);

        /* build the mask to search for */
        mask = pharosU64BuildMask(c);

        /* loop 8 bytes at a time */
        while(length >= 8U)
        {
            /* if the char is inside the 8 byte variable */
            if(pharosU64ContainsMask(*u64 , mask) != 0)
            {
                /* stop searching (don't update the u64 since the byte is here) */
                break;
            }
            else
            {
                /* search the previous 8 bytes */
                u64--;

                /* decrease the number of bytes to search for */
                length -= 8U;
            }
        }

        /* update the pointer */
        end = (uint8_t *) u64;
    }

    /* search the remaining unaligned bytes backwards */
    return searchBackwardsUnalignedByte(start , end , c);
}
