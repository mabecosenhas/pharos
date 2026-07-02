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

.type pharosCpuTrapHandler,                         %function
.type pharosCpuMachineSyscallHandler,               %function
.type pharosCpuSyscallRestoreTlb,                   %function
.type pharosCpuUserModeInternalSyscall,             %function


.global pharosCpuTrapHandler
.global pharosCpuMachineSyscallHandler
.global pharosCpuSyscallRestoreTlb
.global pharosCpuUserModeInternalSyscall


.section .text


.func pharosCpuSyscallHandler
pharosCpuSyscallHandler:

    /* at this point:
     *   a0 has running core * 8
     *   a1 has mcause
     *   a2 has PHAROS_CPU_EXCEPTION_ENV_CALL_U
     *   gp contains the syscall number
     * 
     * since this is a syscall (synchronous trap triggered by the software) then we only need to make sure that sp, ra 
     * and s0-s11 registers don't change
     */

    /*************************************************************/
    /* Save the CPU context to the thread stack (sp and mstatus) */
    /*************************************************************/

    /* check for special case of syscall > PHAROS_SYSCALL_API_LAST_CALL */
    li      a2 , PHAROS_SYSCALL_API_LAST_CALL << 3

    /* branch if syscall is > PHAROS_SYSCALL_API_LAST_CALL */
    blt     a2 , gp , pharosCpuUserModeInternalSyscall


    /* restore a0 */
    ld      a0 , -1*8 (sp)

    /* restore a1 */
    ld      a1 , -2*8 (sp)

    /* restore a2 */
    ld      a2 , -3*8 (sp)

    /* read the mstatus */
    csrr    t0 , mstatus

    /* now we can enable interrupts since MSTATUS has been saved and sp does not point to information we need to restore */
    csrs    mstatus , 1 << PHAROS_CPU_MSTATUS_MIE_BIT

    /* load the running hart */
    csrr    t2 , mhartid

    /* multiply by PHAROS_CPU_CORE_RMP_SIZE (size of the PharosRmpCore structure) */
    slli    t2 , t2 , PHAROS_CPU_CORE_RMP_SIZE_LOG2

    /* get the running thread */
    la      t1 , pharosVRmpCore

    /* get the running thread address for the running hart */
    add     t1 , t1 , t2
 
    /* get the running thread address for the running hart */
    ld      t1 , (t1)

    /* get the running kernel stack pointer */
    ld      t3 , PHAROS_CPU_OFFSET_KERNEL_STACK (t1)
   
    /* reserve space on the stack to store the user sp */
    addi    t3 , t3 , -8 * 3

    /* store the user stack inside the kernel stack */
    sd      sp , 0 * 8 (t3)

    /* also save the user stack to the TCB (useful for restoring it for critical sections) */
    sd      sp , PHAROS_CPU_OFFSET_USER_STACK_CURRENT (t1)
 
    /* now that we saved sp of user, we can switch the stack */
    mv      sp , t3
   
    /* store the MSTATUS inside the kernel stack */
    sd      t0 , 1 * 8 (sp) 

    /* store the ra inside the kernel stack */
    sd      ra , 2 * 8 (sp) 

    /***************************************************/
    /* Call the general syscall handler                */
    /***************************************************/

    /* load the syscall vector of functions */
    la      t3 , pharosVSystemCalls

    /* increase the pointer to the vector index being called (gp contains the call number) */
    add     t3 , t3 , gp

    /* t3 now contains the address of the function to be called */
    ld      t3 , (t3)

    /* call the function */
    jalr    t3

    /* return values can be in a0 and a1 */

    /***************************************************/
    /* Restore the registers                           */
    /***************************************************/

    /* restore the MSTATUS from the kernel stack */
    ld      t0 , 1 * 8 (sp) 

     /* restore the mstatus (and disable interrupts) */
    csrw    mstatus , t0

    /* restore the ra inside the kernel stack */
    ld      ra , 2 * 8 (sp) 

    /* restore the sp (user stack) from the kernel stack */
    ld      sp , 0 * 8 (sp) 

    /* set the return address of mret */
    csrw    mepc , ra
 
    /* leave the syscall */
    mret


.endfunc


.func pharosCpuMachineSyscallHandler
pharosCpuMachineSyscallHandler:

    /* at this point:
     *   a0 has running core * 8
     *   a1 has mcause
     *   a2 has PHAROS_CPU_EXCEPTION_ENV_CALL_U
     *   gp contains the syscall number
     * 
     * since this is a syscall (synchronous trap triggered by the software) then we only need to make sure that sp, ra 
     * and s0-s11 registers don't change
     */
     
    /* check the special case of a restore TLB */
    beq     gp , x0 , pharosCpuSyscallRestoreTlb

    /* get the instruction that generated the syscall */
    csrr    a0 , mepc

    /* load the next instruction */
    addi    a0 , a0 , 4

    /* lets return to the following instruction */
    csrw    mepc , a0

    /* restore a0 */
    ld      a0 , -1*8 (sp)

    /* restore a1 */
    ld      a1 , -2*8 (sp)

    /* restore a2 */
    ld      a2 , -3*8 (sp)

    /* leave the exception */
    mret

