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

.type pharosArmEnableInterrupts,                %function 
.type pharosArmDisableInterrupts,               %function 
.type pharosCpuInterruptForceEnable,            %function 
.type pharosCpuInterruptForceDisable,           %function

.global pharosArmEnableInterrupts
.global pharosArmDisableInterrupts
.global pharosCpuInterruptForceEnable
.global pharosCpuInterruptForceDisable


.syntax unified

.section .text
.thumb


.func pharosCpuInterruptForceEnable
pharosCpuInterruptForceEnable:

	/* set the IRQ bit to r0 */
	mov     %r0 , #(0)

    /* clear interrupts by placing 0 in PRIMASK (which will be placed in basepri if basepri > 0x80) */
    msr     PRIMASK , %r0

	/* return to caller with the original PRIMASK in %r0 */
	bx      %lr

.endfunc



.func pharosArmDisableInterrupts
pharosArmDisableInterrupts:

    /* read the primask to %r0 (and return it) */
	mrs 	%r0 , PRIMASK

	/* set the IRQ bit to r1 */
	mov     %r1 , #(1)

    /* clear interrupts by placing 1 in PRIMASK (which will be placed in basepri if basepri > 0x80) */
    msr     PRIMASK , %r1

	/* return to caller with the original PRIMASK in %r0 */
	bx      %lr

.endfunc


.func pharosArmEnableInterrupts
pharosArmEnableInterrupts:

    /* restore the primask to the level set in r0 */
    msr     PRIMASK, %r0

	/* return to caller */
	bx      %lr

.endfunc


.func pharosCpuInterruptForceDisable
pharosCpuInterruptForceDisable:

    /* read the primask to %r0 */
	mrs 	%r0 , PRIMASK

	/* set the IRQ bit to r1 */
	mov     %r1 , #(1)

    /* clear interrupts by placing 1 in PRIMASK (which will be placed in basepri if basepri > 0x80) */
    msr     PRIMASK , %r1

	/* return to caller */
	bx      %lr

.endfunc
