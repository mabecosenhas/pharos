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


#ifndef PHAROS_MEMORY_H
#define PHAROS_MEMORY_H

#include <pharos/kernel/memory/declarations.h>
#include <pharos/hal/cpu/conf.h>

struct EnvMemoryProtection
{
    /**
     * @brief start address of global variables of the partition in memory
     */
    const uint8_t *startAddress[PHAROS_NUMBER_MEMORY_AREAS];

    /**
     * @brief end address of global variables of the partition in memory
     */
    const uint8_t *endAddress[PHAROS_NUMBER_MEMORY_AREAS];
};

typedef struct
{
    uint64_t *startAddress[PHAROS_NUMBER_MEMORY_AREAS];
    uint64_t *endAddress[PHAROS_NUMBER_MEMORY_AREAS];
    uint64_t *dataLoadAddress[PHAROS_NUMBER_MEMORY_AREAS];
    uint64_t *dataRamAddress[PHAROS_NUMBER_MEMORY_AREAS];
    uintptr_t dataSize[PHAROS_NUMBER_MEMORY_AREAS];

    uint64_t *bssAddress[PHAROS_NUMBER_MEMORY_AREAS];
    uintptr_t bssSize[PHAROS_NUMBER_MEMORY_AREAS];

} PharosMemoryArea , *ptrPharosMemoryArea;


/**
 * Checks to see if the specified address falls under the memory protection environment
 * 
 * @param memProtection memory protection environment 
 * @param address address to check
 * 
 * @return returns TRUE if the memory protection environment encompasses the specified address and FALSE otherwise
 */
bool pharosIAddressInsideMemoryArea(ptrEnvMemoryProtection memProtection , uint8_t *address);


/**
 * Fills the destination with a pattern
 * 
 * @param dst destination
 * @param src pattern to place
 * @param size number of patterns to place [4 bytes]
 */
void pharosISet64(uint64_t *dst , uint64_t src , uintptr_t size);


/**
 * Determines if the pointer is aligned to the specified alignment
 * 
 * @constraint alignment must be a power of 2
 * 
 * @param pointer pointer to determine if it is aligned 
 * @param alignment required alignment (e.g. 2, 4, 8)
 * 
 * @return returns TRUE if the pointer is aligned to the specified alignment and FALSE otherwise 
 */
INLINE bool pharosIIsAligned(const void *pointer , const uintptr_t alignment)
{
    /* convert the pointer */
    uintptr_t aux = (uintptr_t) pointer;

    /* check if it is aligned */
    return(aux & (alignment - 1)) == 0 ? TRUE : FALSE;
}


/**
 * Determines if two pointers can be aligned to the next specified alignment up to a maximum number of bytes
 * 
 * @constraint alignment must be a power of 2
 * 
 * @param p1 first pointer to determine if it is aligned 
 * @param p2 second pointer to determine if it is aligned 
 * @param alignment required alignment (e.g. 2, 4, 8)
 * @param size maximum number of bytes to align
 * 
 * @return returns 0 if the pointers are both are already aligned, smaller than zero if they can't be both be aligned or
 * bigger than zero with the exact number of bytes to add to make them both aligned
 */
INLINE intptr_t pharosIAlignmentNextBothBound(const void *p1 , const void *p2 , const uintptr_t alignment , size_t size)
{
    /* alignment mask */
    uintptr_t a = alignment - 1U;

    /* calculate the number of bytes for p1 to be aligned */
    uintptr_t r1 = ((uintptr_t) p1 & a);

    /* calculate the number of bytes for p2 to be aligned */
    uintptr_t r2 = ((uintptr_t) p2 & a);

    /* determine the number of bytes (from 0 to a) to align p1 */
    uintptr_t r = (alignment - r1) & a;


    /* if the number of bytes is equal and there are sufficient bytes to check, 
     * then return the number of bytes remaining, otherwise return -1 */
    return((r1 == r2) && (r < size)) ? (intptr_t) r : (-1);
}