.endfunc

    /*********************************************************************/
    /* special case of restoring the TLB                                 */
    /*********************************************************************/

.func pharosCpuSyscallRestoreTlb
pharosCpuSyscallRestoreTlb:

    /* place 3 in the MPP bits of the mstatus */
    li      a0 , 3 << PHAROS_CPU_MSTATUS_MPP_BIT

    /* we have to restore to user mode */
    csrc    mstatus , a0

    /*********************************************************************/
    /* Get the running thread of the running core to a0                  */
    /*********************************************************************/

    /* load the running hart */
    csrr    a0 , mhartid

    /* multiply by PHAROS_CPU_CORE_RMP_SIZE (size of the PharosRmpCore structure) */
    slli    a0 , a0 , PHAROS_CPU_CORE_RMP_SIZE_LOG2

    /* get the running thread */
    la      a2 , pharosVRmpCore

    /* calculate the final address where the running thread for the running core is on */
    add     a2 , a2 , a0

    /* get the running thread */
    ld      a0 , (a2)
  
    /* add the offset to the return address */
    addi    a0 , a0 , PHAROS_CPU_CONTEXT_DIRECT_IO_RA_OFFSET

    /* get the return address to a0 */
    ld      a0 , (a0)
     
    /* set the mepc so that the mret will return to the next instruction */
    csrw    mepc , a0
 
    /* restore a0 */
    ld      a0 , -1*8 (sp)

    /* restore a1 */
    ld      a1 , -2*8 (sp)

    /* restore a2 */
    ld      a2 , -3*8 (sp)

    /* leave exception */
    mret
.endfunc


.func pharosCpuUserModeInternalSyscall
pharosCpuUserModeInternalSyscall:

    /* in user mode API internal syscall (a0 - a2 have been saved previously to the stack) */

    /* it's ok to use the a's registers since we don't have to restore them (e.g. in pharosCpuExecuteUserModeEvent) */

    /* place in a0 the instruction that triggered the fault */
    csrr    a0 , mepc

    /* set the argument to the label that follows the ecall */
    addi    a0 , a0 , 4

    /* place in a1 the mstatus */
    csrr    a1 , mstatus

    /* load the syscall vector of functions */
    la      a2 , pharosVSystemCalls

    /* increase the pointer to the vector index being called (gp contains the call number) */
    add     a2 , a2 , gp

    /* a2 now contains the address of the function to be called */
    ld      a2 , (a2)

    /* decrement the stack (3 registers a0...a2 already saved and two more will be saved) */
    addi    sp , sp, -(5 * 8)
   
    /* store the ra */
    sd      ra , 0*8 (sp)
    
    /* store the mepc (a dispatch could occur that would override this register) */
    sd      a0 , 1*8 (sp)

    /* call the function */
    jalr    a2

    /* replace the mstatus */
    csrw    mstatus, a0

    /* restore the mepc */
    ld      a2 , 1*8 (sp)

    /* restore the ra */
    ld      ra , 0*8 (sp)

    /* restore the stack to its original position */
    addi    sp , sp, +(5 * 8)

    /* restore the mepc */
    csrw    mepc , a2

    /* don't need to switch the sp to the kernel stack pointer, it will be done when we return */

    /* restore a0 (will hold the mcause) */
    ld      a0 , -1*8 (sp)

    /* save a1 (will hold temporary info) */
    ld      a1 , -2*8 (sp)

    /* no need to restore a2 since only a0 and a1 are used as return registers */

    /* leave exception */
    mret

.endfunc


.func pharosCpuTrapHandler
.align 4
pharosCpuTrapHandler:

    /* save a0 (will hold the mhartid) */
    sd      a0 , -1*8 (sp)

    /* save a1 (will hold mcause) */
    sd      a1 , -2*8 (sp)

    /* save a2 (will hold something later on) */
    sd      a2 , -3*8 (sp)

    /* load the running hart */
    csrr    a0 , mhartid

    /* multiply by PHAROS_CPU_CORE_RMP_SIZE (size of the PharosRmpCore structure) */
    slli    a0 , a0 , PHAROS_CPU_CORE_RMP_SIZE_LOG2

    /* read the cause */
    csrr    a1, mcause

    /* if the interrupt bit is set (that is, mcause is < 0) then jump to interrupt handler */
    blt     a1, x0, pharosCpuInterruptHandler

    /* if we are here then we are running an exception, not an interrupt */
