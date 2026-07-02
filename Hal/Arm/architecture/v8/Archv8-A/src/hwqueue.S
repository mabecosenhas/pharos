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


.type pharosCpuHwQueueExecuteFilter,    %function
.type pharosCpuHwQueueFilterEnded ,     %function

.global pharosCpuHwQueueExecuteFilter
.global pharosCpuHwQueueFilterEnded

.text

/**
 * PharosHeavyWeightFilterResult pharosCpuHwQueueExecuteFilter(void * data , HeavyWeightQueueNumber id , 
 *                                                             PartitionNumber sender ,  PharosPriority priority , 
 *                                                             uint32_t messageSize , HeavyWeightFilterMethod filter ,
 *                                                             ptrCpuMmuContext context , StackPointer sharedStack);
 * x0 - data
 * x1 - queue number
 * x2 - sender
 * x3 - priority
 * x4 - messageSize
 * x5 - filter
 * x6 - context
 * x7 - sharedStack
 */
.func pharosCpuHwQueueExecuteFilter
pharosCpuHwQueueExecuteFilter:

    /* when we arrive here we are at EL1 mode */

    /* save the local registers: don't assume the filter will not change them even though it shouldn't */
    stp     x8  , x9  , [sp, #-16]!

    /* save sp of EL0 */
    mrs     x8 , sp_el0

    /* save x30 to the stack (will be modified by the call) */
    stp     x8 , x30 , [sp, #-16]!

    /* save the current TTBR */
    mrs     x8 , TTBR0_EL1

    /* save the TTBR to the stack (kernel stack) */
    str     x8 , [sp, #-16]!

    /* save the local registers: don't assume the filter will not change them even though it shouldn't */
    stp     x10 , x11 , [sp, #-16]!
    stp     x12 , x13 , [sp, #-16]!
    stp     x14 , x15 , [sp, #-16]!
    stp     x16 , x17 , [sp, #-16]!
    stp     x18 , x19 , [sp, #-16]!
    stp     x20 , x21 , [sp, #-16]!
    stp     x22 , x23 , [sp, #-16]!
    stp     x24 , x25 , [sp, #-16]!
    stp     x26 , x27 , [sp, #-16]!
    stp     x28 , x29 , [sp, #-16]!
 
    /* save all FPU registers */
    stp		q0 , q1 , [sp, #-32]!
	stp		q2 , q3 , [sp, #-32]!
	stp		q4 , q5 , [sp, #-32]!
	stp		q6 , q7 , [sp, #-32]!
	stp		q8 , q9 , [sp, #-32]!
	stp		q10 , q11 , [sp, #-32]!
	stp		q12 , q13 , [sp, #-32]!
	stp		q14 , q15 , [sp, #-32]!
	stp		q16 , q17 , [sp, #-32]!
	stp		q18 , q19 , [sp, #-32]!
	stp		q20 , q21 , [sp, #-32]!
	stp		q22 , q23 , [sp, #-32]!
	stp		q24 , q25 , [sp, #-32]!
	stp		q26 , q27 , [sp, #-32]!
	stp		q28 , q29 , [sp, #-32]!
	stp		q30 , q31 , [sp, #-32]!

    /*************************************************************************/
    /* switch to EL0                                                         */
    /*************************************************************************/

    /* disable interrupts (if an interrupt occurs it will override the SPSR and ELR and we must protect them */
    msr     DAIFSet , #0b0011

    /* set EL1 SPSR to go to EL0 (we will do an ERET instruction next so we must set up the SPSR of EL0 so we return from the exception to EL0) */
    msr     SPSR_EL1, xzr

    /* load the address of checkEl2 into z1 */
    adr     x8 , pharosCpuHwQueueExecuteFilterCall

    /* set the exception LR register of EL3 to go to pharosCpuHwQueueExecuteFilterCall */
    msr     ELR_EL1, x8

    /* set the EL0 stack pointer to the shared stack */
    msr     sp_el0 , x7

    /* get the new TTBR */
    ldr     x6 , [x6]

    /* set the context to the context of the partition that has the critical section */
    msr     TTBR0_EL1 , x6
    
    /* force the TTBR to be changed */
    isb

    /* return from exception which will cause to go to EL0 and to instruction pharosCpuHwQueueExecuteFilterCall in A64 */
    /* note: this will re-enable interrupts since SPSR == 0 */
    eret

pharosCpuHwQueueExecuteFilterCall:

    /* call the filter function */
    /* PharosHeavyWeightFilterResult heavyWeightFilterMethod(void *data , HeavyWeightQueueNumber id , PartitionNumber sender , PharosPriority priority , uint32_t messageSize); */
    blr      x5

    /*************************************************************************/
    /* Switch to EL1 mode with a syscall made in a well-known address        */
    /*************************************************************************/

    /* now switch to supervisor mode using the syscall */
    svc     PHAROS_SYSCALL_SUPERVISOR_HW_QUEUE_EXECUTE_FILTER_ENDED

    /* address where it is supposed to go after syscall */
hwFilterEndCall:

    /* now we are in EL1 again */

    /* restore FPU registers */
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

    /* restore the local registers */
    ldp     x28 , x29 , [sp], #16
    ldp     x26 , x27 , [sp], #16
    ldp     x24 , x25 , [sp], #16
    ldp     x22 , x23 , [sp], #16
    ldp     x20 , x21 , [sp], #16
    ldp     x18 , x19 , [sp], #16
    ldp     x16 , x17 , [sp], #16
    ldp     x14 , x15 , [sp], #16
    ldp     x12 , x13 , [sp], #16
    ldp     x10 , x11 , [sp], #16

    /* get the previous TTBR_EL1 */
    ldr     x7 , [sp], #16

    /* restore TTBR0_EL1 */
    msr     TTBR0_EL1 , x7

    /* restore SP_EL0 and x30 from the stack */
    ldp     x7 , x30 , [sp], #16

    /* restore x8 and x9 (was the first to be pushed, is the last to be popped) */
    ldp     x8  , x9  , [sp], #16

    /* restore SP of EL0 */
    msr     sp_el0 , x7

    /* return from function */
    ret 

.endfunc


/**
 * CpuModeChangeContext pharosCpuHwQueueFilterEnded(uint8_t *address , CpuModeChangeContext SPSR);
 */
.func pharosCpuHwQueueFilterEnded
pharosCpuHwQueueFilterEnded:

    /* load to r1 the address in which this function is supposed to be called from */
    ldr     x2 , =hwFilterEndCall

    /* if the address is not correct */
    cmp     x0 , x2
    
    /* then leave and don't do anything */
    bne     pharosCpuHwQueueFilterEndedLeave
    
    /* clear the mode from the SPSR -> this will place it at EL0 */
    bic     x0 , x1 , #(SPSR_EL_MASK)

    /* set the EL1 mode */
    orr     x0 , x0 , #4

    /* set to use the SP of the EL level */
    orr     x0 , x0 , #1

    /* else, leave the SPSR as is */
pharosCpuHwQueueFilterEndedLeave:

    /* return from function */
    ret 

.endfunc
