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

#include <pharos/hal/board/declarations.h>
#include <pharos/kernel/interrupt/declarations.h>


/**
 * Compact the size of the I/O registers
 */
#pragma pack(push ,4)

struct ArmInterruptControllerSic
{
    /**
     * Status of the interrupt (after mask)
     * 
     * offset: 0x00
     */
    volatile uint32_t status;

    /**
     * Status of the interrupt (before mask)
     * 
     * offset: 0x04
     */
    volatile uint32_t rawStatus;

    /**
     * Interrupt mask (also used to set the interrupts)
     * 
     * offset: 0x08
     */
    volatile uint32_t enableSet;

    /**
     * clear the interrupt
     * 
     * offset: 0x0c
     */
    volatile uint32_t enableClear;

    /**
     * Set the software interrupt
     * 
     * offset: 0x10
     */
    volatile uint32_t softwareInterruptSet;

    /**
     * Clear the software interrupt
     * 
     * offset: 0x14
     */
    volatile uint32_t softwareInterruptClear;

    /**
     * Reserved
     * 
     * offset: 0x1c
     */
    uint32_t reserved;

    /**
     * Read status of pass-through mask (allows interrupt to pass directly to the primary interrupt controller)
     * Also set the interrupt pass through bits
     * offset: 0x20
     */
    volatile uint32_t picEnableSet;

    /**
     * Clear interrupt pass through bits 
     * 
     * offset: 0x24
     */
    volatile uint32_t picEnableClear;
};

/**
 * restore the previous pack definition
 */
#pragma pack(pop)


/**
 * Initialize the interrupt controller of the board 
 * 
 * @param table interrupt table
 */
void pharosCpuInterruptControllerInitialize(ptrInterruptTable table);


#endif /* PHAROS_INTERRUPTCONTROLLER_H */
