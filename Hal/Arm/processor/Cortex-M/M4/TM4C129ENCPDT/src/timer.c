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
#include <pharos/hal/cpu/cpu.h>
#include <pharos/hal/cpu/rom.h>
#include <pharos/kernel/interrupt/interrupt.h>
#include <pharos/hal/v7m/nvic.h>
#include <pharos/kernel/time/time.h>
#include <pharos/kernel/clock/clock.h>
#include <pharos/hal/boardApi.h>
#include <pharos/hal/cpu/peripheral.h>
#include <pharos/hal/cpu/sections.h>


typedef bool(*InterruptMasterEnable)(void);

typedef void (*TimerConfigure) (uint32_t ui32Port , uint32_t ui8Pins);

typedef void (*TimerLoadSet)(uint32_t ui32Base , uint32_t ui32Timer , uint32_t ui32Value);

typedef void (*InterruptEnable)(uint32_t ui32Interrupt);

typedef void (*TimerIntEnable)(uint32_t ui32Base , uint32_t ui32IntFlags);

typedef void (*TimerLoadSet)(uint32_t ui32Base , uint32_t ui32Timer , uint32_t ui32Value);

typedef uint32_t(*TimerLoadGet)(uint32_t ui32Base , uint32_t ui32Timer);

typedef void (*TimerEnable)(uint32_t ui32Base , uint32_t ui32Timer);

typedef void (*TimerIntDisable)(uint32_t ui32Base , uint32_t ui32IntFlags);

typedef void (*TimerIntClear)(uint32_t ui32Base , uint32_t ui32IntFlags);

typedef void (*TimerDisable)(uint32_t ui32Base , uint32_t ui32Timer);


static uintptr_t timer0PreInterrupt(void *data);

static void timer0Isr(void *argument , uintptr_t preHandlerReturn , PharosInterruptNumber number , const uint8_t *interruptedAddress);


bool pharosCpuInterruptMasterEnable(void)
{
    /* use the ROM function to enable CPU interrupts */
    return ((InterruptMasterEnable) ROM_INTERRUPTTABLE[1])();
}


void pharosCpuInterruptEnable(PharosInterruptNumber interruptNumber)
{
    /* use the ROM function to enable the specific interrupt */
    ((InterruptEnable) ROM_INTERRUPTTABLE[0])(interruptNumber);
}


void pharosCpuTimerIntEnable(PharosCpuTimerAddress timerAddress , PharosCpuTimerIntConf intConf)
{
    /* use the ROM function to enable the timer interrupt with the specified flasgs */
    ((TimerIntEnable) ROM_TIMERTABLE[19])(timerAddress , intConf);
}


void pharosCpuTimerLoadSet(PharosCpuTimerAddress timerAddress , PharosCpuTimerSelect timer , uint32_t value)
{
    /* use the ROM function to load the value to the timer */
    ((TimerLoadSet) ROM_TIMERTABLE[14])(timerAddress , timer , value);
}


uint32_t pharosCpuTimerLoadGet(PharosCpuTimerAddress timerAddress , PharosCpuTimerSelect timer)
{
    /* use the ROM function to get the loaded value from the timer */
    return ((TimerLoadGet) ROM_TIMERTABLE[15])(timerAddress , timer);
}


void pharosCpuTimerEnable(PharosCpuTimerAddress timerAddress , PharosCpuTimerSelect timer)
{
    /* use the ROM function to enable the specified timer */
    ((TimerEnable) ROM_TIMERTABLE[1])(timerAddress , timer);
}


void pharosCpuTimerConfigure(PharosCpuTimerAddress timerAddress , PharosCpuTimerConfiguration configuration)
{
    /* use the ROM function to configure the specified timer */
    ((TimerConfigure) ROM_TIMERTABLE[3])(timerAddress , configuration);
}


void pharosCpuTimerIntClear(PharosCpuTimerAddress timerAddress , PharosCpuTimerIntConf intConf)
{
    /* use the ROM function to clear the interrupt */
    ((TimerIntClear) ROM_TIMERTABLE[0])(timerAddress , intConf);
}


