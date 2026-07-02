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


#ifndef PHAROS_CPU_SYSTEM_TIMER_H
#define	PHAROS_CPU_SYSTEM_TIMER_H

#include <pharos/hal/cpu/declarations.h>
#include <pharos/hal/cpu/conf.h>

#define PHAROS_CPU_ST                   ((ptrPharosCpuSystemTimer) PHAROS_CPU_SYSTEM_TIMER_ADDRESS)

#define PHAROS_CPU_ST_CS_CLEAR(x)       (0xf & ~(1U << x))

/**
 * Compact the size of the I/O registers
 */
#pragma pack(push ,4)


struct PharosCpuSystemTimer
{
    /**
     * System timer control/status (CS)
     * 
     * Offset: 0x0
     */
    volatile uint32_t control;

    /**
     * System timer counter lower 32 bits (CLO)
     * 
     * Offset: 0x4
     */
    volatile uint32_t lowCounter;

    /**
     * System timer counter higher 32 bits (C0)
     * 
     * Offset: 0x8
     */
    volatile uint32_t highCounter;

    /**
     * System timer compare 0 (C0)
     * 
     * Offset: 0xc
     */
    volatile uint32_t compare0;

    /**
     * System timer compare 1 (C1)
     * 
     * Offset: 0x10
     */
    volatile uint32_t compare1;

    /**
     * System timer compare 2 (C2)
     * 
     * Offset: 0x14
     */
    volatile uint32_t compare2;


    /**
     * System timer compare 3 (C3)
     * 
     * Offset: 0x18
     */
    volatile uint32_t compare3;
};


/**
 * restore the pack definition
 */
#pragma pack(pop)



void pharosCpuSystemTimerInit(void);


/* PHAROS_CPU_SYSTEM_TIMER_H */
#endif	
