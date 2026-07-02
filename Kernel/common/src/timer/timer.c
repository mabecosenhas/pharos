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


#include <pharos/kernel/thread/thread.h>
#include <pharos/kernel/timer/timer.h>
#include <pharos/timer.h>
#include <pharos/hal/boardApi.h>
#include <pharos/kernel/core/coreinlines.h>
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/hal/cpu/sections.h>
#include <pharos/kernel/scheduler/schedulerinline.h>


/**
 * Get the timer on the running core/partition
 * 
 * @param number timer number 
 * 
 * @return returns the pointer to the timer or NULL if none
 */
static ptrTimer getTimer(TimerNumber number);


void pharosITimersInitialize(const ptrTimerTable table , ptrPartition partition , bool restart)
{
    /* timer id iterator */
    TimerNumber i;


    /* for every timer */
    for(i = 0U; i < table->size; i++)
    {
        /* get the timer pointer */
        ptrTimer timer = &table->timers[i];

        /* reset the time tick handler */
        timer->timeTickHandler = NULL;

        /* copy the environment id */
        timer->partition = partition;

        if(restart == FALSE)
        {
            /* add the timer time tick to be used when a thread/timers will wait for a specific clock tick */
            pharosIFlatSchedulerTimeTickInit(partition->scheduler , &timer->tickHandler);
        }

    }

    /* timers are now ready to be used */
}


void pharosIPartitionTimersReset(ptrPartition p)
{
    /* timer id iterator */
    TimerNumber i;

    /* interrupt level */
    PharosInterruptLevel interruptLevel;

    /* timer table of the partition */
    ptrTimerTable table = &p->objects.timerTable;

    /* timer iterator */
    ptrTimer timer;


    /* for every timer */
    for(i = 0U; i < table->size; i++)
    {
        /* get the timer pointer */
        timer = &table->timers[i];

        /* don't allow anyone inside threads or interrupts to change the timer */
        pharosCpuDisableInterrupts(&interruptLevel);

        /* if there is an active handler */
        if(timer->timeTickHandler != NULL)
        {
            /* then remove it */
            pharosITimeTickRemoveTimer(timer);
        }

        /* interrupts can now be enabled */
        pharosCpuEnableInterrupts(&interruptLevel);
    }
}


PharosTimerStartR pharosSTimerStart(TimerNumber number , ClockTick period , uint64_t argument , bool isPeriodic)
{
    /* result of the function */
    PharosTimerStartR result;

    /* interrupt level */
    PharosInterruptLevel interruptLevel;

    /* get the timer pointer */
    ptrTimer timer;

    /* timer scheduler */
    ptrFlatScheduler scheduler;


    /* get the timer */
    timer = getTimer(number);

    /* if no timer found */
    if(timer == NULL)
    {
        /* invalid timer id */
        result = PHAROS_TIMER_START_INVALID;
    }
    else if(period < timer->minimumPeriod)
    {
        /* invalid period */
        result = PHAROS_TIMER_START_INVALID_PERIOD;
    }
    else
    {
        /* get the timer scheduler */
        scheduler = timer->partition->scheduler;

        /* don't allow anyone inside threads or interrupts to change the timer */
        pharosCpuDisableInterrupts(&interruptLevel);

        /* if the timer is currently being scheduled */
        if(timer->timeTickHandler != NULL)
        {
            /* interrupts can now be enabled */
            pharosCpuEnableInterrupts(&interruptLevel);

            /* timer is being scheduled */
            result = PHAROS_TIMER_START_ALREADY_STARTED;
        }
        else
        {
            /* set the argument on the timer */
            timer->argument = argument;

            /* set the current period */
            timer->currentPeriod = period;

            /* set the flag if the timer is periodic */
            timer->flags.isPeriodic = isPeriodic;

            /* schedule the timer to fire in "ticks" clock ticks */
            pharosITimeTickAddTimer(scheduler , timer , timer->currentPeriod + pharosIClockTicksGet(scheduler));

            /* interrupts can now be enabled */
            pharosCpuEnableInterrupts(&interruptLevel);

            /* timer is now scheduled */
            result = PHAROS_TIMER_START_SUCCESS;
        }
    }

    /* return the method return value */
    return result;
}


PharosTimerStopR pharosSTimerStop(TimerNumber timerId)
{
    /* result of the function */
    PharosTimerStopR result;

    /* interrupt level */
    PharosInterruptLevel interruptLevel;

    /* get the timer pointer */
    ptrTimer timer;


    /* get the timer */
    timer = getTimer(timerId);

    /* if no timer found */
    if(timer == NULL)
    {
        /* invalid timer id */
        result = PHAROS_TIMER_STOP_INVALID;
    }
    else
    {
        /* don't allow anyone inside threads or interrupts to change the timer */
        pharosCpuDisableInterrupts(&interruptLevel);

        /* if the timer is not scheduled */
        if(timer->timeTickHandler == NULL)
        {
            /* interrupts can now be enabled */
            pharosCpuEnableInterrupts(&interruptLevel);

            /* timer is being scheduled */
            result = PHAROS_TIMER_STOP_ALREADY_STOPPED;
        }
        else
        {
            /* remove the timer */
            pharosITimeTickRemoveTimer(timer);

            /* interrupts can now be enabled */
            pharosCpuEnableInterrupts(&interruptLevel);

            /* return success */
            result = PHAROS_TIMER_STOP_SUCCESS;
        }
    }

    /* return the method return value */
    return result;
}


static ptrTimer getTimer(TimerNumber number)
{
    /* result of the method */
    ptrTimer result;

    /* then get the table of timers in the current partition */
    ptrTimerTable table = &pharosIPartitionRunObjectTable()->timerTable;


    /* if the queue id is too high */
    if((table == NULL) || (number >= table->size))
    {
        /* return invalid */
        result = NULL;
    }
        /* if the id is OK */
    else
    {
        /* return the pointer to the queue */
        result = &table->timers[number];
    }

    /* return the timer pointer */
    return result;
}


void pharosITimerExecute(ptrTimer timer)
{
    /* timer scheduler */
    ptrFlatScheduler scheduler;


    /* get the timer scheduler */
    scheduler = timer->partition->scheduler;

    /* set the running environment when executing the timer */
    pharosICore()->runningTimerPartition = timer->partition;

    /* update the time tick handler BEFORE executing the handler since the handler could start again this very timer */

    /* if the timer is periodic */
    if(timer->flags.isPeriodic == TRUE)
    {
        /* schedule the timer to execute again */
        pharosITimeTickAddTimer(scheduler , timer , (timer->currentPeriod + pharosIClockTicksGet(scheduler)));
    }
    else
    {
        /* reset the handler (shows that the timer has already executed) */
        timer->timeTickHandler = NULL;
    }

    /* execute the timer */
    timer->body(timer->partition->data , timer->argument);
}


ptrPartition pharosITimerRunningPartition(void)
{
    /* get the partition running the timer */
    return pharosICore()->runningTimerPartition;
}
