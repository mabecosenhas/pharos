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
#include <pharos/hal/cpu/conf.h>
#include <pharos/kernel/core/syscall.h>

#define IRQ_PEND_BASE_OFFSET                        (0)
#define IRQ_PEND1_OFFSET                            (4)
#define IRQ_PEND2_OFFSET                            (8)

.type pharosCpuIrqHandler,                          %function 

.global pharosCpuIrqHandler


.section .text

/* @note nested interrupts are supported but not tested
 * raspberry pi3 is the only ARMv8-A board supported and it does not have interrupt priorities (only IRQ and FIQ) 
 * so there is no way to test nested interrupts */

.func pharosCpuIrqHandler
pharosCpuIrqHandler:

    /* when we reach here we are running at EL1, stack pointer is currently pointing to the running thread kernel 
     * stack and interrupts are disabled */

    /* save the register that might change when we call a C function to the current stack 
     * (at this point may be the running thread stack or the interrupt stack) */
    stp		x0 , x1 , [sp, #-16]!

    /****************************************************/
    /* check if this is a nested or outermost interrupt */
	/****************************************************/

    /* get the runnning core to x0 */
    mrs     x0 , mpidr_el1

    /* load the mask to x1 to leave only the running core number */
    mov     x1 , PHAROS_CPU_MPIDR_AFFINITY_MASK

    /* clear all but the running core */
    bic     x0 , x0 , x1

    /* get the offset of the core on the pharosVRmpCore table */
    lsl     x0 , x0 , PHAROS_CPU_CORE_RMP_SIZE_LOG2

    /* get the address where the cores are on */
    ldr     x1 , =pharosVRmpCore    

    /* get the address of the &pharosVRmpCore[runningCore] */
    add     x1 , x0 , x1

    /* add the interrupt nest level offset &pharosVRmpCore[runningCore]->interruptLevel  */
    add     x1 , x1 , PHAROS_CORE_TABLE_INTERRUPT_LEVEL_OFFSET

    /* load to x1 the value of the pharosVRmpCore[runningCore]->interruptLevel */
    ldr     x1 , [x1]

    /* compare if not 0 */
    cmp     x1 , #0

    /* if the nest level is != 0 then we are in a nested interrupt and we don't need to switch stack */
    bne     nestedInterrupt   

outermostInterrupt:

    /************************************************/
    /* load to x0 the address of the running thread */
	/************************************************/

    /* get the address where the cores are on */
    ldr     x1 , =pharosVRmpCore    

    /* calculate the final address where the running thread for the running core is on 
     * &pharosVRmpCore[runningCore]->runningThread (offset is 0 since this is the first field) */
    add     x1 , x0 , x1

    /* load to x1 the address of the running thread */
    ldr     x1 , [x1]

    /************************************************/
    /* save the registers to the TCB                */
	/************************************************/

    /* get the address where to store the IRQ stack */
    add     x1 , x1 , OFFSET_IRQ_FRAME

    /* store the registers to the TCB */
    stp		x2  , x3  , [x1] , #16
    stp		x4  , x5  , [x1] , #16
    stp		x6  , x7  , [x1] , #16
    stp		x8  , x9  , [x1] , #16
    stp		x10 , x11 , [x1] , #16
    stp		x12 , x13 , [x1] , #16
    stp		x14 , x15 , [x1] , #16

    /* store x29 and x30 (will be modified once we call a C function). x29 will have the running core number */
    stp		x29 , x30 , [x1] , #16

    /* save SPSR and ELR to stack */

    /* get the SPSR EL1 to x2 */
    mrs		x2 , SPSR_EL1

    /* get the ELR EL1 to x0 (will be the argument to pharosCpuCInterruptHandler) */
	mrs		x0 , ELR_EL1

    /* push SPSR and ELR to thread TCB */
    stp     x2 , x0 , [x1] , #16
    
    /* restore the original x0-x1 to x2-x3 (now the stack is restored to its original place) */
    ldp		x2 , x3 , [sp] , #16
    
    /* store the original x0 and x1 to the TCB */
    stp		x2 , x3 , [x1] , #16

    /* save the current stack pointer (to a local variable that will be saved when we call a C function) */
    mov     x2 , sp

    /* get the sp_el0 */
    mrs     x3 , sp_el0

    /* store the sp and sp_el0 to the TCB */
    stp		x2 , x3 , [x1] , #16

    /* save all FPU registers */
    stp		q0  , q1  , [x1] , #32
	stp		q2  , q3  , [x1] , #32
	stp		q4  , q5  , [x1] , #32
	stp		q6  , q7  , [x1] , #32
	stp		q8  , q9  , [x1] , #32
	stp		q10 , q11 , [x1] , #32
	stp		q12 , q13 , [x1] , #32
	stp		q14 , q15 , [x1] , #32
	stp		q16 , q17 , [x1] , #32
	stp		q18 , q19 , [x1] , #32
	stp		q20 , q21 , [x1] , #32
	stp		q22 , q23 , [x1] , #32
	stp		q24 , q25 , [x1] , #32
	stp		q26 , q27 , [x1] , #32
	stp		q28 , q29 , [x1] , #32
	stp		q30 , q31 , [x1] , #32

    /*  at this point all registers of the caller have been saved to the thread TCB with the following format: 
     *  x2 - x7
     *  x9 , x10
     *  x29 , x30
     *  SPSR_EL1
     *  ELR_EL1
     *  x0 , x1
     *  sp , sp_el0
     *  q0-31 floating point registers
     */

    /*******************************************************************/
    /* Get running core number to x29 and &pharosVRmpCore[core] to x11 */
	/*******************************************************************/

    /* get the runnning core to x0 */
    mrs     x0 , mpidr_el1

    /* load the mask to x1 to leave only the running core number */
    mov     x1 , PHAROS_CPU_MPIDR_AFFINITY_MASK

    /* clear all but the running core */
    bic     x29 , x0 , x1

    /* get the offset of the core on the pharosVRmpCore table */
    lsl     x11 , x29 , PHAROS_CPU_CORE_RMP_SIZE_LOG2

    /* get the address where the cores are on */
    ldr     x0 , =pharosVRmpCore    

    /* get the address of the &pharosVRmpCore[runningCore] */
    add     x11 , x0 , x11

    /**************************************************************************/
    /* switch stack                                                           */
	/**************************************************************************/

    /* stack should be => sp = (core + 1) * interrupt_stack_size + interrupt_stack_base */
    /* <=> sp = core * interrupt_stack_size + interrupt_stack_size + interrupt_stack_base */

    /* calculate the interrupt_stack_size + interrupt_stack_base */
    ldr     x2 , =pharosCpuInterruptStack + PHAROS_CPU_INTERRUPT_STACK_SIZE

    /* calculate the core * interrupt_stack_size */
    lsl     x3 , x29 , PHAROS_CPU_INTERRUPT_STACK_SIZE_LOG2 

    /* sum both and place on the sp */
    add     sp , x2 , x3

    /**************************************************************************/
    /* call C function to handle the interrupt                                */
	/**************************************************************************/

    /* can now call C function that will read the pendable interrupt, enable interrupts, and process the interrupts */
    bl      pharosCpuCInterruptHandler
    
    /* at this time x0 contains the pharosVRmpCore[core]->dispatchLevel */ 

    /************************************************************************************/
    /* load to x1 the address of the running thread pharosVRmpCore[core]->runningThread */
	/************************************************************************************/

    /* at this time, x11 has the &pharosVRmpCore[core], which is equivalent to &pharosVRmpCore[core]->runningThread */

    /* load to x1 the address of the running thread -> pharosVRmpCore[core]->runningThread */
    ldr     x1 , [x11]

    /* get the address where to store the IRQ stack */
    add     x1 , x1 , OFFSET_IRQ_FRAME_SP

    /* save the original sp to x2 and sp_el0 to x3 */
    ldp     x2 , x3 , [x1]

    /* switch to the thread kernel stack */
    mov     sp , x2

    /* switch the sp_el0 */
    msr     sp_el0 , x3

    /*******************************************************************************/
    /* check if a context switch is possible (pharosVRmpCore[core]->dispatchLevel) */
	/*******************************************************************************/

    /* compare the &pharosVRmpCore[core]->dispatchLevel */
    cmp     x0 , 0
    
    /* if not zero then leave the interrupt and do not perform a context switch */
    bne     interruptLeave

    /***********************************************************************************/
    /* check if a context switch is needed (pharosVRmpCore[core]->contextSwitchNeeded) */
	/***********************************************************************************/

    /* get the address -> &pharosVRmpCore[core]->contextSwitchNeeded */

    add     x1 , x11 , PHAROS_CORE_TABLE_CONTEXT_NEEDED_OFFSET
    
    /* load to x1 the context switch needed value -> pharosVRmpCore[core]->contextSwitchNeeded */
    ldr     x1 , [x1]

    /* check if pharosVRmpCore[core]->contextSwitchNeeded is 0 */
    cmp     x1 , 0

    /* of pharosVRmpCore[core]->contextSwitchNeeded == 0, then a context switch is not needed and 
     * we can leave the interrupt */
    beq     interruptLeave

    /********************************************/
    /* do a context switch inside the interrupt */
	/********************************************/

    /* get the address of the running thread (pharosVRmpCore[core]->runningThread) to x0 
     * (argument to the pharosIContextSwitch) */
    ldr     x0 , [x11]

    /* dispatch to other threads if needed */
	bl      pharosIContextSwitch

interruptLeave: 

    /* get the address of the running thread pharosVRmpCore[core]->runningThread */
    ldr     x0 , [x11]

    /* get the address where to store the IRQ stack (SPSR offset) */
    add     x1 , x0 , OFFSET_IRQ_FRAME_SPSR

    /* load the SPSR and ELR to x2-x3 */
    ldp     x2  , x3  , [x1] , #16

    /* restore the SPSR EL1 from x2 */
    msr		SPSR_EL1 , x2

    /* restore the ELR EL1 from x3 */
	msr		ELR_EL1 , x3

    /* get the address where to store the IRQ stack */
    add     x0 , x0 , OFFSET_IRQ_FRAME

    /* restore the registers */
    ldp     x2  , x3  , [x0] , #16
    ldp     x4  , x5  , [x0] , #16
    ldp     x6  , x7  , [x0] , #16
    ldp     x8  , x9  , [x0] , #16
    ldp     x10 , x11 , [x0] , #16
    ldp     x12 , x13 , [x0] , #16
    ldp     x14 , x15 , [x0] , #16
    ldp     x29 , x30 , [x0] , #16

    /* jump over the intermediate registers (6 registers) - note: now use x1 */
    add     x1 , x0 , #8*6

    /* restore all FPU registers */
    ldp		q0  , q1  , [x1] , #32
	ldp		q2  , q3  , [x1] , #32
	ldp		q4  , q5  , [x1] , #32
	ldp		q6  , q7  , [x1] , #32
	ldp		q8  , q9  , [x1] , #32
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

    /* move over the SPSR and ELR (already copied them above) - note: now use x0 again */
    add     x0 , x0 , #16
    
    /* restore x0 and x1 */
    ldp     x0  , x1  , [x0]   
    
    /* return from interrupt */
    eret
  

nestedInterrupt:

    /********************************************/
    /* save context to interrupt stack          */
	/********************************************/

    /* at this point we saved x0 and x1 to the interrupt stack, we need to save the rest */

    stp		x0  , x1  , [sp, #-16]!

    /* store the registers to the interrupt stack */
    stp		x2  , x3  , [sp, #-16]!
    stp		x4  , x5  , [sp, #-16]!
    stp		x6  , x7  , [sp, #-16]!
    stp		x8  , x9  , [sp, #-16]!
    stp		x10 , x11 , [sp, #-16]!
    stp		x12 , x13 , [sp, #-16]!
    stp		x14 , x15 , [sp, #-16]!

    /* store x29 and x30 (will be modified once we call a C function). x29 will have the running core number */
    stp		x29 , x30 , [sp, #-16]!

    /* save all FPU registers */
    stp		q0  , q1  , [sp , #-32]!
	stp		q2  , q3  , [sp , #-32]!
	stp		q4  , q5  , [sp , #-32]!
	stp		q6  , q7  , [sp , #-32]!
	stp		q8  , q9  , [sp , #-32]!
	stp		q10 , q11 , [sp , #-32]!
	stp		q12 , q13 , [sp , #-32]!
	stp		q14 , q15 , [sp , #-32]!
	stp		q16 , q17 , [sp , #-32]!
	stp		q18 , q19 , [sp , #-32]!
	stp		q20 , q21 , [sp , #-32]!
	stp		q22 , q23 , [sp , #-32]!
	stp		q24 , q25 , [sp , #-32]!
	stp		q26 , q27 , [sp , #-32]!
	stp		q28 , q29 , [sp , #-32]!
	stp		q30 , q31 , [sp , #-32]!

   /* save SPSR and ELR to stack */

    /* get the SPSR EL1 to x2 */
    mrs		x2 , SPSR_EL1

    /* get the ELR EL1 to x0 (will be the argument to pharosCpuCInterruptHandler) */
	mrs		x0 , ELR_EL1

    /* push SPSR and ELR to thread TCB */
    stp     x2 , x0 , [sp, #-16]!

    /* at this point the stack looks like:
     * x0 , x1
     * x2 , x3
     * x4 , x5
     * x6 , x7
     * x8 , x9
     * x10 , x11
     * x12 , x13
     * x14 , x15
     * x29 , x30
     * q0 , q1
     * q2 , q3
     * q4 , q5
     * q6 , q7
     * q8 , q9
     * q10 , q11
     * q12 , q13
     * q14 , q15
     * q16 , q17
     * q18 , q19
     * q20 , q21
     * q22 , q23
     * q24 , q25
     * q26 , q27
     * q28 , q29
     * q30 , q31
     * SPSR_EL1 , ELR_EL1
     */
    /********************************************/
    /* call C handler function                  */
	/********************************************/

    /* now everything is saved, we can call the C function */

    /* can now call C function that will read the pendable interrupt, enable interrupts, and process the interrupts */
    bl      pharosCpuCInterruptHandler
 
    /********************************************/
    /* restore the context from the stack       */
	/********************************************/
 
    /* get the originals SPSR EL1 and ELR EL1 */
    ldp		x2 , x3 , [sp] , #16

    /* restore the SPSR EL1 from x2 */
    msr		SPSR_EL1 , x2

    /* restore the ELR EL1 from x3 */
	msr		ELR_EL1 , x3

    /* restore all FPU registers */
    ldp		q30 , q31 , [sp] , #32
	ldp		q28 , q29 , [sp] , #32
	ldp		q26 , q27 , [sp] , #32
	ldp		q24 , q25 , [sp] , #32
	ldp		q22 , q23 , [sp] , #32
	ldp		q20 , q21 , [sp] , #32
	ldp		q18 , q19 , [sp] , #32
	ldp		q16 , q17 , [sp] , #32
	ldp		q14 , q15 , [sp] , #32
	ldp		q12 , q13 , [sp] , #32
	ldp		q10 , q11 , [sp] , #32
	ldp		q8  , q9  , [sp] , #32
	ldp		q6  , q7  , [sp] , #32
	ldp		q4  , q5  , [sp] , #32
	ldp		q2  , q3  , [sp] , #32
    ldp		q0  , q1  , [sp] , #32

    /* restore the registers */
    ldp     x29 , x30 , [sp] , #16   
    ldp     x14 , x15 , [x0] , #16
    ldp     x12 , x13 , [x0] , #16
    ldp     x10 , x11 , [x0] , #16
    ldp     x8  , x9  , [x0] , #16
    ldp     x6  , x7  , [x0] , #16
    ldp     x4  , x5  , [x0] , #16
    ldp     x2  , x3  , [x0] , #16
    ldp     x0  , x1  , [x0]

    /* return from interrupt */
    eret
  
.endfunc
