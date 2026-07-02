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


#ifndef PHAROS_HAL_MMU_H
#define PHAROS_HAL_MMU_H


#include <pharos/hal/declarations.h>
#include <pharos/hal/memory/declarations.h>
#include <pharos/hal/hal.h>
#include <pharos/kernel/collection/simplelist.h>
#include <pharos/kernel/partition/declarations.h>


#define NUMBER_SECTIONS_ENTRIES 4096U
#define NUMBER_SMALL_TABLE_ENTRIES 256U

/**
 * Section for 1MiB pages
 */
#define PAGE_SECTION_1M __attribute__ ((section (".pharosPage1M"))) 

/**
 * Section for 4 KiB pages
 */
#define PAGE_SECTION_4K __attribute__ ((section (".pharosPage4K"))) 

/**
 * Calculate the number of tables of 4K based on the number of partitions receivers and the number of pages 
 * 
 * The number of tables per each partition is calculated as:
 * number of pages times the size that each page can store divided by the section size (which in this case is 1MiB)
 * 
 * The number of partitions is the calculated as one for the sender partition and 3 for the receiver partitions.
 * 
 * The number of tables required per each partition is added by 1 because the 4K pages could not be aligned to 1M 
 * and hence an extra table may be required.
 * 
 * @param partitionUsers maximum number of partitions that can send and/or receive a channel bulk
 * @param pages pages in bytes of the buffer of the channel bulk
 */
#define PHAROS_NUMBER_PAGE_TABLE_4K(partitionUsers , pages) ( (2 + (partitionUsers)) * (2 + (VALUE_4KiB * (pages) ) / VALUE_1MiB) )

/**
 * For tables with 1M pages there is no need for additional tables (already included in each partition).
 */
#define PHAROS_NUMBER_PAGE_TABLE_1M(partitionUsers , pages) (0)

#define PHAROS_ALIGN_PAGE_TABLE_4K VARIABLE_ALIGNMENT_X(VALUE_4KiB)

#define PHAROS_ALIGN_PAGE_TABLE_1M VARIABLE_ALIGNMENT_X(VALUE_1MiB)

/**
 * Section descriptor of the MMU is defined by the hardware and its bits must be exactly placed hence it is packed to one byte
 */
#pragma pack(push ,1)

/**
 * Descriptor for a 1MiB section for the MMU 
 */
typedef union
{

    struct
    {
        unsigned int shouldBeZero3 : 1;
        unsigned int shouldBeOne2 : 1;
        unsigned int b : 1;
        unsigned int c : 1;
        unsigned int shouldBeOne1 : 1;
        unsigned int domain : 4;
        unsigned int shouldBeZero2 : 1;
        unsigned int ap : 2;
        unsigned int shouldBeZero1 : 8;
        unsigned int sectionBaseAddress : 12;
    } section;

    struct
    {
        unsigned int shouldBeOne3 : 1;
        unsigned int shouldBeZero2 : 1;
        unsigned int shouldBeZero3 : 2;
        unsigned int shouldBeOne1 : 1;
        unsigned int domain : 4;
        unsigned int shouldBeZero : 1;
        unsigned int coarsePageTableBaseAddress : 22;
    } coarse;

    struct
    {
        unsigned int shouldBeOne3 : 1;
        unsigned int shouldBeOne2 : 1;
        unsigned int shouldBeZero3 : 2;
        unsigned int shouldBeOne1 : 1;
        unsigned int domain : 4;
        unsigned int shouldBeZero : 3;
        unsigned int finePageTableBaseAddress : 20;
    } fine;

    uint32_t value;
} MmuSectionDescriptor , *ptrMmuSectionDescriptor;

typedef struct
{
    unsigned int shouldBeZero : 1;
    unsigned int shouldBeOne : 1;
    unsigned int b : 1;
    unsigned int c : 1;
    unsigned int ap0 : 2;
    unsigned int ap1 : 2;
    unsigned int ap2 : 2;
    unsigned int ap3 : 2;
    unsigned int baseAddress : 20;
} Mmu4KDescriptor , *ptrMmu4KDescriptor;

/**
 * Table of sections of the MMU
 */
