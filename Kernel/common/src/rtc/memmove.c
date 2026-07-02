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


void *memmove(void *dest , const void *src , size_t n)
{
    /* use the rtcMemMove8 to move data */
    rtcMemMove8(dest , src , n);

    /* return the original destination */
    return dest;
}


void rtcMemMove8(void *dst , const void *src , size_t length)
{
    /* first check the case that we have an overlap (the beginning of the destination is inside the source array) */
    if((dst > src) && (dst <= src + length))
    {
        /* in this case we have to copy backwards, otherwise the first bytes of the source would be placed on the first 
         * bytes of the destination which are inside the source array */
        rtcMemCopyReverse8(dst , src , length);
    }
    else
    {
        /* else, just copy the bytes as usual */
        memcpy(dst , src , length);
    }
}
