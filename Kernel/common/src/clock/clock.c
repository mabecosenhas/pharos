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


#include <pharos/kernel/clock/clock.h>
#include <pharos/kernel/clock/clockinline.h>
#include <pharos/kernel/thread/thread.h>
#include <pharos/clock.h>
#include <pharos/hal/boardApi.h>
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/hal/cpu/sections.h>
#include <pharos/kernel/scheduler/schedulerinline.h>


/**
 * Pharos current date. Is invalid (month = 0)
 */
KERNEL_SECTION static Date pharosVdate = {
    .y = 0 ,
    .month = 0 ,
    .d = 0 ,
    .h = 0 ,
    .m = 0 ,
    .s = 0 ,
    .us = 0 ,
};

/**
 * Table with the number of days on each month on a non-leap year
 */
static const uint8_t pharosVDaysPerMonthNormal[] = {
    0 , /* unused (months start at 1) */
    31 , /* January */
    28 , /* February */
    31 , /* March */
    30 , /* April */
    31 , /* May */
    30 , /* June */
    31 , /* July */
    31 , /* August*/
    30 , /* September */
    31 , /* October */
    30 , /* November */
    31 , /* December */
};

/**
 * Table with the number of days on each month on a leap year
 */
static const uint8_t pharosVDaysPerMonthLeap[] = {
    0 , /* unused (months start at 1) */
    31 , /* January */
    29 , /* February */
    31 , /* March */
    30 , /* April */
    31 , /* May */
    30 , /* June */
    31 , /* July */
    31 , /* August*/
    30 , /* September */
    31 , /* October */
    30 , /* November */
    31 , /* December */
};


/**
 * checks if the date is correct (e.g. year != 0, milliseconds < 999)
 * 
 * @param date date to check
 * 
 * @return returns TRUE if the date is valid and FALSE otherwise
 */
static bool checkDate(const ptrDate date);


/**
 * get the number of days of the specified month and year
 * 
 * @param year year
 * @param month month
 * @return returns the number of days on the month
 */
static uint8_t dayOfMonth(uint16_t year , uint8_t month);


/**
 * increase month by 1 (changes year if needed)
 * 
 * @param date date to increase by one month
 */
INLINE void incrementMonth(const ptrDate date);


/**
 * Checks if the seconds is too big.
 * 
 * NOTE: assumes date->s is smaller than 120 (that is, date->s < 120 is always TRUE)
 * 
 * @param date date to increment
 */
INLINE void adjustSeconds(ptrDate date);


void pharosIIncDateOneClockTick(void)
{
    /* number of microseconds per tick */
    uint32_t delta = pharosIMicrosecondsPerClockTick();


    /* increment the number of microseconds elapsed */
    pharosVdate.us += delta;

    /* adjust microseconds */
    if(pharosVdate.us >= 1000000U)
    {
        /* increase the number of seconds (this assumes that the clock tick can not be higher than 1000000 microseconds */
        pharosVdate.s++;
        pharosVdate.us -= 1000000U;

        /* increment one second in the clock */
        adjustSeconds(&pharosVdate);
    }
}


PharosClockGetR pharosSClockGetDate(ptrDate date)
{
    /* result of the method */
    PharosClockGetR result;

    /* interrupt level */
    PharosInterruptLevel interruptLevel;


    /* if the date is not valid */
    if(pharosCpuBufferIsInsideProtection((uint8_t *) date , sizeof (Date) , pharosIPartitionRunningMemProt()) == FALSE)
    {
        /* return invalid date */
        result = PHAROS_CLOCK_GET_DATE_INVALID;
    }
        /* check if the month is invalid */
    else if(pharosVdate.month == 0U)
    {
        /* then the clock has not yet been set */
        result = PHAROS_CLOCK_GET_DATE_CLOCK_NOT_SET;
    }
    else
    {
        /* don't allow anyone inside threads or interrupts to change the timer */
        pharosCpuDisableInterrupts(&interruptLevel);

        /* copy the date */
        *date = pharosVdate;

        /* interrupts can now be enabled */
        pharosCpuEnableInterrupts(&interruptLevel);

        /* return success */
        result = PHAROS_CLOCK_GET_DATE_SUCCESS;
    }

    /* return the method result */
    return result;
}


ClockTick pharosSClockGetTicks(void)
{
    /* result of the method */
    volatile ClockTick result;

    /* interrupt level */
    PharosInterruptLevel interruptLevel;


    /* don't allow anyone inside threads or interrupts to change the timer */
    pharosCpuDisableInterrupts(&interruptLevel);

    /* get the clock ticks */
    result = pharosIClockTicksGet(pharosICore()->runningFlatScheduler);

    /* interrupts can now be enabled */
    pharosCpuEnableInterrupts(&interruptLevel);

    /* return the method result */
    return result;
}