pharosCpuExceptionHandler:

    /* load to a2 the exception number for environment call from U-mode */
    li      a2 , PHAROS_CPU_EXCEPTION_ENV_CALL_U

    /* check if the exception is a syscall from user mode */
    beq     a1 , a2 , pharosCpuSyscallHandler

    /* check if the exception number is a syscall from Machine mode */
    li      a2 , PHAROS_CPU_EXCEPTION_ENV_CALL_M

    /* check if the exception is a syscall from machine mode */
    beq     a1 , a2 , pharosCpuMachineSyscallHandler

    /* if we are here then it is not a syscall */

    /* clear the interrupt flag bit */
    andi    a1 , a1 , 0xff

    /* not a syscall, then a general exception that should be handled accordingly */

    /*********************************************************************/
    /* increment thread dispatch                                         */
    /*********************************************************************/

    /* get the address where the thread dispatch level is needed for each core */
    la      a2 , pharosVRmpCore

    /* add the thread dispatch level offset */
    add     a2 , a2 , PHAROS_CORE_TABLE_DISPATCH_LEVEL_OFFSET

     /* get the dispatch level address for the running hart */
    add     a2 , a2 , a0

    /* load the value of thread dispatch level */
    ld      a0 , (a2)
    
    /* increment the thread dispatch level */
    addi    a0 , a0 , 1

    /* store it back */
    sd      a0 , (a2)

    /*********************************************************************/
    /* call exception handler                                            */
    /*********************************************************************/

    /* place in a0 the exception number */
    mv      a0 , a1

    /* place in a1 the instruction that triggered the exception */
    csrr    a1 , mepc

    /* place in the third argument the address that was being accessed (if it makes sense, otherwise mtval could be 0) */
    csrr    a2 , mtval

    /* decrement the stack (3 registers a0...a2 already saved plus the next 13) */
    addi    sp , sp, -(16 * 8)
   
    sd      ra , 0*8 (sp)
    sd      a3 , 1*8 (sp)
    sd      a4 , 2*8 (sp)
    sd      a5 , 3*8 (sp)
    sd      a6 , 4*8 (sp)
    sd      a7 , 5*8 (sp)
    sd      t0 , 6*8 (sp)
    sd      t1 , 7*8 (sp)
    sd      t2 , 8*8 (sp)
    sd      t3 , 9*8 (sp)
    sd      t4 , 10*8 (sp)
    sd      t5 , 11*8 (sp)
    sd      t6 , 12*8 (sp)

    /* invoke the C handler for the exception */
    jal     pharosCpuExceptionHandlerC

    /*********************************************************************/
    /* decrement thread dispatch                                         */
    /*********************************************************************/

    /* load the running hart */
    csrr    a0 , mhartid

    /* multiply by PHAROS_CPU_CORE_RMP_SIZE (size of the PharosRmpCore structure) */
    slli    a0 , a0 , PHAROS_CPU_CORE_RMP_SIZE_LOG2

    /* get the address where the thread dispatch level is needed for each core */
    la     a2 , pharosVRmpCore

    /* add the thread dispatch level offset */
    add     a2 , a2 , PHAROS_CORE_TABLE_DISPATCH_LEVEL_OFFSET

     /* get the dispatch level address for the running hart */
    add     a2 , a2 , a0

    /* load the value of thread dispatch level */
    ld      a0 , (a2)
    
    /* decrement the thread dispatch level */
    addi    a0 , a0 , -1

    /* store it back */
    sd      a0 , (a2)

    /*********************************************************************/
    /* dispatch to another thread (if needed)                            */
    /*********************************************************************/

    /* dispatch to another thread if needed */
    jal     pharosIDispatchThread
 
    /* will only reach here if the thread that originated the fault is using a direct I/O */
   
    /*********************************************************************/
    /* Get the running thread of the running core to a0                  */
    /*********************************************************************/

    /* load the running hart */
    csrr    a0 , mhartid

    /* multiply by PHAROS_CPU_CORE_RMP_SIZE (size of the PharosRmpCore structure) */
    slli    a0 , a0 , PHAROS_CPU_CORE_RMP_SIZE_LOG2

    /* get the address of the array of running threads */
	la      a2 , pharosVRmpCore

    /* calculate the final address where the running thread for the running core is on */
    add     a2 , a2 , a0

    /* get the running thread */
    ld      a0 , (a2)

    /*********************************************************************/
    /* restore the registers saved to the stack                          */
    /*********************************************************************/

    /* when we leave the exception we execute the thread direct I/O memory area (ioAccess of the thread CPU context) */
    addi    a0 , a0 , PHAROS_CPU_CONTEXT_DIRECT_IO_OFFSET
 
    /* set that address as the return address of the exception */
    csrw    mepc , a0

    /* place 3 in the MPP bits of the mstatus */
    li      a0 , 3 << PHAROS_CPU_MSTATUS_MPP_BIT

    /* we have to restore to a machine mode so that it can execute the instruction that triggered the fault */
    csrs    mstatus , a0 

    /* restore all registers that could have changed */
    ld      ra , 0*8 (sp)
    ld      a3 , 1*8 (sp)
    ld      a4 , 2*8 (sp)
    ld      a5 , 3*8 (sp)
    ld      a6 , 4*8 (sp)
    ld      a7 , 5*8 (sp)
    ld      t0 , 6*8 (sp)
    ld      t1 , 7*8 (sp)
    ld      t2 , 8*8 (sp)
    ld      t3 , 9*8 (sp)
    ld      t4 , 10*8 (sp)
    ld      t5 , 11*8 (sp)
    ld      t6 , 12*8 (sp)

    /* restore a0 */
    ld      a0 , 15*8 (sp)

    /* restore a1 */
    ld      a1 , 14*8 (sp)

    /* restore a2 */
    ld      a2 , 13*8 (sp)

    /* restore the stack  */
    addi    sp , sp, +(16 * 8)
   
    /* leave exception */
    mret


