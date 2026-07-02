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


#ifndef PHAROS_SYSTICK_H
#define	PHAROS_SYSTICK_H

#include <pharos/hal/v7m/declarations.h>
#include <pharos/hal/v7m/conf.h>

#define PHAROS_CPU_SYSTICK  ((ptrPharosCpuSysTick) PHAROS_CPU_SYSTICK_ADDRESS)


#define PHAROS_CPU_SYSTICK_CTRL_CPUCLK        (1 << 2)
#define PHAROS_CPU_SYSTICK_CTRL_TICKINT       (1 << 1)
#define PHAROS_CPU_SYSTICK_CTRL_ENABLE        (1 << 0)

/**
 * Compact the size of the I/O registers
 */
#pragma pack(push ,4)


struct PharosCpuSysTick
{
    /**
     * SysTick Control and Status Register (SYST_CSR)
     * 
     * Offset: 0x00
     */
    volatile uint32_t controlAndStatus;

    /**
     * SysTick Reload Value Register (SYST_RVR)
     * 
     * Offset: 0x00
     */
    volatile uint32_t reloadValue;

    /**
     * SysTick Current Value Register (SYST_CVR)
     * 
     * Offset: 0x00
     */
    volatile uint32_t currentValue;

    /**
     * SysTick Calibration value Register (SYST_CALIB)
     * 
     * Offset: 0x00
     */
    volatile uint32_t calibrationValue;
};


/**
 * restore the previous pack definition
 */
#pragma pack(pop)


/**
 * Initialize the systick timer
 */
void pharosCpuSystickInit(void);



/* PHAROS_SYSTICK_H */
#endif	
