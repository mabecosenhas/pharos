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


.type pharosCpuHwQueueExecuteFilterLow, %function
.type pharosCpuHwQueueFilterEnded , %function

.global pharosCpuHwQueueExecuteFilterLow
.global pharosCpuHwQueueFilterEnded

.syntax unified
.text
.thumb


/**
 * PharosHeavyWeightFilterResult pharosCpuHwQueueExecuteFilter(void *data , HeavyWeightQueueNumber id ,
 *                                                             PartitionNumber sender ,  PharosPriority priority , 
 *                                                             uint32_t messageSize , HeavyWeightFilterMethod filter ,
 *                                                             ptrThreadInfo thread , StackPointer sharedStack);
 * r0 - data
 * r1 - queue number
 * r2 - sender
 * r3 - nothing
 * [sp +  0] - priority
 * [sp +  4] - priority
 * [sp +  8] - messageSize
 * [sp +  12] - filter
 * [sp +  16] - thread
 * [sp +  20] - sharedStack
 */
.func pharosCpuHwQueueExecuteFilterLow
pharosCpuHwQueueExecuteFilterLow:

    /* when we arrive here we are at supervisor mode with its sp and lr registers */

    /* save remaining registers now (offset of stack will be 10*4 = 40) */
    push    {%r4 - %r12 , %lr}

    /* save the original supervisor sp to r4 */
    add     %r4 , %sp , #40

    /* get to %r9 the priority (will be pushed to the user stack) */
    ldr     %r9 , [%r4, #0]

    /* get to %r10 the priority (will be pushed to the user stack) */
    ldr     %r10 , [%r4, #4]

    /* get to %r11 the messageSize (will be pushed to the user stack) */
    ldr     %r11 , [%r4, #8]

    /* get to %r5 the stack to be used*/
    ldr     %r5 , [%r4, #20]

    /* get to %r6 the filter function to be called */
    ldr     %r6 , [%r4, #12]

    /* get to %r4 the running thread to be called */
    ldr     %r4 , [%r4, #16]

    /* save the floating point registers */
    vpush   {%d0 - %d15}   

    /*******************************************************************************************************************/
    /* Switch stack (including copying arguments from supervisor to user stack) and finally CPU mode to user mode */
    /*******************************************************************************************************************/

    /* get the previous syscall stack from the TCB */
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

    /* place in the user stack the arguments (save an extra register to keep the stack correctly aligned) */
    push    {%r9 , %r10 , %r11 , %r12}

    /* the arguments %r0-r3 have already the correct value */

    /* call the filter function */
    /* PharosHeavyWeightFilterResult heavyWeightFilterMethod(void *data , HwQueueNumber id , PartitionNumber sender , PharosPriority priority , uint32_t messageSize); */
    blx     %r6

    /* restore the previous stack position */
    pop     {%r9 , %r10 , %r11 , %r12}

    /*************************************************************************/
    /* Switch to supervisor mode with a syscall made in a well-known address */
    /*************************************************************************/

    /* now switch to supervisor mode using the syscall */
    swi     PHAROS_SYSCALL_SUPERVISOR_HW_QUEUE_EXECUTE_FILTER_ENDED

    /* address where it is supposed to go after syscall */
hwFilterEndCall:

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
 * uint32_t pharosCpuHwQueueFilterEnded(uint8_t *address);
 */
.func pharosCpuHwQueueFilterEnded
pharosCpuHwQueueFilterEnded:

    /* load to r1 the address in which this function is supposed to be called from */
    ldr     %r2 , =hwFilterEndCall

    /* place in %r0 the value 0 if the address are the same and != 0 if they are different */
    sub     %r0 , %r0 , %r2
    
    /* leave and restore ARM/Thumb mode */
    bx      %lr

.endfunc
