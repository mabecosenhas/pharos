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


#include <pharos/channel.h>
#include <pharos/kernel/partition/declarations.h>
#include <pharos/kernel/partition/partition.h>
#include <pharos/kernel/partition/partition.h>
#include <pharos/kernel/channel/channelbulkkernel.h>
#include <pharos/kernel/interrupt/interrupt.h>
#include <pharos/kernel/object/protection.h>
#include <pharos/kernel/assert/assert.h>
#include <pharos/hal/boardApi.h>
#include <pharos/hal/memory/mmu.h>
#include <pharos/hal/cpu/sections.h>


extern uint8_t interruptStackBssEnd;
extern uint8_t interruptStackBssStart;

extern uint8_t pharosPageStart1M;
extern uint8_t pharosPageEnd1M;
extern uint8_t pharosPageStart4K;
extern uint8_t pharosPageEnd4K;


extern uint8_t pharosSharedStackTablesEnd;

extern PageTable4K pharosSharedStackTablesStart;
extern uint8_t pharosNumberSharedStackMmuTablesPerPartition;


#define AP_NO_ACESS         0U
#define AP_READ_ONLY_ACCESS 2U
#define AP_FULL_ACESS       3U

#define CP15_C1_R_NO_ACESS 0U
#define CP15_C1_S_NO_ACESS 0U

#define CP15_C1_R_FULL_ACESS 1U
#define CP15_C1_S_FULL_ACESS 1U

#define PAGE_TABLE_C_BIT_WRITE_THROUGH 1U
#define PAGE_TABLE_B_BIT_WRITE_THROUGH 0U

/**
 * The first domain is client (will check the access permission bits (AP) and R and S (on C1))
 * All remaining domains deny any access but are not used
 */
#define CP15_DOMAIN_ACCESS_CONTROL_CLIENT 0b01


/**
 * Initializes a page table with no permissions on the start address
 * 
 * @param table table to initialize
 * @param startAddress start address of the page table
 */
static void pharosMmuPageTablesInitialize(const ptrMemPageTable table , uint8_t *startAddress , uint32_t numberTables);


/**
 * Adds a memory page table to the context of the MMU
 * 
 * @param context context where to add the page table
 * @param table table to add
 * @param buffer start address of the page to add
 * 
 * @return returns the number of pages added (0 if none where needed)
 */
static uint32_t pharosMmuAddPageTable(ptrPartitionCpuMmuContext context , ptrMemPageTable table , uint32_t index , const uint8_t *buffer);


/**
 * Removes the permissions set by pharosMmuGivePermissions function on the specified MMU context.
 * 
 * @param context context of the MMU
 * @param startAddress start address of the section/large page/small page/tiny page to remove permissions from
 * 
 * @return returns TRUE if the permissions where removed and FALSE otherwise
 */
static void pharosCpuMmuRemovePermissions(ptrPartitionCpuMmuContext context , uint8_t *startAddress);


/**
 * Gives permissions to the defined start address to the table that contains the specified address.
 * 
 * @contraint assumes that the type matches the content of the MMU (that is, if the MMU contains a section descriptor for the specified address then the type should also be PHAROS_PAGE_TABLE_1M).
 * 
 * @param context context of the MMU
 * @param startAddress start address of the section/small page to give permissions to
 * @param pageSize number of pages to add
 * 
 * @return returns TRUE if the permissions where added and FALSE otherwise
 */
static void pharosCpuMmuGivePermissions(ptrPartitionCpuMmuContext context , uint8_t *startAddress);


/**
 * enables the MMU in assembly (initializes the coprocessor C15 registers)
 */
void pharosCpuEnableMmu(void);

/**
 * Each partition, driver and the kernel has its own table of sections which map the entire memory (4 Gib) of virtual memory to its physical addresses.
 * The mapping is such that the virtual address is equal to the physical address (AKA real addressing).
 * Each section table contains 4096 entries and each entry maps 1 MiB of memory (1 MiB corresponds to one section). So each partition, driver and kernel must be aligned to 1 MiB
 */
