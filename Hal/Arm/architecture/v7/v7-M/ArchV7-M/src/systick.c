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


#include <pharos/hal/v7m/systick.h>
#include <pharos/hal/boardApi.h>
#include <pharos/kernel/interrupt/interrupt.h>
#include <pharos/hal/v7m/nvic.h>
#include <pharos/kernel/time/time.h>
#include <pharos/kernel/clock/clockinline.h>
#include <pharos/hal/v7m/scb.h>
#include <pharos/hal/cpu/sections.h>


/**
 * Maximum reload value of a systick -> 24 bits
 */
#define PHAROS_CPU_SYSTICK_MAX_VALUE    (0xFFFFFFU)


/**
 * number of systick ISRs per clock tick (only used for the systickIsrComplex)
 */
KERNEL_SECTION_BSS static uint32_t isrNumber;


/**
 * current number of systick ISRs since the last clock tick (only used for the systickIsrComplex)
 */
KERNEL_SECTION_BSS static uint32_t currentIsrNumber;


/**
 * Complex systick ISR (multiple systick ISRs before a clock tick has elapsed)
 * 
 * @param argument ignored
 * @param preInterruptData ignored
 * @param number interrupt number (systick)
 * @param interruptedAddress address that was interrupted
 */
static void systickIsrComplex(void *argument , uintptr_t preInterruptData , PharosInterruptNumber number , const uint8_t *interruptedAddress);


/**
 * Single-systick ISR (only one systick ISR before a clock tick has elapsed)
 * 
 * @param argument ignored
 * @param preInterruptData ignored
 * @param number interrupt number (systick)
 * @param interruptedAddress address that was interrupted
 */
static void systickIsrSimple(void *argument , uintptr_t preInterruptData , PharosInterruptNumber number , const uint8_t *interruptedAddress);


/**
 * Calculate the reload value to place on the systick register
 * 
 * @param micro number of microseconds per clock tick
 * 
 * @return returns the reload value 
 */
static uint32_t pharosCpuSystickReloadCalc(uint32_t micro);


/**
 * Calculate the number of clock ISRs per clock tick
 * 
 * @param micro number of microseconds per clock tick
 * 
 * @return returns the number of systicks ISRs per clock tick
 */
static uint32_t pharosCpuSystickNumberIsrs(uint32_t micro);


/**
 * timer (thread time execution expired timer) 1 clear interrupt
 */
static void systickClearInterrupt(void *argument , uintptr_t preHandlerReturn);


/**
 * Initialize the systick hardware
 */
static void pharosCpuSystickInitHw(void);


void systickIsrComplex(void *argument , uintptr_t preInterruptData , PharosInterruptNumber number , const uint8_t *interruptedAddress)
{
    /* increase the number of systick ISR since the last clock tick */
    currentIsrNumber++;

    /* if the number of required systick ISRs has been reached for one clock tick */
    if(currentIsrNumber == isrNumber)
    {
        /* call time tick interrupt handler */
        pharosITimeTickIsr();

        /* and reset the number of systick ISR since the last clock tick */
        currentIsrNumber = 0;
    }
}


void systickIsrSimple(void *argument , uintptr_t preInterruptData , PharosInterruptNumber number , const uint8_t *interruptedAddress)
{
    /* call time tick interrupt handler */
    pharosITimeTickIsr();
}


void systickClearInterrupt(void *argument , uintptr_t preHandlerReturn)
{
    /* read the control to clear the pending interrupt */
    PHAROS_CPU_SYSTICK->controlAndStatus;
}


PHAROS_WEAK void pharosCpuClockReset(void)
{
    /* reload the timer */
    pharosCpuSystickInitHw();
}


uint32_t pharosCpuClockReadCounter(void)
{
    /* read the time till the next clock tick */
    return PHAROS_CPU_SYSTICK->currentValue / PHAROS_CPU_FREQ_MHZ;
}


uint64_t pharosCpuClockReadCounterRaw(void)
{
    /* use the standard method to read the counter */
    return pharosCpuClockReadCounter();
}


uint64_t pharosCpuClockMicrosecondsPerTickRaw(void)
{
    /* a tick occurs every configured microseconds per tick on this CPU */
    return pharosCpuSystickReloadCalc(pharosIMicrosecondsPerClockTick()) / PHAROS_CPU_FREQ_MHZ;
}


