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

.type pharosCpuDataAbortHandler, %function

.global pharosCpuDataAbortHandler

.syntax unified
.text

.arm
.func pharosCpuDataAbortHandler
pharosCpuDataAbortHandler:

   
    /*********************************************************************/
    /* switch to supervisor mode                                         */
    /*********************************************************************/

    /* switch to supervisor mode since the abort handler function must be working on supervisor mode (may issue syscalls) */
    msr     cpsr_c , #(CPSR_SUPERVISOR_MODE | CPSR_DISABLE_IRQ)

    /* save %r0 - %r12 and %lr_supervisor to the supervisor stack */
    push    {%r0 - %r12 , %lr}

    msr     cpsr_c , #(CPSR_ABORT_MODE | CPSR_DISABLE_IRQ)

    /* get the %lr_abort to %r0 (will be an argument to the abort handler) */
    sub     %r0 , %lr , #8

    /* switch back to supervisor mode */
    msr     cpsr_c , #(CPSR_SUPERVISOR_MODE | CPSR_DISABLE_IRQ)

    /*********************************************************************/
    /* Increment thread dispatch disable level to disable dispatches     */
    /*********************************************************************/
	
	/* Increment interrupt nest and thread dispatch disable level */
	ldr     r1, =pharosVDispatchLevel
	ldr     r3, [r1]
	add     r3, #1
	str     r3, [r1]

	/*************************************************/
    /*    Call BSP dependent interrupt dispatcher    */
	/*************************************************/

    /* get to r1 the memory address that was trying to be accessed (the DFAR register - Data Fault Address Register) */
    mrc     p15, 0, r1, c6, c0, 0

    /* enable interrupts */
    msr     cpsr_c , #(CPSR_SUPERVISOR_MODE)

    /* call the abort handler (%r0 has already been set) */
	blx      armCDataAbortHandler

    /* decrement the thread dispatch level and store it in r3 */
	ldr     r1, =pharosVDispatchLevel
	ldr     r3, [r1]
	sub     r3, #1
	str     r3, [r1]
    
	/* dispatch the thread inside the interrupt (leaving with interrupts disabled) */
	blx     pharosIDispatchThread

    /* restore all registers and stack */
    pop     {%r0 - %r12 , %lr}

    /* will only reach here if the thread that originated the fault is using a direct I/O */

    /***************************************************************/
    /* set the return address (lr) to execute the IO direct memory */
    /***************************************************************/

    /* change back to the abort mode (to use the spsr_ABORT) */
    msr     cpsr_c , #(CPSR_ABORT_MODE | CPSR_DISABLE_IRQ)

    /* save r0 */
    push    {%r0}

    /* load to %r0 the address of the running thread */
    ldr     %r0, =pharosVRunningThread

    /* get the address of the running thread CPU context */
    ldr     %r0 , [%r0]

    /* set the return address to the direct I/O memory access */
    add     %lr , %r0 , #OFFSET_IO_ACCESS

    /* if we are issuing a direct I/O then return with priviledges mode (SYSTEM mode) */
    mrs     %r0 , SPSR

    /* clear the mode */
    and     %r0 , %r0 , #(CPSR_CPU_MODE_MASK)

    /* switch to system mode */
    add     %r0 , %r0 , #(CPSR_SYSTEM_MODE)

    /* save the SPSR so when the abort returns, it will return to System mode (instead of user mode) */
    msr     SPSR , %r0

    /* restore r0 so every register is as it was before the abort was called except for lr_abort which now will be set to execute the direct I/O memory access */
    pop     {%r0}
    
	/* Return from the abort using the same lr */
	subs	pc, lr , #0

.endfunc
