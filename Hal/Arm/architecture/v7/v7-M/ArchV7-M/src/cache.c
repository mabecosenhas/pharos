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


#include <pharos/hal/v7m/scb.h>
#include <pharos/hal/thumb.h>
#include <pharos/hal/v7m/cache.h>
#include <pharos/hal/v7m/cpuid.h>

#define SCB_CCSIDR_ASSOCIATIVITY_Pos        3U                                            /*!< SCB CCSIDR: Associativity Position */
#define SCB_CCSIDR_ASSOCIATIVITY_Msk       (0x3FFUL << SCB_CCSIDR_ASSOCIATIVITY_Pos)      /*!< SCB CCSIDR: Associativity Mask */

#define SCB_CCSIDR_NUMSETS_Pos             13U                                            /*!< SCB CCSIDR: NumSets Position */
#define SCB_CCSIDR_NUMSETS_Msk             (0x7FFFUL << SCB_CCSIDR_NUMSETS_Pos)           /*!< SCB CCSIDR: NumSets Mask */


/* Cache Size ID Register Macros */
#define CCSIDR_WAYS(x)                      (((x) & SCB_CCSIDR_ASSOCIATIVITY_Msk) >> SCB_CCSIDR_ASSOCIATIVITY_Pos)
#define CCSIDR_SETS(x)                      (((x) & SCB_CCSIDR_NUMSETS_Msk      ) >> SCB_CCSIDR_NUMSETS_Pos      )


#define SCB_CCR_IC_Pos                      17U                                           /*!< SCB CCR: Instruction cache enable bit Position */
#define SCB_CCR_IC_Msk                      (1UL << SCB_CCR_IC_Pos)                        /*!< SCB CCR: Instruction cache enable bit Mask */


/* SCB D-Cache Invalidate by Set-way Register */
#define SCB_DCISW_WAY_Pos                   30U                                            /*!< SCB DCISW: Way Position */
#define SCB_DCISW_WAY_Msk                   (3UL << SCB_DCISW_WAY_Pos)                     /*!< SCB DCISW: Way Mask */

#define SCB_CCR_DC_Pos                      16U                                           /*!< SCB CCR: Cache enable bit Position */
#define SCB_CCR_DC_Msk                      (1UL << SCB_CCR_DC_Pos)                        /*!< SCB CCR: Cache enable bit Mask */

#define SCB_DCISW_SET_Pos                   5U                                            /*!< SCB DCISW: Set Position */
#define SCB_DCISW_SET_Msk                   (0x1FFUL << SCB_DCISW_SET_Pos)                 /*!< SCB DCISW: Set Mask */




/**
 * Invalidate the instruction and the data cache
 */
void pharosCpuInvalidateCaches(void);


/**
 * Invalidates the instruction cache 
 */
void pharosCpuInvalidateICache(void);

/**
 * Flush the data cache 
 */
void pharosCpuFlushDataCache(void);


void pharosCpuCacheDataFlush(uint8_t *start , uint8_t *end);



void pharosCpuCacheIEnable(void)
{
    pharosCpuDsb();
    pharosCpuIsb();

    /* invalidate the instruction cache */
    pharosCpuInvalidateICache();

    /* enable the instruction cache */
    PHAROS_CPU_SCB->configurationAndControl |= SCB_CCR_IC_Msk;

    pharosCpuDsb();
    pharosCpuIsb();
}


void pharosCpuCacheDEnable(void)
{
    uint32_t ccsidr;
    uint32_t sets;
    uint32_t ways;

    /* Level 1 data cache */
    PHAROS_CPU_CPUID->cacheSizeSelection = (0U << 1U) | 0U;
    pharosCpuDsb();

    /* get the cache size */
    ccsidr = PHAROS_CPU_CPUID->cacheSizeId;

    /* invalidate D-Cache */
    sets = (uint32_t) (CCSIDR_SETS(ccsidr));

    /* for each set */
    do
    {
        /* get the number of ways of the set */
        ways = (uint32_t) (CCSIDR_WAYS(ccsidr));

        /* for each way inside the set */
        do
        {
            /* invalidate the d-cache way */
            PHAROS_CPU_CACHE->dCacheInvalidateBySetWay = (((sets << SCB_DCISW_SET_Pos) & SCB_DCISW_SET_Msk) |
                                                          ((ways << SCB_DCISW_WAY_Pos) & SCB_DCISW_WAY_Msk));
        }
        while(ways--);
    }
    while(sets--);


    pharosCpuDsb();

    /* enable D-Cache */
    PHAROS_CPU_SCB->configurationAndControl |= (uint32_t) SCB_CCR_DC_Msk;

    pharosCpuDsb();
    pharosCpuIsb();
}


void pharosCpuInvalidateICache(void)
{
    pharosCpuDsb();
    pharosCpuIsb();

    /* invalidate the instruction cache */
    PHAROS_CPU_CACHE->instructionCacheInvalidateAll = 0U;

    pharosCpuDsb();
    pharosCpuIsb();
}


void pharosCpuCacheFlushData(uint8_t *start , uint32_t size)
{
    int32_t op_size = size;
    uint32_t op_addr = (uint32_t) start;

    /* read the line size of the cache */
    int32_t linesize = 1U << PHAROS_CPU_CPUID_CCSIDR_LINSIZE(PHAROS_CPU_CPUID->cacheSizeId);

    pharosCpuDsb();

    while(op_size > 0)
    {
        PHAROS_CPU_CACHE->dataCacheCleanInvalidateMva = op_addr;
        op_addr += linesize;
        op_size -= linesize;
    }

    pharosCpuDsb();
    pharosCpuIsb();
}

void pharosCpuCacheFlushInstruction(uint8_t *start , uint32_t size)
{
    /* invalidate all the instruction cache */
    pharosCpuInvalidateICache();
}
