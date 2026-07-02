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


#ifndef PHAROS_ARM_TIMER_H
#define	PHAROS_ARM_TIMER_H

#include <pharos/hal/cpu/declarations.h>
#include <pharos/hal/cpu/conf.h>

#define PHAROS_CPU_TIMER        ((ptrPharosCpuTimer) PHAROS_CPU_TIMER_ADDRESS)

/**
 * Compact the size of the I/O registers
 */
#pragma pack(push ,4)


struct PharosCpuTimer
{
    /**
     * timer load register  
     * 
     * Offset: 0x0
     */
    volatile uint32_t load;

    /**
     * timer value register  
     * 
     * Read-only 
     * 
     * Offset: 0x4
     */
    volatile uint32_t value;

    /**
     * timer load register 
     * 
     * Offset: 0x8
     */
    volatile uint32_t control;

    /**
     * timer IRQ Clear/Ack register  
     * 
     * Write-only
     * 
     * Offset: 0xc
     */
    volatile uint32_t irqClear;

    /**
     * timer raw IRQ 
     * 
     * Offset: 0x10
     */
    volatile uint32_t rawIrq;

    /**
     * timer masked IRQ register 
     * 
     * Offset: 0x14
     */
    volatile uint32_t maskedIrq;

    /**
     * timer reload register
     * 
     * Offset: 0x18
     */
    volatile uint32_t reload;

    /**
     * timer pre-divider register
     * 
     * Offset: 0x1c
     */
    volatile uint32_t predivider;

    /**
     * timer free running counter register
     * 
     * Offset: 0x20
     */
    volatile uint32_t freeRunningCounter;
};

/**
 * restore the pack definition
 */
#pragma pack(pop)


/* PHAROS_ARM_TIMER_H */
#endif	
