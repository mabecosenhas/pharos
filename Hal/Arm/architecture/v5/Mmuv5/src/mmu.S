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


.type pharosCpuInvalidateMmu, %function
.type pharosCpuEnableMmu, %function
.type pharosCpuSetTlb, %function
.type pharosCpuGetTlb, %function

.global pharosCpuInvalidateMmu
.global pharosCpuEnableMmu
.global pharosCpuSetTlb
.global pharosCpuGetTlb

.syntax unified
.text


.arm
.func pharosCpuInvalidateMmu
pharosCpuInvalidateMmu:

    /* flush TLB */
    mcr     p15 , 0 , %r0 , c8 , c7 , 0

    /* three delay slots to enable the new fetch */
    nop
    nop

    /* return from function */
    bx      lr
.endfunc
  

.arm
.func pharosCpuEnableMmu
pharosCpuEnableMmu:

    /* place in r0 the address of the kernel TBL address */
    ldr     %r0, =kernelSectionTable

    /* program the c2 register to place the kernel TBL address */
    mcr     p15, 0 , %r0 , c2 , c0 , 0

    /* set the domains on r0 (only the first domain is activated and all remaining are disabled */
    mov     %r0 , #0x1

    /* program the c3 register with the domains */
    mcr     p15, 0 , %r0 , c3, c0 , 0

    /* read control MMU register to r1 (co-processor C15, register c1) */
    mrc     p15, 0 , %r1 , c1 , c0 , 0

    /* set the M bit to enable the MMU */
    orr     r1 , #0x1 

    /* write r1 to MMU control register - will enable the MMU */
    mcr     p15, 0 , %r1 , c1, c0 , 0 

    /* delay slots not needed since the previous and the next instructions don't access any memory */
  
    /* return from function */
    bx      lr
.endfunc
  

.arm
.func pharosCpuSetTlb
pharosCpuSetTlb:

    /* %r0 contains the new TLB to set */

    /* program the c2 register to place the kernel TBL address */
    mcr     p15, 0 , %r0 , c2 , c0 , 0

    /* three delay slots to enable the new fetch */
    nop
    nop
    nop

    /* flush TLB */
    mov     %r0 , #0
    mcr     p15, 0, %r0, c8, c7, 0
  
    nop
    nop
    nop

    /* return from function */
    bx      lr
.endfunc


.arm
.func pharosCpuGetTlb
pharosCpuGetTlb:
    /* return to %r0 the current TLB */
    mrc     p15, 0 , %r0 , c2 , c0 , 0

    /* return from function */
    bx      %lr
.endfunc