/**
 * Determines if two pointers can be aligned to the specified alignment
 * 
 * @constraint alignment must be a power of 2
 * 
 * @param p1 first pointer to determine if it is aligned 
 * @param p2 second pointer to determine if it is aligned 
 * @param alignment required alignment (e.g. 2, 4, 8)
 * @param size maximum number of bytes to align
 * 
 * @return returns 0 if the pointers are both are already aligned, smaller than zero if they can't be both be aligned or
 * bigger than zero with the exact number of bytes to add to make them both aligned
 */
INLINE intptr_t pharosIAlignmentNextBothUnbound(const void *p1 , const void *p2 , const uintptr_t alignment)
{
    /* alignment mask */
    uintptr_t a = alignment - 1U;

    /* calculate the number of bytes for p1 to be aligned */
    uintptr_t r1 = ((uintptr_t) p1 & a);

    /* calculate the number of bytes for p2 to be aligned */
    uintptr_t r2 = ((uintptr_t) p2 & a);

    /* determine the number of bytes (from 0 to a) to align p1 */
    uintptr_t r = (alignment - r1) & a;


    /* if the number of bytes is equal and there are sufficient bytes to check, 
     * then return the number of bytes remaining, otherwise return -1 */
    return((r1 == r2)) ? (intptr_t) r : (-1);
}


/**
 * Determines the number of bytes until p1 is aligned (or -1) if it cannot be aligned within size
 * 
 * @constraint alignment must be a power of 2
 * 
 * @param p1 pointer to be aligned
 * @param alignment required alignment (e.g. 2, 4, 8)
 * @param size maximum number of bytes to align
 * 
 * @return returns 0 if the pointer is already aligned, smaller than zero if it can't aligned or
 * bigger than zero with the exact number of bytes to add to make it aligned
 */
INLINE intptr_t pharosIAlignmentNextBound(const void *p1 , const uintptr_t alignment , size_t size)
{
    /* alignment mask */
    uintptr_t a = alignment - 1U;

    /* calculate the number of bytes for p1 to be aligned */
    uintptr_t r1 = ((uintptr_t) p1 & a);

    /* determine the number of bytes (from 0 to a) to align p1 */
    uintptr_t r = (alignment - r1) & a;


    /* if the number of bytes is equal and there are sufficient bytes to check, 
     * then return the number of bytes remaining, otherwise return -1 */
    return(r < size) ? (intptr_t) r : (-1);
}


/**
 * Determines the number of bytes until p1 is aligned 
 * 
 * @constraint alignment must be a power of 2
 * 
 * @param p1 pointer to be aligned
 * @param alignment required alignment (e.g. 2, 4, 8)
 * 
 * @return returns 0 if the pointer is already aligned or bigger than zero with the exact number of bytes to add to 
 * make it aligned
 */
INLINE intptr_t pharosIAlignmentNextUnbound(const void *p1 , const uintptr_t alignment)
{
    /* alignment mask */
    uintptr_t a = alignment - 1U;

    /* calculate the number of bytes for p1 to be aligned */
    uintptr_t r1 = ((uintptr_t) p1 & a);

    /* determine the number of bytes (from 0 to a) to align p1 */
    uintptr_t r = (alignment - r1) & a;


    /* return the number of bytes remaining */
    return r;
}


/**
 * Determines if two pointers can be aligned to the previous specified alignment up to a maximum number of bytes
 * 
 * @constraint alignment must be a power of 2
 * 
 * @param p1 first pointer to determine if it is aligned 
 * @param p2 second pointer to determine if it is aligned 
 * @param alignment required alignment (e.g. 2, 4, 8)
 * @param size maximum number of bytes to align
 * 
 * @return returns 0 if the pointers are both are already aligned, smaller than zero if they can't be both be aligned or
 * bigger than zero with the exact number of bytes to add to make them both aligned
 */
