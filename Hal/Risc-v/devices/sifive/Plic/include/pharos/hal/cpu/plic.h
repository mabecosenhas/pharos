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


#ifndef PHAROS_PLIC_H
#define	PHAROS_PLIC_H


#include <pharos/hal/hal.h>
#include <pharos/hal/cpu/conf.h>
#include <pharos/hal/cpu/declarations.h>
#include <pharos/kernel/interrupt/declarations.h>


/**
 * Compact the size of the I/O registers
 */
#pragma pack(push , 1)


typedef struct
{
    /**
     * Reserved
     * 
     * offset: 0x00
     */
    uint32_t reserved1;

    /**
     * Priorities for each of the interrupts
     * 
     * offset: 0x04
     */
    volatile uint32_t priorities[PHAROS_CPU_ISR_LAST];

    /**
     * Reserved
     */
    uint8_t reserved2[0x1000U - 0x4U - PHAROS_CPU_ISR_LAST * 4U];

    /**
     * Pending interrupts
     * 
     * offset: 0x1000
     */
    volatile uint32_t pending[PHAROS_CPU_ISR_LAST];

    /**
     * Reserved
     */
    uint8_t reserved3[0x2000U - 0x1000U - (PHAROS_CPU_ISR_LAST) * 4U];

    /**
     * Interrupt enables for each core (each core has 32 registers)
     * 
     * offset: 0x2000
     */
    volatile uint32_t interruptEnable[PHAROS_NUMBER_CORES][32U];

    /**
     * Reserved
     */
    uint8_t reserved4[0x200000U - 0x2000U - PHAROS_NUMBER_CORES * 32U * 4U];

    /**
     * Priority threshold and claim/complete registers per core (hart)
     * 
     * offset: 0x200000
     */
    struct
    {
        /**
         * Priority threshold above which interrupts are enabled at the core (hart).
         * 
         * For example, if set with "1" then all interrupts with priority larger than "1" will be enabled 
         * and interrupts with priority equal to "1" (or "0") will be disabled
         */
        volatile uint32_t priorityThreshold;
        
        /**
         * At the start of an interrupt should read this register to get the highest priority interrupt to process 
         * and claim it. In the end of the interrupt should write to this register to announce to the CPU that this 
         * interrupt has been completely processed and can go on to the next interrupt.
         */
        volatile uint32_t claimComplete;

        /**
         * Padding to fill the 4 KiB page
         */
        uint8_t padding[0x1000U - 0x8U];
    } aux[PHAROS_NUMBER_CORES];

} PharosCpuPlic , *ptrPharosCpuPlic;

/**
 * restore the previous pack definition
 */
#pragma pack(pop)


/**
 * Initialize the interrupt controller 
 * 
 * @param table interrupt table
 */
void pharosCpuIcInitialize(ptrInterruptTable table);


/**
 * Checks if the interrupt is pending
 * 
 * @param number interrupt number
 * 
 * @return returns TRUE if the interrupt is pending and FALSE otherwise
 */
bool pharosCpuInterruptIsPending(PharosInterruptNumber number);

/* PHAROS_PLIC_H */
#endif	
