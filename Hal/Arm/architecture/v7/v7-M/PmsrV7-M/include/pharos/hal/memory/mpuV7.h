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


#ifndef PHAROS_HAL_ARM_ARCH_MPUV7_H
#define PHAROS_HAL_ARM_ARCH_MPUV7_H

#include <pharos/hal/declarations.h>
#include <pharos/hal/memory/declarations.h>
#include <pharos/kernel/partition/declarations.h>
#include <pharos/hal/cpu/conf.h>
#include <pharos/kernel/collection/linkedlist.h>
#include <pharos/hal/v7m/mpu.h>
#include <pharos/kernel/thread/declarations.h>

typedef enum
{
    /**
     * Smallest region of 256 bytes
     */
    PHAROS_MPU_REGION_SIZE_256B = VALUE_256B ,

    PHAROS_MPU_REGION_SIZE_512B = VALUE_512B ,
    PHAROS_MPU_REGION_SIZE_1K = VALUE_1KiB ,
    PHAROS_MPU_REGION_SIZE_2K = VALUE_2KiB ,
    PHAROS_MPU_REGION_SIZE_4K = VALUE_4KiB ,
    PHAROS_MPU_REGION_SIZE_8K = VALUE_8KiB ,
    PHAROS_MPU_REGION_SIZE_16K = VALUE_16KiB ,
    PHAROS_MPU_REGION_SIZE_32K = VALUE_32KiB ,
    PHAROS_MPU_REGION_SIZE_64K = VALUE_64KiB ,
    PHAROS_MPU_REGION_SIZE_128K = VALUE_128KiB ,
    PHAROS_MPU_REGION_SIZE_256K = VALUE_256KiB ,
    PHAROS_MPU_REGION_SIZE_512K = VALUE_512KiB ,
    PHAROS_MPU_REGION_SIZE_1M = VALUE_1MiB ,
    PHAROS_MPU_REGION_SIZE_2M = VALUE_2MiB ,
    PHAROS_MPU_REGION_SIZE_4M = VALUE_4MiB ,
    PHAROS_MPU_REGION_SIZE_8M = VALUE_8MiB ,
    PHAROS_MPU_REGION_SIZE_16M = VALUE_16MiB ,
    PHAROS_MPU_REGION_SIZE_32M = VALUE_32MiB ,
    PHAROS_MPU_REGION_SIZE_64M = VALUE_64MiB ,
    PHAROS_MPU_REGION_SIZE_128M = VALUE_128MiB ,
    PHAROS_MPU_REGION_SIZE_256M = VALUE_256MiB ,
    PHAROS_MPU_REGION_SIZE_512M = VALUE_512MiB ,
    PHAROS_MPU_REGION_SIZE_1G = VALUE_1GiB ,
    PHAROS_MPU_REGION_SIZE_2G = VALUE_2GiB ,

    /**
     * Largest region of 4G
     */
    PHAROS_MPU_REGION_SIZE_4G = VALUE_4GiB ,
} PharosMpuRegionSize;

typedef struct
{
    /**
     * base address (RBAR register)
     */
    uint32_t base;

    /**
     * size (RASR register)
     */
    uint32_t size;
} PharosMpuV7MRegion , *ptrPharosMpuV7MRegion;

typedef struct
{
    /**
     * region
     */
    PharosMpuV7MRegion region;

    /**
     * node on the list of used or free regions
     */
    LinkedListNode node;

    /**
     * Index of the Region on the list of regions
     */
    uint8_t index;
} PharosCpuMpuRegion , *ptrPharosCpuMpuRegion;

/**
 * Context of the MPU for each thread.
 * 
 * Note that:
 *  MPU region 0 is already assigned for text + read-only data
 *  MPU region 1 is already assigned for shared data
 *  MPU region 2 is already assigned for IP calls area
 *  and that MPU region 3 will be assigned for the shared stack
 *  at least MPU region 4 (but can be more) will be for the partition data
 *  a variable number of MPU regions will be for I/O
 * 
 * So the list of dynamically assigned regions (for channels) can only go from 5 up to PHAROS_CPU_MPU_NUMBER_REGIONS
 */