pharosCpuInterruptHandler:


    /* at this point:
     *   a0 has running core * sizeof(PharosRmpCore)
     *   a1 has mcause
     *   a2 has already been saved to the stack 
     */

    /****************************************************************/
    /* Check if running in the outermost interrupt and increment it */
    /****************************************************************/

    /* get the address of the array of running threads */
	la      a2 , pharosVRmpCore

    /* make a2 point to &pharosVRmpCore[runningCore] */
    add     a2 , a2 , a0

    /* add the interrupt nest level offset to make a2 point to &pharosVRmpCore[core]->interruptLevel */
    add     a2 , a2 , PHAROS_CORE_TABLE_INTERRUPT_LEVEL_OFFSET

    /* load the value of pharosVRmpCore[core]->interruptLevel */
    ld      a2 , (a2)

    /* if not 0, then go to nested interrupt (if nested, sp is already on the interrupt stack) */
    bne     a2, x0 , nestedInterrupt

    /* if we are here then this is an outermost interrupt */
outermostInterrupt:

    /***************************************************/
    /* Save context to TCB                             */
    /***************************************************/

    /* get the running thread */
    la      a2 , pharosVRmpCore

    /* make a2 point to &pharosVRmpCore[runningCore] which is equal to &pharosVRmpCore[runningCore]->runningThread 
     * since the runningThread is the first field */
    add     a2 , a2 , a0

    /* get the running thread address for the running hart */
    ld      a2 , (a2)   

    /* save the current stack pointer to the TCB. Save this first to be able to enable interrupts sooner */
    sd      sp  , PHAROS_CPU_TCB_IRQ_SP_OFFSET (a2)

    /* save also another variable to hold the running thread, mpec and mstatus and mcause */
    sd      t0  , PHAROS_CPU_TCB_IRQ_T0_OFFSET (a2)
    sd      t1  , PHAROS_CPU_TCB_IRQ_T1_OFFSET (a2)
    sd      t2  , PHAROS_CPU_TCB_IRQ_T2_OFFSET (a2)
    sd      t3  , PHAROS_CPU_TCB_IRQ_T3_OFFSET (a2)

    /* save the running thread to t0 (we just saved t0 so we can use it) */
    mv      t0 , a2

    /* save mepc */
    csrr    t1 , mepc

    /* save mstatus */
    csrr    t2 , mstatus

    /* save mcause */
    mv      t3 , a1

    /* restore a0 */
    ld      a0 , -1*8 (sp)

    /* restore a1 */
    ld      a1 , -2*8 (sp)

    /* restore a2 */
    ld      a2 , -3*8 (sp)

    /* save the remaining context to the running thread TCB */
    sd      ra  , PHAROS_CPU_TCB_IRQ_RA_OFFSET  (t0)
    sd      gp  , PHAROS_CPU_TCB_IRQ_GP_OFFSET  (t0)
    sd      tp  , PHAROS_CPU_TCB_IRQ_TP_OFFSET  (t0)
    sd      a0  , PHAROS_CPU_TCB_IRQ_A0_OFFSET  (t0)
    sd      a1  , PHAROS_CPU_TCB_IRQ_A1_OFFSET  (t0)
    sd      a2  , PHAROS_CPU_TCB_IRQ_A2_OFFSET  (t0)
    sd      a3  , PHAROS_CPU_TCB_IRQ_A3_OFFSET  (t0)
    sd      a4  , PHAROS_CPU_TCB_IRQ_A4_OFFSET  (t0)
    sd      a5  , PHAROS_CPU_TCB_IRQ_A5_OFFSET  (t0)
    sd      a6  , PHAROS_CPU_TCB_IRQ_A6_OFFSET  (t0)
    sd      a7  , PHAROS_CPU_TCB_IRQ_A7_OFFSET  (t0)
    sd      s0  , PHAROS_CPU_TCB_IRQ_S0_OFFSET  (t0)
    sd      s1  , PHAROS_CPU_TCB_IRQ_S1_OFFSET  (t0)
    sd      s2  , PHAROS_CPU_TCB_IRQ_S2_OFFSET  (t0)
    sd      s3  , PHAROS_CPU_TCB_IRQ_S3_OFFSET  (t0)
    sd      s4  , PHAROS_CPU_TCB_IRQ_S4_OFFSET  (t0)
    sd      s5  , PHAROS_CPU_TCB_IRQ_S5_OFFSET  (t0)
    sd      s6  , PHAROS_CPU_TCB_IRQ_S6_OFFSET  (t0)
    sd      s7  , PHAROS_CPU_TCB_IRQ_S7_OFFSET  (t0)
    sd      s8  , PHAROS_CPU_TCB_IRQ_S8_OFFSET  (t0)
    sd      s9  , PHAROS_CPU_TCB_IRQ_S9_OFFSET  (t0)
    sd      s10 , PHAROS_CPU_TCB_IRQ_S10_OFFSET (t0)
    sd      s11 , PHAROS_CPU_TCB_IRQ_S11_OFFSET (t0)
    sd      t4  , PHAROS_CPU_TCB_IRQ_T4_OFFSET  (t0)
    sd      t5  , PHAROS_CPU_TCB_IRQ_T5_OFFSET  (t0)
    sd      t6  , PHAROS_CPU_TCB_IRQ_T6_OFFSET  (t0)

    /* save also the MEPC and MSTATUS */
    sd      t1  , PHAROS_CPU_TCB_IRQ_MEPC_OFFSET    (t0)
    sd      t2  , PHAROS_CPU_TCB_IRQ_MSTATUS_OFFSET (t0)

    /* if the CPU has floating point registers (double precision floating point) */
