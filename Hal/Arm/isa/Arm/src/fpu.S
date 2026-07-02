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

#ifdef PHAROS_CPU_HAS_VFP

.type pharosCpuFpuInitialize,               %function
.type pharosCpuFpuGetFpexc,                 %function
.type pharosCpuFpuGetFpscr,                 %function
.type pharosCpuFpuGetFpinst,                %function
.type pharosCpuFpuGetFpinst2,               %function



.global pharosCpuFpuInitialize
.global pharosCpuFpuGetFpexc
.global pharosCpuFpuGetFpscr
.global pharosCpuFpuGetFpinst
.global pharosCpuFpuGetFpinst2

.syntax unified
.text

.arm
.func pharosCpuFpuInitialize
pharosCpuFpuInitialize:

    
    /* read the current value of the FPEXC register */
    vmrs    %r0 , FPEXC
    
    /* modify it to include the enable bit */
    orr     %r0 , %r0 , #FPEXC_ENABLE_MASK

    /* set it with the modified bit */
    vmsr    FPEXC , %r0

    /* leave and restore ARM/Thumb mode */
    bx      %lr

.endfunc

.arm
.func pharosCpuFpuGetFpexc
pharosCpuFpuGetFpexc:

    /* read the current value of the FPEXC register */
    vmrs    %r0 , FPEXC
    
    /* leave and restore ARM/Thumb mode */
    bx      %lr

.endfunc

.arm
.func pharosCpuFpuGetFpscr
pharosCpuFpuGetFpscr:

    /* read the current value of the FPSCR register */
    vmrs    %r0 , FPSCR
    
    /* leave and restore ARM/Thumb mode */
    bx      %lr

.endfunc

.arm
.func pharosCpuFpuGetFpinst
pharosCpuFpuGetFpinst:

    /* read the current value of the FPINT register */
    vmrs    %r0 , FPINST
    
    /* leave and restore ARM/Thumb mode */
    bx      %lr

.endfunc


.arm
.func pharosCpuFpuGetFpinst2
pharosCpuFpuGetFpinst2:

    /* read the current value of the FPINST2 register */
    vmrs    %r0 , FPINST2
    
    /* leave and restore ARM/Thumb mode */
    bx      %lr

.endfunc

#endif
