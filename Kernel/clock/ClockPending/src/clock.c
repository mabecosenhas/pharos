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
#include <pharos/kernel/core/core.h>
#include <pharos/kernel/clock/clockinline.h>
#include <pharos/kernel/scheduler/schedulerinline.h>


uint64_t pharosIClockGetSinceBoot(void)
{
    /* microseconds since last clock tick*/
    volatile uint32_t microseconds;

    /* get the number of microseconds per clock tick */
    uint32_t uticks = pharosIMicrosecondsPerClockTick();

    /* number of clock ticks since boot */
    ClockTick clockTicks;


    /* read the number of microseconds on the counter */
    clockTicks = pharosIClockTicksGet(pharosICore()->runningFlatScheduler);

    /* read the timer value */
    microseconds = pharosCpuClockReadCounter();

    /* if the clock interrupt is pending we might have read the microseconds before or after the clock tick occurred
     * (but not processed since interrupts are disabled) */
    if(pharosCpuClockIsrIsPending() == TRUE)
    {
        /* interrupt is pending, so read again the microseconds and subtract 2 clock ticks */
        microseconds = (2U * uticks) - pharosCpuClockReadCounter();
    }
    else
    {
        /* the timer counts backwards so we must subtract to get the number of microseconds since last tick */
        microseconds = uticks - microseconds;
    }

    /* calculate the result using the last us read */
    return ((clockTicks * uticks) + microseconds);
}


void pharosITimeDelay(uint32_t microseconds)
{
    /* read the current time */
    uint64_t t = pharosCpuClockReadCounterRaw();

    /* number of microseconds per tick */
    uint64_t microPerTick = pharosCpuClockMicrosecondsPerTickRaw();

    /* calculate the number of ticks that have to elapse */
    uint32_t ticks;

    /* number of microseconds left to execute after all the ticks */
    uint64_t wall;

    uint64_t current;

    uint64_t previous = t;


    /* calculate the number of ticks that have to elapse */
    ticks = microseconds / microPerTick;

    /* if the current time requires an extra clock tick */
    if(t < microseconds)
    {
        /* calculate the wall time */
        wall = microPerTick - (microseconds - t) % microPerTick;
    }
    else
    {
        /* calculate the wall time */
        wall = (t - microseconds) % microPerTick;
    }

    /* wait for the number of ticks to elapse (number of times clock counter goes through 0) */
    while(ticks > 0U)
    {
        /* read the current clock */
        current = pharosCpuClockReadCounterRaw();

        /* if it is bigger than the previous read */
        if(previous < current)
        {
            /* that means that a clock tick occurred */
            ticks--;
        }

        /* update the previous clock read */
        previous = current;
    }

    /* read the current clock */
    current = pharosCpuClockReadCounterRaw();

    /* wait until the wall is reached */
    while(current > wall)
    {
        /* read again the time until the wall reaches */
        current = pharosCpuClockReadCounterRaw();
    }
}
