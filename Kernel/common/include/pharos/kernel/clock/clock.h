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


#ifndef PHAROS_CLOCK_H
#define PHAROS_CLOCK_H


#include <pharos/kernel/clock/declarations.h>
#include <pharos/kernel/time/declarations.h>
#include <pharos/declarations.h>
#include <pharos/kernel/core/declarations.h>

struct Date
{
    /**
     * year
     */
    int16_t y;

    /**
     * month from 1 to 12
     */
    uint8_t month;

    /**
     * day from 1 to 31
     */
    uint8_t d;

    /**
     * hour from 0 to 23
     */
    uint8_t h;

    /**
     * minute from 0 to 59
     */
    uint8_t m;

    /**
     * second from 0 to 59
     */
    uint8_t s;

    /**
     * microsecond from 0 to 999.999 (there is no milliseconds)
     */
    uint32_t us;
};


extern uint32_t pharosVMicroPerClockTick[];


/**
 * increments the date by one clock tick
 * 
 * @contraint Assumes a clock tick a maximum of 1 second!!!
 * 
 * @param date date to add the nanoseconds 
 * @param nanoseconds number of nanoseconds to add
 */
void pharosIIncDateOneClockTick(void);


/**
 * Internal call to get the clock ticks
 * 
 * @return returns then number of clock ticks
 */
ClockTick pharosSClockGetTicks(void);


/**
 * Get the number of microseconds since boot with interrupts disabled 
 * 
 * @constraint interrupts must be disabled when calling this function
 * 
 * @return returns the number of microseconds since boot
 */
uint64_t pharosIClockGetSinceBoot(void);

/**
 * Syscall for pharosClockGetSinceBoot function
 * 
 * @return returns the number of microseconds elapsed since the boot
 */
uint64_t pharosSClockGetSinceBoot(void);

/**
 * Gets the current date
 * 
 * @contraint runs in supervisor mode 
 * 
 * @param date pointer to the address where to place the current date
 * 
 * @return returns success or the error
 */
PharosClockGetR pharosSClockGetDate(ptrDate date);


/**
 * Set the clock date
 * 
 * @contraint runs in supervisor mode 
 * 
 * @param date new date to set
 * 
 * @return returns success or the error
 */
PharosClockSetR pharosSClockSetDate(ptrDate date);


#endif /* CLOCK_H */
