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


/**
 * @brief Handles generic interrupts and maps them to the C handler.
 * Saves/restores the CPU registers and FPU registers, performs thread dispatch if needed
 * Assumes a full MMU is in place.
 */


#include <pharos/hal/cpu/asm.h>

#define INTERRUPT_VECTOR_ADDRESS_OFFSET (0x30)

.type pharosCpuIsrHandler, %function
.type pharosCpuUndefHandler, %function

.type pharosCpuFiqHandler, %function


.global pharosCpuIsrHandler
.global pharosCpuUndefHandler

.global pharosCpuFiqHandler

.syntax unified
.text



.arm
.func pharosCpuIsrHandler
pharosCpuIsrHandler: 

    /* save r0 - r2 to the IRQ stack for now because we need to change to the Kernel mode and for that we need r0 - r2 to do so */
    push    {%r0 , %r1 , %r2}

    /* get to %r2 the current TLB */
    mrc     p15, 0 , %r2 , c2 , c0 , 0

    /* load to r1 the address of the kernel TLB */
    ldr     %r1, =kernelSectionTable

    /* program the c2 register to place the kernel TBL address */
    mcr     p15, 0 , %r1, c2 , c0 , 0

    /* 3 slots before can access the kernel memory area */
    nop
    nop

    ldr     %r0, =pharosVInterruptNestingLevel
    ldr     %r0, [%r0]

    cmp     %r0 , #0

    /* if the nest level is != 0 then we are in a nested interrupt */
    bne     nestedInterrupt
    
outermostInterrupt:

    /*************************************************/
    /* save registers to running thread TCB          */
	/*************************************************/

    /* get the SPSR to r0 */
    mrs     %r0 , SPSR

    /* load to %r1 the pointer to the address of the running thread */
    ldr     %r1, =pharosVRunningThread

    /* r1 = address of the running thread CPU context */
    ldr     %r1 , [%r1]
  
    /* make r1 point to the interrrupt part of the thread control block */
    add     %r1 , %r1 , OFFSET_IRQ_FRAME + OFFSET_IRQ_FRAME_SPSR

    /* save the SPSR, current TLB , registers r3 - r12 and lr to the thread control block */
    stmia   %r1! , {%r0 , %r2 , %r3 - %r12 , %lr}
    
    /* save r1 to r3 (which has already been saved to the thread control block) */
    mov     %r3 , %r1

    /* get the original value of r0 and r1, IRQ stack is now restored to its original position */
    pop     {%r0 , %r1 , %r2}

    /* store the original r0 and r1 */
    stmia   %r3! , {%r0 , %r1 , %r2}

    /* load the interrupt address that was interrupted (will be an argument to the function) */
    mov     %r0 , %lr

    /* switch to supervisor mode to save its registers */
    msr     cpsr_c , #(CPSR_SUPERVISOR_MODE| CPSR_DISABLE_IRQ)

    /* save the current stack pointer on the supervisor mode since we will change to the IRQ stack pointer */
    mov     %r4 , %sp

    /* save to r4 the lr_supervisor */
    mov     %r5 , %lr

    /* get the SPSR_supervisor to r5 */
    mrs     %r6 , SPSR

    /* store the supervisor lr and SPSR (if a context switch is needed these need to be restored afterwards) */
    stmia   %r3! , {%r4 , %r5 , %r6}
    
    /* only compile this code if FPU is supported */
#ifdef PHAROS_CPU_HAS_VFP

    /* save floating point registers */
    vstm    r3! , {s0 - s31}   

    /* save the FPSCR to r4 (already saved and will be overriden afterwards) */
    vmrs    %r4 , FPEXC

    vmrs    %r5 , FPSCR

    vmrs    %r6 , FPINST

    vmrs    %r7 , FPINST2 

    /* store the FPSCR to the thread TCB */
    stmia	r3!,  {r4 , %r5 ,%r6 , %r7}

