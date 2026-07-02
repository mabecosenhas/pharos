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

.type pharosCpuResourceExecuteAccess,   %function
.type pharosCpuResourceExecuteFilter ,  %function
.type pharosCpuResourceFilterEnded ,    %function
.type pharosCpuResourceAccessEnded ,    %function

.global pharosCpuResourceExecuteAccess
.global pharosCpuResourceExecuteFilter
.global pharosCpuResourceFilterEnded
.global pharosCpuResourceAccessEnded

.text

/**
 * PharosResourceFilterR pharosCpuResourceExecuteFilter(void *data , ResourceNumber id , uint64_t input ,
 *                                                      PartitionNumber callerId , ResourceFilterMethod filter ,
 *                                                      ptrPartitionCpuMmuContext context , StackPointer sharedStack)
 * a0 - data
 * a1 - resource id 
 * a2 - input
 * a3 - caller partition number
 * a4 - filter
 * a5 - context
 * a6 - sharedStack
 */
.func pharosCpuResourceExecuteFilter
pharosCpuResourceExecuteFilter:

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
    csrr    t0 , mhartid

    /* get the offset of the core on the pharosVRmpCore table */
    slli    t0 , t0 , PHAROS_CPU_CORE_RMP_SIZE_LOG2

    /* get the running thread */
    la      a7 , pharosVRmpCore

    /* get the running thread address for the running hart */
    add     a7 , a7 , t0

    /* get the running thread address for the running hart */
    ld      a7 , (a7)   

    /* save the kernel stack pointer in the TCB (we will override it) */
    ld      t1 , PHAROS_CPU_OFFSET_KERNEL_STACK (a7)
  
    /* save it to the kernel stack */
    sd      t1 , 15*8 (sp)

    /* if the CPU has floating point registers (double precision floating point) */
#ifdef PHAROS_CPU_FP

    addi    sp , sp , -33 * 8

    /* reads the fcsr register to t0 */
    frcsr   t0 

    fsd     f0  , 0*8 (sp)
    fsd     f1  , 1*8 (sp)
    fsd     f2  , 2*8 (sp)
    fsd     f3  , 3*8 (sp)
    fsd     f4  , 4*8 (sp)
    fsd     f5  , 5*8 (sp)
    fsd     f6  , 6*8 (sp)
    fsd     f7  , 7*8 (sp)
    fsd     f8  , 8*8 (sp)
    fsd     f9  , 9*8 (sp)
    fsd     f10 , 10*8 (sp)
    fsd     f11 , 11*8 (sp)
    fsd     f12 , 12*8 (sp)
    fsd     f13 , 13*8 (sp)
    fsd     f14 , 14*8 (sp)
    fsd     f15 , 15*8 (sp)
    fsd     f16 , 16*8 (sp)
    fsd     f17 , 17*8 (sp)
    fsd     f18 , 18*8 (sp)
    fsd     f19 , 19*8 (sp)
    fsd     f20 , 20*8 (sp)
    fsd     f21 , 21*8 (sp)
    fsd     f22 , 22*8 (sp)
    fsd     f23 , 23*8 (sp)
    fsd     f24 , 24*8 (sp)
    fsd     f25 , 25*8 (sp)
    fsd     f26 , 26*8 (sp)
    fsd     f27 , 27*8 (sp)
    fsd     f28 , 28*8 (sp)
    fsd     f29 , 29*8 (sp)
    fsd     f30 , 30*8 (sp)
    fsd     f31 , 31*8 (sp)
    sd      t0  , 32*8 (sp)
 
#endif

    /*************************************************************************/
    /* switch to user mode                                                   */
    /*************************************************************************/
    
    /* save the stack pointer to the TCB (it's ok because we already saved it) */
    sd      sp , PHAROS_CPU_OFFSET_KERNEL_STACK (a7)

    /* load the new SATP value */
    ld      a5 , (a5) 

    /* switch MMU permissions */
    csrw    satp , a5

    /* switch to shared stack */
    mv      sp , a6

    /* set the return address (ra) to pharosCpuResourceExecuteFilterCall */
    la      ra , pharosCpuResourceExecuteFilterCall

    /* make sure the MPIE is set to allow interrupts */
    li      a5 , PHAROS_CPU_MSTATUS_MPIE

    /* disable interrupts so that mstatus and mepc are not messed with */
    csrc    mstatus , 1 << PHAROS_CPU_MSTATUS_MIE_BIT

    /* allow interrupts when an mret is performed */
    csrs    mstatus , a5

    /* set the return address of mret to pharosCpuResourceExecuteFilterCall */
    csrw    mepc , ra

    /* to a machine return which will jump to pharosCpuResourceExecuteFilterCall */
    mret
 
pharosCpuResourceExecuteFilterCall:

    /* jump to the entry point of the thread (no arguments needed) */
    jalr    ra , a4
 
    /*************************************************************************/
    /* Switch to machine mode with a syscall made in a well-known address    */
    /*************************************************************************/
    li      gp , PHAROS_SYSCALL_SUPERVISOR_RESOURCE_EXECUTE_FILTER_ENDED << 3

    /* make the syscall to restore supervisor mode */
    ecall 
 
    /* address where it is supposed to go after syscall */
