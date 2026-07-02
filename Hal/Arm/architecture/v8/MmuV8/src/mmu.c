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


#include <pharos/hal/interrupt.h>
#include <pharos/hal/memory/mmu.h>
#include <pharos/kernel/object/protection.h>
#include <pharos/hal/cpu/asm.h>
#include <pharos/kernel/partition/partition.h>
#include <pharos/channel.h>
#include <pharos/kernel/error/error.h>
#include <pharos/hal/v8a/conf.h>
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/hal/cpu/sections.h>


uint32_t pharosCpuScrtlEl1Get();
void pharosCpuScrtlEl1Set();


extern uint64_t pharosNumberSharedStackMmuTables2PerPartition;
extern uint64_t pharosNumberSharedStackMmuTables3PerPartition;
extern uint64_t pharosSharedStackSize;


extern PharosCpuMmuLevel2Table pharosPartition0MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition1MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition2MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition3MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition4MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition5MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition6MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition7MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition8MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition9MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition10MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition11MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition12MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition13MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition14MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition15MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition16MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition17MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition18MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition19MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition20MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition21MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition22MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition23MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition24MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition25MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition26MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition27MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition28MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition29MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition30MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition31MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition32MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition33MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition34MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition35MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition36MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition37MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition38MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition39MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition40MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition41MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition42MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition43MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition44MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition45MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition46MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition47MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition48MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition49MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition50MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition51MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition52MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition53MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition54MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition55MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition56MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition57MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition58MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition59MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition60MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition61MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition62MmuLevel2Start;
extern PharosCpuMmuLevel2Table pharosPartition63MmuLevel2Start;


extern PharosCpuMmuLevel3Table pharosPartition0MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition1MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition2MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition3MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition4MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition5MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition6MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition7MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition8MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition9MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition10MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition11MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition12MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition13MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition14MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition15MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition16MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition17MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition18MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition19MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition20MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition21MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition22MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition23MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition24MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition25MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition26MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition27MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition28MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition29MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition30MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition31MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition32MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition33MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition34MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition35MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition36MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition37MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition38MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition39MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition40MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition41MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition42MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition43MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition44MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition45MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition46MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition47MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition48MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition49MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition50MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition51MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition52MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition53MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition54MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition55MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition56MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition57MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition58MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition59MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition60MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition61MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition62MmuLevel3Start;
extern PharosCpuMmuLevel3Table pharosPartition63MmuLevel3Start;


/**
 * Table defined in linker script for kernel level 3
 */
extern PharosCpuMmuLevel3Table pharosKernelMmuLevel3Start;

extern PharosCpuMmuLevel2Table pharosSharedStackTables2Start[];
extern PharosCpuMmuLevel3Table pharosSharedStackTables3Start[];

extern uint8_t pharosNumberSharedStackMmuTablesPerPartition;


extern uint8_t PHAROS_CPU_IO_START;
extern uint8_t PHAROS_CPU_IO_END;


KERNEL_SECTION_USS PharosCpuMmuLevel0Table partitionsTable0[PHAROS_PARTITION_MAXIMUM_NUMBER] PHAROS_ALIGN_4KiB;
KERNEL_SECTION_USS PharosCpuMmuLevel1Table partitionsTable1[PHAROS_PARTITION_MAXIMUM_NUMBER] PHAROS_ALIGN_4KiB;

KERNEL_SECTION_USS PharosCpuMmuLevel0Table kernelTable0Table[PHAROS_NUMBER_CORES] PHAROS_ALIGN_4KiB;
KERNEL_SECTION_USS PharosCpuMmuLevel1Table kernelTable1Table[PHAROS_NUMBER_CORES] PHAROS_ALIGN_4KiB;
KERNEL_SECTION_USS PharosCpuMmuLevel2Table kernelTable2Table[PHAROS_NUMBER_CORES] PHAROS_ALIGN_4KiB;
KERNEL_SECTION_USS PharosCpuMmuLevel3Table kernelTable3Table[PHAROS_NUMBER_CORES][512] PHAROS_ALIGN_4KiB;

const ptrPharosCpuMmuLevel2Table partitionLevel2Tables[] = {
    &pharosPartition0MmuLevel2Start ,
    &pharosPartition1MmuLevel2Start ,
    &pharosPartition2MmuLevel2Start ,
    &pharosPartition3MmuLevel2Start ,
    &pharosPartition4MmuLevel2Start ,
    &pharosPartition5MmuLevel2Start ,
    &pharosPartition6MmuLevel2Start ,
    &pharosPartition7MmuLevel2Start ,
    &pharosPartition8MmuLevel2Start ,
    &pharosPartition9MmuLevel2Start ,
    &pharosPartition10MmuLevel2Start ,
    &pharosPartition11MmuLevel2Start ,
    &pharosPartition12MmuLevel2Start ,
    &pharosPartition13MmuLevel2Start ,
    &pharosPartition14MmuLevel2Start ,
    &pharosPartition15MmuLevel2Start ,
    &pharosPartition16MmuLevel2Start ,
    &pharosPartition17MmuLevel2Start ,
    &pharosPartition18MmuLevel2Start ,
    &pharosPartition19MmuLevel2Start ,
    &pharosPartition20MmuLevel2Start ,
    &pharosPartition21MmuLevel2Start ,
    &pharosPartition22MmuLevel2Start ,
    &pharosPartition23MmuLevel2Start ,
    &pharosPartition24MmuLevel2Start ,
    &pharosPartition25MmuLevel2Start ,
    &pharosPartition26MmuLevel2Start ,
    &pharosPartition27MmuLevel2Start ,
    &pharosPartition28MmuLevel2Start ,
    &pharosPartition29MmuLevel2Start ,
    &pharosPartition30MmuLevel2Start ,
    &pharosPartition31MmuLevel2Start ,
    &pharosPartition32MmuLevel2Start ,
    &pharosPartition33MmuLevel2Start ,
    &pharosPartition34MmuLevel2Start ,
    &pharosPartition35MmuLevel2Start ,
    &pharosPartition36MmuLevel2Start ,
    &pharosPartition37MmuLevel2Start ,
    &pharosPartition38MmuLevel2Start ,
    &pharosPartition39MmuLevel2Start ,
    &pharosPartition40MmuLevel2Start ,
    &pharosPartition41MmuLevel2Start ,
    &pharosPartition42MmuLevel2Start ,
    &pharosPartition43MmuLevel2Start ,
    &pharosPartition44MmuLevel2Start ,
    &pharosPartition45MmuLevel2Start ,
    &pharosPartition46MmuLevel2Start ,
    &pharosPartition47MmuLevel2Start ,
    &pharosPartition48MmuLevel2Start ,
    &pharosPartition49MmuLevel2Start ,
    &pharosPartition50MmuLevel2Start ,
    &pharosPartition51MmuLevel2Start ,
    &pharosPartition52MmuLevel2Start ,
    &pharosPartition53MmuLevel2Start ,
    &pharosPartition54MmuLevel2Start ,
    &pharosPartition55MmuLevel2Start ,
    &pharosPartition56MmuLevel2Start ,
    &pharosPartition57MmuLevel2Start ,
    &pharosPartition58MmuLevel2Start ,
    &pharosPartition59MmuLevel2Start ,
    &pharosPartition60MmuLevel2Start ,
    &pharosPartition61MmuLevel2Start ,
    &pharosPartition62MmuLevel2Start ,
    &pharosPartition63MmuLevel2Start ,
};


const ptrPharosCpuMmuLevel3Table partitionLevel3Tables[] = {
    &pharosPartition0MmuLevel3Start ,
    &pharosPartition1MmuLevel3Start ,
    &pharosPartition2MmuLevel3Start ,
    &pharosPartition3MmuLevel3Start ,
    &pharosPartition4MmuLevel3Start ,
    &pharosPartition5MmuLevel3Start ,
    &pharosPartition6MmuLevel3Start ,
    &pharosPartition7MmuLevel3Start ,
    &pharosPartition8MmuLevel3Start ,
    &pharosPartition9MmuLevel3Start ,
    &pharosPartition10MmuLevel3Start ,
    &pharosPartition11MmuLevel3Start ,
    &pharosPartition12MmuLevel3Start ,
    &pharosPartition13MmuLevel3Start ,
    &pharosPartition14MmuLevel3Start ,
    &pharosPartition15MmuLevel3Start ,
    &pharosPartition16MmuLevel3Start ,
    &pharosPartition17MmuLevel3Start ,
    &pharosPartition18MmuLevel3Start ,
    &pharosPartition19MmuLevel3Start ,
    &pharosPartition20MmuLevel3Start ,
    &pharosPartition21MmuLevel3Start ,
    &pharosPartition22MmuLevel3Start ,
    &pharosPartition23MmuLevel3Start ,
    &pharosPartition24MmuLevel3Start ,
    &pharosPartition25MmuLevel3Start ,
    &pharosPartition26MmuLevel3Start ,
    &pharosPartition27MmuLevel3Start ,
    &pharosPartition28MmuLevel3Start ,
    &pharosPartition29MmuLevel3Start ,
    &pharosPartition30MmuLevel3Start ,
    &pharosPartition31MmuLevel3Start ,
    &pharosPartition32MmuLevel3Start ,
    &pharosPartition33MmuLevel3Start ,
    &pharosPartition34MmuLevel3Start ,
    &pharosPartition35MmuLevel3Start ,
    &pharosPartition36MmuLevel3Start ,
    &pharosPartition37MmuLevel3Start ,
    &pharosPartition38MmuLevel3Start ,
    &pharosPartition39MmuLevel3Start ,
    &pharosPartition40MmuLevel3Start ,
    &pharosPartition41MmuLevel3Start ,
    &pharosPartition42MmuLevel3Start ,
    &pharosPartition43MmuLevel3Start ,
    &pharosPartition44MmuLevel3Start ,
    &pharosPartition45MmuLevel3Start ,
    &pharosPartition46MmuLevel3Start ,
    &pharosPartition47MmuLevel3Start ,
    &pharosPartition48MmuLevel3Start ,
    &pharosPartition49MmuLevel3Start ,
    &pharosPartition50MmuLevel3Start ,
    &pharosPartition51MmuLevel3Start ,
    &pharosPartition52MmuLevel3Start ,
    &pharosPartition53MmuLevel3Start ,
    &pharosPartition54MmuLevel3Start ,
    &pharosPartition55MmuLevel3Start ,
    &pharosPartition56MmuLevel3Start ,
    &pharosPartition57MmuLevel3Start ,
    &pharosPartition58MmuLevel3Start ,
    &pharosPartition59MmuLevel3Start ,
    &pharosPartition60MmuLevel3Start ,
    &pharosPartition61MmuLevel3Start ,
    &pharosPartition62MmuLevel3Start ,
    &pharosPartition63MmuLevel3Start ,
};


