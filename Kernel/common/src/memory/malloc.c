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


#include <pharos/kernel/memory/memory.h>
#include <pharos/kernel/memory/malloc.h>
#include <pharos/kernel/core/core.h>
#include <pharos/kernel/assert/assert.h>


#define PHAROS_DYNAMIC_MEMORY_UNAVAILABLE       (0xffU)


static uint8_t * blockAddress(ptrPharosDynamicMemory area , uint32_t index);
static void updateSizeParents(ptrPharosDynamicMemory area , uint32_t index);


/**
 * find the memory block associated with the address
 * 
 * @param area dynamic area
 * @param address address to get the block from 
 * @param level address where to write the level on the tree
 * @param size address where to write the size 
 * 
 * @return returns the block index
 */
static int32_t findBlock(ptrPharosDynamicMemory area , uint8_t *address , uint32_t *level , uint64_t *size);


/**
 * Dynamically allocated the memory area with the minimum size
 * 
 * @param size size to allocate
 * @param area dynamic area
 * 
 * @return returns the pointer to the dynamic memory or NULL if no free space exists
 */
static void *pharosIMalloc(uint64_t size , ptrPharosDynamicMemory area);

/**
 * Frees the allocated memory
 * 
 * @param address address that was previously allocated 
 * @param area dynamic area
 * 
 * @return returns the number of bytes free (or 0 if none)
 */
static uint64_t pharosIFree(void *address , ptrPharosDynamicMemory area);


/**
 * Determines if x is a power of 2
 * 
 * @param x value to determine 
 * 
 * @return returns TRUE if it is a power of 2 and FALSE otherwise
 */
INLINE bool isPowerOfTwo(uintptr_t x)
{
    /* check if x is different than 0 and if (x) and (x-1) are zero (they should be zero if x is a power of 2) */
    return x && (!(x & (x - 1)));
}


/**
 * Determine the next power of 2 of a number
 * 
 * @param number number
 * 
 * @return returns the next power of 2 of the number
 */
INLINE uint32_t nextPower2(uint32_t number)
{
    /* calculate the next power of 2 of the number */
    return 1 << ((32 - __builtin_clz(number - 1)));
}


/**
 * Get the parent index 
 * 
 * @param index index to get the parent of
 * 
 * @return returns the parent index or -1 if no parent is available
 */
INLINE int32_t parent(uint32_t index)
{
    if(index == 0U)
    {
        return -1;
    }
    else
    {
        return (index - 1) / 2;
    }
}


/**
 * get the left child of a node
 * 
 * @param area dynamic area 
 * @param index index to get the child from 
 * 
 * @return returns the index of the left child
 */
INLINE int32_t leftChild(ptrPharosDynamicMemory area , uint32_t index)
{
    index = index * 2U + 1U;

    if(index > area->numberBlocks)
    {
        return -1;
    }
    else
    {
        return index;
    }
}


/**
 * get the right child of a node
 * 
 * @param area dynamic area 
 * @param index index to get the child from 
 * 
 * @return returns the index of the right child
 */
INLINE int32_t rightChild(ptrPharosDynamicMemory area , uint32_t index)
{
    index = index * 2U + 2U;

    if(index > area->numberBlocks)
    {
        return -1;
    }
    else
    {
        return index;
    }
}


/**
 * Gets the sibling of the index
 * 
 * @param index index to get the sibling from
 * 
 * @return returns the index of the sibling
 */
int32_t sibling(uint32_t index)
{
    if(index == 0U)
    {
        return -1;
    }
    else
    {
        return ((index - 1) ^ 1) + 1;
    }
}


/**
 * Get the smallest size bigger than the specified size
 * 
 * @param area dynamic area
 * @param size minimum size
 * 
 * @return returns the minimum size that will be allocated
 */
INLINE uint32_t findSmallestSize(ptrPharosDynamicMemory area , uint32_t size)
{
    /* return the next power or 2 of the size or the minimum size */
    return MAX(area->minSize , 1U << pharosUint32Log2Ceil(size));
}


void pharosIDynamicMemoryInitialize(ptrPharosDynamicMemory area)
{
    /* block iterator */
    uint32_t i;


    /* only initialize if there is some size */
    if(area->availableSize != 0U)
    {
        area->minSizeLog2 = pharosUint32Log2Ceil(area->minSize);

        area->blocks[0] = 1 + pharosUint32Log2Ceil(area->totalSize / area->minSize);

        area->levels = area->blocks[0];

        for(i = 1; i < area->numberBlocks; i++)
        {
            area->blocks[i] = area->blocks[parent(i)] - 1;
        }

        /* find the index of the blocks that start to be unavailable */
        int index = (((uint64_t) area->availableSize * (uint64_t) area->totalSize) / area->minSize) / area->totalSize;

        /* offset of half the memory blocks */
        uint32_t offset = area->totalSize / area->minSize - 1U;

        /* go to every leaf block that is outside of the memory */
        for(i = offset + index; i < area->numberBlocks; i++)
        {
            /* set the block as unavailable */
            area->blocks[i] = PHAROS_DYNAMIC_MEMORY_UNAVAILABLE;
        }

        /* go to every leaf block that is outside of the memory */
        for(i = offset + index; i < area->numberBlocks; i++)
        {
            /* and update its parent size */
            updateSizeParents(area , i);
        }
    }
}


