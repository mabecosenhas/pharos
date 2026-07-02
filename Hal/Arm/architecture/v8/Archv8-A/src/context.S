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


#include <pharos/hal/cpu/asm.h>


.type pharosCpuContextSwitch,                       %function 
.type pharosCpuRestoreHeir,                         %function

.global pharosCpuContextSwitch
.global pharosCpuRestoreHeir


.section .text


/**
 * void pharosCpuContextSwitch(ptrThreadCpuContext current , ptrThreadCpuContext heir);
 * x0 = current
 * x1 = heir
 */
.func pharosCpuContextSwitch
pharosCpuContextSwitch:

    /* save EL1 sp to x2 */
    mov     x2 , sp

    /* save EL0 sp to x3 */
    mrs     x3 , sp_el0

    /* get the ELR EL1 to x4 */
	mrs		x4 , ELR_EL1

    /* get the SPSR EL1 to x5 */
    mrs		x5 , SPSR_EL1

    /* get the TTBR0_EL1 to x6 */
    mrs     x6 , TTBR0_EL1
   
    /* store EL1 sp and EL0 sp to thread CPU context */
    stp     x2 , x3 , [x0] , #16

    /* store ELR_EL1 and SPSR_EL1 to thread CPU context */
    stp     x4 , x5 , [x0] , #16
  
    /* store the TTBR0_EL1 and x8 registers to thread CPU context */
    stp     x6 , x8 , [x0] , #16

    /* store two registers to thread CPU context */
    stp     x9 , x10 , [x0] , #16

    /* store two registers to thread CPU context */
    stp     x11 , x12 , [x0] , #16

    /* store two registers to thread CPU context */
    stp     x13 , x14 , [x0] , #16

    /* store two registers to thread CPU context */
    stp     x15 , x16 , [x0] , #16

    /* store two registers to thread CPU context */
    stp     x17 , x18 , [x0] , #16

    /* store two registers to thread CPU context */
    stp     x19 , x20 , [x0] , #16

    /* store two registers to thread CPU context */
    stp     x21 , x22 , [x0] , #16

    /* store two registers to thread CPU context */
    stp     x23 , x24 , [x0] , #16

    /* store two registers to thread CPU context */
    stp     x25 , x26 , [x0] , #16

    /* store two registers to thread CPU context */
    stp     x27 , x28 , [x0] , #16

    /* store two registers to thread CPU context */
    stp     x29 , x30 , [x0] , #16

    /* save all FPU registers */
    stp		q0 , q1 , [x0] , #32
	stp		q2 , q3 , [x0] , #32
	stp		q4 , q5 , [x0] , #32
	stp		q6 , q7 , [x0] , #32
	stp		q8 , q9 , [x0] , #32
	stp		q10 , q11 , [x0] , #32
	stp		q12 , q13 , [x0] , #32
	stp		q14 , q15 , [x0] , #32
	stp		q16 , q17 , [x0] , #32
	stp		q18 , q19 , [x0] , #32
	stp		q20 , q21 , [x0] , #32
	stp		q22 , q23 , [x0] , #32
	stp		q24 , q25 , [x0] , #32
	stp		q26 , q27 , [x0] , #32
	stp		q28 , q29 , [x0] , #32
	stp		q30 , q31 , [x0] , #32
    
    /* restore */
    
    /* get the SP_EL1 to x2 and SP_EL0 to x3 */
    ldp     x2 , x3 , [x1] , #16

    /* load ELR_EL1 and SPSR_EL1 to x4 and x5 */
    ldp     x4 , x5 , [x1] , #16

    /* load TTBR0_EL1 and x8 from the thread CPU context */
    ldp     x6 , x8 , [x1] , #16

    /* load both registers from the thread CPU context */
    ldp     x9 , x10 , [x1] , #16

    /* load both registers from the thread CPU context */
    ldp     x11 , x12 , [x1] , #16

    /* load both registers from the thread CPU context */
    ldp     x13 , x14 , [x1] , #16

    /* load both registers from the thread CPU context */
    ldp     x15 , x16 , [x1] , #16

    /* load both registers from the thread CPU context */
    ldp     x17 , x18 , [x1] , #16

    /* load both registers from the thread CPU context */
    ldp     x19 , x20 , [x1] , #16

    /* load both registers from the thread CPU context */
    ldp     x21 , x22 , [x1] , #16

    /* load both registers from the thread CPU context */
    ldp     x23 , x24 , [x1] , #16

    /* load both registers from the thread CPU context */
    ldp     x25 , x26 , [x1] , #16

    /* load both registers from the thread CPU context */
    ldp     x27 , x28 , [x1] , #16

    /* load both registers from the thread CPU context */
    ldp     x29 , x30 , [x1] , #16

    /* restore the TLB for the EL0 (if we do it here, for sure we don't need an isb before executing the heir thread 
     * since the pipeline will never be so long (over 21 instructions still left to be executed on this function alone) */
    msr     TTBR0_EL1 , x6

    /* restore the EL0 sp */
    msr     sp_el0 , x3

    /* restore the EL1 sp */
    mov     sp , x2

    /* restore the SPSR EL1 from x5 */
    msr		SPSR_EL1 , x5

    /* restore the ELR EL1 from x4 */
	msr		ELR_EL1 , x4

    /* restore FPU registers */
    ldp		q0 , q1 , [x1] , #32
	ldp		q2 , q3 , [x1] , #32
	ldp		q4 , q5 , [x1] , #32
	ldp		q6 , q7 , [x1] , #32
	ldp		q8 , q9 , [x1] , #32
	ldp		q10 , q11 , [x1] , #32
	ldp		q12 , q13 , [x1] , #32
	ldp		q14 , q15 , [x1] , #32
	ldp		q16 , q17 , [x1] , #32
	ldp		q18 , q19 , [x1] , #32
	ldp		q20 , q21 , [x1] , #32
	ldp		q22 , q23 , [x1] , #32
	ldp		q24 , q25 , [x1] , #32
	ldp		q26 , q27 , [x1] , #32
	ldp		q28 , q29 , [x1] , #32
	ldp		q30 , q31 , [x1] , #32

    /* return to the heir thread */
    ret 

