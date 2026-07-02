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


#ifndef PHAROS_RISCV_MEMORY_MMU_H
#define PHAROS_RISCV_MEMORY_MMU_H


#include <pharos/hal/declarations.h>
#include <pharos/kernel/partition/declarations.h>
#include <pharos/declarations.h>
#include <pharos/hal/memory/declarations.h>
#include <pharos/hal/hal.h>
#include <pharos/kernel/collection/simplelist.h>
#include <pharos/hal/cpu/conf.h>


#define PHAROS_CPU_MMU_SV39


/**
 * Macro to help the application calculate the number of MMU level 1 tables that a channel bulk requires
 * 
 * For example, if the channel bulk is divided into 4 KiB bulks and there are 32 bulks on the channel bulk  
 * and there are at most 3 partitions using the channel bulk to send (and one to receive) then the number of MMU table 2
 * is (2 + (32 * 4 KiB/1GiB) * (3 + 1 + 1) = 2 * 5 = 10
 * 
 * For example, if the channel bulk is divided into 1 GiB bulks there is no need for MMU level 1 tables 
 * (since the level 1 tables divide the bulks into 2 MiB which are not needed since the MMU level 0 tables already 
 * divide the bulks into pages of 1 GiB)
 * 
 * @param numberBulks number of channel bulks required
 * @param size size in bytes of the buffer of the channel bulk
 * @param senders maximum number of partitions that can get a channel bulk
 */
#define PHAROS_APP_CHANNEL_BULK_TABLE_1(numberBulks , size, partitionUsers)   ( size == PHAROS_CHANNEL_BULK_QUEUE_1G ? 0U : (2U + (numberBulks * size) / VALUE_1GiB) * (partitionUsers + 1U))


/**
 * Macro to help the application calculate the number of MMU level 2 tables that a channel bulk requires
 * 
 * For example, if the channel bulk is divided into 4 KiB bulks and there are 32 bulks on the channel bulk
 * and there are at most 3 partitions using the channel bulk to send (and one to receive) then the number of MMU table 2
 * is (2 + (32 * 4 KiB)/2MiB) * (3 + 1 + 1) = 2 * 5 = 10
 * 
 * For example, if the channel bulk is divided into 4 KiB bulks and there are 2048 bulks on the channel bulk
 * and there are at most 5 partitions using the channel bulk to send and two to receive then the number of MMU table 2
 * is (2 + (2048 * 4 KiB))/2MiB) * (5 + 2 + 1) = 6 * 8 = 48
 * 
 * For example, if the channel bulk is divided into 2 MiB bulks or 1GiB there is no need for MMU level 2 tables 
 * (since the level 2 tables divide the bulks into 4 KiB which are not needed since the MMU level 1 tables already 
 * divide the bulks into pages of 2 MiB)
 * 
 * @param numberBulks number of channel bulks required
 * @param type size in bytes of the buffer of the channel bulk
 * @param senders maximum number of partitions that can get a channel bulk
 */
#define PHAROS_APP_CHANNEL_BULK_TABLE_2(numberBulks , size, partitionUsers)   ( size == PHAROS_CHANNEL_BULK_QUEUE_4K ? (2U + (numberBulks * size) / VALUE_2MiB) * (partitionUsers + 1U) : 0)


#define PHAROS_CPU_MMU_TABLE_NUMBER_ENTRIES        (512U)

/**
 * The SATP register in mode Sv39 (this component is for mode Sv39)
 */
#define PHAROS_CPU_SATP_MODE                        PHAROS_CPU_SATP_MODE_SV39


/**
 * Section for 2MiB pages
 */
#define PAGE_SECTION_2M                             __attribute__ ((section (".pharosPage2M"))) 

/**
 * Section for 4 KiB pages
 */
#define PAGE_SECTION_4K                             __attribute__ ((section (".pharosPage4K"))) 

/**
 * Alignment for the channel bulk table (all MMU tables must be aligned to 4 KiB)
 */
#define PHAROS_CHANNEL_BULK_TABLE_ALIGNMENT         PHAROS_ALIGN_4KiB

/**
 * MMU descriptor
 */
typedef struct
{
    /**
     * Valid bit (should be one for valid, 0 for invalid descriptor)
     */
    uint32_t v : 1;

    /**
     * Read page (one for read, 0 for not be able to read)
     */
    uint32_t r : 1;

    /**
     * Write page (one for write)
     */
    uint32_t w : 1;

    /**
     * Execute page 
     */
    uint32_t x : 1;

    /**
     * User accessible page
     */
    uint32_t u : 1;

    /**
     * Global page
     */
    uint32_t g : 1;

    /**
     * Accessed page
     */
    uint32_t a : 1;

    /**
     * Dirty page
     */
    uint32_t d : 1;

    /**
     * Reserved for later use
     */
    uint32_t rsw : 2;

    /**
     * Physical page address
     */
    uint64_t ppn : 44;

    /**
     * Reserved
     */
    uint32_t reserved : 10;


} PharosCpuMmuDescriptor , *ptrPharosCpuMmuDescriptor;