void *pharosIMallocKernel(uint64_t size)
{
    /* check there is a running partition */
    PHAROS_ASSERT(pharosIPartitionGetRunning() != NULL , "Invalid partition");

    /* allocated memory area using the user dynamic memory of the running partition */
    return pharosIMalloc(size , &(pharosIPartitionGetRunning()->kernelDynamicMemory));
}


void *pharosIMallocSharedStack(uint64_t size)
{
    /* check there is a running partition */
    PHAROS_ASSERT(pharosIPartitionGetRunning() != NULL , "Invalid partition");

    /* allocated memory area using the user dynamic memory of the running partition */
    return pharosIMalloc(size , &(pharosIPartitionGetRunning()->sharedStackDynamicMemory));
}


void *pharosSMalloc(uint64_t size)
{
    /* check there is a running partition */
    PHAROS_ASSERT(pharosIPartitionGetRunning() != NULL , "Invalid partition");

    /* allocated memory area using the user dynamic memory of the running partition */
    return pharosIMalloc(size , &(pharosIPartitionGetRunning()->dynamicMemory));
}


uint64_t pharosIFreeKernel(void *address)
{
    /* check there is a running partition */
    PHAROS_ASSERT(pharosIPartitionGetRunning() != NULL , "Invalid partition");

    return pharosIFree(address , &(pharosIPartitionGetRunning()->kernelDynamicMemory));
}


uint64_t pharosIFreeSharedStack(void *address)
{
    /* check there is a running partition */
    PHAROS_ASSERT(pharosIPartitionGetRunning() != NULL , "Invalid partition");

    /* free the shared stack */
    return pharosIFree(address , &(pharosIPartitionGetRunning()->sharedStackDynamicMemory));
}


uint64_t pharosFreeS(void *address)
{
    /* check there is a running partition */
    PHAROS_ASSERT(pharosIPartitionGetRunning() != NULL , "Invalid partition");

    return pharosIFree(address , &(pharosIPartitionGetRunning()->dynamicMemory));
}


uint64_t pharosIFree(void *address , ptrPharosDynamicMemory area)
{
    /* interrupt level */
    PharosInterruptLevel interrupt;

    /* level of the block */
    uint32_t level;

    /* return value (number of bytes free) */
    uint64_t size;

    /* cast the pointer */
    uint8_t *a = (uint8_t *) address;


    /* if the address is outside the memory area */
    if(a < area->memory || a > area->memory + area->availableSize)
    {
        /* return nothing is free */
        size = 0U;
    }
    else
    {
        /* find the memory block associated with the address */
        int32_t index = findBlock(area , a , &level , &size);

        /* if could not find the block (address not aligned) */
        if(index < 0)
        {
            /* return invalid memory */
            size = 0U;
        }
        else
        {
            /* disable interrupts so nothing can change the value of the global variables */
            pharosCpuDisableInterrupts(&interrupt);

            /* reset the block with the initial level */
            area->blocks[index] = level;

            /* update the parents size */
            updateSizeParents(area , index);

            /* re-enable interrupts*/
            pharosCpuEnableInterrupts(&interrupt);
        }
    }

    /* return the number of bytes free */
    return size;
}


void *pharosIMalloc(uint64_t size , ptrPharosDynamicMemory area)
{
    /* interrupt level */
    PharosInterruptLevel interrupt;

    /* result of the function */
    void *result = NULL;

    /* iterator of the blocks */
    int i = 0;

    /* get the smallest size */
    uint32_t smallestSize = pharosUint32Log2Ceil(findSmallestSize(area , size)) - pharosUint32Log2Ceil(area->minSize) + 1U;

    /* get the level iterator */
    uint32_t level = area->levels;


    /* if there is no defined memory */
    if(area->memory == NULL)
    {
        result = NULL;
    }
    else
    {
        /* disable interrupts so nothing can change the value of the global variables */
        pharosCpuDisableInterrupts(&interrupt);

        /* check if asking too much memory */
        if(smallestSize > area->blocks[0])
        {
            /* re-enable interrupts*/
            pharosCpuEnableInterrupts(&interrupt);

            /* then we cannot allocate the desired memory (block[0] has the maximum amount of free memory) */
            result = NULL;
        }
        else
        {
            while(result == NULL)
            {
                /* check if we reached the smallest size */
                if(level == smallestSize)
                {
                    /* we have found the block with appropriate size, set it as used */
                    area->blocks[i] = 0;

                    /* update the size of the parents */
                    updateSizeParents(area , i);

                    /* re-enable interrupts*/
                    pharosCpuEnableInterrupts(&interrupt);

                    /* return the address */
                    result = blockAddress(area , i);

                    break;
                }
                    /* the index has a too big space, search inside a child */
                else
                {
                    /* get the index of the left child */
                    int32_t left = leftChild(area , i);

                    /* get the right child */
                    int32_t right = rightChild(area , i);

                    /* if the left child has enough space or the right does not have enough space */
                    if((area->blocks[left] >= smallestSize && area->blocks[left] <= area->blocks[right]) || area->blocks[right] < smallestSize)
                    {
                        /* search the left child on the next iteration */
                        i = left;
                    }
                    else
                    {
                        i = right;
                    }

                    /* decrease the level that we are in */
                    level--;
                }
            }
        }
    }

    /* return NULL if not able to allocate or the address of the newly allocated memory area */
    return result;
}


