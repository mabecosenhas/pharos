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

.type pharosCpuExecuteUserModeAper,         %function
.type pharosCpuExecuteUserModePer,          %function
.type pharosCpuExecuteUserModeEvent,        %function
.type pharosCpuExecuteUserModeMutex,        %function
.type pharosCpuExecuteUserModeLwQueue,      %function
.type pharosCpuExecuteUserModeHwQueue,      %function
.type pharosCpuExecuteUserModeChannel,      %function
.type pharosCpuSwitchSupervisorMode,        %function

.global pharosCpuExecuteUserModeAper
.global pharosCpuExecuteUserModePer
.global pharosCpuExecuteUserModeEvent
.global pharosCpuExecuteUserModeMutex
.global pharosCpuExecuteUserModeLwQueue
.global pharosCpuExecuteUserModeHwQueue
.global pharosCpuExecuteUserModeChannel
.global pharosCpuSwitchSupervisorMode

/**********************************************************************************************************/
/* labels that the syscall internal handlers will use to make sure the syscall is made at the right place */
/**********************************************************************************************************/

.global pharosAperiodicThreadAddress
.global pharosPeriodicThreadAddress
.global pharosSporadicThreadEndChannel
.global pharosSporadicThreadEndHwQueue
.global pharosSporadicThreadEndLwQueue
.global pharosSporadicThreadEndMutex
.global pharosSporadicThreadEndEvent


.text


/**
 * void pharosCpuExecuteUserModeAper(void *partitionData , ptrThreadInfo thread);
 * a0 - partitionData
 * a1 - thread
 */
.func pharosCpuExecuteUserModeAper
pharosCpuExecuteUserModeAper:
    
    /* when we arrive here we are in machine mode */

    /* save all registers that the callee is supposed to save (the application may not respect the procedure 
     * call standard and do a messy thing, so we have to make sure that it can't brake Pharos) */
    
    addi    sp , sp , -15 * 8

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
  
    /* save the kernel stack pointer in the TCB (we will override it) */
    ld      t1 , PHAROS_CPU_OFFSET_KERNEL_STACK (a1)
  
    /* save it to the kernel stack */
    sd      t1 , 14*8 (sp)

    /*************************************************************************/
    /* switch to user mode                                                   */
    /*************************************************************************/

    /* get the entry point of the function */
    ld      a7 , PHAROS_CPU_OFFSET_ENTRY_POINT (a1)

    /* save the stack pointer to the TCB */
    sd      sp , PHAROS_CPU_OFFSET_KERNEL_STACK (a1)

    /* switch to user stack */
    ld      sp , PHAROS_CPU_OFFSET_USER_STACK (a1)

    /* set the return address (ra) to pharosCpuExecuteUserModeAperCall */
    la      ra , pharosCpuExecuteUserModeAperCall

    /* disable interrupts (so that mepc is not changed) */
    csrc    mstatus , 1 << PHAROS_CPU_MSTATUS_MIE_BIT

    /* set the return address of mret to pharosCpuExecuteUserModeAperCall */
    csrw    mepc , ra

    /* to a machine return which will jump to pharosCpuExecuteUserModeAperCall */
    mret
 
pharosCpuExecuteUserModeAperCall:

    /* jump to the entry point of the thread (no arguments needed) */
    jalr    ra , a7

    /*************************************************************************/
    /* Switch to EL1 mode with a syscall made in a well-known address        */
    /*************************************************************************/
    li      gp , PHAROS_SYSCALL_APERIODIC_THREAD_ENDED << 3

    /* make the syscall to restore supervisor mode */
    ecall   

     /* define the label position just after the swi call */
pharosAperiodicThreadAddress:

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
    ld      t1  , 14*8 (sp)

    /* restore the original position of the kernel stack on the TCB */
    sd      t1 , PHAROS_CPU_OFFSET_KERNEL_STACK (a7)

    /* restore the stack pointer position */
    addi    sp , sp , 15 * 8

    /* return from function */
    ret 

.endfunc


/**
 * void pharosCpuExecuteUserModePer(void *partitionData , ptrThreadInfo thread);
 */
