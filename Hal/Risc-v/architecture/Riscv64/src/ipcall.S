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

.text
    

/**
 * PharosIpCallR pharosCpuExecuteIpCallFilter(void *partitionData , PartitionNumber partition , uint8_t *input , 
 *                                            uint8_t *stack ,  ptrCpuMmuContext context , void *filterFunction);
 * 
 * a0 - partitionData
 * a1 - partition
 * a2 - input
 * a3 - stack
 * a4 - context
 * a5 - filterFunction
 */
.func pharosCpuExecuteIpCallFilter
pharosCpuExecuteIpCallFilter:

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
    ld      a4 , (a4) 

    /* switch MMU permissions */
    csrw    satp , a4

    /* switch to shared stack */
    mv      sp , a3

    /* set the return address (ra) to pharosCpuExecuteIpCallFilterCall */
    la      ra , pharosCpuExecuteIpCallFilterCall

    /* make sure the MPIE is set to allow interrupts */
    li      a4 , PHAROS_CPU_MSTATUS_MPIE

    /* disable interrupts so that mstatus and mepc are not messed with */
    csrc    mstatus , 1 << PHAROS_CPU_MSTATUS_MIE_BIT

    /* allow interrupts when an mret is performed */
    csrs    mstatus , a4

    /* set the return address of mret to pharosCpuExecuteIpCallFilterCall */
    csrw    mepc , ra

    /* to a machine return which will jump to pharosCpuExecuteIpCallFilterCall */
    mret
 
pharosCpuExecuteIpCallFilterCall:

    /* call the filter function */

    /* PharosIpCallR filter(PartitionNumber number , void *input); */
    jalr    ra , a5
 
    /*************************************************************************/
    /* Switch to machine mode with a syscall made in a well-known address    */
    /*************************************************************************/
    li      gp , PHAROS_SYSCALL_SUPERVISOR_IPC_FILTER_ENDED << 3

    /* make the syscall to restore supervisor mode */
    ecall  

    /* address where it is supposed to go after syscall */
ipCallFilterEndCall:

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
 * PharosIpCallR pharosCpuExecuteIpCallIo(void *partitionData , uint8_t *input , uint8_t *output , uint8_t *stack ,  ptrCpuMmuContext context , void *functionPointer);
 *
 * a0 - partitionData
 * a1 - input
 * a2 - output
 * a3 - stack
 * a4 - context
 * a5 - functionPointer
 */
.func pharosCpuExecuteIpCallIo
pharosCpuExecuteIpCallIo:

    /* when we arrive here we are at machine mode */

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
 
    /* save the stack pointer to the TCB */
    sd      sp , PHAROS_CPU_OFFSET_KERNEL_STACK (a7)
  
    /* load the new SATP value */
    ld      a4 , (a4) 

    /* switch MMU permissions */
    csrw    satp , a4

    /* switch to shared stack */
    mv      sp , a3

    /* set the return address (ra) to pharosCpuExecuteIpCallIoCall */
    la      ra , pharosCpuExecuteIpCallIoCall

    /* make sure the MPIE is set to allow interrupts */
    li      a4 , PHAROS_CPU_MSTATUS_MPIE

    /* disable interrupts so that mstatus and mepc are not messed with */
    csrc    mstatus , 1 << PHAROS_CPU_MSTATUS_MIE_BIT

    /* allow interrupts when an mret is performed */
    csrs    mstatus , a4

    /* set the return address of mret to pharosCpuExecuteIpCallIoCall */
    csrw    mepc , ra

    /* to a machine return which will jump to pharosCpuExecuteIpCallIoCall */
    mret

pharosCpuExecuteIpCallIoCall:

    /* call the function */

    /* PharosIpCallR function(void *input , void *output); */
    jalr    ra , a5

    /*************************************************************************/
    /* Switch to EL1 mode with a syscall made in a well-known address        */
    /*************************************************************************/
     li      gp , PHAROS_SYSCALL_SUPERVISOR_IPC_FUNCTION_IO_ENDED << 3

    /* make the syscall to restore supervisor mode */
    ecall  

    /* address where it is supposed to go after syscall */
ipcFunctionIoEndCall:

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
 * PharosIpCallR pharosCpuExecuteIpCallO(void *partitionData ,  uint8_t *output , uint8_t *stack ,  ptrCpuMmuContext context , void *functionPointer);
 *
 * a0 - partitionData
 * a1 - output
 * a2 - stack
 * a3 - context
 * a4 - functionPointer
 */
