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


.type pharosArmDisableInterrupts,       %function
.type pharosArmEnableInterrupts,        %function
.type pharosCpuInterruptForceEnable,    %function 
.type pharosCpuInterruptForceDisable,   %function

.global pharosArmDisableInterrupts
.global pharosArmEnableInterrupts
.global pharosCpuInterruptForceEnable
.global pharosCpuInterruptForceDisable


.text


/**
 * uint64_t pharosArmDisableInterrupts(void);
 */
.func pharosArmDisableInterrupts
pharosArmDisableInterrupts:

    /* read DAIF */
    mrs     x0 , DAIF

    /* disable interrupts (IRQ and FIQ) */
    msr     DAIFSet , #0b0011

    /* make sure memory instructions have been completed */
    dsb     sy

    /* make sure previous instructions have been completed */
    isb     sy

    /* return */
    ret

.endfunc


/**
 * void pharosArmEnableInterrupts(uint32_t level);
 */
.func pharosArmEnableInterrupts
pharosArmEnableInterrupts:

    /* restore previous value */
    msr     DAIF , x0

    /* make sure memory instructions have been completed */
    dsb     sy

    /* make sure previous instructions have been completed */
    isb     sy
    
    /* return */
    ret

.endfunc


.func pharosCpuInterruptForceEnable
pharosCpuInterruptForceEnable:

    /* enable interrupts (IRQ + FIQ) */
    msr     DAIFClr , #0b0011
    
    /* make sure memory instructions have been completed */
    dsb     sy

    /* make sure previous instructions have been completed */
    isb     sy

    /* return */
    ret

.endfunc


/**
 * void pharosCpuInterruptForceDisable(void);
 */
.func pharosCpuInterruptForceDisable
pharosCpuInterruptForceDisable:

    /* disable interrupts (IRQ and FIQ) */
    msr     DAIFSet , #0b0011

    /* make sure memory instructions have been completed */
    dsb     sy

    /* make sure previous instructions have been completed */
    isb     sy

    /* return */
    ret

.endfunc    
