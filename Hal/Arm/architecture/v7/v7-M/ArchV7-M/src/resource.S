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
#include <pharos/hal/def.h>

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
.thumb


/**
 * PharosResourceFilterR pharosCpuResourceExecuteFilter(void *data , ResourceNumber id , uint64_t input , 
 *                                                      PartitionNumber callerId , ResourceFilterMethod filter , 
 *                                                      ptrCpuMmuContext context , StackPointer sharedStack);
 * 
 * r0 - data
 * r1 - resource id 
 * r2 - input
 * r3 - input
 * [sp +  0] - caller partition number
 * [sp +  4] - filter
 * [sp +  8] - context
 * [sp +  12] - sharedStack
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

    /* get to %r10 the caller partition number (will be pushed to the user stack) */
    ldr     %r10 , [%r4, #0]

    /* get to %r5 the stack to be used*/
    ldr     %r5 , [%r4, #12]

    /* get to %r6 the filter function to be called */
    ldr     %r6 , [%r4, #4]

    /* save the floating point registers */
    vpush   {%d0 - %d15}   

    /*******************************************************************************************************************/
    /* Switch stack (including copying arguments from supervisor to user stack), TLB and finally CPU mode to user mode */
    /*******************************************************************************************************************/

    /* get the running thread variable address */
    ldr     %r4 , =pharosVRunningThread

    /* get the running thread address */
    ldr     %r4 , [%r4]

    /* get the previous syscall stack */
    ldr     %r7 , [%r4 , #OFFSET_PREVIOUS_SVC_STACK]

    /* get the previous current kernel stack */
    ldr     %r8 , [%r4 , #OFFSET_CURRENT_KERNEL_STACK]

    /* push the previous syscall stack and the current stack into the current thread kernel stack */
    push    {%r7 , %r8}

    /* store the PSP in the current kernel stack (to be restored later on) */
    str     %sp , [%r4 , #OFFSET_CURRENT_KERNEL_STACK]

    /* place 0 in r8 */
    mov     %r8 , #FALSE

    /* store the FALSE in the "insideKernel" flag */
    str     %r8 , [%r4 , #OFFSET_INSIDE_KERNEL]

    /* switch the sp to shared stack */
    mov     %sp , %r5

    /* read the current control register */
    mrs     %r4 , control

    /* set the thread mode to unpriviledged */
    orr     %r4 , %r4 , #1

    /* switch to unpriviledged mode */
    msr     control , %r4

    /* place in the user stack the callerId */
    push    {%r10 , %r11}

    /* call the filter function */
    /* PharosResourceFilterResult resourceFilterMethod(void *data , ResourceNumber resourceId , uint64_t input , PartitionNumber callerId); */
    blx     %r6

    /* restore the previous stack position */
    pop     {%r10 , %r11}

    /*************************************************************************/
    /* Switch to supervisor mode with a syscall made in a well-known address */
    /*************************************************************************/

    /* now switch to supervisor mode using the syscall */
    swi     PHAROS_SYSCALL_SUPERVISOR_RESOURCE_EXECUTE_FILTER_ENDED

    /* address where it is supposed to go after syscall */
resourceFilterEndCall:

    /* get the running thread variable address */
    ldr     %r4 , =pharosVRunningThread

    /* get the running thread address */
    ldr     %r4 , [%r4]

    /* restore the PSP stack */
    ldr     %sp , [%r4 , #OFFSET_CURRENT_KERNEL_STACK]

    /* get to r7 the previous syscall stack */
    pop     {%r7 , %r8}

    /* set the previous syscall stack to the current thread */
    str     %r7 , [%r4 , #OFFSET_PREVIOUS_SVC_STACK]

    /* restore the current kernel thread stack of the current thread */
    str     %r8 , [%r4 , #OFFSET_CURRENT_KERNEL_STACK]

    /* restore the floating point registers */
    vpop    {%d0 - %d15}

    /* restore the used registers (can do this after the TLB has changed to save some nop cycles) */
    pop     {%r4 - %r12 , %lr}

    /* return from function */
	bx      %lr

.endfunc



/**
 * uint64_t pharosCpuResourceExecuteAccess(void *data , uint64_t input , ResourceAccessMethod method , 
 *                                         ptrCpuMmuContext context , StackPointer sharedStack);
 * 
 * r0 - data
 * r1 - nothing
 * r2 - input -> must be aligned
 * r3 - input
 * [sp +  0] - method
 * [sp +  4] - context
 * [sp +  8] - sharedStack
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

    /* get to %r6 the function to be called */
    ldr     %r6 , [%r4, #0]

    /* get to %r5 the stack to be used*/
    ldr     %r5 , [%r4, #8]

    /* save the floating point registers */
    vpush   {%d0 - %d15}

    /*************************************************/
    /* Switch stack and switch CPU mode to user mode */
    /*************************************************/

    /* get the running thread variable address */
    ldr     %r4 , =pharosVRunningThread

    /* get the running thread address */
    ldr     %r4 , [%r4]

    /* get the previous syscall stack */
    ldr     %r7 , [%r4 , #OFFSET_PREVIOUS_SVC_STACK]

    /* get the previous current kernel stack */
    ldr     %r8 , [%r4 , #OFFSET_CURRENT_KERNEL_STACK]

    /* push the previous syscall stack and the current stack into the current thread kernel stack */
    push    {%r7 , %r8}

    /* store the PSP in the current kernel stack (to be restored later on) */
    str     %sp , [%r4 , #OFFSET_CURRENT_KERNEL_STACK]

    /* place 0 in r8 */
    mov     %r8 , #FALSE

    /* store the FALSE in the "insideKernel" flag */
    str     %r8 , [%r4 , #OFFSET_INSIDE_KERNEL]

    /* switch the sp to shared stack */
    mov     %sp , %r5

    /* read the current control register */
    mrs     %r4 , control

    /* set the thread mode to unpriviledged */
    orr     %r4 , %r4 , #1

    /* switch to unpriviledged mode */
    msr     control , %r4

    /* call the resource access function uint64_t resourceAccessMethod(void *data , uint64_t input) */
    blx     %r6

    /*************************************************************************/
    /* Switch to supervisor mode with a syscall made in a well-known address */
    /*************************************************************************/

    /* now switch to supervisor mode using the syscall */
    swi     PHAROS_SYSCALL_SUPERVISOR_RESOURCE_EXECUTE_ACCESS_ENDED

    /* address where it is supposed to go after syscall */
resourceAccessEndCall:

    /* get the running thread variable address */
    ldr     %r4 , =pharosVRunningThread

    /* get the running thread address */
    ldr     %r4 , [%r4]

    /* restore the PSP stack */
    ldr     %sp , [%r4 , #OFFSET_CURRENT_KERNEL_STACK]

    /* get to r7 the previous syscall stack */
    pop     {%r7 , %r8}

    /* set the previous syscall stack to the current thread */
    str     %r7 , [%r4 , #OFFSET_PREVIOUS_SVC_STACK]

    /* restore the current kernel thread stack of the current thread */
    str     %r8 , [%r4 , #OFFSET_CURRENT_KERNEL_STACK]

    /* restore the floating point registers */
    vpop    {%d0 - %d15}

    /* restore the used registers (can do this after the TLB has changed to save some nop cycles) */
    pop     {%r4 - %r12 , %lr}

    /* return from function */
	bx      %lr

.endfunc



/**
 * uint32_t pharosCpuResourceFilterEnded(uint8_t *address );
 */
.func pharosCpuResourceFilterEnded
pharosCpuResourceFilterEnded:

    /* load to r1 the address in which this function is supposed to be called from */
    ldr     %r2 , =resourceFilterEndCall

    /* place in %r0 the value 0 if the address are the same and != 0 if they are different */
    sub     %r0 , %r0 , %r2
    
    /* leave and restore ARM/Thumb mode */
    bx      %lr

.endfunc


/**
 * uint32_t pharosCpuResourceAccessEnded(uint8_t *address);
 */
.func pharosCpuResourceAccessEnded
pharosCpuResourceAccessEnded:

    /* load to r1 the address in which this function is supposed to be called from */
    ldr     %r2 , =resourceAccessEndCall

    /* place in %r0 the value 0 if the address are the same and != 0 if they are different */
    sub     %r0 , %r0 , %r2
    
    /* leave and restore ARM/Thumb mode */
    bx      %lr

.endfunc
