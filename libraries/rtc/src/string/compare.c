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
#include <pharos/rtc/rtcinternal.h>
#include <pharos/declarations.h>
#include <pharos/hal/cpu/conf.h>
#include <pharos/kernel/memory/memory.h>
#include <pharos/rtc/rtcinline.h>


static int32_t compareUnalignedString(const char *s1 , const char *s2)
{
    /* check until alignment is complete */
    while(*s1 == *s2)
    {
        /* if we have reached the end of the string */
        if(*s1 == '\0')
        {
            /* stop looking */
            break;
        }

        /* increment the first iterator */
        s1++;

        /* increment the second iterator */
        s2++;
    }

    /* return the difference of the last char looked (either '\0' or they are different) */
    return *s1 - *s2;
}


intptr_t rtcStringCompare(const char *s1 , const char *s2)
{
    /* flag */
    bool ended;

    /* return value of the method */
    intptr_t result;

    /* 64-bit pointer for faster search */
    uint64_t *u64s1;

    /* 64-bit pointer for faster search */
    uint64_t *u64s2;

    /* check if there is a possibility to align both pointers (within the size) and calculate how many 
     * bytes to get there */
    intptr_t alignment = pharosIAlignmentNextBothUnbound(s1 , s2 , PHAROS_CPU_COPY8_ALIGN);


    /* if there is a possibility for alignment */
    if(alignment >= 0)
    {
        /* compares the unaligned string */
        result = rtcStringCompareUnalignedSize(s1 , s2 , alignment , &ended);

        /* if search ended */
        if(ended == TRUE)
        {
            /* return the result */
            return result;
        }

        /* MISRA: we know that now u64s1 is aligned to 8 bytes */
        u64s1 = (uint64_t *) (s1 + alignment);

        /* MISRA: we know that now u64s2 is aligned to 8 bytes */
        u64s2 = (uint64_t *) (s2 + alignment);

        /* now both u64s1 and u64s2 are aligned */

        /* while there are at least 8 bytes to search for and a termination ('\0') is not there */
        while((*u64s1 == *u64s2) && (pharosU64ContainsTermination(*u64s1) == 0))
        {
            /* increment first iterator */
            u64s1++;

            /* increment second iterator */
            u64s2++;
        }

        /* update the first 1-byte iterator */
        s1 = (char *) u64s1;

        /* update the second 1-byte iterator */
        s2 = (char *) u64s2;
    }

    /* compares the unaligned string */
    return compareUnalignedString(s1 , s2);
}