/**
 * Kernel section table 
 */
KERNEL_SECTION_USS PharosCpuMmuLevel0Table kernelSectionTable PHAROS_ALIGN_4KiB;

void pharosCpuMmuInitializeLow();

static void initializeTable2Invalid(ptrPharosCpuMmuLevel2Table table);
static void initializeTable3Invalid(ptrPharosCpuMmuLevel3Table table);

static bool checkIntervalsIntersect(const uint8_t *a0 , const uint8_t *a1 , const uint8_t *b0 , const uint8_t *b1);


static bool hasPermissions(uint8_t *address , ptrEnvMemoryProtection protection);

/**
 * Checks if the interval defined by the address and the size has any permission inside the protection 
 * @param address start of the interval to check
 * @param size size of the interval to check
 * @param protection protection interval
 * @return returns TRUE if the interval defined by address and address+size intersects with the protection
 */
static bool hasAnyPermission(uint8_t *address , uint64_t size , ptrEnvMemoryProtection protection);

/**
 * Add a table level 1 with invalid entries to an entry on the level 0 table
 * 
 * @param table0 level 0 table where to add the table level 1
 * @param table1 table 1 to add to the table level 0 
 * @param indexLevel0 index on the table 0 where to add the table 1
 */
static void table1LevelAdd(ptrPharosCpuMmuLevel0Table table0 , ptrPharosCpuMmuLevel1Table table1 , uint16_t indexLevel0);

/**
 * Add a table level 2 with invalid entries to an entry on the level 1 table
 * 
 * @param table1 level 1 table where to add the table level 2
 * @param table2 table 2 to add to the table level 1
 * @param indexLevel1 index on the table 1 where to add the table 2
 */
static void table2LevelAdd(ptrPharosCpuMmuLevel1Table table1 , ptrPharosCpuMmuLevel2Table table2 , uint16_t indexLevel1);

/**
 * Add a table level 3 with invalid entries to an entry on the level 2 table
 * 
 * @param table2 level 2 table where to add the table level 3
 * @param table3 table 3 to add to the table level 2
 * @param indexLevel2 index on the table 2 where to add the table 3
 * @param address start address to initialize the level 3 table
 */
static void table3LevelAddTable(ptrPharosCpuMmuLevel2Table table2 , ptrPharosCpuMmuLevel3Table table3 , uint16_t indexLevel2 , uint8_t *address);


static bool descriptor3IsInvalid(PharosCpuMmuLevel3Descriptor descritor);


static bool descriptor2IsBlock(PharosCpuMmuLevel2Descriptor descritor);


static bool descriptor2IsTable(PharosCpuMmuLevel2Descriptor descritor);


static bool descriptor1IsInvalid(PharosCpuMmuLevel1Descriptor descritor);


static bool descriptor0IsInvalid(PharosCpuMmuLevel0Descriptor descritor);


static void initSharedStack(ptrPharosCpuMmuLevel0Table tableLevel0 , ptrPharosCpuMmuLevel2Table *iterator2 ,
                            ptrPharosCpuMmuLevel3Table *iterator3);

/**
 * Change the permissions (with the AP fields) of the level 3 table descriptor
 * 
 * @param descriptor descriptor to change the permissions 
 * @param ap new permissions
 */
static void change4KSectionPermissions(ptrPharosCpuMmuLevel3Descriptor descriptor , uint8_t ap);


/**
 * Change the permissions (with the AP fields) of the level 2 table descriptor
 * 
 * @param descriptor descriptor to change the permissions 
 * @param ap new permissions
 */
static void change2MSectionPermissions(ptrPharosCpuMmuLevel2Descriptor descriptor , uint8_t ap);


/**
 * Change the permissions (with the AP fields) of the level 2 table descriptor and set the address for real-addressing
 * 
 * @param descriptor descriptor to change the permissions 
 * @param address address to initialize the descriptor to
 */
static void add2MSectionPermissionsAddress(ptrPharosCpuMmuLevel2Descriptor descriptor , uint8_t *address);


/**
 * Change the permissions of the shared stack
 * 
 * @param sharedStack shared stack to change the permissions in
 * @param context MMU context to change the permissions in
 * @param apValue new permissions
 */
static void sharedStackChangePermissions(ptrStackShared sharedStack , ptrPartitionCpuMmuContext context , uint8_t apValue);


/**
 * Initialize the all the tables of a given environment (partition)
 * Starts at level 0 and goes all the way to level 2 (partitions are aligned to 2 MiB so no need to go level 3)
 * 
 * @param table level 0 table used as basis 
 * @param iterator1 iterator for level 1 (to be used to add additional level 1 tables, as needed) 
 * @param iterator2 iterator for level 2 (to be used to add additional level 2 tables, as needed)
 * @param env memory space of the partition
 */
static void initializeLevel0Table(ptrPharosCpuMmuLevel0Table table , ptrPharosCpuMmuLevel1Table *iterator1 , ptrPharosCpuMmuLevel2Table *iterator2 , ptrPharosCpuMmuLevel3Table *iterator3 , ptrEnvMemoryProtection env);


/**
 * Initializes the MMU tables, starting at level 1 and going down to level 2 (partitions are aligned to 2 MiB so no need to go level 3)
 * 
 * @param table1 level 1 table
 * @param iterator2 iterator for level 2 (to be used to add additional level 2 tables, as needed)
 * @param address address being initialized
 * @param env memory space of the partition
 */
static void initializeLevel1Table(ptrPharosCpuMmuLevel1Table table1 , ptrPharosCpuMmuLevel2Table *iterator2 , ptrPharosCpuMmuLevel3Table *iterator3 , uint8_t *address , ptrEnvMemoryProtection env);


/**
 * Initializes the level 2 table of a given environment (partition).
 * 
 * @param table2 level 2 table
 * @param address address being initialized
 * @param protection memory space of the partition
 */
static void initializeLevel2Table(ptrPharosCpuMmuLevel2Table table2 , ptrPharosCpuMmuLevel3Table *iterator3 , uint8_t *address , ptrEnvMemoryProtection protection);


/**
 * Initializes the level 3 table of a given environment (partition).
 * 
 * @param table3 level 3 table
 * @param address address being initialized
 * @param protection memory space of the partition
 */
static void initializeLevel3Table(ptrPharosCpuMmuLevel3Table table3 , uint8_t *address , ptrEnvMemoryProtection protection);


/**
 * Calculate the index on a level 0 table for the specified address
 * Address uses up to 48 bits (as defined in the ARMv8 spec)
 * 
 * @param address address to calculate the index for
 * 
 * @return returns the index on the level 0 table (0-511)
 */
static uint16_t calculateIndex512G(uint8_t *address);


/**
 * Calculate the index on a level 1 table for the specified address and given the index on the level 0 table
 * Address uses up to 48 bits (as defined in the ARMv8 spec)
 * 
 * @param address address to calculate the index for
 * @param index0 index on the table 0
 * 
 * @return returns the index on the level 1 table (0-511)
 */
static uint16_t calculateIndex1G(uint8_t *address , uint16_t index0);


/**
 * Calculate the index on a level 2 table for the specified address and given the index on the level 0 and 1 tables
 * Address uses up to 48 bits (as defined in the ARMv8 spec)
 * 
 * @param address address to calculate the index for
 * @param index0 index on the table 0
 * @param index1 index on the table 1
 * 
 * @return returns the index on the level 2 table (0-511)
 */
static uint16_t calculateIndex2M(uint8_t *address , uint16_t index0 , uint16_t index1);


/**
 * Calculate the index on a level 3 table for the specified address and given the index on the level 0, 1 and 2 tables
 * Address uses up to 48 bits (as defined in the ARMv8 spec)
 * 
 * @param address address to calculate the index for
 * @param index0 index on the table 0
 * @param index1 index on the table 1
 * @param index2 index on the table 2
 * 
 * @return returns the index on the level 3 table (0-511)
 */
static uint16_t calculateIndex4K(uint8_t *address , uint16_t index0 , uint16_t index1 , uint16_t index2);

static uint32_t add2MBulk(ptrPharosCpuMmuLevel1Table table1 , ptrChannelBulkQueue queue , uint8_t *address , ptrPartition partition);
static uint32_t add64KBulk(ptrPharosCpuMmuLevel1Table table1 , ptrChannelBulkQueue queue , uint8_t *address , ptrPartition partition);
static uint32_t add4KBulk(ptrPharosCpuMmuLevel1Table table1 , ptrChannelBulkQueue queue , uint8_t *address , ptrPartition partition);
static void remove4KBulk(ptrPharosCpuMmuLevel1Table table1 , ptrChannelBulkQueue queue , uint8_t *address);

static void remove64KBulk(ptrPharosCpuMmuLevel1Table table1 , ptrChannelBulkQueue queue , uint8_t *address);

static void remove2MBulk(ptrPharosCpuMmuLevel1Table table1 , ptrChannelBulkQueue queue , uint8_t *address);
static ptrPharosCpuMmuLevel2Table linkLevel2Table(ptrPharosCpuMmuLevel1Table table1 , uint16_t index1 , ptrPharosCpuMmuLevel2Table table2 , uint8_t *address);
static ptrPharosCpuMmuLevel3Table linkLevel3Table(ptrPharosCpuMmuLevel2Table table2 , uint16_t index2 , ptrPharosCpuMmuLevel3Table table3 , uint8_t *address);

static uint8_t *physical2Virtual(uint8_t *address);
static void initializeIoMemArea(ptrPharosCpuMmuLevel0Table table0 , ptrIoMemAreaTable table);


