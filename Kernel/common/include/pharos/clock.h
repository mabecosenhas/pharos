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


#ifndef PHAROS_API_CLOCK_H
#define PHAROS_API_CLOCK_H


#include <pharos/declarations.h>
#include <pharos/kernel/clock/clock.h>
#include <pharos/kernel/time/time.h>


/**
 * Get the date of the system
 * 
 * @param date pointer where to place the system date
 */
PharosClockGetR pharosClockGetDate(ptrDate date);


/**
 * Set the system date. 
 * Note: by changing the date does NOT activate any threads or timers nor trigger any timeouts.
 * 
 * @param date new date to set
 */
PharosClockSetR pharosClockSetDate(ptrDate date);


/**
 * Get the number of clock ticks that have occurred since boot
 * 
 * @return returns the number of clock ticks since boot 
 */
ClockTick pharosClockGetTicks(void);


/**
 * Get the number of microseconds since the system boot
 * 
 * @return returns the number of microseconds since the last boot (reset) of the system
 */
uint64_t pharosClockGetSinceBoot(void);


/**
 * Supervisor mode function that performs an active delay for a specified number of microseconds 
 * Function is safe to use in partition initialization functions, interrupt routines, etc.
 * 
 * @note care must be taken when invoke this function since a thread with user permissions will GIVE A MEMORY EXCEPTION.
 * 
 * @constraint caller must have supervisor permissions since this method accesses 
 * 
 * @param microseconds number of microseconds to wait
 */
void pharosITimeDelay(uint32_t microseconds);


#endif /* CLOCK_H */
