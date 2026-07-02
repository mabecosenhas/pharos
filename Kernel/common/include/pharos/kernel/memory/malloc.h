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


#ifndef PHAROS_KERNEL_MALLOC_H
#define PHAROS_KERNEL_MALLOC_H

#include <pharos/kernel/memory/declarations.h>

/**
 * Macro that determines the next power of 2 of the available space. For example, if
 *   PHAROS_DYNAMIC_MEMORY_SIZE(2KiB)   = 2KiB
 *   PHAROS_DYNAMIC_MEMORY_SIZE(1.5KiB) = 2KiB
 *   PHAROS_DYNAMIC_MEMORY_SIZE(1.1KiB) = 2KiB
 *   PHAROS_DYNAMIC_MEMORY_SIZE(1KiB)   = 1KiB
 */
#define PHAROS_DYNAMIC_MEMORY_SIZE(availableSpace)                  ((uint64_t)(PHAROS_DYNAMIC_MEMORY_C(availableSpace) == 2U ? PHAROS_DYNAMIC_MEMORY_L(availableSpace) * 2U : PHAROS_DYNAMIC_MEMORY_L(availableSpace)))

#define PHAROS_DYNAMIC_MEMORY_BLOCK_SIZE(availableSpace, minSize)   (2U * PHAROS_DYNAMIC_MEMORY_SIZE(availableSpace) / minSize - 1)

#define PHAROS_DYNAMIC_MEMORY_L(availableSpace)                     (1 << (PHAROS_LOG2(availableSpace)))
#define PHAROS_DYNAMIC_MEMORY_C(availableSpace)                     (PHAROS_CEILING(availableSpace , PHAROS_DYNAMIC_MEMORY_L(availableSpace)))

struct PharosDynamicMemory
{
    /**
     * Pointer to the start of the memory area
     * 
     * note: application should initialize this with the address of a memory area with size of at least the totalSize
     */
    uint8_t *memory;

    /**
     * total size of the memory area
     * 
     * note: application should initialize this with the size (in bytes) of the memory area to be used
     */
    uint32_t availableSize;

    /**
     * minimum size of each block (minimum size that can be allocated)
     * 
     * note: application should initialize this with the size (in bytes) of the minimum amount of memory of a block.
     * This information is relevant since a small mininum size requires more information to keep of which memory blocks 
     * are free and which are not (see PHAROS_DYNAMIC_MEMORY_BLOCK_SIZE macro)
     */
    uint32_t minSize;

    /**
     * Array where the information of which memory blocks are allocated and the size of each block
     * 
     * note: application should initialize this with a vector in the KERNEL_SECTION and with space using the macro
     * PHAROS_DYNAMIC_MEMORY_SIZE(totalSize, minSize). 
     * 
     * Example:
     * 
     * PARTITION_0_SECTION_USS uint8_t memory[1.5 * VALUE_2MiB]; 
     * KERNEL_SECTION blocks[PHAROS_DYNAMIC_MEMORY_BLOCK_SIZE(sizeof(memory) , 32)];
     * .
     * .
     * .
     * .blocks = blocks,
     */
    uint8_t *blocks;

    /**
     * Number of bytes in the blocks field
     * 
     * note: application should initialize this value with PHAROS_DYNAMIC_MEMORY_BLOCK_SIZE macro (or the sizeof macro)
     * 
     * Example:
     * 
     * PARTITION_0_SECTION_USS uint8_t memory[1.5 * VALUE_2MiB]; 
     * KERNEL_SECTION blocks[PHAROS_DYNAMIC_MEMORY_BLOCK_SIZE(sizeof(memory) , 32)];
     * .
     * .
     * .
     * .numberBlocks = sizeof(blocks),
     */
    uint32_t numberBlocks;

    /**
     * total size of the number of blocks
     * 
     * note: application should initialize this with the PHAROS_DYNAMIC_MEMORY_SIZE macro
     * 
     * Example:
     * 
     * PARTITION_0_SECTION_USS uint8_t memory[1.5 * VALUE_2MiB]; 
     * .
     * .
     * .
     * .totalSize = PHAROS_DYNAMIC_MEMORY_SIZE(sizeof(memory)),
     */
    uint32_t totalSize;

    uint32_t minSizeLog2;
    uint32_t levels;
};


void *pharosSMalloc(uint64_t size);

uint64_t pharosFreeS(void *pointer);

void *pharosIMallocKernel(uint64_t size);

uint64_t pharosIFreeKernel(void *pointer);

void *pharosIMallocSharedStack(uint64_t size);

uint64_t pharosIFreeSharedStack(void *address);

void pharosIDynamicMemoryInitialize(ptrPharosDynamicMemory area);


/* PHAROS_KERNEL_MALLOC_H */
#endif 
