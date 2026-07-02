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

.text

/**
 * uint32_t pharosCpuCriticalSectionExecute(void *data , uint32_t arg1, uint32_t arg2 , uint32_t arg3 , uint32_t arg4 , ptrCpuMmuContext context ,CriticalSectionHandler handler);
 *
 * x0 - data
 * x1 - arg1
 * x2 - arg2
 * x3 - arg3
 * x4 - arg4
 * x5 - context
 * x6 - handler
 */
.func pharosCpuCriticalSectionExecute
pharosCpuCriticalSectionExecute:

    /* when we arrive here we are at EL1 mode */

    /* save sp of EL0 */
    mrs     x7 , sp_el0

    /* save x30 to the stack (will be modified by the call) */
    stp     x7 , x30 , [sp, #-16]!

    /* save the local registers: don't assume the critical function will not change them even though it shouldn't */
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
    
    /*************************************************************************/
    /* switch to EL0                                                         */
    /*************************************************************************/

    /* load the address of checkEl2 into z1 */
    adr     x7 , criticalSectionExecuteCall

    /* disable interrupts (IRQ + FIQ) to prevent an interrupt to mess with SPSR and ELR */
    msr     DAIFSet , #0b0011

    /* read SPSR */
    mrs     x5 , SPSR_EL1

    /* disable IRQ and FIQ */
    orr     x5 , x5 , #(1 << 6 | 1 << 7)

    /* set EL1 SPSR return (we will do an ERET instruction next so we must set up the SPSR of EL0 so we return from the exception to EL0) */
    msr     SPSR_EL1, x5

    /* set the exception LR register of EL0 to go to criticalSectionExecuteCall */
    msr     ELR_EL1, x7

    /* return from exception which will cause to go to EL0 and to instruction criticalSectionExecuteCall in A64 */
    eret

criticalSectionExecuteCall:

    /* uint32_t handler(void *data , uint32_t arg1 , uint32_t arg2 , uint32_t arg3 , uint32_t arg4); */
    blr      x6

    /*************************************************************************/
    /* Switch to supervisor mode with a syscall made in a well-known address */
    /*************************************************************************/

    /* now switch to supervisor mode using the syscall */
    svc     PHAROS_SYSCALL_SUPERVISOR_CRITICAL_SECTION_EXECUTE_END

    /* address where it is supposed to go after syscall */
criticalSectionExecuteEndCall:

    /* now we are in EL1 again */

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

    /* restore x30 from the stack */
    ldp     x7 , x30 , [sp], #16

    /* restore SP of EL0 */
    msr     sp_el0 , x7

    /* return from function */
    ret 

.endfunc


/**
 * CpuModeChangeContext pharosCpuCriticalSectionEnded(uint8_t *address , CpuModeChangeContext context);
 */ 
.func pharosCpuCriticalSectionEnded
pharosCpuCriticalSectionEnded:

    /* load to r1 the address in which this function is supposed to be called from */
    ldr     x2 , =criticalSectionExecuteEndCall

    /* if the address is not correct */
    cmp     x0 , x2
    
    /* then leave and don't do anything */
    bne     pharosCpuCriticalSectionEndedLeave
    
    /* set the EL1 mode */
    orr     x0 , x0 , #4

    /* set to use the SP of the EL level */
    orr     x0 , x0 , #1

    /* else, leave the SPSR as is */
pharosCpuCriticalSectionEndedLeave:

    /* return from function */
    ret 

.endfunc
  
