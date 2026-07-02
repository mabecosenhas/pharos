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


#include <pharos/hal/memory/mmu.h>
#include <pharos/hal/cpu/asm.h>
#include <pharos/kernel/partition/partition.h>
#include <pharos/channel.h>
#include <pharos/kernel/error/error.h>
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/hal/cpu/sections.h>
#include <pharos/kernel/channel/channelbulkkernel.h>


extern uint32_t pharosNumberSharedStackMmuTables2PerPartition;

extern PharosCpuMmuLevelTable pharosPartition0MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition1MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition2MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition3MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition4MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition5MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition6MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition7MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition8MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition9MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition10MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition11MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition12MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition13MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition14MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition15MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition16MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition17MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition18MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition19MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition20MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition21MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition22MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition23MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition24MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition25MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition26MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition27MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition28MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition29MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition30MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition31MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition32MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition33MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition34MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition35MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition36MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition37MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition38MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition39MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition40MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition41MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition42MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition43MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition44MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition45MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition46MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition47MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition48MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition49MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition50MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition51MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition52MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition53MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition54MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition55MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition56MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition57MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition58MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition59MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition60MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition61MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition62MmuLevel1Start;
extern PharosCpuMmuLevelTable pharosPartition63MmuLevel1Start;


extern PharosCpuMmuLevelTable pharosSharedStackTables2Start[];


/**
 * All the level 0 partition tables
 */
KERNEL_SECTION_USS static PharosCpuMmuLevelTable pharosPartitionLevel0Mmu[PHAROS_PARTITION_MAXIMUM_NUMBER] PHAROS_ALIGN_4KiB;


const ptrPharosCpuMmuLevelTable partitionLevel1Tables[] = {
    &pharosPartition0MmuLevel1Start ,
    &pharosPartition1MmuLevel1Start ,
    &pharosPartition2MmuLevel1Start ,
    &pharosPartition3MmuLevel1Start ,
    &pharosPartition4MmuLevel1Start ,
    &pharosPartition5MmuLevel1Start ,
    &pharosPartition6MmuLevel1Start ,
    &pharosPartition7MmuLevel1Start ,
    &pharosPartition8MmuLevel1Start ,
    &pharosPartition9MmuLevel1Start ,
    &pharosPartition10MmuLevel1Start ,
    &pharosPartition11MmuLevel1Start ,
    &pharosPartition12MmuLevel1Start ,
    &pharosPartition13MmuLevel1Start ,
    &pharosPartition14MmuLevel1Start ,
    &pharosPartition15MmuLevel1Start ,
    &pharosPartition16MmuLevel1Start ,
    &pharosPartition17MmuLevel1Start ,
    &pharosPartition18MmuLevel1Start ,
    &pharosPartition19MmuLevel1Start ,
    &pharosPartition20MmuLevel1Start ,
    &pharosPartition21MmuLevel1Start ,
    &pharosPartition22MmuLevel1Start ,
    &pharosPartition23MmuLevel1Start ,
    &pharosPartition24MmuLevel1Start ,
    &pharosPartition25MmuLevel1Start ,
    &pharosPartition26MmuLevel1Start ,
    &pharosPartition27MmuLevel1Start ,
    &pharosPartition28MmuLevel1Start ,
    &pharosPartition29MmuLevel1Start ,
    &pharosPartition30MmuLevel1Start ,
    &pharosPartition31MmuLevel1Start ,
    &pharosPartition32MmuLevel1Start ,
    &pharosPartition33MmuLevel1Start ,
    &pharosPartition34MmuLevel1Start ,
    &pharosPartition35MmuLevel1Start ,
    &pharosPartition36MmuLevel1Start ,
    &pharosPartition37MmuLevel1Start ,
    &pharosPartition38MmuLevel1Start ,
    &pharosPartition39MmuLevel1Start ,
    &pharosPartition40MmuLevel1Start ,
    &pharosPartition41MmuLevel1Start ,
    &pharosPartition42MmuLevel1Start ,
    &pharosPartition43MmuLevel1Start ,
    &pharosPartition44MmuLevel1Start ,
    &pharosPartition45MmuLevel1Start ,
    &pharosPartition46MmuLevel1Start ,
    &pharosPartition47MmuLevel1Start ,
    &pharosPartition48MmuLevel1Start ,
    &pharosPartition49MmuLevel1Start ,
    &pharosPartition50MmuLevel1Start ,
    &pharosPartition51MmuLevel1Start ,
    &pharosPartition52MmuLevel1Start ,
    &pharosPartition53MmuLevel1Start ,
    &pharosPartition54MmuLevel1Start ,
    &pharosPartition55MmuLevel1Start ,
    &pharosPartition56MmuLevel1Start ,
    &pharosPartition57MmuLevel1Start ,
    &pharosPartition58MmuLevel1Start ,
    &pharosPartition59MmuLevel1Start ,
    &pharosPartition60MmuLevel1Start ,
    &pharosPartition61MmuLevel1Start ,
    &pharosPartition62MmuLevel1Start ,
    &pharosPartition63MmuLevel1Start ,
};


KERNEL_SECTION_USS PharosCpuMmuLevelTable kernelTable0Table[PHAROS_NUMBER_CORES] PHAROS_ALIGN_4KiB;

static bool checkIntervalsIntersect(const uint8_t *a0 , const uint8_t *a1 , const uint8_t *b0 , const uint8_t *b1);

static void initializeLevel0KernelTable(ptrPharosCpuMmuLevelTable table);

static void initializeLevel0Table(ptrPharosCpuMmuLevelTable table , ptrPharosCpuMmuLevelTable *iterator1 , ptrEnvMemoryProtection env);

static void initializeLevel1Table(ptrPharosCpuMmuLevelTable table1 , uint8_t *address , ptrEnvMemoryProtection protection);

