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


#include <pharos/hal/cpu/cpu.h>
#include <pharos/kernel/clock/clockinline.h>
#include <pharos/kernel/time/time.h>
#include <pharos/hal/boardApi.h>
#include <pharos/kernel/interrupt/interrupt.h>
#include <pharos/hal/interrupt/vim.h>
#include <pharos/hal/cpu/rti.h>
#include <pharos/hal/cpu/conf.h>
#include <pharos/hal/cpu/sections.h>


#define PHAROS_CPU_UPDATE_COMPARE1_VAL      (0U)

static void timer1Isr(void *argument , uintptr_t preHandlerReturn , PharosInterruptNumber number , const uint8_t *interruptedAddress);
static void timer2Isr(void *argument , uintptr_t preHandlerReturn , PharosInterruptNumber number , const uint8_t *interruptedAddress);
static uintptr_t timer1PreInterrupt(void *data);
static uintptr_t timer2PreInterrupt(void *data);


void pharosRtiHwInit(void)
{
    /* disable both RTI0 and RTI1, halt the clock on debug and select the PLL2 as the clock source */
    PHAROS_CPU_RTI->globalControl = PHAROS_CPU_RTI_GCTRL_DIS(0U) | PHAROS_CPU_RTI_GCTRL_DIS(1U) |
            PHAROS_CPU_RTI_GCTRL_COS_DIS | PHAROS_CPU_RTI_GCTRL_PLL2;

    /* use RTIUC0 as the clock source for RTIFRC0, don't increment free running counter 0 */
    PHAROS_CPU_RTI->timebaseControl = PHAROS_CPU_RTI_TBCTRL_NOINC | PHAROS_CPU_RTI_TBCTRL_RTIUC0;

    /* make all the compare registers compare with free running counter 0 */
    PHAROS_CPU_RTI->compareControl = PHAROS_CPU_RTI_COMPCTRL_CMP0C0 | PHAROS_CPU_RTI_COMPCTRL_CMP1C0 |
            PHAROS_CPU_RTI_COMPCTRL_CMP2C0 | PHAROS_CPU_RTI_COMPCTRL_CMP3C0;

    /* reset the up counter 0 */
    PHAROS_CPU_RTI->upCounter0 = 0U;

    /* reset the up counter 1 */
    PHAROS_CPU_RTI->upCounter1 = 0U;

    /* reset the free running counter 0 */
    PHAROS_CPU_RTI->freeRunningCounter0 = 0;

    /* reset the free running counter 1 */
    PHAROS_CPU_RTI->freeRunningCounter1 = 0;

    /* set the prescalar to source input [MHz] minus 1. That is the source clock is running at 75 MHz, 
     * then the output of the prescalar comes out at 1 MHz (1 us) */
    PHAROS_CPU_RTI->compareUpCounter0 = 74U;

    /* set the prescalar to source input [MHz] minus 1. That is the source clock is running at 75 MHz, 
     * then the output of the prescalar comes out at 1 MHz (1 us) */
    PHAROS_CPU_RTI->compareUpCounter1 = 74U;

    /* set the current compare value */
    PHAROS_CPU_RTI->compare0 = pharosIMicrosecondsPerClockTick();

    /* initialize the compare for compare 1 with some value (compare 1 ISRs will not be enabled now) */
    PHAROS_CPU_RTI->compare1 = 0;

    /* set the reload value of the compare register once the compare is reached. This makes the timer periodic */
    PHAROS_CPU_RTI->updateCompare0 = pharosIMicrosecondsPerClockTick();

    /* set the reload value of the compare register to make it one shot every us 
     * (if the delta is too small, then this will make the interrupt be triggered a little bit later)
     * if this value was zero, then if delta is 1, at the time the interrupt is enabled, the free running counter could have 
     * be bigger than the counter and the interrupt would not be triggered. With one we are always sure it will be triggered
     * at some point. The interrupt clear (timer2ClearInterrupt) will then disable the WCET interrupt */
    PHAROS_CPU_RTI->updateCompare1 = PHAROS_CPU_UPDATE_COMPARE1_VAL;

    /* disable interrupts for RTI0 and RTI1, timebase interrupt and overflow interrupts */
    PHAROS_CPU_RTI->interruptClear = PHAROS_CPU_RTI_CLEARINTENA_INT(0U) | PHAROS_CPU_RTI_CLEARINTENA_INT(1U) |
            PHAROS_CPU_RTI_CLEARINTENA_TB | PHAROS_CPU_RTI_CLEARINTENA_OVL(0U) | PHAROS_CPU_RTI_CLEARINTENA_OVL(1U);

    /* clear pending interrupts, timebase interrupt and overflow interrupts */
    PHAROS_CPU_RTI->interruptFlag = PHAROS_CPU_RTI_INTFLAG_INT_CLR(0U) | PHAROS_CPU_RTI_INTFLAG_INT_CLR(1U) |
            PHAROS_CPU_RTI_INTFLAG_TB | PHAROS_CPU_RTI_INTFLAG_OVL(0U) | PHAROS_CPU_RTI_INTFLAG_OVL(1U);

    /* enable the periodic clock interrupt generation (for the clock interrupt) */
    PHAROS_CPU_RTI->interruptSet = PHAROS_CPU_RTI_SETINTENA_INT(0U);

    /* enable the RTI0 clock (Pharos internal clock timer) */
    PHAROS_CPU_RTI->globalControl |= PHAROS_CPU_RTI_GCTRL_EN(0U);
}


