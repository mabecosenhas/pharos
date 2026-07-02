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


#ifndef PHAROS_FPU_H
#define	PHAROS_FPU_H

#include <pharos/hal/v7m/declarations.h>
#include <pharos/hal/v7m/conf.h>

#define PHAROS_CPU_FPU     ((ptrPharosCpuFpu) PHAROS_CPU_FPU_ADDRESS)


/**
 * Compact the size of the I/O registers
 */
#pragma pack(push ,4)


struct PharosCpuFpu
{
    /**
     * Floating Point Context Control Register (FPCCR)
     * 
     *  
     * 
     * Offset: 0x00
     */
    volatile uint32_t contextControl;

    /**
     * Floating Point Context Address Register (FPCAR)
     * 
     *  
     * 
     * Offset: 0x04
     */
    volatile uint32_t contextAddress;

    /**
     * Floating Point Default Status Control Register (FPDSCR)
     * 
     *  
     * 
     * Offset: 0x08
     */
    volatile uint32_t defaultStatusControl;

    /**
     * Media and FP Feature Register 0 (MVFR0)
     * 
     *  
     * 
     * Offset: 0x0c
     */
    volatile uint32_t mediaAndFpFeature0;

    /**
     * Media and FP Feature Register 1 (MVFR1)
     * 
     *  
     * 
     * Offset: 0x10
     */
    volatile uint32_t mediaAndFpFeature1;

    /**
     * Media and FP Feature Register 2 (MVFR2)
     * 
     *  
     * 
     * Offset: 0x14
     */
    volatile uint32_t mediaAndFpFeature2;
};


/**
 * restore the previous pack definition
 */
#pragma pack(pop)


void pharosCpuFpuInit(void);

/* PHAROS_FPU_H */
#endif	
