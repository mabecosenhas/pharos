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

.type pharosCpuStart,                           %function 

.global pharosCpuStart

.syntax unified

.section .text
.thumb


.thumb_func

.func pharosCpuStart
pharosCpuStart:

    /* once the system starts, it is in the Thread mode with priveledges and with the Main stack */

    /* the main stack is initialized with the initVector initialStackPointer (pharosVStartupStack)
    
    /* initialize registers */
    mov     %r0 , #0
    mov     %r1 , #0
    mov     %r2 , #0
    mov     %r3 , #0
    mov     %r4 , #0
    mov     %r5 , #0
    mov     %r6 , #0
    mov     %r7 , #0
    mov     %r8 , #0
    mov     %r9 , #0
    mov     %r10 , #0
    mov     %r11 , #0
    mov     %r12 , #0

    /* set the return address of the start hook to the next address plus 1 (thumb mode) */
	ldr     %lr, =pharosStartThumb + 1

    /* jump to the hook, when it returns, it will be in thumb mode */
	b       startHook

pharosStartThumb:
    /* read the control register to r2 */
    mrs     %r2, control

    /* set the bit for the process stack */
    orr     %r2, #0x2

    /* write back to the control register */
    msr     control, %r2

    /* make sure the control is done */
    isb

    /* initialize the sp register in process stack */
	ldr     %sp, =(pharosVStartupStack + PHAROS_CPU_STARTUP_STACK_SIZE)

    /* jump to C code */
    blx     pharosCpuBoot

    /* if somehow CPU boot returns, just call the shutdown */
    blx     pharosCpuShutdown

    /* internal function */
startHook:
    bx      %lr

.endfunc
