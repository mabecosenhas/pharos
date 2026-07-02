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
#include <pharos/kernel/clock/clock.h>
#include <pharos/kernel/time/time.h>
#include <pharos/hal/boardApi.h>
#include <pharos/kernel/interrupt/interrupt.h>
#include <pharos/hal/cpu/conf.h>
#include <pharos/hal/cpu/declarations.h>
#include <pharos/hal/cpu/timer.h>
#include <pharos/hal/cpu/rcc.h>

/**
 * WCET interrupt handler
 * 
 * @param data 
 */
static uintptr_t timer5PreInterrupt(void *data);


/**
 * WCET interrupt handler
 * 
 * @param argument
 * @param partitionData
 * @param number
 * @param interruptedAddress
 */
static void timer5Isr(void *argument , uintptr_t preInterruptData , PharosInterruptNumber number , const uint8_t *interruptedAddress);


uintptr_t timer5PreInterrupt(void *data)
{
    /* clear the UIF flag */
    PHAROS_CPU_TIMER_5->status = 0;
    
    return 0;
}


void timer5Isr(void *argument , uintptr_t preInterruptData , PharosInterruptNumber number , const uint8_t *interruptedAddress)
{
    /* call WCET handler */
    pharosIWcetHandlerIsr(interruptedAddress);
}


void pharosCpuTimerWcetInit(void)
{
    /* global variable used only in this function with the timer 1 interrupt configuration */
    static KERNEL_SECTION PartitionIsrConf timer5PartitionConf;

    /* global variable used only in this function with the timer 1 interrupt configuration */
    static KERNEL_SECTION InterruptConfiguration timer5Conf;


    /* initialize timer1 configuration with the simple handler (one interrupt per tick) */
    timer5PartitionConf.isrHandler = timer5Isr;

    /* timer1 is not from a partition */
    timer5PartitionConf.partition = NULL;

    /* set the interrupt configuration for the clock interrupt */
    timer5PartitionConf.interruptConf = &timer5Conf;

    /* setup the interrupt number for timer 5 */
    timer5Conf.number = PHAROS_CPU_ISR_TIMER5;

    /* low priority */
    timer5Conf.cpu.priority = PHAROS_INTERRUPT_PRIO_APP_LOW - 1;

    /* set the clear interrupt routine for the periodic clock */
    timer5Conf.preInterruptHandler = timer5PreInterrupt;
    
    /* reset the post interrupt handler */
    timer5Conf.postInterruptHandler = NULL;

    /* initialize Timer 5 (WCET timer) clock peripheral */
    PHAROS_CPU_RCC->apb1PeripheralClockEnable |= PHAROS_CPU_RCC_APB1ENR_TIM5_EN;

    /* clear the update event flags */
    PHAROS_CPU_TIMER_5->status = 0;

    /* set the pre-scalar to the CPU frequency (so that it generates a 1us tick) */
    PHAROS_CPU_TIMER_5->prescalar = PHAROS_CPU_APB2_FREQ_MHZ - 1U;

    /* reset the control 1 register */
    PHAROS_CPU_TIMER_5->control1 = PHAROS_CPU_TIM_CR1_RESET;

    /* reset the control 2 register */
    PHAROS_CPU_TIMER_5->control2 = PHAROS_CPU_TIM_CR2_RESET;

    /* reset the slave mode, including the SMS (slave mode disabled) */
    PHAROS_CPU_TIMER_5->slaveModeControl = PHAROS_CPU_TIM_SMCR_RESET;

    /* reset the interrupts and DMA triggers */
    PHAROS_CPU_TIMER_5->dmaInterruptEnable = PHAROS_CPU_TIM_DIER_RESET;

    /* load the number of microseconds to wait for */
    PHAROS_CPU_TIMER_5->autoReload = 10U * 1000U * 1000U;

    /* force the reload of the pre-scalar value */
    PHAROS_CPU_TIMER_5->eventGeneration = 1U;

    /* enable the update interrupt */
    PHAROS_CPU_TIMER_5->dmaInterruptEnable &= 0x00;

    /* clear the update event flags */
    PHAROS_CPU_TIMER_5->status = 0;

    /* reset the event generation flags */
    PHAROS_CPU_TIMER_5->eventGeneration = 0U;

    /* disable the timer */
    PHAROS_CPU_TIMER_5->control1 = PHAROS_CPU_TIM_CR1_CEN_DIS;

    /* enable the interrupt */
    PHAROS_CPU_TIMER_5->dmaInterruptEnable |= PHAROS_CPU_TIM_DIER_UIE_EN;

    /* catch the interrupt for the OS periodic clock */
    pharosIInterruptInstallInit(&timer5Conf , &timer5PartitionConf);
}


void pharosCpuWcetClockFire(uint32_t delta)
{
    /* load the number of microseconds to wait for */
    PHAROS_CPU_TIMER_5->autoReload = delta - 1;

    /* enable the timer to fire in delta us */
    PHAROS_CPU_TIMER_5->control1 = PHAROS_CPU_TIM_CR1_DIR_UP | PHAROS_CPU_TIM_CR1_URS_OU |
            PHAROS_CPU_TIM_CR1_UDIS_EN | PHAROS_CPU_TIM_CR1_ARPE_EN |
            PHAROS_CPU_TIM_CR1_CMS_EDGE | PHAROS_CPU_TIM_CR1_CEN_EN | PHAROS_CPU_TIM_CR1_OPM_ONE;
}


void pharosCpuWcetClockDisable(void)
{
    /* disable the WCET Timer */
    PHAROS_CPU_TIMER_5->control1 = PHAROS_CPU_TIM_CR1_CEN_DIS;
}
