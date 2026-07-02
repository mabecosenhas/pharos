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


static size_t searchUnaligned(const char *s , const char *end);


size_t strnlen(const char *s , size_t maxSize)
{
    /* return value of this method */
    size_t result = 0U;

    /* calculate number of unaligned bytes */
    intptr_t alignment = pharosIAlignmentNextBound((uint8_t *) s , PHAROS_CPU_COPY8_ALIGN , maxSize);

    /* calculate the end of the string */
    const char *end;


    /* if can be aligned within maxSize */
    if(alignment >= 0)
    {
        /* calculate the end of the string aligned */
        end = s + alignment;

        /* search the unaligned bytes */
        result = searchUnaligned(s , end);

        /* if found the '\0' */
        if(result != 0)
        {
            /* return the number of bytes searched for */
            return result;
        }

        /* update the number of bytes searched */
        result = alignment;

        /* MISRA: we know that now u64 has the required alignment for 8 bytes */
        const uint64_t *u64 = (uint64_t *) end;

        /* update the end of the string */
        end = s + maxSize;

        /* search for the termination on the 8-byte word until the end of the string */
        while(((uintptr_t) u64 < (uintptr_t) (end - 8U)) && (pharosU64ContainsTermination(*u64) == 0))
        {
            /* increment the 8 byte iterator */
            u64++;
        }

        /* update the number of chars searched */
        result += ((uintptr_t) u64 - (uintptr_t) (s + alignment));

        /* cast again to char pointer */
        s = (char *) u64;

    }

    /* find out the length of the remaining string */
    while((*s != '\0') && (result < maxSize))
    {
        /* increment the pointer */
        s++;

        /* increment the size */
        result++;
    }

    /* return the number of bytes until the '\0' char */
    return result;
}


size_t searchUnaligned(const char *s , const char *end)
{
    const char *aux = s;

    while(s < end)
    {
        if(*s == '\0')
        {
            /* return the number of bytes searched */
            return s - aux;
        }
        else
        {
            s++;
        }
    }

    /* did not find end of string */
    return 0;
}