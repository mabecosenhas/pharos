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


#include <pharos/hal/io.h>
#include <pharos/hal/cpu/context.h>
#include <pharos/hal/arm.h>
#include <pharos/kernel/partition/declarations.h>
#include <pharos/kernel/partition/ipcall.h>
#include <pharos/kernel/core/syscall.h>


#define ARM_MOV_R0_R1               (0xe1a00001U)
#define ARM_MOV_R1_R2               (0xe1a01002U)
#define ARM_PUSH_R0                 (0xe52d0004U)
#define ARM_POP_R0                  (0xe49d0004U)
#define ARM_LDR_R0_PC_PLUS(X)       (0xe59f0000U + (X))
#define ARM_LDR_R2_PC_PLUS(X)       (0xe59f2000U + (X))
#define ARM_LDR_R3_PC_PLUS(X)       (0xe59f3000U + (X))
#define ARM_SWI(X)                  (0xef000000U + (X))
#define ARM_BX_LR                   (0xe12fff1eU)
#define ARM_PUSH_R0_R4_R12_LR       (0xe92d5ff1U)
#define ARM_POP_R2_R4_R12_LR        (0xe8bd5ff4U)
#define ARM_MRS_R0_CPSR             (0xe10f0000U)
#define ARM_AND_R0_R0_VAL(x)        (0xe2000000U + (x))
#define ARM_CMP_R0_VAL(x)           (0xe3500000U + (x))
#define ARM_BEQ_VAL(x)              (0x0a000000U + (x))
#define ARM_LDR_PC_PC_PLUS(x)       (0xe59ff000U + (x))


void pharosCpuExecuteMemAddress(ptrThreadCpuContext context , uint8_t *interruptedInstruction)
{
    /* convert the instruction to a 32 bit address */
    uint32_t *instruction = (uint32_t *) interruptedInstruction;

    /* get the IO access */
    ptrThreadIoMemAccess access = &context->ioAccess;


    /* copy the original instruction */
    access->originalInstruction = *instruction;

    /* should return to the instruction following the interrupted instruction */
    access->returnAddress = (uint32_t) interruptedInstruction + 4U;

    /* invalidate the instruction cache */
    pharosCpuInvalidateICache();
}


void pharosCpuIpCallPreambleInit(const ptrIpCall call , ptrIpCallMinimum minimum , uint32_t callNumber , PartitionNumber partition)
{
    /* get the preamble */
    ptrIpCallPreamble preamble = &minimum->preamble;


    /* init the preamble */

    /* mov the r1 register to r0 */
    preamble->mov_r0_r1 = ARM_MOV_R0_R1;

    /* mov the r2 register to r1 */
    preamble->mov_r1_r2 = ARM_MOV_R1_R2;

    /* set the r2 with the syscall number to execute */
    preamble->mov_r2_pc_plus = ARM_LDR_R2_PC_PLUS((uint8_t *) & preamble->call_number - (uint8_t *) & preamble->mov_r2_pc_plus - 8U);

    /* set the r3 with the partition number */
    preamble->mov_r3_pc_plus = ARM_LDR_R3_PC_PLUS((uint8_t *) & preamble->partition_number - (uint8_t *) & preamble->mov_r3_pc_plus - 8U);

    /* save r0 to stack */
    preamble->push_r0 = ARM_PUSH_R0;

    /* get the CPSR to r0 */
    preamble->mrs_r0_cpsr = ARM_MRS_R0_CPSR;

    /* clear the mode from the CPSR */
    preamble->and_r0_r0_01f = ARM_AND_R0_R0_VAL(ARM_PSR_M_MASK);

    /* compare the mode with the user mode */
    preamble->cmp_r0_user_mode = ARM_CMP_R0_VAL(CPSR_USER_MODE);

    /* restore r0 to its original value */
    preamble->pop_r0 = ARM_POP_R0;

    /* if in user mode then jump the next instruction */
    preamble->beq_user_mode = ARM_BEQ_VAL(&preamble->push_r0_r4_r12_lr - &preamble->beq_user_mode - 2U);

    /* if not in user mode then jump directly to the IPC pharosSIpCall method */
    preamble->ldr_pc_pc_plus = ARM_LDR_PC_PC_PLUS((uint8_t *) & preamble->pharosSIpCallAddress - (uint8_t *) & preamble->ldr_pc_pc_plus - 8U);

    /* first push the registers to be used (r0, r4, r12 and lr) */
    preamble->push_r0_r4_r12_lr = ARM_PUSH_R0_R4_R12_LR;

    /* call the syscall */
    preamble->swi_OS_CUSTOM_SYSCALL = ARM_SWI(PHAROS_SYSCALL_IP_CALL);

    /* restore the registers used (r2 - instead of r0 since r0 has the return value, r4, r12 and lr) */
    preamble->pop_r2_r4_2_r12_lr = ARM_POP_R2_R4_R12_LR;

    /* leave the IPC call */
    preamble->bx_lr = ARM_BX_LR;

    /* set the call number */
    preamble->call_number = callNumber;

    /* set the partition number of the call */
    preamble->partition_number = partition;

    preamble->pharosSIpCallAddress = (uint32_t) pharosSIpCall;

    /* set the preamble */
    minimum->ipcFunctionPointer = preamble;
}


void pharosArmIoAccessInitialize(ptrThreadIoMemAccess ioAccess)
{
    /* set the "push {r0} instruction */
    ioAccess->push1_r0 = ARM_PUSH_R0;

    /* set the return ldr r0 , [pc , #4] instruction */
    ioAccess->ldr_r0_returnAddress = ARM_LDR_R0_PC_PLUS(4);

    /* set the push {r0} instruction */
    ioAccess->push2_r0 = ARM_PUSH_R0;

    /* set the swi PHAROS_SYSCALL_RESTORE_TLB instruction */
    ioAccess->swi_return_direct_io = ARM_SWI(PHAROS_SYSCALL_RESTORE_TLB);
}
