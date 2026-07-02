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


#ifndef PHAROS_SCB_H
#define	PHAROS_SCB_H

#include <pharos/hal/v7m/declarations.h>
#include <pharos/hal/v7m/conf.h>

#define PHAROS_CPU_SCB                      ((ptrPharosCpuScb) PHAROS_CPU_SCB_ADDRESS)


#define PHAROS_CPU_SCB_ICSR_PENDSVSET       (1U << 28U)
#define PHAROS_CPU_SCB_ICSR_PENDSVCLEAR     (1U << 27U)

/* enable the memory fault (MPU) */
#define PHAROS_CPU_SHCSR_MEMFAULTENA        (1U << 16U)

/* enable usage fault */
#define PHAROS_CPU_SHCSR_USGFAULTENA        (1U << 16U)

/* enable bus fault */
#define PHAROS_CPU_SHCSR_BUSFAULTENA        (1U << 16U)


/**
 * Compact the size of the I/O registers
 */
#pragma pack(push ,4)


struct PharosCpuScb
{
    /**
     * CPUID Base Register (CPUID)
     * 
     * Provides identification information for the processor
     * 
     * Offset: 0x00
     */
    volatile uint32_t cpuId;

    /**
     * Interrupt Control and State Register (ICSR)
     * 
     * Provides software control of the NMI, PendSV, and SysTick exceptions, and provides interrupt status information
     * 
     * Offset: 0x04
     */
    volatile uint32_t interruptControl;

    /**
     * Vector Table Offset Register (VTOR)
     * 
     * Holds the vector table address
     * 
     * Offset: 0x08
     */
    volatile ptrPharosCpuInterruptTable vectorTableOffset;

    /**
     * Application Interrupt and Reset Control Register (AIRCR)
     * 
     * Sets or returns interrupt control data
     * 
     * Offset: 0x0c
     */
    volatile uint32_t appInterruptResetControl;

    /**
     * System Control Register (SCR)
     * 
     * Sets or returns system control data
     * 
     * Offset: 0x10
     */
    volatile uint32_t systemControl;

    /**
     * Configuration and Control Register (CCR)
     * 
     * Sets or returns configuration and control data, and provides control over caching and branch prediction
     * 
     * Offset: 0x14
     */
    volatile uint32_t configurationAndControl;

    /**
     * System Handler Priority Register 1, 2 and 3 (SHPR1, SHPR2, SHPR3)
     * 
     * Sets or returns priority for system handlers
     * 
     * Offset: 0x18
     */
    volatile uint8_t systemHandlerPriority[12];

    /**
     *  System Handler Control and State Register (SHCSR)
     * 
     * 
     * 
     * Offset: 0x24
     */
    volatile uint32_t systemHandlerControlState;

    /**
     *  Configurable Fault Status Register (CFSR)
     * 
     * 
     * 
     * Offset: 0x28
     */
    volatile uint32_t configurableFaultStatus;


    /**
     *  HardFault Status Register (HFSR)
     * 
     * 
     * 
     * Offset: 0x2c
     */
    volatile uint32_t hardFaultStatus;

    /**
     * Debug Fault Status Register (DFSR)
     * 
     * 
     * 
     * Offset: 0x30
     */
    volatile uint32_t debugFaultStatus;

    /**
     * MemManage Fault Address Register (MMFAR)
     * 
     * 
     * 
     * Offset: 0x34
     */
    volatile uint8_t * memMngFaultAddress;

    /**
     *  BusFault Address Register (BFAR)
     * 
     * 
     * Offset: 0x38
     */
    volatile uint32_t busFaultAddress;

    /**
     *  Auxiliary Fault Status Register (AFSR)
     * 
     * 
     * 
     * Offset: 0x3c
     */
    volatile uint32_t auxiliaryFaultStatus;

    /**
     * Reserved
     * 
     * Offset: 0x40
     */
    uint32_t reserved1[18];

    /**
     * Coprocessor Access Control Register (CPACR)
     * 
     * Offset: 0x88
     */
    volatile uint32_t coprocessorAccessControl;
};


/**
 * restore the previous pack definition
 */
#pragma pack(pop)


/* PHAROS_SCB_H */
#endif	