typedef struct
{
    /**
     * List of free regions that can be dynamically assigned (for channel bulks)
     */
    LinkedList freeRegions;

    /**
     * List of used regions that can be dynamically assigned (for channel bulks)
     */
    LinkedList usedRegions;

    /**
     * number of memory regions used for the partition and I/O
     */
    uint32_t numberPartitionAndIoRegions;

    /**
     * Regions for partitions, I/O and channels
     */
    PharosCpuMpuRegion tableRegions[PHAROS_CPU_MPU_NBR_RGI_VAR];
} PharosCpuMpuPartitionContext , *ptrPharosCpuMpuPartitionContext;

struct IoMemAreaDirect
{
    /**
     * Start of the I/O direct memory area
     * 
     * Note: application must initialize these values 
     */
    uint8_t *start;

    /**
     * Size of the I/O direct memory area
     * 
     * Note: application must initialize these values 
     */
    PharosMpuRegionSize size;

    /**
     * Attributes of the I/O direct memory area
     * 
     * Note: application must initialize these values 
     */
    struct
    {
        /**
         * Flag indicating if the memory area can only be accessed with supervisor permissions
         */
        unsigned int supervisor : 1;

        /**
         * Check the ARM V7 documentation for further information on these bits and how they correlate to the cache
         */
        unsigned int c : 1;
        unsigned int b : 1;
        unsigned int s : 1;
        unsigned int tex : 3;
    } attributes;
};

/**
 * Region mapping:
 *  - fixed:
 *    - one for the text
 *    - one for the shared data
 *    - one for the shared stack (if needed)
 *  - variable:
 *    - some for the partition data/bss/uss
 *    - some for I/O
 *    - some for channel bulks
 * 
 * that is, if a direct I/O triggers a fault and the memory is accessible to the partition, then the last used 
 * region slot will be initialized for that memory area. That can be the shared section, I/O direct memory areas or
 * channel bulks
 * 
 * the background region is used when switched to privileged mode
 */
struct PartitionCpuMmuContext
{
    /**
     * Value to place (address) on the MPUBASE register relative to the partition data areas
     */
    uint32_t base[PHAROS_NUMBER_MEMORY_AREAS];

    /**
     * Value to place (size) on the MPUATTR register relative to the partition data areas
     */
    uint32_t size[PHAROS_NUMBER_MEMORY_AREAS];

    /**
     * number of memory areas of the partition
     */
    uint32_t number;
};

/**
 * Empty structure (no pages in MPU)
 */
struct MemPageTable
{
};

struct IoMemAreaTable
{
    /**
     * table of direct areas 
     */
    ptrIoMemAreaDirect directAreas;

    /**
     * number of items in the table of direct areas 
     */
    uint32_t numberDirectAreas;
};

struct PharosCpuChannelBulkInfo
{
    /**
     *  pointer to the next channel bulk on the partition 
     */
    LinkedListNode node;

    /**
     * region associated with the bulk. NULL if none
     */
    ptrPharosCpuMpuRegion region;
};

struct PharosCpuPartitionInfo
{
    /**
     * List of channel bulks that the partition owns
     */
    LinkedList channelBulks;

    /**
     * Partition MPU context
     */
    PharosCpuMpuPartitionContext partitionMpuContext;
};

struct PharosCpuChannelBulkQueueInf
{
    /**
     * Size of each bulk [bytes]
     * 
     * Note: application needs to initialize this value
     */
    PharosMpuRegionSize size;
};


/**
 * Switches the MPU regions to the specified context
 * 
 * @note only changes the variable MPU regions. The fixed MPU regions (e.g. text, shared section, ip calls, shared stack) are not changed
 * @note this function is useful when making a filter call, resource access or inter-partition call
 * 
 * @param newContext new context to switch
 */
void pharosCpuMpuSwitchPartition(ptrPharosCpuMpuPartitionContext newContext);


/**
 * Save the CPU MPU to the specified address 
 * 
 * @param mpu address where to save the current MPU
 */
void pharosCpuMpuSave(ptrThreadInfo mpu);


/**
 * Restore to the CPU the specified MPU registers
 * 
 * @param mpu MPU registers to restore
 */
void pharosCpuMpuRestore(ptrThreadInfo mpu);


#endif /* PHAROS_HAL_ARM_ARCH_MPUV7_H */