#ifdef PHAROS_CPU_FP

    /* reads the fcsr register to a3 */
    frcsr   a3 
    
    /* save the context of the FPU to the TCB */
    fsd     f0  , PHAROS_CPU_TCB_IRQ_F0_OFFSET   (t0)
    fsd     f1  , PHAROS_CPU_TCB_IRQ_F1_OFFSET   (t0)
    fsd     f2  , PHAROS_CPU_TCB_IRQ_F2_OFFSET   (t0)
    fsd     f3  , PHAROS_CPU_TCB_IRQ_F3_OFFSET   (t0)
    fsd     f4  , PHAROS_CPU_TCB_IRQ_F4_OFFSET   (t0)
    fsd     f5  , PHAROS_CPU_TCB_IRQ_F5_OFFSET   (t0)
    fsd     f6  , PHAROS_CPU_TCB_IRQ_F6_OFFSET   (t0)
    fsd     f7  , PHAROS_CPU_TCB_IRQ_F7_OFFSET   (t0)
    fsd     f8  , PHAROS_CPU_TCB_IRQ_F8_OFFSET   (t0)
    fsd     f9  , PHAROS_CPU_TCB_IRQ_F9_OFFSET   (t0)
    fsd     f10 , PHAROS_CPU_TCB_IRQ_F10_OFFSET  (t0)
    fsd     f11 , PHAROS_CPU_TCB_IRQ_F11_OFFSET  (t0)
    fsd     f12 , PHAROS_CPU_TCB_IRQ_F12_OFFSET  (t0)
    fsd     f13 , PHAROS_CPU_TCB_IRQ_F13_OFFSET  (t0)
    fsd     f14 , PHAROS_CPU_TCB_IRQ_F14_OFFSET  (t0)
    fsd     f15 , PHAROS_CPU_TCB_IRQ_F15_OFFSET  (t0)
    fsd     f16 , PHAROS_CPU_TCB_IRQ_F16_OFFSET  (t0)
    fsd     f17 , PHAROS_CPU_TCB_IRQ_F17_OFFSET  (t0)
    fsd     f18 , PHAROS_CPU_TCB_IRQ_F18_OFFSET  (t0)
    fsd     f19 , PHAROS_CPU_TCB_IRQ_F19_OFFSET  (t0)
    fsd     f20 , PHAROS_CPU_TCB_IRQ_F20_OFFSET  (t0)
    fsd     f21 , PHAROS_CPU_TCB_IRQ_F21_OFFSET  (t0)
    fsd     f22 , PHAROS_CPU_TCB_IRQ_F22_OFFSET  (t0)
    fsd     f23 , PHAROS_CPU_TCB_IRQ_F23_OFFSET  (t0)
    fsd     f24 , PHAROS_CPU_TCB_IRQ_F24_OFFSET  (t0)
    fsd     f25 , PHAROS_CPU_TCB_IRQ_F25_OFFSET  (t0)
    fsd     f26 , PHAROS_CPU_TCB_IRQ_F26_OFFSET  (t0)
    fsd     f27 , PHAROS_CPU_TCB_IRQ_F27_OFFSET  (t0)
    fsd     f28 , PHAROS_CPU_TCB_IRQ_F28_OFFSET  (t0)
    fsd     f29 , PHAROS_CPU_TCB_IRQ_F29_OFFSET  (t0)
    fsd     f30 , PHAROS_CPU_TCB_IRQ_F30_OFFSET  (t0)
    fsd     f31 , PHAROS_CPU_TCB_IRQ_F31_OFFSET  (t0)
    sd      a3  , PHAROS_CPU_TCB_IRQ_FCSR_OFFSET (t0)
 
