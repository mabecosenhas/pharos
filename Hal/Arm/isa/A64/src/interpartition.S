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
#include <pharos/hal/cpu/conf.h>

.type pharosCpuSaveInterPartitionContext,                       %function 

.global pharosCpuSaveInterPartitionContext
.global pharosCpuInterParititionContextExit


.section .text

/**
 * void pharosCpuSaveInterPartitionContext(ptrPharosCpuInterPartitionContext context);
 * x0 = context
 */
.func pharosCpuSaveInterPartitionContext
pharosCpuSaveInterPartitionContext:

    /* save the context of the inter-partition to the context (x0) */

    /* save EL1 sp to x2 */
    mov     x2 , sp

    /* save EL0 sp to x3 */
    mrs     x3 , sp_el0

    /* get the ELR EL1 to x4 */
	mrs		x4 , ELR_EL1

    /* get the SPSR EL1 to x5 */
    mrs		x5 , SPSR_EL1

    /* get the TTBR0_EL1 to x6 */
    mrs     x6 , TTBR0_EL1

    /* store EL1 sp and EL0 sp to thread CPU context */
    stp     x2 , x3 , [x0] , #16
   
    /* store ELR_EL1 and SPSR_EL1 to thread CPU context */
    stp     x4 , x5 , [x0] , #16
  
    /* store the TTBR0_EL1 and x8 registers to thread CPU context */
    stp     x6 , x8 , [x0] , #16

    /* store two registers to thread CPU context */
    stp     x9 , x10 , [x0] , #16

    /* store two registers to thread CPU context */
    stp     x11 , x12 , [x0] , #16

    /* store two registers to thread CPU context */
    stp     x13 , x14 , [x0] , #16

    /* store two registers to thread CPU context */
    stp     x15 , x16 , [x0] , #16

    /* store two registers to thread CPU context */
    stp     x17 , x18 , [x0] , #16

    /* store two registers to thread CPU context */
    stp     x19 , x20 , [x0] , #16

    /* store two registers to thread CPU context */
    stp     x21 , x22 , [x0] , #16

    /* store two registers to thread CPU context */
    stp     x23 , x24 , [x0] , #16

    /* store two registers to thread CPU context */
    stp     x25 , x26 , [x0] , #16

    /* store two registers to thread CPU context */
    stp     x27 , x28 , [x0] , #16

    /* store two registers to thread CPU context */
    stp     x29 , x30 , [x0] , #16

    /* return 0 */
    mov     x0 , xzr

    /* return to the thread */
    ret 

    /* if we get here that means that the inter-partition call was interrupted */
pharosCpuInterParititionContextExit:

    /* get the original lr from the TCB */

    /* get the runnning core to x0 */
    mrs     x0 , mpidr_el1

    /* load the mask to x1 to leave only the running core number */
    mov     x1 , PHAROS_CPU_MPIDR_AFFINITY_MASK

    /* clear all but the running core */
    bic     x0 , x0 , x1

    /* get the index on the array of the running core */
    lsl     x0 , x0 , PHAROS_CPU_CORE_RMP_SIZE_LOG2

    /* get the address of the array of running threads */
	ldr     x1 , =pharosVRmpCore

    /* calculate the final address where the running thread for the running core is on */
    add     x0 , x0 , x1

    /* get the running thread */
    ldr     x0 , [x0]

    /* restore the original x30 */
    ldr     x30 , [x0 , OFFSET_X30_IPC]

    /* return TRUE */
    mov     x0 , TRUE

    /* return to the thread */
    ret

.endfunc
