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

.type pharosArmDisableInterrupts,       %function
.type pharosArmEnableInterrupts,        %function
.type pharosCpuTempEnableInterrupts ,   %function
.type pharosCpuInterruptForceEnable,    %function
.type pharosCpuInterruptForceDisable,    %function

.global pharosArmDisableInterrupts
.global pharosArmEnableInterrupts
.global pharosCpuTempEnableInterrupts
.global pharosCpuInterruptForceEnable
.global pharosCpuInterruptForceDisable


.syntax unified
.text
.arm


.func pharosArmDisableInterrupts
pharosArmDisableInterrupts:

    /* get current CPSR and save it to the return value */
    mrs     %r0, cpsr

    /* disable interrupts on current CPSR */
    orr     %r1, %r0, #CPSR_DISABLE_IRQ

    /* place original CPSR but with interrupts disabled */
    msr     cpsr_c, %r1

    /* return */
    bx      %lr

.endfunc    


.func pharosArmEnableInterrupts
pharosArmEnableInterrupts:

    /* restore the CPSR given in argument */
    msr     cpsr_c, %r0

    /* return */
    bx      %lr
.endfunc


.func pharosCpuTempEnableInterrupts
pharosCpuTempEnableInterrupts:

    /* get current CPSR */
    mrs     %r1, cpsr

    /* place original CPSR but with interrupts disabled */
    msr     cpsr_c, %r0

    /* restore the CPSR */
    msr     cpsr_c, %r1

    /* return */
    bx      %lr
.endfunc


.func pharosCpuInterruptForceEnable
pharosCpuInterruptForceEnable:

    /* get current CPSR */
    mrs     %r0, cpsr

    /* enable interrupts on current CPSR */
    bic     %r1, %r0, #CPSR_DISABLE_IRQ

    /* place original CPSR but with interrupts disabled */
    msr     cpsr_c, %r1

    /* return */
    bx      %lr

.endfunc


.func pharosCpuInterruptForceDisable
pharosCpuInterruptForceDisable:

    /* get current CPSR and save it to the return value */
    mrs     %r0, cpsr

    /* disable interrupts on current CPSR */
    orr     %r1, %r0, #CPSR_DISABLE_IRQ

    /* place original CPSR but with interrupts disabled */
    msr     cpsr_c, %r1

    /* return */
    bx      %lr

.endfunc
