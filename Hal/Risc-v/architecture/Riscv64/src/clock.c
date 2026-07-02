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


#include <pharos/hal/hal.h>
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/hal/cpu/mtime.h>
#include <pharos/kernel/clock/clockinline.h>
#include <pharos/hal/interrupt.h>
#include <pharos/hal/cpu/sections.h>


/**
 * Set the biggest possible time (equivalent to ~500.000 years since it is measured in microseconds)
 */
#define PHAROS_MTIMER_INVALID_WCET      (0xFFFFFFFFFFFFFFFFUL)


/**
 * Next instant when the WCET timer is supposed to be executed
 */
KERNEL_SECTION_USS static volatile uint64_t nextWcet[PHAROS_NUMBER_CORES];


/**
 * Next instant when the clock tick is supposed to be executed
 */
KERNEL_SECTION_USS static volatile uint64_t nextTick[PHAROS_NUMBER_CORES];


/**
 * Number of microseconds per clock tick 
 */
KERNEL_SECTION_USS static uint32_t clockTick[PHAROS_NUMBER_CORES];


INLINE void fireTimer(CoreNumber core)
{
    /* set the timer to trigger at the next closest event */
    PHAROS_CPU_CLINT->mtimecmp[core] = MIN(nextWcet[core] , nextTick[core]);
}


PHAROS_WEAK void pharosCpuClockReset(void)
{
    /* get the running core */
    CoreNumber core = pharosICoreRunning();


    /* calculate the next instant when the clock tick should occur */
    nextTick[core] = pharosCpuMTime() + clockTick[core];

    /* fire the timer */
    fireTimer(core);
}


PHAROS_WEAK void pharosCpuClockInitialize(void)
{
    /* get the running core */
    CoreNumber core = pharosICoreRunning();


    /* initialize the clock tick */
    clockTick[core] = PHAROS_CPU_MICRO_2_TIMER(pharosIMicrosecondsPerClockTick());

    /* set to not trigger an interrupt (the pharosCpuClockReset will initialize this with a proper value just before 
     * multitasking begins) */
    nextTick[core] = PHAROS_MTIMER_INVALID_WCET;

    /* invalidate the WCET Timer */
    nextWcet[core] = PHAROS_MTIMER_INVALID_WCET;

    /* fire the timer */
    fireTimer(core);
}


void pharosCpuMtimerIsr(const uint8_t *interruptedAddress)
{
    /* get the running core */
    CoreNumber core = pharosICoreRunning();

    /* get the current time */
    volatile uint64_t time = pharosCpuMTime();


    /* disable the timer interrupt so we can enable nested interrupts */
    pharosCpuMTimerInterruptDisable();

    /* now we can enable interrupts! */
    pharosCpuInterruptNestedEnable();

    /* if the WCET interrupt is overdue */
    if(time >= nextWcet[core])
    {
        /* invalidate the next WCET */
        nextWcet[core] = PHAROS_MTIMER_INVALID_WCET;

        /* call WCET handler */
        pharosIWcetHandlerIsr(interruptedAddress);
    }

    /* if the next tick is overdue */
    if(time >= nextTick[core])
    {
        /* update the instant when to fire the clock tick next */
        nextTick[core] += clockTick[core];

        /* call time tick interrupt handler */
        pharosITimeTickIsr();
    }

    /* fire the timer (this will disable the timer interrupt until the next wall time) */
    fireTimer(core);

    /* can now process another timer interrupt */
    pharosCpuMTimerInterruptEnable();
}


uint64_t pharosCpuClockReadCounter(void)
{
    /* read the time since boot */
    return pharosCpuMTime();
}


void pharosCpuWcetClockFire(uint32_t delta)
{
    /* get the running core */
    CoreNumber core = pharosICoreRunning();


    /* set the next instant to fire */
    nextWcet[core] = pharosCpuMTime() + delta;

    /* update the timer fire (if needed) */
    fireTimer(core);
}


void pharosCpuWcetClockDisable(void)
{
    /* get the running core */
    CoreNumber core = pharosICoreRunning();


    /* turn off the WCET response */
    nextWcet[core] = PHAROS_MTIMER_INVALID_WCET;

    /* update the timer fire (if needed) */
    fireTimer(core);
}