KERNEL_SECTION_USS MmuSectionTable partitionSectionTables[MAXIMUM_NUMBER_PARTITIONS] PHAROS_ALIGN_16KiB;

/**
 * Kernel section table 
 */
KERNEL_SECTION_USS MmuSectionTable kernelSectionTable PHAROS_ALIGN_16KiB;


KERNEL_SECTION_USS EnvMemoryProtection sharedSectionMemoryProtection;

void pharosCpuInvalidateMmu(void);

void pharosCpuSetTlb(uint32_t tlb);

uint32_t pharosCpuGetTlb(void);


static uint32_t add4KPage(ptrPartitionCpuMmuContext mmuContext , ptrChannelBulkQueue queue , uint8_t * address);

/**
 * determines if the specified section descriptor already points to a 4KiB page table
 * 
 * @param descritor section descriptor to check
 * 
 * @return returns TRUE if the section descriptor points to a 4KiB page table and FALSE otherwise
 */
static bool sectionDescriptorHas4KPage(MmuSectionDescriptor descritor);


/**
 * Sets up the section descriptor to point to the page table
 * 
 * @param descriptor section descriptor
 * @param table table to point to
 */
static void addSectionDescritorAdd4KPage(const ptrMmuSectionDescriptor descriptor , ptrPageTable4K table);


/**
 * Get the 4K table specified on the section descriptor (assumes the section descriptor is pointing to a valid 4K page table
 * 
 * @param descriptor section descriptor
 * @return 
 */
static ptrPageTable4K sectionDescriptorGet4KTable(const ptrMmuSectionDescriptor descriptor);


static void initializeIoMemArea(ptrMmuSectionTable sectionTable , ptrIoMemAreaTable table);

/**
 * Change the permissions of a 4KiB section on a 4KiB page table.
 * 
 * @param pageTable table to change 
 * @param section section of the startAddress
 * @param startAddress startAddress
 * @param apValue value to place on the AP fields
 */
static void change4KSectionPermissions(const ptrPageTable4K pageTable , uint32_t section , uint8_t *startAddress , uint8_t apValue);


/**
 * Initializes all the descriptors of the table but does initialize the base address since it is unknown
 * 
 * @param table table to initialize 
 */
static void initializeNoAccess4KTable(const ptrPageTable4K table);


static void sharedStackChangePermissions(ptrStackShared sharedStack , ptrPartitionCpuMmuContext context , uint8_t apValue);


static void initializeRealAddressing4KTable(ptrPageTable4K table , uint32_t section);

/**
 * Initializes a section table
 * 
 * @param table table to initialize
 */
static void initializeSectionTable(const ptrMmuSectionTable table , const ptrEnvMemoryProtection protection);


static void mmuInitializePage4K(ptrPageTable4K table , uint32_t section);

static void changePermissions(ptrPartitionCpuMmuContext context , uint8_t *startAddress , uint8_t apValue);


/**
 * Gets the MMU page size for the specified address. The address must be inside the channel bulk area otherwise the
 * method will return 0
 * 
 * @param address address to determine the page size
 * 
 * @return returns the page size of the specified address
 */
static uint32_t pharosCpuMmuPageSize(const uint8_t *address);


INLINE uint32_t sectionNumber(const uint8_t *address)
{
    return ((uint32_t) address) >> 20;
}


void pharosCpuPartitionInitialize(ptrPartition partition)
{
    /* do nothing */
}


void pharosCpuPartitionMmuInit(ptrPartitionCpuMmuContext context , ptrEnvMemoryProtection memProtection)
{
    /* do nothing */
}


void sharedStackChangePermissions(ptrStackShared sharedStack , ptrPartitionCpuMmuContext context , uint8_t apValue)
{
    uint32_t i;

    uint8_t *address = sharedStack->stack.stack;

    ptrPageTable4K table;

    ptrMmuSectionTable sectionTable = (ptrMmuSectionTable) context->tlb;

    uint32_t numberDescriptors = (uint32_t) (sharedStack->stack.size / VALUE_4KiB);

    /* add all needed descriptors */
    for(i = 0U; i < numberDescriptors; i++)
    {
        /* determine the section where the shared stack iterator is */
        uint32_t section = sectionNumber(address);

        /* get the 4K table for the address iterator */
        table = sectionDescriptorGet4KTable(&sectionTable->sectionDescriptor[section]);

        /* add permissions for the 4K descriptor */
        change4KSectionPermissions(table , section , address , apValue);

        /* update the address to the next descriptor */
        address += VALUE_4KiB;
    }

    /* invalidate the MMU */
    pharosCpuInvalidateMmu();
}