PHAROS_WEAK void pharosCpuClockInitialize(void)
{
    /* global variable used only in this function with the timer 1 interrupt configuration */
    static KERNEL_SECTION PartitionIsrConf timer1PartitionConf;
    static KERNEL_SECTION PartitionIsrConf timer2PartitionConf;

    /* global variable used only in this function with the timer 1 interrupt configuration */
    static KERNEL_SECTION InterruptConfiguration timer1;
    static KERNEL_SECTION InterruptConfiguration timer2;

    /* initialize timer1 configuration with the handler */
    timer1PartitionConf.isrHandler = timer1Isr;
    timer2PartitionConf.isrHandler = timer2Isr;

    /* timer1 is not from a partition */
    timer1PartitionConf.partition = NULL;

    /* timer2 (WCET is not for an interrupt) */
    timer2PartitionConf.partition = NULL;

    /* set the interrupt configuration for the clock interrupt */
    timer1PartitionConf.interruptConf = &timer1;

    /* set the interrupt configuration for the WCET interrupt */
    timer2PartitionConf.interruptConf = &timer2;

    /* setup the interrupt number for the RTI compare 0*/
    timer1.number = PHAROS_CPU_ISR_RTI0_CMP;

    /* setup the interrupt number for the RTI compare 1 */
    timer2.number = PHAROS_CPU_ISR_RTI1_CMP;

    /* highest priority (besides the ESM and reserved which are fixed) */
    timer1.cpu.channel = PHAROS_INTERRUPT_CHANNEL_2;

    /* set a low interrupt priority for WCET (no need to set it high) */
    timer2.cpu.channel = PHAROS_INTERRUPT_CHANNEL_126;

    /* set the clear interrupt routine for the periodic clock */
    timer1.preInterruptHandler = timer1PreInterrupt;

    /* reset the post interrupt handler */
    timer1.postInterruptHandler = NULL;

    /* set the clear interrupt routine for the WCET */
    timer2.preInterruptHandler = timer2PreInterrupt;

    /* reset the post interrupt handler */
    timer2.postInterruptHandler = NULL;

    /* set the clock interrupt to wake up the CPU if it is dormant */
    timer1.cpu.isWakeUp = TRUE;

    /* set the WCET interrupt to don't wake up the CPU (if the interrupt is active that means that there must be a 
     * thread running and hence the CPU is not dormant) */
    timer2.cpu.isWakeUp = FALSE;

    /* not installed */
    timer1.isInstalled = FALSE;

    /* not installed */
    timer2.isInstalled = FALSE;

    /* install the interrupt configuration for timer 1 */
    pharosCpuVimInitializeIsrConf(&timer1);

    /* install the interrupt configuration for timer 2 */
    pharosCpuVimInitializeIsrConf(&timer2);

    /* catch the interrupt for the OS periodic clock */
    pharosIInterruptInstallInit(&timer1 , &timer1PartitionConf);

    /* catch the interrupt for WCET */
    pharosIInterruptInstallInit(&timer2 , &timer2PartitionConf);

    /* init the hardware */
    pharosRtiHwInit();
}


