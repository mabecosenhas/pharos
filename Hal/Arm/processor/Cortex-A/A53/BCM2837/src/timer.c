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


#include <pharos/hal/cpu/timer.h>
#include <pharos/kernel/interrupt/interrupt.h>
#include <pharos/kernel/clock/clock.h>
#include <pharos/kernel/time/time.h>
#include <pharos/hal/cpu/gptimer.h>
#include <pharos/hal/cpu/interruptController.h>
#include <pharos/hal/boardApi.h>
#include <pharos/hal/cpu/sections.h>
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/kernel/clock/clockinline.h>
#include <pharos/hal/clock/clock.h>


/**
 * Clock frequency of the crystal (19.2 MHz)
 */
#define PHAROS_CPU_CRYSTAL_SPEED            (19200000LU)


#define PHAROS_CPU_PRESCALAR_LOAD           (((uint64_t) (2U * VALUE_1GiB * 10) / PHAROS_CPU_CRYSTAL_SPEED) / 10U)

#define PHAROS_CPU_TIMER_CTRL_HLT_DEBUG     (1U << 8U)
#define PHAROS_CPU_TIMER_CTRL_ENABLE        (1U << 7U)
#define PHAROS_CPU_TIMER_CTRL_DISABLE       (0U << 7U)
#define PHAROS_CPU_TIMER_CTRL_23_BITS       (1U << 1U)
#define PHAROS_CPU_TIMER_CTRL_INT_EN        (1U << 5U)


/**
 * WCET ISR handler
 * 
 * @param number interrupt fired
 * @param interruptedAddress address that was interrupted
 */
static void wcetIsr(void *argument , uintptr_t preInterruptData , PharosInterruptNumber number , const uint8_t *interruptedAddress);


/**
 * Clear interrupt for timer 
 */
static void wcetClear(void *data , uintptr_t preHandlerReturn);


/**
 * ISR handler for clock tick timer interrupt (system timer 1)
 */

static void timer1Isr(void *argument , uintptr_t preInterruptData , PharosInterruptNumber number , const uint8_t *interruptedAddress);


/**
 * Clear interrupt for timer 1
 */
static void timer1ClearInterrupt(void *data , uintptr_t preHandlerReturn);


KERNEL_SECTION_USS static uint32_t clockTick[PHAROS_NUMBER_CORES];

KERNEL_SECTION_USS static uint64_t cntfrq[PHAROS_NUMBER_CORES];

KERNEL_SECTION_USS static uint64_t last[PHAROS_NUMBER_CORES];


PHAROS_WEAK void pharosCpuClockReset(void)
{
    CoreNumber core = pharosICoreRunning();


    /* update the last instant a clock tick occurred */
    last[core] = pharosCpuCntpCtEl0Get() + clockTick[core];

    /* set the time value to setup the clock as a 32-bit downcounter */
    pharosCpuCntpCvalEl0Set(last[core]);

    /* enable the clock timer (CNTP) */
    pharosCpuCntpCtlEl0Set(1 << 0);
}


PHAROS_WEAK void pharosCpuClockInitialize(void)
{
    CoreNumber core = pharosICoreRunning();

    /* global variable used only in this function with the timer 1 interrupt configuration */
    static KERNEL_SECTION_USS PartitionIsrConf timer1PartitionConf[PHAROS_NUMBER_CORES];

    /* global variable used only in this function with the timer interrupt configuration */
    static KERNEL_SECTION_USS PartitionIsrConf timer2PartitionConf[PHAROS_NUMBER_CORES];

    /* global variable used only in this function with the timer 1 interrupt configuration */
    static KERNEL_SECTION_USS InterruptConfiguration timer1[PHAROS_NUMBER_CORES];

    /* global variable used only in this function with the timer interrupt configuration */
    static KERNEL_SECTION_USS InterruptConfiguration timer2[PHAROS_NUMBER_CORES];

    /* get the clock frequency */
    cntfrq[core] = pharosCpuCntFreqGet();

    /* initialize timer1 configuration with the handler */
    timer1PartitionConf[core].isrHandler = timer1Isr;

    /* timer1 is not from a partition */
    timer1PartitionConf[core].partition = NULL;

    /* set the interrupt configuration */
    timer1PartitionConf[core].interruptConf = &timer1[core];

    /* setup the interrupt number for the timer 1 */
    timer1[core].number = PHAROS_CPU_ISR_CNTPN;

    /* highest priority */
    timer1[core].cpu.priority = PHAROS_CPU_INTERRUPT_IRQ;

    /* set the clear interrupt routine */
    timer1[core].postInterruptHandler = timer1ClearInterrupt;

    /* not installed */
    timer1[core].isInstalled = FALSE;

    /* initialize timer configuration with the handler */
    timer2PartitionConf[core].isrHandler = wcetIsr;

    /* timer is not from a partition */
    timer2PartitionConf[core].partition = NULL;

    /* set the interrupt configuration */
    timer2PartitionConf[core].interruptConf = &timer2[core];

    /* setup the interrupt number for the timer 1 */
    timer2[core].number = PHAROS_CPU_ISR_CNTV;

    /* highest priority */
    timer2[core].cpu.priority = PHAROS_CPU_INTERRUPT_IRQ;

    /* set the clear interrupt routine */
    timer2[core].postInterruptHandler = wcetClear;

    /* not installed */
    timer2[core].isInstalled = FALSE;

    /* initialize the number of clock cycles needed for a clock tick */
    clockTick[core] = ((uint64_t) pharosIMicrosecondsPerClockTick() * cntfrq[core]) / VALUE_1M - 1;

    /* catch the timer 1 interrupt */
    pharosIInterruptInstallInit(&timer1[core] , &timer1PartitionConf[core]);

    /* catch the timer 2 interrupt */
    pharosIInterruptInstallInit(&timer2[core] , &timer2PartitionConf[core]);

    /* update the last instant a clock tick occurred */
    last[core] = pharosCpuCntpCtEl0Get() + clockTick[core];

    /* set the time value to setup the clock as a 32-bit down-counter */
    pharosCpuCntpCvalEl0Set(last[core]);

    /* enable the clock timer (CNTP) */
    pharosCpuCntpCtlEl0Set(1 << 0);
}