static bool hasAnyPermission(uint8_t *address , uint64_t size , ptrEnvMemoryProtection protection);

static bool hasAnyPermission(uint8_t *address , uint64_t size , ptrEnvMemoryProtection protection);

static void sharedStackChangePermissions(ptrStackShared sharedStack , ptrPartitionCpuMmuContext context , bool uValue);

static void initSharedStack(ptrPharosCpuMmuLevelTable tableLevel0 , ptrPharosCpuMmuLevelTable *iterator2);

static bool descriptorIsInvalid(PharosCpuMmuDescriptor descritor);
static bool descriptorIsNextTable(PharosCpuMmuDescriptor descritor);

static ptrPharosCpuMmuLevelTable getTable(PharosCpuMmuDescriptor descriptor);


static void table2LevelAdd(ptrPharosCpuMmuLevelTable table1 , ptrPharosCpuMmuLevelTable table2 , uint16_t indexLevel1 , uint8_t *stackStart);

static void table1LevelAddInvalid(ptrPharosCpuMmuLevelTable table , ptrPharosCpuMmuLevelTable table1Iterator , uint16_t indexLevel0);
static void table2LevelAddInvalid(ptrPharosCpuMmuLevelTable table1 , ptrPharosCpuMmuLevelTable table2 , uint16_t indexLevel1);

static void initializeChannelBulkTable(ptrPharosCpuMmuLevelTable table);

static ptrPharosCpuMmuLevelTable linkLevel2Table(ptrPharosCpuMmuLevelTable table1 , uint16_t index1 , ptrPharosCpuMmuLevelTable table2 , uint8_t * address);

static uint32_t add4KBulk(ptrPharosCpuMmuLevelTable table1 , ptrChannelBulkQueue queue , uint8_t * address , ptrPartition partition);
static uint32_t add2MBulk(ptrPharosCpuMmuLevelTable table1 , ptrChannelBulkQueue queue , uint8_t * address , ptrPartition partition);
static uint32_t add1GBulk(ptrPharosCpuMmuLevelTable table0 , ptrChannelBulkQueue queue , uint8_t * address , ptrPartition partition);

static uint16_t calculateIndex1G(uint8_t *address);
static uint16_t calculateIndex2M(uint8_t *address , uint16_t index0);
static uint16_t calculateIndex4K(uint8_t *address , uint16_t index0 , uint16_t index1);

static uint64_t table2Ppn(ptrPharosCpuMmuLevelTable table);

static void changeDescriptorPermissionsAddress(ptrPharosCpuMmuDescriptor descriptor , uint32_t uValue , uint8_t * address);

static void remove4KBulk(ptrPharosCpuMmuLevelTable table1 , ptrChannelBulkQueue queue , uint8_t * address);
static void remove2MBulk(ptrPharosCpuMmuLevelTable table1 , ptrChannelBulkQueue queue , uint8_t * address);

static void remove1GBulk(ptrPharosCpuMmuLevelTable table1 , ptrChannelBulkQueue queue , uint8_t * address);

static void initializeIoMemArea(ptrPharosCpuMmuLevelTable table0 , ptrIoMemAreaTable table);
static void table1DescriptorInitIo(ptrPharosCpuMmuLevelTable table , ptrPharosCpuMmuLevelTable *table1Iterator , uint8_t *address);
static void table2DescriptorInitIo(ptrPharosCpuMmuLevelTable table , ptrPharosCpuMmuLevelTable *table1Iterator , ptrPharosCpuMmuLevelTable *table2Iterator ,
                                   uint8_t *address);

static void tableDescriptorInitBlock2Io(ptrPharosCpuMmuDescriptor descriptor , uint8_t *address);


void initializeLevel0KernelTable(ptrPharosCpuMmuLevelTable table)
{
    /* iterator on the table */
    uint16_t i;
    uint8_t *address = 0;

    /* for the full 512 GiB , at steps of 1 GiB */
    for(i = 0; i < PHAROS_CPU_MMU_TABLE_NUMBER_ENTRIES; i++ , address += VALUE_1GiB)
    {
        /* copy the address on the previous table */
        table->descriptor[i].v = 1;

        /* kernel can read any address */
        table->descriptor[i].r = 1;

        /* kernel can write any address */
        table->descriptor[i].w = 1;

        /* kernel can execute any address */
        table->descriptor[i].x = 1;

        /* reserved bits */
        table->descriptor[i].rsw = 0;

        /* mark it as accessed so a trap is not triggered when accessing it for the first time */
        table->descriptor[i].a = 1;

        /* mark it as dirty so a trap is not triggered when writing it for the first time */
        table->descriptor[i].d = 1;

        /* since this is the kernel table the user has no access to it */
        table->descriptor[i].u = 0;

        /* this descriptor is not global */
        table->descriptor[i].g = 0;

        /* initialize it with real-addressing */
        table->descriptor[i].ppn = ((uintptr_t) address) >> 12;
    }
}


void initializeLevel0Table(ptrPharosCpuMmuLevelTable table , ptrPharosCpuMmuLevelTable *iterator1 , ptrEnvMemoryProtection env)
{
    /* iterator on the table */
    uint16_t i;
    uint8_t *address = 0;


    /* for the full 512 GiB , at steps of 1 GiB */
    for(i = 0; i < PHAROS_CPU_MMU_TABLE_NUMBER_ENTRIES; i++ , address += VALUE_1GiB)
    {
        /* check if the 1 GiB are used by the partition  */
        if(hasAnyPermission(address , VALUE_1GiB , env) == TRUE)
        {
            /* copy the address on the previous table */
            table->descriptor[i].v = 1;

            /* set the RWX to make the descriptor a pointer to a next level table */
            table->descriptor[i].r = 0;
            table->descriptor[i].w = 0;
            table->descriptor[i].x = 0;

            /* reserved bits */
            table->descriptor[i].rsw = 0;

            /* mark it as accessed so a trap is not triggered when accessing it for the first time */
            table->descriptor[i].a = 1;

            /* mark it as dirty so a trap is not triggered when writing it for the first time */
            table->descriptor[i].d = 1;

            /* since the user has some access to it we mark it with user access */
            table->descriptor[i].u = 1;

            /* this descriptor is not global */
            table->descriptor[i].g = 0;

            /* initialize it with the address of the next level table */
            table->descriptor[i].ppn = ((uintptr_t) * iterator1) >> 12;

            /* we need an extra table for this area */
            initializeLevel1Table(*iterator1 , address , env);

            /* increase the iterator for level 1 tables */
            (*iterator1)++;
        }
        else
        {
            /* place invalid descriptor */
            table->descriptor[i].v = 0;

            /* initialize it with the address */
            table->descriptor[i].ppn = ((uintptr_t) address) >> 12;
        }
    }
}


