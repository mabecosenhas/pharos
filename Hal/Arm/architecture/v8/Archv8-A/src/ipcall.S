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
.global pharosCpuIpCallFilterExit
.global pharosCpuIpCallIoExit
.global pharosCpuIpCallOExit

.text
    

/**
 * PharosIpCallR pharosCpuExecuteIpCallFilter(void *partitionData , PartitionNumber partition , uint8_t *input , 
 *                                            uint8_t *stack ,  ptrCpuMmuContext context , void *filterFunction);
 * 
 * x0 - partitionData
 * x1 - partition
 * x2 - input
 * x3 - stack
 * x4 - context
 * x5 - filterFunction
 */
.func pharosCpuExecuteIpCallFilter
pharosCpuExecuteIpCallFilter:

    /* when we arrive here we are at EL1 mode */

    /* save sp of EL0 */
    mrs     x7 , sp_el0

    /* save x30 to the stack (will be modified by the call) */
    stp     x7 , x30 , [sp, #-16]!

    /* save the current TTBR */
    mrs     x7 , TTBR0_EL1

    /* save the TTBR to the stack (kernel stack) */
    str     x7 , [sp, #-16]!

    /* save the local registers: don't assume the filter will not change them even though it shouldn't */
    stp     x8  , x9  , [sp, #-16]!
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
    adr     x7 , pharosCpuExecuteIpCallFilterCall

    /* set the exception LR register of EL3 to go to pharosCpuExecuteIpCallFilterCall */
    msr     ELR_EL1, x7

    /* set the EL0 stack pointer to the shared stack */
    msr     sp_el0 , x3

    /* get the new TTBR */
    ldr     x4 , [x4]

    /* set the context to the context of the partition that has the critical section */
    msr     TTBR0_EL1 , x4
    
    /* force the TTBR to be changed */
    isb

    /* return from exception which will cause to go to EL0 and to instruction pharosCpuExecuteIpCallFilterCall in A64 */
    /* note: this will re-enable interrupts since SPSR == 0 */
    eret

pharosCpuExecuteIpCallFilterCall:

    /* call the filter function */

    /* PharosIpCallR filter(void *partitionData , PartitionNumber number , void *input); */
    blr     x5
 
    /*************************************************************************/
    /* Switch to EL1 mode with a syscall made in a well-known address        */
    /*************************************************************************/

    /* now switch to supervisor mode using the syscall */
    svc     PHAROS_SYSCALL_SUPERVISOR_IPC_FILTER_ENDED

    /* address where it is supposed to go after syscall */
ipCallFilterEndCall:

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
    ldp     x8  , x9  , [sp], #16

    /* get the previous TTBR_EL1 and SP_EL1 */
    ldr     x7 , [sp], #16

    /* restore TTBR0_EL1 */
    msr     TTBR0_EL1 , x7

    /* restore SP_EL0 and x30 from the stack */
    ldp     x7 , x30 , [sp], #16

    /* restore SP of EL0 */
    msr     sp_el0 , x7

    /* return from function */
    ret 

.endfunc 


/**
 * PharosIpCallR pharosCpuExecuteIpCallIo(void *partitionData , uint8_t *input , uint8_t *output , uint8_t *stack ,  ptrCpuMmuContext context , void *functionPointer);
 *
 * x0 - partitionData
 * x1 - input
 * x2 - output
 * x3 - stack
 * x4 - context
 * x5 - functionPointer
 */
.func pharosCpuExecuteIpCallIo
pharosCpuExecuteIpCallIo:

    /* when we arrive here we are at EL1 mode */

    /* save sp of EL0 */
    mrs     x7 , sp_el0

    /* save x30 to the stack (will be modified by the call) */
    stp     x7 , x30 , [sp, #-16]!

    /* save the current TTBR */
    mrs     x7 , TTBR0_EL1

    /* save the TTBR to the stack (kernel stack) */
    str     x7 , [sp, #-16]!

    /* save the local registers: don't assume the filter will not change them even though it shouldn't */
    stp     x8  , x9  , [sp, #-16]!
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
    adr     x7 , pharosCpuExecuteIpCallIoCall

    /* set the exception LR register of EL3 to go to pharosCpuExecuteIpCallIoCall */
    msr     ELR_EL1, x7

    /* set the EL0 stack pointer to the shared stack */
    msr     sp_el0 , x3

    /* get the new TTBR */
    ldr     x4 , [x4]

    /* set the context to the context of the partition that has the critical section */
    msr     TTBR0_EL1 , x4
    
    /* force the TTBR to be changed */
    isb

    /* return from exception which will cause to go to EL0 and to instruction pharosCpuExecuteIpCallIoCall in A64 */
    /* note: this will re-enable interrupts since SPSR == 0 */
    eret

pharosCpuExecuteIpCallIoCall:

    /* call the function */

    /* PharosIpCallR function(void *partitionData , void *input , void *output); */
    blr     x5

    /*************************************************************************/
    /* Switch to EL1 mode with a syscall made in a well-known address        */
    /*************************************************************************/
 
pharosCpuIpCallIoExit:

    /* now switch to supervisor mode using the syscall */
    svc     PHAROS_SYSCALL_SUPERVISOR_IPC_FUNCTION_IO_ENDED

    /* address where it is supposed to go after syscall */
ipcFunctionIoEndCall:

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
    ldp     x8  , x9  , [sp], #16

    /* get the previous TTBR_EL1 */
    ldr     x7 , [sp], #16

    /* restore TTBR0_EL1 */
    msr     TTBR0_EL1 , x7

    /* restore SP_EL0 and x30 from the stack */
    ldp     x7 , x30 , [sp], #16

    /* restore SP of EL0 */
    msr     sp_el0 , x7

    /* return from function */
    ret 

.endfunc


/**
 * PharosIpCallR pharosCpuExecuteIpCallO(void *partitionData ,  uint8_t *output , uint8_t *stack ,  ptrCpuMmuContext context , void *functionPointer);
 *
 * x0 - partitionData
 * x1 - output
 * x2 - stack
 * x3 - context
 * x4 - functionPointer
 */
.func pharosCpuExecuteIpCallO
pharosCpuExecuteIpCallO:

    /* when we arrive here we are at EL1 mode */

    /* save sp of EL0 */
    mrs     x7 , sp_el0

    /* save x30 to the stack (will be modified by the call) */
    stp     x7 , x30 , [sp, #-16]!

    /* save the current TTBR */
    mrs     x7 , TTBR0_EL1

    /* save the TTBR to the stack (kernel stack) */
    str     x7 , [sp, #-16]!

    /* save the local registers: don't assume the filter will not change them even though it shouldn't */
    stp     x8  , x9  , [sp, #-16]!
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
    adr     x7 , pharosCpuExecuteIpCallOCall

    /* set the exception LR register of EL3 to go to pharosCpuExecuteIpCallOCall */
    msr     ELR_EL1, x7

    /* set the EL0 stack pointer to the shared stack */
    msr     sp_el0 , x2

    /* get the new TTBR */
    ldr     x3 , [x3]

    /* set the context to the context of the partition that has the critical section */
    msr     TTBR0_EL1 , x3
    
    /* force the TTBR to be changed */
    isb

    /* return from exception which will cause to go to EL0 and to instruction pharosCpuExecuteIpCallOCall in A64 */
    /* note: this will re-enable interrupts since SPSR == 0 */
    eret

pharosCpuExecuteIpCallOCall:

    /* call the function */

    /* PharosIpCallR function(void *partitionData , void *output); */
    blr     x4
 
    /*************************************************************************/
    /* Switch to EL1 mode with a syscall made in a well-known address        */
    /*************************************************************************/
  
pharosCpuIpCallOExit:

    /* now switch to supervisor mode using the syscall */
    svc     PHAROS_SYSCALL_SUPERVISOR_IPC_FUNCTION_O_ENDED

    /* address where it is supposed to go after syscall */
ipcFunctionOEndCall:

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
    ldp     x8  , x9  , [sp], #16

    /* get the previous TTBR_EL1 */
    ldr     x7 , [sp], #16

    /* restore TTBR0_EL1 */
    msr     TTBR0_EL1 , x7

    /* restore SP_EL0 and x30 from the stack */
    ldp     x7 , x30 , [sp], #16

    /* restore SP of EL0 */
    msr     sp_el0 , x7

    /* return from function */
    ret 

.endfunc       


/**
 * CpuModeChangeContext pharosCpuIpCallFilterEnded(uint8_t *address , CpuModeChangeContext SPSR);
 */
.func pharosCpuIpCallFilterEnded
pharosCpuIpCallFilterEnded:
  
    /* load to x2 the address in which this function is supposed to be called from */
    ldr     x2 , =ipCallFilterEndCall

    /* if the address is not correct */
    cmp     x0 , x2
    
    /* then leave and don't do anything */
    bne     pharosCpuIpCallFilterEndedLeave
    
    /* clear the mode from the SPSR -> this will place it at EL0 */
    bic     x0 , x1 , #(SPSR_EL_MASK)

    /* set the EL1 mode */
    orr     x0 , x0 , #4

    /* set to use the SP of the EL level */
    orr     x0 , x0 , #1

    /* else, leave the SPSR as is */
pharosCpuIpCallFilterEndedLeave:

     /* return from function */
    ret

.endfunc


/**
 * CpuModeChangeContext pharosCpuIpCallIoEnded(uint8_t *address , CpuModeChangeContext SPSR);
 */
.func pharosCpuIpCallIoEnded
pharosCpuIpCallIoEnded:

    /* load to x2 the address in which this function is supposed to be called from */
    ldr     x2 , =ipcFunctionIoEndCall

    /* if the address is not correct */
    cmp     x0 , x2
    
    /* then leave and don't do anything */
    bne     pharosCpuIpcIoEndedLeave
    
    /* clear the mode from the SPSR -> this will place it at EL0 */
    bic     x0 , x1 , #(SPSR_EL_MASK)
 
    /* set the EL1 mode */
    orr     x0 , x0 , #4

    /* set to use the SP of the EL level */
    orr     x0 , x0 , #1

    /* else, leave the SPSR as is */
pharosCpuIpcIoEndedLeave:

    /* return from function */
    ret

.endfunc


/**
 * CpuModeChangeContext pharosCpuIpCallOEnded(uint8_t *address , CpuModeChangeContext SPSR);
 */
.func pharosCpuIpCallOEnded
pharosCpuIpCallOEnded:

    /* load to x2 the address in which this function is supposed to be called from */
    ldr     x2 , =ipcFunctionOEndCall

    /* if the address is not correct */
    cmp     x0 , x2
    
    /* then leave and don't do anything */
    bne     pharosCpuIpcOEndedLeave
    
    /* clear the mode from the SPSR -> this will place it at EL0 */
    bic     x0 , x1 , #(SPSR_EL_MASK)

    /* set the EL1 mode */
    orr     x0 , x0 , #4

    /* set to use the SP of the EL level */
    orr     x0 , x0 , #1

    /* else, leave the SPSR as is */
pharosCpuIpcOEndedLeave:

    /* return from function */
    ret

.endfunc