void pharosCpuAddSharedStack(ptrThreadInfo thread , ptrPartitionCpuMmuContext context)
{
    sharedStackChangePermissions(thread->sharedAccessStack , context , AP_FULL_ACESS);
}


void pharosCpuRemoveSharedStack(ptrThreadInfo thread , ptrPartitionCpuMmuContext context)
{
    sharedStackChangePermissions(thread->sharedAccessStack , context , AP_NO_ACESS);
}


ptrPageTable4K get4KTable(ptrMmuSectionTable sectionTable , uint32_t sectionNumber)
{
    /* get the section table */
    return (ptrPageTable4K) (sectionTable->sectionDescriptor[sectionNumber].coarse.coarsePageTableBaseAddress << 10);
}


void changePermissions(ptrPartitionCpuMmuContext context , uint8_t *startAddress , uint8_t apValue)
{
    /* interrupt level */
    PharosInterruptLevel interruptLevel;

    /* get the section number (1 MiB) */
    uint32_t section = sectionNumber(startAddress);

    ptrMmuSectionTable sectionTable = (ptrMmuSectionTable) context->tlb;

    /* get the page size based on the section that is on the address */
    uint32_t pageSize = pharosCpuMmuPageSize(startAddress);


    /* if the descriptor is for a section */
    if(pageSize == VALUE_1MiB)
    {
        /* don't allow anyone inside threads or interrupts to change the MMU descriptor */
        pharosCpuDisableInterrupts(&interruptLevel);

        /* add the full permission */
        sectionTable->sectionDescriptor[section].section.ap = apValue;

        /* interrupts can now be enabled */
        pharosCpuEnableInterrupts(&interruptLevel);
    }
        /* must have page size of 4K */
    else if(pageSize != 0U)
    {
        /* change the permissions to the 4K page table */
        change4KSectionPermissions(get4KTable(sectionTable , section) , section , startAddress , apValue);
    }

    /* invalidate the MMU */
    pharosCpuInvalidateMmu();
}


void pharosCpuMmuGivePermissions(ptrPartitionCpuMmuContext context , uint8_t *startAddress)
{
    /* change the permissions to full access */
    changePermissions(context , startAddress , AP_FULL_ACESS);
}


void pharosCpuMmuRemovePermissions(ptrPartitionCpuMmuContext context , uint8_t *startAddress)
{
    /* change the permissions to remove access */
    changePermissions(context , startAddress , AP_NO_ACESS);
}


void initializeRealAddressing4KTable(ptrPageTable4K table , uint32_t section)
{
    uint32_t j;

    /* calculate the base address ((section << 20) >> 12) of the start of the section for real addressing (no virtual addressing) */
    uint32_t baseAddress = section << 8;

    /* initialize the table 4K */
    for(j = 0U; j < 256U; j++)
    {
        /* initialize the bits that should be 1 */
        table->descriptor[j].shouldBeOne = 1U;

        /* initialize the bits that should be 0 */
        table->descriptor[j].shouldBeZero = 0U;

        /* write through */
        table->descriptor[j].b = PAGE_TABLE_B_BIT_WRITE_THROUGH;

        /* write through */
        table->descriptor[j].c = PAGE_TABLE_C_BIT_WRITE_THROUGH;

        /* AP for the bottom 1K */
        table->descriptor[j].ap0 = AP_NO_ACESS;

        /* AP for the second 1K */
        table->descriptor[j].ap1 = AP_NO_ACESS;

        /* AP for the third 1K */
        table->descriptor[j].ap2 = AP_NO_ACESS;

        /* AP for the top 1K */
        table->descriptor[j].ap3 = AP_NO_ACESS;

        /* initialize the base address */
        table->descriptor[j].baseAddress = baseAddress + j;
    }
}


