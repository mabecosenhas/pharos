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


.type pharosCpuCriticalSectionExecute      , %function
.type pharosCpuCriticalSectionEnded        , %function

.global pharosCpuCriticalSectionExecute
.global pharosCpuCriticalSectionEnded
.global criticalSectionExecuteEndCall

.syntax unified
.text
.thumb


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

    /* when we arrive here we are with the thread kernel stack and priviledges */

    /* save remaining registers now (offset of stack will be 10*4 = 40) */
    push    {%r4 - %r12 , %lr}

    /* save the original supervisor sp to r4 */
    add     %r4 , %sp , #40

    /* get to %r10 the arg4 (will be pushed to the user stack) */
    ldr     %r10 , [%r4, #0]

    /* get to %r5 the filter function to be called */
    ldr     %r5 , [%r4, #8]

    /*******************************************************************************************************************/
    /* Switch stack (including copying arguments from supervisor to user stack), TLB and finally CPU mode to user mode */
    /*******************************************************************************************************************/

    /* get the running thread variable address */
    ldr     %r4 , =pharosVRunningThread

    /* get the running thread address */
    ldr     %r4 , [%r4]

    /* store the PSP in the current kernel stack (to be restored later on) */
    str     %sp , [%r4 , #OFFSET_CURRENT_KERNEL_STACK]

    /* get the previous syscall stack */
    ldr     %r7 , [%r4 , #OFFSET_PREVIOUS_SVC_STACK]
   
    /* set the IRQ bit to r1 */
	mov     %r8 , #(1)

    /* clear interrupts by forcing 1 to primask */
    msr     PRIMASK , %r8

    /* place 0 in r8 */
    mov     %r8 , #FALSE

    /* store the FALSE in the "insideKernel" flag */
    str     %r8 , [%r4 , #OFFSET_INSIDE_KERNEL]

    /* switch to the user stack */
    mov     %sp , %r7
    
    /* read the current control register */
    mrs     %r4 , control

    /* set the thread mode to unpriviledged */
    orr     %r4 , %r4 , #1

    /* switch to unpriviledged mode */
    msr     control , %r4

    /* place in the user stack the arg4 (save an extra register to keep the stack correctly aligned) */
    push    {%r10 , %r11}

    /* call the critical section handler function with disabled interrupts and in user mode */
    /* uint32_t handler(void *data , uint32_t arg1 , uint32_t arg2 , uint32_t arg3 , uint32_t arg4); */
    blx     %r5

    /* restore the previous stack position */
    pop     {%r10 , %r11}

    /*************************************************************************/
    /* Switch to supervisor mode with a syscall made in a well-known address */
    /*************************************************************************/

    /* now switch to supervisor mode using the syscall */
    swi     PHAROS_SYSCALL_SUPERVISOR_CRITICAL_SECTION_EXECUTE_END

    /* address where it is supposed to go after syscall */
criticalSectionExecuteEndCall:

    /* get the running thread variable address */
    ldr     %r4 , =pharosVRunningThread

    /* get the running thread address */
    ldr     %r4 , [%r4]

    /* switch to kernel stack */
    ldr     %sp , [%r4 , #OFFSET_CURRENT_KERNEL_STACK]

    /* enable interrupts by placing 0 to primask */
    mov     %r6 , #0

    /* restore primask */
    msr     PRIMASK , %r6

    /* restore the used registers (can do this after the TLB has changed to save some nop cycles) */
    pop     {%r4 - %r12 , %lr}

    /* return from function */
	bx      %lr

.endfunc


/**
 * uint32_t pharosCpuCriticalSectionEnded(uint8_t *address);
 */ 
.func pharosCpuCriticalSectionEnded
pharosCpuCriticalSectionEnded:

    /* load to r1 the address in which this function is supposed to be called from */
    ldr     %r2 , =criticalSectionExecuteEndCall

    /* place in %r0 the value 0 if the address are the same and != 0 if they are different */
    sub     %r0 , %r0 , %r2
    
    /* leave and restore ARM/Thumb mode */
    bx      lr

.endfunc
  