void pharosCpuMmuInitialize()
{
    /* get the core we are running on */
    CoreNumber core = pharosICoreRunning();

    /* iterator */
    uint32_t i;
    EnvMemoryProtection kernelProtection;

    ptrPharosCpuMmuLevel1Table kernelIterator1 = &kernelTable1Table[core];
    ptrPharosCpuMmuLevel2Table kernelIterator2 = &kernelTable2Table[core];
    ptrPharosCpuMmuLevel3Table iterator3 = &kernelTable3Table[core][0];

    /* partition table on the running core */
    ptrPartitionTable partitions = pharosICorePartitionTable();

    kernelProtection.startAddress[0] = &pharosKernelStart;
    kernelProtection.endAddress[0] = &pharosIpCallStart;


    /* set the attribute 1 as normal memory and attribute 0 as device memory */
    pharosCpuMair_El1Set((MAIR_EL1_ATTRIBUTE_MEMORY << (MAIR_EL1_ATTRIBUTE_MEMORY_INDEX * 8)) |
                         (MAIR_EL1_ATTRIBUTE_DEVICE << (MAIR_EL1_ATTRIBUTE_DEVICE_INDEX * 8)));

    /* go through each of the partition on the running core */
    for(i = 0U; i < partitions->size; i++)
    {
        /* get the partition */
        ptrPartition p = &partitions->partitions[i];

        PartitionNumber number = p->id;

        /* get the shared stack MMU tables level 2 for the partition */
        ptrPharosCpuMmuLevel2Table iterator2SharedStack = &pharosSharedStackTables2Start[number * (uintptr_t) & pharosNumberSharedStackMmuTables2PerPartition ];

        /* get the shared stack MMU tables level 3 for the partition */
        ptrPharosCpuMmuLevel3Table iterator3SharedStack = &pharosSharedStackTables3Start[number * (uintptr_t) & pharosNumberSharedStackMmuTables3PerPartition ];

        /* set the level 0 table */
        p->context.level0Table = &partitionsTable0[number];

        /* set the level 1 table */
        p->context.level1Table = &partitionsTable1[number];

        /* get the section table to be use for this partition */
        ptrPharosCpuMmuLevel0Table tableLevel0 = p->context.level0Table;

        /* iterator for level 1. Actually this is not an iterator and is fixed (can "only" support up to 512 GiB per partition) */
        ptrPharosCpuMmuLevel1Table iterator1 = p->context.level1Table;

        /* iterator for level 2 pages. These additional pages are defined in the linker script. They are enough to support the memory of the partition */
        ptrPharosCpuMmuLevel2Table iterator2 = partitionLevel2Tables[number];

        /* iterator for level 3 pages. These additional pages are defined in the linker script. They are enough to support the memory of the partition */
        ptrPharosCpuMmuLevel3Table iterator3 = partitionLevel3Tables[number];

        /* initialize the level 0 table */
        initializeLevel0Table(tableLevel0 , &iterator1 , &iterator2 , &iterator3 , &p->memProtection);

        /* initialize the address */
        initSharedStack(tableLevel0 , &iterator2SharedStack , &iterator3SharedStack);

        /* initialize the IO MMU areas */
        initializeIoMemArea(tableLevel0 , &p->ioTable);
    }

    /* initialize the kernel tables (for the idle thread) */
    initializeLevel0Table(&kernelTable0Table[core] , &kernelIterator1 , &kernelIterator2 , &iterator3 , &kernelProtection);
}


void initializeLevel3Table(ptrPharosCpuMmuLevel3Table table3 , uint8_t *address , ptrEnvMemoryProtection protection)
{
    uint16_t i;

    /* go through the 1 GiB of memory, starting at address and going 4 KiB at a time */
    for(i = 0; i < 512U; i++ , address += VALUE_4KiB)
    {
        /* fill the address (AKA real-addressing) */
        table3->descriptor[i].descriptor.address = ((uintptr_t) address) >> 12;

        /* place the 1 to state it is a block descriptor */
        table3->descriptor[i].descriptor.shouldBeOneOne = 0b11;

        /* set not contiguous */
        table3->descriptor[i].descriptor.contiguous = CONTIGUOUS_NO;

        /* always secure */
        table3->descriptor[i].descriptor.ns = SECURE_YES;

        /* shareable (needed for the MPSC queue (in RMP component) so that the strx and ldrx are paired) */
        table3->descriptor[i].descriptor.sh = SHAREABLE_OUTER;

        /* not global */
        table3->descriptor[i].descriptor.ng = GLOBAL_YES;

        /* initialize the access flag */
        table3->descriptor[i].descriptor.af = ACCESS_INITIALIZE;

        /* initialize reserved with 0's */
        table3->descriptor[i].descriptor.reserved = 0U;

        /* reset ignored bits */
        table3->descriptor[i].descriptor.ignored = 0U;


        /* if inside the text zone */
        if(checkIntervalsIntersect(address , address + VALUE_4KiB , &pharosStartText , &pharosEndText) == TRUE)
        {
            /* then initialize the section descriptor with read-only access */
            /* set to memory type to "normal" */
            table3->descriptor[i].descriptor.attrIndx = MAIR_EL1_ATTRIBUTE_MEMORY_INDEX;

            /* EL1 and EL0 can only read */
            table3->descriptor[i].descriptor.ap = AP_EL1_EL0_RO;

            /* can execute */
            table3->descriptor[i].descriptor.pxn = EXECUTE;

            /* can execute */
            table3->descriptor[i].descriptor.uxn_xn = EXECUTE;
        }
            /* if inside the environment memory zone */
        else if((address >= protection->startAddress[0]) && (address < protection->endAddress[0]))
        {
            /* set to memory type to "normal" */
            table3->descriptor[i].descriptor.attrIndx = MAIR_EL1_ATTRIBUTE_MEMORY_INDEX;

            /* EL1 and EL0 can read/write */
            table3->descriptor[i].descriptor.ap = AP_EL1_EL0_RW;

            /* not execute */
            table3->descriptor[i].descriptor.pxn = EXECUTE_NEVER;

            /* not execute */
            table3->descriptor[i].descriptor.uxn_xn = EXECUTE_NEVER;
        }
            /* if inside the interrupt stack zone */
        else if((address >= &pharosCpuInterruptStack[pharosICoreRunning()][0]) && (address < &pharosCpuInterruptStack[pharosICoreRunning()][PHAROS_CPU_INTERRUPT_STACK_SIZE - 1]))
        {
            /* set to memory type to "normal" */
            table3->descriptor[i].descriptor.attrIndx = MAIR_EL1_ATTRIBUTE_MEMORY_INDEX;

            /* EL1 can read/write */
            table3->descriptor[i].descriptor.ap = AP_EL1_RW;

            /* not execute */
            table3->descriptor[i].descriptor.pxn = EXECUTE_NEVER;

            /* not execute */
            table3->descriptor[i].descriptor.uxn_xn = EXECUTE_NEVER;
        }
            /* if inside the shared memory zone */
        else if((address >= &pharosSharedStart) && (address < &pharosSharedEnd))
        {
            /* set to memory type to "normal" */
            table3->descriptor[i].descriptor.attrIndx = MAIR_EL1_ATTRIBUTE_MEMORY_INDEX;

            /* EL1 and EL0 can read and write */
            table3->descriptor[i].descriptor.ap = AP_EL1_EL0_RW;

            /* not execute */
            table3->descriptor[i].descriptor.pxn = EXECUTE_NEVER;

            /* not execute */
            table3->descriptor[i].descriptor.uxn_xn = EXECUTE_NEVER;
        }
            /* if inside the inter-partition calls zone */
        else if((address >= &pharosIpCallStart) && (address < &pharosIpCallEnd))
        {
            /* set to memory type to "normal" */
            table3->descriptor[i].descriptor.attrIndx = MAIR_EL1_ATTRIBUTE_MEMORY_INDEX;

            /* EL1 and EL0 can read (only) */
            table3->descriptor[i].descriptor.ap = AP_EL1_EL0_RO;

            /* can execute */
            table3->descriptor[i].descriptor.pxn = EXECUTE;

            /* can execute */
            table3->descriptor[i].descriptor.uxn_xn = EXECUTE;
        }
            /* check if otherwise in the kernel section */
        else if((address >= &pharosKernelStart) && (address < &pharosKernelEnd))
        {
            /* set to memory type to "normal" */
            table3->descriptor[i].descriptor.attrIndx = MAIR_EL1_ATTRIBUTE_MEMORY_INDEX;

            /* EL1 can read and write */
            table3->descriptor[i].descriptor.ap = AP_EL1_RW;

            /* privileged cannot execute  */
            table3->descriptor[i].descriptor.pxn = EXECUTE;

            /* unprivileged cannot execute */
            table3->descriptor[i].descriptor.uxn_xn = EXECUTE_NEVER;
        }
        else
        {
            /* does not have any permissions */

            /* set to memory type to "normal" */
            table3->descriptor[i].descriptor.attrIndx = MAIR_EL1_ATTRIBUTE_MEMORY_INDEX;

            /* only EL1 can read and write */
            table3->descriptor[i].descriptor.ap = AP_EL1_RW;

            /* not execute */
            table3->descriptor[i].descriptor.pxn = EXECUTE;

            /* not execute */
            table3->descriptor[i].descriptor.uxn_xn = EXECUTE_NEVER;
        }
    }
}


void initializeLevel2Table(ptrPharosCpuMmuLevel2Table table2 , ptrPharosCpuMmuLevel3Table *iterator3 , uint8_t *address , ptrEnvMemoryProtection protection)
{
    uint16_t i;


    /* go through the 1 GiB of memory, starting at address and going 2 MiB at a time */
    for(i = 0; i < 512U; i++ , address += VALUE_2MiB)
    {
        /* check if the 2 MiB are used */
        if(hasAnyPermission(address , VALUE_2MiB , protection) == TRUE)
        {
            /* copy the address on the previous table */
            table2->descriptor[i].table.address = ((uintptr_t) * iterator3) >> 12;

            /* initialize the table 1 property to point to a table 2 */
            table2->descriptor[i].table.shouldBeOneOne = 0b11;

            /* always secure */
            table2->descriptor[i].table.nsTable = NS_TABLE_SECURE_YES;

            /* ignore the permissions set by this table, the following table will contain the correct permissions */
            table2->descriptor[i].table.apTable = AP_TABLE_IGNORE;

            /* ignore the execute flag on this table, the following table will contain the correct flag */
            table2->descriptor[i].table.uxn_xnTable = XN_TABLE_IGNORE;

            /* ignore the execute flag on this table, the following table will contain the correct flag */
            table2->descriptor[i].table.pxnTable = PXN_TABLE_IGNORE;

            /* we need an extra table for this area */
            initializeLevel3Table(*iterator3 , address , protection);

            /* increase the iterator for level 3 tables */
            (*iterator3)++;
        }
        else
        {
            /* place block descriptor (for now assume that ALL memory address are not valid even for EL1.
             * This could be redone later on if the clock or UART are on these addresses for a specific CPU */
            table2->descriptor[i].block.shouldBeZero2 = 0;
            table2->descriptor[i].block.shouldBeZeroOne = 0b01;
            table2->descriptor[i].block.address = ((uintptr_t) address) >> 12;
            table2->descriptor[i].block.uxn_xn = EXECUTE;
            table2->descriptor[i].block.pxn = EXECUTE_NEVER;
            table2->descriptor[i].block.ap = AP_EL1_RW;
            table2->descriptor[i].block.af = ACCESS_INITIALIZE;
            table2->descriptor[i].block.attrIndx = MAIR_EL1_ATTRIBUTE_DEVICE_INDEX;
            table2->descriptor[i].block.contiguous = CONTIGUOUS_NO;
            table2->descriptor[i].block.ignored = 0;
            table2->descriptor[i].block.ng = GLOBAL_YES;
            table2->descriptor[i].block.ns = SECURE_YES;
            table2->descriptor[i].block.reserved = 0;
            table2->descriptor[i].block.sh = SHAREABLE_NO;
        }
    }
}


