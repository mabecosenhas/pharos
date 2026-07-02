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


#include <pharos/hal/board/conf.h>
#include <pharos/kernel/clock/clockinline.h>
#include <pharos/hal/clockSp804.h>
#include <pharos/kernel/time/time.h>
#include <pharos/hal/boardApi.h>
#include <pharos/hal/vicPl190.h>
#include <pharos/kernel/interrupt/interrupt.h>
#include <pharos/hal/cpu/sections.h>


/**
 * ISR handler for both Clock tick interrupt and WCET timer interrupt (timer 0 and timer 1)
 * 
 * @param number will be TIMER1_INTERRUPT_VECTOR_NUMBER = TIMER2_INTERRUPT_VECTOR_NUMBER
 */

static void timer1And2Isr(void *a , uintptr_t data , PharosInterruptNumber number , const uint8_t *address);


/**
 * Pre interrupt for timer 1 and timer 2
 */
static uintptr_t timer1And2PreInterrupt(void *arg);


PHAROS_WEAK void pharosCpuClockReset(void)
{
    /* reload the timer */
    armSp804TimerInitPeriodic32Bit(pharosIMicrosecondsPerClockTick() , PHAROS_TIMER1_BASE_ADDRESS);
}


PHAROS_WEAK void pharosCpuClockInitialize(void)
{
    /* global variable used only in this function with the timer 1 interrupt configuration */
    static KERNEL_SECTION PartitionIsrConf timer1PartitionConf;

    /* global variable used only in this function with the timer 1 interrupt configuration */
    static KERNEL_SECTION InterruptConfiguration timer1;


    /* initialize timer1 configuration with the handler */
    timer1PartitionConf.isrHandler = timer1And2Isr;

    /* timer1 is not from a partition */
    timer1PartitionConf.partition = NULL;

    /* set the interrupt configuration */
    timer1PartitionConf.interruptConf = &timer1;

    /* setup the interrupt number for the timer 1 */
    timer1.number = PHAROS_INTERRUPT_NUMBER_TIMER_01;

    /* highest priority */
    timer1.cpu.priority = PHAROS_INTERRUPT_PRIORITY_0;

    /* set the pre interrupt that clears the interrupt */
    timer1.preInterruptHandler = timer1And2PreInterrupt;

    /* reset the post interrupt routine */
    timer1.postInterruptHandler = NULL;

    /* not installed */
    timer1.isInstalled = FALSE;

    /* reset the argument */
    timer1.argument = NULL;

    /* catch the interrupt (no need to catch for timer2 since timer1 and timer2 shared the same interrupt number) */
    pharosIInterruptInstallInit(&timer1 , &timer1PartitionConf);

    /* initialize the timer 1 to generate a periodic interrupt */
    armSp804TimerInitPeriodic32Bit(pharosIMicrosecondsPerClockTick() , PHAROS_TIMER1_BASE_ADDRESS);
}


uintptr_t timer1And2PreInterrupt(void *arg)
{
    /* return value */
    uintptr_t res = 0;


    /* if the interrupt was from timer 1 */
    if(armSp804TimerInterruptIsPending(PHAROS_TIMER1_BASE_ADDRESS) == TRUE)
    {
        /* ack timer 1 */
        armSp804TimerClearInterrupt(PHAROS_TIMER1_BASE_ADDRESS);

        /* set the first bit */
        res |= 1U;
    }

    /* if the interrupt was from timer 2 */
    if(armSp804TimerInterruptIsPending(PHAROS_TIMER2_BASE_ADDRESS) == TRUE)
    {
        /* ack timer 2 */
        armSp804TimerClearInterrupt(PHAROS_TIMER2_BASE_ADDRESS);

        /* set the second bit */
        res |= 2U;
    }

    /* return 0, 1, 2 or 3 depending on the interrupt(s) that was triggered */
    return res;
}


void timer1And2Isr(void *a , uintptr_t preInterruptData , PharosInterruptNumber number , const uint8_t *address)
{
    /* if the interrupt was from timer 1 */
    if((preInterruptData & 0x1U) != 0)
    {
        /* call time tick interrupt handler */
        pharosITimeTickIsr();
    }

    /* if the interrupt was from timer 2 */
    if((preInterruptData & 0x2U) != 0)
    {
        /* call WCET handler */
        pharosIWcetHandlerIsr(address);
    }
}


uint32_t pharosCpuClockReadCounter(void)
{
    /* read the SP804 timer internal counter */
    return armSp804TimerRead(PHAROS_TIMER1_BASE_ADDRESS);
}


bool pharosCpuClockIsrIsPending(void)
{
    /* read the SP804 timer interrupt pending info */
    return armSp804TimerInterruptIsPending(PHAROS_TIMER1_BASE_ADDRESS);
}


uint64_t pharosCpuClockReadCounterRaw(void)
{
    /* use the standard method to read the counter */
    return pharosCpuClockReadCounter();
}


uint64_t pharosCpuClockMicrosecondsPerTickRaw(void)
{
    /* a tick occurs every configured microseconds per tick on this CPU */
    return pharosIMicrosecondsPerClockTick();
}


void pharosCpuWcetClockFire(uint32_t delta)
{
    /* set the SP804 to one shot on the timer2 */
    armSp804TimerInitOneShot32Bit(delta , PHAROS_TIMER2_BASE_ADDRESS);
}


void pharosCpuWcetClockDisable(void)
{
    /* disable the timer2 on the SP804 */
    armSp804TimerDisable(PHAROS_TIMER2_BASE_ADDRESS);

    /* clear any pending interrupt of timer 2 */
    armSp804TimerClearInterrupt(PHAROS_TIMER2_BASE_ADDRESS);
}
