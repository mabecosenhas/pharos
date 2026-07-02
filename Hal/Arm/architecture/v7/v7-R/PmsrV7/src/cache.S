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


.type pharosCpuCacheEnable , %function

.global pharosCpuCacheEnable
 
/* following code runs on ARM instruction set */
.arm


.syntax unified
.text

/* void pharosCpuCacheEnable() */
.func pharosCpuCacheEnable
pharosCpuCacheEnable:

    /* Read System Control Register */
    mrc     p15, 0, %r0, c1, c0, 0

    /* Disable data cache bit */
    bic     %r0, %r0, #0x1 << 2

    /* Disable instruction cache bit */
    bic     %r0, %r0, #0x1 << 12

    /* make sure all the data has been flushed in the pipeline */
    dsb

    /* Write System Control Register (disabling data and instruction caches) */
    mcr     p15, 0, %r0, c1, c0, 0 

    /* make sure all instructions have been flushed in the pipeline */
    isb

    /* Clean entire data cache. This routine depends on the data cache size. It can be
    omitted if it is known that the data cache has no dirty data, for example if the cache
    has not been enabled yet. */

    /* read the Auxiliary Control Register */
    mrc     p15, 0, r1, c1, c0, 1 

    /* clear bits 5:3 (CEC) from the Auxiliary Control Register */
    bic     %r1 , %r1 , #38

    /* this will make CEC = 010b, which will cause an abort on parity and ECC cache errors, 
     * force a cache write-through and enable hw recovery */
    orr     %r1 , %r1 , #(2U << 3U)

    /* write to Auxiliary Control Register, with the new abort/recovery cache policy */
    mcr     p15, 0, %r1, c1, c0, 1 

    /* Invalidate entire data cache */
    mcr     p15, 0, %r0, c15, c5, 0

    /* Invalidate entire instruction cache */
    mcr     p15, 0, %r0, c7, c5, 0 

    /* read System Control Register */
    mrc     p15, 0, %r0, c1, c0, 0 

    /* enable data cache bit */
    orr     %r0, %r0, #0x1 << 2
    
    /* enable instruction cache bit */
    orr     %r0, %r0, #0x1 << 12
    
    /* make sure all the data has been flushed in the pipeline */
    dsb

    /* Write System Control Register */
    mcr     p15, 0, %r0, c1, c0, 0

    /* make sure all instructions have been flushed in the pipeline */
    isb

    /* return from function */
    bx      lr

.endfunc
