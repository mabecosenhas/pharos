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
#include <pharos/hal/cpu/conf.h>


.type pharosCpuCriticalSectionExecute   , %function
.type pharosCpuCriticalSectionEnded        , %function

.global pharosCpuCriticalSectionExecute
.global pharosCpuCriticalSectionEnded

.text

/**
 * uint32_t pharosCpuCriticalSectionExecute(void *data , uint32_t arg1, uint32_t arg2 , uint32_t arg3 , uint32_t arg4 , ptrCpuMmuContext context ,CriticalSectionHandler handler);
 *
 * a0 - data
 * a1 - arg1
 * a2 - arg2
 * a3 - arg3
 * a4 - arg4
 * a5 - context
 * a6 - handler
 */
.func pharosCpuCriticalSectionExecute
pharosCpuCriticalSectionExecute:


    /* when we arrive here we are in machine mode */

    /* save all registers that the callee is supposed to save (the application may not respect the procedure 
     * call standard and do a messy thing, so we have to make sure that it can't brake Pharos) */
    
    addi    sp , sp , -16 * 8

    /* get the current satp */
    csrr    t0 , satp

    /* save the callee saved registers into our stack to make sure the filter does not mess with them (even though by the calling convention it shouldn't) */
    sd      x1  , 0*8 (sp)
    sd      x3  , 1*8 (sp)
    sd      x4  , 2*8 (sp)
    sd      x8  , 3*8 (sp)
    sd      x9  , 4*8 (sp)
    sd      x18 , 5*8 (sp)
    sd      x19 , 6*8 (sp)
    sd      x20 , 7*8 (sp)
    sd      x21 , 8*8 (sp)
    sd      x22 , 9*8 (sp)
    sd      x23 , 10*8 (sp)
    sd      x24 , 11*8 (sp)
    sd      x25 , 12*8 (sp)
    sd      x26 , 13*8 (sp)

    /* save the satp to stack so that we can restore it later on */
    sd      t0 , 14*8 (sp)
  
    /* load the running hart */
    csrr    t2 , mhartid

    /* get the offset of the core on the pharosVRmpCore table */
    slli    t2 , t2 , PHAROS_CPU_CORE_RMP_SIZE_LOG2

    /* get the running thread */
    la      a7 , pharosVRmpCore

    /* get the running thread address for the running hart */
    add     a7 , a7 , t2

    /* get the running thread address for the running hart */
    ld      a7 , (a7)   

    /* save the kernel stack pointer in the TCB (we will override it) */
    ld      t1 , PHAROS_CPU_OFFSET_KERNEL_STACK (a7)
  
    /* save it to the kernel stack */
    sd      t1 , 15*8 (sp)

    /*************************************************************************/
    /* switch to user mode                                                   */
    /*************************************************************************/
 
    /* save the stack pointer to the TCB */
    sd      sp , PHAROS_CPU_OFFSET_KERNEL_STACK (a7)

    /* load the new SATP value */
    ld      a5 , (a5) 

    /* switch MMU permissions */
    csrw    satp , a5

    /* load the user stack */
    ld      sp , PHAROS_CPU_OFFSET_USER_STACK_CURRENT (a7)

    /* set the return address (ra) to pharosCriticalSectionExecuteCall */
    la      ra , pharosCriticalSectionExecuteCall

    /* make sure the MPIE is set to disable interrupts */
    li      a5 , PHAROS_CPU_MSTATUS_MPIE | PHAROS_CPU_MSTATUS_SPIE

    /* disable interrupts so that mstatus and mepc are not messed with */
    csrc    mstatus , 1 << PHAROS_CPU_MSTATUS_MIE_BIT

    /* disable interrupts when an mret is performed */
    csrc    mstatus , a5

    /* workaround: seams that clearing the MPIE does not disable interrupts in user mode, so we disable interrupts using mie */
    li      a5 , 1 << 3 | 1 << 7 | 1 << 11

    /* disables interrupts */
    csrc    mie , a5

    /* set the return address of mret to pharosCriticalSectionExecuteCall */
    csrw    mepc , ra

    /* to a machine return which will jump to pharosCriticalSectionExecuteCall */
    mret
 
pharosCriticalSectionExecuteCall:

    /* jump to the entry point of the thread (no arguments needed) */
    jalr    ra , a6
 
    /*************************************************************************/
    /* Switch to machine mode with a syscall made in a well-known address    */
    /*************************************************************************/
    li      gp , PHAROS_SYSCALL_SUPERVISOR_CRITICAL_SECTION_EXECUTE_END << 3

    /* make the syscall to restore supervisor mode */
    ecall 
 
    /* address where it is supposed to go after syscall */
criticalSectionExecuteEndCall:

    /* now we are in machine mode again */
 
    /* workaround: seams that clearing the MPIE does not disable interrupts in user mode, so we disable interrupts using mie */
    li      a5 , 1 << 3 | 1 << 7 | 1 << 11

    /* enable interrupts */
    csrs    mie , a5

    /* load the running hart */
    csrr    a6 , mhartid

    /* get the offset of the core on the pharosVRmpCore table */
    slli    a6 , a6 , PHAROS_CPU_CORE_RMP_SIZE_LOG2

    /* get the running thread on the running core */
    la      a7 , pharosVRmpCore

    /* get the running thread address for the running hart */
    add     a7 , a7 , a6

    /* get the running thread address for the running hart */
    ld      a7 , (a7)   

    /* now that we have the running thread (in a7) we can restore the stack pointer */
    ld      sp , PHAROS_CPU_OFFSET_KERNEL_STACK (a7)

    /* restore the registers */
    ld      x1  , 0*8 (sp)
    ld      x3  , 1*8 (sp)
    ld      x4  , 2*8 (sp)
    ld      x8  , 3*8 (sp)
    ld      x9  , 4*8 (sp)
    ld      x18 , 5*8 (sp)
    ld      x19 , 6*8 (sp)
    ld      x20 , 7*8 (sp)
    ld      x21 , 8*8 (sp)
    ld      x22 , 9*8 (sp)
    ld      x23 , 10*8 (sp)
    ld      x24 , 11*8 (sp)
    ld      x25 , 12*8 (sp)
    ld      x26 , 13*8 (sp)
    ld      t0 ,  14*8 (sp)
    ld      t1 ,  15*8 (sp)

    /* restore the satp */
    csrw    satp , t0

    /* restore the stack pointer position */
    addi    sp , sp , 16 * 8

    /* restore the original position of the kernel stack on the TCB */
    sd      t1 , PHAROS_CPU_OFFSET_KERNEL_STACK (a7)

    /* return from function */
    ret 

.endfunc


/**
 * CpuModeChangeContext pharosCpuCriticalSectionEnded(uint8_t *address , CpuModeChangeContext context);
 */ 
.func pharosCpuCriticalSectionEnded
pharosCpuCriticalSectionEnded:


    /* load to r1 the address in which this function is supposed to be called from */
    la      a2 , criticalSectionExecuteEndCall

    /* if the address is not correct then leave and don't do anything */
    bne     a0 , a2 , pharosCpuCriticalSectionEndedLeave
    
    /* place the MPP and MPIE bits in the mstatus (context in a1) to change it to machine mode and enable interrupts */
    li      a2 , PHAROS_CPU_MSTATUS_MPP_MACHINE | PHAROS_CPU_MSTATUS_MPIE

    /* return the original mstatus with the MPP bits set */
    or      a0 , a1 , a2

    /* else, leave the mstatus as is */
pharosCpuCriticalSectionEndedLeave:

    /* return from function */
    ret 

.endfunc
  
