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


#ifndef PHAROS_MPUV7_H
#define PHAROS_MPUV7_H

#include <pharos/hal/declarations.h>
#include <pharos/hal/memory/declarations.h>
#include <pharos/kernel/partition/declarations.h>
#include <pharos/hal/cpu/conf.h>
#include <pharos/kernel/collection/linkedlist.h>
#include <pharos/hal/memory/pmsa.h>
#include <pharos/kernel/thread/declarations.h>

typedef enum
{
    /**
     * Smallest region of 256 bytes
     */
    PHAROS_MPU_REGION_SIZE_256B = 256U ,
    PHAROS_MPU_REGION_SIZE_512B = 512U ,
    PHAROS_MPU_REGION_SIZE_1K = VALUE_1KiB ,
    PHAROS_MPU_REGION_SIZE_2K = (2U * VALUE_1KiB) ,
    PHAROS_MPU_REGION_SIZE_4K = (4U * VALUE_1KiB) ,
    PHAROS_MPU_REGION_SIZE_8K = (8U * VALUE_1KiB) ,
    PHAROS_MPU_REGION_SIZE_16K = (16U * VALUE_1KiB) ,
    PHAROS_MPU_REGION_SIZE_32K = (32U * VALUE_1KiB) ,
    PHAROS_MPU_REGION_SIZE_64K = (64U * VALUE_1KiB) ,
    PHAROS_MPU_REGION_SIZE_128K = (128U * VALUE_1KiB) ,
    PHAROS_MPU_REGION_SIZE_256K = (256U * VALUE_1KiB) ,
    PHAROS_MPU_REGION_SIZE_512K = (512U * VALUE_1KiB) ,
    PHAROS_MPU_REGION_SIZE_1M = (1U * VALUE_1MiB) ,
    PHAROS_MPU_REGION_SIZE_2M = (2U * VALUE_1MiB) ,
    PHAROS_MPU_REGION_SIZE_4M = (4U * VALUE_1MiB) ,
    PHAROS_MPU_REGION_SIZE_8M = (8U * VALUE_1MiB) ,
    PHAROS_MPU_REGION_SIZE_16M = (16U * VALUE_1MiB) ,
    PHAROS_MPU_REGION_SIZE_32M = (32U * VALUE_1MiB) ,
    PHAROS_MPU_REGION_SIZE_64M = (64U * VALUE_1MiB) ,
    PHAROS_MPU_REGION_SIZE_128M = (128U * VALUE_1MiB) ,
    PHAROS_MPU_REGION_SIZE_256M = (256U * VALUE_1MiB) ,
    PHAROS_MPU_REGION_SIZE_512M = (512U * VALUE_1MiB) ,
    PHAROS_MPU_REGION_SIZE_1G = (1U * VALUE_1GiB) ,
    PHAROS_MPU_REGION_SIZE_2G = (2U * VALUE_1GiB) ,

    /**
     * Largest region of 4G
     */

    PHAROS_MPU_REGION_SIZE_4G = (4U * VALUE_1GiB) ,
} PharosMpuRegionSize;

typedef struct
{
    uint32_t base;
    uint32_t size;
    uint32_t attributes;


} PharosMpuV7RRegion;

typedef struct
{
    /**
     * region
     */
    PharosMpuV7RRegion region;

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
 *  MPU region 1 is already assigned for IPC read-only data
 *  MPU region 2 is already assigned for shared data
 *  MPU region 3 for shared stack
 * 
 * So the list of dynamically assigned regions can only go from 4 up to PHAROS_CPU_MPU_NUMBER_REGIONS
 */
typedef struct
{
    /**
     * List of free regions that can be dynamically assigned
     */
    LinkedList freeRegions;

    /**
     * List of used regions that can be dynamically assigned
     */
    LinkedList usedRegions;

    /**
     * number of memory regions used for the partition and I/O
     */
    uint32_t numberPartitionAndIoRegions;

    /**
     * Shared stack region
     */
    PharosCpuMpuRegion sharedStackRegion;

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
 *  - one for the text
 *  - one for the shared data
 *  - one for the partition data/bss
 *  - one for the shared stack (if needed)
 *  - the remaining are attributed to the last direct I/O access that triggered an exception 
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
     * Value to place (address) on the DRBAR register relative to the partition data areas
     */
    uint32_t base[PHAROS_NUMBER_MEMORY_AREAS];

    /**
     * Value to place (size) on the DRSR register relative to the partition data areas
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
 * 
 * @return 
 */
void pharosPmsrBusEventExportEnable(void);


/**
 * Initialize the VIC
 */
void pharosCpuVicInitialize(void);


/**
 * Restore to the CPU the specified MPU registers
 * 
 * @param thread thread to restore
 */
void pharosCpuMpuRestore(ptrThreadInfo thread);


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
 * Perform the context switch to the first thread (without changing the MPU registers)
 * 
 * @param heir first thread to run
 */
void pharosCpuRestoreHeirLow(ptrThreadCpuContext heir) __attribute__((noreturn));


#endif /* MPUV7_H */