.endfunc


/**
 * void pharosCpuRestoreHeir(ptrThreadCpuContext heir);
 * x0 = heir
 */
.func pharosCpuRestoreHeir
pharosCpuRestoreHeir:

    /* get current execution level (EL) */
    mrs     x2 , CurrentEL

     /* get the SP_EL1 to x2 and SP_EL0 to x3 */
    ldp     x2 , x3 , [x0] , #16

    /* load ELR_EL1 and SPSR_EL1 to x4 and x5 */
    ldp     x4 , x5 , [x0] , #16

    /* load TTBR0_EL1 and x8 from the thread CPU context */
    ldp     x6 , x8 , [x0] , #16

    /* load both registers from the thread CPU context */
    ldp     x9 , x10 , [x0] , #16

    /* load both registers from the thread CPU context */
    ldp     x11 , x12 , [x0] , #16

    /* load both registers from the thread CPU context */
    ldp     x13 , x14 , [x0] , #16

    /* load both registers from the thread CPU context */
    ldp     x15 , x16 , [x0] , #16

    /* load both registers from the thread CPU context */
    ldp     x17 , x18 , [x0] , #16

    /* load both registers from the thread CPU context */
    ldp     x19 , x20 , [x0] , #16

    /* load both registers from the thread CPU context */
    ldp     x21 , x22 , [x0] , #16

    /* load both registers from the thread CPU context */
    ldp     x23 , x24 , [x0] , #16

    /* load both registers from the thread CPU context */
    ldp     x25 , x26 , [x0] , #16

    /* load both registers from the thread CPU context */
    ldp     x27 , x28 , [x0] , #16

    /* load both registers from the thread CPU context */
    ldp     x29 , x30 , [x0] , #16

    /* restore the EL0 sp */
    msr     sp_el0 , x3

    /* restore the EL1 sp */
    mov     sp , x2

    /* restore the SPSR EL1 from x5 */
    msr		SPSR_EL1 , x5

    /* restore the ELR EL1 from x4 */
	msr		ELR_EL1 , x4

    /* restore FPU registers */
    ldp		q0 , q1 , [x0] , #32
	ldp		q2 , q3 , [x0] , #32
	ldp		q4 , q5 , [x0] , #32
	ldp		q6 , q7 , [x0] , #32
	ldp		q8 , q9 , [x0] , #32
	ldp		q10 , q11 , [x0] , #32
	ldp		q12 , q13 , [x0] , #32
	ldp		q14 , q15 , [x0] , #32
	ldp		q16 , q17 , [x0] , #32
	ldp		q18 , q19 , [x0] , #32
	ldp		q20 , q21 , [x0] , #32
	ldp		q22 , q23 , [x0] , #32
	ldp		q24 , q25 , [x0] , #32
	ldp		q26 , q27 , [x0] , #32
	ldp		q28 , q29 , [x0] , #32
	ldp		q30 , q31 , [x0] , #32
	
    /* make sure the previous data instructions have completed */
    dsb     ish

    /* make sure the previous instructions have completed */
    isb

    /* restore the TLB for the EL0 */
    msr     TTBR0_EL1 , x6

    /* read the current SCTRL */
    mrs     x6 , SCTLR_EL1

    /* set the value to x5 to clear the SCTRL */
    /* clear EE, little endian translation tables */
    /* clear E0E */ 
    /* clear WXN */ 
    /* clear SA0 */ 
    /* clear SA */ 
    /* clear A, no aligment check */ 
    ldr     x5 , =((1 << 25) | (1 << 24) | (1 << 19) |(1 <<  4) | (1 <<  3) | (1 <<  1)) 

    /* clear the bits on the SCTRL */
    bic     x6 , x6 , x5

    /* set the bits */
    ldr     x5 , =(0xC00801 | (1U << 12) | (1 << 2))

    /* set the bits on the SCTRL */
    orr     x6 , x6 , x5

    /* set the SCTRL */
    msr     SCTLR_EL1 , x6

    /* force changes to the TTBR0_EL1 */
    isb

    /* return to the heir thread */
    eret 

.endfunc
