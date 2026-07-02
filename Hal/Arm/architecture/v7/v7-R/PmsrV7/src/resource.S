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

.type pharosCpuResourceExecuteAccessLow, %function
.type pharosCpuResourceExecuteFilterLow , %function
.type pharosCpuResourceFilterEnded , %function
.type pharosCpuResourceAccessEnded , %function

.global pharosCpuResourceExecuteAccessLow
.global pharosCpuResourceExecuteFilterLow
.global pharosCpuResourceFilterEnded
.global pharosCpuResourceAccessEnded

.syntax unified
.text


/**
 * PharosResourceFilterR pharosCpuResourceExecuteFilter(ResourceNumber id , PartitionNumber callerId , uint64_t input , ResourceFilterMethod filter , ptrCpuMmuContext context , StackPointer sharedStack);
 * 
 * r0 - resource number 
 * r1 - caller partition number
 * r2 - input
 * r3 - input
 * [sp +  0] - filter
 * [sp +  4] - context
 * [sp +  8] - sharedStack
 */
.func pharosCpuResourceExecuteFilterLow
pharosCpuResourceExecuteFilterLow:

    /***********************************************************************************/
    /* Save current registers to supervisor stack and get arguments to local registers */
    /***********************************************************************************/

    /* when we arrive here we are at supervisor mode with its sp and lr registers */

    /* save remaining registers now (offset of stack will be 10*4 = 40) */
    push    {%r4 - %r12 , %lr}

    /* save the original supervisor sp to r4 */
    add     %r4 , %sp , #40

    /* get to %r5 the stack to be used*/
    ldr     %r5 , [%r4, #8]

    /* get to %r6 the filter function to be called */
    ldr     %r6 , [%r4, #0]

    /* save the floating point registers */
    vpush   {%d0 - %d15}

    /*******************************************************************************************************************/
    /* Switch stack (including copying arguments from supervisor to user stack), TLB and finally CPU mode to user mode */
    /*******************************************************************************************************************/

    /* change to system mode */
    msr     cpsr_c , #(CPSR_SYSTEM_MODE | CPSR_DISABLE_IRQ)

    /* the arguments %r0-r3 have already the correct value */
    
    /* save the user mode sp to r8 */
    mov     %r8 , %sp

    /* switch the sp */
    mov     %sp , %r5

    /* change back to supervisor mode */
    msr     cpsr_c , #(CPSR_SUPERVISOR_MODE | CPSR_DISABLE_IRQ)

    /* save the user mode stack to the supervisor stack */
    push    {%r8}

    /* change to user mode */
    msr     cpsr_c , #(CPSR_USER_MODE)

    /* call the filter function */
    /* PharosResourceFilterResult resourceFilterMethod(ResourceNumber resourceId , PartitionNumber callerId , uint64_t input); */
    blx     %r6

    /*************************************************************************/
    /* Switch to supervisor mode with a syscall made in a well-known address */
    /*************************************************************************/

    /* prepare the call to the syscall */
    push    {%r0 , %r4-%r12 , %lr}

    /* now switch to supervisor mode using the syscall */
    swi     PHAROS_SYSCALL_SUPERVISOR_RESOURCE_EXECUTE_FILTER_ENDED

    /* address where it is supposed to go after syscall */
resourceFilterEndCall:

    /* here we are in supervisor mode */

    /* get the user stack */
    pop     {%r8}

    /* change to system mode to restore the registers that were saved in user mode */
    msr     cpsr_c , #(CPSR_SYSTEM_MODE | CPSR_DISABLE_IRQ)

    /* restore the user mode sp */
    add     %sp , %r8 , -(11*4)

    /* restore the registers before the syscall (using sp_user) */
    pop     {%r0 , %r4-%r12 , %lr}

    /* change back to supervisor mode */
    msr     cpsr_c , #(CPSR_SUPERVISOR_MODE | CPSR_DISABLE_IRQ)

    /* restore the floating point registers */
    vpop    {%d0 - %d15}  

    /* restore the used registers (can do this after the TLB has changed to save some nop cycles) */
    pop     {%r4 - %r12 , %lr}

    /* change back to supervisor mode */
    msr     cpsr_c , #(CPSR_SUPERVISOR_MODE)

    /* return from function */
	bx      %lr

.endfunc


/**
 * uint64_t pharosCpuResourceExecuteAccess(uint64_t input , ResourceAccessMethod method , ptrCpuMmuContext context , StackPointer sharedStack);
 * 
 * r0 - input
 * r1 - input
 * r2 - method
 * r3 - context
 * [sp +  0] - sharedStack
 */
.func pharosCpuResourceExecuteAccessLow
pharosCpuResourceExecuteAccessLow:


    /***********************************************************************************/
    /* Save current registers to supervisor stack and get arguments to local registers */
    /***********************************************************************************/

    /* when we arrive here we are at supervisor mode with its sp and lr registers */

    /* save remaining registers now (offset of stack will be 10*4 = 40) */
    push    {%r4 - %r12 , %lr}

    /* save the original supervisor sp to r4 */
    add     %r4 , %sp , #40

    /* get to %r5 the stack to be used*/
    ldr     %r5 , [%r4, #0]

    /* store the method in %r6 */
    mov     %r6 , %r2

    /* save the floating point registers */
    vpush   {%d0 - %d15}

    /*******************************************************************************************************************/
    /* Switch stack (including copying arguments from supervisor to user stack), TLB and finally CPU mode to user mode */
    /*******************************************************************************************************************/

    /* change to system mode */
    msr     cpsr_c , #(CPSR_SYSTEM_MODE | CPSR_DISABLE_IRQ)

    /* save the user mode sp to r8 */
    mov     %r8 , %sp

    /* switch the sp */
    mov     %sp , %r5

    /* change back to supervisor mode */
    msr     cpsr_c , #(CPSR_SUPERVISOR_MODE | CPSR_DISABLE_IRQ)

    /* save the user mode stack to the supervisor stack */
    push    {%r8}

    /* change to user mode */
    msr     cpsr_c , #(CPSR_USER_MODE)

    /* call the resource access function */
    blx     %r6

    /*************************************************************************/
    /* Switch to supervisor mode with a syscall made in a well-known address */
    /*************************************************************************/

    /* also push r1 which belongs to the return value of the resource access function */
    push    {%r1}

    /* prepare the call to the syscall */
    push    {%r0 , %r4-%r12 , %lr}

    /* now switch to supervisor mode using the syscall */
    swi     PHAROS_SYSCALL_SUPERVISOR_RESOURCE_EXECUTE_ACCESS_ENDED

    /* address where it is supposed to go after syscall */
resourceAccessEndCall:

    /* here we are in supervisor mode */

    /* get the user stack */
    pop     {%r8}

    /* change to system mode to restore the registers that were saved in user mode */
    msr     cpsr_c , #(CPSR_SYSTEM_MODE | CPSR_DISABLE_IRQ)

    /* restore the user mode sp */
    add     %sp , %r8 , -(12*4)

    /* restore the registers before the syscall (using sp_user) */
    pop     {%r0 , %r4-%r12 , %lr}

    /* restore the r1 register which may have been changed by the syscall */
    pop     {%r1}

    /*************************************************************************/
    /* Switch to kernel TLB                                                  */
    /*************************************************************************/

    /* change back to supervisor mode */
    msr     cpsr_c , #(CPSR_SUPERVISOR_MODE | CPSR_DISABLE_IRQ)

    /*************************************************************************/
    /* Switch to thread TLB, restore saved saved registers                   */
    /*************************************************************************/

    /* restore the floating point registers */
    vpop    {%d0 - %d15}  

    /* restore the used registers (can do this after the TLB has changed to save some nop cycles) */
    pop     {%r4 - %r12 , %lr}

    /* change back to supervisor mode */
    msr     cpsr_c , #(CPSR_SUPERVISOR_MODE)

    /* return from function */
	bx      %lr

.endfunc


/**
 * CpuModeChangeContext pharosCpuResourceFilterEnded(uint8_t *address , CpuModeChangeContext context);
 */
.func pharosCpuResourceFilterEnded
pharosCpuResourceFilterEnded:

    /* load to r1 the address in which this function is supposed to be called from */
    ldr     %r2 , =resourceFilterEndCall

    /* if the address is not correct */
    cmp     %r0 , %r2
    
    /* then leave and don't do anything */
    bne     pharosCpuResourceFilterEndedLeave
    
    /* clear the mode from the SPSR */
    and     %r1 , %r1 , #(CPSR_CPU_MODE_MASK)

    /* place the new supervisor mode */
    orr     %r0 , %r1 , #(CPSR_SUPERVISOR_MODE)

    /* else, leave the SPSR as is */
pharosCpuResourceFilterEndedLeave:

    /* leave and restore ARM/Thumb mode */
    bx      lr

.endfunc


/**
 * CpuModeChangeContext pharosCpuResourceAccessEnded(uint8_t *address , CpuModeChangeContext context);
 */
.func pharosCpuResourceAccessEnded
pharosCpuResourceAccessEnded:

    /* load to r1 the address in which this function is supposed to be called from */
    ldr     %r2 , =resourceAccessEndCall

    /* if the address is not correct */
    cmp     %r0 , %r2
    
    /* then leave and don't do anything */
    bne     pharosCpuResourceAccessEndedLeave
    
    /* clear the mode from the SPSR */
    and     %r1 , %r1 , #(CPSR_CPU_MODE_MASK)

    /* place the new supervisor mode */
    orr     %r0 , %r1 , #(CPSR_SUPERVISOR_MODE)

    /* else, leave the SPSR as is */
pharosCpuResourceAccessEndedLeave:

    /* leave and restore ARM/Thumb mode */
    bx      lr

.endfunc