void initializeLevel1Table(ptrPharosCpuMmuLevelTable table1 , uint8_t *address , ptrEnvMemoryProtection protection)
{
    uint16_t i;


    /* go through the 1 GiB of memory, starting at address and going 2 MiB at a time */
    for(i = 0; i < PHAROS_CPU_MMU_TABLE_NUMBER_ENTRIES; i++ , address += VALUE_2MiB)
    {
        /* set real-addressing mode */
        table1->descriptor[i].ppn = ((uintptr_t) address) >> 12;

        /* set as valid */
        table1->descriptor[i].v = 1;

        /* reset the read permission */
        table1->descriptor[i].r = 1;

        /* reset the write permission (will be overwritten later) */
        table1->descriptor[i].w = 1;

        /* reset the execute permission (will be overwritten later) */
        table1->descriptor[i].x = 0;

        /* reserved bits */
        table1->descriptor[i].rsw = 0;

        /* mark it as accessed so a trap is not triggered when accessing it for the first time */
        table1->descriptor[i].a = 1;

        /* mark it as dirty so a trap is not triggered when writing it for the first time */
        table1->descriptor[i].d = 1;

        /* since the user has some access to it we mark it with user access (will be overwritten if needed) */
        table1->descriptor[i].u = 1;

        /* this descriptor is not global */
        table1->descriptor[i].g = 0;

        /* if inside the text zone */
        if(checkIntervalsIntersect(address , address + VALUE_2MiB , &pharosStartText , &pharosEndText) == TRUE)
        {
            /* allow the partition to execute the page */
            table1->descriptor[i].x = 1;

            /* don't allow the partition to write */
            table1->descriptor[i].w = 0;

            /* don't allow the partition to read */
            table1->descriptor[i].r = 1;
        }
            /* if inside the environment memory zone */
        else if((address >= protection->startAddress[0]) && (address < protection->endAddress[0]))
        {
            /* allow the partition to write to the page */
            table1->descriptor[i].w = 1;
        }
            /* if inside the shared memory zone */
        else if((address >= &pharosSharedStart) && (address < &pharosSharedEnd))
        {
            /* don't allow the partition to write to the shared stack (we will allow it later on) */
            table1->descriptor[i].w = 1;

            /* don't allow the partition to read from the shared stack (we will allow it later on) */
            table1->descriptor[i].r = 1;
        }
            /* if inside the inter-partition calls zone */
        else if((address >= &pharosIpCallStart) && (address < &pharosIpCallEnd))
        {
            /* allow the partition to execute the page */
            table1->descriptor[i].x = 1;

            /* cannot write to ip section */
            table1->descriptor[i].w = 0;

        }
        else if((address >= &pharosKernelStart) && (address < &pharosKernelEnd))
        {
            /* allow the partition to execute the page (for direct I/O)  */
            table1->descriptor[i].x = 1;

            /* user has no access to it */
            table1->descriptor[i].u = 0;

            /* don't allow the partition to write */
            table1->descriptor[i].w = 0;

            /* don't allow the partition to read */
            table1->descriptor[i].r = 0;
        }
        else
        {
            /* user has no access to it */
            table1->descriptor[i].u = 0;
        }
    }
}


uint16_t calculateIndex1G(uint8_t *address)
{
    /* calculate the index on 1 GiB sections */
    return ((uintptr_t) address) >> 30U;
}


uint16_t calculateIndex2M(uint8_t *address , uint16_t index0)
{
    /* calculate the index on 2 MiB sections given the offset of 1 GiB */
    return ((uintptr_t) address - index0 * VALUE_1GiB) >> 21U;
}


uint16_t calculateIndex4K(uint8_t *address , uint16_t index0 , uint16_t index1)
{
    /* calculate the index on 4 KiB sections given the offset of 512 GiB, 1 GiB and 2 MiB */
    return ((uintptr_t) address - index0 * VALUE_1GiB - index1 * VALUE_2MiB) >> 12U;
}


INLINE ptrPharosCpuMmuLevelTable satpGetTable(uint64_t satp)
{
    /* get the table (it is offset by 12 bits) */
    return (ptrPharosCpuMmuLevelTable) (satp << 12);
}


ptrPharosCpuMmuLevelTable getTable(PharosCpuMmuDescriptor descriptor)
{
    /* check if the descriptor is a level 1 table */
    if(descriptor.r == 0U && descriptor.w == 0U && descriptor.x == 0U)
    {
        /* calculate the address of the table */
        return (ptrPharosCpuMmuLevelTable) ((uintptr_t) descriptor.ppn << 12U);
    }
        /* not a table */
    else
    {
        /* return not a table */
        return NULL;
    }
}


void table2LevelAddInvalid(ptrPharosCpuMmuLevelTable table1 , ptrPharosCpuMmuLevelTable table2 , uint16_t indexLevel1)
{
    /* iterator of the level 2 tables */
    uint32_t i;


    /* copy the address on the previous table */
    table1->descriptor[indexLevel1].ppn = (uintptr_t) (table2) >> 12;

    /* initialize the table 1 property to point to a table 2 */
    table1->descriptor[indexLevel1].r = 0;
    table1->descriptor[indexLevel1].w = 0;
    table1->descriptor[indexLevel1].x = 0;
    table1->descriptor[indexLevel1].u = 1;

    /* go to every level 2 descriptor and initialize it */
    for(i = 0U; i < PHAROS_CPU_MMU_TABLE_NUMBER_ENTRIES; i++)
    {
        /* initialize with invalid */
        table2->descriptor[i].v = 0;
    }
}