void pharosCpuSystickInitHw(void)
{
    /* reset the systick */
    PHAROS_CPU_SYSTICK->controlAndStatus = 0U;

    /* set the number of microseconds per tick */
    PHAROS_CPU_SYSTICK->reloadValue = pharosCpuSystickReloadCalc(pharosIMicrosecondsPerClockTick());

    /* set the first clock tick to fire within a clock tick */
    PHAROS_CPU_SYSTICK->currentValue = PHAROS_CPU_SYSTICK->reloadValue;

    /* enable the systick interrupt, using the CPU clock */
    PHAROS_CPU_SYSTICK->controlAndStatus = PHAROS_CPU_SYSTICK_CTRL_CPUCLK | PHAROS_CPU_SYSTICK_CTRL_TICKINT |
            PHAROS_CPU_SYSTICK_CTRL_ENABLE;

    /* read the control */
    PHAROS_CPU_SYSTICK->controlAndStatus;
}


void pharosCpuSystickInit(void)
{
    /* global variable used only in this function with the timer 1 interrupt configuration */
    static KERNEL_SECTION PartitionIsrConf timer1PartitionConf;

    /* global variable used only in this function with the timer 1 interrupt configuration */
    static KERNEL_SECTION InterruptConfiguration timer1;


    /* calculate the number of interrupts required for one clock tick */
    isrNumber = pharosCpuSystickNumberIsrs(pharosIMicrosecondsPerClockTick());

    /* check if only one interrupt per tick is needed */
    if(isrNumber == 1U)
    {
        /* initialize timer1 configuration with the simple handler (one interrupt per tick) */
        timer1PartitionConf.isrHandler = systickIsrSimple;
    }
        /* else, more than one clock tick is required */
    else
    {
        /* initialize timer1 configuration with the handler (multiple interrupts per tick) */
        timer1PartitionConf.isrHandler = systickIsrComplex;
    }

    /* timer1 is not from a partition */
    timer1PartitionConf.partition = NULL;

    /* set the interrupt configuration for the clock interrupt */
    timer1PartitionConf.interruptConf = &timer1;

    /* setup the interrupt number for the RTI compare 0*/
    timer1.number = PHAROS_CPU_EXCEPTION_SYSTICK;

    /* highest priority (besides the ESM and reserved which are fixed) */
    timer1.cpu.priority = PHAROS_INTERRUPT_PRIO_APP_LOW;

    /* set the clear interrupt routine for the periodic clock */
    timer1.postInterruptHandler = systickClearInterrupt;

    /* not installed */
    timer1.isInstalled = FALSE;

    /* catch the interrupt for the OS periodic clock */
    pharosIInterruptInstallInit(&timer1 , &timer1PartitionConf);

    /* init the systick hardware */
    pharosCpuSystickInitHw();
}


bool pharosCpuClockIsrIsPending(void)
{
    /* read the pending status on the ICSR register, PENDSTSET bit */
    return (PHAROS_CPU_SCB->interruptControl & (1 << 26)) == 0 ? FALSE : TRUE;
}


uint32_t pharosCpuSystickReloadCalc(uint32_t micro)
{
    /* if the clock tick fits in 2^24 / PHAROS_CPU_FREQ_MHZ */
    if(PHAROS_CPU_FREQ_MHZ * micro < PHAROS_CPU_SYSTICK_MAX_VALUE)
    {
        /* reload value is equal to the number of microseconds times the clock speed */
        return PHAROS_CPU_FREQ_MHZ * micro;
    }
    else
    {
        /* trigger an interrupt each maximum value permitted (e.g. 100 ms or 10 ms) */
        return PHAROS_CPU_FREQ_MHZ * PHAROS_CPU_SYSTICK_MAX;
    }
}


uint32_t pharosCpuSystickNumberIsrs(uint32_t micro)
{
    /* if the clock tick fits in 2^24 / PHAROS_CPU_FREQ_MHZ */
    if(PHAROS_CPU_FREQ_MHZ * micro < PHAROS_CPU_SYSTICK_MAX_VALUE)
    {
        /* trigger only one interrupt per clock tick */
        return 1U;
    }
    else
    {
        /* calculate the number of interrupts needed for each tick */
        return micro / PHAROS_CPU_SYSTICK_MAX;
    }
}
