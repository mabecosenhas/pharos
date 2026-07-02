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


#ifndef PHAROS_ARM_H
#define PHAROS_ARM_H


#include <pharos/hal/hal.h>
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
 *  requirements. This value was chosen to take advantages of caches.
 */
#define STRUCTURE_ALIGNMENT                 (8U)


/**
 * Number of bytes for stack alignment
 */
#define PHAROS_STACK_ALIGNMENT              (16U)


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


/**
 * Perform the data cache of a memory segment
 * 
 * @param start start of the memory segment
 * @param end end of the memory segment
 */
void pharosCpuCacheDataFlush(uint8_t *start , uint8_t *end);


/**
 * Perform the assembly ISB instruction
 */
void pharosCpuIsb(void);


/**
 * Perform the assembly DSB instruction
 */

void pharosCpuDsb(void);


/**
 * Perform the assembly DMB instruction
 */
void pharosCpuDmb(void);


#endif /* ARM_H */
