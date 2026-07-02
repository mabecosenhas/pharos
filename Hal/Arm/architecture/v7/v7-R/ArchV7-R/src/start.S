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
    ldr     %pc, reset_handler_addr
    ldr     %pc, undef_handler_addr
    ldr     %pc, swi_handler_addr
    ldr     %pc, prefetch_abort_handler_addr
    ldr     %pc, data_abort_handler_addr

    /* reserved */
    b       . 

    ldr     %pc, irq_handler_addr
    ldr     %pc, fiq_handler_addr
 
reset_handler_addr: 
    .word reset_handler

undef_handler_addr: 
    .word pharosCpuUndefHandler

swi_handler_addr: 
    .word pharosSwiHandler

prefetch_abort_handler_addr: 
    .word pharosCpuDataAbortHandler

data_abort_handler_addr: 
    .word pharosCpuDataAbortHandler

irq_handler_addr: 
    .word pharosCpuIsrHandler

fiq_handler_addr: 
    .word pharosFiqHandler


pharosCpuVectorsEnd:
 
.func reset_handler

reset_handler:
    
    /***********************************************************/
    /* Enter Supervisor mode                                   */
    /***********************************************************/

    /* initialize common registers */
    mov     %r0 , #0
    mov     %r1 , #0
    mov     %r2 , #0
    mov     %r3 , #0
    mov     %r4 , #0
    mov     %r5 , #0
    mov     %r6 , #0
    mov     %r7 , #0

    /* initialize supervisor registers */
    mov     %r8 , #0
    mov     %r9 , #0
    mov     %r10 , #0
    mov     %r11 , #0
    mov     %r12 , #0
    
    /* set supervisor stack */
    ldr     %sp, =(pharosVStartupStack + PHAROS_CPU_STARTUP_STACK_SIZE)

    /* initialize supervisor lr */
    ldr     %lr , =reset_handler

    /* get Program Status Register to r0 (we will come back to supervisor mode using this register) */
    mrs     %r0, cpsr

    /***********************************************************/
    /* Enter IRQ mode                                          */
    /***********************************************************/

    /* go in IRQ mode */
    mov     %r1, #(ARM_PSR_M_IRQ | ARM_PSR_I | ARM_PSR_F)

    /* switch to IRQ mode */
    msr     cpsr, %r1

    /* initialize IRQ lr */
    ldr     %lr , =reset_handler

    /* initialize IRQ stack */
    ldr     %sp , =pharosCpuIrqStack + IRQ_STACK_SIZE

    /***********************************************************/
    /* Enter FIQ mode                                          */
    /***********************************************************/

    /* Enter FIQ mode  */
	mov     %r1, #(ARM_PSR_M_FIQ | ARM_PSR_I | ARM_PSR_F)
	msr     cpsr, %r1

    /* initialize FIQ registers */
    mov     %r8 , #0
    mov     %r9 , #0
    mov     %r10 , #0
    mov     %r11 , #0
    mov     %r12 , #0

    /* initialize FIQ lr */
    ldr     %lr , =reset_handler

    /* initialize FIQ stack */
	ldr     %sp , =fiqStack + FIQ_STACK_SIZE

    /***********************************************************/
    /* Enter Abort mode                                        */
    /***********************************************************/

    /* Enter ABT mode and set up the ABT stack pointer */
	mov     %r1, #(ARM_PSR_M_ABORT | ARM_PSR_I | ARM_PSR_F)
	msr     cpsr, %r1

    /* initialize abort lr */
    ldr     %lr , =reset_handler

    /* initialize abort stack */
	ldr     %sp , =abortStack + ABORT_STACK_SIZE

    /***********************************************************/
    /* Enter Undefined mode                                   */
    /***********************************************************/

	/* Enter UND mode and set up the UND stack pointer */
	mov     %r1, #(ARM_PSR_M_UNDEFINED | ARM_PSR_I | ARM_PSR_F)
	msr     cpsr, %r1

    /* initialize undefined mode lr */
    ldr     %lr , =reset_handler

    /* initialize undefined mode stack */
	ldr     %sp , =undefinedStack + UNDEFINED_STACK_SIZE

    /* Enter supervisor mode and set up the supervisor stack pointer */
	mov     %r1, #(ARM_PSR_M_SUPERVISOR | ARM_PSR_I | ARM_PSR_F)
	msr     cpsr, %r1
	ldr     sp, =supervisorStack + SUPERVISOR_STACK_SIZE

    /***********************************************************/
    /* Enter system mode (shares registers with user mode)     */
    /***********************************************************/

    /* Enter system mode and set up the system stack pointer (also the same as the user mode stack pointer) */
	mov     %r1, #(ARM_PSR_M_SYSTEM | ARM_PSR_I | ARM_PSR_F)
	msr     cpsr, %r1

    /* initialize system mode stack */
	ldr     %sp, =systemStack + SYSTEM_STACK_SIZE

    /* initialize system mode lr */
    ldr     %lr , =reset_handler

    /* go back to Supervisor mode (original mode) */
    msr     cpsr, %r0

#ifdef PHAROS_CPU_HAS_VFP

    /* get the co-processor access control register, CPACR */
    mrc     p15, #0x00, r2, c1, c0, #0x02

    /* enable full access */
    orr     %r2, %r2, #0xF00000

    /* write back to CPACR */
    mcr     p15, #0x00, %r2, c1, c0, #0x02

    /* set the r2 with enable FPU to the FPEXC register */
    mov     %r2, #0x40000000

    /* enable FPU */
    fmxr    fpexc, %r2

    /* reset all FPU registers */
    fmdrr   %d0, %r1, %r1
    fmdrr   %d1, %r1, %r1
    fmdrr   %d2, %r1, %r1
    fmdrr   %d3, %r1, %r1
    fmdrr   %d4, %r1, %r1
    fmdrr   %d5, %r1, %r1
    fmdrr   %d6, %r1, %r1
    fmdrr   %d7, %r1, %r1
    fmdrr   %d8, %r1, %r1
    fmdrr   %d9, %r1, %r1
    fmdrr   %d10, %r1, %r1
    fmdrr   %d11, %r1, %r1
    fmdrr   %d12, %r1, %r1
    fmdrr   %d13, %r1, %r1
    fmdrr   %d14, %r1, %r1
    fmdrr   %d15, %r1, %r1

#endif

        bl    next1
next1:
        bl    next2
next2:
        bl    next3
next3:
        bl    next4
next4:

    /* jump to C code */
    blx     pharosCpuBoot

    /* if somehow CPU boot returns, just call the shutdown */
    blx     pharosCpuShutdown

.endfunc
