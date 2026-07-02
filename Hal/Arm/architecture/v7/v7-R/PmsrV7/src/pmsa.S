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


.type pharosCpuPmsaSctlrSet ,       %function
.type pharosCpuPmsaSctlrGet ,       %function
.type pharosCpuPmsaRgnrSet ,        %function
.type pharosCpuPmsaDfarGet ,        %function
.type pharosCpuPmsaDrsrSet ,        %function
.type pharosCpuPmsaDrsrSet ,        %function
.type pharosCpuPmsaDracrSet ,       %function
.type pharosCpuPmsaDracrGet ,       %function
.type pharosCpuPmsaDrbarSet ,       %function
.type pharosCpuPmsaDrbarGet ,       %function
.type pharosCpuPmsaDrsrGet ,        %function 
.type pharosCpuDsb ,                %function 
.type pharosCpuIsb ,                %function 
.type pharosCpuDmb,                 %function

.global pharosCpuDmb
.global pharosCpuDsb
.global pharosCpuIsb
.global pharosCpuPmsaSctlrSet
.global pharosCpuPmsaSctlrGet
.global pharosCpuPmsaRgnrSet
.global pharosCpuPmsaDfarGet
.global pharosCpuPmsaDrsrSet
.global pharosCpuPmsaDrsrSet
.global pharosCpuPmsaDracrSet
.global pharosCpuPmsaDracrGet
.global pharosCpuPmsaDrbarSet
.global pharosCpuPmsaDrbarGet
.global pharosCpuPmsaDrsrGet
 
/* following code runs on ARM instruction set */
.arm
.syntax unified
.text


.func pharosCpuDmb
pharosCpuDmb:

    /* perform the DMB instruction */
    dmb

    /* leave and restore ARM/Thumb mode */
    bx      lr

.endfunc


.func pharosCpuIsb
pharosCpuIsb:

    /* perform the ISB instruction */
    isb

    /* leave and restore ARM/Thumb mode */
    bx      lr

.endfunc


.func pharosCpuDsb
pharosCpuDsb:

    /* perform the DSB instruction */
    dsb

    /* leave and restore ARM/Thumb mode */
    bx      lr

.endfunc


/* uint32_t pharosCpuPmsaSctlrGet() */
.func pharosCpuPmsaSctlrGet
pharosCpuPmsaSctlrGet:

    /* read the PMSA register  */
    mrc   p15, #0, r0, c1, c0, #0
    
    /* return from function */
    bx      lr

.endfunc


/* void pharosCpuPmsaSctlrSet(uint32_t) */
.func pharosCpuPmsaSctlrSet
pharosCpuPmsaSctlrSet:

    /* write to the PMSA register */
    mcr   p15, #0, r0, c1, c0, #0

    /* return from function */
    bx      lr

.endfunc


/* uint32_t pharosCpuPmsaRgnrSet() */
.func pharosCpuPmsaRgnrSet
pharosCpuPmsaRgnrSet:

    /* set the PMSA RGNR register  */
    mcr     p15, #0, r0, c6, c2, #0
    
    /* return from function */
    bx      lr

.endfunc


/* uint32_t pharosCpuPmsaDfarGet() */
.func pharosCpuPmsaDfarGet
pharosCpuPmsaDfarGet:

    /* read the PMSA DFAR register  */
    mrc     p15, #0, r0, c6, c0, #0
    
    /* return from function */
    bx      lr

.endfunc


/* void pharosCpuPmsaDrsrSet(uint32_t) */
.func pharosCpuPmsaDrsrSet
pharosCpuPmsaDrsrSet:

    /* write to the PMSA DRSR register */
    mcr     p15, #0, r0, c6, c1, #2

    /* return from function */
    bx      lr

.endfunc


/* uint32_t pharosCpuPmsaDrsrGet() */
.func pharosCpuPmsaDrsrGet
pharosCpuPmsaDrsrGet:

    /* read the PMSA DRSR register  */
    mrc     p15, #0, r0, c6, c1, #2
    
    /* return from function */
    bx      lr

.endfunc


/* void pharosCpuPmsaDracrSet(uint32_t) */
.func pharosCpuPmsaDracrSet
pharosCpuPmsaDracrSet:

    /* write to the PMSA DRACR register */
    mcr     p15, #0, r0, c6, c1, #4

    /* return from function */
    bx      lr

.endfunc


/* uint32_t pharosCpuPmsaDracrGet() */
.func pharosCpuPmsaDracrGet
pharosCpuPmsaDracrGet:

    /* read the PMSA DRACR register  */
    mrc     p15, #0, r0, c6, c1, #4
    
    /* return from function */
    bx      lr

.endfunc


/* void pharosCpuPmsaDrbarSet(uint32_t) */
.func pharosCpuPmsaDrbarSet
pharosCpuPmsaDrbarSet:

    /* write to the PMSA DRACR register */
    mcr     p15, #0, r0, c6, c1, #0

    /* return from function */
    bx      lr

.endfunc


/* void pharosCpuPmsaDrbarGet(uint32_t) */
.func pharosCpuPmsaDrbarGet
pharosCpuPmsaDrbarGet:

    /* write to the PMSA DRACR register */
    mrc     p15, #0, r0, c6, c1, #0

    /* return from function */
    bx      lr

.endfunc
