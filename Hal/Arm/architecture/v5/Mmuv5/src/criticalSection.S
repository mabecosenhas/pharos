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


.type pharosCpuCriticalSectionExecute   , %function
.type pharosCpuCriticalSectionEnded        , %function

.global pharosCpuCriticalSectionExecute
.global pharosCpuCriticalSectionEnded

.arm
.syntax unified
.text

/**
 * uint32_t pharosCpuCriticalSectionExecute(void *data , uint32_t arg1, uint32_t arg2 , uint32_t arg3 , uint32_t arg4 , ptrCpuMmuContext context ,CriticalSectionHandler handler);
 *
 * r0 - data
 * r1 - arg1
 * r2 - arg2
 * r3 - arg3
 * [sp +  0] - arg4
 * [sp +  4] - context
 * [sp +  8] - handler
 * 
 */
.func pharosCpuCriticalSectionExecute
pharosCpuCriticalSectionExecute:

    /* when we arrive here we are at supervisor mode with its sp and lr registers */

    /* save remaining registers now (offset of stack will be 10*4 = 40) */
    push    {%r4 - %r12 , %lr}

    /* save the original supervisor sp to r4 */
    add     %r4 , %sp , #40

    /* get to %r10 the arg4 (will be pushed to the user stack) */
    ldr     %r10 , [%r4, #0]

    /* get to %r6 the filter function to be called */
    ldr     %r6 , [%r4, #8]

    /* get to %r7 the new tlb (we have to switch to user mode) */
    ldr     %r7 , [%r4, #4]
    ldr     %r7 , [%r7]
   
    /* save current TLB to r9 */
    mrc     p15, 0 , %r9 , c2 , c0 , 0

    /* save TLB to supervisor stack to be protected */
    push    {%r9}

    /*******************************************************************************************************************/
    /* Switch stack (including copying arguments from supervisor to user stack), TLB and finally CPU mode to user mode */
    /*******************************************************************************************************************/

    /* change to system mode */
    msr     cpsr_c , #(CPSR_SYSTEM_MODE | CPSR_DISABLE_IRQ)

    /* the arguments %r0-r3 have already the correct value */
    
    /* switch TLB using r7 that contains the new tlb */
    mcr     p15, 0 , %r7 , c2 , c0 , 0

    /* flush TLB */
    mcr     p15, 0, %r7, c8, c7, 0

    /* re-enable interrupts for a small amount of time */
    msr     cpsr_c , #(CPSR_SYSTEM_MODE)

    /* change to user mode with interrupts disabled */
    msr     cpsr_c , #(CPSR_USER_MODE | CPSR_DISABLE_IRQ)

    /* place in the user stack the arg4 (save an extra register to keep the stack correctly aligned) */
    push    {%r10 , %r11}

    /* call the filter function */
    /* uint32_t handler(void *data , uint32_t arg1 , uint32_t arg2 , uint32_t arg3 , uint32_t arg4); */
    blx     %r6

    /* restore the previous stack position */
    pop     {%r10 , %r11}

    /*************************************************************************/
    /* Switch to supervisor mode with a syscall made in a well-known address */
    /*************************************************************************/

    /* prepare the call to the syscall */
    push    {%r0 , %r4-%r12}

    /* now switch to supervisor mode using the syscall */
    swi     PHAROS_SYSCALL_SUPERVISOR_CRITICAL_SECTION_EXECUTE_END

    /* address where it is supposed to go after syscall */
criticalSectionExecuteEndCall:

    /* change to system mode to restore the registers that were saved in user mode */
    msr     cpsr_c , #(CPSR_SYSTEM_MODE | CPSR_DISABLE_IRQ)

    /* restore the registers before the syscall */
    pop     {%r0 , %r4-%r12}

    /*************************************************************************/
    /* Switch to kernel TLB, restore sp_user to its original value           */
    /*************************************************************************/

    /* switch to kernel TLB so we can access to the supervisor stack for sure */

    /* load to r2 the address of the kernel TLB */
    ldr     r2, =kernelSectionTable

    /* program the c2 register to place the kernel TBL address */
    mcr     p15, 0 , r2, c2 , c0 , 0
  
    /* flush the current TLB */
    mcr     p15, 0, r2, c8, c7, 0

    /* change back to supervisor mode */
    msr     cpsr_c , #(CPSR_SUPERVISOR_MODE | CPSR_DISABLE_IRQ)

    /*************************************************************************/
    /* Switch to thread TLB, restore saved saved registers                   */
    /*************************************************************************/

    /* now can access the supervisor stack */
    pop     {%r9}

    /* switch TLB to its original value using r9 */
    mcr     p15, 0 , %r9 , c2 , c0 , 0

    /* flush TLB */
    mcr     p15, 0, %r9, c8, c7, 0

    /* restore the used registers (can do this after the TLB has changed to save some nop cycles) */
    pop     {%r4 - %r12 , %lr}

    /* change back to supervisor mode */
    msr     cpsr_c , #(CPSR_SUPERVISOR_MODE)

    /* return from function */
	bx      %lr

.endfunc

/**
 * void pharosCpuCriticalSectionEnded(uint8_t *address , uint32_t SPSR);
 */ 
.func pharosCpuCriticalSectionEnded
pharosCpuCriticalSectionEnded:

    /* load to r1 the address in which this function is supposed to be called from */
    ldr     %r2 , =criticalSectionExecuteEndCall

    /* if the address is not correct */
    cmp     %r0 , %r2
    
    /* then leave and don't do anything */
    bne     pharosCpuCriticalSectionEndedLeave
    
    /* clear the mode from the SPSR */
    and     %r1 , %r1 , #(CPSR_CPU_MODE_MASK)

    /* place the new supervisor mode */
    orr     %r0 , %r1 , #(CPSR_SUPERVISOR_MODE)

    /* else, leave the SPSR as is */
pharosCpuCriticalSectionEndedLeave:

    /* leave and restore ARM/Thumb mode */
    bx      lr

.endfunc
  
