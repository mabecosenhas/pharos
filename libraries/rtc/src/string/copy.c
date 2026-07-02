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


static char *copyUnalignedString(char *dst , const char *src , size_t max , bool *flag)
{
    /* calculate end of the array to copy (reserve space for the '\0' */
    char *end = dst + max - 1;


    /* special case of not copying anything */
    if(max == 0U)
    {
        /* returning dst + max = dst */
        return dst;
    }

    /* for all the bytes left to copy */
    while(dst < end)
    {
        /* check if we reached the end of the string */
        if(*src == '\0')
        {
            *flag = TRUE;

            break;
        }

        /* copy byte by byte */
        *dst = *src;

        /* increment the byte index to copy */
        src++;

        dst++;
    }

    /* if did not find the '\0' on the src and has reached the end of the copy */

    /* place the end */
    *dst = '\0';

    /* did not find it */
    return dst;
}


char *rtcStrcopy(char *dst , const char *src , size_t max)
{
    bool flag = FALSE;

    char *result;

    uint64_t *d;
    uint64_t *s;

    /* check if there is a possibility to align both pointers (within the size) and calculate how many 
     * bytes to get there */
    intptr_t alignment = pharosIAlignmentNextBothBound(dst , src , PHAROS_CPU_COPY8_ALIGN , max);


    /* if there is a possibility for alignment */
    if(alignment >= 0)
    {
        /* copy first part of the unaligned string */
        result = copyUnalignedString(dst , src , alignment , &flag);

        /* if we have found the '\0' */
        if(flag == TRUE)
        {
            /* nothing else to do */
            return result;
        }

        /* we know that n is bigger than the alignment (otherwise alignment would be < 0) */
        max -= alignment;

        /* not both us1 and us2 are aligned */

        /* MISRA: we know that now d has the required alignment for 8 bytes */
        d = (uint64_t *) (dst + alignment);

        /* MISRA: we know that now s has the required alignment for 8 bytes */
        s = (uint64_t *) (src + alignment);

        /* while there are 8 bytes left to copy */
        while(max >= 8U)
        {
            /* check if any of the 8 bytes has a '\0' */
            if(pharosU64ContainsTermination(*s) != 0U)
            {
                /* copy 8 bytes at a time */
                *d = *s;
            }
                /* else, there is an '/0' */
            else
            {
                /* stop copying the 8 bytes */
                break;
            }

            /* increment the source iterator */
            s++;

            /* increment the destination iterator */
            d++;

            /* decrease the number of bytes to copy */
            max -= 8U;
        }
    }

    /* copy remaining bytes */
    result = copyUnalignedString(dst , src , max , &flag);

    /* return the end of the string */
    return result;
}