#endif

    /***************************************************/
    /* Change stack to the interrupt stack             */
    /***************************************************/
    
    /* get the base address of the hart 0 interrupt stack */
    la      a3 , pharosCpuInterruptStack 

    /* load the running hart */
    csrr    a0 , mhartid

     /* increment the core number by one */
    add     a0 , a0 , 1
 
    /* get to a0 the size of the interrupt stack times the hart number */
    slli    a0 , a0 , PHAROS_CPU_INTERRUPT_STACK_SIZE_LOG2 

    /* calculate the address of the pharosCpuInterruptStack for the running hart */
    add     sp , a3 , a0
 
    /***************************************************/
    /* call the C handler                              */
    /***************************************************/

    /* set the first argument (mcause) */
    mv      a0 , t3 

    /* set the second argument (mepc = instruction that was interrupted) */
    mv      a1 , t1

    /* call the C handler */
    jal     pharosCpuCInterruptHandler
 
    /* at this time a0 contains the &pharosVRmpCore[core] */ 

    /*******************************************************************************/
    /* check if a context switch is possible (pharosVRmpCore[core]->dispatchLevel) */
	/*******************************************************************************/
    
    /* add the thread dispatch nest level offset to make a2 point to &pharosVRmpCore[core]->dispatchLevel */
    add     a2 , a0 , PHAROS_CORE_TABLE_DISPATCH_LEVEL_OFFSET

    /* load the value of pharosVRmpCore[core]->dispatchLevel */
    ld      a2 , (a2)

    /* check if a thread dispatch is possible */
    bne     a2 , x0 , leaveOutermost

    /***********************************************************************************/
    /* check if a context switch is needed (pharosVRmpCore[core]->contextSwitchNeeded) */
	/***********************************************************************************/
    
    /* add the thread dispatch nest level offset to make a2 point to &pharosVRmpCore[core]->contextSwitchNeeded */
    add     a2 , a0 , PHAROS_CORE_TABLE_CONTEXT_NEEDED_OFFSET

    /* load the value of pharosVRmpCore[core]->contextSwitchNeeded */
    ld      a2 , (a2)

    /* check if a thread dispatch is needed */
    beq     a2 , x0 , leaveOutermost

    /********************************************************/
    /* do a thread dispatch                                 */
    /********************************************************/

    /* load the value of pharosVRmpCore[core]->runningThread (first argument to pharosIContextSwitch) */
    ld      a0 , (a0)
  
    /* dispatch to other threads if needed */
	jal     pharosIContextSwitch

leaveOutermost:  

    /***************************************************/
    /* restore most of the context                     */
    /***************************************************/

    /* load the running hart */
    csrr    a0 , mhartid

    /* multiply by PHAROS_CPU_CORE_RMP_SIZE (size of the PharosRmpCore structure) */
    slli    a0 , a0 , PHAROS_CPU_CORE_RMP_SIZE_LOG2

    /* get the address of the cores array (&pharosVRmpCore[0]) */
    la      a2 , pharosVRmpCore

    /* get the running thread address for the running hart (&pharosVRmpCore[runningCore]->runningThread) */
    add     a2 , a2 , a0

    /* get the running thread address for the running hart */
    ld      a2 , (a2)   

    /* if the CPU has floating point registers (double precision floating point) */