void timer1Isr(void *argument , uintptr_t preInterruptData , PharosInterruptNumber number , const uint8_t *interruptedAddress)
{
    /* call time tick interrupt handler */
    pharosITimeTickIsr();
}


void timer1ClearInterrupt(void *data , uintptr_t preHandlerReturn)
{
    /* increment the last clock tick instant */
    last[pharosICoreRunning()] += clockTick[pharosICoreRunning()];

    /* set the interrupt to the next clock tick */
    pharosCpuCntpCvalEl0Set(last[pharosICoreRunning()]);
}


uint64_t pharosCpuClockReadCounterRaw(void)
{
    /* running core */
    CoreNumber core = pharosICoreRunning();


    /* return the number of microseconds to the next tick */
    return (pharosCpuClockMicrosecondsPerTickRaw() - pharosCpuCntpCtEl0Get()) / (cntfrq[core] / VALUE_1M);
}


uint64_t pharosCpuClockMicrosecondsPerTickRaw(void)
{
    /* the clock goes round at each UINT64_MAX */
    return UINT64_MAX;
}


uint32_t pharosCpuClockReadCounter(void)
{
    /* get the current counter */
    uint64_t ct = pharosCpuCntpCtEl0Get();


    /* if the counter is greater than the compare value */
    if(ct >= last[pharosICoreRunning()])
    {
        /* then return the number of ticks to the next next clock tick 
         * (a clock tick should have already occurred but we have the interrupts disabled) */
        return pharosIMicrosecondsPerClockTick() - ((ct - last[pharosICoreRunning()]) * VALUE_1M / cntfrq[pharosICoreRunning()]);
    }
        /* if the counter is lower than the compare value */
    else
    {
        /* return the number of ticks to the next clock tick */
        return ( (last[pharosICoreRunning()] - ct) * VALUE_1M / cntfrq[pharosICoreRunning()]);
    }
}


bool pharosCpuClockIsrIsPending(void)
{
    /* if the current compare value (last = CVAL) is smaller than the counter (CT) then an interrupt is pending */
    return last[pharosICoreRunning()] <= pharosCpuCntpCtEl0Get() ? TRUE : FALSE;
}


void wcetIsr(void *argument , uintptr_t preInterruptData , PharosInterruptNumber number , const uint8_t *interruptedAddress)
{
    /* call WCET handler */
    pharosIWcetHandlerIsr(interruptedAddress);
}


void wcetClear(void *data , uintptr_t preHandlerReturn)
{
    /* disable the WCET timer (CNTV) */
    pharosCpuCntvCtlEl0Set(0 << 0);

    /* place one on the counter so that any pending interrupt is cleared */
    pharosCpuCntvTvalEl0Set(1);
}


void pharosCpuWcetClockFire(uint32_t delta)
{
    /* initialize the number of clock cycles needed for the delta interval */
    delta = ((uint64_t) delta * (uint64_t) cntfrq[pharosICoreRunning()]) / VALUE_1M - 1;

    /* set the time value to setup the clock as a 32-bit down-counter */
    pharosCpuCntvTvalEl0Set(delta);

    /* enable the WCET timer (CNTV) */
    pharosCpuCntvCtlEl0Set(1 << 0);
}


void pharosCpuWcetClockDisable(void)
{
    /* disable the WCET timer (CNTV) */
    pharosCpuCntvCtlEl0Set(0 << 0);
}
