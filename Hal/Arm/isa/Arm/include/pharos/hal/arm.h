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


/* 
 * File:   arm.h
 *
 * Created on September 25, 2017
 */

#ifndef PHAROS_ARM_H
#define PHAROS_ARM_H


#include <pharos/hal/hal.h>
#include <pharos/interrupt.h>
#include <pharos/hal/io.h>
#include <pharos/hal/cpu/context.h>




/**
 *  @brief force alignment of critical data structures
 *
 *  The following is the variable attribute used to force alignment
 *  of critical data structures.  On some processors it may make
 *  sense to have these aligned on tighter boundaries than
 *  the minimum requirements of the compiler in order to have as
 *  much of the critical data area as possible in a cache line.
 *
 *  The ARM does not appear to have particularly strict alignment
 *  requirements.  This value was chosen to take advantages of caches.
 */
#define STRUCTURE_ALIGNMENT         (8U)

#define PHAROS_STACK_ALIGNMENT      (16U)


/**
 * Invalidate the instruction and the data cache
 */
void pharosCpuInvalidateCaches(void);


/**
 * Invalidates the instruction cache 
 */
void pharosCpuInvalidateICache(void);

/**
 * Flush the data cache 
 */
void pharosCpuFlushDataCache(void);


void pharosCpuCacheDataFlush(uint8_t *start , uint8_t *end);

/**
 * Converts the leftmost bit set to the corresponding number. 
 * 
 * For example, if bitNumber is:
 *   0b00001100 then the result will be 3
 *   0b00001110 then the result will be 2
 *   0b00000001 then the result will be 1
 * 
 * @param bitNumber bit number to determine
 * 
 * @return returns the corresponding number
 */
uint32_t armbit2Integer(uint32_t bitNumber);


/**
 * Get the current stack pointer value
 * 
 * @return returns the current stack pointer value
 */
uint8_t *pharosCpuSpGet(void);


#endif /* ARM_H */
