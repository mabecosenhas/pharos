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


.type pharosCpuFlushDataCache,              %function
.type pharosCpuInvalidateCaches ,           %function
.type pharosCpuInvalidateICache ,           %function
.type pharosCpuCacheDataFlush ,             %function

.global pharosCpuFlushDataCache
.global pharosCpuInvalidateCaches
.global pharosCpuInvalidateICache
.global pharosCpuCacheDataFlush

.syntax unified
.text


.arm
.func pharosCpuFlushDataCache
pharosCpuFlushDataCache:
    /* save the registers used on this function */
    push    {%r0}

    /* flush the data cache */
    mov     r0, #0
	mcr     p15, 0, r0, c7, c6, 0

    /* restore the registers changed */
    pop     {%r0}

    /* return from function */
	bx      lr
.endfunc


.arm
.func pharosCpuInvalidateCaches
pharosCpuInvalidateCaches:
    
    /* move 0 to r0 needed for the invalidate operation */
    mov     r0, #0

    /* invalidate both caches */
    mcr     p15, 0, %r0, c7, c7, 0


    /* return from function */
	bx      lr
.endfunc

 
.arm
.func pharosCpuInvalidateICache
pharosCpuInvalidateICache:

    /* move 0 to r0 needed for the invalidate operation */
    mov     r0, #0

    /* invalidate the instruction cache */
    mcr     p15, 0, %r0, c7, c5, 0

    /* return from function */
	bx      lr
.endfunc


.func pharosCpuCacheDataFlush
pharosCpuCacheDataFlush:

    /* get to r0 the start address aligned to the data cache line */
    bic     %r0 , %r0  , #7

nextLine: 

    /* clean data at address start address */
    mcr     p15, #0, %r0, c7, c10, #1 

    /* move to next cache line */
    add     %r0 , %r0 , #8

    /* check if finished */
    cmp     %r0 , %r1

    /* if current address is lower than the end address then clean the next address */
    blo     nextLine

    /* data sync barrier */
    mcr     p15, #0, %r0, c7, c10, #4

    /* return from function */
	bx      lr

.endfunc