resourceFilterEndCall:

    /* now we are in machine mode again */
 
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

    /* if the CPU has floating point registers (double precision floating point) */
#ifdef PHAROS_CPU_FP

    fld     f0  , 0*8 (sp)
    fld     f1  , 1*8 (sp)
    fld     f2  , 2*8 (sp)
    fld     f3  , 3*8 (sp)
    fld     f4  , 4*8 (sp)
    fld     f5  , 5*8 (sp)
    fld     f6  , 6*8 (sp)
    fld     f7  , 7*8 (sp)
    fld     f8  , 8*8 (sp)
    fld     f9  , 9*8 (sp)
    fld     f10 , 10*8 (sp)
    fld     f11 , 11*8 (sp)
    fld     f12 , 12*8 (sp)
    fld     f13 , 13*8 (sp)
    fld     f14 , 14*8 (sp)
    fld     f15 , 15*8 (sp)
    fld     f16 , 16*8 (sp)
    fld     f17 , 17*8 (sp)
    fld     f18 , 18*8 (sp)
    fld     f19 , 19*8 (sp)
    fld     f20 , 20*8 (sp)
    fld     f21 , 21*8 (sp)
    fld     f22 , 22*8 (sp)
    fld     f23 , 23*8 (sp)
    fld     f24 , 24*8 (sp)
    fld     f25 , 25*8 (sp)
    fld     f26 , 26*8 (sp)
    fld     f27 , 27*8 (sp)
    fld     f28 , 28*8 (sp)
    fld     f29 , 29*8 (sp)
    fld     f30 , 30*8 (sp)
    fld     f31 , 31*8 (sp)
    ld      t0  , 32*8 (sp)
 
    /* restores the fcsr register */
    fscsr   t0 

    addi    sp , sp , 33 * 8

#endif

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
 * CpuModeChangeContext pharosCpuResourceFilterEnded(uint8_t *address , CpuModeChangeContext context);
 */
.func pharosCpuResourceFilterEnded
pharosCpuResourceFilterEnded:

    /* load to r1 the address in which this function is supposed to be called from */
    la      a2 , resourceFilterEndCall

    /* if the address is not correct then leave and don't do anything */
    bne     a0 , a2 , pharosCpuResourceFilterEndedLeave
    
    /* place the MPP bits in the mstatus (context in a1) to change it to machine mode */
    li      a2 , PHAROS_CPU_MSTATUS_MPP_MACHINE

    /* return the original mstatus with the MPP bits set */
    or      a0 , a1 , a2

    /* else, leave the mstatus as is */
pharosCpuResourceFilterEndedLeave:

    /* return from function */
    ret 

.endfunc


/**
 * uint64_t pharosCpuResourceExecuteAccess(void *data , uint64_t input , ResourceAccessMethod method ,
 *                                          ptrPartitionCpuMmuContext context , StackPointer sharedStack);
 * a0 - data
 * a1 - input
 * a2 - method
 * a3 - context
 * a4 - sharedStack
 */
.func pharosCpuResourceExecuteAccess
pharosCpuResourceExecuteAccess:

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
    csrr    t0 , mhartid

    /* get the offset of the core on the pharosVRmpCore table */
    slli    t0 , t0 , PHAROS_CPU_CORE_RMP_SIZE_LOG2

    /* get the running thread */
    la      a7 , pharosVRmpCore

    /* get the running thread address for the running hart */
    add     a7 , a7 , t0

    /* get the running thread address for the running hart */
    ld      a7 , (a7)   

    /* save the kernel stack pointer in the TCB (we will override it) */
    ld      t1 , PHAROS_CPU_OFFSET_KERNEL_STACK (a7)
  
    /* save it to the kernel stack */
    sd      t1 , 15*8 (sp)

    /* if the CPU has floating point registers (double precision floating point) */
#ifdef PHAROS_CPU_FP

    addi    sp , sp , -33 * 8

    /* reads the fcsr register to t0 */
    frcsr   t0 

    fsd     f0  , 0*8 (sp)
    fsd     f1  , 1*8 (sp)
    fsd     f2  , 2*8 (sp)
    fsd     f3  , 3*8 (sp)
    fsd     f4  , 4*8 (sp)
    fsd     f5  , 5*8 (sp)
    fsd     f6  , 6*8 (sp)
    fsd     f7  , 7*8 (sp)
    fsd     f8  , 8*8 (sp)
    fsd     f9  , 9*8 (sp)
    fsd     f10 , 10*8 (sp)
    fsd     f11 , 11*8 (sp)
    fsd     f12 , 12*8 (sp)
    fsd     f13 , 13*8 (sp)
    fsd     f14 , 14*8 (sp)
    fsd     f15 , 15*8 (sp)
    fsd     f16 , 16*8 (sp)
    fsd     f17 , 17*8 (sp)
    fsd     f18 , 18*8 (sp)
    fsd     f19 , 19*8 (sp)
    fsd     f20 , 20*8 (sp)
    fsd     f21 , 21*8 (sp)
    fsd     f22 , 22*8 (sp)
    fsd     f23 , 23*8 (sp)
    fsd     f24 , 24*8 (sp)
    fsd     f25 , 25*8 (sp)
    fsd     f26 , 26*8 (sp)
    fsd     f27 , 27*8 (sp)
    fsd     f28 , 28*8 (sp)
    fsd     f29 , 29*8 (sp)
    fsd     f30 , 30*8 (sp)
    fsd     f31 , 31*8 (sp)
    sd      t0  , 32*8 (sp)
 
