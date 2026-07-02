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


/**
 * @file
 *
 * @brief Boot and system start code in ARM mode.
 */

/* External symbols */
#include <pharos/hal/cpu/asm.h>

.global pharosStart
.global pharosCpuIrqStack
.global fiqStack


.section .textStart
 
/* following code runs on ARM instruction set */
.arm

.global pharosCpuVectorsStart
.global pharosCpuVectorsEnd

pharosStart:
pharosCpuVectorsStart:
    LDR     PC, reset_handler_addr
    LDR     PC, undef_handler_addr
    LDR     PC, swi_handler_addr
    LDR     PC, prefetch_abort_handler_addr
    LDR     PC, data_abort_handler_addr

    /* reserved */
    B       . 

    LDR     PC, irq_handler_addr
    LDR     PC, fiq_handler_addr
 
reset_handler_addr: 
    .word pharosCpuResetHandler

undef_handler_addr: 
    .word pharosCpuUndefHandler

swi_handler_addr: 
    .word pharosCpuSwiHandler

prefetch_abort_handler_addr: 
    .word pharosCpuDataAbortHandler

data_abort_handler_addr: 
    .word pharosCpuDataAbortHandler

irq_handler_addr: 
    .word pharosCpuIsrHandler

fiq_handler_addr: 
    .word pharosCpuFiqHandler


pharosCpuVectorsEnd:
 
.func pharosCpuResetHandler
pharosCpuResetHandler:
    /* set Supervisor stack */
    ldr     %sp, =(pharosVStartupStack + PHAROS_CPU_STARTUP_STACK_SIZE)

    /* copy vector table to address 0 */
    blx     pharosCpuCopyIsrVector

    /* get Program Status Register to r0 */
    mrs     %r0, cpsr

    /* go in IRQ mode and set the IRQ stack pointer */
    mov     %r1, #(ARM_PSR_M_IRQ | ARM_PSR_I | ARM_PSR_F)
    msr     cpsr, %r1
    ldr     %sp, =pharosCpuIrqStack + IRQ_STACK_SIZE

    /* Enter FIQ mode and set up the FIQ stack pointer */
	mov     %r1, #(ARM_PSR_M_FIQ | ARM_PSR_I | ARM_PSR_F)
	msr     cpsr, %r1
	ldr     %sp, =fiqStack + FIQ_STACK_SIZE

    /* Enter ABT mode and set up the ABT stack pointer */
	mov     %r1, #(ARM_PSR_M_ABORT | ARM_PSR_I | ARM_PSR_F)
	msr     cpsr, %r1
	ldr     %sp, =abortStack + ABORT_STACK_SIZE

	/* Enter UND mode and set up the UND stack pointer */
	mov     %r1, #(ARM_PSR_M_UNDEFINED | ARM_PSR_I | ARM_PSR_F)
	msr     cpsr, %r1
	ldr     %sp, =undefinedStack + UNDEFINED_STACK_SIZE

    /* Enter system mode and set up the system stack pointer (also the same as the user mode stack pointer) */
	mov     %r1, #(ARM_PSR_M_SYSTEM | ARM_PSR_I | ARM_PSR_F)
	msr     cpsr, %r1
	ldr     %sp, =systemStack + SYSTEM_STACK_SIZE

    /* clear the I and F bits to to enable IRQs and FIQs */
    bic     %r0, %r0, #(ARM_PSR_I | ARM_PSR_F)

    /* enable IRQs and FIQs and go back to Supervisor mode (original mode) */
    MSR     cpsr, %r0

    /* jump to C code */
    blx     pharosCpuBoot
    blx     pharosCpuShutdown

.endfunc