uintptr_t timer1PreInterrupt(void *data)
{
    /* clear RTI 0 interrupt */
    PHAROS_CPU_RTI->interruptFlag = PHAROS_CPU_RTI_INTFLAG_CLEAR(0U);

    return 0;
}


uintptr_t timer2PreInterrupt(void *data)
{
    /* disable the interrupt ("update compare 1" is 1, which means that the interrupt will be triggered as soon as we clear it) */
    PHAROS_CPU_RTI->interruptClear = PHAROS_CPU_RTI_CLEARINTENA_INT(1U);
    PHAROS_CPU_RTI->interruptClear;

    /* clear RTI 1 interrupt */
    PHAROS_CPU_RTI->interruptFlag = PHAROS_CPU_RTI_INTFLAG_CLEAR(1U);

    PHAROS_CPU_RTI->interruptFlag;
    pharosCpuDsb();
    pharosCpuIsb();

    return 0;
}


void timer1Isr(void *argument , uintptr_t preHandlerReturn , PharosInterruptNumber number , const uint8_t *interruptedAddress)
{
    /* call time tick interrupt handler */
    pharosITimeTickIsr();
}


void timer2Isr(void *argument , uintptr_t preHandlerReturn , PharosInterruptNumber number , const uint8_t *interruptedAddress)
{
    /* call WCET handler */
    pharosIWcetHandlerIsr(interruptedAddress);
}


void pharosCpuClockReset()
{
    /* init the hardware */
    pharosRtiHwInit();
}


uint32_t pharosCpuClockReadCounter(void)
{
    /* read the time till the next clock tick */
    return PHAROS_CPU_RTI->compare0 - PHAROS_CPU_RTI->freeRunningCounter0;
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


bool pharosCpuClockIsrIsPending(void)
{
    /* get the "is pending" bit and check if it is set */
    return ((PHAROS_CPU_RTI->interruptFlag & PHAROS_CPU_RTI_INTFLAG_IS_PEND(0U)) == PHAROS_CPU_RTI_INTFLAG_IS_PEND(0U)) ? TRUE : FALSE;
}


void pharosCpuWcetClockFire(uint32_t delta)
{
    /* disable the interrupt */
    PHAROS_CPU_RTI->interruptClear = PHAROS_CPU_RTI_CLEARINTENA_INT(1U);
    PHAROS_CPU_RTI->interruptClear;

    /* clear any pending timer 1 interrupt */
    PHAROS_CPU_RTI->interruptFlag = PHAROS_CPU_RTI_INTFLAG_INT_CLR(1U);
    PHAROS_CPU_RTI->interruptFlag;

    pharosCpuDsb();
    pharosCpuIsb();

    /* set the current compare value to the free running counter plus the delta interval */
    PHAROS_CPU_RTI->compare1 = delta + PHAROS_CPU_RTI->freeRunningCounter0;
    PHAROS_CPU_RTI->compare1;

    pharosCpuDsb();
    pharosCpuIsb();

    /* set the interrupt */
    PHAROS_CPU_RTI->interruptSet = PHAROS_CPU_RTI_SETINTENA_INT(1U);
    PHAROS_CPU_RTI->interruptSet;

    pharosCpuDsb();
    pharosCpuIsb();
}


void pharosCpuWcetClockDisable(void)
{
    /* disable the interrupt compare 1 */
    PHAROS_CPU_RTI->interruptClear = PHAROS_CPU_RTI_CLEARINTENA_INT(1U);

    pharosCpuDsb();
    pharosCpuIsb();
}
