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
 * a0 = context
 */
.func pharosCpuSaveInterPartitionContext
pharosCpuSaveInterPartitionContext:

    /* save the context of the inter-partition to the thread */
 
    /* read the current SATP */
    csrr    a2 , satp

    sd      ra  , PHAROS_CPU_CONTEXT_RA_OFFSET  (a0)
    sd      sp  , PHAROS_CPU_CONTEXT_SP_OFFSET  (a0)
    sd      t0  , PHAROS_CPU_CONTEXT_T0_OFFSET  (a0)
    sd      t1  , PHAROS_CPU_CONTEXT_T1_OFFSET  (a0)
    sd      t2  , PHAROS_CPU_CONTEXT_T2_OFFSET  (a0)
    sd      t3  , PHAROS_CPU_CONTEXT_T3_OFFSET  (a0)
    sd      t4  , PHAROS_CPU_CONTEXT_T4_OFFSET  (a0)
    sd      t5  , PHAROS_CPU_CONTEXT_T5_OFFSET  (a0)
    sd      t6  , PHAROS_CPU_CONTEXT_T6_OFFSET  (a0)
    sd      s0  , PHAROS_CPU_CONTEXT_S0_OFFSET  (a0)
    sd      s1  , PHAROS_CPU_CONTEXT_S1_OFFSET  (a0)
    sd      s2  , PHAROS_CPU_CONTEXT_S2_OFFSET  (a0)
    sd      s3  , PHAROS_CPU_CONTEXT_S3_OFFSET  (a0)
    sd      s4  , PHAROS_CPU_CONTEXT_S4_OFFSET  (a0)
    sd      s5  , PHAROS_CPU_CONTEXT_S5_OFFSET  (a0)
    sd      s6  , PHAROS_CPU_CONTEXT_S6_OFFSET  (a0)
    sd      s7  , PHAROS_CPU_CONTEXT_S7_OFFSET  (a0)
    sd      s8  , PHAROS_CPU_CONTEXT_S8_OFFSET  (a0)
    sd      s9  , PHAROS_CPU_CONTEXT_S9_OFFSET  (a0)
    sd      s10 , PHAROS_CPU_CONTEXT_S10_OFFSET (a0)
    sd      s11 , PHAROS_CPU_CONTEXT_S11_OFFSET (a0)
    
    /* store the current satp register */
    sd      a2 , PHAROS_CPU_CONTEXT_SATP_OFFSET (a0)

    /* read mstatus to t0  */
    csrr    t0 , mstatus

    /* store mstatus */
    sd      t0 , PHAROS_CPU_CONTEXT_MSTATUS_OFFSET (a0)

    /* return FALSE */
    li      a0 , FALSE

    /* return from function */
    ret 

    /* if we get here that means that the inter-partition call was interrupted */
pharosCpuInterParititionContextExit:

    /* get the original lr from the TCB */

    /* load the running hart */
    csrr    a0 , mhartid

    /* get the offset of the core on the pharosVRmpCore table */
    slli    a0 , a0 , PHAROS_CPU_CORE_RMP_SIZE_LOG2

    /* get the address of the array of running threads */
	la      a2 , pharosVRmpCore

    /* calculate the final address where the running thread for the running core is on */
    add     a2 , a2 , a0

    /* get the running thread */
    ld      a0 , (a2)
    
    /* get the return address */
    ld      ra , PHAROS_CPU_CONTEXT_RA_IPC (a0)

    /* return TRUE */
    li      a0 , TRUE

    /* return from function */
    ret 

.endfunc
