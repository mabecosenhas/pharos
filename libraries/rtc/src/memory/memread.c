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


uint32_t *rtcMemReadFixed32(uint32_t *dest , volatile uint32_t *src , size_t times)
{
    /* calculate the end of the destination address */
    uint32_t *end = dest + times;


    /* loop until reached the end */
    while(dest < end)
    {
        /* read from the same source location */
        *dest = *src;

        /* increase the destination pointer */
        dest++;
    }

    /* return the dest + times */
    return dest;
}


uint16_t *rtcMemReadFixed16(uint16_t *dest , volatile uint16_t *src , size_t times)
{
    /* calculate the end of the destination address */
    uint16_t *end = dest + times;


    /* loop until reached the end */
    while(dest < end)
    {
        /* read from the same source location */
        *dest = *src;

        /* increase the destination pointer */
        dest++;
    }

    /* return the dest + times */
    return dest;
}



uint8_t *rtcMemReadFixed8(uint8_t *dest , volatile uint8_t *src , size_t times)
{
    /* calculate the end of the destination address */
    uint8_t *end = dest + times;


    /* loop until reached the end */
    while(dest < end)
    {
        /* read from the same source location */
        *dest = *src;

        /* increase the destination pointer */
        dest++;
    }

    /* return the dest + times */
    return dest;
}
