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
#include <pharos/rtc/rtcinternal.h>


int32_t rtcStringCompareN(const char *s1 , const char *s2 , size_t size)
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
    intptr_t alignment = pharosIAlignmentNextBothBound(s1 , s2 , PHAROS_CPU_COPY8_ALIGN , size);


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

        /* we know that n is bigger than the alignment (otherwise alignment would be < 0) */
        size -= alignment;

        /* not both us1 and us2 are aligned */

        /* MISRA: we know that now us1 is aligned to 8 bytes */
        u64s1 = (uint64_t *) (s1 + alignment);

        /* MISRA: we know that now us1 is aligned to 8 bytes */
        u64s2 = (uint64_t *) (s2 + alignment);


        /* while there are at least 8 bytes to search for */
        while(size >= 8U)
        {
            /* if the 8 bytes are equal */
            if(*u64s1 == *u64s2)
            {
                /* check if any of the bytes contains a termination */
                if(pharosU64ContainsTermination(*u64s1) != 0)
                {
                    /* stop looking after a termination is found */
                    break;
                }
            }
            else
            {
                /* stop looking (8 bytes at a time) if they are different */
                break;
            }

            /* increment first iterator */
            u64s1++;

            /* increment second iterator */
            u64s2++;

            /* decrement number of bytes to search for */
            size -= 8U;
        }

        /* update the first 1-byte iterator */
        s1 = (char *) u64s1;

        /* update the second 1-byte iterator */
        s2 = (char *) u64s2;
    }

    /* compares the unaligned string */
    return rtcStringCompareUnalignedSize(s1 , s2 , size , &ended);
}


intptr_t rtcStringCompareUnalignedSize(const char *s1 , const char *s2 , size_t size , bool *ended)
{
    /* calculate the end of the string */
    const char *s1End = s1 + size - 1;


    /* initialize the flag */
    *ended = FALSE;

    /* if there are no bytes to search for */
    if(size == 0U)
    {
        /* then they are equal */
        return 0;
    }

    /* check until alignment is complete */
    while(TRUE)
    {
        /* check if both bytes are equal */
        if(*s1 == *s2)
        {
            /* if we have reached the end of the string */
            if(*s2 == '\0')
            {
                /* stop searching */
                *ended = TRUE;

                /* stop looking */
                break;
            }
        }
        else
        {
            /* stop searching */
            *ended = TRUE;

            /* return the difference */
            break;
        }

        /* if reached the end of the string */
        if(s1 == s1End)
        {
            /* don't increment the pointers */
            break;
        }

        /* increment the first iterator */
        s1++;

        /* increment the second iterator */
        s2++;
    }

    /* did not find differences */
    return (*s1) - (*s2);
}
