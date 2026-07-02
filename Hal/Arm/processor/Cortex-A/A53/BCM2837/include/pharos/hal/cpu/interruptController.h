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


#ifndef PHAROS_INTERRUPTCONTROLLER_H
#define PHAROS_INTERRUPTCONTROLLER_H

#include <pharos/hal/cpu/declarations.h>
#include <pharos/hal/cpu/conf.h>
#include <pharos/kernel/interrupt/declarations.h>
#include <pharos/kernel/partition/declarations.h>

#define PHAROS_CPU_IC  ((ptrPharosInterruptController) PHAROS_CPU_INT_CTRL_ADDRESS)

#define PHAROS_CPU_IRQBASEPEND_PEND2        (1U << 9U)
#define PHAROS_CPU_IRQBASEPEND_PEND1        (1U << 8U)

#define PHAROS_CPU_IRQBASEPEND_MASK         (0xffU)


#define PHAROS_CPU_IC_PEND1_BIT(x)          (1U << x)
#define PHAROS_CPU_IC_PEND2_BIT(x)          (1U << (x-32U))

#define PHAROS_CPU_IC_FIQ_EN                (1U << 7U)

/**
 * Compact the size of the I/O registers
 */
#pragma pack(push ,4)

struct PharosInterruptController
{
    /**
     * Basic pending interrupts
     * 
     * offset: 0x0
     */
    volatile uint32_t irqBasicPending;

    /**
     * Pendable interrupt register 1
     * 
     * offset: 0x4
     */
    volatile uint32_t irqPending1;

    /**
     * Pendable interrupt register 2
     * 
     * offset: 0x8
     */
    volatile uint32_t irqPending2;

    /**
     * FIQ control register
     * 
     * offset: 0xc
     */
    volatile uint32_t fiqControl;

    /**
     * Enable IRQ 1 register
     * 
     * offset: 0x10
     */
    volatile uint32_t enableIrq1;

    /**
     * Enable IRQ 1 register
     * 
     * offset: 0x14
     */
    volatile uint32_t enableIrq2;

    /**
     * Enable IRQ 1 register
     * 
     * offset: 0x18
     */
    volatile uint32_t enableBasicIrq;

    /**
     * Enable IRQ 1 register
     * 
     * offset: 0x1c
     */
    volatile uint32_t disableIrq1;

    /**
     * Enable IRQ 1 register
     * 
     * offset: 0x20
     */
    volatile uint32_t disableIrq2;

    /**
     * Enable IRQ 1 register
     * 
     * offset: 0x24
     */
    volatile uint32_t disableBasicIrq;
};

/**
 * restore the pack definition
 */
#pragma pack(pop)

struct PharosCpuInterruptConfiguration
{
    /**
     * Priority of interrupt 
     * 
     * @note application must initialize this number
     * @note at most ONE interrupt can be FIQ
     */
    PharosInterruptPriority priority;
};

/**
 * Force enabling the interrupts (written in assembly)
 */
void pharosCpuIntForceEnable();


/**
 * Initialize the CPU Interrupt controller 
 * 
 * @param table interrupt table to initialize
 */
void pharosCpuIcInitialize(ptrInterruptTable table);


/* PHAROS_INTERRUPTCONTROLLER_H */
#endif 
