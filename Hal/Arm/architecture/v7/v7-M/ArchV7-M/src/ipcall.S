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


.type pharosCpuExecuteIpCallFilterLow,      %function
.type pharosCpuExecuteIpCallIoLow,          %function
.type pharosCpuExecuteIpCallOLow,           %function
.type pharosCpuIpCallIoEnded ,              %function
.type pharosCpuIpCallFilterEnded,           %function
.type pharosCpuIpCallOEnded,                %function

.global pharosCpuExecuteIpCallFilterLow
.global pharosCpuExecuteIpCallIoLow
.global pharosCpuExecuteIpCallOLow
.global pharosCpuIpCallIoEnded
.global pharosCpuIpCallFilterEnded
.global pharosCpuIpCallOEnded

.syntax unified
.text
.thumb
    
     
/**
 * PharosIpCallR pharosCpuExecuteIpCallFilterLow(void *partitionData , PartitionNumber partition , uint8_t *input ,
 *                                               uint8_t *stack , void *filterFunction);
 * 
 * r0 - partitionData
 * r1 - partition
 * r2 - input
 * r3 - stack
 * [sp +  0] - filterFunction
 */
.func pharosCpuExecuteIpCallFilterLow
pharosCpuExecuteIpCallFilterLow:

    /* when we arrive here we are in thread mode, with process stack and priviledged mode */

    /* save remaining registers now (offset of stack will be 10*4 = 40) */
    push    {%r4 - %r12 , %lr} 

    /* save the original supervisor sp to r4 */
    add     %r4 , %sp , #40

    /* get to %r6 the filter function to be called */
    ldr     %r6 , [%r4, #0]

    /* save the floating point registers */
    vpush    {%d0 - %d15}   

    /*******************************************************************************************************************/
    /* Switch stack (including copying arguments from supervisor to user stack) and finally CPU mode to user mode */
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
    mov     %sp , %r3

    /* read the current control register */
    mrs     %r4 , control

    /* set the thread mode to unpriviledged */
    orr     %r4 , %r4 , #1

    /* switch to unpriviledged mode */
    msr     control , %r4

    /* the arguments %r0-r1 have already the correct value */
    
    /* call the filter function */
    /* PharosIpCallR filter(void *partitionData , PartitionNumber number , void *input); */
    blx     %r6

    /*************************************************************************/
    /* Switch to supervisor mode with a syscall made in a well-known address */
    /*************************************************************************/

    /* now switch to supervisor mode using the syscall */
    swi     PHAROS_SYSCALL_SUPERVISOR_IPC_FILTER_ENDED

    /* address where it is supposed to go after syscall */
ipCallFilterEndCall:

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

    /* restore the used registers */
    pop     {%r4 - %r12 , %lr}

    /* return from function */
	bx      %lr

.endfunc 


/**
 * PharosIpCallR pharosCpuExecuteIpCallIoLow(void *partitionData , uint8_t *input , uint8_t *output , uint8_t *stack , 
 *                                           void *functionPointer);
 *
 * r0 - partitionData
 * r1 - input
 * r2 - output
 * r3 - stack
 * [sp +  0] - functionPointer
 */
.func pharosCpuExecuteIpCallIoLow
pharosCpuExecuteIpCallIoLow:

    /* when we arrive here we are at supervisor mode with its sp and lr registers */

    /* save remaining registers now (offset of stack will be 10*4 = 40) */
    push    {%r4 - %r12 , %lr}

    /* save the original supervisor sp to r4 */
    add     %r4 , %sp , #40

    /* get to %r6 the function to be called */
    ldr     %r6 , [%r4, #0]

    /* save the floating point registers */
    vpush   {%d0 - %d15}   

    /*******************************************************************************************************************/
    /* Switch stack (including copying arguments from supervisor to user stack) and finally CPU mode to user mode      */
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
    mov     %sp , %r3

    /* read the current control register */
    mrs     %r4 , control

    /* set the thread mode to unpriviledged */
    orr     %r4 , %r4 , #1

    /* switch to unpriviledged mode */
    msr     control , %r4

    /* the arguments %r0-r1 have already the correct value */

    /* call the filter function */
    /* PharosIpCallR function(void *partitionData , void *input , void *output); */
    blx     %r6

    /*************************************************************************/
    /* Switch to supervisor mode with a syscall made in a well-known address */
    /*************************************************************************/

    /* now switch to supervisor mode using the syscall */
    swi     PHAROS_SYSCALL_SUPERVISOR_IPC_FUNCTION_IO_ENDED

    /* address where it is supposed to go after syscall */
ipcFunctionIoEndCall:

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

    /* restore the used registers */
    pop     {%r4 - %r12 , %lr}

    /* return from function */
	bx      %lr

.endfunc       


/**
 * PharosIpCallR pharosCpuExecuteIpCallOLow(void *partitionData , uint8_t *output , uint8_t *stack ,
 *                                          void *functionPointer);
 *
 * r0 - partitionData
 * r1 - output
 * r2 - stack
 * r3 - functionPointer
 */
.func pharosCpuExecuteIpCallOLow
pharosCpuExecuteIpCallOLow:

    /* when we arrive here we are at supervisor mode with its sp and lr registers */

    /* save remaining registers now (offset of stack will be 10*4 = 40) */
    push    {%r4 - %r12 , %lr}

    /* save the floating point registers */
    vpush    {%d0 - %d15}   

    /*******************************************************************************************************************/
    /* Switch stack (including copying arguments from supervisor to user stack) and finally CPU mode to user mode */
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
    mov     %sp , %r2

    /* read the current control register */
    mrs     %r4 , control

    /* set the thread mode to unpriviledged */
    orr     %r4 , %r4 , #1

    /* switch to unpriviledged mode */
    msr     control , %r4

    /* the arguments %r0-r1 have already the correct value */

    /* call the filter function */
    /* PharosIpCallR function(void *partitionData , void *output); */
    blx     %r3

    /*************************************************************************/
    /* Switch to supervisor mode with a syscall made in a well-known address */
    /*************************************************************************/

    /* now switch to supervisor mode using the syscall */
    swi     PHAROS_SYSCALL_SUPERVISOR_IPC_FUNCTION_O_ENDED

    /* address where it is supposed to go after syscall */
ipcFunctionOEndCall:

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

    /* restore the used registers */
    pop     {%r4 - %r12 , %lr}

    /* return from function */
	bx      %lr

.endfunc       


/**
 * uint32_t pharosCpuIpCallFilterEnded(uint8_t *address);
 */
.func pharosCpuIpCallFilterEnded
pharosCpuIpCallFilterEnded:

    /* load to r1 the address in which this function is supposed to be called from */
    ldr     %r2 , =ipCallFilterEndCall

    /* place in %r0 the value 0 if the address are the same and != 0 if they are different */
    sub     %r0 , %r0 , %r2
    
    /* leave and restore ARM/Thumb mode */
    bx      lr

.endfunc


/**
 * uint32_t pharosCpuIpCallIoEnded(uint8_t *address);
 */
.func pharosCpuIpCallIoEnded
pharosCpuIpCallIoEnded:

    /* load to r1 the address in which this function is supposed to be called from */
    ldr     %r2 , =ipcFunctionIoEndCall

    /* place in %r0 0 if the address are the same and != 0 if they are different */
    sub     %r0 , %r0 , %r2
    
    /* leave and restore ARM/Thumb mode */
    bx      lr

.endfunc


/**
 * uint32_t pharosCpuIpCallOEnded(uint8_t *address);
 */
.func pharosCpuIpCallOEnded
pharosCpuIpCallOEnded:

    /* load to r1 the address in which this function is supposed to be called from */
    ldr     %r2 , =ipcFunctionOEndCall

    /* place in %r0 0 if the address are the same and != 0 if they are different */
    sub     %r0 , %r0 , %r2
    
    /* leave and restore ARM/Thumb mode */
    bx      lr

.endfunc