void initializeNoAccess4KTable(const ptrPageTable4K table)
{
    uint32_t j;


    /* initialize the table 4K */
    for(j = 0U; j < 256U; j++)
    {
        /* initialize the bits that should be 1 */
        table->descriptor[j].shouldBeOne = 1U;

        /* initialize the bits that should be 0 */
        table->descriptor[j].shouldBeZero = 0U;

        /* write through */
        table->descriptor[j].b = PAGE_TABLE_B_BIT_WRITE_THROUGH;

        /* write through */
        table->descriptor[j].c = PAGE_TABLE_C_BIT_WRITE_THROUGH;

        /* AP for the bottom 1K */
        table->descriptor[j].ap0 = AP_NO_ACESS;

        /* AP for the second 1K */
        table->descriptor[j].ap1 = AP_NO_ACESS;

        /* AP for the third 1K */
        table->descriptor[j].ap2 = AP_NO_ACESS;

        /* AP for the top 1K */
        table->descriptor[j].ap3 = AP_NO_ACESS;

        /* the base address will be initialized with the appropriate section addresses when the table is actually used */
        /* cannot initialized now because we don't know to which section it is going to be used */
    }
}


void pharosMmuPageTablesInitialize(const ptrMemPageTable table , uint8_t *startAddress , uint32_t numberTables)
{
    uint32_t i;

    /* get the page size based on the section that is on the address */
    uint32_t pageSize = pharosCpuMmuPageSize(startAddress);


    /* if the descriptor is for a 4K page tables */
    if(pageSize == VALUE_4KiB)
    {
        /* for every MMU page table */
        for(i = 0U; i < numberTables; i++)
        {
            /* initialize the table 4K */
            initializeNoAccess4KTable(&table->table4K[i]);
        }
    }

    /* for 1M there is no need to add page tables since the partition table already does that */
}


void mmuInitializePage4K(ptrPageTable4K table , uint32_t section)
{
    uint16_t j;

    /* calculate the base address ((section << 20) >> 12) of the start of the section for real addressing (no virtual addressing) */
    uint32_t baseAddress = section << 8;


    /* initialize the table 4K */
    for(j = 0U; j < 256U; j++)
    {
        /* initialize the base address */
        table->descriptor[j].baseAddress = baseAddress;

        /* increase the base address */
        baseAddress++;
    }
}


uint32_t pharosCpuMmuPageSize(const uint8_t *address)
{
    uint32_t result;

    /* if the there is a non-empty 1M section and the buffer is inside the 1M zone (see linker script) */
    if((&pharosPageStart1M != &pharosPageEnd1M) && (address >= &pharosPageStart1M) && (address <= &pharosPageEnd1M))
    {
        /* return 1 MiB */
        result = VALUE_1MiB;
    }
        /* if the there is a non-empty 4K section and the buffer is inside the 4K zone (see linker script) */
    else if((&pharosPageStart4K != &pharosPageEnd4K) && (address >= &pharosPageStart4K) && (address <= &pharosPageEnd4K))
    {
        /* return 4 KiB */
        result = VALUE_4KiB;
    }
    else
    {
        /* invalid address (not inside the channel bulk regions) */
        result = 0U;
    }

    /* return the number of bytes per page of the specified address */
    return result;
}

extern ChannelBulkKernel bulksInter[];