.func pharosCpuExecuteUserModePer
pharosCpuExecuteUserModePer:
   
    /* when we arrive here we are in machine mode */

    /* save all registers that the callee is supposed to save (the application may not respect the procedure 
     * call standard and do a messy thing, so we have to make sure that it can't brake Pharos) */

    addi    sp , sp , -15 * 8

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

    /* save the kernel stack pointer in the TCB (we will override it) */
    ld      t1 , PHAROS_CPU_OFFSET_KERNEL_STACK (a1)
  
    /* save it to the kernel stack */
    sd      t1 , 14*8 (sp)

    /*************************************************************************/
    /* switch to user mode                                                   */
    /*************************************************************************/

    /* get the entry point of the function */
    ld      a7 , PHAROS_CPU_OFFSET_ENTRY_POINT (a1)

    /* save the stack pointer to the TCB */
    sd      sp , PHAROS_CPU_OFFSET_KERNEL_STACK (a1)

    /* switch to user stack */
    ld      sp , PHAROS_CPU_OFFSET_USER_STACK (a1)

    /* set the return address (ra) to pharosCpuExecuteUserModePerCall */
    la      ra , pharosCpuExecuteUserModePerCall

    /* disable interrupts (so that mepc is not changed) */
    csrc    mstatus , 1 << PHAROS_CPU_MSTATUS_MIE_BIT

    /* set the return address of mret to pharosCpuExecuteUserModePerCall */
    csrw    mepc , ra

    /* to a machine return which will jump to pharosCpuExecuteUserModePerCall */
    mret
 
pharosCpuExecuteUserModePerCall:

    /* jump to the entry point of the thread (no arguments needed) */
    jalr    ra , a7

    /*************************************************************************/
    /* Switch to EL1 mode with a syscall made in a well-known address        */
    /*************************************************************************/

    li      gp , PHAROS_SYSCALL_PERIODIC_THREAD_ENDED << 3
    
    /* make the syscall to restore supervisor mode */
    ecall   

     /* define the label position just after the swi call */
pharosPeriodicThreadAddress:

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
    ld      t1  , 14*8 (sp)

    /* restore the original position of the kernel stack on the TCB */
    sd      t1 , PHAROS_CPU_OFFSET_KERNEL_STACK (a7)

    /* restore the stack pointer position */
    addi    sp , sp , 15 * 8

    /* return from function */
    ret 

.endfunc


/**
 * void pharosCpuExecuteUserModeMutex(void *partitionData , SemaphoreMutexNumber ssId , ptrThreadInfo thread);
 */
.func pharosCpuExecuteUserModeMutex
pharosCpuExecuteUserModeMutex:
    
    /* when we arrive here we are in machine mode */

    /* save all registers that the callee is supposed to save (the application may not respect the procedure 
     * call standard and do a messy thing, so we have to make sure that it can't brake Pharos) */

    addi    sp , sp , -15 * 8

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

    /* save the kernel stack pointer in the TCB (we will override it) */
    ld      t1 , PHAROS_CPU_OFFSET_KERNEL_STACK (a2)
  
    /* save it to the kernel stack */
    sd      t1 , 14*8 (sp)

    /*************************************************************************/
    /* switch to user mode                                                   */
    /*************************************************************************/

    /* get the entry point of the function */
    ld      a7 , PHAROS_CPU_OFFSET_ENTRY_POINT (a2)

    /* save the stack pointer to the TCB */
    sd      sp , PHAROS_CPU_OFFSET_KERNEL_STACK (a2)

    /* switch to user stack */
    ld      sp , PHAROS_CPU_OFFSET_USER_STACK (a2)

    /* set the return address (ra) to pharosCpuExecuteUserModeMutexCall */
    la      ra , pharosCpuExecuteUserModeMutexCall

    /* disable interrupts (so that mepc is not changed) */
    csrc    mstatus , 1 << PHAROS_CPU_MSTATUS_MIE_BIT

    /* set the return address of mret to pharosCpuExecuteUserModeMutexCall */
    csrw    mepc , ra

    /* to a machine return which will jump to pharosCpuExecuteUserModeMutexCall */
    mret