.func pharosCpuExecuteIpCallO
pharosCpuExecuteIpCallO:

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
 
    /* save the stack pointer to the TCB */
    sd      sp , PHAROS_CPU_OFFSET_KERNEL_STACK (a7)
  
    /* load the new SATP value */
    ld      a3 , (a3)

    /* switch MMU permissions */
    csrw    satp , a3

    /* switch to shared stack */
    mv      sp , a2

    /* set the return address (ra) to pharosCpuExecuteIpCallOCall */
    la      ra , pharosCpuExecuteIpCallOCall

    /* make sure the MPIE is set to allow interrupts */
    li      a3 , PHAROS_CPU_MSTATUS_MPIE

    /* disable interrupts so that mstatus and mepc are not messed with */
    csrc    mstatus , 1 << PHAROS_CPU_MSTATUS_MIE_BIT

    /* allow interrupts when an mret is performed */
    csrs    mstatus , a3

    /* set the return address of mret to pharosCpuExecuteIpCallOCall */
    csrw    mepc , ra

    /* to a machine return which will jump to pharosCpuExecuteIpCallOCall */
    mret

pharosCpuExecuteIpCallOCall:

    /* call the function */

    /* PharosIpCallR function(void *output); */
    jalr    ra , a4
 
    /*************************************************************************/
    /* Switch to machine mode with a syscall made in a well-known address    */
    /*************************************************************************/

    li      gp , PHAROS_SYSCALL_SUPERVISOR_IPC_FUNCTION_O_ENDED << 3

    /* make the syscall to restore supervisor mode */
    ecall 
 
    /* address where it is supposed to go after syscall */
ipcFunctionOEndCall:

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
 * CpuModeChangeContext pharosCpuIpCallFilterEnded(uint8_t *address , CpuModeChangeContext SPSR);
 */
.func pharosCpuIpCallFilterEnded
pharosCpuIpCallFilterEnded:
  

    /* load to r1 the address in which this function is supposed to be called from */
    la      a2 , ipCallFilterEndCall

    /* if the address is not correct then leave and don't do anything */
    bne     a0 , a2 , pharosCpuIpCallFilterEndedLeave
    
    /* place the MPP bits in the mstatus (context in a1) to change it to machine mode */
    li      a2 , PHAROS_CPU_MSTATUS_MPP_MACHINE

    /* return the original mstatus with the MPP bits set */
    or      a0 , a1 , a2

    /* else, leave the mstatus as is */
pharosCpuIpCallFilterEndedLeave:

    /* return from function */
    ret 

.endfunc


/**
 * CpuModeChangeContext pharosCpuIpCallIoEnded(uint8_t *address , CpuModeChangeContext SPSR);
 */
.func pharosCpuIpCallIoEnded
pharosCpuIpCallIoEnded:

    /* load to r1 the address in which this function is supposed to be called from */
    la      a2 , ipcFunctionIoEndCall

    /* if the address is not correct then leave and don't do anything */
    bne     a0 , a2 , pharosCpuIpcIoEndedLeave
    
    /* place the MPP bits in the mstatus (context in a1) to change it to machine mode */
    li      a2 , PHAROS_CPU_MSTATUS_MPP_MACHINE

    /* return the original mstatus with the MPP bits set */
    or      a0 , a1 , a2

    /* else, leave the mstatus as is */
pharosCpuIpcIoEndedLeave:

    /* return from function */
    ret 
 
.endfunc


/**
 * CpuModeChangeContext pharosCpuIpCallOEnded(uint8_t *address , CpuModeChangeContext SPSR);
 */
.func pharosCpuIpCallOEnded
pharosCpuIpCallOEnded:

    /* load to r1 the address in which this function is supposed to be called from */
    la      a2 , ipcFunctionOEndCall

    /* if the address is not correct then leave and don't do anything */
    bne     a0 , a2 , pharosCpuIpcOEndedLeave
    
    /* place the MPP bits in the mstatus (context in a1) to change it to machine mode */
    li      a2 , PHAROS_CPU_MSTATUS_MPP_MACHINE

    /* return the original mstatus with the MPP bits set */
    or      a0 , a1 , a2

    /* else, leave the mstatus as is */
pharosCpuIpcOEndedLeave:

    /* return from function */
    ret 

.endfunc
