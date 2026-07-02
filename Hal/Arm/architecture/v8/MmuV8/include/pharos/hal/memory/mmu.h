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


#ifndef PHAROS_CPU_MMU_H
#define PHAROS_CPU_MMU_H


#include <pharos/declarations.h>
#include <pharos/hal/memory/declarations.h>
#include <pharos/hal/hal.h>
#include <pharos/kernel/collection/simplelist.h>
#include <pharos/kernel/partition/declarations.h>
#include <pharos/hal/cpu/declarations.h>
#include <pharos/hal/v8a/declarations.h>
#include <pharos/hal/v8a/context.h>


/**
 * Macro to help the application calculate the number of MMU level 2 tables that a channel bulk requires
 * 
 * This is required in general since any partition can (potentially) send or receive bulks from
 * any channel bulk queue. Hence, the MMU tables of the partition have to be updated to allow access (or not) to each 
 * channel bulk as they are send or free (or re-sent to another partition). To be able to set the MMU tables it is 
 * required that the MMU tables are "known" in advance, that is, at compile time. Furthermore, it is required that 
 * they are known per core instance. That is, Pharos must know the maximum number of partitions that can use a bulk 
 * queue per each core. This is because Pharos uses no spin locks to synchronize between cores and therefore each core
 * must have its own data separated from the others.
 * 
 * For example, if the channel bulk is divided into 4 KiB bulks and there are 32 bulks on the channel bulk  
 * and there are at most 3 partitions using the channel bulk to send and one to receive, then the number of MMU table 2
 * is (2 + (32 * 4 KiB/1GiB) * (3 + 1 + 1) = 2 * 4 = 8
 * 
 * @param numberBulks number of channel bulks required
 * @param size size in bytes of the buffer of the channel bulk
 * @param partitionUsers maximum number of partitions that can send and/or receive a channel bulk
 */
#define PHAROS_APP_CHANNEL_BULK_TABLE_2(numberBulks , type, partitionUsers)   ( (2U + (numberBulks * type) / VALUE_1GiB) * (partitionUsers + 1U))


/**
 * Macro to help the application calculate the number of MMU level 3 tables that a channel bulk requires per each core.
 * That is, you must know on your application the maximum number of number of partitions that send or receive channel
 * bulks per each core. 
 * 
 * This is required in general since any partition can (potentially) send or receive bulks from
 * any channel bulk queue. Hence, the MMU tables of the partition have to be updated to allow access (or not) to each 
 * channel bulk as they are send or free (or re-sent to another partition). To be able to set the MMU tables it is 
 * required that the MMU tables are "known" in advance, that is, at compile time. Furthermore, it is required that 
 * they are known per core instance. That is, Pharos must know the maximum number of partitions that can use a bulk 
 * queue per each core. This is because Pharos uses no spin locks to synchronize between cores and therefore each core
 * must have its own data separated from the others.
 * 
 * For example, if the channel bulk is divided into 4 KiB bulks and there are 32 bulks on the channel bulk
 * and there are at most 3 partitions using the channel bulk to send and one to receive, then the number of MMU table 3
 * is (2 + (32 * 4 KiB)/2MiB) * (3 + 1 + 1) = 2 * 5 = 10
 * 
 * For example, if the channel bulk is divided into 4 KiB bulks and there are 2048 bulks on the channel bulk
 * and there are at most 5 partitions using the channel bulk to send and two to receive then the number of MMU table 3
 * is (2 + (2048 * 4 KiB))/2MiB) * (5 + 2 + 1) = 6 * 8 = 48
 * 
 * For example, if the channel bulk is divided into 2 MiB bulks there is no need for MMU level 3 tables 
 * (since the level 3 tables divide the bulks into 4 KiB which are not needed since the MMU level 2 tables already 
 * divide the bulks into pages of 2 MiB.
 * 
 * @param numberBulks number of channel bulks required
 * @param size size in bytes of the buffer of the channel bulk
 * @param partitionUsers maximum number of partitions that can send and/or receive a channel bulk
 */
#define PHAROS_APP_CHANNEL_BULK_TABLE_3(numberBulks , type, partitionUsers)   ( type == PHAROS_CHANNEL_BULK_QUEUE_2M ? 0U : (2U + (numberBulks * type) / VALUE_2MiB) * (partitionUsers + 1U))


