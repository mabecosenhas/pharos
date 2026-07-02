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
#include <pharos/hal/def.h>


.type pharosCpuSaveInterPartitionContextLow,                       %function 

.global pharosCpuSaveInterPartitionContextLow
.global pharosCpuInterParititionContextExit

.syntax unified
.text


/**
 * bool pharosCpuSaveInterPartitionContext(ptrPharosCpuInterPartitionContext context);
 * r0 = context

 */
.func pharosCpuSaveInterPartitionContextLow
pharosCpuSaveInterPartitionContextLow:

    /* save the context of the inter-partition to the thread */

    /* save r4 to r12 and lr to the context */
    stmia   %r0! , {%r4 - %r12 , %lr}

    /* save PSP to r1 */
    mov     %r1 , %sp
 
    /* save PSR to r2 */
    mrs     %r2 , xPSR

    /* save control to r3 */
    mrs     %r3 , CONTROL

    /* store these registers in the thread TCB */
    stmia   %r0! , {%r1 - %r3}

    /* save floating point register to r1 */
    vmrs    %r1 , FPSCR

    /* save the PRIMASK */
    mrs     %r2 , PRIMASK

    /* store these registers in the thread TCB */
    stmia   %r0! , {%r1 - %r2}
    
    /* return FALSE */
    mov     %r0 , #FALSE

    /* return from function */
    bx      %lr

    /* if we get here that means that the inter-partition call was interrupted */
pharosCpuInterParititionContextExit:

    /* get the original lr from the TCB */

    /* load to %r0 the pointer to the address of the running thread */
    ldr     %r0, =pharosVRunningThread

    /* r0 = address of the running thread CPU context */
    ldr     %r0 , [%r0]
    
    /* get the return address */
    ldr     %lr , [%r0 , #OFFSET_LR_IPC]

    /* return TRUE */
    mov     %r0 , #TRUE

    /* return from function */
    bx      %lr

.endfunc
