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


#ifndef PHAROS_V7M_NVIC_H
#define PHAROS_V7M_NVIC_H

#include <pharos/hal/hal.h>
#include <pharos/hal/v7m/declarations.h>
#include <pharos/hal/v7m/v7m.h>
#include <pharos/hal/v7m/isr.h>
#include <pharos/kernel/interrupt/declarations.h>
#include <pharos/kernel/partition/declarations.h>
#include <pharos/kernel/collection/linkedlist.h>


#define PHAROS_INTERRUPT_PRIORITY_HIGHEST       (0U)

/**
 * Priority of an interrupt of an app. The two lowest priorities should be for syscall and pendable exceptions
 */
#define PHAROS_INTERRUPT_PRIO_APP_LOW           (PHAROS_INTERRUPT_PRIORITY_LOWEST-1)


#define PHAROS_CPU_NVIC                         ((ptrPharosCpuNvic) PHAROS_CPU_NVIC_ADDRESS)


/**
 * Make sure the structure is aligned
 */
#pragma pack(push ,4)

struct PharosCpuNvic
{
    /**
     * Interrupt Set Enable Register (ISER0 - ISER15)
     * 
     * Enables, or reads the enable state of a group of interrupts.
     * 
     * offset: 0x0
     */
    volatile uint32_t interruptSetEnable[8U];

    /**
     * Reserved
     */
    volatile uint32_t reserved1[24U];

    /**
     * Interrupt Clear-Enable Registers, (ICER0 - ICER15)
     * 
     * Disables, or reads the enable state of, a group of registers.
     * 
     * offset: 0x80
     */
    volatile uint32_t interruptClearEnable[8U];

    /**
     * Reserved
     */
    volatile uint32_t reserved2[24U];

    /**
     * Interrupt Set-Pending Registers, (ISPR0 - ISPR15)
     * 
     * For a group of interrupts, changes interrupt status to pending, or shows the current pending status
     * 
     * offset: 0x100
     */
    volatile uint32_t interrruptSetPending[8U];

    /**
     * Reserved
     */
    volatile uint32_t reserved3[24U];

    /**
     * Interrupt Clear-Pending Registers (ICPR0 - ICPR15)
     * 
     * For a group of interrupts, clears the interrupt pending status, or shows the current pending status
     * 
     * offset: 0x180
     */
    volatile uint32_t interruptClearPending[8U];

    /**
     * Reserved
     */
    volatile uint32_t reserved4[24U];

    /**
     * Interrupt Active Bit Registers (IABR0 - IABR15)
     * 
     * For a group of 32 interrupts, shows whether each interrupt is active.
     * 
     * offset: 0x200
     */
    volatile uint32_t interruptActiveBit[8U];

    /**
     * Reserved
     */
    volatile uint32_t reserved5[56U];

    /**
     * Interrupt Priority Registers (IPR0 - IPR123)
     * 
     * Sets or reads interrupt priorities.
     * 
     * offset: 0x300
     */
    volatile uint8_t interruptPriority[240U];

    /**
     * Reserved
     */
    volatile uint32_t reserved6[644U];

    /**
     * Software Trigger Interrupt Register (STIR)
     * 
     * Provides a mechanism for software to generate an interrupt.
     * 
     * offset: 0xe00
     */
    volatile uint32_t softwareTriggerInterrupt;
};

/**
 * This structure contains the CPU registers that are automatically saved to the stack when an interrupt occurs
 */
struct PharosCpuExceptionFrame
{
    uint32_t register_r0;
    uint32_t register_r1;
    uint32_t register_r2;
    uint32_t register_r3;
    uint32_t register_r12;
    uint16_t *register_lr;
    uint16_t *register_pc;
    uint32_t register_xpsr;
#ifdef PHAROS_CPU_HAS_VFP
    uint32_t register_s0;
    uint32_t register_s1;
    uint32_t register_s2;
    uint32_t register_s3;
    uint32_t register_s4;
    uint32_t register_s5;
    uint32_t register_s6;
    uint32_t register_s7;
    uint32_t register_s8;
    uint32_t register_s9;
    uint32_t register_s10;
    uint32_t register_s11;
    uint32_t register_s12;
    uint32_t register_s13;
    uint32_t register_s14;
    uint32_t register_s15;
    uint32_t register_fpscr;
#endif
    uint32_t reserved;
};

/**
 * Interrupt table for the Cortex v7M CPUs
 * 
 * All Cortex v7-M share the same basic interrupt table (until the systick handler)
 */
struct PharosCpuInterruptTable
{
    uint8_t *initialStackPointer;

    /**
     * Reset handler
     */
    PharosExceptionHandler resetHandler;

    /**
     * Non-maskable interrupt handler
     */
    PharosExceptionHandler nmiHandler;

    /**
     * hard fault handler
     */
    PharosExceptionHandler hardFaultHandler;

    /**
     * MPU fault handler
     */
    PharosExceptionHandler mpuFaultHandler;

    /**
     * bus fault handler
     */
    PharosExceptionHandler busFaultHandler;

    /**
     * usage fault handler
     */
    PharosExceptionHandler usageFaultHandler;

    /**
     * reserved (unused)
     */
    PharosExceptionHandler reserved1;

    /**
     * reserved (unused)
     */
    PharosExceptionHandler reserved2;

    /**
     * reserved (unused)
     */
    PharosExceptionHandler reserved3;

    /**
     * reserved (unused)
     */
    PharosExceptionHandler reserved4;

    /**
     * syscall handler
     */
    PharosExceptionHandler syscallHandler;

    /**
     * debug handler
     */
    PharosExceptionHandler debugMonitorHandler;

    /**
     * reserved (unused)
     */
    PharosExceptionHandler reserved5;

    /**
     * pendable interrupt handler
     */
    PharosExceptionHandler pendableHandler;

    /**
     * Systick handler
     */
    PharosExceptionHandler systickHandler;

    /**
     * This union contains all the CPU specific interrupts. Either in the CPU table itself (isrTable) or as a generic
     * interrupt table (interruptVector)
     */
    union
    {
        /**
         * CPU specific table
         */
        PharosCpuIsrTable isrTable;

        /**
         * Generic interrupt table with the same size as the CPU specific table. This is useful for the generic 
         * ARM v7M code to install an interrupt
         */
        PharosRawInterruptHandler interruptVector[sizeof(PharosCpuIsrTable) / sizeof(PharosRawInterruptHandler)];
    } interruptTable;
};


/**
 * restore the pack attribute to its previous value
 */
#pragma pack(pop)

struct PharosCpuInterruptConfiguration
{
    /**
     * Priority of the interrupt
     * 
     * @note application must initialize this number
     */
    PharosInterruptPriority priority;
};



/**
 * Interrupt stack
 */
extern uint8_t pharosCpuInterruptStack[];


/**
 * Initialize the NVIC with the specified interrupt table
 * 
 * @param table interrupt table
 */
void pharosCpuNvicInitialize(ptrInterruptTable table);


/**
 * Check if the specified interrupt is pending 
 * 
 * @constraint only available for interrupts, not for exceptions. That is: number > PHAROS_CPU_EXCEPTION_SYSTICK
 * 
 * @param number interrupt number
 * 
 * @return returns TRUE if the interrupt is pending and FALSE otherwise
 */
bool pharosCpuInterruptIsPending(PharosInterruptNumber number);



/* PHAROS_V7M_NVIC_H */
#endif 
