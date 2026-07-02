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


char *rtcStringConcatN(char * dest , const char * src , size_t n)
{
    /* aux pointer (we have to return dest in the end (which will have the place where the string was concatenated) */
    char *aux = dest;


    /* check if destination is aligned */
    if(pharosIIsAligned(dest , PHAROS_CPU_COPY8_ALIGN) == TRUE)
    {
        /* if the destination string contains a '\0' */
        while(pharosU64ContainsTermination(*(uint64_t *) aux) == 0L)
        {
            /* skip to next 8 bytes */
            aux += 8U;
        }
    }

    /* continue to search byte by byte */
    while((*aux) != '\0')
    {
        aux++;
    }

    /* store the location of where the concatenation took place */
    dest = aux;

    /* while there are bytes left to copy and src does not end with a '\0' */
    while(n > 0)
    {
        /* copy one byte */
        *aux = *src;

        /* if reached the end of the string */
        if(*src == '\0')
        {
            /* then leave the function (we already copied '\0') */
            return dest;
        }

        /* increase the destination pointer */
        aux++;

        /* increase the source pointer */
        src++;

        /* decrease the number of bytes left to copy */
        n--;
    }

    /* if we reached here it means that the we can only write one more byte and it must be '\0' */
    *aux = '\0';

    /* return the pointer where the string was concatenated */
    return dest;
}