#endif

    /*  at this point all registers of the caller have been saved to the thread TCB with the following format: 
     *  FPEXC, FPSCR, FPINST and FPINST2
     *  s0-31 floating point registers
     *  SPSR_super
     *  lr_super
     *  sp_super
     *  r2
     *  r1
     *  r0
     *  lr_irq
     *  r12
     *  r11
     *  r10
     *  r9
     *  r8
     *  r7
     *  r6
     *  r5
     *  r4
     *  r3
     *  TLB
     *  SPSR_irq
     */

    /*************************************************************************************/
    /* increment interrupt nesting level and thread dispatch level to prevent dispatches */
	/*************************************************************************************/

    /* Increment the pharosIsInsideInterrupt level (so we can detect nested interrupts or non-nested interrupts when we enable interrupts next) */
	ldr     %r1, =pharosVInterruptNestingLevel
    
    /* set the nest level to 1 */
	mov     %r2, #1

    /* store the pharosVInterruptNestingLevel = 1 */
	str     %r2, [%r1]

    /* Increment thread dispatch disable level (disable dispatch inside the interrupt handler) */
	ldr     r2, =pharosVDispatchLevel
	ldr     r1, [r2]
	add     r1, #1
	str     r1, [r2]

    /*************************************************/
    /* Call BSP dependent interrupt dispatcher       */
	/*************************************************/

    /* switch the supervisor stack to the IRQ stack */
    ldr     %sp , =pharosIrqSupervisorStack + IRQ_SUPERVISOR_STACK_SIZE

    /* call interrupt handler (interrupts are disabled when calling this function. 
     * This function should enable interrrutps as soon as possible) */
	blx     pharosArmCInterruptHandler

    /* disable interrupts */
    msr     cpsr_c , #(CPSR_SUPERVISOR_MODE | CPSR_DISABLE_IRQ)

    /* decrement the dispatch level (enable dispatch) */
	ldr     r1, =pharosVDispatchLevel
    ldr     r0, [r1]
	sub     r0, #1
	str     r0, [r1]
    
    /* interrupts will be disabled until the end of the interrupt so this is a safe place to state that we are no longer inside an IRQ */
	ldr     r1, =pharosVInterruptNestingLevel
    ldr     r0, [r1]
	sub     r0, #1
	str     r0, [r1]

    /* check if there is a need to dispatch again (to r0) */
    ldr     r0, =pharosVNeedsThreadContextSwitch
    ldr     r0, [r0]
	
    /* if there is no need for a context switch */
    cmp     r0, #0

    /* then leave */
    beq     leaveOutermostInterrupt

    /* check if thread dispatch is enabled */
    ldr     r0, =pharosVDispatchLevel
    ldr     r0, [r0]
	
    /* cannot dispatch threads at this moment */
    cmp     r0, #0
    bne     leaveOutermostInterrupt

    /* set the argument of the pharosIContextSwitch function */
    ldr     r0, =pharosVRunningThread
    ldr     r0, [r0]

    /* restore the previous stack pointer value */
    ldr     %sp ,  [%r0 , #(OFFSET_IRQ_FRAME + OFFSET_IRQ_FRAME_SP_SUPERVISOR)]

	/* dispatch the thread inside the interrupt */
	blx      pharosIContextSwitch
    
leaveOutermostInterrupt:

    /*********************************************************************/
    /* restore the running thread registers                              */
    /*********************************************************************/
    
    /* load to %r0 the pointer to the address of the running thread */
    ldr     %r0, =pharosVRunningThread

    /* r0 = address of the running thread CPU context */
    ldr     %r0 , [%r0]

    /* only compile this code if FPU is supported */
#ifdef PHAROS_CPU_HAS_VFP

    /* make r1 point to the interrrupt part of the thread control block to the floating point registers */
    add     %r1 , %r0 , #(OFFSET_IRQ_FRAME + OFFSET_IRQ_FRAME_S0)

    /* restore the floating point registers */
    vldm    r1! , {s0 - s31}  

    /* get the special FPU registers */
    ldmia	r1!, {%r4 , %r5 , %r6 , %r7}

    /* restore the thread FPEXC */
    vmsr    FPEXC, %r4

    /* restore the thread FPSCR */
    vmsr    FPSCR, %r5

    /* restore the thread FPINST */
    vmsr    FPINST, %r6

    /* restore the thread FPINST2 */
    vmsr    FPINST2 , %r7

#endif

    /*********************************************************************/
    /* restore the supervisor registers                                  */
    /*********************************************************************/

    /* make r2 point to the interrrupt part of the thread control block to the SPCR */
    add     %r2 , %r0 , OFFSET_IRQ_FRAME + OFFSET_IRQ_FRAME_SP_SUPERVISOR

    /* get the supervisor sp , lr and SPSR (if a context switch is needed these need to be restored afterwards) */
    ldmia   %r2 , {%r1 , %r2 , %r3}
    
    /* restore the sp_supervisor */
    mov     %sp , %r1
    
    /* restore the lr_supervisor */
    mov     %lr , %r2 

    /* restore the original SPCR_supervisor */
    msr     spsr, %r3

    /*********************************************************************/
    /* restore the IRQ registers                                         */
    /*********************************************************************/

    /* switch to back IRQ mode */
    msr     cpsr_c , #(CPSR_IRQ_MODE | CPSR_DISABLE_IRQ)

    /* make r0 point to the interrrupt part of the thread control block to the SPCR */
    add     %r0 , %r0 , OFFSET_IRQ_FRAME + OFFSET_IRQ_FRAME_SPSR

    /* get the original SPCR, TLB (won't actually be used) and r3 to r12 and lr registers */
    ldmia   %r0! , {%r1 , %r2 , %r3-%r12 , %lr}

    /* restore the original SPCR */
    msr     spsr, %r1

    /* restore the original r0 and r1 and r2 registers */
    ldmia   %r0 , {%r0 , %r1 , %r2}

    /* at this point the registers in IRQ mode have been restored */

    /*********************************************************************/
    /* restore the running thread TLB                                    */
    /*********************************************************************/
    
    /* save the original r0 to the IRQ stack */
    push    {%r0}

    /* load to %r0 the pointer to the address of the running thread */
    ldr     %r0, =pharosVRunningThread

    /* r0 = address of the running thread CPU context */
    ldr     %r0 , [%r0]

    /* get the running thread TLB */
    ldr     %r0 , [%r0 , #(OFFSET_IRQ_FRAME + OFFSET_IRQ_FRAME_TLB)]

    /* program the c2 register to place the thread TBL address */
    mcr     p15, 0 , %r0 , c2 , c0 , 0
    
    /* restore the original %r0 */
    pop     {%r0}

    /* Return from interrupt */
	subs	pc, lr, #4


nestedInterrupt:
    /* if we are in a nested interrupt the previous mode was supervisor and the stack is inside the pharosIrqSupervisorStack */

    /* at this point r0, r1 and r2 have been pushed to the IRQ stack */

    pop     {%r0 , %r1 , %r2}

    /* switch to supervisor mode to save its registers */
    msr     cpsr_c , #(CPSR_SUPERVISOR_MODE| CPSR_DISABLE_IRQ)

    /* save all remaining the registers to the IRQ stack */
    push    {%r0 - %r12 , %lr}

    /* get the SPSR_supervisor to r6 */
    mrs     %r6 , SPSR

    /* store the supervisor SPSR (stack pointer is assumed to be correct after calling the interrupt routines) */
    push    {%r6}
    
    /* switch to irq mode to save its registers */
    msr     cpsr_c , #(CPSR_IRQ_MODE| CPSR_DISABLE_IRQ)

    /* save SPSR_irq to r0 */
    mrs     %r0 , SPSR

    /* save lr_irq to r1 */
    mov     %r1 , %lr

    /* switch to supervisor mode to save its registers */
    msr     cpsr_c , #(CPSR_SUPERVISOR_MODE| CPSR_DISABLE_IRQ)

    /* save the SPSR_irq and lr_irq to the supervisor stack */
    push    {%r0 , %r1}

    /* status of the IRQ stack at this point:
     *  lr_irq      <- current sp_super
     *  SPSR_irq
     *  SPSR_super
     *  lr_super    
     *  r12
     *  r11
     *  r10
     *  r9
     *  r8
     *  r7
     *  r6
     *  r5
     *  r4
     *  r3
     *  r2
     *  r1
     *  r0          <- initial sp_super
     */
    
    /* load the interrupt address that was interrupted */   
    mov     %r0 , %lr

    /*************************************************/
    /*    Call BSP dependent interrupt dispatcher    */
	/*************************************************/

    /* call interrupt routine with interrupts enabled */
	blx     pharosArmCInterruptHandler

returnNested:

    /* get the original values for irq mode */
    pop     {%r0 , %r1}

    /* switch to irq mode to restore its registers */
    msr     cpsr_c , #(CPSR_IRQ_MODE| CPSR_DISABLE_IRQ)
    
    /* restore SPSR_irq */
    msr     SPSR , %r0 

    /* restore lr_irq */
    mov     %lr , %r1

    /* switch to supervisor to restore its registers */
    msr     cpsr_c , #(CPSR_SUPERVISOR_MODE | CPSR_DISABLE_IRQ)
    
    /* get original SPSR_super to %r6 */
    pop     {%r6}

    /* restore SPSR_super */
    msr     SPSR , %r6 

    /* restore registers of supervisor mode */
    pop     {%r0 - %r12 , %lr}

    /* switch to irq mode to restore its registers */
    msr     cpsr_c , #(CPSR_IRQ_MODE| CPSR_DISABLE_IRQ)
    
    /* Return from interrupt */
	subs	pc, lr, #4

.endfunc


.arm
.func pharosCpuUndefHandler
pharosCpuUndefHandler:
    b       pharosCpuDataAbortHandler
.endfunc


.arm
.func pharosCpuFiqHandler
pharosCpuFiqHandler:
    b  .
.endfunc