pharosCpuExecuteUserModeMutexCall:

    /* jump to the entry point of the thread (a0 already has the argument) */
    jalr    ra , a7
 
    /*************************************************************************/
    /* Switch to EL1 mode with a syscall made in a well-known address        */
    /*************************************************************************/
    
    li      gp , PHAROS_SYSCALL_SPORADIC_THREAD_ENDED << 3

    /* make the syscall to restore supervisor mode */
    ecall   

     /* define the label position just after the swi call */
pharosSporadicThreadEndMutex:

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
    ld      t1  , 14*8 (sp)

    /* restore the original position of the kernel stack on the TCB */
    sd      t1 , PHAROS_CPU_OFFSET_KERNEL_STACK (a7)

    /* restore the stack pointer position */
    addi    sp , sp , 15 * 8

    /* return from function */
    ret 

.endfunc
 



/**
 * void pharosCpuExecuteUserModeEvent(void *partitionData , Event event , ptrThreadInfo thread);
 */
.func pharosCpuExecuteUserModeEvent
pharosCpuExecuteUserModeEvent:

    /* when we arrive here we are in machine mode */

    /* save all registers that the callee is supposed to save (the application may not respect the procedure 
     * call standard and do a messy thing, so we have to make sure that it can't brake Pharos) */

    addi    sp , sp , -15 * 8

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

    /* save the kernel stack pointer in the TCB (we will override it) */
    ld      t1 , PHAROS_CPU_OFFSET_KERNEL_STACK (a2)
  
    /* save it to the kernel stack */
    sd      t1 , 14*8 (sp)

    /*************************************************************************/
    /* switch to user mode                                                   */
    /*************************************************************************/

    /* get the entry point of the function */
    ld      a7 , PHAROS_CPU_OFFSET_ENTRY_POINT (a2)

    /* save the stack pointer to the TCB */
    sd      sp , PHAROS_CPU_OFFSET_KERNEL_STACK (a2)

    /* switch to user stack */
    ld      sp , PHAROS_CPU_OFFSET_USER_STACK (a2)

    /* set the return address (ra) to pharosCpuExecuteUserModeEventCall */
    la      ra , pharosCpuExecuteUserModeEventCall

    /* disable interrupts (so that mepc is not changed) */
    csrc    mstatus , 1 << PHAROS_CPU_MSTATUS_MIE_BIT

    /* set the return address of mret to pharosCpuExecuteUserModeEventCall */
    csrw    mepc , ra

    /* to a machine return which will jump to pharosCpuExecuteUserModeEventCall */
    mret

pharosCpuExecuteUserModeEventCall:

    /* jump to the entry point of the thread (a0 already has the argument) */
    jalr    ra , a7
 
    /*************************************************************************/
    /* Switch to EL1 mode with a syscall made in a well-known address        */
    /*************************************************************************/

    li      gp , PHAROS_SYSCALL_SPORADIC_THREAD_ENDED << 3

    /* make the syscall to restore supervisor mode */
    ecall

     /* define the label position just after the swi call */
pharosSporadicThreadEndEvent:

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
    ld      t1  , 14*8 (sp)

    /* restore the original position of the kernel stack on the TCB */
    sd      t1 , PHAROS_CPU_OFFSET_KERNEL_STACK (a7)

    /* restore the stack pointer position */
    addi    sp , sp , 15 * 8

    /* return from function */
    ret 

.endfunc


/**
 * void pharosCpuExecuteUserModeLwQueue(void *partitionData , LightWeightQueueNumber number, ptrBulk message, uint8_t *buffer , uint32_t size , ptrThreadInfo thread);
 * 
 * a0 - partitionData
 * a1 - number
 * a2 - message
 * a3 - buffer
 * a4 - size
 * a5 - thread
 */