uint32_t add4KPage(ptrPartitionCpuMmuContext mmuContext , ptrChannelBulkQueue queue , uint8_t * address)
{
    /* interrupt level */
    PharosInterruptLevel level;

    /* page added (or 0 if cannot) */
    uint32_t result = VALUE_4KiB;

    /* section table (same as the TLB) */
    ptrMmuSectionTable sectionTable = (ptrMmuSectionTable) mmuContext->tlb;

    /* get the section number (1 MiB) */
    uint32_t section = sectionNumber(address);


    /* check if the descriptor is already pointing to a page table (check the first 2 bits that should be 01) */
    if(sectionDescriptorHas4KPage(sectionTable->sectionDescriptor[section]) == TRUE)
    {
        /* disable interrupts we can safely allocate a level 2 table */
        pharosCpuDisableInterrupts(&level);

        /* give permissions on the bulk to the running thread partition */
        change4KSectionPermissions(get4KTable(sectionTable , section) , section , address , AP_FULL_ACESS);

        /* re-enable interrupts*/
        pharosCpuEnableInterrupts(&level);
    }
        /* else, not pointing to a 4KiB table, so we must add it */
    else
    {
        /* disable interrupts we can safely allocate a level 2 table */
        pharosCpuDisableInterrupts(&level);

        /* if there are enough tables to add */
        if(queue->cpuInfo.numberMmuPageTables > 0U)
        {
            ptrPageTable4K table = &queue->cpuInfo.mmuPageTables.table4K[queue->cpuInfo.pageIterator];

            /* initialize the new table of 4 KiB */
            mmuInitializePage4K(table , section);

            /* link the section table to the new 4 KiB table */
            addSectionDescritorAdd4KPage(&sectionTable->sectionDescriptor[section] , table);

            /* increase the page iterator */
            queue->cpuInfo.pageIterator++;

            /* decrease the number of tables available */
            queue->cpuInfo.numberMmuPageTables--;

            /* give permissions on the bulk to the running thread partition */
            change4KSectionPermissions(get4KTable(sectionTable , section) , section , address , AP_FULL_ACESS);

            /* re-enable interrupts*/
            pharosCpuEnableInterrupts(&level);
        }
        else
        {
            /* re-enable interrupts*/
            pharosCpuEnableInterrupts(&level);

            /* could not add */
            result = 0U;
        }
    }

    /* return the number of pages used */
    return result;
}


uint32_t pharosCpuMmuGetTlb(ptrPartition partition)
{
    return (uint32_t) &partitionSectionTables[partition->id];
}


static void initializeSectionTable(const ptrMmuSectionTable table , const ptrEnvMemoryProtection protection)
{
    uint32_t i;


    /* go through every section */
    for(i = 0U; i < NUMBER_SECTIONS_ENTRIES; i++)
    {
        /* calculate section start point */
        uint8_t *address = (uint8_t *) (uint32_t) (i * VALUE_1MiB);

        /* initialize the base address of the section equal to the physical address (we are using real addressing, i.e. no translation) */
        table->sectionDescriptor[i].section.sectionBaseAddress = i;

        /* initialize the bits that should be 1 */
        table->sectionDescriptor[i].section.shouldBeOne1 = 1U;
        table->sectionDescriptor[i].section.shouldBeOne2 = 1U;

        /* initialize the bits that should be 0 */
        table->sectionDescriptor[i].section.shouldBeZero1 = 0U;
        table->sectionDescriptor[i].section.shouldBeZero2 = 0U;
        table->sectionDescriptor[i].section.shouldBeZero3 = 0U;

        /* define the cache to be write through on this section */
        table->sectionDescriptor[i].section.b = PAGE_TABLE_B_BIT_WRITE_THROUGH;

        /* define the cache to be write through on this section */
        table->sectionDescriptor[i].section.c = PAGE_TABLE_C_BIT_WRITE_THROUGH;

        /* use always the same domain (the AP bits define the access) */
        table->sectionDescriptor[i].section.domain = 0U;

        /* must give access to first section that contains the instructions */
        if((&pharosStartText >= address) && (&pharosStartText < address + VALUE_1MiB))
        {
            /* then initialize the section descriptor with read-only access */
            table->sectionDescriptor[i].section.ap = AP_READ_ONLY_ACCESS;
        }
        else if((&pharosEndText >= address) && (&pharosEndText < address + VALUE_1MiB))
        {
            /* then initialize the section descriptor with read-only access */
            table->sectionDescriptor[i].section.ap = AP_READ_ONLY_ACCESS;
        }
            /* if the address belongs to the area of the environment */
        else if((address >= protection->startAddress[0]) && (address < protection->endAddress[0]))
        {
            /* then initialize the section descriptor with full access */
            table->sectionDescriptor[i].section.ap = AP_FULL_ACESS;
        }
            /* if the address belongs to the interrupt stack bss */
        else if((address >= &interruptStackBssStart) && (address < &interruptStackBssEnd))
        {
            /* then initialize the section descriptor with full access */
            table->sectionDescriptor[i].section.ap = AP_FULL_ACESS;
        }
            /* or if it is on the shared section */
        else if((address >= sharedSectionMemoryProtection.startAddress[0]) && (address < sharedSectionMemoryProtection.endAddress[0]))
        {
            /* then initialize the section descriptor with full access */
            table->sectionDescriptor[i].section.ap = AP_FULL_ACESS;
        }
            /* if the address is the IP calls section (read-only) */
        else if((address >= &pharosIpCallStart) && (address < &pharosIpCallEnd))
        {
            /* then initialize the section descriptor with read-only access */
            table->sectionDescriptor[i].section.ap = AP_READ_ONLY_ACCESS;
        }
            /* otherwise, does not have permissions to access it */
        else
        {
            /* then initialize with no access */
            table->sectionDescriptor[i].section.ap = AP_NO_ACESS;
        }
    }
}


