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

.type pharosCpuPspGet,                  %function
.type pharosCpuPspSet,                  %function
.type pharosCpuControlRemovePriv,       %function
.type pharosCpuControlAddPriv,          %function
.type pharosCpuControlIsUnpriv,         %function
.type pharosCpuArmv7mSyscall,           %function
.type pharosCpuArmv7mSyscallLastCall,   %function
.type pharosCpuArmv7mDispatch,          %function
.type pharosCpuIpsrGet,                 %function
.type pharosCpuControlGet,              %function
.type pharosCpuPrimaskEnable,           %function
.type pharosCpuIdleJobBody,             %function

.global pharosCpuPspGet
.global pharosCpuPspSet
.global pharosCpuControlRemovePriv
.global pharosCpuControlAddPriv
.global pharosCpuControlIsUnpriv
.global pharosCpuArmv7mDispatch
.global pharosCpuArmv7mSyscallLastCall
.global pharosCpuArmv7mSyscall
.global pharosCpuIpsrGet
.global pharosCpuControlGet
.global pharosCpuPrimaskEnable
.global pharosCpuIdleJobBody

.syntax unified
.text
.thumb

.func pharosCpuPrimaskEnable
pharosCpuPrimaskEnable:

    /* move to %r0 the value to enable interrupts on the primask */
    mov     %r0 , #0

    /* enable interrupts on the primask */
    msr     PRIMASK , %r0
    
    /* return from function */
	bx      %lr

.endfunc 


.func pharosCpuPspGet
pharosCpuPspGet:

    /* get the psp to %r0 */
    mrs     %r0 , PSP

    /* return from function */
	bx      %lr

.endfunc 


.func pharosCpuPspSet
pharosCpuPspSet:

    /* set the psp from %r0 */
    msr     PSP , %r0

    /* return from function */
	bx      %lr

.endfunc 


.func pharosCpuControlAddPriv
pharosCpuControlAddPriv:

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

    /* clear the priviledge bit (0 means that the thread mode will have priviledges) */
    bic     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* write the control register again */
    msr     CONTROL , %r0

    /* return from function */
	bx      %lr

.endfunc 


.func pharosCpuControlRemovePriv
pharosCpuControlRemovePriv:

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

    /* set the priviledge bit (1 means that the thread mode will have not priviledges) */
    orr     %r0 , %r0, #CONTROL_PRIV_NON_PRIV

    /* write the control register again */
    msr     CONTROL , %r0

    /* return from function */
	bx      %lr

.endfunc 


.func pharosCpuControlIsUnpriv
pharosCpuControlIsUnpriv:

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

    /* clear the priviledge bit (0 means that the thread mode will have priviledges) */
    and     %r0 , %r0, #CONTROL_PRIV_BIT

    /* return from function */
	bx      %lr

.endfunc 


.func pharosCpuArmv7mDispatch
pharosCpuArmv7mDispatch:

    /* when we reach here we are running in Thread mode, inside the kernel stack and with privileges */

    /* dispatch to another thread */
    blx     pharosIDispatchThread

    /* call the syscall to return from a syscall (basically to restore the previous stack) */
    swi     0xFE

.endfunc 


.func pharosCpuArmv7mSyscall
pharosCpuArmv7mSyscall:

    /* when we reach here we are running in Thread mode, inside the kernel stack and with privileges */

    /* don't change r0-r3 and don't change the stack so as to preserve the arguments to the syscall */

    /* call the function with exactly the same arguments (r0-r3 and stack) */
    blx     %r12

    /* call the syscall to return from a syscall */
    swi     0xFF

.endfunc 


.func pharosCpuIpsrGet
pharosCpuIpsrGet:

    /* read to r0 the IPSR register */
    mrs     %r0 , IPSR

    /* return from function */
	bx      %lr

.endfunc 


.func pharosCpuControlGet
pharosCpuControlGet:

    /* read to r0 the control register */
    mrs     %r0 , CONTROL

    /* return from function */
	bx      %lr
.endfunc


.func pharosCpuIdleJobBody
pharosCpuIdleJobBody:

    /* wait for interrupt */
    wfe

     /* return from function */
	bx      %lr

.endfunc