void initializeLevel1Table(ptrPharosCpuMmuLevel1Table table1 , ptrPharosCpuMmuLevel2Table *iterator2 , ptrPharosCpuMmuLevel3Table *iterator3 , uint8_t *address , ptrEnvMemoryProtection env)
{
    uint16_t i;


    /* go through the 512 GiB of memory, starting at address and going 1 GiB at a time */
    for(i = 0; i < 512U; i++ , address += VALUE_1GiB)
    {
        /* check if the 512 GiB are inside the I/O */
        if(hasAnyPermission(address , VALUE_1GiB , env) == TRUE)
        {
            /* copy the address on the previous table */
            table1->descriptor[i].table.address = ((uintptr_t) * iterator2) >> 12;

            /* initialize the table 1 property to point to a table 2 */
            table1->descriptor[i].table.shouldBeOneOne = 0b11;

            /* always secure */
            table1->descriptor[i].table.nsTable = NS_TABLE_SECURE_YES;

            /* ignore the permissions set by this table, the following table will contain the correct permissions */
            table1->descriptor[i].table.apTable = AP_TABLE_IGNORE;

            /* ignore the execute flag on this table, the following table will contain the correct flag */
            table1->descriptor[i].table.uxn_xnTable = XN_TABLE_IGNORE;

            /* ignore the execute flag on this table, the following table will contain the correct flag */
            table1->descriptor[i].table.pxnTable = PXN_TABLE_IGNORE;

            /* we need an extra table for this area */
            initializeLevel2Table(*iterator2 , iterator3 , address , env);

            /* increase the iterator for level 2 tables */
            (*iterator2)++;
        }
        else
        {
            /* place invalid descriptor (for now assume that ALL memory address are not valid even for EL1.
             * This could be redone later on if the clock or UART are on these addresses for a specific CPU */
            table1->descriptor[i].block.shouldBeZero2 = 0;
            table1->descriptor[i].block.shouldBeZeroOne = 0b01;
            table1->descriptor[i].block.address = ((uintptr_t) address) >> 12;
            table1->descriptor[i].block.uxn_xn = EXECUTE_NEVER;
            table1->descriptor[i].block.pxn = EXECUTE_NEVER;
            table1->descriptor[i].block.ap = AP_EL1_RW;
            table1->descriptor[i].block.af = ACCESS_INITIALIZE;
            table1->descriptor[i].block.attrIndx = MAIR_EL1_ATTRIBUTE_DEVICE_INDEX;
            table1->descriptor[i].block.contiguous = CONTIGUOUS_NO;
            table1->descriptor[i].block.ignored = 0;
            table1->descriptor[i].block.ng = GLOBAL_YES;
            table1->descriptor[i].block.ns = SECURE_YES;
            table1->descriptor[i].block.reserved = 0;
            table1->descriptor[i].block.sh = SHAREABLE_NO;
        }
    }
}


void initializeLevel0Table(ptrPharosCpuMmuLevel0Table table , ptrPharosCpuMmuLevel1Table *iterator1 , ptrPharosCpuMmuLevel2Table *iterator2 , ptrPharosCpuMmuLevel3Table *iterator3 , ptrEnvMemoryProtection env)
{
    /* iterator on the table */
    uint16_t i;
    uint8_t *address = 0;

    /* for the full 256 TiB, at steps of 512 GiB */
    for(i = 0; i < 512U; i++ , address += VALUE_512GiB)
    {
        /* check if the 512 GiB are used by the partition  */
        if(hasAnyPermission(address , VALUE_512GiB , env) == TRUE)
        {
            /* copy the address on the previous table */
            table->descriptor[i].table.address = ((uintptr_t) * iterator1) >> 12;

            /* initialize the table 1 property to point to a table 2 */
            table->descriptor[i].table.shouldBeOneOne = 0b11;

            /* always secure */
            table->descriptor[i].table.nsTable = NS_TABLE_SECURE_YES;

            /* ignore the permissions set by this table, the following table will contain the correct permissions */
            table->descriptor[i].table.apTable = AP_TABLE_IGNORE;

            /* ignore the execute flag on this table, the following table will contain the correct flag */
            table->descriptor[i].table.uxn_xnTable = XN_TABLE_IGNORE;

            /* ignore the execute flag on this table, the following table will contain the correct flag */
            table->descriptor[i].table.pxnTable = PXN_TABLE_IGNORE;

            /* we need an extra table for this area */
            initializeLevel1Table(*iterator1 , iterator2 , iterator3 , address , env);

            /* increase the iterator for level 1 tables */
            (*iterator1)++;
        }
        else
        {
            /* place invalid descriptor (for now assume that ALL memory address are not valid even for EL1.
             * This could be redone later on if the clock or UART are on these addresses for a specific CPU */
            table->descriptor[i].invalid.shouldBeZero = 0;
        }
    }
}


void pharosCpuPartitionMmuInit(ptrPartitionCpuMmuContext context , ptrEnvMemoryProtection memProtection)
{
    /* do nothing */
}


uint16_t calculateIndex512G(uint8_t *address)
{
    /* calculate the index on 512 GiB sections */
    return ((uintptr_t) address) >> 39U;
}


uint16_t calculateIndex1G(uint8_t *address , uint16_t index0)
{
    /* calculate the index on 1 GiB sections given the offset of 512 GiB */
    return ((uintptr_t) address - index0 * VALUE_512GiB) >> 30U;
}


uint16_t calculateIndex2M(uint8_t *address , uint16_t index0 , uint16_t index1)
{
    /* calculate the index on 2 MiB sections given the offset of 512 GiB and 1 GiB */
    return ((uintptr_t) address - index0 * VALUE_512GiB - index1 * VALUE_1GiB) >> 21U;
}


uint16_t calculateIndex4K(uint8_t *address , uint16_t index0 , uint16_t index1 , uint16_t index2)
{
    /* calculate the index on 4 KiB sections given the offset of 512 GiB, 1 GiB and 2 MiB */
    return ((uintptr_t) address - index0 * VALUE_512GiB - index1 * VALUE_1GiB - index2 * VALUE_2MiB) >> 12U;
}


ptrPharosCpuMmuLevel1Table getLevel1Table(PharosCpuMmuLevel0Descriptor descriptor)
{
    /* check if the descriptor is a level 1 table */
    if(descriptor.table.shouldBeOneOne == 0b11)
    {
        /* return the level 1 table address */
        return (ptrPharosCpuMmuLevel1Table) (uintptr_t) (descriptor.table.address << 12U);
    }
        /* not a table */
    else
    {
        /* return not a table */
        return NULL;
    }
}


ptrPharosCpuMmuLevel2Table getLevel2Table(PharosCpuMmuLevel1Descriptor descriptor)
{
    /* check if the descriptor is a level 2 table */
    if(descriptor.table.shouldBeOneOne == 0b11)
    {
        /* return the level 2 table address */
        return (ptrPharosCpuMmuLevel2Table) (uintptr_t) (descriptor.table.address << 12U);
    }
        /* not a table */
    else
    {
        /* return not a table */
        return NULL;
    }
}


ptrPharosCpuMmuLevel3Table getLevel3Table(PharosCpuMmuLevel2Descriptor descriptor)
{
    /* check if the descriptor is a level 3 table */
    if(descriptor.table.shouldBeOneOne == 0b11)
    {
        /* return the level 3 table address */
        return (ptrPharosCpuMmuLevel3Table) (uintptr_t) (descriptor.table.address << 12U);
    }
        /* not a table */
    else
    {
        /* return not a table */
        return NULL;
    }
}


void table1DescriptorInitBlock1Io(ptrPharosCpuMmuLevel1Descriptor descriptor , uint8_t *address)
{
    /* fill the address (AKA real-addressing) */
    descriptor->block.address = ((uintptr_t) address) >> 30U;

    /* place the 1 to state it is a block descriptor */
    descriptor->block.shouldBeZeroOne = 0b01;

    /* set not contiguous */
    descriptor->block.contiguous = CONTIGUOUS_NO;

    /* always secure */
    descriptor->block.ns = SECURE_YES;

    /* not shareable */
    descriptor->block.sh = SHAREABLE_NO;

    /* not global */
    descriptor->block.ng = GLOBAL_NO;

    /* initialize the access flag */
    descriptor->block.af = ACCESS_INITIALIZE;

    /* initialize reserved with 0's */
    descriptor->block.reserved = 0U;

    /* reset ignored bits */
    descriptor->block.ignored = 0U;

    /* initialize attributes for device memory type */
    descriptor->block.attrIndx = MAIR_EL1_ATTRIBUTE_DEVICE_INDEX;

    /* set read/write for EL0 and EL1 */
    descriptor->block.ap = AP_EL1_EL0_RW;

    /* not execute */
    descriptor->block.pxn = EXECUTE_NEVER;

    /* not execute */
    descriptor->block.uxn_xn = EXECUTE_NEVER;
}


void tableDescriptorInitBlock3Io(ptrPharosCpuMmuLevel3Descriptor descriptor , uint8_t *address)
{
    /* fill the address (AKA real-addressing) */
    descriptor->descriptor.address = ((uintptr_t) address) >> 12U;

    /* place the 0's */
    descriptor->descriptor.shouldBeZero = 0b0000U;

    /* place the 1 to state it is a 4K page descriptor */
    descriptor->descriptor.shouldBeOneOne = 0b11U;

    /* set not contiguous */
    descriptor->descriptor.contiguous = CONTIGUOUS_NO;

    /* always secure */
    descriptor->descriptor.ns = SECURE_YES;

    /* not shareable */
    descriptor->descriptor.sh = SHAREABLE_NO;

    /* not global */
    descriptor->descriptor.ng = GLOBAL_NO;

    /* initialize the access flag */
    descriptor->descriptor.af = ACCESS_INITIALIZE;

    /* initialize reserved with 0's */
    descriptor->descriptor.reserved = 0U;

    /* reset ignored bits */
    descriptor->descriptor.ignored = 0U;

    /* initialize attributes for device memory type */
    descriptor->descriptor.attrIndx = MAIR_EL1_ATTRIBUTE_DEVICE_INDEX;

    /* set read/write for EL0 and EL1 */
    descriptor->descriptor.ap = AP_EL1_EL0_RW;

    /* not execute */
    descriptor->descriptor.pxn = EXECUTE_NEVER;

    /* not execute */
    descriptor->descriptor.uxn_xn = EXECUTE_NEVER;
}