#endif

    /*************************************************************************/
    /* switch to user mode                                                   */
    /*************************************************************************/
    
    /* save the stack pointer to the TCB (it's ok because we already saved it) */
    sd      sp , PHAROS_CPU_OFFSET_KERNEL_STACK (a7)

    /* load the new SATP value */
    ld      a3 , (a3) 

    /* switch MMU permissions */
    csrw    satp , a3

    /* switch to shared stack */
    mv      sp , a4

    /* set the return address (ra) to pharosCpuResourceExecuteAccessCall */
    la      ra , pharosCpuResourceExecuteAccessCall

    /* make sure the MPIE is set to allow interrupts */
    li      a4 , PHAROS_CPU_MSTATUS_MPIE

    /* disable interrupts so that mstatus and mepc are not messed with */
    csrc    mstatus , 1 << PHAROS_CPU_MSTATUS_MIE_BIT

    /* allow interrupts when an mret is performed */
    csrs    mstatus , a4

    /* set the return address of mret to pharosCpuResourceExecuteAccessCall */
    csrw    mepc , ra

    /* to a machine return which will jump to pharosCpuResourceExecuteAccessCall */
    mret
 
pharosCpuResourceExecuteAccessCall:
 
    /* call the resource access function */
    jalr    a2

    /*************************************************************************/
    /* Switch to machine mode with a syscall made in a well-known address    */
    /*************************************************************************/

    li      gp , PHAROS_SYSCALL_SUPERVISOR_RESOURCE_EXECUTE_ACCESS_ENDED << 3

    /* make the syscall to restore supervisor mode */
    ecall 

    /* address where it is supposed to go after syscall */
resourceAccessEndCall:

    /* now we are in machine mode again */
 
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

    /* if the CPU has floating point registers (double precision floating point) */
#ifdef PHAROS_CPU_FP

    fld     f0  , 0*8 (sp)
    fld     f1  , 1*8 (sp)
    fld     f2  , 2*8 (sp)
    fld     f3  , 3*8 (sp)
    fld     f4  , 4*8 (sp)
    fld     f5  , 5*8 (sp)
    fld     f6  , 6*8 (sp)
    fld     f7  , 7*8 (sp)
    fld     f8  , 8*8 (sp)
    fld     f9  , 9*8 (sp)
    fld     f10 , 10*8 (sp)
    fld     f11 , 11*8 (sp)
    fld     f12 , 12*8 (sp)
    fld     f13 , 13*8 (sp)
    fld     f14 , 14*8 (sp)
    fld     f15 , 15*8 (sp)
    fld     f16 , 16*8 (sp)
    fld     f17 , 17*8 (sp)
    fld     f18 , 18*8 (sp)
    fld     f19 , 19*8 (sp)
    fld     f20 , 20*8 (sp)
    fld     f21 , 21*8 (sp)
    fld     f22 , 22*8 (sp)
    fld     f23 , 23*8 (sp)
    fld     f24 , 24*8 (sp)
    fld     f25 , 25*8 (sp)
    fld     f26 , 26*8 (sp)
    fld     f27 , 27*8 (sp)
    fld     f28 , 28*8 (sp)
    fld     f29 , 29*8 (sp)
    fld     f30 , 30*8 (sp)
    fld     f31 , 31*8 (sp)
    ld      t0  , 32*8 (sp)
 
    /* restores the fcsr register */
    fscsr   t0 

    addi    sp , sp , 33 * 8

#endif

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
 * CpuModeChangeContext pharosCpuResourceAccessEnded(uint8_t *address , CpuModeChangeContext context);
 */
.func pharosCpuResourceAccessEnded
pharosCpuResourceAccessEnded:

    /* load to r1 the address in which this function is supposed to be called from */
    la      a2 , resourceAccessEndCall

    /* if the address is not correct then leave and don't do anything */
    bne     a0 , a2, pharosCpuResourceAccessEndedLeave
     
    /* place the MPP bits in the mstatus (context in a1) to change it to machine mode */
    li      a2 , PHAROS_CPU_MSTATUS_MPP_MACHINE

    /* return the original mstatus with the MPP bits set */
    or      a0 , a1 , a2

    /* else, leave the mstatus as is */
pharosCpuResourceAccessEndedLeave:

    /* return from function */
    ret 

.endfunc