void table2LevelAdd(ptrPharosCpuMmuLevelTable table1 , ptrPharosCpuMmuLevelTable table2 , uint16_t indexLevel1 , uint8_t *address)
{
    /* iterator of the level 2 tables */
    uint32_t i;


    /* copy the address on the previous table */
    table1->descriptor[indexLevel1].ppn = (uintptr_t) (table2) >> 12;

    /* initialize the table 1 property to point to a table 2 */
    table1->descriptor[indexLevel1].r = 0;
    table1->descriptor[indexLevel1].w = 0;
    table1->descriptor[indexLevel1].x = 0;
    table1->descriptor[indexLevel1].u = 1;

    /* go to every level 2 descriptor and initialize it */
    for(i = 0U; i < PHAROS_CPU_MMU_TABLE_NUMBER_ENTRIES; i++ , address += VALUE_4KiB)
    {
        /* initialize with valid */
        table2->descriptor[i].v = 1;

        /* mark it as can read */
        table2->descriptor[i].r = 1;

        /* mark it as can write */
        table2->descriptor[i].w = 1;

        /* mark it as cannot execute */
        table2->descriptor[i].x = 0;

        /* mark it as user cannot access it (will be changed if needed) */
        table2->descriptor[i].u = 0;

        /* non-global */
        table2->descriptor[i].g = 0;

        /* reserved bits */
        table2->descriptor[i].rsw = 0;

        /* mark it as accessed so a trap is not triggered when accessing it for the first time */
        table2->descriptor[i].a = 1;

        /* mark it as dirty so a trap is not triggered when writing it for the first time */
        table2->descriptor[i].d = 1;

        /* set real-addressing */
        table2->descriptor[i].ppn = (uintptr_t) address >> 12;
    }
}


bool descriptorIsInvalid(PharosCpuMmuDescriptor descritor)
{
    /* check the valid bit */
    return descritor.v == 0U ? TRUE : FALSE;
}


bool descriptorIsNextTable(PharosCpuMmuDescriptor descritor)
{
    /* check if it is valid and check the r,w and x bits (all must be 0) */
    return ((descritor.v == 1U) && (descritor.r == 0U) && (descritor.w == 0U) && (descritor.x == 0U)) ? TRUE : FALSE;
}


void initializeIoMemArea(ptrPharosCpuMmuLevelTable table0 , ptrIoMemAreaTable table)
{
    /* iterator of the MMU 4K tables used (if 4KiB tables are selected) */
    ptrPharosCpuMmuLevelTable table4KIterator = &table->mmuTables4K[0];

    /* iterator of the MMU 2M tables used (if 2MiB tables are selected) */
    ptrPharosCpuMmuLevelTable table2MIterator = &table->mmuTables2M[0];

    /* iterators of the MMU areas and the 4KiB/2MiB tables */
    uint32_t i , j;


    /* for each I/O area */
    for(i = 0U; i < table->numberMmuAreas; i++)
    {
        /* get the I/O area */
        ptrIoMemAreaMmu area = &table->mmuAreas[i];


        /* if the table is 2 MiB */
        if(area->type == PHAROS_PAGE_TABLE_2M)
        {
            /* get the start address */
            uint8_t * start = area->table.pages2M.start;

            for(j = 0U; j < area->table.pages2M.numberPages; j++ , start += VALUE_2MiB)
            {
                /* initialize the level 0 table descriptor with I/O permissions, adding a table level 1 if needed */
                table1DescriptorInitIo(table0 , &table2MIterator , start);
            }
        }
            /* else, the table is 4 KiB */
        else
        {
            /* get the start address */
            uint8_t * start = area->table.pages4K.start;

            for(j = 0U; j < area->table.pages4K.numberPages; j++ , start += VALUE_4KiB)
            {
                /* initialize the level 0 table descriptor with I/O permissions, adding a table level 1 and level 2 if needed */
                table2DescriptorInitIo(table0 , &table2MIterator , &table4KIterator , start);
            }
        }
    }
}


void table1DescriptorInitIo(ptrPharosCpuMmuLevelTable table0 , ptrPharosCpuMmuLevelTable *table1Iterator , uint8_t *address)
{
    ptrPharosCpuMmuLevelTable tableLevel1;

    uint16_t index0 = calculateIndex1G(address);
    uint16_t index1 = calculateIndex2M(address , index0);

    /* check if descriptor 0 is for a next table */
    if(descriptorIsNextTable(table0->descriptor[index0]) == FALSE)
    {
        /* it is invalid, so we must initialize a new table level 1 */

        /* add the new level 1 table with all entries invalid */
        table1LevelAddInvalid(table0 , *table1Iterator , index0);

        /* set the level 1 table */
        tableLevel1 = *table1Iterator;

        /* increment the table iterator */
        (*table1Iterator)++;
    }
    else
    {
        /* get the current table level 1 */
        tableLevel1 = getTable(table0->descriptor[index0]);
    }

    /* if the descriptor of level 1 is invalid (means that it was not used before) */
    if(tableLevel1 != NULL)
    {
        /* then set the block entry to give I/O permissions to it */
        tableDescriptorInitBlock2Io(&tableLevel1->descriptor[index1] , address);
    }
    else
    {
        /* announce error */
        pharosErrorAnnounce(PHAROS_ERR_I0_MEM_OVERLAPPING , address);
    }
}