void tableDescriptorInitBlock2Io(ptrPharosCpuMmuLevel2Descriptor descriptor , uint8_t *address)
{

    /* fill the address (AKA real-addressing) */
    descriptor->block.address = ((uintptr_t) address) >> 12U;

    /* place the 1 to state it is a block descriptor */
    descriptor->block.shouldBeZeroOne = 0b01U;

    /* set not contiguous */
    descriptor->block.contiguous = CONTIGUOUS_NO;

    /* always secure */
    descriptor->block.ns = SECURE_YES;

    /* not shareable */
    descriptor->block.sh = SHAREABLE_NO;

    /* not global */
    descriptor->block.ng = GLOBAL_NO;

    /* initialize the access flag */
    descriptor->block.af = ACCESS_INITIALIZE;

    /* initialize reserved with 0's */
    descriptor->block.reserved = 0U;

    /* reset ignored bits */
    descriptor->block.ignored = 0U;

    /* initialize attributes for device memory type */
    descriptor->block.attrIndx = MAIR_EL1_ATTRIBUTE_DEVICE_INDEX;

    /* set read/write for EL0 and EL1 */
    descriptor->block.ap = AP_EL1_EL0_RW;

    /* not execute */
    descriptor->block.pxn = EXECUTE_NEVER;

    /* not execute */
    descriptor->block.uxn_xn = EXECUTE_NEVER;
}


void table1DescriptorInitInvalid(ptrPharosCpuMmuLevel1Descriptor descriptor)
{
    /* set the zero bits */
    descriptor->invalid.shouldBeZero = 0U;
}


void table2DescriptorInitInvalid(ptrPharosCpuMmuLevel2Descriptor descriptor)
{
    /* set the zero bits */
    descriptor->invalid.shouldBeZero = 0;
}


void table3DescriptorInitEl1(ptrPharosCpuMmuLevel3Descriptor descriptor , uint8_t *address)
{
    /* fill the address (AKA real-addressing) */
    descriptor->descriptor.address = ((uintptr_t) address) >> 12U;

    /* place the 0's */
    descriptor->descriptor.shouldBeZero = 0b0000;

    /* place the 1 to state it is a 4K page descriptor */
    descriptor->descriptor.shouldBeOneOne = 0b11;

    /* set not contiguous */
    descriptor->descriptor.contiguous = CONTIGUOUS_NO;

    /* always secure */
    descriptor->descriptor.ns = SECURE_YES;

    /* not shareable */
    descriptor->descriptor.sh = SHAREABLE_NO;

    /* not global */
    descriptor->descriptor.ng = GLOBAL_NO;

    /* initialize the access flag */
    descriptor->descriptor.af = ACCESS_INITIALIZE;

    /* initialize reserved with 0's */
    descriptor->descriptor.reserved = 0U;

    /* reset ignored bits */
    descriptor->descriptor.ignored = 0U;

    /* initialize attributes for device memory type */
    descriptor->descriptor.attrIndx = MAIR_EL1_ATTRIBUTE_MEMORY_INDEX;

    /* set read/write for EL1 */
    descriptor->descriptor.ap = AP_EL1_RW;

    /* not execute */
    descriptor->descriptor.pxn = EXECUTE_NEVER;

    /* not execute */
    descriptor->descriptor.uxn_xn = EXECUTE_NEVER;
}


void table1LevelAdd(ptrPharosCpuMmuLevel0Table table , ptrPharosCpuMmuLevel1Table table1Iterator , uint16_t indexLevel0)
{
    uint32_t i;

    /* copy the address on the previous table */
    table->descriptor[indexLevel0].table.address = (uintptr_t) (table1Iterator) >> 12;

    /* initialize the table 1 property to point to a table 2 */
    table->descriptor[indexLevel0].table.shouldBeOneOne = 0b11;

    /* always secure */
    table->descriptor[indexLevel0].table.nsTable = NS_TABLE_SECURE_YES;

    /* ignore the permissions set by this table, the following table will contain the correct permissions */
    table->descriptor[indexLevel0].table.apTable = AP_TABLE_IGNORE;

    /* ignore the execute flag on this table, the following table will contain the correct flag */
    table->descriptor[indexLevel0].table.uxn_xnTable = XN_TABLE_IGNORE;

    /* ignore the execute flag on this table, the following table will contain the correct flag */
    table->descriptor[indexLevel0].table.pxnTable = PXN_TABLE_IGNORE;

    /* go to every level 1 descriptor and initialize it */
    for(i = 0U; i < 512U; i++)
    {
        /* initialize with invalid */
        table1DescriptorInitInvalid(&table1Iterator->descriptor[i]);
    }
}


void table3LevelAddTable(ptrPharosCpuMmuLevel2Table table2 , ptrPharosCpuMmuLevel3Table table3 , uint16_t indexLevel2 , uint8_t *address)
{
    uint32_t i;

    /* copy the address on the previous table */
    table2->descriptor[indexLevel2].table.address = (uintptr_t) (table3) >> 12;

    /* initialize the table 1 property to point to a table 2 */
    table2->descriptor[indexLevel2].table.shouldBeOneOne = 0b11;

    /* always secure */
    table2->descriptor[indexLevel2].table.nsTable = NS_TABLE_SECURE_YES;

    /* ignore the permissions set by this table, the following table will contain the correct permissions */
    table2->descriptor[indexLevel2].table.apTable = AP_TABLE_IGNORE;

    /* ignore the execute flag on this table, the following table will contain the correct flag */
    table2->descriptor[indexLevel2].table.uxn_xnTable = XN_TABLE_IGNORE;

    /* ignore the execute flag on this table, the following table will contain the correct flag */
    table2->descriptor[indexLevel2].table.pxnTable = PXN_TABLE_IGNORE;

    /* go to every level 3 descriptor and initialize it */
    for(i = 0U; i < 512U; i++ , address += VALUE_4KiB)
    {
        /* initialize with invalid */

        /* fill the address (AKA real-addressing) */
        table3->descriptor[i].descriptor.address = ((uintptr_t) address) >> 12;

        /* place the 1 to state it is a block descriptor */
        table3->descriptor[i].descriptor.shouldBeOneOne = 0b11;

        /* set not contiguous */
        table3->descriptor[i].descriptor.contiguous = CONTIGUOUS_NO;

        /* always secure */
        table3->descriptor[i].descriptor.ns = SECURE_YES;

        /* not shareable */
        table3->descriptor[i].descriptor.sh = SHAREABLE_INNER;

        /* not global */
        table3->descriptor[i].descriptor.ng = GLOBAL_YES;

        /* initialize the access flag */
        table3->descriptor[i].descriptor.af = ACCESS_INITIALIZE;

        /* initialize reserved with 0's */
        table3->descriptor[i].descriptor.reserved = 0U;

        /* reset ignored bits */
        table3->descriptor[i].descriptor.ignored = 0U;

        /* set to memory type to "normal" */
        table3->descriptor[i].descriptor.attrIndx = MAIR_EL1_ATTRIBUTE_MEMORY_INDEX;

        /* only EL1 can read and write */
        table3->descriptor[i].descriptor.ap = AP_EL1_RW;

        /* not execute */
        table3->descriptor[i].descriptor.pxn = EXECUTE;

        /* not execute */
        table3->descriptor[i].descriptor.uxn_xn = EXECUTE_NEVER;
    }
}


void table3LevelAddEl1(ptrPharosCpuMmuLevel2Table table2 , ptrPharosCpuMmuLevel3Table table3 , uint16_t indexLevel2 , uint8_t *address)
{
    uint32_t i;

    /* copy the address on the previous table */
    table2->descriptor[indexLevel2].table.address = (uintptr_t) (table3) >> 12;

    /* initialize the table 1 property to point to a table 2 */
    table2->descriptor[indexLevel2].table.shouldBeOneOne = 0b11;

    /* always secure */
    table2->descriptor[indexLevel2].table.nsTable = NS_TABLE_SECURE_YES;

    /* ignore the permissions set by this table, the following table will contain the correct permissions */
    table2->descriptor[indexLevel2].table.apTable = AP_TABLE_IGNORE;

    /* ignore the execute flag on this table, the following table will contain the correct flag */
    table2->descriptor[indexLevel2].table.uxn_xnTable = XN_TABLE_IGNORE;

    /* ignore the execute flag on this table, the following table will contain the correct flag */
    table2->descriptor[indexLevel2].table.pxnTable = PXN_TABLE_IGNORE;

    /* go to every level 3 descriptor and initialize it */
    for(i = 0U; i < 512U; i++ , address += VALUE_4KiB)
    {
        /* initialize with invalid */
        table3DescriptorInitEl1(&table3->descriptor[i] , address);
    }
}


void table2LevelAdd(ptrPharosCpuMmuLevel1Table table1 , ptrPharosCpuMmuLevel2Table table2 , uint16_t indexLevel1)
{
    uint32_t i;

    /* copy the address on the previous table */
    table1->descriptor[indexLevel1].table.address = (uintptr_t) (table2) >> 12;

    /* initialize the table 1 property to point to a table 2 */
    table1->descriptor[indexLevel1].table.shouldBeOneOne = 0b11;

    /* always secure */
    table1->descriptor[indexLevel1].table.nsTable = NS_TABLE_SECURE_YES;

    /* ignore the permissions set by this table, the following table will contain the correct permissions */
    table1->descriptor[indexLevel1].table.apTable = AP_TABLE_IGNORE;

    /* ignore the execute flag on this table, the following table will contain the correct flag */
    table1->descriptor[indexLevel1].table.uxn_xnTable = XN_TABLE_IGNORE;

    /* ignore the execute flag on this table, the following table will contain the correct flag */
    table1->descriptor[indexLevel1].table.pxnTable = PXN_TABLE_IGNORE;

    /* go to every level 2 descriptor and initialize it */
    for(i = 0U; i < 512U; i++)
    {
        /* initialize with invalid */
        table2DescriptorInitInvalid(&table2->descriptor[i]);
    }
}