#ifdef PHAROS_CPU_FP

    /* load the FPU registers */
    ld      a3  , PHAROS_CPU_TCB_IRQ_FCSR_OFFSET (a2)
    fld     f0  , PHAROS_CPU_TCB_IRQ_F0_OFFSET   (a2)
    fld     f1  , PHAROS_CPU_TCB_IRQ_F1_OFFSET   (a2)
    fld     f2  , PHAROS_CPU_TCB_IRQ_F2_OFFSET   (a2)
    fld     f3  , PHAROS_CPU_TCB_IRQ_F3_OFFSET   (a2)
    fld     f4  , PHAROS_CPU_TCB_IRQ_F4_OFFSET   (a2)
    fld     f5  , PHAROS_CPU_TCB_IRQ_F5_OFFSET   (a2)
    fld     f6  , PHAROS_CPU_TCB_IRQ_F6_OFFSET   (a2)
    fld     f7  , PHAROS_CPU_TCB_IRQ_F7_OFFSET   (a2)
    fld     f8  , PHAROS_CPU_TCB_IRQ_F8_OFFSET   (a2)
    fld     f9  , PHAROS_CPU_TCB_IRQ_F9_OFFSET   (a2)
    fld     f10 , PHAROS_CPU_TCB_IRQ_F10_OFFSET  (a2)
    fld     f11 , PHAROS_CPU_TCB_IRQ_F11_OFFSET  (a2)
    fld     f12 , PHAROS_CPU_TCB_IRQ_F12_OFFSET  (a2)
    fld     f13 , PHAROS_CPU_TCB_IRQ_F13_OFFSET  (a2)
    fld     f14 , PHAROS_CPU_TCB_IRQ_F14_OFFSET  (a2)
    fld     f15 , PHAROS_CPU_TCB_IRQ_F15_OFFSET  (a2)
    fld     f16 , PHAROS_CPU_TCB_IRQ_F16_OFFSET  (a2)
    fld     f17 , PHAROS_CPU_TCB_IRQ_F17_OFFSET  (a2)
    fld     f18 , PHAROS_CPU_TCB_IRQ_F18_OFFSET  (a2)
    fld     f19 , PHAROS_CPU_TCB_IRQ_F19_OFFSET  (a2)
    fld     f20 , PHAROS_CPU_TCB_IRQ_F20_OFFSET  (a2)
    fld     f21 , PHAROS_CPU_TCB_IRQ_F21_OFFSET  (a2)
    fld     f22 , PHAROS_CPU_TCB_IRQ_F22_OFFSET  (a2)
    fld     f23 , PHAROS_CPU_TCB_IRQ_F23_OFFSET  (a2)
    fld     f24 , PHAROS_CPU_TCB_IRQ_F24_OFFSET  (a2)
    fld     f25 , PHAROS_CPU_TCB_IRQ_F25_OFFSET  (a2)
    fld     f26 , PHAROS_CPU_TCB_IRQ_F26_OFFSET  (a2)
    fld     f27 , PHAROS_CPU_TCB_IRQ_F27_OFFSET  (a2)
    fld     f28 , PHAROS_CPU_TCB_IRQ_F28_OFFSET  (a2)
    fld     f29 , PHAROS_CPU_TCB_IRQ_F29_OFFSET  (a2)
    fld     f30 , PHAROS_CPU_TCB_IRQ_F30_OFFSET  (a2)
    fld     f31 , PHAROS_CPU_TCB_IRQ_F31_OFFSET  (a2)

    /* write a3 to the FCSR */
    fscsr   a3
#endif

    /* restore part of the context from the running thread TCB */
    ld      ra  , PHAROS_CPU_TCB_IRQ_RA_OFFSET  (a2)
    ld      gp  , PHAROS_CPU_TCB_IRQ_GP_OFFSET  (a2)
    ld      tp  , PHAROS_CPU_TCB_IRQ_TP_OFFSET  (a2)
    ld      s0  , PHAROS_CPU_TCB_IRQ_S0_OFFSET  (a2)
    ld      s1  , PHAROS_CPU_TCB_IRQ_S1_OFFSET  (a2)
    ld      a1  , PHAROS_CPU_TCB_IRQ_A1_OFFSET  (a2)
    ld      a3  , PHAROS_CPU_TCB_IRQ_A3_OFFSET  (a2)
    ld      a4  , PHAROS_CPU_TCB_IRQ_A4_OFFSET  (a2)
    ld      a5  , PHAROS_CPU_TCB_IRQ_A5_OFFSET  (a2)
    ld      a6  , PHAROS_CPU_TCB_IRQ_A6_OFFSET  (a2)
    ld      a7  , PHAROS_CPU_TCB_IRQ_A7_OFFSET  (a2)
    ld      s4  , PHAROS_CPU_TCB_IRQ_S4_OFFSET  (a2)
    ld      s5  , PHAROS_CPU_TCB_IRQ_S5_OFFSET  (a2)
    ld      s6  , PHAROS_CPU_TCB_IRQ_S6_OFFSET  (a2)
    ld      s7  , PHAROS_CPU_TCB_IRQ_S7_OFFSET  (a2)
    ld      s8  , PHAROS_CPU_TCB_IRQ_S8_OFFSET  (a2)
    ld      s9  , PHAROS_CPU_TCB_IRQ_S9_OFFSET  (a2)
    ld      s10 , PHAROS_CPU_TCB_IRQ_S10_OFFSET (a2)
    ld      s11 , PHAROS_CPU_TCB_IRQ_S11_OFFSET (a2)
    ld      t4  , PHAROS_CPU_TCB_IRQ_T4_OFFSET  (a2)
    ld      t5  , PHAROS_CPU_TCB_IRQ_T5_OFFSET  (a2)
    ld      t6  , PHAROS_CPU_TCB_IRQ_T6_OFFSET  (a2)

    /* get MEPC and MSTATUS */
    ld      s2  , PHAROS_CPU_TCB_IRQ_MEPC_OFFSET    (a2)
    ld      s3  , PHAROS_CPU_TCB_IRQ_MSTATUS_OFFSET (a2)

    /* restore mstatus */
    csrw    mstatus , s3 
 
    /* restore mepc */
    csrw    mepc, s2

    /* restore the stack pointer */
    ld      sp  , PHAROS_CPU_TCB_IRQ_SP_OFFSET (a2)

    /* restore the remaining registers */
    ld      a0  , PHAROS_CPU_TCB_IRQ_A0_OFFSET (a2)
    ld      t0  , PHAROS_CPU_TCB_IRQ_T0_OFFSET (a2)
    ld      t1  , PHAROS_CPU_TCB_IRQ_T1_OFFSET (a2)
    ld      t2  , PHAROS_CPU_TCB_IRQ_T2_OFFSET (a2)
    ld      t3  , PHAROS_CPU_TCB_IRQ_T3_OFFSET (a2)
    ld      s2  , PHAROS_CPU_TCB_IRQ_S2_OFFSET (a2)
    ld      s3  , PHAROS_CPU_TCB_IRQ_S3_OFFSET (a2)

    /* restore a2 for last */
    ld      a2  , PHAROS_CPU_TCB_IRQ_A2_OFFSET  (a2)