ptrPageTable4K initializeIoMemArea4KiB(ptrMmuSectionTable sectionTable , ptrIoMemAreaMmu area , uint8_t *start , uint32_t section , ptrPageTable4K tableIterator)
{
    /* iterator of the page initialization on a 1MiB section */
    uint32_t j;

    /* number of 4 KiB pages missing to initialize */
    uint32_t numberPagesMissing = area->table.pages4K.numberPages;

    /* auxiliary variable with the number of pages to initialize in each iteration (cannot be higher than 256) */
    uint32_t pages2Init;

    /* determine the first page number of the page inside the table */
    uint16_t pageNumber = ((uint32_t) start - (section << 20)) >> 12;

    /* auxiliary variable containing the table of 4KiB pages to initialize */
    ptrPageTable4K table4K;


    /* while there are 4 KiB pages to initialize */
    while(numberPagesMissing > 0U)
    {
        /* check if the descriptor is already pointing to a page table (check the first 2 bits that should be 01) */
        if(sectionDescriptorHas4KPage(sectionTable->sectionDescriptor[section]) == TRUE)
        {
            /* table of 4 KiB is already set for this 1 MiB section, use it */
            table4K = sectionDescriptorGet4KTable(&sectionTable->sectionDescriptor[section]);
        }
        else
        {
            /* make the section table point to the 4K table */
            addSectionDescritorAdd4KPage(&sectionTable->sectionDescriptor[section] , tableIterator);

            /* use the new table */
            table4K = tableIterator;

            /* initialize the table with all addresses using real-addressing (virtual address == physical address) */
            initializeRealAddressing4KTable(table4K , section);

            /* increase the table iterator */
            tableIterator++;
        }


        /* if the number of pages exceeds is too high (is greater than the 1 MiB section) */
        if(numberPagesMissing > 256U - pageNumber)
        {
            /* then initialize only until the end of the 1 MiB section */
            pages2Init = 256 - pageNumber;
        }
            /* if the number of pages is "small" */
        else
        {
            /* then initialize them all */
            pages2Init = numberPagesMissing;
        }

        /* for every page */
        for(j = 0U; j < pages2Init; j++)
        {
            /* AP for the bottom 1K */
            table4K->descriptor[pageNumber].ap0 = AP_FULL_ACESS;

            /* AP for the second 1K */
            table4K->descriptor[pageNumber].ap1 = AP_FULL_ACESS;

            /* AP for the third 1K */
            table4K->descriptor[pageNumber].ap2 = AP_FULL_ACESS;

            /* AP for the top 1K */
            table4K->descriptor[pageNumber].ap3 = AP_FULL_ACESS;

            /* increase the number of the page */
            pageNumber++;
        }

        /* decrease the number of pages missing  */
        numberPagesMissing -= pages2Init;

        /* increase the next section */
        section++;

        /* reset the page for the next 1 MiB section (if needed) */
        pageNumber = 0;
    }


    return tableIterator;
}


