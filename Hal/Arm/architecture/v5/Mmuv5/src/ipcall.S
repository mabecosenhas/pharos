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

.type pharosCpuExecuteIpCallFilter,     %function
.type pharosCpuExecuteIpCallIo,         %function
.type pharosCpuExecuteIpCallO,          %function
.type pharosCpuIpCallIoEnded ,          %function
.type pharosCpuIpCallFilterEnded,       %function
.type pharosCpuIpCallOEnded,            %function

.global pharosCpuExecuteIpCallFilter
.global pharosCpuExecuteIpCallIo
.global pharosCpuExecuteIpCallO
.global pharosCpuIpCallIoEnded
.global pharosCpuIpCallFilterEnded
.global pharosCpuIpCallOEnded

.syntax unified
.text
    
     
/**
 * PharosIpCallR pharosCpuExecuteIpCallFilter(void *partitionData , PartitionNumber partition , uint8_t *input , uint8_t *stack ,  ptrCpuMmuContext context , void *filterFunction);
 * 
 * r0 - partitionData
 * r1 - partition
 * r2 - input
 * r3 - stack
 * [sp +  0] - context
 * [sp +  4] - filterFunction
 */
.arm
.func pharosCpuExecuteIpCallFilter
pharosCpuExecuteIpCallFilter:

    /* when we arrive here we are at supervisor mode with its sp and lr registers */

    /* save remaining registers now (offset of stack will be 10*4 = 40) */
    push    {%r4 - %r12 , %lr} 

    /* save the original supervisor sp to r4 */
    add     %r4 , %sp , #40

    /* get to %r6 the filter function to be called */
    ldr     %r6 , [%r4, #4]

    /* get to %r7 the new tlb */
    ldr     %r7 , [%r4, #0]
    ldr     %r7 , [%r7]

    /* save current TLB to r9 */
    mrc     p15, 0 , %r9 , c2 , c0 , 0

    /* save TLB to supervisor stack to be protected */
    push    {%r9}

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
    mov     %sp , %r3

    /* change back to supervisor mode */
    msr     cpsr_c , #(CPSR_SUPERVISOR_MODE | CPSR_DISABLE_IRQ)

    /* save the user mode stack to the supervisor stack */
    push    {%r8}

    /* switch TLB using r7 that contains the new tlb */
    mcr     p15, 0 , %r7 , c2 , c0 , 0

    /* change to user mode */
    msr     cpsr_c , #(CPSR_USER_MODE)

    /* the arguments %r0-r2 have already the correct value */
    
    /* call the filter function */
    /* PharosIpCallR filter(void *partitionData ,PartitionNumber number , void *input); */
    blx     %r6

    /*************************************************************************/
    /* Switch to supervisor mode with a syscall made in a well-known address */
    /*************************************************************************/

    /* prepare the call to the syscall */
    push    {%r0 , %r4 - %r12 , %lr}

    /* now switch to supervisor mode using the syscall */
    swi     PHAROS_SYSCALL_SUPERVISOR_IPC_FILTER_ENDED

    /* address where it is supposed to go after syscall */
ipCallFilterEndCall:

     /* here we are in supervisor mode */

    /*************************************************************************/
    /* Switch to kernel TLB, restore sp_user to its original value           */
    /*************************************************************************/

    /* switch to kernel TLB so we can access to the supervisor stack for sure */

    /* load to r2 the address of the kernel TLB */
    ldr     %r2, =kernelSectionTable

    /* program the c2 register to place the kernel TBL address */
    mcr     p15, 0 , %r2, c2 , c0 , 0
  
    /* flush the current TLB */
    mcr     p15, 0, %r2, c8, c7, 0

    /* get the user stack */
    pop     {%r8}

    /* change to system mode to restore the registers that were saved in user mode */
    msr     cpsr_c , #(CPSR_SYSTEM_MODE | CPSR_DISABLE_IRQ)

    /* right now the user mode stack is the shared stack */

    /* restore the registers before the syscall (using sp_user) */
    pop     {%r0 , %r4-%r12 , %lr}

    /* restore the user mode sp */
    mov     %sp , %r8 

    /* change back to supervisor mode */
    msr     cpsr_c , #(CPSR_SUPERVISOR_MODE | CPSR_DISABLE_IRQ)

    /*************************************************************************/
    /* Switch to thread TLB, restore saved saved registers                   */
    /*************************************************************************/

    /* restore the floating point registers */
    vpop    {%d0 - %d15}  

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
 * PharosIpCallR pharosCpuExecuteIpCallIo(void *partitionData , uint8_t *input , uint8_t *output , uint8_t *stack , ptrPartitionCpuMmuContext context , void *functionPointer , ptrPartition caller);
 *
 * r0 - partitionData
 * r1 - input
 * r2 - output
 * r3 - stack
 * [sp +  0] - context
 * [sp +  4] - functionPointer
 * [sp +  8] - caller
 */
.arm
.func pharosCpuExecuteIpCallIo
pharosCpuExecuteIpCallIo:

    /* when we arrive here we are at supervisor mode with its sp and lr registers */

    /* save remaining registers now (offset of stack will be 10*4 = 40) */
    push    {%r4 - %r12 , %lr}

    /* save the original supervisor sp to r4 */
    add     %r4 , %sp , #40
 
    /* get to %r6 the function to be called */
    ldr     %r6 , [%r4, #4]

    /* get to %r7 the new tlb */
    ldr     %r7 , [%r4, #0]
    ldr     %r7 , [%r7]

    /* save current TLB to r9 */
    mrc     p15, 0 , %r9 , c2 , c0 , 0

    /* save TLB to supervisor stack to be protected */
    push    {%r9}

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
    mov     %sp , %r3

    /* change back to supervisor mode */
    msr     cpsr_c , #(CPSR_SUPERVISOR_MODE | CPSR_DISABLE_IRQ)

    /* save the user mode stack to the supervisor stack */
    push    {%r8}

    /* the arguments %r0-r1 have already the correct value */
    
    /* switch TLB using r7 that contains the new tlb */
    mcr     p15, 0 , %r7 , c2 , c0 , 0

    /* flush TLB */
    mcr     p15, 0, %r7, c8, c7, 0

    /* change to user mode */
    msr     cpsr_c , #(CPSR_USER_MODE)

    /* the arguments %r0-r2 have already the correct value */

    /* call the filter function */
    /* PharosIpCallR function(void *partitionData , void *input , void *output); */
    blx     %r6

    /*************************************************************************/
    /* Switch to supervisor mode with a syscall made in a well-known address */
    /*************************************************************************/

    /* prepare the call to the syscall */
    push    {%r0 , %r4 - %r12 , %lr}

    /* now switch to supervisor mode using the syscall */
    swi     PHAROS_SYSCALL_SUPERVISOR_IPC_FUNCTION_IO_ENDED

    /* address where it is supposed to go after syscall */
ipcFunctionIoEndCall:

     /* here we are in supervisor mode */

    /*************************************************************************/
    /* Switch to kernel TLB, restore sp_user to its original value           */
    /*************************************************************************/

    /* switch to kernel TLB so we can access to the supervisor stack for sure */

    /* load to r2 the address of the kernel TLB */
    ldr     %r2, =kernelSectionTable

    /* program the c2 register to place the kernel TBL address */
    mcr     p15, 0 , %r2, c2 , c0 , 0
  
    /* flush the current TLB */
    mcr     p15, 0, %r2, c8, c7, 0

    /* get the user stack */
    pop     {%r8}

    /* change to system mode to restore the registers that were saved in user mode */
    msr     cpsr_c , #(CPSR_SYSTEM_MODE | CPSR_DISABLE_IRQ)

    /* right now the user mode stack is the shared stack */

    /* restore the registers before the syscall (using sp_user) */
    pop     {%r0 , %r4-%r12 , %lr}

    /* restore the user mode sp */
    mov     %sp , %r8 

    /* change back to supervisor mode */
    msr     cpsr_c , #(CPSR_SUPERVISOR_MODE | CPSR_DISABLE_IRQ)

    /*************************************************************************/
    /* Switch to thread TLB, restore saved saved registers                   */
    /*************************************************************************/

    /* restore the floating point registers */
    vpop    {%d0 - %d15}  

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
 * PharosIpCallR pharosCpuExecuteIpCallO(void *partitionData , uint8_t *output , uint8_t *stack ,  ptrCpuMmuContext context , void *functionPointer);
 *
 * r0 - partitionData
 * r1 - output
 * r2 - stack
 * r3 - context
 * [sp +  0] - functionPointer
 */
.arm
.func pharosCpuExecuteIpCallO
pharosCpuExecuteIpCallO:

    /* when we arrive here we are at supervisor mode with its sp and lr registers */

    /* save remaining registers now (offset of stack will be 10*4 = 40) */
    push    {%r4 - %r12 , %lr}

    /* save the original supervisor sp to r4 */
    add     %r4 , %sp , #40

    /* get to %r6 the function to be called */
    ldr     %r6 , [%r4, #0]

    /* get to %r7 the new tlb */
    ldr     %r7 , [%r3]

    /* save current TLB to r9 */
    mrc     p15, 0 , %r9 , c2 , c0 , 0

    /* save TLB to supervisor stack to be protected */
    push    {%r9}

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
    mov     %sp , %r2

    /* change back to supervisor mode */
    msr     cpsr_c , #(CPSR_SUPERVISOR_MODE | CPSR_DISABLE_IRQ)

    /* save the user mode stack to the supervisor stack */
    push    {%r8}

    /* the arguments %r0-r1 have already the correct value */
    
    /* switch TLB using r7 that contains the new tlb */
    mcr     p15, 0 , %r7 , c2 , c0 , 0

    /* flush TLB */
    mcr     p15, 0, %r7, c8, c7, 0

    /* change to user mode */
    msr     cpsr_c , #(CPSR_USER_MODE)

    /* the arguments %r0-r1 have already the correct value */

    /* call the filter function */
    /* PharosIpCallR function(void *partitionData , void *output); */
    blx     %r6

    /*************************************************************************/
    /* Switch to supervisor mode with a syscall made in a well-known address */
    /*************************************************************************/

    /* prepare the call to the syscall */
    push    {%r0 , %r4-%r12 , %lr}

    /* now switch to supervisor mode using the syscall */
    swi     PHAROS_SYSCALL_SUPERVISOR_IPC_FUNCTION_O_ENDED

    /* address where it is supposed to go after syscall */
ipcFunctionOEndCall:

    /* here we are in supervisor mode */

    /*************************************************************************/
    /* Switch to kernel TLB, restore sp_user to its original value           */
    /*************************************************************************/

    /* switch to kernel TLB so we can access to the supervisor stack for sure */

    /* load to r2 the address of the kernel TLB */
    ldr     %r2, =kernelSectionTable

    /* program the c2 register to place the kernel TBL address */
    mcr     p15, 0 , %r2, c2 , c0 , 0
  
    /* flush the current TLB */
    mcr     p15, 0, %r2, c8, c7, 0

    /* get the user stack */
    pop     {%r8}

    /* change to system mode to restore the registers that were saved in user mode */
    msr     cpsr_c , #(CPSR_SYSTEM_MODE | CPSR_DISABLE_IRQ)

    /* right now the user mode stack is the shared stack */

    /* restore the registers before the syscall (using sp_user) */
    pop     {%r0 , %r4-%r12 , %lr}

    /* restore the user mode sp */
    mov     %sp , %r8 

    /* change back to supervisor mode */
    msr     cpsr_c , #(CPSR_SUPERVISOR_MODE | CPSR_DISABLE_IRQ)

    /*************************************************************************/
    /* Switch to thread TLB, restore saved saved registers                   */
    /*************************************************************************/

    /* restore the floating point registers */
    vpop    {%d0 - %d15}  

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
 * CpuModeChangeContext pharosCpuIpCallFilterEnded(uint8_t *address , CpuModeChangeContext SPSR);
 */
.func pharosCpuIpCallFilterEnded
pharosCpuIpCallFilterEnded:

    /* load to r1 the address in which this function is supposed to be called from */
    ldr     %r2 , =ipCallFilterEndCall

    /* if the address is not correct */
    cmp     %r0 , %r2
    
    /* then leave and don't do anything */
    bne     pharosCpuIpCallFilterEndedLeave
    
    /* clear the mode from the SPSR */
    and     %r1 , %r1 , #(CPSR_CPU_MODE_MASK)

    /* place the new supervisor mode */
    orr     %r0 , %r1 , #(CPSR_SUPERVISOR_MODE)

    /* else, leave the SPSR as is */
pharosCpuIpCallFilterEndedLeave:

    /* leave and restore ARM/Thumb mode */
    bx      lr

.endfunc


/**
 * CpuModeChangeContext pharosCpuIpCallIoEnded(uint8_t *address , CpuModeChangeContext SPSR);
 */
.func pharosCpuIpCallIoEnded
pharosCpuIpCallIoEnded:

    /* load to r1 the address in which this function is supposed to be called from */
    ldr     %r2 , =ipcFunctionIoEndCall

    /* if the address is not correct */
    cmp     %r0 , %r2
    
    /* then leave and don't do anything */
    bne     pharosCpuIpcIoEndedLeave
    
    /* clear the mode from the SPSR */
    and     %r1 , %r1 , #(CPSR_CPU_MODE_MASK)

    /* place the new supervisor mode */
    orr     %r0 , %r1 , #(CPSR_SUPERVISOR_MODE)

    /* else, leave the SPSR as is */
pharosCpuIpcIoEndedLeave:

    /* leave and restore ARM/Thumb mode */
    bx      lr

.endfunc


/**
 * CpuModeChangeContext pharosCpuIpCallOEnded(uint8_t *address , CpuModeChangeContext SPSR);
 */
.func pharosCpuIpCallOEnded
pharosCpuIpCallOEnded:

    /* load to r1 the address in which this function is supposed to be called from */
    ldr     %r2 , =ipcFunctionOEndCall

    /* if the address is not correct */
    cmp     %r0 , %r2
    
    /* then leave and don't do anything */
    bne     pharosCpuIpcOEndedLeave
    
    /* clear the mode from the SPSR */
    and     %r1 , %r1 , #(CPSR_CPU_MODE_MASK)

    /* place the new supervisor mode */
    orr     %r0 , %r1 , #(CPSR_SUPERVISOR_MODE)

    /* else, leave the SPSR as is */
pharosCpuIpcOEndedLeave:

    /* leave and restore ARM/Thumb mode */
    bx      lr

.endfunc