leavingInterrupt:
    /* at this point the MEPC, MSTATUS and all registers and stack content is the same as the begginning of the interrupt */

    /* return from interrupt */
    mret

    /* in case of a nested interrupt we don't have to switch stacks (we already running on the interrupt stack) */
nestedInterrupt:

    /***************************************************/
    /* Save all used registers to the interrupt stack  */
    /***************************************************/

    /* decrement the stack to save space for all registers (already saved a0-a2 and to save) */
    addi    sp , sp, -(18 * 8)

    /* a0 - a2 have already been saved */

    /* save mepc */
    csrr    a1 , mepc

    /* save mstatus */
    csrr    a2 , mstatus
 
    /* save caller saved registers */
    sd      ra , 0*8 (sp)
    sd      a1 , 1*8 (sp)
    sd      a2 , 2*8 (sp)
    sd      a3 , 3*8 (sp)
    sd      a4 , 4*8 (sp)
    sd      a5 , 5*8 (sp)
    sd      a6 , 6*8 (sp)
    sd      a7 , 7*8 (sp)
    sd      t0 , 8*8 (sp)
    sd      t1 , 9*8 (sp)
    sd      t2 , 10*8 (sp)
    sd      t3 , 11*8 (sp)
    sd      t4 , 12*8 (sp)
    sd      t5 , 13*8 (sp)
    sd      t6 , 14*8 (sp)
   
    /* a0 - a2 were are saved in sp + 15*8 , 16*8 and 17*8 */

    /* if the CPU has floating point registers (double precision floating point) */
#ifdef PHAROS_CPU_FP

    /* save space for the 33 registers (32 FPU plust FCSR) */
    addi    sp , sp, -(33 * 8)

    /* reads the fcsr register to a3 */
    frcsr   a3 
    
    /* save the context of the FPU to the TCB */
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
    sd      a3  , 32*8 (sp)
 
#endif


    /***************************************************/
    /* call the C handler                              */
    /***************************************************/

    /* set the first argument */
    csrr    a0 , mcause
 
    /* call the C handler (a1 has already been initialized) */
    jal     pharosCpuCInterruptHandler

    /********************************************************/
    /* restore all saved registers from the interrupt stack */
    /********************************************************/


    /* if the CPU has floating point registers (double precision floating point) */
#ifdef PHAROS_CPU_FP
    
    /* save the context of the FPU to the TCB */
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
    ld      a3  , 32*8 (sp) 

    /* write a3 to the fcsr */
    fscsr   a3

    /* restore the stack to its original position */
    addi    sp, sp, (33 * 8)

#endif   

    /* restore base registers (including mepc and mstatus to a1 and a2 */
    ld      ra , 0*8 (sp)
    ld      a1 , 1*8 (sp)
    ld      a2 , 2*8 (sp)
    ld      a3 , 3*8 (sp)
    ld      a4 , 4*8 (sp)
    ld      a5 , 5*8 (sp)
    ld      a6 , 6*8 (sp)
    ld      a7 , 7*8 (sp)
    ld      t0 , 8*8 (sp)
    ld      t1 , 9*8 (sp)
    ld      t2 , 10*8 (sp)
    ld      t3 , 11*8 (sp)
    ld      t4 , 12*8 (sp)
    ld      t5 , 13*8 (sp)
    ld      t6 , 14*8 (sp)

    /* restore mstatus (and disable interrupts) */
    csrw    mstatus, a2
 
    /* restore mepc */
    csrw    mepc , a1

    /* restore the stack to its original position */
    addi    sp, sp, (18 * 8)

    /* restore a0  */
    ld      a0 , -1*8 (sp)

    /* restore a1 */
    ld      a1 , -2*8 (sp)

    /* restore a2 */
    ld      a2 , -3*8 (sp)

    /* return */
    mret

.endfunc