void table2DescriptorInitIo(ptrPharosCpuMmuLevelTable table0 , ptrPharosCpuMmuLevelTable *table1Iterator , ptrPharosCpuMmuLevelTable *table2Iterator ,
                            uint8_t *address)
{
    ptrPharosCpuMmuLevelTable tableLevel1;
    ptrPharosCpuMmuLevelTable tableLevel2;

    uint16_t index0 = calculateIndex1G(address);
    uint16_t index1 = calculateIndex2M(address , index0);
    uint16_t index2 = calculateIndex4K(address , index0 , index1);

    /* check if descriptor 0 is for a next table */
    if(descriptorIsNextTable(table0->descriptor[index0]) == FALSE)
    {
        /* it is not a next level table, so we must initialize a new table level 1 */

        /* add the new level 1 table with all entries invalid */
        table1LevelAddInvalid(table0 , *table1Iterator , index0);

        /* set the level 1 table */
        tableLevel1 = *table1Iterator;

        /* increment the table iterator */
        table1Iterator++;
    }
    else
    {
        /* get the current table level 1 */
        tableLevel1 = getTable(table0->descriptor[index0]);
    }

    /* check if the level 1 table entry is invalid */
    if(descriptorIsNextTable(tableLevel1->descriptor[index1]) == FALSE)
    {
        /* it is not a next level table, so we must initialize a new table level 2 */

        /* add the new level 2 table with all entries invalid */
        table2LevelAdd(tableLevel1 , *table2Iterator , index1 , address);

        /* set the level 1 table */
        tableLevel2 = *table2Iterator;

        /* increment the table iterator */
        table2Iterator++;
    }
    else
    {
        /* get the current table level 2 */
        tableLevel2 = getTable(tableLevel1->descriptor[index1]);
    }

    /* if the descriptor of level 2 is invalid (means that it was not used before) */
    if(tableLevel2 != NULL)
    {
        /* then set the block entry to give I/O permissions to it */
        tableDescriptorInitBlock2Io(&tableLevel2->descriptor[index2] , address);
    }
    else
    {
        /* announce error */
        pharosErrorAnnounce(PHAROS_ERR_I0_MEM_OVERLAPPING , address);
    }
}


void tableDescriptorInitBlock2Io(ptrPharosCpuMmuDescriptor descriptor , uint8_t *address)
{
    /* mark as valid */
    descriptor->v = 1;

    /* user can read any address */
    descriptor->r = 1;

    /* user can write any address */
    descriptor->w = 1;

    /* IO memory region is not supposed to be executed */
    descriptor->x = 0;

    /* reserved bits */
    descriptor->rsw = 0;

    /* mark it as accessed so a trap is not triggered when accessing it for the first time */
    descriptor->a = 1;

    /* mark it as dirty so a trap is not triggered when writing it for the first time */
    descriptor->d = 1;

    /* add permissions to the user to access it */
    descriptor->u = 1;

    /* this descriptor is not global */
    descriptor->g = 0;

    /* initialize it with real-addressing */
    descriptor->ppn = ((uintptr_t) address) >> 12;
}


void initSharedStack(ptrPharosCpuMmuLevelTable tableLevel0 , ptrPharosCpuMmuLevelTable *table2Iterator)
{
    /* iterator of the 2MiB tables */
    uint32_t i;

    /* start of the shared stack */
    uint8_t * stackStart = (uint8_t *) & pharosSharedStackStart;

    /* number of tables (level2) per partition */
    uint32_t numberTables2PerPartition = (uint32_t) (uintptr_t) & pharosNumberSharedStackMmuTables2PerPartition;

    /* index on the 1 GiB table */
    uint16_t index0 = calculateIndex1G(stackStart);

    /* index on the 2 MiB table */
    uint16_t index1 = calculateIndex2M(stackStart , index0);

    /* get the current table level 1 */
    ptrPharosCpuMmuLevelTable tableLevel1 = getTable(tableLevel0->descriptor[index0]);


    /* initialize all tables for level 2 for the shared stack area */
    for(i = 0U; i < numberTables2PerPartition; i++ , stackStart += VALUE_2MiB)
    {
        /* add new table 2 */
        table2LevelAdd(tableLevel1 , *table2Iterator , index1 , stackStart);

        /* increment the table iterator */
        table2Iterator++;
    }
}


void pharosCpuMmuInitialize()
{
    /* get the core we are running on */
    CoreNumber core = pharosICoreRunning();

    /* iterator */
    uint32_t i;

    ptrPharosCpuMmuLevelTable iterator1;

    /* number of shared stack MMU tables per partition */
    const uint32_t numberSharedStackMmuTables = (uint32_t) (uintptr_t) & pharosNumberSharedStackMmuTables2PerPartition;

    /* partition table on the running core */
    ptrPartitionTable partitions = pharosICorePartitionTable();


    /* go through each of the partition on the running core */
    for(i = 0U; i < partitions->size; i++)
    {
        /* get the partition */
        ptrPartition p = &partitions->partitions[i];

        /* get the partition number */
        PartitionNumber number = p->id;

        /* get the shared stack table for the partition */
        ptrPharosCpuMmuLevelTable iterator2SharedStack = &pharosSharedStackTables2Start[number * numberSharedStackMmuTables];


        /* get the level 0 table for the partition */
        ptrPharosCpuMmuLevelTable table0 = &pharosPartitionLevel0Mmu[number];

        /* set the level 0 table */
        p->context.satp = ((uintptr_t) & pharosPartitionLevel0Mmu[number] >> 12) | PHAROS_CPU_SATP_MODE;

        /* set the start of the level 1 array of tables */
        iterator1 = partitionLevel1Tables[number];

        /* initialize the level 0 table */
        initializeLevel0Table(table0 , &iterator1 , &p->memProtection);

        /* initialize the address */
        initSharedStack(table0 , &iterator2SharedStack);

        /* initialize the IO MMU areas */
        initializeIoMemArea(table0 , &p->ioTable);
    }

    /* initialize the kernel tables (for the idle thread) */
    initializeLevel0KernelTable(&kernelTable0Table[core]);
}


void pharosCpuPartitionMmuInit(ptrPartitionCpuMmuContext context , ptrEnvMemoryProtection memProtection)
{
    /* do nothing */
}