void initializeIoMemArea(ptrMmuSectionTable sectionTable , ptrIoMemAreaTable table)
{
    /* iterator of the MMU 4K tables used (if 4KiB tables are selected) */
    ptrPageTable4K tableIterator = &table->mmuTables[0];

    /* iterators of the mmu areas and the 4KiB/1MiB tables */
    uint32_t i , j;

    /* section number (1 MiB) */
    uint32_t section;


    for(i = 0U; i < table->numberMmuAreas; i++)
    {
        if(table->mmuAreas[i].type == PHAROS_PAGE_TABLE_4K)
        {
            /* calculate the section number */
            section = sectionNumber(table->mmuAreas[i].table.pages4K.start);

            /* initialize all the 4 KiB page tables required */
            tableIterator = initializeIoMemArea4KiB(sectionTable , &table->mmuAreas[i] , table->mmuAreas[i].table.pages4K.start , section , tableIterator);
        }
        else
        {
            /* calculate the section number */
            section = sectionNumber(table->mmuAreas[i].table.pages1M.start);

            /* for every section */
            for(j = 0U; j < table->mmuAreas[i].table.pages1M.numberPages; j++)
            {
                /* give permissions to the 1M section */
                sectionTable->sectionDescriptor[section].section.ap = AP_FULL_ACESS;

                /* increase the section number */
                section++;
            }
        }
    }
}


void pharosCpuMmuInitialize(void)
{
    /* iterator */
    uint32_t i , j;
    uint32_t section;
    uint8_t *address;
    ptrPageTable4K iterator = &pharosSharedStackTablesStart;
    uint32_t numberPagesPerPartition = (uint32_t) (((uint32_t) & pharosSharedStackEnd - (uint32_t) & pharosSharedStackStart) / VALUE_1MiB);

    /* global variable used only on this function: Memory protection information regarding the kernel */
    static EnvMemoryProtection kernelSectionMemoryProtection;

    kernelSectionMemoryProtection.startAddress[0] = 0;
    kernelSectionMemoryProtection.endAddress[0] = (uint8_t *) 0xffffffffffffffff;

    sharedSectionMemoryProtection.startAddress[0] = &pharosSharedStart;
    sharedSectionMemoryProtection.endAddress[0] = &pharosSharedEnd;

    /* initialize the kernel section */
    initializeSectionTable(&kernelSectionTable , &kernelSectionMemoryProtection);

    /* go through each of the partition */
    for(i = 0U; i < pharosIPartitionGetNumber(); i++)
    {
        /* get the partition */
        ptrPartition p = pharosIPartitionGet(i);

        /* get the section table to be use for this partition */
        ptrMmuSectionTable sectionTable = &partitionSectionTables[i];

        /* initialize the section table */
        initializeSectionTable(sectionTable , &p->memProtection);

        /* initialize the IO MMU areas */
        initializeIoMemArea(sectionTable , &p->ioTable);

        /* initialize the address */
        address = (uint8_t *) & pharosSharedStackStart;

        /* initialize all tables for the shared stack area */
        for(j = 0U; j < numberPagesPerPartition; j++)
        {
            /* determine the section where the address iterator belongs to */
            section = sectionNumber(address);

            /* initialize the page table with the specified section to provide addressing */
            initializeRealAddressing4KTable(iterator , section);

            /* add the 4k table descriptor to the section */
            addSectionDescritorAdd4KPage(&sectionTable->sectionDescriptor[section] , iterator);

            /* move to the next page table */
            iterator++;

            /* and to the next address */
            address += VALUE_1MiB;
        }

        /* now that the TLB has been all set, initialize the partition TLB */
        p->context.tlb = (uint32_t) sectionTable;
    }

    /* enable the MMU using the co-processor registers */
    pharosCpuEnableMmu();
}


void armCDataAbortHandler(uint8_t * instruction_address , uint8_t * mem_address)
{
    /* announce an invalid memory access and handle it */
    pharosIInvalidMemAccess(instruction_address , mem_address);
}