typedef struct
{
    MmuSectionDescriptor sectionDescriptor[NUMBER_SECTIONS_ENTRIES];
} MmuSectionTable , *ptrMmuSectionTable;

/**
 * Table of 4KiB pages of the MMU
 */
struct PageTable4K
{
    Mmu4KDescriptor descriptor[NUMBER_SMALL_TABLE_ENTRIES];
} PHAROS_ALIGN_PAGE_TABLE_4K;

struct MemPageTable
{
    ptrPageTable4K table4K;
};

/**
 * restore the previous pack definition
 */
#pragma pack(pop)

struct IoMemAreaMmu
{
    PharosPageTableType type;

    unsigned int isWriteThrough : 1;

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
             * Should be from 1 to 256 (at most can have 256 4K pages inside a 1MiB memory area)
             */
            unsigned int numberPages : 9;
        } pages4K;

        struct
        {
            /**
             * start address of the 1MiB section. 
             * Must be aligned to 1MiB. If not, the previous 1MiB alignment will be considered.
             */
            uint8_t *start;

            /**
             * number of 1M pages
             */
            uint16_t numberPages;
        } pages1M;

    } table;
};

struct IoMemAreaDirect
{
    uint8_t *start;
    uint32_t size;
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
     *  additional tables of 4K to store the MMU areas 
     */
    ptrPageTable4K mmuTables;

    /**
     * number of additional tables of 4K. The number should be equal to the number of mmuAreas with type equal to 4K
     */
    uint32_t numberMmuTables;
};

/**
 * Partition MMU context is the same as the thread MMU context for arm926ej-s
 */
struct PartitionCpuMmuContext
{
    volatile uint32_t tlb;
};

struct PharosCpuMpuPartitionContext
{
    /* nothing, the thread context is the same as the partition context */
};

struct PharosCpuChannelBulkInfo
{
    /* nothing, using an MMU does not require any additional information */
};

struct PharosCpuPartitionInfo
{
    /* nothing, using an MMU does not require any additional information */
};

typedef struct
{
    uint8_t bufferPage512B[512];
} PharosPage512B , *ptrPharosPage512B;

typedef struct
{
    uint8_t bufferPage1K[1024];
} PharosPage1K , *ptrPharosPage1K;

typedef struct
{
    uint8_t bufferPage4K[4 * 1024];
} PharosPage4K , *ptrPharosPage4K;

typedef struct
{
    uint8_t bufferPage16K[16 * 1024];
} PharosPage16K , *ptrPharosPage16K;

typedef struct
{
    uint8_t bufferPage64K[64 * 1024];
} PharosPage64K , *ptrPharosPage64K;

typedef struct
{
    uint8_t bufferPage256K[256 * 1024];
} PharosPage256K , *ptrPharosPage256K;

typedef struct
{
    uint8_t bufferPage1M[1024 * 1024];
} PharosPage1M , *ptrPharosPage1M;

typedef union
{
    ptrPharosPage512B page512B;
    ptrPharosPage1K page1K;
    ptrPharosPage4K page4K;
    ptrPharosPage16K page16K;
    ptrPharosPage64K page64K;
    ptrPharosPage256K page256K;
    ptrPharosPage1M page1M;
} PharosPage;

struct PharosCpuChannelBulkQueueInf
{
    /**
     * Additional MMU page Tables 
     * 
     * Note: application must initialize this (if MMU is present)!
     */
    MemPageTable mmuPageTables;

    /**
     * Number of additional MMU page Tables 
     * 
     * Note: application must initialize this!
     */
    uint32_t numberMmuPageTables;

    /**
     * Address where the bulk memory area start
     * 
     * Note: application must initialize this field
     */
    PharosPage pages;

    /**
     * Index of the mmuPageTables that is free
     */
    uint32_t pageIterator;
};


/**
 * Gets the TLB of the partition
 * 
 * @param id number of the partition
 * 
 * @return returns the TLB of the partition
 */
uint32_t pharosCpuMmuGetTlb(ptrPartition partition);


/**
 * Section table for the MMU
 */
extern MmuSectionTable kernelSectionTable;


#endif /* MMU_H */