.func pharosCpuExecuteUserModeLwQueue
pharosCpuExecuteUserModeLwQueue:
    
    /* when we arrive here we are in machine mode */

    /* save all registers that the callee is supposed to save (the application may not respect the procedure 
     * call standard and do a messy thing, so we have to make sure that it can't brake Pharos) */

    addi    sp , sp , -15 * 8

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

    /* save the kernel stack pointer in the TCB (we will override it) */
    ld      t1 , PHAROS_CPU_OFFSET_KERNEL_STACK (a5)
  
    /* save it to the kernel stack */
    sd      t1 , 14*8 (sp)

    /*************************************************************************/
    /* switch to user mode                                                   */
    /*************************************************************************/

    /* get the entry point of the function */
    ld      a7 , PHAROS_CPU_OFFSET_ENTRY_POINT (a5)

    /* save the stack pointer to the TCB */
    sd      sp , PHAROS_CPU_OFFSET_KERNEL_STACK (a5)

    /* switch to user stack */
    ld      sp , PHAROS_CPU_OFFSET_USER_STACK (a5)

    /* set the return address (ra) to pharosCpuExecuteUserModeLwQueueCall */
    la      ra , pharosCpuExecuteUserModeLwQueueCall

    /* disable interrupts (so that mepc is not changed) */
    csrc    mstatus , 1 << PHAROS_CPU_MSTATUS_MIE_BIT

    /* set the return address of mret to pharosCpuExecuteUserModeLwQueueCall */
    csrw    mepc , ra

    /* to a machine return which will jump to pharosCpuExecuteUserModeLwQueueCall */
    mret

pharosCpuExecuteUserModeLwQueueCall:

    /* jump to the entry point of the thread (a0 - a3 already have the argument) */
    jalr    ra , a7

    /*************************************************************************/
    /* Switch to EL1 mode with a syscall made in a well-known address        */
    /*************************************************************************/

    li      gp , PHAROS_SYSCALL_SPORADIC_THREAD_ENDED << 3

    /* make the syscall to restore supervisor mode */
    ecall

     /* define the label position just after the swi call */
pharosSporadicThreadEndLwQueue:

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
    ld      t1  , 14*8 (sp)

    /* restore the original position of the kernel stack on the TCB */
    sd      t1 , PHAROS_CPU_OFFSET_KERNEL_STACK (a7)

    /* restore the stack pointer position */
    addi    sp , sp , 15 * 8

    /* return from function */
    ret 

.endfunc


/**
 * void pharosCpuExecuteUserModeHwQueue(void *partitionData , HeavyWeightQueueNumber queueId, uint8_t *buffer, 
 *                                      uint32_t size , PharosPriority priority , FutureNumber future , 
 *                                      ptrThreadInfo thread);
 *
 * a0 - partitionData
 * a1 - queueId
 * a2 - buffer
 * a3 - size
 * a4 - priority
 * a5 - future
 * a6 - thread
 */
.func pharosCpuExecuteUserModeHwQueue
pharosCpuExecuteUserModeHwQueue:

    /* when we arrive here we are in machine mode */

    /* save all registers that the callee is supposed to save (the application may not respect the procedure 
     * call standard and do a messy thing, so we have to make sure that it can't brake Pharos) */

    addi    sp , sp , -15 * 8

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

    /* save the kernel stack pointer in the TCB (we will override it) */
    ld      t1 , PHAROS_CPU_OFFSET_KERNEL_STACK (a6)
  
    /* save it to the kernel stack */
    sd      t1 , 14*8 (sp)

    /*************************************************************************/
    /* switch to user mode                                                   */
    /*************************************************************************/

    /* get the entry point of the function */
    ld      a7 , PHAROS_CPU_OFFSET_ENTRY_POINT (a6)

    /* save the stack pointer to the TCB */
    sd      sp , PHAROS_CPU_OFFSET_KERNEL_STACK (a6)

    /* switch to user stack */
    ld      sp , PHAROS_CPU_OFFSET_USER_STACK (a6)

    /* set the return address (ra) to pharosCpuExecuteUserModeHwQueueCall */
    la      ra , pharosCpuExecuteUserModeHwQueueCall

    /* disable interrupts (so that mepc is not changed) */
    csrc    mstatus , 1 << PHAROS_CPU_MSTATUS_MIE_BIT

    /* set the return address of mret to pharosCpuExecuteUserModeHwQueueCall */
    csrw    mepc , ra

    /* to a machine return which will jump to pharosCpuExecuteUserModeHwQueueCall */
    mret

