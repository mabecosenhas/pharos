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


size_t rtcStringSearchAccept(const char *s , const char *accept)
{
    /* iterator in "s" */
    const char *iterator;

    /* iterator in "accept" */
    const char *acceptIterator;


    /* search characters in the "s" string */
    for(iterator = s; *iterator != '\0'; iterator++)
    {
        /* check if the character "iterator" is inside the "accept" string */
        for(acceptIterator = accept; (*acceptIterator != '\0') && (*iterator != *acceptIterator); acceptIterator++);

        /* if reached the end of the accept string without having located the iterator */
        if(*acceptIterator == '\0')
        {
            /* then stop looking, we did not find the iterator char */
            break;
        }
    }

    /* return the index of the first char in "s" that is not in "accept" */
    return iterator - s;
}
