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
#include <pharos/rtc/rtcinline.h>
#include <pharos/kernel/memory/memory.h>
#include <pharos/hal/cpu/conf.h>


static bool searchUnaligned(const char *s , const char *end , size_t *result)
{
    /* save the start of the array */
    const char *aux = s;


    /* while we have not reached the end (max number of bytes) */
    while(s < end)
    {
        /* search for the termination */
        if(*s == '\0')
        {
            /* found it, return the number of bytes searched */
            *result += s - aux;

            /* found it */
            return TRUE;
        }
        else
        {
            /* increment the pointer */
            s++;
        }
    }

    /* did not find end of string */
    return FALSE;
}


size_t rtcStringLength(const char *s)
{
    /* return value of this method */
    size_t result = 0U;

    /* calculate number of unaligned bytes */
    intptr_t alignment = pharosIAlignmentNextUnbound(s , PHAROS_CPU_COPY8_ALIGN);

    const uint64_t *u64;


    /* if not already aligned */
    if(alignment != 0)
    {
        /* search unaligned the first bytes , if found the termination */
        if(searchUnaligned(s , s + alignment , &result) != 0)
        {
            /* return it right away */
            return result;
        }

        /* increase the pointer */
        s += alignment;

        /* update the number of bytes searched */
        result = alignment;
    }

    /* MISRA: we know that now u64 has the required alignment for 8 bytes */
    u64 = (uint64_t *) s;

    /* search for the termination 8 bytes at a time */
    while(pharosU64ContainsTermination(*u64) == 0)
    {
        /* increment the 8-byte pointer */
        u64++;
    }

    /* update the number of chars searched */
    result += ((uintptr_t) u64 - (uintptr_t) s);

    /* cast again to char pointer */
    s = (char *) u64;

    /* search in the last bytes where the termination is (must be in the next 8 bytes) */
    searchUnaligned(s , s + 8U , &result);

    /* return the number of bytes until the '\0' char */
    return result;
}
