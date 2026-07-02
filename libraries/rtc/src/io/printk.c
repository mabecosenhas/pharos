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


PHAROS_OPTIMIZE_O0 void printk(const char *fmt , ...)
{
    /* @note: on arm-v8a-a53-rasp3-qemu-release this function fails the test 195 if the attribute -O0 is removed
     * (GCC 8.2.0). Reason still to figure out...maybe related to variable arguments */

    /* variable argument list */
    va_list ap;


    /* start the variable arguments */
    va_start(ap , fmt);

    /* use the rtcPrintkInternal (without buffer) to print using the outputChar function */
    rtcPrintkInternal(outputChar , NULL , fmt , ap);

    /* clean up the variable arguments */
    va_end(ap);
}


PHAROS_OPTIMIZE_O0 void rtcPrink(const char *fmt , ...)
{
    /* @note: attribute -O0 is placed due to the same as the printk function */

    /* variable argument list */
    va_list ap;


    /* start the variable arguments */
    va_start(ap , fmt);

    /* use the rtcPrintkInternal (without buffer) to print using the outputChar function */
    rtcPrintkInternal(outputChar , NULL , fmt , ap);

    /* clean up the variable arguments */
    va_end(ap);
}