#define PHAROS_CHANNEL_BULK_TABLE2_ALIGNMENT PHAROS_ALIGN_4KiB
#define PHAROS_CHANNEL_BULK_TABLE3_ALIGNMENT PHAROS_ALIGN_4KiB


#define PHAROS_CPU_LEVEL0_NUMBER_ENTRIES        (512U)
#define PHAROS_CPU_LEVEL1_NUMBER_ENTRIES        (512U)
#define PHAROS_CPU_LEVEL2_NUMBER_ENTRIES        (512U)
#define PHAROS_CPU_LEVEL3_NUMBER_ENTRIES        (512U)


#define EXECUTE_NEVER       (1U) 
#define EXECUTE             (0U)

#define CONTIGUOUS_YES      (1U)
#define CONTIGUOUS_NO       (0U)

#define GLOBAL_YES          (0U)
#define GLOBAL_NO           (1U)

/**
 * The access flag is set to 0 if an access is made to the page
 */
#define ACCESS_INITIALIZE   (1U)

#define SHAREABLE_NO        (0b00)
#define SHAREABLE_OUTER     (0b10)
#define SHAREABLE_INNER     (0b11)

/**
 * AP bits to define that only EL1 can read and write
 */
#define AP_EL1_RW           (0b00)

/**
 * AP bits to define that EL1 and EL0 can read and write
 */
#define AP_EL1_EL0_RW       (0b01)

/**
 * AP bits to define that only EL0 can only read 
 */
#define AP_EL1_RO           (0b10)

/**
 * AP bits to define that EL1 and EL0 can only read 
 */
#define AP_EL1_EL0_RO       (0b11)

#define SECURE_YES          (0U)
#define SECURE_NO           (1U)



/**
 * Index on the MAIR_EL1 register to place device memory type
 */
#define MAIR_EL1_ATTRIBUTE_DEVICE_INDEX     (1U)

/**
 * Index on the MAIR_EL1 register to place normal memory type
 */
#define MAIR_EL1_ATTRIBUTE_MEMORY_INDEX     (0U)

/**
 * Value to place on the MAIR_EL1 register for device memory type
 */
#define MAIR_EL1_ATTRIBUTE_DEVICE           (0x04)

/**
 * Value to place on the MAIR_EL1 register for normal memory type
 */
#define MAIR_EL1_ATTRIBUTE_MEMORY           (0xff)

#define AP_TABLE_IGNORE                     (0b00)

#define NS_TABLE_SECURE_YES                 (0U)
#define NS_TABLE_SECURE_NO                  (1U)
#define XN_TABLE_IGNORE                     (0U)
#define PXN_TABLE_IGNORE                    (0U)

/**
 * Section for 4 KiB pages.
 * This section is where the bulks of 4 KiB should be placed
 */
#define PAGE_SECTION_4K __attribute__ ((section (".pharosPage4K"))) 


/**
 * Section for 2MiB pages
 * This section is where the bulks of 2 MiB should be placed
 */
#define PAGE_SECTION_2M __attribute__ ((section (".pharosPage2M"))) 


/**
 * Section descriptor of the MMU is defined by the hardware and its bits must be exactly placed hence it is packed to one byte
 */
#pragma pack(push ,1)

/**
 * Block descriptor for level 1 
 */
struct PharosCpuMmuBlock1Descriptor
{
    uint32_t shouldBeZeroOne : 2;

    /**
     * lower attributes
     */
    uint32_t attrIndx : 3;
    uint32_t ns : 1;
    uint32_t ap : 2;
    uint32_t sh : 2;
    uint32_t af : 1;
    uint32_t ng : 1;

    /**
     * offset of the physical memory
     */
    uint64_t address : 36;
    uint32_t shouldBeZero2 : 4;

    /**
     * upper attributes
     */
    uint32_t contiguous : 1;
    uint32_t pxn : 1;
    uint32_t uxn_xn : 1;
    uint32_t reserved : 4;
    uint32_t ignored : 5;
};

/**
 * Block descriptor for level 2 
 */
struct PharosCpuMmuBlock2Descriptor
{
    uint32_t shouldBeZeroOne : 2;

    /**
     * lower attributes
     */
    uint32_t attrIndx : 3;
    uint32_t ns : 1;
    uint32_t ap : 2;
    uint32_t sh : 2;
    uint32_t af : 1;
    uint32_t ng : 1;

