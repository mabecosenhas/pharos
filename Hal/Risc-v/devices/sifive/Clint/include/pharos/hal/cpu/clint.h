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


#ifndef PHAROS_CLINT_H
#define	PHAROS_CLINT_H

#include <pharos/hal/cpu/declarations.h>
#include <pharos/hal/cpu/conf.h>
#include <pharos/kernel/core/declarations.h>



#define PHAROS_CPU_CLINT       ((ptrPharosCpuClint) PHAROS_CPU_CLINT_ADDRESS)


typedef struct
{
    /**
     * MSIP registers 
     * 
     * offset: 0x00
     */
    volatile uint32_t msip[PHAROS_NUMBER_CORES];

    /**
     * Reserved
     * 
     * offset: 0x10
     */
    uint8_t reserved1[0x4000U - PHAROS_NUMBER_CORES * 4];


    /**
     * mtimecmp for hart 0-4
     * 
     * offset: 0x4000
     */
    volatile uint64_t mtimecmp[PHAROS_NUMBER_CORES];

    /**
     * Reserved
     * 
     * offset: 0x04028
     */
    uint8_t reserved2[0xbff8U - 0x4000U - PHAROS_NUMBER_CORES * 8U];

    /**
     * Machine time 
     * 
     * offset: 0xbff8
     */
    volatile uint64_t mtime;
} PharosCpuClint , *ptrPharosCpuClint;


/**
 * Clear the software interrupt
 * 
 * @param number core in which to clear the interrupt
 */
void pharosCpuMulticoreClearInterrupt(CoreNumber number);


/* PHAROS_CLINT_H */
#endif	
