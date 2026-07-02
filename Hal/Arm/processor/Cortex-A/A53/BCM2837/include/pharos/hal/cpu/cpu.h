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


#ifndef PHAROS_CPU_H
#define	PHAROS_CPU_H

#include <pharos/hal/cpu/declarations.h>
#include <pharos/hal/v8a/declarations.h>
#include <pharos/hal/v8a/context.h>
#include <pharos/hal/interrupt.h>
#include <pharos/hal/cpu/interruptController.h>
#include <pharos/hal/cpu/asm.h>
#include <pharos/hal/cpu/conf.h>

#define PHAROS_CPU_REGISTERS                ((ptrPharosCpuRegisters) PHAROS_CPU_REGISTERS_ADDRESS)


#define PHAROS_CPU_LIR_CORE0_IRQ            (0 << 0)


struct CpuModeChangeContext
{
    uint32_t spsr;
};


struct CpuCoreConfiguration
{
    
};


/**
 * Compact the size of the I/O registers
 */
#pragma pack(push ,4)


struct PharosCpuRegisters
{
    /**
     * Control register
     * 
     * offset: 0x00
     */
    volatile uint32_t control;

    /**
     * reserved
     * 
     * offset: 0x04
     */
    uint32_t reserved1;

    /**
     * Core timer prescalar
     * 
     * offset: 0x08
     */
    volatile uint32_t prescalar;

    /**
     * GPU interrupts routing
     * 
     * offset: 0x0c
     */
    volatile uint32_t gpuInterruptRouting;

    /**
     * Performance Monitor Interrupts routing-set
     * 
     * offset: 0x10
     */
    volatile uint32_t perfMonIntRoutingSet;

    /**
     * Performance Monitor Interrupts routing-clear
     * 
     * offset: 0x14
     */
    volatile uint32_t perfMonIntRoutingClear;

    /**
     * Reserved
     * 
     * offset: 0x18
     */
    uint32_t reserved2;

    /**
     * Core timer access 
     * 
     * offset: 0x1c and 0x20
     */
    volatile uint64_t coreTimerAccess;

    /**
     * Local interrupt (1-7) routing
     * 
     * offset: 0x24
     */
    volatile uint32_t localInterruptRouting1;

    /**
     * Local interrupt (8-15) routing
     * 
     * offset: 0x28
     */
    volatile uint32_t localInterruptRouting2;

    /**
     * Axi outstanding counters
     * 
     * offset: 0x2c
     */
    volatile uint32_t axiOutstandingCounters;

    /**
     * Axi outstanding IRQ
     * 
     * offset: 0x30
     */
    volatile uint32_t axiOutstandingIrq;

    /**
     * Local timer control & status
     * 
     * offset: 0x34
     */
    volatile uint32_t localTimerControl;

    /**
     * Local timer write flags
     * 
     * offset: 0x38
     */
    volatile uint32_t localTimerClearReload;

    /**
     * Reserved
     * 
     * offset: 0x3c
     */
    volatile uint32_t reserved3;

    /**
     * Cores timers Interrupt control
     * 
     * offset: 0x40
     */
    volatile uint32_t coreTimerInterruptControl[PHAROS_NUMBER_CORES];

    /**
     * Cores Mailboxes Interrupt control
     * 
     * offset: 0x50
     */
    volatile uint32_t coreMailBoxesInterruptControl[PHAROS_NUMBER_CORES];

    /**
     * Cores IRQ Source
     * 
     * offset: 0x60
     */
    volatile uint32_t coreIrqSources[PHAROS_NUMBER_CORES];

    /**
     * Cores FIQ Source
     * 
     * offset: 0x70
     */
    volatile uint32_t coreFiqSources[PHAROS_NUMBER_CORES];

    /**
     * Core Mailboxes write set
     * 
     * offset: 0x80
     */
    volatile uint32_t coreMailboxesWriteSet[PHAROS_NUMBER_CORES][4U];


    /**
     * Core Mailboxes read & write high to clear
     * 
     * offset: 0xc0
     */
    volatile uint32_t coreMailboxesRead[PHAROS_NUMBER_CORES][4U];
};

/**
 * restore the pack definition
 */
#pragma pack(pop)


void pharosCpuInitialize(void);

/* PHAROS_CPU_H */
#endif	