int32_t findBlock(ptrPharosDynamicMemory area , uint8_t *address , uint32_t *level , uint64_t *size)
{
    /* offset of the array of blocks = 2 ^(N-1) - 1 to the minimum blocks (they start at the middle of the array) */
    uint32_t offset = area->totalSize / area->minSize - 1;

    /* get the index for the specified address on the array of the minimum blocks */
    uint32_t minIndexArray = (address - area->memory) >> area->minSizeLog2;

    /* get the minimum block to the corresponding address */
    int32_t blockIndex = offset + minIndexArray;


    /* initialize the level */
    *level = 1;

    while(blockIndex >= 0 && area->blocks[blockIndex] != 0)
    {
        /* search the parent */
        blockIndex = parent(blockIndex);

        (*level)++;
    }

    /* if we found the block index */
    if(blockIndex >= 0)
    {
        /* let's calculate to see if the address is where it is supposed to be */

        /* check the align requirements for the level */
        int32_t align = ((*level) << area->minSizeLog2) - 1;

        /* check if the address minus the start of the memory are aligned */
        if(((address - area->memory) & align) != 0)
        {
            /* if not aligned then this is an invalid address */
            blockIndex = -1;
        }
        else
        {
            /* update the size */
            *size = area->minSize << ((*level) - 1);
        }
    }

    /* return the block index or -1 if invalid */
    return blockIndex;
}


/**
 * array is
 * 
 * index   =    0     |    1    |    2    |    3    |    4    |    5     |    6    |   7    |
 * address =    0     |   0/2   |   1/2   |   0/4   |   1/4   |   2/4    |   3/4   |  0/8   | 
 * power   =    0     |    1    |    1    |    2    |    2    |    2     |    2    |   3    |
 * 2^power =    1     |    2    |    2    |    4    |    4    |    4     |    4    |   8    |
 */
uint8_t * blockAddress(ptrPharosDynamicMemory area , uint32_t index)
{
    /* return value */
    uint8_t *result;


    /* for index 0 and 1 the block address is easy */
    if(index <= 1)
    {
        /* it is just the memory address */
        result = area->memory;
    }
    else
    {
        /* get the sub-array inside the array that deals with the index power
         * For example:
         * index == 2 => (2+1)/2 = 1.5 => power = 2
         * index == 3 => (3+1)/2 = 2   => power = 4
         * index == 4 => (4+1)/2 = 2.5 => power = 4
         * index == 5 => (5+1)/2 = 3   => power = 4
         * index == 6 => (6+1)/2 = 3.5 => power = 4
         * index == 7 => (7+1)/2 = 4   => power = 8
         */
        uint32_t power = nextPower2(1 + (index + 1) / 2);

        /* return the start address plus the offset associated to the index */
        result = area->memory + (index - (power - 1)) * area->totalSize / power;
    }

    /* return the address of the specified block index */
    return result;
}


void updateSizeParents(ptrPharosDynamicMemory area , uint32_t index)
{
    /* get the parent of the index */
    int32_t p = parent(index);


    /* loop for all parents */
    while(p >= 0)
    {
        /* get the memory area of the index position */
        uint32_t mem_index = (area->blocks[index] == PHAROS_DYNAMIC_MEMORY_UNAVAILABLE) ? 0 : area->blocks[index];

        /* get the memory area of the sibling to the index position */
        uint32_t mem_sibli = (area->blocks[sibling(index)] == PHAROS_DYNAMIC_MEMORY_UNAVAILABLE) ? 0 : area->blocks[sibling(index)];

        /* the parent will have the maximum of them */
        uint32_t min = MAX(mem_index , mem_sibli);

        /* if both are equal (and != 0) */
        if(mem_index != 0 && mem_index == mem_sibli)
        {
            /* then the parent will have both of them */
            min++;
        }

        /* check if the parent already has that value */
        if(min == area->blocks[p])
        {
            /* it has, so nothing to update */
            break;
        }

        /* update the parent block size available */
        area->blocks[p] = min;

        /* update the index to the next iteration */
        index = p;

        /* update the parent to the next iteration */
        p = parent(index);
    }
}