void initializeChannelBulkTable(ptrPharosCpuMmuLevelTable table)
{
    /* descriptor iterator */
    uint16_t i;


    /* go through all the entries */
    for(i = 0; i < PHAROS_CPU_MMU_TABLE_NUMBER_ENTRIES; i++)
    {
        /* mark as valid */
        table->descriptor[i].v = 1;

        /* mark it as can read */
        table->descriptor[i].r = 1;

        /* mark it as can write */
        table->descriptor[i].w = 1;

        /* mark it as cannot execute */
        table->descriptor[i].x = 0;

        /* mark it as user can't access it (later can be changed) */
        table->descriptor[i].u = 0;

        /* non-global */
        table->descriptor[i].g = 0;

        /* reserved bits */
        table->descriptor[i].rsw = 0;

        /* mark it as accessed so a trap is not triggered when accessing it for the first time */
        table->descriptor[i].a = 1;

        /* mark it as dirty so a trap is not triggered when writing it for the first time */
        table->descriptor[i].d = 1;
    }
}


uint8_t * pharosCpuChannelBulkQueueInit(ptrChannelBulkQueue queue , uint32_t * bulkSize)
{
    /* iterator on the tables */
    uint32_t i;

    /* core iterator */
    CoreNumber core;


    /* for every core */
    for(core = 0; core < PHAROS_NUMBER_CORES; core++)
    {
        /* for each level 1 table */
        for(i = 0; i < queue->cpuInfo.numberLevel1Tables[core]; i++)
        {
            /* get the l1 table for this core */
            ptrPharosCpuMmuLevelTable l1 = queue->cpuInfo.level1Pages[core];

            /* initialize the table with memory configuration but do not initialize the address */
            initializeChannelBulkTable(&l1[i]);
        }

        /* for each level 2 table */
        for(i = 0; i < queue->cpuInfo.numberLevel2Tables[core]; i++)
        {
            /* get the l2 table for this core */
            ptrPharosCpuMmuLevelTable l2 = queue->cpuInfo.level2Pages[core];

            /* initialize the table with memory configuration but do not initialize the address */
            initializeChannelBulkTable(&l2[i]);
        }
    }

    /* initialize the size */
    *bulkSize = queue->cpuInfo.type;

    /* return the address of the first page */
    return (uint8_t *) queue->cpuInfo.pages.page4K;
}


uint32_t pharosCpuChannelBulkAdd(ptrChannelBulkQueue queue , ptrChannelBulkKernel bulk , ptrPartition partition)
{
    /* result of the function */
    uint32_t result;

    /* get the running thread MMU level 0 table */
    ptrPharosCpuMmuLevelTable table0 = satpGetTable(partition->context.satp);

    /* get the MMU level 1 table */
    ptrPharosCpuMmuLevelTable table1;

    /* index of the level 0 table */
    uint16_t index0 = calculateIndex1G(bulk->bulkFullBuffer.buffer);


    /* check if for a 1G bulk */
    if(queue->cpuInfo.type == PHAROS_CHANNEL_BULK_QUEUE_1G)
    {
        /* add the 1 GiB bulk */
        result = add1GBulk(table0 , queue , bulk->bulkFullBuffer.buffer , partition);
    }
    else
    {
        /* get the level 1 table */
        table1 = getTable(table0->descriptor[index0]);

        /* for a channel bulk with 4K size */
        if(queue->cpuInfo.type == PHAROS_CHANNEL_BULK_QUEUE_4K)
        {
            /* add the 4K bulk */
            result = add4KBulk(table1 , queue , bulk->bulkFullBuffer.buffer , partition);
        }
            /* else, it is PHAROS_CHANNEL_BULK_QUEUE_2M -> 2 MiB */
        else
        {
            /* add the 2M bulk */
            result = add2MBulk(table1 , queue , bulk->bulkFullBuffer.buffer , partition);
        }
    }

    /* return the page size added or 0 if impossible to add */
    return result;
}


void pharosCpuChannelBulkRemove(ptrChannelBulkKernel bulk , ptrPartition partition)
{
    /* get the MMU level 1 table */
    ptrPharosCpuMmuLevelTable table1;

    /* get the running thread MMU level 0 table */
    ptrPharosCpuMmuLevelTable table0 = satpGetTable(partition->context.satp);

    /* index of the level 0 table */
    uint16_t index0 = calculateIndex1G(bulk->bufferMessage.buffer);


    /* check if for a 1G bulk */
    if(bulk->queue->cpuInfo.type == PHAROS_CHANNEL_BULK_QUEUE_1G)
    {
        /* add the 1 GiB bulk */
        remove1GBulk(table0 , bulk->queue , bulk->bufferMessage.buffer);
    }
    else
    {
        /* get the level 1 table */
        table1 = getTable(table0->descriptor[index0]);

        /* for a channel bulk with 4K size */
        if(bulk->queue->cpuInfo.type == PHAROS_CHANNEL_BULK_QUEUE_4K)
        {
            /* remove the 4K bulk */
            remove4KBulk(table1 , bulk->queue , bulk->bufferMessage.buffer);
        }
            /* else, it is PHAROS_CHANNEL_BULK_QUEUE_2M -> 2 MiB */
        else
        {
            /* remove the 2M bulk */
            remove2MBulk(table1 , bulk->queue , bulk->bufferMessage.buffer);
        }
    }
}


ptrPharosCpuMmuDescriptor getTableLevel2Descriptor(ptrPharosCpuMmuLevelTable tableLevel0 , uint8_t * address)
{
    /* calculate the index of the address for the top level (0) table */
    uint16_t index0 = calculateIndex1G(address);

    /* calculate the index of the address for the level 1 table */
    uint16_t index1 = calculateIndex2M(address , index0);

    /* calculate the index of the address for the level 2 table */
    uint16_t index2 = calculateIndex4K(address , index0 , index1);

    /* get the level 1 table */
    ptrPharosCpuMmuLevelTable tableLevel1 = getTable(tableLevel0->descriptor[index0]);

    /* get the level 2 table */
    ptrPharosCpuMmuLevelTable tableLevel2 = getTable(tableLevel1->descriptor[index1]);


    /* return the descriptor for the 2 level table for the specified address */
    return &tableLevel2->descriptor[index2];
}