bool sectionDescriptorHas4KPage(MmuSectionDescriptor descritor)
{
    bool result;

    /* if the descriptor bits are correct */
    if((descritor.coarse.shouldBeOne3 == 1U) && (descritor.coarse.shouldBeZero2 == 0U))
    {
        /* it is, nothing to be done (used no additional table) */
        result = TRUE;
    }
    else
    {
        /* is not a 4KiB page */
        result = FALSE;
    }

    /* return true or false depending on the section descriptor point to a 4KiB table or not */
    return result;
}


ptrPageTable4K sectionDescriptorGet4KTable(const ptrMmuSectionDescriptor descriptor)
{
    return (ptrPageTable4K) (descriptor->coarse.coarsePageTableBaseAddress << 10);
}


void addSectionDescritorAdd4KPage(const ptrMmuSectionDescriptor descriptor , ptrPageTable4K table)
{
    /* initialize the coarse should be zero */
    descriptor->coarse.shouldBeZero = 0U;

    /* initialize the coarse should be zero */
    descriptor->coarse.shouldBeZero2 = 0U;

    /* initialize the coarse should be zero */
    descriptor->coarse.shouldBeZero3 = 0U;

    /* initialize the coarse should be one */
    descriptor->coarse.shouldBeOne1 = 1U;

    /* initialize the coarse should be one */
    descriptor->coarse.shouldBeOne3 = 1U;

    /* use always the same domain (the AP bits define the access) */
    descriptor->coarse.domain = 0U;

    /* initialize the base address of the coarse table */
    descriptor->coarse.coarsePageTableBaseAddress = ((uint32_t) table) >> 10;
}


void change4KSectionPermissions(const ptrPageTable4K pageTable , uint32_t section , uint8_t *startAddress , uint8_t apValue)
{
    /* determine the number of the page inside the table */
    uint16_t pageNumber = ((uint32_t) startAddress - (section << 20)) >> 12;

    /* AP for the bottom 1K */
    pageTable->descriptor[pageNumber].ap0 = apValue;

    /* AP for the second 1K */
    pageTable->descriptor[pageNumber].ap1 = apValue;

    /* AP for the third 1K */
    pageTable->descriptor[pageNumber].ap2 = apValue;

    /* AP for the top 1K */
    pageTable->descriptor[pageNumber].ap3 = apValue;
}


uint32_t pharosCpuChannelBulkAdd(ptrChannelBulkQueue queue , ptrChannelBulkKernel bulk , ptrPartition partition)
{
    /* get the page size based on the section that is on the address */
    uint32_t pageSize = pharosCpuMmuPageSize(bulk->bulkFullBuffer.buffer);

    /* result of the method. Only 4KiB pages are needed to add tables */
    uint32_t result = 0U;


    /* if the descriptor is for a 4KiB page */
    if(pageSize == VALUE_4KiB)
    {
        /* add a 4 KiB page (if enough tables are available) */
        result = add4KPage(&partition->context , queue , bulk->bulkFullBuffer.buffer);
    }
        /* else, is adding a 1 MiB section */
    else
    {
        /* give the permissions for the 1MiB section directly */
        pharosCpuMmuGivePermissions(&partition->context , bulk->bulkFullBuffer.buffer);

        /* for 1 MiB no extra tables needed, so always return success */
        result = VALUE_1MiB;
    }

    return result;
}


void pharosCpuChannelBulkRemove(ptrChannelBulkKernel bulk , ptrPartition partition)
{
    /* remove the permissions for the bulk to the running partition */
    pharosCpuMmuRemovePermissions(&partition->context , bulk->bulkFullBuffer.buffer);
}


uint8_t *pharosCpuChannelBulkQueueInit(ptrChannelBulkQueue queue , uint32_t *bulkSize)
{
    uint8_t *bufferIterator;


    /* get the address of the first page */
    bufferIterator = (uint8_t *) queue->cpuInfo.pages.page4K;

    /* calculate the page size based on its address */
    *bulkSize = pharosCpuMmuPageSize(bufferIterator);

    /* initialize it with no access. Does this at the beginning to avoid doing it later when it is used */
    pharosMmuPageTablesInitialize(&queue->cpuInfo.mmuPageTables , bufferIterator , queue->cpuInfo.numberMmuPageTables);

    /* return the address of the first page */
    return (uint8_t *) queue->cpuInfo.pages.page4K;
}
