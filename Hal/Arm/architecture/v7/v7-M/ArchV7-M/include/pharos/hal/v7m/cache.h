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


#ifndef PHAROS_CACHE_H
#define	PHAROS_CACHE_H

#include <pharos/hal/hal.h>
#include <pharos/hal/v7m/declarations.h>
#include <pharos/hal/v7m/conf.h>

#define PHAROS_CPU_CACHE     ((ptrPharosCpuCache) PHAROS_CPU_CACHE_ADDRESS)

/**
 * Compact the size of the I/O registers
 */
#pragma pack(push ,4)


struct PharosCpuCache
{
    /**
     *  I-Cache Invalidate All to PoU (ICIALLU)
     * 
     * Offset: 0x0
     */
    volatile uint32_t instructionCacheInvalidateAll;

    /**
     * Reserved
     * 
     * Offset: 0x4
     */
    volatile uint32_t reserved;

    /**
     *  I-Cache Invalidate by MVA to PoU (ICIMVAU)
     * 
     * Offset: 0x8
     */
    volatile uint32_t instructionCacheInvalidateMva;

    /**
     * D-Cache Invalidate by MVA to PoC (DCIMVAC)
     * 
     * Offset: 0xc
     */
    volatile uint32_t dataCacheInvalidateMva;

    /**
     * D-Cache Invalidate by Set-way (DCISW)
     * 
     * Offset: 0x10
     */
    volatile uint32_t dCacheInvalidateBySetWay;

    /**
     * D-Cache Clean by MVA to PoU (DCCMVAU)
     * 
     * Offset: 0x14
     */
    volatile uint32_t dataCacheCleanMva;

    /**
     * D-Cache Clean by MVA to PoC (DCCMVAC)
     * 
     * Offset: 0x18
     */
    volatile uint32_t dataCacheCleanByMva;

    /**
     * D-Cache Clean by Set-way (DCCSW)
     * 
     * Offset: 0x1c
     */
    volatile uint32_t dataCacheCleanSetway;

    /**
     * D-Cache Clean and Invalidate by MVA to PoC (DCCIMVAC)
     * 
     * Offset: 0x20
     */
    volatile int32_t dataCacheCleanInvalidateMva;

    /**
     * D-Cache Clean and Invalidate by Set-way (DCCISW)
     * 
     * Offset: 0x24
     */
    volatile uint32_t dataCacheCleanInvalidateSetway;
};


/**
 * restore the previous pack definition
 */
#pragma pack(pop)

/**
 * Enable the i-cache
 */
void pharosCpuCacheIEnable(void);


/**
 * Enable the d-cache
 */
void pharosCpuCacheDEnable(void);


/**
 * Invalidate the instruction cache
 */
void pharosCpuInvalidateICache(void);


/**
 * Flush data cache from the specified addresses
 * 
 * @param start start address
 * @param size size of the memory area to flush
 */
void pharosCpuCacheFlushData(uint8_t *start , uint32_t size);


/**
 * Flush instruction cache from the specified addresses
 * 
 * @param start start address
 * @param size size of the memory area to flush
 */
void pharosCpuCacheFlushInstruction(uint8_t *start , uint32_t size);


/* PHAROS_CACHE_H */
#endif	