void sharedStackChangePermissions(ptrStackShared sharedStack , ptrPartitionCpuMmuContext context , bool uValue)
{
    /* iterator for the 4KiB sections of the shared stack */
    uint32_t i;

    /* address to change the permissions */
    uint8_t *address = sharedStack->stack.stack;

    /* descriptor to change the permissions */
    ptrPharosCpuMmuDescriptor descriptor;

    /* get the context level 0 table */
    ptrPharosCpuMmuLevelTable tableLevel0 = satpGetTable(context->satp);

    /* calculate number of sections of 4 KiB are used by the shared stack */
    uint32_t numberDescriptors = (uint32_t) (sharedStack->stack.size / VALUE_4KiB);


    /* add all needed descriptors */
    for(i = 0U; i < numberDescriptors; i++)
    {
        /* get the 4K table for the address iterator */
        descriptor = getTableLevel2Descriptor(tableLevel0 , address);

        /* add permissions for the 4K descriptor */
        descriptor->u = uValue;

        /* invalidate the TLB for the specified address */
        pharosCpuMmuInvalidateAddress(address);

        /* update the address to the next descriptor */
        address += VALUE_4KiB;
    }
}


void pharosCpuAddSharedStack(ptrThreadInfo thread , ptrPartitionCpuMmuContext context)
{
    sharedStackChangePermissions(thread->sharedAccessStack , context , TRUE);
}


void pharosCpuRemoveSharedStack(ptrThreadInfo thread , ptrPartitionCpuMmuContext context)
{
    sharedStackChangePermissions(thread->sharedAccessStack , context , FALSE);
}


uint64_t pharosCpuMmuGetSatp(ptrPartition partition)
{
    /* if no partition is set */
    if(partition == NULL)
    {
        /* then return the kernel TTRB */
        return ((uintptr_t) & kernelTable0Table[pharosICoreRunning()] >> 12) | PHAROS_CPU_SATP_MODE;
    }
        /* if a partition is set */
    else
    {
        /* return the partition tlb */
        return partition->context.satp;
    }
}


void pharosCpuPartitionInitialize(ptrPartition partition)
{
    /* nothing to do */
}


bool checkIntervalsIntersect(const uint8_t *a0 , const uint8_t *a1 , const uint8_t *b0 , const uint8_t * b1)
{
    /* check if intervals intersect */
    if(b0 > a1 || a0 > b1 || b0 == b1)
    {
        /* they don't */
        return FALSE;
    }
    else
    {
        /* they do */
        return TRUE;
    }
}


bool hasAnyPermission(uint8_t *address , uint64_t size , ptrEnvMemoryProtection protection)
{
    bool result;

    /* calculate end address */
    uint8_t *end = address + size - 1;


    /* if inside the text zone */
    if(checkIntervalsIntersect(address , end , &pharosStartText , &pharosEndText) == TRUE)
    {
        /* has some kind of permissions */
        result = TRUE;
    }
        /* if inside the environment memory zone */
    else if(checkIntervalsIntersect(address , end , protection->startAddress[0] , protection->endAddress[0]) == TRUE)
    {
        /* has some kind of permissions */
        result = TRUE;
    }
        /* if inside the shared memory zone */
    else if(checkIntervalsIntersect(address , end , &pharosSharedStart , &pharosSharedEnd) == TRUE)
    {
        /* has some kind of permissions */
        result = TRUE;
    }
        /* if inside the inter-partition calls zone */
    else if(checkIntervalsIntersect(address , end , &pharosIpCallStart , &pharosIpCallEnd) == TRUE)
    {
        /* has some kind of permissions */
        result = TRUE;
    }
    else
    {
        /* does not have any permissions */
        result = FALSE;
    }

    return result;
}


void changeDescriptorPermissionsAddress(ptrPharosCpuMmuDescriptor descriptor , uint32_t uValue , uint8_t * address)
{
    /* mark it with the user access value */
    descriptor->u = uValue;

    /* fill the address (AKA real-addressing) */
    descriptor->ppn = ((uintptr_t) address) >> 12;
}


uint32_t add1GBulk(ptrPharosCpuMmuLevelTable table0 , ptrChannelBulkQueue queue , uint8_t * address , ptrPartition partition)
{
    /* calculate the index on the level 0 table */
    uint16_t index0 = calculateIndex1G(address);

    /* give the descriptor access to the memory area */
    table0->descriptor[index0].u = 1;

    /* return the number of bytes added */
    return VALUE_1GiB;
}


uint32_t add2MBulk(ptrPharosCpuMmuLevelTable table1 , ptrChannelBulkQueue queue , uint8_t * address , ptrPartition partition)
{
    /* calculate the index on the level 0 table */
    uint16_t index0 = calculateIndex1G(address);

    /* calculate the index on the level 1 table */
    uint16_t index1 = calculateIndex2M(address , index0);

    /* give the descriptor access to the memory area */
    table1->descriptor[index1].u = 1;

    /* return the 2 MiB added */
    return VALUE_2MiB;
}


