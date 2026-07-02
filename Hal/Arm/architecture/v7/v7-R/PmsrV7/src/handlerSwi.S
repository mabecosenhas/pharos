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

.type pharosSwiHandler,  %function

.global pharosSwiHandler


.syntax unified
.text

.arm


.func pharosSwiHandler
pharosSwiHandler:

    /* at this point can use r0 has been saved to stack and we can use it */

    /*********************************************************************/
    /* get SWI number callled in r0                                      */
    /*********************************************************************/
    ldr     %r0, [%lr,#-4]

    /* clear the bits 24-31 to get the swi called (e.g. SWI 0x50 will give 0x50) */
    bic     r0, r0, #0xff000000

    /* check if it is the special case of just restoring the TLB */
    cmp     %r0 , PHAROS_SYSCALL_RESTORE_TLB

    /* it is, go fastly to the restore MMU */
    beq     userModeSyscallRestoreTlb

    /* in user mode API syscall at least r0 and r4-r12 have been saved previously */

    /*********************************************************************/
    /* switch to kernel TLB to enable full access                        */
    /*********************************************************************/

    /* if the syscall is not for a API call */
    cmp     %r0 , PHAROS_SYSCALL_API_LAST_CALL

    /* just call the corresponding function (if r0 > PHAROS_SYSCALL_API_LAST_CALL) */
    bhi     userModeInternalSyscall


userModeApiSyscall:

    /*********************************************************************/
    /* save registers to the supervisor stack                            */
    /*********************************************************************/

    /* save the SPSR to r4 */
    mrs     %r4 , SPSR

    /* so we can save the SPSR and supervisor mode lr */
    push   {%r4 , %lr}

    /* at this point all registers of the caller (thread or interrupt) have been saved to the thread supervisor stack except for lr and sp on the previous mode */
    
    /* switch back to supervisor mode */
    msr     cpsr_c , #(CPSR_SUPERVISOR_MODE | CPSR_DISABLE_IRQ)

    /**
     *  at this point all registers of the caller have been saved to the supervisor stack with the following format: 
     *
     *  supervisor mode lr 
     *  SPRC                       <- highest address
     */

    /*********************************************************************/
    /* get SWI number callled in r7                                      */
    /*********************************************************************/
   
    /* multiply by 4 the register (each address has 4 bytes) */
    lsl     r7 , r0,  2

    
    /*********************************************************************/
    /* restore the arguments of the syscall (to r0-r3 and kernel stack)  */
    /*********************************************************************/
restoreArgumentsUser:
  
    /* r1-r3 have not been touched in this method so they are the same */
    
    /* switch to system mode to get the user mode sp */
    msr     cpsr_c , #(CPSR_SYSTEM_MODE | CPSR_DISABLE_IRQ)

    /* get the original value of r0 */
    ldr     r0 , [sp]

    /* save to a local register the user mode stack */
    mov     %r4 , %sp

    /* switch to system mode to get the user mode sp */
    msr     cpsr_c , #(CPSR_SUPERVISOR_MODE | CPSR_DISABLE_IRQ)


    /* load the vector of stack size for each syscall */
    ldr     %r5 , =pharosVCpuSyscallStackSize

    /* point to the correct address inside the vector (r7 contains the index of the syscall) */
    add     %r5 , %r5 , %r7

    /* load the value */
    ldr     %r5 , [%r5]
    
    /* if the number of bytes is 0 */
    cmp     %r5 , 0

    /* we have already finished copying the stack (no stack to copy since everything is inside r0-r3 */
    beq     _pharosFinishedRestoringArguments
    
    /* in this CPU there is only one situation: copy 8 or 12 bytes */
    cmp     %r5 , 12

    beq     _pharosRestore12Arguments

_pharosRestore8Arguments:
    /* So load the 8 bytes from the stack */
    ldr     %r6 , [%r4 ,#(12*4)]

    ldr     %r4 , [%r4 ,#(11*4)]

    /* and place them on the supervisor stack */
    push    {%r4 , %r6}

    b       _pharosFinishedRestoringArguments

_pharosRestore12Arguments:
    
    /* So load the 12 bytes from the stack */
    ldr     %r10 , [%r4 ,#(13*4)]

    /* So load the 8 bytes from the stack */
    ldr     %r6 , [%r4 ,#(12*4)]

    ldr     %r4 , [%r4 ,#(11*4)]

    /* and place them on the supervisor stack */
    push    {%r4 , %r6 , %r10}

_pharosFinishedRestoringArguments:

    /* store in r6 the vector array of the method to call */
    ldr     %r6 , =pharosVSystemCalls
    
    /* increase the pointer to the vector index being called */
    add     %r6 , %r6 , %r7

    /* r6 now contains the address of the function to be called */
    ldr     %r6 , [%r6]

    /*********************************************************************/
    /* call the syscall                                                  */
    /*********************************************************************/

    /* enable interrupts */
    msr     cpsr_c , #(CPSR_SUPERVISOR_MODE)
  
    /* call the syscall! */
    blx     %r6

    /* restore the supervisor stack to its original place (taking out the stack that was copied) */
    add     %sp , %sp , %r5

    
_pharosFinishedRestoringStack:
    /* disable interrupts */
    msr     cpsr_c , #(CPSR_SUPERVISOR_MODE | CPSR_DISABLE_IRQ)

    /* at this point r0 and at most r1 have the return value, we must not touch them */
  
    /************************************************************************/
    /* restore registers context of the running thread from the syscall TCB */
    /************************************************************************/
       
    /* get the original supervisor mode SPCR (to r5), supervisor mode lr (to r6) */
    pop     {%r3 , %lr}

    /* restore the SPCR in supervisor mode */
    msr     spsr, %r3

    /* leave syscall interrupt routine using the supervisor mode lr and SPSR to determine the next instruction and mode */
    movs    pc,lr

    /***************************************************************************/
    /* Internal syscall made inside user mode (e.g. leaving a resource filter) */
    /***************************************************************************/

userModeInternalSyscall:

    /* in user mode API internal syscall at least r0 and r4-r12 have been saved previously */

   
    /* r0 has at this point the syscall number to perform */

    /* multiply by 4 the register (each address has 4 bytes) */
    lsl     r0 , 2

    /* save the lr register */
    push    {%r8 , %lr}

    /* save the SPSR to r1 (so we can enable interrupts since the SPSR is NOT saved in context switches nor interrupts)
     * and use as argument to the syscall */
    mrs     %r1 , SPSR

    /* enable interrupts */
    msr     cpsr_c , #(CPSR_SUPERVISOR_MODE)

    /* store in r6 the vector array of the method to call */
    ldr     %r6 , =pharosVSystemCalls
    
    /* increase the pointer to the vector index being called */
    add     %r6 , %r6 , %r0

    /* r6 now contains the address of the function to be called */
    ldr     %r6 , [%r6]

    /* set the argument with the address that originated this call (r1 already has the SPSR) */
    mov     %r0 , %lr

    /* call the syscall! This syscall must obey the prototype CpuModeChangeContext (*handler)(uint8_t *address , CpuModeChangeContext context) */
    blx     %r6

    /* disable interrupts */
    msr     cpsr_c , #(CPSR_SUPERVISOR_MODE | CPSR_DISABLE_IRQ)   

    /* restore the lr register */
    pop     {%r8 , %lr}
      
    /* place on the SPSR mode the result of the syscall (r0) */
    msr     SPSR , %r0

    /* leave syscall interrupt routine using the supervisor mode lr and SPSR to determine the next instruction and mode */
    movs    pc,lr



    /*********************************************************************/
    /* special case of restoring the TLB                                 */
    /*********************************************************************/
userModeSyscallRestoreTlb:

    /* save the original value of %r1 */
    push    {%r1}
    
    /* switch to system mode to get the user mode sp */
    msr     cpsr_c , #(CPSR_SYSTEM_MODE | CPSR_DISABLE_IRQ)
    
    /* get to r1 the return address once this syscall ends */
    pop     {%r1}

    /* get the original value of r0 */
    pop     {%r0}

    /* at this point r1 has the return address and *sp_supervisor has the original r1 value */

    /* switch back to supervisor mode to get the supervisor mode sp */
    msr     cpsr_c , #(CPSR_SUPERVISOR_MODE | CPSR_DISABLE_IRQ)

    /* swap the return address (in r1) with the value on the stack (original r1 value) */
    /* this way the stack gets the return address and r1 gets its original value */

    /* save r0 */
    push    {%r0}

    /* place in r0 the original r1 value */
    ldr     %r0 , [%sp , #4]

    /* store in *sp the return address (when this syscall ends) */
    str     %r1 , [%sp , #4]

    /* move to r1 its original value */
    mov     %r1 , %r0

    /* now read the current SPSR */
    mrs     %r0 , SPSR

    /* clear the mode */
    and     %r0 , %r0 , #(CPSR_CPU_MODE_MASK)

    /* switch to system mode */
    add     %r0 , %r0 , #(CPSR_USER_MODE)

    /* save the SPSR so when the abort returns, it will return to System mode (instead of user mode) */
    msr     SPSR , %r0

    /* restore the original r0 */
    pop     {%r0}

    /* leave syscall interrupt routine using the supervisor mode lr and SPSR to determine the next instruction and mode */
    ldmfd    sp! , {pc}^

.endfunc