    /**
     * offset of the physical memory
     */
    uint64_t address : 36;
    uint32_t shouldBeZero2 : 4;

    /**
     * upper attributes
     */
    uint32_t contiguous : 1;
    uint32_t pxn : 1;
    uint32_t uxn_xn : 1;
    uint32_t reserved : 4;
    uint32_t ignored : 5;
};

/**
 * Table descriptor for level 0, 1 or 2
 */
struct PharosCpuMmuTableDescriptor
{
    uint32_t shouldBeOneOne : 2;
    uint32_t ignored : 10;

    /**
     * address of the level 1 table
     */
    uint64_t address : 36;
    uint32_t shouldBeZero : 4;
    uint32_t ignored2 : 7;
    uint32_t pxnTable : 1;
    uint32_t uxn_xnTable : 1;
    uint32_t apTable : 1;
    uint32_t nsTable : 1;
};

/**
 * Invalid descriptor
 */
struct PharosCpuMmuInvalidDescriptor
{
    uint32_t shouldBeZero : 1;
    uint64_t ignore : 63;
};

struct PharosCpuMmuPage4KDescriptor
{
    uint32_t shouldBeOneOne : 2;

    /**
     * lower attributes
     */
    uint32_t attrIndx : 3;
    uint32_t ns : 1;
    uint32_t ap : 2;
    uint32_t sh : 2;
    uint32_t af : 1;
    uint32_t ng : 1;

    /**
     * address of the level 3 table
     */
    uint64_t address : 36;

    uint32_t shouldBeZero : 4;

    /**
     * upper attributes
     */
    uint32_t contiguous : 1;
    uint32_t pxn : 1;
    uint32_t uxn_xn : 1;
    uint32_t reserved : 4;
    uint32_t ignored : 5;
};

/**
 * Level 0 descriptor for 4 KiB page addressing
 */
typedef union
{
    uint64_t descriptor;

    PharosCpuMmuTableDescriptor table;
    PharosCpuMmuInvalidDescriptor invalid;

} PharosCpuMmuLevel0Descriptor , *ptrPharosCpuMmuLevel0Descriptor;

/**
 * Level 1 descriptor for 4 KiB page addressing
 */
typedef union
{
    uint64_t descriptor;

    PharosCpuMmuTableDescriptor table;
    PharosCpuMmuBlock1Descriptor block;
    PharosCpuMmuInvalidDescriptor invalid;
} PharosCpuMmuLevel1Descriptor , *ptrPharosCpuMmuLevel1Descriptor;

/**
 * Level 2 descriptor for 4 KiB page addressing
 */
typedef union
{
    uint64_t descriptor;

    PharosCpuMmuTableDescriptor table;
    PharosCpuMmuBlock2Descriptor block;
    PharosCpuMmuInvalidDescriptor invalid;
} PharosCpuMmuLevel2Descriptor , *ptrPharosCpuMmuLevel2Descriptor;

/**
 * Level 3 descriptor for 4 KiB page addressing
 */
typedef union
{
    PharosCpuMmuPage4KDescriptor descriptor;
    PharosCpuMmuInvalidDescriptor invalid;
    uint64_t raw;
} PharosCpuMmuLevel3Descriptor , *ptrPharosCpuMmuLevel3Descriptor;

/**
 * MMU level 0 table
 */
typedef struct
{
    PharosCpuMmuLevel0Descriptor descriptor[PHAROS_CPU_LEVEL0_NUMBER_ENTRIES];
} PharosCpuMmuLevel0Table , *ptrPharosCpuMmuLevel0Table;

/**
 * MMU level 1 table
 */
typedef struct
{
    PharosCpuMmuLevel1Descriptor descriptor[PHAROS_CPU_LEVEL1_NUMBER_ENTRIES];
} PharosCpuMmuLevel1Table , *ptrPharosCpuMmuLevel1Table;

/**
 * MMU level 2 table
 */
struct PharosCpuMmuLevel2Table
{
    PharosCpuMmuLevel2Descriptor descriptor[PHAROS_CPU_LEVEL2_NUMBER_ENTRIES];
};

/**
 * MMU level 3 table
 */