uint32_t add4KBulk(ptrPharosCpuMmuLevelTable table1 , ptrChannelBulkQueue queue , uint8_t * address , ptrPartition partition)
{
    /* interrupt level */
    PharosInterruptLevel level;

    /* level 2 table */
    ptrPharosCpuMmuLevelTable table2;

    /* default return value (unless an error occurs) */
    uint32_t result = VALUE_4KiB;

    /* calculate the index on the level 0 table */
    uint16_t index0 = calculateIndex1G(address);

    /* calculate the index on the level 1 table */
    uint16_t index1 = calculateIndex2M(address , index0);

    /* calculate the index on the level 2 table */
    uint16_t index2 = calculateIndex4K(address , index0 , index1);

    /* get the partition core */
    CoreNumber core = partition->core->number;


    /* get the current MMU table level 2 */
    table2 = getTable(table1->descriptor[index1]);

    /* check that the table exists */
    if(table2 == NULL)
    {
        /* disable interrupts we can safely allocate a level 2 table */
        pharosCpuDisableInterrupts(&level);

        /* if there are enough tables to allocate */
        if(queue->cpuInfo.numberLevel2Tables[core] > 0U)
        {
            /* add the level 2 table to the level 1 table */
            table2 = linkLevel2Table(table1 , index1 , queue->cpuInfo.level2Pages[core] , address);

            queue->cpuInfo.level2Pages[core]++;

            /* decrease the number of available level 3 tables */
            queue->cpuInfo.numberLevel2Tables[core]--;

            /* re-enable interrupts*/
            pharosCpuEnableInterrupts(&level);

            /* give permissions to the descriptor */
            changeDescriptorPermissionsAddress(&table2->descriptor[index2] , TRUE , address);
        }
        else
        {
            /* re-enable interrupts*/
            pharosCpuEnableInterrupts(&level);

            /* return error */
            result = 0;
        }
    }
    else
    {
        /* give permissions to the descriptor */
        changeDescriptorPermissionsAddress(&table2->descriptor[index2] , TRUE , address);
    }

    /* return 0 if could not add the tables or 2MiB if successful */
    return result;
}


uint64_t table2Ppn(ptrPharosCpuMmuLevelTable table)
{
    return ((uintptr_t) table) >> 12;
}


ptrPharosCpuMmuLevelTable linkLevel2Table(ptrPharosCpuMmuLevelTable table1 , uint16_t index1 , ptrPharosCpuMmuLevelTable table2 , uint8_t * address)
{
    uint16_t i;

    /* copy the address on the previous table */
    table1->descriptor[index1].ppn = table2Ppn(table2);

    /* initialize the table 1 property to point to a table 2 */
    table1->descriptor[index1].r = 0;
    table1->descriptor[index1].w = 0;
    table1->descriptor[index1].x = 0;

    /* go through all entries on table 2  */
    for(i = 0; i < PHAROS_CPU_MMU_TABLE_NUMBER_ENTRIES; i++)
    {
        /* mark as valid */
        table2->descriptor[i].v = 1;

        /* mark it as can read */
        table2->descriptor[i].r = 1;

        /* mark it as can write */
        table2->descriptor[i].w = 1;

        /* mark it as cannot execute */
        table2->descriptor[i].x = 0;

        /* mark it as user can't access it (later can be changed) */
        table2->descriptor[i].u = 0;

        /* non-global */
        table2->descriptor[i].g = 0;

        /* reserved bits */
        table2->descriptor[i].rsw = 0;

        /* mark it as accessed so a trap is not triggered when accessing it for the first time */
        table2->descriptor[i].a = 1;

        /* mark it as dirty so a trap is not triggered when writing it for the first time */
        table2->descriptor[i].d = 1;
    }

    return table2;
}


void remove4KBulk(ptrPharosCpuMmuLevelTable table1 , ptrChannelBulkQueue queue , uint8_t * address)
{
    /* calculate the index on the level 0 table */
    uint16_t index0 = calculateIndex1G(address);

    /* calculate the index on the level 1 table */
    uint16_t index1 = calculateIndex2M(address , index0);

    /* calculate the index on the level 2 table */
    uint16_t index2 = calculateIndex4K(address , index0 , index1);

    /* get the current MMU table level 2 */
    ptrPharosCpuMmuLevelTable table2 = getTable(table1->descriptor[index1]);


    /* remove permissions in the descriptor */
    table2->descriptor[index2].u = 0;

    /* make sure the changes are reflected from now on */
    pharosCpuMmuInvalidateAddress(address);
}


void remove1GBulk(ptrPharosCpuMmuLevelTable table0 , ptrChannelBulkQueue queue , uint8_t * address)
{
    /* calculate the index on the level 0 table */
    uint16_t index0 = calculateIndex1G(address);


    /* remove permissions in the descriptor */
    table0->descriptor[index0].u = 0;

    /* make sure the changes are reflected from now on */
    pharosCpuMmuInvalidateAddress(address);
}


void remove2MBulk(ptrPharosCpuMmuLevelTable table1 , ptrChannelBulkQueue queue , uint8_t * address)
{
    /* calculate the index on the level 0 table */
    uint16_t index0 = calculateIndex1G(address);

    /* calculate the index on the level 1 table */
    uint16_t index1 = calculateIndex2M(address , index0);


    /* remove permissions to the table*/
    table1->descriptor[index1].u = 0;

    /* make sure the changes are reflected from now on */
    pharosCpuMmuInvalidateAddress(address);
}


void table1LevelAddInvalid(ptrPharosCpuMmuLevelTable table0 , ptrPharosCpuMmuLevelTable table1Iterator , uint16_t indexLevel0)
{
    uint32_t i;


    /* copy the address on the previous table */
    table0->descriptor[indexLevel0].ppn = (uintptr_t) (table1Iterator) >> 12;

    /* initialize the table 1 property to point to a table 2 */
    table0->descriptor[indexLevel0].r = 0;
    table0->descriptor[indexLevel0].w = 0;
    table0->descriptor[indexLevel0].x = 0;

    /* user can access it (bit ignored since it is a link to the next level page table */
    table0->descriptor[indexLevel0].u = 1;

    /* bit ignored */
    table0->descriptor[indexLevel0].a = 1;

    /* bit ignored */
    table0->descriptor[indexLevel0].d = 1;

    /* not global */
    table0->descriptor[indexLevel0].g = 0;

    /* reserved */
    table0->descriptor[indexLevel0].rsw = 0;

    /* mark it as valid */
    table0->descriptor[indexLevel0].v = 1;

    /* go to every level 1 descriptor and initialize it */
    for(i = 0U; i < PHAROS_CPU_MMU_TABLE_NUMBER_ENTRIES; i++)
    {
        /* initialize with invalid */
        //table1Iterator->descriptor[i].v = 0;
    }
}
