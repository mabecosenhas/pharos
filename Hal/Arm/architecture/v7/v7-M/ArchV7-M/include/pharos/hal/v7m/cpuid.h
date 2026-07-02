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


#ifndef PHAROS_CPUID_H
#define	PHAROS_CPUID_H

#include <pharos/hal/hal.h>
#include <pharos/hal/v7m/declarations.h>
#include <pharos/hal/v7m/conf.h>

#define PHAROS_CPU_CPUID     ((ptrPharosCpuCpuid) PHAROS_CPU_CPUID_ADDRESS)

#define PHAROS_CPU_CPUID_CCSIDR_LINSIZE(x)          (x & 0b111)


/**
 * Compact the size of the I/O registers
 */
#pragma pack(push ,4)


struct PharosCpuCpuid
{
    /**
     * CPUID Base Register (CPUID)
     * 
     * Provides identification information for the processor.
     * 
     * offset: 0x0
     */
    volatile uint32_t cpuId;

    /**
     * Reserved
     */
    volatile uint32_t reserved1[15U];

    /**
     * Processor Feature Register 0 (ID_PFR0)
     * 
     * Gives top-level information about the instruction sets supported by the processor. This 
     * register is part of the Identification registers functional group.
     * 
     * offset: 0x40
     */
    volatile uint32_t processorFeature0;

    /**
     * Processor Feature Register 1 (ID_PFR1)
     * 
     * Gives top-level information about the instruction sets supported by the processor. This 
     * register is part of the Identification registers functional group.
     * 
     * offset: 0x44
     */
    volatile uint32_t processorFeature1;

    /**
     * Debug Feature Register 0 (ID_DFR0)
     * 
     * Gives top-level information about the debug system used in the processor. This register is 
     * part of the Identification registers functional group.
     * 
     * offset: 0x48
     */
    volatile uint32_t debugFeature0;

    /**
     * Auxiliary Feature Register 0 (ID_AFR0)
     * 
     * Gives information about the IMPLEMENTATION DEFINED features of a processor implementation
     * 
     * offset: 0x4c
     */
    volatile uint32_t auxFeature0;

    /**
     * Memory Model Feature Register 0 (ID_MMFR0)
     * 
     * Gives information about the implemented memory model and memory management support.
     * 
     * offset: 0x50
     */
    volatile uint32_t memoryModeFeature0;

    /**
     * Memory Model Feature Register 1 (ID_MMFR1)
     * 
     * 
     * offset: 0x54
     */
    volatile uint32_t memoryModeFeature1;

    /**
     * Memory Model Feature Register 2 (ID_MMFR2)
     * 
     * 
     * offset: 0x58
     */
    volatile uint32_t memoryModeFeature2;

    /**
     * Memory Model Feature Register 3 (ID_MMFR3)
     * 
     * 
     * offset: 0x5c
     */
    volatile uint32_t memoryModeFeature3;

    /**
     * Instruction Set Attribute Register 0 (ID_ISAR0)
     * 
     * offset: 0x60
     */
    volatile uint32_t instructionSetAttr0;

    /**
     * Instruction Set Attribute Register 1 (ID_ISAR1)
     *  
     * offset: 0x64
     */
    volatile uint32_t instructionSetAttr1;

    /**
     * Instruction Set Attribute Register 2 (ID_ISAR2)
     * 
     * offset: 0x68
     */
    volatile uint32_t instructionSetAttr2;

    /**
     * Instruction Set Attribute Register 3 (ID_ISAR3)
     * 
     * offset: 0x6c
     */
    volatile uint32_t instructionSetAttr3;

    /**
     * Instruction Set Attribute Register 4 (ID_ISAR4)
     * 
     * Offset: 0x70
     */
    volatile uint32_t instructionSetAttr4;

    /**
     * Reserved
     */
    volatile uint32_t reserved2;


    /**
     * Cache Level ID Register (CLIDR)
     * 
     * identifies the type of cache, or caches, implemented at each level, up to a maximum of seven levels and 
     * the level of coherency and level of unification for the cache hierarchy
     * 
     * offset: 0x78
     */
    volatile uint32_t cacheLevelId;


    /**
     * Cache Type Register (CTR)
     * 
     * offset: 0x7c
     */
    volatile uint32_t cacheType;

    /**
     * Cache Size ID Registers (CCSIDR)
     * 
     * provides information about the architecture of the caches.
     * 
     * offset: 0x80
     */
    volatile uint32_t cacheSizeId;

    /**
     * Cache Size Selection Register (CSSELR)
     * 
     * Offset: 0x84
     */
    volatile uint32_t cacheSizeSelection;
};


/**
 * restore the previous pack definition
 */
#pragma pack(pop)


/* PHAROS_CPUID_H */
#endif	