void table3DescriptorInitIo(ptrPharosCpuMmuLevel0Table table , ptrPharosCpuMmuLevel1Table *table1Iterator ,
                            ptrPharosCpuMmuLevel2Table *table2Iterator , ptrPharosCpuMmuLevel3Table *table3Iterator ,
                            uint8_t *address)
{
    ptrPharosCpuMmuLevel1Table tableLevel1;
    ptrPharosCpuMmuLevel2Table tableLevel2;
    ptrPharosCpuMmuLevel3Table tableLevel3;

    uint16_t index0 = calculateIndex512G(address);
    uint16_t index1 = calculateIndex1G(address , index0);
    uint16_t index2 = calculateIndex2M(address , index0 , index1);
    uint16_t index3 = calculateIndex4K(address , index0 , index1 , index2);

    /* check if descriptor 0 is invalid */
    if(descriptor0IsInvalid(table->descriptor[index0]) == TRUE)
    {
        /* it is invalid, so we must initialize a new table level 1 */

        /* add the new level 1 table with all entries invalid */
        table1LevelAdd(table , *table1Iterator , index0);

        /* set the level 1 table */
        tableLevel1 = *table1Iterator;

        /* increment the table iterator */
        table1Iterator++;
    }
    else
    {
        /* get the current table level 1 */
        tableLevel1 = getLevel1Table(table->descriptor[index0]);
    }


    /* check if the level 1 table entry is invalid */
    if(descriptor1IsInvalid(tableLevel1->descriptor[index1]) == TRUE)
    {
        /* it is invalid, so we must initialize a new table level 2 */

        /* add the new level 2 table with all entries invalid */
        table2LevelAdd(tableLevel1 , *table2Iterator , index1);

        /* set the level 1 table */
        tableLevel2 = *table2Iterator;

        /* increment the table iterator */
        table2Iterator++;
    }
    else
    {
        /* get the current table level 2 */
        tableLevel2 = getLevel2Table(tableLevel1->descriptor[index1]);
    }

    /* if the descriptor of level 2 is invalid (means that it was not used before) */
    if(descriptor2IsTable(tableLevel2->descriptor[index2]) == FALSE)
    {
        /* it is invalid, so we must initialize a new table level 2 */

        /* add the new level 3 table with all entries set to EL1 RO */
        table3LevelAddTable(tableLevel2 , *table3Iterator , index2 , address);

        /* set the level 3 table */
        tableLevel3 = *table3Iterator;

        /* increment the table iterator */
        table3Iterator++;
    }
    else
    {
        /* get the current table level 2 */
        tableLevel3 = getLevel3Table(tableLevel2->descriptor[index2]);
    }

    if(tableLevel3 != NULL)
    {
        /* then set the block entry to give I/O permissions to it */
        tableDescriptorInitBlock3Io(&tableLevel3->descriptor[index3] , address);
    }
    else
    {
        /* announce error */
        pharosErrorAnnounce(PHAROS_ERR_I0_MEM_OVERLAPPING , address);
    }
}


void table2DescriptorInitIo(ptrPharosCpuMmuLevel0Table table , ptrPharosCpuMmuLevel1Table *table1Iterator , ptrPharosCpuMmuLevel2Table *table2Iterator ,
                            uint8_t *address)
{
    ptrPharosCpuMmuLevel1Table tableLevel1;
    ptrPharosCpuMmuLevel2Table tableLevel2;

    uint16_t index0 = calculateIndex512G(address);
    uint16_t index1 = calculateIndex1G(address , index0);
    uint16_t index2 = calculateIndex2M(address , index0 , index1);

    /* check if descriptor 0 is invalid */
    if(descriptor0IsInvalid(table->descriptor[index0]) == TRUE)
    {
        /* it is invalid, so we must initialize a new table level 1 */

        /* add the new level 1 table with all entries invalid */
        table1LevelAdd(table , *table1Iterator , index0);

        /* set the level 1 table */
        tableLevel1 = *table1Iterator;

        /* increment the table iterator */
        table1Iterator++;
    }
    else
    {
        /* get the current table level 1 */
        tableLevel1 = getLevel1Table(table->descriptor[index0]);
    }

    /* check if the level 1 table entry is invalid */
    if(descriptor1IsInvalid(tableLevel1->descriptor[index1]) == TRUE)
    {
        /* it is invalid, so we must initialize a new table level 2 */

        /* add the new level 2 table with all entries invalid */
        table2LevelAdd(tableLevel1 , *table2Iterator , index1);

        /* set the level 1 table */
        tableLevel2 = *table2Iterator;

        /* increment the table iterator */
        table2Iterator++;
    }
    else
    {
        /* get the current table level 2 */
        tableLevel2 = getLevel2Table(tableLevel1->descriptor[index1]);
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


bool checkIntervalsIntersect(const uint8_t *a0 , const uint8_t *a1 , const uint8_t *b0 , const uint8_t *b1)
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


void initializeIoMemArea(ptrPharosCpuMmuLevel0Table table0 , ptrIoMemAreaTable table)
{
    /* iterator of the MMU 4K tables used (if 4KiB tables are selected) */
    ptrPharosCpuMmuLevel3Table table4KIterator = &table->mmuTables4K[0];

    ptrPharosCpuMmuLevel2Table table2MIterator = &table->mmuTables2M[0];

    ptrPharosCpuMmuLevel1Table table1GIterator = &table->mmuTables1G[0];

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
                table2DescriptorInitIo(table0 , &table1GIterator , &table2MIterator , start);
            }
        }
            /* else, the table is 4 KiB */
        else
        {
            /* get the start address */
            uint8_t * start = area->table.pages4K.start;

            for(j = 0U; j < area->table.pages4K.numberPages; j++ , start += VALUE_4KiB)
            {
                /* initialize the level 0 table descriptor with I/O permissions, adding a table level 1 if needed */
                table3DescriptorInitIo(table0 , &table1GIterator , &table2MIterator , &table4KIterator , start);
            }
        }
    }
}


void initSharedStack(ptrPharosCpuMmuLevel0Table tableLevel0 , ptrPharosCpuMmuLevel2Table *table2Iterator , ptrPharosCpuMmuLevel3Table *table3Iterator)
{
    /* iterators of the MMU areas and the 4KiB/2MiB tables */
    uint32_t i , j;

    uint8_t * stackStart = (uint8_t *) & pharosSharedStackStart;

    uintptr_t numberPages2PerPartition = (uintptr_t) & pharosNumberSharedStackMmuTables2PerPartition;
    uintptr_t numberPages3PerPartition = (uintptr_t) & pharosNumberSharedStackMmuTables3PerPartition;

    uint16_t index0 = calculateIndex512G(stackStart);
    uint16_t index1 = calculateIndex1G(stackStart , index0);

    /* get the current table level 1 */
    ptrPharosCpuMmuLevel1Table tableLevel1 = getLevel1Table(tableLevel0->descriptor[index0]);
    ptrPharosCpuMmuLevel2Table tableLevel2;

    /* initialize all tables for level 2 for the shared stack area */
    for(i = 0U; i < numberPages2PerPartition; i++)
    {
        uint16_t index2 = calculateIndex2M(stackStart , index0 , index1);

        /* check if descriptor 1 is invalid */
        if(descriptor1IsInvalid(tableLevel1->descriptor[index2]) == TRUE)
        {
            /* it is invalid, add new table 2 */
            table2LevelAdd(tableLevel1 , *table2Iterator , index1);

            /* set the level 1 table */
            tableLevel2 = *table2Iterator;

            /* increment the table iterator */
            table2Iterator++;
        }
        else
        {
            /* get the level 2 table */
            tableLevel2 = getLevel2Table(tableLevel1->descriptor[index1]);
        }

        /* for each level 3 table inside the shared stack area */
        for(j = 0U; j < numberPages3PerPartition; j++ , stackStart += VALUE_2MiB)
        {
            /* add new table 3 with permissions only for EL1 */
            table3LevelAddEl1(tableLevel2 , *table3Iterator , index2 , stackStart);

            /* increment the table iterator */
            table3Iterator++;
        }
    }
}


void pharosCpuPartitionInitialize(ptrPartition partition)
{
    /* nothing to do */
}