pharosCpuExecuteUserModeHwQueueCall:

    /* jump to the entry point of the thread (a0 - a5 already have the argument) */
    jalr    ra , a7
 
    /*************************************************************************/
    /* Switch to EL1 mode with a syscall made in a well-known address        */
    /*************************************************************************/

    li      gp , PHAROS_SYSCALL_SPORADIC_THREAD_ENDED << 3

    /* make the syscall to restore supervisor mode */
    ecall

     /* define the label position just after the swi call */
pharosSporadicThreadEndHwQueue:

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
    ld      t1  , 14*8 (sp)

    /* restore the original position of the kernel stack on the TCB */
    sd      t1 , PHAROS_CPU_OFFSET_KERNEL_STACK (a7)

    /* restore the stack pointer position */
    addi    sp , sp , 15 * 8

    /* return from function */
    ret 

.endfunc


/**
 * void pharosCpuExecuteUserModeChannel(void *partitionData , ptrChannelBulk bulk , 
 *                                      ChannelId channelId , PharosPriority priority , ptrFutureId number ,
 *                                      ptrThreadInfo thread);
 *
 * a0 - partitionData
 * a1 - bulk
 * a2 - channelId
 * a3 - priority
 * a4 - number
 * a5 - thread
 */
.func pharosCpuExecuteUserModeChannel
pharosCpuExecuteUserModeChannel:

    /* when we arrive here we are in machine mode */

    /* save all registers that the callee is supposed to save (the application may not respect the procedure 
     * call standard and do a messy thing, so we have to make sure that it can't brake Pharos) */

    addi    sp , sp , -15 * 8

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

    /* save the kernel stack pointer in the TCB (we will override it) */
    ld      t1 , PHAROS_CPU_OFFSET_KERNEL_STACK (a5)
  
    /* save it to the kernel stack */
    sd      t1 , 14*8 (sp)

    /*************************************************************************/
    /* switch to user mode                                                   */
    /*************************************************************************/

    /* get the entry point of the function (we are overriding a7 so it must be the last instruction to use it) */
    ld      a7 , PHAROS_CPU_OFFSET_ENTRY_POINT (a5)

    /* save the stack pointer to the TCB */
    sd      sp , PHAROS_CPU_OFFSET_KERNEL_STACK (a5)

    /* switch to user stack */
    ld      sp , PHAROS_CPU_OFFSET_USER_STACK (a5)

    /* set the return address (ra) to pharosCpuExecuteUserModeChannelCall */
    la      ra , pharosCpuExecuteUserModeChannelCall

    /* disable interrupts (so that mepc is not changed) */
    csrc    mstatus , 1 << PHAROS_CPU_MSTATUS_MIE_BIT

    /* set the return address of mret to pharosCpuExecuteUserModeChannelCall */
    csrw    mepc , ra

    /* to a machine return which will jump to pharosCpuExecuteUserModeChannelCall */
    mret

pharosCpuExecuteUserModeChannelCall:

    /* jump to the entry point of the thread (a0 - a4 already have the argument) */
    jalr    ra , a7

    /*************************************************************************/
    /* Switch to EL1 mode with a syscall made in a well-known address        */
    /*************************************************************************/

    li      gp , PHAROS_SYSCALL_SPORADIC_THREAD_ENDED << 3

    /* make the syscall to restore supervisor mode */
    ecall

     /* define the label position just after the swi call */
pharosSporadicThreadEndChannel:

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
    ld      t1  , 14*8 (sp)

    /* restore the original position of the kernel stack on the TCB */
    sd      t1 , PHAROS_CPU_OFFSET_KERNEL_STACK (a7)

    /* restore the stack pointer position */
    addi    sp , sp , 15 * 8

    /* return from function */
    ret 

.endfunc
