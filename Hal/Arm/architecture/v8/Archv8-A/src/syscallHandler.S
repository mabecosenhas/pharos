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
#include <pharos/kernel/core/syscall.h>
#include <pharos/hal/cpu/conf.h>


/**
 * @file
 *
 * @brief Boot and system start code in ARM mode.
 */

/* External symbols */
#include <pharos/hal/cpu/asm.h>

.type pharosCpuSyscallHandler,                          %function 
.type pharosCpuDataErrorHandler,                        %function

.global pharosCpuSyscallHandler
.global pharosCpuDataErrorHandler

.section .text



.func pharosCpuDataErrorHandler
pharosCpuDataErrorHandler:

    /* restore the x8 and x9 (at this point the sp is restored to its original value) */
    ldp     x8 , x9 , [sp] , #16

    /* save the register that might change when we call a C function to the current stack 
     * (at this point may be the running thread stack or the interrupt stack) */
    stp		x0 , x1 , [sp, #-16]!
    stp		x2 , x3 , [sp, #-16]!
    stp		x4 , x5 , [sp, #-16]!
    stp		x6 , x7 , [sp, #-16]!  

    /* also save some registers that are maintained accross C calls to store the stack pointer */
    stp		x9 , x10 , [sp, #-16]!  

    /* store x30 (will be modified once we call a C function). x29 will have the core number */
    stp		x29 , x30 , [sp, #-16]!  

    /****************************************************************/
    /* increment thread dispatch level                              */
    /****************************************************************/

    /* get the runnning core to x10 */
    mrs     x10 , mpidr_el1

    /* load the mask to x1 to leave only the running core number */
    mov     x1 , PHAROS_CPU_MPIDR_AFFINITY_MASK

    /* clear all but the running core */
    bic     x29 , x10 , x1

    /* get the index on the array of the running core to x29 (it will remain until the end of the function) */
    lsl     x29 , x29 , PHAROS_CPU_CORE_RMP_SIZE_LOG2

    /* get the address where the thread dispatch level for each core */
    ldr     x1 , =pharosVRmpCore

    /* add the dispatch level offset */
    add     x1 , x1 , PHAROS_CORE_TABLE_DISPATCH_LEVEL_OFFSET

    /* calculate the final address where the thread dispatch level for the running core is on */
    add     x2 , x29 , x1
    
    /* load the thread dispatch disable level */
	ldr     x1 , [x2]

    /* Increment thread dispatch disable level (disable dispatch inside the interrupt handler) */
	add     x1 , x1 , #1

    /* store it again */
	str     x1 , [x2]

    /****************************************************************/
    /* save SPSR and ELR to stack                                   */
    /****************************************************************/

    /* get the ELR EL1 to x0 */
	mrs		x0 , ELR_EL1

    /* get the SPSR EL1 to x3 (will be the argument to pharosCpuCInterruptHandler) */
    mrs		x3 , SPSR_EL1

    /* read the FAR address to x1 (will be the memAddress argument to the pharosIInvalidMemAccess call) */
    mrs     x1 , FAR_EL1

    /* push SPSR and ELR to interrupt stack */
    stp     x0 , x3 , [sp , #-16]!
    
    /****************************************************************/
    /* call abort handler                                           */
    /****************************************************************/

    /* enable interrupts (IRQ + FIQ) since we already saved the SPSR and ELR */
    //msr     DAIFClr , #0b0011

    /* call the abort handler (x0 has already been set to the instruction address and x1 has the memAddress) */
	bl      pharosIInvalidMemAccess

    /* disable interrupts (IRQ + FIQ) to prevent an interrupt to mess with SPSR and ELR */
    msr     DAIFSet , #0b0011

    /****************************************************************/
    /* decrement thread dispatch level                              */
    /****************************************************************/

    /* get the address where the thread dispatch level for each core */
    ldr     x1 , =pharosVRmpCore
 
    /* add the thread dispatch level offset */
    add     x1 , x1 , PHAROS_CORE_TABLE_DISPATCH_LEVEL_OFFSET

    /* calculate the final address where the thread dispatch level for the running core is on */
    add     x2 , x29 , x1
    
	/* load the thread dispatch disable level */
	ldr     x1 , [x2]

    /* decrement thread dispatch disable level (disable dispatch inside the interrupt handler) */
	sub     x1 , x1 , #1

    /* store it again */
	str     x1 , [x2]

    /****************************************************************/
    /* thread dispatch                                              */
    /****************************************************************/

    /* enable interrupts (IRQ + FIQ) since we already saved the SPSR and ELR */
    msr     DAIFClr , #0b0011

    /* dispatch the thread inside the interrupt. A dispatch may be needed in case of
     * the thread is suspended due to the invalid memory access */
	bl      pharosIDispatchThread

    /* will only reach here if the thread that originated the fault is using a direct I/O */

    /* disable interrupts (IRQ + FIQ) to prevent an interrupt to mess with SPSR and ELR */
    msr     DAIFSet , #0b0011

    /* restore the registers saved to the stack */
     
    /* get ELR and SPSR from stack  */
    ldp     x2 , x3 , [sp] , #16

    /*********************************************************************/
    /* Get the running thread of the running core to x0                  */
    /*********************************************************************/

    /* get the address of the array of running threads (same address as pharosVRmpCore) */
    ldr     x1 , =pharosVRmpCore

    /* calculate the final address where the running thread for the running core is on */
    add     x0 , x29 , x1

    /* get the running thread */
    ldr     x0 , [x0]

    /****************************************************************/
    /* set the return address (ELR) to execute the IO direct memory */
    /****************************************************************/

    /* set the return address to the direct I/O memory access */
    add     x0 , x0 , #OFFSET_DIRECT_IO_SAVE_ELR

    /* save to this location the ELR so that we can restore it later on (see userModeSyscallRestoreTlb) and increment x0
     * so that x0 points to OFFSET_IO_ACCESS  */
    str     x2 , [x0]

    add     x0 , x0 , #8
 
    /* set the return address to the IO direct memory */
	msr		ELR_EL1 , x0

    /*********************************************************************/
    /* change SPSR to return in EL1 mode to execute the IO direct memory */
    /*********************************************************************/

    /* x3 contains the original SPSR at this point, we will set it to return to EL1 mode now */
    orr     x3 , x3 , #4

    /* set the SPSR to return from the exception to the EL1 mode (all remaining flags are unchanged) */
    msr		SPSR_EL1 , x3
   
    /* restore remaining registers from stack */
    ldp		x29 , x30 , [sp] , #16 
    ldp		x9 , x10 , [sp] , #16 
    ldp		x6 , x7 , [sp] , #16  
    ldp		x4 , x5 , [sp] , #16
    ldp		x2 , x3 , [sp] , #16
    ldp		x0 , x1 , [sp] , #16
    
    /* return from interrupt (will re-enable interrupts since it will restore SPSR) */
    eret

.endfunc


.func pharosCpuSyscallHandler
pharosCpuSyscallHandler:

    /* when we reach here we are running at EL1, 
     * stack pointer is currently pointing to the running thread kernel stack 
     * interrupts are disabled 
     * in case of a syscall, x0-x7 have inputs to the syscall 
     * in case of a data abort, ELR contains the return address */

    /* there are no arguments to restore from the EL0 stack, every argument is inside x0 to x6 */
    
    /* use x8 to store the svc number issued (in case of restore TLB, x8 was previously saved, in other cases was not used) */
    stp		x8  , x9  , [sp, #-16]!

    /* find out if the exception is a syscall or a data abort */
    mrs     x8 , ESR_EL1

    /* get only the EC bits */
    lsr     x8 , x8 , #26
    
    /* if the EC == 0x24, then its a data abort */
    cmp     x8 , #0x24

    /* execute error handler (and don't return) */
    beq     pharosCpuDataErrorHandler

    /* save ELR to x8 */
    mrs		x8 , ELR_EL1

    /* get the assembly instruction executed to x8 */
    ldr     x8 , [x8 , #-4]

    /* shift right for 5 bits to remove the unwanted bits */
    lsr     x8 , x8 , #5

    /* clear all unwanted bits. This way we get in x8 the syscall number invoked */
    and     x8 , x8 , 0xffff

    /* check if it is the special case of restoring the TLB */
    cmp     x8 , PHAROS_SYSCALL_RESTORE_TLB

    /* if it is then restore the TLB */
    beq     userModeSyscallRestoreTlb

    /* only the restore TLB does not need to re-enable interrupts, for the other cases we must save ELR_EL1 and SPSR_El1 */

    /* if we are here then the caller has already saved x8-x9 to its stack */

    /* save x10 and x11 to the stack as well */
    stp		x10 , x11 , [sp, #-16]!

    /* get the SPSR EL1 to x8 */
    mrs		x9 , SPSR_EL1

    /* get the ELR EL1 to x9 */
	mrs		x10 , ELR_EL1

    /* push SPSR and ELR to stack */
    stp     x9 , x10 , [sp , #-16]!

    /* X8 = x8 * 8 bytes (to give the offset of the system call to be invoked) */
    lsl     x8 , x8 , 3

    /************************************************************************************************/
    /* "normal" syscall and Internal syscall made inside user mode (e.g. leaving a resource filter) */
    /************************************************************************************************/

    /* store in x10 the vector array of the method to call */
    ldr     x10 , =pharosVSystemCalls
    
    /* increase the pointer to the vector index being called */
    add     x10 , x10 , x8

    /* x10 now contains the address of the function to be called */
    ldr     x10 , [x10]

    /* store x30 (return address) because the call will change it */
    stp     x29 , x30 , [sp , #-16]!

    /* if the syscall is not for a API call */
    cmp     x8 , (PHAROS_SYSCALL_API_LAST_CALL << 3)

    /* just call the corresponding function (if x0 > PHAROS_SYSCALL_API_LAST_CALL) */
    bhi     userModeInternalSyscall   

    /************************************************************************************************/
    /* restore the previous interrupt level                                                         */
    /* (if interrupts were disabled, keep them disabled, we could be on a critical access method)   */
    /************************************************************************************************/

    /* check if the interrupts were enabled (x9 has the SPSR) */
    and     x9 , x9 , (1 << 7) | (1 << 6)
    
    /* check if any bit was set */
    cmp     xzr , x9

    /* if it was set, then don't enable interrupts */
    bne     dontEnableInterrupts

    /* enable interrupts (somehow, enabling interrupts before breaks the code (517) even 
     * though register x8 is saved on pharosCpuIrqHandler */
    msr     DAIFClr , #0b0011

dontEnableInterrupts:

    /* call the syscall */
    blr     x10

    /* restore x30 */
    ldp     x29 , x30 , [sp] , #16

    /* pop SPSR and ELR from stack */
    ldp     x9 , x10 , [sp] , #16

    /* disable interrupts (IRQ + FIQ) to prevent an interrupt to mess with SPSR and ELR */
    msr     DAIFSet , #0b0011

    /* restore SPSR */
    msr     SPSR_EL1 , x9
    
    /* restore ELR */
    msr     ELR_EL1 , x10

    /* restore the registers from stack */
    ldp     x10 , x11 , [sp] , #16

    /* restore the registers from stack */
    ldp     x8 , x9 , [sp] , #16

    /* return from syscall */
    eret

    /*********************************************************************/
    /* special case of restoring the TLB                                 */
    /*********************************************************************/
userModeSyscallRestoreTlb:

    /* we have to restore the SPSR to EL0 (we are in EL1) and the original ELR */
    
    /* get the SPSR EL1 to x9 */
    mrs		x9 , SPSR_EL1

    /* clear the EL1 bit so that we return to EL0 */
    bic     x9 , x9 , #4

    /* set the SPSR so that we return to EL0 */
    msr     SPSR_EL1 , x9

    /* push x0 and x1 */
    stp		x0 , x1 , [sp, #-16]!

    /*********************************************************************/
    /* Get the running thread of the running core to x0                  */
    /*********************************************************************/

    /* get the runnning core to x0 */
    mrs     x0 , mpidr_el1

    /* load the mask to x1 to leave only the running core number */
    mov     x1 , PHAROS_CPU_MPIDR_AFFINITY_MASK

    /* clear all but the running core */
    bic     x0 , x0 , x1

    /* get the index on the array of the running core */
    lsl     x0 , x0 , PHAROS_CPU_CORE_RMP_SIZE_LOG2

    /* get the address of the array of running threads */
	ldr     x1 , =pharosVRmpCore

    /* calculate the final address where the running thread pointer for the running core is on */
    add     x0 , x0 , x1

    /* get the running thread */
    ldr     x9 , [x0]

    /* pop x0 and x1 */
    ldp     x0 , x1 , [sp] , #16

    /* set the return address to the direct I/O memory access */
    add     x9 , x9 , #OFFSET_DIRECT_IO_SAVE_ELR

    /* get to x9 the return address */
    ldr     x9 , [x9]

    /* execute the following instruction */
    add     x9 , x9 , 4

    /* restore the ELR */
    msr     ELR_EL1 , x9

    /* restore the registers from stack */
    ldp     x8 , x9 , [sp] , #16
 
    /* return from syscall */
    eret 


userModeInternalSyscall:

    /* in user mode API internal syscall at least x8 and x9 have been saved previously */

    /* store two registers to the stack */
    stp     x0 , x1 , [sp,#-16]!

    /* place the argument to the function */
    mrs     x0 , ELR_EL1

    /* place the second argument to the function */
    mrs     x1 , SPSR_EL1

    /* call the syscall */
    blr     x10

    /* disable interrupts (IRQ + FIQ) to prevent an interrupt to mess with SPSR and ELR */
    msr     DAIFSet , #0b0011

    /* restore SPSR with the value returned from the function */
    msr     SPSR_EL1 , x0
    
    /* restore x0 */
    ldp     x0 , x1 , [sp] , #16

    /* restore x30 */
    ldp     x29 , x30 , [sp] , #16

    /* pop SPSR and ELR from stack */
    ldp     x9 , x10 , [sp] , #16
   
    /* restore ELR */
    msr     ELR_EL1 , x10

    /* restore the registers from stack */
    ldp     x10 , x11 , [sp] , #16

    /* restore the registers from stack */
    ldp     x8 , x9 , [sp] , #16

    /* return from syscall */
    eret

.endfunc