INLINE intptr_t pharosIAlignmentPreviousBothSizeBound(const void *p1 , const void *p2 , const uintptr_t alignment ,
                                                      size_t size)
{
    /* calculate the number of bytes for p1 to be aligned */
    uintptr_t r1 = ((uintptr_t) p1 & (alignment - 1));

    /* calculate the number of bytes for p2 to be aligned */
    uintptr_t r2 = ((uintptr_t) p2 & (alignment - 1));


    /* if the number of bytes is equal and there are sufficient bytes to check, 
     * then return the number of bytes to the previous alignment, otherwise return -1 */
    return((r1 == r2) && (r1 < size)) ? ((intptr_t) r1) : (-1);
}


/**
 * Determines the number of bytes before p1 is aligned (or -1) if it cannot be aligned within size
 * 
 * @constraint alignment must be a power of 2
 * 
 * @param p1 pointer to be aligned
 * @param alignment required alignment (e.g. 2, 4, 8)
 * @param size maximum number of bytes to align
 * 
 * @return returns 0 if the pointer is already aligned, smaller than zero if it can't aligned or
 * bigger than zero with the exact number of bytes to add to make it aligned
 */
INLINE intptr_t pharosIAlignmentPreviousBound(const void *p1 , const uintptr_t alignment , size_t size)
{
    /* calculate the number of bytes for p1 to be aligned */
    uintptr_t r1 = ((uintptr_t) p1 & (alignment - 1));


    /* if the number of bytes is equal and there are sufficient bytes to check, 
     * then return the number of bytes, otherwise return -1 */
    return(r1 < size) ? ((intptr_t) r1) : -1;
}


/**
 * Determines the number of bytes before p1 is aligned
 * 
 * @constraint alignment must be a power of 2
 * 
 * @param p1 pointer to be aligned
 * @param alignment required alignment (e.g. 2, 4, 8)
 * 
 * @return returns the exact number of bytes to add to make it aligned
 */
INLINE uintptr_t pharosIAlignmentPreviousUnbound(const void *p1 , const uintptr_t alignment)
{
    /* calculate the number of bytes for p1 to be aligned */
    return((uintptr_t) p1 & (alignment - 1));
}


/**
 * Determines the number of bytes before p1 is aligned
 * 
 * @constraint alignment must be a power of 2
 * 
 * @param p1 pointer to be aligned
 * @param alignment required alignment (e.g. 2, 4, 8)
 * 
 * @return returns the exact number of bytes to add to make it aligned
 */
INLINE uintptr_t pharosIAlignmentPreviousUnboundUintptr(uintptr_t p1 , const uintptr_t alignment)
{
    /* calculate the number of bytes for p1 to be aligned */
    return(p1 & (alignment - 1));
}


/**
 * Determines if the end of string '\0' is a byte in x
 * 
 * @param x uint64_t to determine
 * 
 * @return returns 0 if it does not contain a '\0' and != 0 otherwise 
 */
INLINE int64_t pharosU64ContainsTermination(uint64_t x)
{
    /* search for '\0' in the 64 bit variable */
    return(x - 0x0101010101010101U) & ((~x) & 0x8080808080808080U);
}


/**
 * Builds a mask of 64-bits with the 8-bit pattern
 * 
 * @param c 8-bit pattern with which the fill the 64-bits
 * 
 * @return returns the 8-byte mask
 */
INLINE uint64_t pharosU64BuildMask(uint8_t c)
{
    /* cast to a 64-bit variable */
    uint64_t u = c;

    /* build the mask with the char to find */
    return(u << 56U) | (u << 48U) | (u << 40U) | (u << 32U) | (u << 24U) | (u << 16U) | (u << 8U) | (u << 0U);
}


/**
 * Determines if the specified mask (build it with pharosU64BuildMask) is inside the uint64_t variable
 * 
 * @param x uint64_t to determine
 * @param mask mask to detect (build it with pharosU64BuildMask)
 * 
 * @return returns 0 if it does not contain the char and != 0 otherwise 
 */
INLINE int64_t pharosU64ContainsMask(uint64_t x , uint64_t mask)
{
    /* check if the uint8_t is inside x (the uint8t_t generated the mask) */
    return((x ^ mask) - 0x0101010101010101LU) & ~(x ^ mask) & 0x8080808080808080LU;
}

/* PHAROS_MEMORY_H */
#endif 
