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


/* External symbols */
#include <pharos/hal/cpu/asm.h>

.type pharosCpuContextSwitchLow,                %function
.type pharosCpuRestoreHeirLow ,          %function
.type pharosCpuSwitchSupervisorMode ,           %function

.global pharosCpuContextSwitchLow
.global pharosCpuRestoreHeirLow
.global pharosCpuSwitchSupervisorMode

.syntax unified

.thumb


.thumb_func

.func pharosCpuContextSwitchLow
pharosCpuContextSwitchLow:

    /* save r4 to r12 and lr to the thread TCB */
    stmia   %r0! , {%r4 - %r12 , %lr}

    /* save PSP to r2 */
    mov     %r2 , %sp
 
    /* save PSR to r3 */
    mrs     %r3 , xPSR

    /* save control to r4 */
    mrs     %r4 , CONTROL

    /* save floating point register to r4 */
    vmrs    %r5 , FPSCR

    /* save the PRIMASK */
    mrs     %r6 , PRIMASK

    /* store these registers in the thread TCB */
    stmia   %r0! , {%r2 - %r6}

    /* save the floating point registers to the thread TCB */
    vstm    %r0 , {%d0 - %d15}

    /* finished saving the context, lets start to restore the heir thread context */

    /* restore the r4-r12 and lr registers */
    ldm     %r1! , {%r4 - %r12 , %lr}

    /* get the PSP, PSR and CONTROL registers */
    ldm     %r1! , {%r0 , %r2 , %r3}

    /* restore the PSP */
    mov     %sp , %r0 

    /* restore the PSR */
    msr     xPSR , %r2

    /* restore the control register */
    msr     CONTROL , %r3

    /* get FPSCR and PRIMASK register from TCB */
    ldm     %r1! , {%r0 , %r2}

    /* restore the FPSCR */
    vmsr    FPSCR , %r0 

    /* reload the primask register */
    msr     PRIMASK , %r2

    /* restore the floating point registers */
    vldm    %r1 , {%d0 - %d15}

    /* return to caller */
    bx      %lr

.endfunc


.func pharosCpuRestoreHeirLow
pharosCpuRestoreHeirLow:
    /**************************************************************/
    /* Restore registers                                          */
    /**************************************************************/
     
    /* restore all the registers with the exception of r0-r3 (note that r2 and r3 will contain the previously saved CPSR and TLB) */
	ldmia	%r0!,  {%r4 - %r12, %lr}
  
    /* get the PSP, PSR and FPSCR registers */
    ldm     %r0! , {%r1 , %r2 , %r3}

    /* restore the PSP */
    msr     PSP , %r1 

    /* restore the PSR */
    msr     xPSR , %r2

    /* restore the floating point register */
    vmsr    FPSCR , %r3

    /* restore the floating point registers */
    vldm    %r0 , {%d0-%d15}

    /* enable interrupts */
    mov     %r0 , #0

    /* restore the primask to enable interrupts to the level set in r0 */
    msr     PRIMASK, %r0

    /* return to caller */
    bx      %lr

.endfunc


/**
 * Switch the CPU mode to supervisor mode when returning from an interrupt using (switches the SPSR), for example, movs
 *
 * CpuModeChangeContext pharosCpuSwitchSupervisorMode(CpuModeChangeContext context);
 *
 * CpuModeChangeContext is the control register on this ARM architecture (V7-M)
 */
.func pharosCpuSwitchSupervisorMode
pharosCpuSwitchSupervisorMode:

    /* set the priviledge mode on the control register (clear the first bit) */
    bic     %r0 , %r0 , #(1 << CONTROL_PRIV_BIT)

    /* leave and restore ARM/Thumb mode */
    bx      %lr

.endfunc