bool hasAnyPermission(uint8_t *address , uint64_t size , ptrEnvMemoryProtection protection)
{
    bool result;

    /* calculate end address */
    uint8_t *end = address + size;

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
        /* if inside the interrupt stack zone */
    else if(checkIntervalsIntersect(address , end , &pharosCpuInterruptStack[pharosICoreRunning()][0U] , &pharosCpuInterruptStack[pharosICoreRunning()][PHAROS_CPU_INTERRUPT_STACK_SIZE - 1U]) == TRUE)
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


bool hasPermissions(uint8_t *address , ptrEnvMemoryProtection protection)
{
    bool result;

    /* if inside the text zone */
    if((address >= &pharosStartText) && (address < &pharosEndText))
    {
        /* has some kind of permissions */
        result = TRUE;
    }
        /* if inside the environment memory zone */
    else if((address >= protection->startAddress[0]) && (address < protection->endAddress[0]))
    {
        /* has some kind of permissions */
        result = TRUE;
    }
        /* if inside the interrupt stack zone */
    else if((address >= &pharosCpuInterruptStack[pharosICoreRunning()][0U]) && (address < &pharosCpuInterruptStack[pharosICoreRunning()][PHAROS_CPU_INTERRUPT_STACK_SIZE - 1U]))
    {
        /* has some kind of permissions */
        result = TRUE;
    }
        /* if inside the shared memory zone */
    else if((address >= &pharosSharedStart) && (address < &pharosSharedEnd))
    {
        /* has some kind of permissions */
        result = TRUE;
    }
        /* if inside the inter-partition calls zone */
    else if((address >= &pharosIpCallStart) && (address < &pharosIpCallEnd))
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


bool descriptor3IsInvalid(PharosCpuMmuLevel3Descriptor descritor)
{
    return descritor.invalid.shouldBeZero == 0U ? TRUE : FALSE;
}


bool descriptor2IsBlock(PharosCpuMmuLevel2Descriptor descritor)
{
    return descritor.block.shouldBeZeroOne == 0b01U ? TRUE : FALSE;
}


bool descriptor2IsTable(PharosCpuMmuLevel2Descriptor descritor)
{
    return descritor.table.shouldBeOneOne == 0b11U ? TRUE : FALSE;
}


bool descriptor1IsInvalid(PharosCpuMmuLevel1Descriptor descritor)
{
    return descritor.invalid.shouldBeZero == 0U ? TRUE : FALSE;
}


bool descriptor0IsInvalid(PharosCpuMmuLevel0Descriptor descritor)
{
    return descritor.invalid.shouldBeZero == 0U ? TRUE : FALSE;
}


void pharosCpuAddSharedStack(ptrThreadInfo thread , ptrPartitionCpuMmuContext context)
{
    sharedStackChangePermissions(thread->sharedAccessStack , context , AP_EL1_EL0_RW);
}


void pharosCpuRemoveSharedStack(ptrThreadInfo thread , ptrPartitionCpuMmuContext context)
{
    sharedStackChangePermissions(thread->sharedAccessStack , context , AP_EL1_RW);
}


ptrPharosCpuMmuLevel3Descriptor getTableLevel3Descriptor(ptrPharosCpuMmuLevel0Table tableLevel0 , uint8_t * address)
{
    uint16_t index0 = calculateIndex512G(address);
    uint16_t index1 = calculateIndex1G(address , index0);
    uint16_t index2 = calculateIndex2M(address , index0 , index1);
    uint16_t index3 = calculateIndex4K(address , index0 , index1 , index2);

    ptrPharosCpuMmuLevel1Table tableLevel1 = getLevel1Table(tableLevel0->descriptor[index0]);
    ptrPharosCpuMmuLevel2Table tableLevel2 = getLevel2Table(tableLevel1->descriptor[index1]);
    ptrPharosCpuMmuLevel3Table tableLevel3 = getLevel3Table(tableLevel2->descriptor[index2]);

    return &tableLevel3->descriptor[index3];
}


void change4KSectionPermissions(ptrPharosCpuMmuLevel3Descriptor descriptor , uint8_t ap)
{
    /* set the permissions */
    descriptor->descriptor.ap = ap;
}


void change2MSectionPermissions(ptrPharosCpuMmuLevel2Descriptor descriptor , uint8_t ap)
{
    descriptor->block.ap = ap;
}


void add2MSectionPermissionsAddress(ptrPharosCpuMmuLevel2Descriptor descriptor , uint8_t * address)
{
    /* set the permissions */
    descriptor->block.ap = AP_EL1_EL0_RW;

    /* fill the address (AKA real-addressing) */
    descriptor->block.address = ((uintptr_t) address) >> 12;
}


void sharedStackChangePermissions(ptrStackShared sharedStack , ptrPartitionCpuMmuContext context , uint8_t apValue)
{
    /* iterator for the 4KiB sections of the shared stack */
    uint32_t i;

    /* address to change the permissions */
    uint8_t *address = sharedStack->stack.stack;

    /* descriptor to change the permissions */
    ptrPharosCpuMmuLevel3Descriptor descriptor;

    /* get the context level 0 table */
    ptrPharosCpuMmuLevel0Table tableLevel0 = context->level0Table;

    /* calculate number of sections of 4 KiB are used by the shared stack */
    uint32_t numberDescriptors = (uint32_t) (sharedStack->stack.size / VALUE_4KiB);


    /* add all needed descriptors */
    for(i = 0U; i < numberDescriptors; i++)
    {
        /* get the 4K table for the address iterator */
        descriptor = getTableLevel3Descriptor(tableLevel0 , address);

        /* add permissions for the 4K descriptor */
        change4KSectionPermissions(descriptor , apValue);

        /* invalidate the TLB for the specified address */
        pharosCpuMmuInvalidateAddress(address);

        /* update the address to the next descriptor */
        address += VALUE_4KiB;
    }
}


ptrPharosCpuMmuLevel2Table linkLevel2Table(ptrPharosCpuMmuLevel1Table table1 , uint16_t index1 , ptrPharosCpuMmuLevel2Table table2 , uint8_t * address)
{
    uint16_t i;

    /* copy the address on the previous table */
    table1->descriptor[index1].table.address = ((uintptr_t) table2) >> 12;

    /* initialize the table 1 property to point to a table 2 */
    table1->descriptor[index1].table.shouldBeOneOne = 0b11;

    /* always secure */
    table1->descriptor[index1].table.nsTable = NS_TABLE_SECURE_YES;

    /* ignore the permissions set by this table, the following table will contain the correct permissions */
    table1->descriptor[index1].table.apTable = AP_TABLE_IGNORE;

    /* ignore the execute flag on this table, the following table will contain the correct flag */
    table1->descriptor[index1].table.uxn_xnTable = XN_TABLE_IGNORE;

    /* ignore the execute flag on this table, the following table will contain the correct flag */
    table1->descriptor[index1].table.pxnTable = PXN_TABLE_IGNORE;

    /* align the address to the previous 512 MiB */
    address = (uint8_t *) ((uintptr_t) address & ~((uintptr_t) VALUE_512MiB - 1U));

    /* go through the 512MiB of memory, starting at address and going 2 MiB at a time */
    for(i = 0; i < PHAROS_CPU_LEVEL2_NUMBER_ENTRIES; i++ , address += VALUE_2MiB)
    {
        /* mark the block as invalid */
        table2->descriptor[i].invalid.shouldBeZero = 0;
    }

    return table2;
}


ptrPharosCpuMmuLevel3Table linkLevel3Table(ptrPharosCpuMmuLevel2Table table2 , uint16_t index2 , ptrPharosCpuMmuLevel3Table table3 , uint8_t * address)
{
    uint16_t i;

    /* copy the address on the previous table */
    table2->descriptor[index2].table.address = ((uintptr_t) table3) >> 12;

    /* initialize the table 1 property to point to a table 2 */
    table2->descriptor[index2].table.shouldBeOneOne = 0b11;

    /* always secure */
    table2->descriptor[index2].table.nsTable = NS_TABLE_SECURE_YES;

    /* ignore the permissions set by this table, the following table will contain the correct permissions */
    table2->descriptor[index2].table.apTable = AP_TABLE_IGNORE;

    /* ignore the execute flag on this table, the following table will contain the correct flag */
    table2->descriptor[index2].table.uxn_xnTable = XN_TABLE_IGNORE;

    /* ignore the execute flag on this table, the following table will contain the correct flag */
    table2->descriptor[index2].table.pxnTable = PXN_TABLE_IGNORE;

    /* align the address to the previous 2 MiB */
    address = (uint8_t *) ((uintptr_t) address & ~((uintptr_t) VALUE_2MiB - 1U));

    /* go through the 2MiB of memory, starting at address and going 4 KiB at a time */
    for(i = 0; i < PHAROS_CPU_LEVEL3_NUMBER_ENTRIES; i++ , address += VALUE_4KiB)
    {
        /* fill the address (AKA real-addressing) */
        table3->descriptor[i].descriptor.address = ((uintptr_t) address) >> 12;

        /* place the 0's */
        table3->descriptor[i].descriptor.shouldBeZero = 0b0000;

        /* place the 11 to state it is a table descriptor */
        table3->descriptor[i].descriptor.shouldBeOneOne = 0b11;

        /* set not contiguous */
        table3->descriptor[i].descriptor.contiguous = CONTIGUOUS_NO;

        /* always secure */
        table3->descriptor[i].descriptor.ns = SECURE_YES;

        /* not shareable */
        table3->descriptor[i].descriptor.sh = SHAREABLE_NO;

        /* not global */
        table3->descriptor[i].descriptor.ng = GLOBAL_NO;

        /* initialize the access flag */
        table3->descriptor[i].descriptor.af = ACCESS_INITIALIZE;

        /* initialize reserved with 0's */
        table3->descriptor[i].descriptor.reserved = 0U;

        /* reset ignored bits */
        table3->descriptor[i].descriptor.ignored = 0U;

        /* does not have any permissions */

        /* set to memory type to "normal" */
        table3->descriptor[i].descriptor.attrIndx = MAIR_EL1_ATTRIBUTE_MEMORY_INDEX;

        /* only EL1 can read and write */
        table3->descriptor[i].descriptor.ap = AP_EL1_RW;

        /* not execute */
        table3->descriptor[i].descriptor.pxn = EXECUTE_NEVER;

        /* not execute */
        table3->descriptor[i].descriptor.uxn_xn = EXECUTE_NEVER;
    }

    return table3;
}


void initializeTable3Invalid(ptrPharosCpuMmuLevel3Table table3)
{
    uint16_t i;


    /* go through the 1 GiB of memory, starting at address and going 2 MiB at a time */
    for(i = 0; i < PHAROS_CPU_LEVEL2_NUMBER_ENTRIES; i++)
    {
        /* place the 0's */
        table3->descriptor[i].descriptor.shouldBeZero = 0b0000;

        /* place the 11 to state it is a block descriptor */
        table3->descriptor[i].descriptor.shouldBeOneOne = 0b11;

        /* set not contiguous */
        table3->descriptor[i].descriptor.contiguous = CONTIGUOUS_NO;

        /* always secure */
        table3->descriptor[i].descriptor.ns = SECURE_YES;

        /* not shareable */
        table3->descriptor[i].descriptor.sh = SHAREABLE_NO;

        /* not global */
        table3->descriptor[i].descriptor.ng = GLOBAL_NO;

        /* initialize the access flag */
        table3->descriptor[i].descriptor.af = ACCESS_INITIALIZE;

        /* initialize reserved with 0's */
        table3->descriptor[i].descriptor.reserved = 0U;

        /* reset ignored bits */
        table3->descriptor[i].descriptor.ignored = 0U;

        /* does not have any permissions */

        /* set to memory type to "normal" */
        table3->descriptor[i].descriptor.attrIndx = MAIR_EL1_ATTRIBUTE_MEMORY_INDEX;

        /* only EL1 can read and write */
        table3->descriptor[i].descriptor.ap = AP_EL1_RW;

        /* not execute */
        table3->descriptor[i].descriptor.pxn = EXECUTE_NEVER;

        /* not execute */
        table3->descriptor[i].descriptor.uxn_xn = EXECUTE_NEVER;
    }
}


void initializeTable2Invalid(ptrPharosCpuMmuLevel2Table table2)
{
    uint16_t i;


    /* go through the 1 GiB of memory, starting at address and going 2 MiB at a time */
    for(i = 0; i < PHAROS_CPU_LEVEL2_NUMBER_ENTRIES; i++)
    {
        /* place the 1 to state it is a block descriptor */
        table2->descriptor[i].block.shouldBeZeroOne = 0b01;

        /* set not contiguous */
        table2->descriptor[i].block.contiguous = CONTIGUOUS_NO;

        /* always secure */
        table2->descriptor[i].block.ns = SECURE_YES;

        /* not shareable */
        table2->descriptor[i].block.sh = SHAREABLE_NO;

        /* not global */
        table2->descriptor[i].block.ng = GLOBAL_NO;

        /* initialize the access flag */
        table2->descriptor[i].block.af = ACCESS_INITIALIZE;

        /* initialize reserved with 0's */
        table2->descriptor[i].block.reserved = 0U;

        /* reset ignored bits */
        table2->descriptor[i].block.ignored = 0U;

        /* does not have any permissions */

        /* set to memory type to "normal" */
        table2->descriptor[i].block.attrIndx = MAIR_EL1_ATTRIBUTE_MEMORY_INDEX;

        /* only EL1 can read and write */
        table2->descriptor[i].block.ap = AP_EL1_RW;

        /* not execute */
        table2->descriptor[i].block.pxn = EXECUTE_NEVER;

        /* not execute */
        table2->descriptor[i].block.uxn_xn = EXECUTE_NEVER;
    }
}


void remove4KBulk(ptrPharosCpuMmuLevel1Table table1 , ptrChannelBulkQueue queue , uint8_t * address)
{
    uint16_t index0 = calculateIndex512G(address);
    uint16_t index1 = calculateIndex1G(address , index0);
    uint16_t index2 = calculateIndex2M(address , index0 , index1);
    uint16_t index3 = calculateIndex4K(address , index0 , index1 , index2);

    ptrPharosCpuMmuLevel2Table table2;
    ptrPharosCpuMmuLevel3Table table3;

    /* get the current MMU table level 2 */
    table2 = getLevel2Table(table1->descriptor[index1]);

    /* now we have a table 2, get the table 3 */
    table3 = getLevel3Table(table2->descriptor[index2]);

    /* remove permissions to the table*/
    change4KSectionPermissions(&table3->descriptor[index3] , AP_EL1_RW);

    /* invalidate the MMU address so that the new value is placed on the TLB cache */
    pharosCpuMmuInvalidateAddress(address);
}


void remove64KBulk(ptrPharosCpuMmuLevel1Table table1 , ptrChannelBulkQueue queue , uint8_t * address)
{
    /* iterator for each of the 4K pages inside the 64K bulk */
    uint32_t i;

    /* for each 4K page */
    for(i = 0; i < VALUE_64KiB / VALUE_4KiB; i++ , address += VALUE_4KiB)
    {
        /* remove the 4KiB page */
        remove4KBulk(table1 , queue , address);
    }
}


void remove2MBulk(ptrPharosCpuMmuLevel1Table table1 , ptrChannelBulkQueue queue , uint8_t * address)
{
    ptrPharosCpuMmuLevel2Table table2;

    uint16_t index0 = calculateIndex512G(address);
    uint16_t index1 = calculateIndex1G(address , index0);
    uint16_t index2 = calculateIndex2M(address , index0 , index1);

    /* get the current MMU table level 2 */
    table2 = getLevel2Table(table1->descriptor[index1]);

    /* remove permissions to the table*/
    change2MSectionPermissions(&table2->descriptor[index2] , AP_EL1_RW);
}


uint32_t add4KBulk(ptrPharosCpuMmuLevel1Table table1 , ptrChannelBulkQueue queue , uint8_t * address , ptrPartition partition)
{
    /* interrupt level */
    PharosInterruptLevel level;

    /* get the partition core */
    CoreNumber core = partition->core->number;

    uint16_t index0 = calculateIndex512G(address);
    uint16_t index1 = calculateIndex1G(address , index0);
    uint16_t index2 = calculateIndex2M(address , index0 , index1);
    uint16_t index3 = calculateIndex4K(address , index0 , index1 , index2);

    ptrPharosCpuMmuLevel2Table table2;
    ptrPharosCpuMmuLevel3Table table3;

    /* get the current MMU table level 2 */
    table2 = getLevel2Table(table1->descriptor[index1]);

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

            /* increment the iterator of the level 2 tables */
            queue->cpuInfo.level2Pages[core]++;

            /* decrease the number of available level 2 tables */
            queue->cpuInfo.numberLevel2Tables[core]--;

            /* re-enable interrupts*/
            pharosCpuEnableInterrupts(&level);
        }
        else
        {
            /* re-enable interrupts*/
            pharosCpuEnableInterrupts(&level);

            /* return error */
            return 0;
        }
    }

    /* now we have a table 2, get the table 3 */
    table3 = getLevel3Table(table2->descriptor[index2]);

    /* check that the table exists */
    if(table3 == NULL)
    {
        /* disable interrupts we can safely allocate a level 2 table */
        pharosCpuDisableInterrupts(&level);

        /* if there are enough tables to allocate */
        if(queue->cpuInfo.numberLevel3Tables[core] > 0U)
        {
            /* add the level 3 table to the level 2 table */
            table3 = linkLevel3Table(table2 , index2 , queue->cpuInfo.level3Pages[core] , address);

            /* increment the iterator of the level 3 tables */
            queue->cpuInfo.level3Pages[core]++;

            /* decrease the number of available level 3 tables */
            queue->cpuInfo.numberLevel3Tables[core]--;

            /* re-enable interrupts*/
            pharosCpuEnableInterrupts(&level);
        }
        else
        {
            /* re-enable interrupts*/
            pharosCpuEnableInterrupts(&level);

            /* return error */
            return 0;
        }
    }

    /* add permissions to the level 3 table and set the address on the descriptor */
    change4KSectionPermissions(&table3->descriptor[index3] , AP_EL1_EL0_RW);
    pharosCpuMmuInvalidateAddress(address);

    return VALUE_4KiB;
}