uint64_t pharosSClockGetSinceBoot(void)
{
    /* interrupt level */
    PharosInterruptLevel interruptLevel;

    /* microseconds since boot */
    volatile uint64_t result;


    /* don't allow anyone inside threads or interrupts to change the interrupt handler table */
    pharosCpuDisableInterrupts(&interruptLevel);

    /* get the us since boot */
    result = pharosIClockGetSinceBoot();

    /* interrupts can now be enabled */
    pharosCpuEnableInterrupts(&interruptLevel);

    /* return the number of us since boot */
    return result;
}


PharosClockSetR pharosSClockSetDate(ptrDate date)
{
    /* result of the method */
    PharosClockSetR result;

    /* interrupt level */
    PharosInterruptLevel interruptLevel;


    /* check if the date is valid */
    /* if the date is not valid */
    if(pharosCpuBufferIsInsideProtection((uint8_t *) date , sizeof (Date) , pharosIPartitionRunningMemProt()) == FALSE)
    {
        /* return invalid date */
        result = PHAROS_CLOCK_SET_DATE_INVALID;
    }
    else if(checkDate(date) == TRUE)
    {
        /* don't allow anyone inside threads or interrupts to change the timer */
        pharosCpuDisableInterrupts(&interruptLevel);

        /* copy the date (protected since this may be called from interrupt) */
        pharosVdate = *date;

        /* interrupts can now be enabled */
        pharosCpuEnableInterrupts(&interruptLevel);

        /* return success */
        result = PHAROS_CLOCK_SET_DATE_SUCCESS;
    }
    else
    {
        /* return date is invalid */
        result = PHAROS_CLOCK_SET_DATE_INVALID;
    }

    /* return the method result */
    return result;

}


bool checkDate(const ptrDate date)
{
    /* result of the method */
    bool result = TRUE;


    /* check the months */
    if((date->month < 1U) || (date->month > 12U))
    {
        /* then month is incorrect */
        result = FALSE;
    }
        /* check the days */
    else if((date->d < 1U) || (date->d > dayOfMonth(date->y , date->month)))
    {
        /* then day is incorrect */
        result = FALSE;
    }
        /* check the hours */
    else if(date->h > 23U)
    {
        /* then hour is incorrect */
        result = FALSE;
    }
        /* check the minutes */
    else if(date->m > 59U)
    {
        /* then minute is incorrect */
        result = FALSE;
    }
        /* check the seconds */
    else if(date->s > 59U)
    {
        /* then seconds is incorrect */
        result = FALSE;
    }
        /* check the us */
    else if(date->us > 999999U)
    {
        /* then us is incorrect */
        result = FALSE;
    }
        /* everything is correct */
    else
    {
        /* return valid date */
        result = TRUE;
    }

    /* return the result */
    return result;
}


uint8_t dayOfMonth(uint16_t year , uint8_t month)
{
    /* result of the method */
    uint8_t result;


    /* if it is a leap year */
    if((year & 3U) == 0U)
    {
        /* return the number of days in the leap month */
        result = pharosVDaysPerMonthLeap[month];
    }
        /* else, it is a normal year */
    else
    {
        /* return the number of days in the normal month */
        result = pharosVDaysPerMonthNormal[month];
    }

    /* return the result */
    return result;
}


INLINE void incrementMonth(const ptrDate date)
{
    /* if the month is too great */
    if(date->month > 12U)
    {
        /* reset the month */
        date->month = 1U;

        /* and increase the years */
        date->y++;
    }
    else
    {
        /* increase the month */
        date->month++;
    }
}


INLINE void adjustSeconds(ptrDate date)
{
    /* adjust seconds */
    if(date->s >= 60U)
    {
        /* increment the minutes */
        date->m++;

        /* decrease the seconds */
        date->s -= 60U;

        /* adjust minutes */
        if(date->m >= 60U)
        {
            /* increment the hours */
            date->h++;

            /* decrease the minutes */
            date->m -= 60U;

            /* adjust hours */
            if(date->h >= 24U)
            {
                /* increment the days */
                date->d++;

                /* decrease the hours */
                date->h -= 24U;

                /* adjust days */
                if(date->d > dayOfMonth(date->y , date->month))
                {
                    /* decrement the days */
                    date->d = date->d - dayOfMonth(date->y , date->month);

                    /* increment the month (and year) */
                    incrementMonth(date);
                }
            }
        }
    }
}
