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

.type pharosCpuInterruptDisable ,       %function
.type pharosCpuInterruptEnable ,        %function
.type pharosCpuInterruptForceEnable ,   %function
.type pharosCpuInterruptForceDisable,   %function


.global pharosCpuInterruptDisable
.global pharosCpuInterruptEnable
.global pharosCpuInterruptForceEnable
.global pharosCpuInterruptForceDisable


.func pharosCpuInterruptForceEnable
pharosCpuInterruptForceEnable:

    /* enable interrupts */
    csrs    mstatus , 1 << PHAROS_CPU_MSTATUS_MIE_BIT

    /* return from function */
    ret

.endfunc


.func pharosCpuInterruptDisable
pharosCpuInterruptDisable:

    /* read the mstatus to a0 and clear the bit 3 */
    csrrc   a0 , mstatus , 1 << PHAROS_CPU_MSTATUS_MIE_BIT

    /* leave the function */
    ret

.endfunc


.func pharosCpuInterruptEnable
pharosCpuInterruptEnable:

    /* write a0 to mstatus */
    csrrw   a0 , mstatus , a0

    /* leave the function */
    ret

.endfunc

 
.func pharosCpuInterruptForceDisable
pharosCpuInterruptForceDisable:

    /* clear the bit 3 of the mstatus */
    csrrc   a0 , mstatus , 1 << PHAROS_CPU_MSTATUS_MIE_BIT

    /* leave the function */
    ret

.endfunc