uint32_t add64KBulk(ptrPharosCpuMmuLevel1Table table1 , ptrChannelBulkQueue queue , uint8_t * address , ptrPartition partition)
{
    /* iterator for each of the 4K pages inside the 64K bulk */
    uint32_t i;

    /* initialize the result with success value */
    uint32_t result = VALUE_64KiB;

    /* for each 4K page */
    for(i = 0; i < VALUE_64KiB / VALUE_4KiB; i++ , address += VALUE_4KiB)
    {
        /* add the page */
        if(add4KBulk(table1 , queue , address , partition) == 0U)
        {
            /* set the result to invalid */
            result = 0;

            /* stop adding more tables */
            break;
        }
    }

    /* return the 64KiB or 0 if could not add MMU tables */
    return result;
}


uint32_t add2MBulk(ptrPharosCpuMmuLevel1Table table1 , ptrChannelBulkQueue queue , uint8_t * address , ptrPartition partition)
{
    /* level 2 MMU table */
    ptrPharosCpuMmuLevel2Table table2;

    /* get the partition core */
    CoreNumber core = partition->core->number;

    uint32_t result = VALUE_2MiB;

    uint16_t index0 = calculateIndex512G(address);
    uint16_t index1 = calculateIndex1G(address , index0);
    uint16_t index2 = calculateIndex2M(address , index0 , index1);


    /* get the current MMU table level 2 */
    table2 = getLevel2Table(table1->descriptor[index1]);

    /* check that the table exists */
    if(table2 == NULL)
    {
        /* if there are enough tables to allocate */
        if(queue->cpuInfo.numberLevel2Tables[core] > 0U)
        {
            /* add the level 2 table to the level 1 table */
            table2 = linkLevel2Table(table1 , index1 , queue->cpuInfo.level2Pages[core] , address);

            /* decrease the number of available level 3 tables */
            queue->cpuInfo.numberLevel2Tables[core]--;

            /* give permissions to the descriptor */
            add2MSectionPermissionsAddress(&table2->descriptor[index2] , address);
        }
        else
        {
            /* announce error */
            pharosSErrorAnnounce(PHAROS_CPU_ERROR_CHANNEL_LEVEL2_TABLES_EMPTY , NULL);

            /* return error */
            result = 0;
        }
    }
    else
    {
        /* give permissions to the descriptor */
        add2MSectionPermissionsAddress(&table2->descriptor[index2] , address);
    }

    /* return 0 if could not add the tables or 2MiB if successful */
    return result;
}


uint32_t pharosCpuChannelBulkAdd(ptrChannelBulkQueue queue , ptrChannelBulkKernel bulk , ptrPartition partition)
{
    /* result of the function */
    uint32_t result;

    /* get the running thread MMU level 0 table */
    ptrPharosCpuMmuLevel0Table table0 = partition->context.level0Table;

    /* get the MMU level 1 table */
    ptrPharosCpuMmuLevel1Table table1;

    /* index of the level 0 table */
    uint16_t index0 = calculateIndex512G(bulk->bulkFullBuffer.buffer);


    /* get the level 1 table */
    table1 = getLevel1Table(table0->descriptor[index0]);

    /* for a channel bulk with 4K size */
    if(queue->cpuInfo.type == PHAROS_CHANNEL_BULK_QUEUE_4K)
    {
        /* add the 4K bulk */
        result = add4KBulk(table1 , queue , bulk->bulkFullBuffer.buffer , partition);
    }
        /* for a channel bulk with 64K size */
    else if(queue->cpuInfo.type == PHAROS_CHANNEL_BULK_QUEUE_64K)
    {
        /* add the 64K bulk */
        result = add64KBulk(table1 , queue , bulk->bulkFullBuffer.buffer , partition);
    }
        /* else, it is PHAROS_CHANNEL_BULK_QUEUE_2M -> 2 MiB */
    else
    {
        /* add the 2M bulk */
        result = add2MBulk(table1 , queue , bulk->bulkFullBuffer.buffer , partition);
    }

    /* return the page size added or 0 if impossible to add */
    return result;
}


void pharosCpuChannelBulkRemove(ptrChannelBulkKernel bulk , ptrPartition partition)
{
    /* get the MMU level 1 table */
    ptrPharosCpuMmuLevel1Table table1;

    /* get the running thread MMU level 0 table */
    ptrPharosCpuMmuLevel0Table table0 = partition->context.level0Table;

    /* calculate the index 0 on the 512 GiB table */
    uint16_t index0 = calculateIndex512G(bulk->bufferMessage.buffer);


    /* get the table given the index on table 0 */
    table1 = getLevel1Table(table0->descriptor[index0]);

    /* for a channel bulk with 4K size */
    if(bulk->queue->cpuInfo.type == PHAROS_CHANNEL_BULK_QUEUE_4K)
    {
        /* add the 4K bulk */
        remove4KBulk(table1 , bulk->queue , bulk->bufferMessage.buffer);
    }
        /* for a channel bulk with 64K size */
    else if(bulk->queue->cpuInfo.type == PHAROS_CHANNEL_BULK_QUEUE_64K)
    {
        /* add the 64K bulk */
        remove64KBulk(table1 , bulk->queue , bulk->bufferMessage.buffer);
    }
        /* else, it is PHAROS_CHANNEL_BULK_QUEUE_2M -> 2 MiB */
    else
    {
        /* add the 2M bulk */
        remove2MBulk(table1 , bulk->queue , bulk->bufferMessage.buffer);
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
        /* for each level 2 table */
        for(i = 0; i < queue->cpuInfo.numberLevel2Tables[core]; i++)
        {
            /* get the l2 table for this core */
            ptrPharosCpuMmuLevel2Table l2 = queue->cpuInfo.level2Pages[core];

            /* initialize the table with memory configuration but do not initialize the address */
            initializeTable2Invalid(&l2[i]);
        }

        /* for each level 3 table */
        for(i = 0; i < queue->cpuInfo.numberLevel3Tables[core]; i++)
        {
            /* get the l3 table for this core */
            ptrPharosCpuMmuLevel3Table l3 = queue->cpuInfo.level3Pages[core];

            /* initialize the table with memory configuration but do not initialize the address */
            initializeTable3Invalid(&l3[i]);
        }
    }

    /* initialize the size */
    *bulkSize = queue->cpuInfo.type;

    /* return the address of the first page */
    return (uint8_t *) queue->cpuInfo.pages.page4K;
}


ptrPharosCpuMmuLevel0Table pharosCpuMmuGetTlb(ptrPartition partition)
{
    /* if no partition is set */
    if(partition == NULL)
    {
        /* then return the kernel TTRB */
        return &kernelTable0Table[pharosICoreRunning()];
    }
        /* if a partition is set */
    else
    {
        /* return the partition tlb */
        return partition->context.level0Table;
    }
}
