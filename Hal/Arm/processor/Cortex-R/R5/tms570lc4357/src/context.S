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

.type pharosCpuContextSwitchLow,            %function
.type pharosCpuRestoreHeirLow,              %function
.type pharosCpuSwitchUserMode,              %function
.type pharosCpuSwitchSupervisorMode,        %function



.global pharosCpuContextSwitchLow
.global pharosCpuRestoreHeirLow
.global pharosCpuSwitchUserMode
.global pharosCpuSwitchSupervisorMode

.syntax unified
.text

.arm




.func pharosCpuContextSwitchLow
pharosCpuContextSwitchLow:

    /*********************************************************************/
    /* Saving context                                                    */
    /*********************************************************************/

    /* save CPSR register to r2 */
	mrs     %r2, cpsr

       /* switch to system mode (with the user mode registers) to save the user mode sp and lr */
    msr     cpsr_c , #(CPSR_SYSTEM_MODE| CPSR_DISABLE_IRQ)

    /* save all registers with the exception of r0-r3 and r12 which are defined by ARM to be temporary (note that r2 contains the CPSR and r3 the TLB) */
	stmia	r0!,  {r2, r4, r5, r6, r7, r8, r9, r10, r11, r13, r14}

    /* switch to supervisor mode to save the supervisor mode sp and lr */
    msr     cpsr_c , #(CPSR_SUPERVISOR_MODE| CPSR_DISABLE_IRQ)

    /* save supervisor sp and lr */
    stmia	r0!,  {r13, r14}

    /* only compile this code if FPU is supported */
#ifdef PHAROS_CPU_HAS_VFP
    /* save floating point registers */
    vstm    r0! , {s0 - s31}   

    /* save the FPSCR to r4 (already saved and will be overriden afterwards) */
    vmrs    %r4 , FPEXC

    /* save the thread FPSCR */
    vmrs    %r5 , FPSCR

    /* store the FPSCR to the thread TCB */
    stmia	r0!,  {r4 , %r5}
#endif

    /*********************************************************************/
    /* Restoring context                                                 */
    /*********************************************************************/

    /* switch to system mode (with the user mode registers) to restore the user mode sp and lr */
    msr     cpsr_c , #(CPSR_SYSTEM_MODE| CPSR_DISABLE_IRQ)

    /* make r2 point to the floating point registers of the heir thread */
    add     %r2 , %r1 , #OFFSET_CONTEXT_SWITCH_S0

    /* only compile this code if FPU is supported */
#ifdef PHAROS_CPU_HAS_VFP

    /* restore the floating point registers of the heir thread */
    vldm    r2! , {s0 - s31}  

    /* get the special FPU registers */
    ldmia	r2!, {%r4 , %r5}

    /* restore the thread FPEXC */
    vmsr    FPEXC, %r4

    /* restore the thread FPSCR */
    vmsr    FPSCR, %r5

#endif

    /* restore the registers r2-r11. r0-r3 are arguments and r12 is not needed in this method (used for inter-procedure calls) */
	ldmia	r1!,  {r2, r4, r5, r6, r7, r8, r9, r10, r11 , r13 , r14}

    /* switch to supervisor mode to get the supervisor mode sp and lr */
    msr     cpsr_c , #(CPSR_SUPERVISOR_MODE| CPSR_DISABLE_IRQ)

    /* now can restore the sp (r13) and lr (r14) register in the supervisor mode */
    ldmia	r1!,  {r13, r14}

     /* switch the cpsr to the heir thread CPU mode */
    msr     cpsr, %r2
   
    /* leave and restore ARM/Thumb mode */
    bx      lr

.endfunc


.arm
.func pharosCpuRestoreHeirLow
pharosCpuRestoreHeirLow:

    /**************************************************************/
    /* Restore registers                                          */
    /**************************************************************/
 
    /* switch to system mode to set the r13 (sp) and r14 (lr) in user mode */
    /* (system and user mode shared the same registers while in system mode we maintain the privileges) */
    msr     cpsr_c , #(CPSR_SYSTEM_MODE| CPSR_DISABLE_IRQ | CPSR_DISABLE_FIQ)
    
    /* restore all the registers with the exception of r0-r3 (note that r2 and r3 will contain the previously saved CPSR and TLB) */
	ldmia	r0!,  {r2, r4, r5, r6, r7, r8, r9, r10, r11, r13, r14}
  
    /* switch to supervisor mode to get the supervisor mode sp and lr */
    msr     cpsr_c , #(CPSR_SUPERVISOR_MODE| CPSR_DISABLE_IRQ | CPSR_DISABLE_FIQ)

    /* now can restore the sp (r13) and lr (r14) register in the supervisor mode */
    ldmia	r0!,  {r13, r14}

#ifdef PHAROS_CPU_HAS_VFP
    /**************************************************************/
    /* Restore FPU                                                */
    /**************************************************************/
 
    /* restore the floating point registers */
    vldm    r0! , {s0 - s31}  

    /* get the FPSCR to r1 (r1 not being used anymore) */
    ldmia	r0!,  {r1}

    /* restore the thread FPSCR */
    vmrs    %r1 , FPEXC 

    /* get the FPSCR to r1 (r1 not being used anymore) */
    ldmia	r0!,  {%r1}

    /* restore the thread FPSCR */
    vmrs    %r1 , FPSCR
#endif

    /**************************************************************/
    /* Restore CPSR                                               */
    /**************************************************************/
 
    /* restore the CPSR of the new thread (re-enables interrupts here) */
	msr     cpsr, %r2

    /* leave and restore ARM/Thumb mode */
    bx      %lr

.endfunc


/**
 * Switch the CPU mode to supervisor mode when returning from an interrupt using (switches the SPSR), for example, movs
 *
 * CpuModeChangeContext pharosCpuSwitchSupervisorMode(CpuModeChangeContext context);
 */
.arm
.func pharosCpuSwitchSupervisorMode
pharosCpuSwitchSupervisorMode:

    /* clear the mode from the SPSR */
    and     %r0 , %r0 , #(CPSR_CPU_MODE_MASK)

    /* place the new supervisor mode */
    orr     %r0 , %r0 , #(CPSR_SUPERVISOR_MODE)

    /* save the SPSR with the supervisor mode */
    msr     SPSR , %r0

    /* leave and restore ARM/Thumb mode */
    bx      lr

.endfunc
