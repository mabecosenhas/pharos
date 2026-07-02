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


#include <pharos/hal/clock/clock.h>


uint64_t pharosIClockGetSinceBoot(void)
{
    return pharosCpuClockReadCounter();
}


void pharosITimeDelay(uint32_t microseconds)
{
    /* calculate the time end the delay should end */
    uint64_t end = microseconds + pharosIClockGetSinceBoot();


    /* wait until the time wall reaches */
    while(pharosIClockGetSinceBoot() < end);
}
