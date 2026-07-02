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


#ifndef PHAROS_CLOCKINLINE_H
#define PHAROS_CLOCKINLINE_H

#include <pharos/kernel/clock/clock.h>
#include <pharos/kernel/core/coreconfinline.h>


/**
 * Get the number of microseconds per clock ticks for the running core
 * 
 * @return returns the number of microseconds per clock ticks for the running core
 */
INLINE uint32_t pharosIMicrosecondsPerClockTick(void)
{
    /* return the number of microseconds per clock tick (only one core supported for now) */
    return pharosICore()->microsecondsPerClockTick;
}


/* PHAROS_CLOCKINLINE_H */
#endif 