void pharosCpuTimerIntDisable(PharosCpuTimerAddress timerAddress , PharosCpuTimerSelect timer)
{
    /* use the ROM function to  */
    ((TimerIntDisable) ROM_TIMERTABLE[20])(timerAddress , timer);
}


void pharosCpuTimerDisable(PharosCpuTimerAddress timerAddress , PharosCpuTimerSelect timer)
{
    /* use the ROM function to disable the timer */
    ((TimerDisable) ROM_TIMERTABLE[2])(timerAddress , timer);
}


uintptr_t timer0PreInterrupt(void *data)
{
    /* clear the interrupt of timer 0 for timer A timeout */
    pharosCpuTimerIntClear(PHAROS_CPU_TIMER0_BASE , PHAROS_CPU_TIMER_TIMA_TIMEOUT);

    return 0U;
}


void timer0Isr(void *argument , uintptr_t preHandlerReturn , PharosInterruptNumber number , const uint8_t *interruptedAddress)
{
    /* call WCET handler */
    pharosIWcetHandlerIsr(interruptedAddress);
}


void pharosCpuTimerWcetInit(void)
{
    /* global variable used only in this function with the timer 1 interrupt configuration */
    static KERNEL_SECTION PartitionIsrConf timer1PartitionConf;

    /* global variable used only in this function with the timer 1 interrupt configuration */
    static KERNEL_SECTION InterruptConfiguration timer1;

    /* initialize timer1 configuration with the simple handler (one interrupt per tick) */
    timer1PartitionConf.isrHandler = timer0Isr;

    /* timer1 is not from a partition */
    timer1PartitionConf.partition = NULL;

    /* set the interrupt configuration for the clock interrupt */
    timer1PartitionConf.interruptConf = &timer1;

    /* setup the interrupt number for the RTI compare 0*/
    timer1.number = PHAROS_CPU_ISR_TIMER0A;

    /* low priority */
    timer1.cpu.priority = PHAROS_INTERRUPT_PRIO_APP_LOW - 1;

    /* set the clear interrupt routine for the periodic clock */
    timer1.preInterruptHandler = timer0PreInterrupt;

    /* reset the post interrupt handler */
    timer1.postInterruptHandler = NULL;

    /* not installed */
    timer1.isInstalled = FALSE;

    /* catch the interrupt for the OS periodic clock */
    pharosIInterruptInstallInit(&timer1 , &timer1PartitionConf);

    /* enable the timer 0 */
    pharosCpuPeripheralEnable(PHAROS_CPU_SYSCTL_PERIPH_TIMER0);

    /* disable the WCET timer */
    pharosCpuTimerDisable(PHAROS_CPU_TIMER0_BASE , PHAROS_CPU_TIMER_SELECT_A);

    /* configure the WCET Timer to be one-shot */
    pharosCpuTimerConfigure(PHAROS_CPU_TIMER0_BASE , PHAROS_CPU_TIMER_CFG_ONE_SHOT);

    /* enable interrupts for timer 0A (WCET timer) */
    pharosCpuInterruptEnable(PHAROS_CPU_ISR_TIMER0A);

    /* enable timer A interrupt to fire on a timer A timeout */
    pharosCpuTimerIntEnable(PHAROS_CPU_TIMER0_BASE , PHAROS_CPU_TIMER_TIMA_TIMEOUT);
}


void pharosCpuWcetClockFire(uint32_t delta)
{
    /* load the number of microseconds to wait for */
    pharosCpuTimerLoadSet(PHAROS_CPU_TIMER0_BASE , PHAROS_CPU_TIMER_SELECT_A , PHAROS_CPU_FREQ_MHZ * delta);

    /* enable the timer to fire in delta us */
    pharosCpuTimerEnable(PHAROS_CPU_TIMER0_BASE , PHAROS_CPU_TIMER_SELECT_A);
}


void pharosCpuWcetClockDisable(void)
{
    /* disable the WCET Timer */
    pharosCpuTimerDisable(PHAROS_CPU_TIMER0_BASE , PHAROS_CPU_TIMER_SELECT_A);
}
