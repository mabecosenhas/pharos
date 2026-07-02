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


#ifndef PHAROS_CPU_TIMER_H
#define	PHAROS_CPU_TIMER_H

#include <pharos/hal/cpu/declarations.h>

#define PHAROS_CPU_TIMER_5                   ((ptrPharosCpuGpTimer2_5) PHAROS_CPU_TIM5_ADDRESS)
#define PHAROS_CPU_TIMER_2                   ((ptrPharosCpuGpTimer2_5) PHAROS_CPU_TIM2_ADDRESS)


#define PHAROS_CPU_TIM_CR1_RESET            (0U)
#define PHAROS_CPU_TIM_CR1_OPM_ONE          (1U << 3U)
#define PHAROS_CPU_TIM_CR1_UDIS_EN          (0U << 1U)
#define PHAROS_CPU_TIM_CR1_UDIS_DIS         (1U << 1U)
#define PHAROS_CPU_TIM_CR1_URS_OU           (1U << 2U)
#define PHAROS_CPU_TIM_CR1_DIR_UP           (0U << 4U)
#define PHAROS_CPU_TIM_CR1_DIR_DOWN         (1U << 4U)
#define PHAROS_CPU_TIM_CR1_CMS_EDGE         (0U << 5U)
#define PHAROS_CPU_TIM_CR1_ARPE_EN          (1U << 7U)
#define PHAROS_CPU_TIM_CR1_ARPE_DIS         (0U << 7U)
#define PHAROS_CPU_TIM_CR1_CMS_CENTER1      (1U << 5U)

#define PHAROS_CPU_TIM_CR1_CEN_EN           (1 << 0)
#define PHAROS_CPU_TIM_CR1_CEN_DIS          (0 << 0)


#define PHAROS_CPU_TIM_CR2_RESET            (0U)

#define PHAROS_CPU_TIM_SMCR_RESET           (0U)

#define PHAROS_CPU_TIM_DIER_RESET           (0U)
#define PHAROS_CPU_TIM_DIER_UIE_EN          (1U << 0U)
#define PHAROS_CPU_TIM_DIER_TIE_EN          (1U << 6U)

#define PHAROS_CPU_TIM_SR_UIF_MASK          (~((uint32_t) (1U << 0U)))


struct PharosCpuGpTimer2_5
{
    /**
     * TIMx control register 1 (TIMx_CR1)
     * 
     * offset:        0x00
     * Reset value: 0x0000
     */
    volatile uint32_t control1;

    /**
     * TIMx control register 2 (TIMx_CR2)
     * 
     * offset:        0x04
     * Reset value: 0x0000
     */
    volatile uint32_t control2;

    /**
     * TIMx slave mode control register (TIMx_SMCR)
     * 
     * offset:        0x08
     * Reset value: 0x0000
     */
    volatile uint32_t slaveModeControl;

    /**
     * TIMx DMA/Interrupt enable register (TIMx_DIER)
     * 
     * offset:        0x0c
     * Reset value: 0x0000
     */
    volatile uint32_t dmaInterruptEnable;

    /**
     * TIMx status register (TIMx_SR)
     * 
     * offset:        0x10
     * Reset value: 0x0000
     */
    volatile uint32_t status;

    /**
     * TIMx event generation register (TIMx_EGR)
     * 
     * offset:        0x14
     * Reset value: 0x0000
     */
    volatile uint32_t eventGeneration;

    /**
     * TIMx capture/compare mode register 1 (TIMx_CCMR1)
     * 
     * offset:        0x18
     * Reset value: 0x0000
     */
    volatile uint32_t captureCompareMode1;

    /**
     * TIMx capture/compare mode register 2 (TIMx_CCMR2)
     * 
     * offset:        0x1c
     * Reset value: 0x0000
     */
    volatile uint32_t captureCompareMode2;

    /**
     * TIMx capture/compare enable register (TIMx_CCER)
     * 
     * offset:        0x20
     * Reset value: 0x0000
     */
    volatile uint32_t captureCompareEnable;

    /**
     * TIMx counter (TIMx_CNT)
     * 
     * offset:        0x24
     * Reset value: 0x0000
     */
    volatile uint32_t counter;

    /**
     * TIMx prescaler (TIMx_PSC)
     * 
     * offset:        0x28
     * Reset value: 0x0000
     */
    volatile uint32_t prescalar;

    /**
     * TIMx auto-reload register (TIMx_ARR)
     * 
     * offset:        0x2c
     * Reset value: 0x0000
     */
    volatile uint32_t autoReload;

    /**
     * Reserved
     */
    volatile uint32_t reserved2;

    /**
     * TIMx capture/compare register 1 (TIMx_CCR1)
     * 
     * offset:        0x34
     * Reset value: 0x0000
     */
    volatile uint32_t captureCompare1;

    /**
     * TIMx capture/compare register 2 (TIMx_CCR2)
     * 
     * offset:        0x38
     * Reset value: 0x0000
     */
    volatile uint32_t captureCompare2;

    /**
     * TIMx capture/compare register 3 (TIMx_CCR3)
     * 
     * offset:        0x3c
     * Reset value: 0x0000
     */
    volatile uint32_t captureCompare3;

    /**
     * TIMx capture/compare register 4 (TIMx_CCR4)
     * 
     * offset:        0x40
     * Reset value: 0x0000
     */
    volatile uint32_t captureCompare4;

    /**
     * Reserved
     */
    volatile uint32_t reserved3;

    /**
     * TIMx DMA control register (TIMx_DCR)
     * 
     * offset:        0x48
     * Reset value: 0x0000
     */
    volatile uint32_t dmaControl;

    /**
     * TIMx DMA address for full transfer (TIMx_DMAR)
     * 
     * offset:        0x4c
     * Reset value: 0x0000
     */
    volatile uint32_t dmaAddress;

    /**
     * TIM2 option register (TIM2_OR) or TIM5 option register (TIM5_OR)
     * 
     * offset:        0x50
     * Reset value: 0x0000
     */
    volatile uint32_t option2_5;
};


/**
 * Initialize the timer 5 for WCET
 */
void pharosCpuTimerWcetInit(void);


/* PHAROS_TIMER_H */
#endif	