/**
 * MMU table
 */
struct PharosCpuMmuLevelTable
{
    PharosCpuMmuDescriptor descriptor[PHAROS_CPU_MMU_TABLE_NUMBER_ENTRIES];
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

    /**
     * table of IO memory areas protected by the MMU
     */
    ptrIoMemAreaMmu mmuAreas;

    /**
     * number of items in the table of IO memory areas protected by the MMU
     */
    uint32_t numberMmuAreas;

    /**
     *  additional tables of 4K entries (total 2MiB) to store the MMU areas 
     */
    ptrPharosCpuMmuLevelTable mmuTables4K;

    /**
     *  additional tables of 2M entries (total 1GiB) to store the MMU areas 
     */
    ptrPharosCpuMmuLevelTable mmuTables2M;

    /**
     * number of additional tables of 4K. The number should be equal to the number of mmuAreas with type equal to 4K
     */
    uint32_t numberMmuTables4K;

    /**
     * number of additional tables of 2M. The number should be equal to the number of mmuAreas with type equal to 2M
     */
    uint32_t numberMmuTables2M;
};

struct IoMemAreaDirect
{
    uint8_t *start;
    uint64_t size;
};

struct IoMemAreaMmu
{
    PharosPageTableType type;

    union
    {

        struct
        {
            /**
             * start address of the 4K page section.
             * Must be aligned to 4K. If not, the previous 4K alignment will be considered.
             */
            uint8_t *start;

            /**
             * Should be from 1 to 512 (at most can have 512 4K pages inside a 2MiB memory area)
             */
            uint16_t numberPages;
        } pages4K;

        struct
        {
            /**
             * start address of the 2MiB section. 
             * Must be aligned to 2MiB. If not, the previous 2MiB alignment will be considered.
             */
            uint8_t *start;

            /**
             * number of 2M pages. At most can be 512 (at most can have 512 pages of 2MiB, in total of 1 GiB)
             */
            uint16_t numberPages;
        } pages2M;

    } table;
};

struct PartitionCpuMmuContext
{
    /**
     * value to place on the satp register
     */
    uint64_t satp;
};

struct PharosCpuPartitionInfo
{
    /* nothing needed */
};

struct PharosCpuChannelBulkInfo
{
    /* nothing, using an MMU does not require any additional information */
};

struct PharosPage4K
{
    uint8_t bufferPage4K[VALUE_4KiB];
};

struct PharosPage2M
{
    uint8_t bufferPage2M[VALUE_2MiB];
};

struct PharosPage1G
{
    uint8_t bufferPage1G[VALUE_1GiB];
};

typedef union
{
    ptrPharosPage4K page4K;
    ptrPharosPage2M page2M;
    ptrPharosPage1G page1G;
} PharosPage;

struct PharosCpuChannelBulkQueueInf
{
    /**
     * Size of each bulk on the bulk queue. Three sizes are supported: 4KiB, 64 KiB and 2MiB
     */
    PharosChannelBulkQueueSize type;

    /**
     * Additional MMU page Tables of level 1 (each entry on a table corresponds to 2 MiB)
     * 
     * Note: application must initialize this 
     */
    ptrPharosCpuMmuLevelTable level1Pages[PHAROS_NUMBER_CORES];

    /**
     * Additional MMU page Tables of level 2 (each entry on a table corresponds to 4KiB)
     * 
     * Note: application must initialize this 
     */
    ptrPharosCpuMmuLevelTable level2Pages[PHAROS_NUMBER_CORES];

    /**
     * Number of additional MMU Tables of level 1
     * 
     * It will decrease in time as the level 2 tables are being allocated to the partitions (as they are being used)
     * 
     * Note: application must initialize this!
     * The application should initialize this using the PHAROS_APP_CHANNEL_BULK_TABLE_1 macro
     */
    uint32_t numberLevel1Tables[PHAROS_NUMBER_CORES];

    /**
     * Number of additional MMU Tables of level 2
     * 
     * It will decrease in time as the level 2 tables are being allocated to the partitions (as they are being used)
     * 
     * Note: application must initialize this!
     * The application should initialize this using the PHAROS_APP_CHANNEL_BULK_TABLE_2 macro
     */
    uint32_t numberLevel2Tables[PHAROS_NUMBER_CORES];

    /**
     * Address where the bulk memory area start
     * 
     * Note: application must initialize this field
     */
    PharosPage pages;
};


uint64_t pharosCpuMmuGetSatp(ptrPartition partition);


/**
 * Invalidate the TLB entries for the specified address
 * 
 * @param address address to invalidate
 */
void pharosCpuMmuInvalidateAddress(uint8_t *address);

/* PHAROS_RISCV_MEMORY_MMU_H */
#endif 
