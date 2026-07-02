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


#ifndef PHAROS_FLASH_H
#define	PHAROS_FLASH_H

#include <pharos/hal/cpu/declarations.h>
#include <pharos/hal/cpu/conf.h>

#define PHAROS_CPU_FLASH            ((ptrPharosCpuFlash) PHAROS_CPU_FLASH_ADDRESS)


struct PharosCpuFlash
{
    /**
     * Flash access control register (FLASH_ACR)
     * 
     * offset:      0x00
     * Reset value: 0x0000 0000
     */
    volatile uint32_t accessControl;

    /**
     * Flash key register (FLASH_KEYR)
     * 
     * offset:      0x04
     * Reset value: 0x0000 0000
     */
    volatile uint32_t key;

    /**
     * Flash option key register (FLASH_OPTKEYR)
     * 
     * offset:      0x08
     * Reset value: 0x0000 0000
     */
    volatile uint32_t optionKey;

    /**
     * Flash status register (FLASH_SR)
     * 
     * offset:      0x0c
     * Reset value: 0x0000 0000
     */
    volatile uint32_t status;

    /**
     * Flash control register (FLASH_CR)
     * 
     * offset:      0x10
     * Reset value: 0x8000 0000
     */
    volatile uint32_t control;

    /**
     * Flash option control register (FLASH_OPTCR)
     * 
     * offset:      0x14
     */
    volatile uint32_t optionControl;

    /**
     * Flash option control1 register (FLASH_OPTCR1)
     * 
     * offset:      0x18
     */
    volatile uint32_t optionControl1;


};


/**
 * Set the flash latency
 * 
 * @param latency number of wait cycles
 */
void pharosCpuFlashSetLatency(uint32_t latency);

/* PHAROS_FLASH_H */
#endif	
