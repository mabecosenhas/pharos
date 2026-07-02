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

.type pharosCpuSaveInterPartitionContext,                       %function 

.global pharosCpuSaveInterPartitionContext
.global pharosCpuInterParititionContextExit


.section .text

/**
 * void pharosCpuSaveInterPartitionContext(ptrPharosCpuInterPartitionContext context);
 * r0 = context
 */
.func pharosCpuSaveInterPartitionContext
pharosCpuSaveInterPartitionContext:

    /* save the context of the inter-partition to the thread */
 
    /* save CPSR register to r2 */
	mrs     %r2, cpsr

    /* save TLB to r3 */
    mrc     p15 , 0 , %r3 , c2 , c0 , 0

    /* switch to system mode (with the user mode registers) to save the user mode sp and lr */
    msr     cpsr_c , #(CPSR_SYSTEM_MODE| CPSR_DISABLE_IRQ)

    /* save all registers with the exception of r0-r3 and r12 which are defined by ARM to be temporary (note that r2 contains the CPSR and r3 the TLB) */
	stmia	r0!,  {r2, r3 , r4, r5, r6, r7, r8, r9, r10, r11, r13, r14}

    /* switch to supervisor mode to save the supervisor mode sp and lr */
    msr     cpsr_c , #(CPSR_SUPERVISOR_MODE| CPSR_DISABLE_IRQ)

    /* save supervisor sp and lr */
    stmia	r0!,  {r13, r14}

    /* return FALSE */
    mov     %r0 , #0

    /* return from function */
    bx      %lr

    /* if we get here that means that the inter-partition call was interrupted */
pharosCpuInterParititionContextExit:

    /* get the original lr from the TCB */

    /* load to %r0 the pointer to the address of the running thread */
    ldr     %r0, =pharosVRunningThread

    /* r0 = address of the running thread CPU context */
    ldr     %r0 , [%r0]
    
    /* get the running thread saved return address */
    ldr     %lr , [%r0 , #OFFSET_LR_IPC]

    /* return TRUE */
    mov     %r0 , #1

    /* return from function */
    bx      %lr

.endfunc