struct PharosCpuMmuLevel3Table
{
    PharosCpuMmuLevel3Descriptor descriptor[PHAROS_CPU_LEVEL3_NUMBER_ENTRIES];
};

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
     *  additional tables of 4K entries (total 2MiB) to store the MMU areas 
     */
    ptrPharosCpuMmuLevel3Table mmuTables4K;

    /**
     *  additional tables of 2M entries (total 1GiB) to store the MMU areas 
     */
    ptrPharosCpuMmuLevel2Table mmuTables2M;

    /**
     *  additional tables of 1G entries (total 512GiB) to store the MMU areas 
     */
    ptrPharosCpuMmuLevel1Table mmuTables1G;

    /**
     * number of additional tables of 4K. The number should be equal to the number of mmuAreas with type equal to 4K
     */
    uint32_t numberMmuTables4K;

    /**
     * number of additional tables of 2M. The number should be equal to the number of mmuAreas with type equal to 2M
     */
    uint32_t numberMmuTables2M;

    /**
     * number of additional tables of 1G. The number should be equal to the number of mmuAreas with type equal to 1G
     */
    uint32_t numberMmuTables1G;
};

/**
 * Partition MMU context is the same as the thread MMU context
 */
struct PartitionCpuMmuContext
{
    ptrPharosCpuMmuLevel0Table level0Table;
    ptrPharosCpuMmuLevel1Table level1Table;
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
    /* nothing needed */
};

struct PharosPage4K
{
    uint8_t bufferPage4K[VALUE_4KiB];
};

struct PharosPage64K
{
    uint8_t bufferPage64K[VALUE_64KiB];
};

struct PharosPage2M
{
    uint8_t bufferPage2M[VALUE_2MiB];
};

struct MemPageTable
{
    ptrPharosCpuMmuLevel3Table table4K;
};

typedef union
{
    ptrPharosPage4K page4K;
    ptrPharosPage64K page64K;
    ptrPharosPage2M page2M;
} PharosPage;


/**
 * restore the pack definition
 */
#pragma pack(pop)

struct PharosCpuChannelBulkQueueInf
{
    /**
     * Size of each bulk on the bulk queue. Three sizes are supported: 4KiB, 64 KiB and 2MiB
     */
    PharosChannelBulkQueueSize type;

    /**
     * Additional MMU page Tables of level 2 (each entry on a table corresponds to 2 MiB)
     * 
     * Note: application must initialize this 
     */
    ptrPharosCpuMmuLevel2Table level2Pages[PHAROS_NUMBER_CORES];

    /**
     * Additional MMU page Tables of level 3 (each entry on a table corresponds to 4KiB)
     * 
     * Note: application must initialize this 
     */
    ptrPharosCpuMmuLevel3Table level3Pages[PHAROS_NUMBER_CORES];

    /**
     * Number of additional MMU Tables of level 2
     * 
     * It will decrease in time as the level 3 tables are being allocated to the partitions (as they are being used)
     * 
     * Note: application must initialize this!
     * The application should initialize this using the PHAROS_APP_CHANNEL_BULK_TABLE_2 macro
     */
    uint32_t numberLevel2Tables[PHAROS_NUMBER_CORES];

    /**
     * Number of additional MMU Tables of level 2
     * 
     * It will decrease in time as the level 3 tables are being allocated to the partitions (as they are being used)
     * 
     * Note: application must initialize this!
     * The application should initialize this using the PHAROS_APP_CHANNEL_BULK_TABLE_3 macro
     */
    uint32_t numberLevel3Tables[PHAROS_NUMBER_CORES];

    /**
     * Address where the bulk memory area start
     * 
     * Note: application must initialize this field
     */
    PharosPage pages;
};


/**
 * Gets the TLB of the partition
 * 
 * @param id number of the partition
 * 
 * @return returns the TLB of the partition
 */
ptrPharosCpuMmuLevel0Table pharosCpuMmuGetTlb(ptrPartition partition);


/**
 * Set the MAIR_EL1 register
 * 
 * @param mair new MAIR register value
 */
void pharosCpuMair_El1Set(uint64_t mair);


/**
 * Enables the MMU
 */
void pharosCpuEnableMmu(void);


/**
 * Invalidate the TLB entries for the specified address
 * 
 * @param address address to invalidate
 */
void pharosCpuMmuInvalidateAddress(uint8_t *address);

/* PHAROS_CPU_MMU_H */
#endif 
