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
#include <pharos/hal/thumb.h>
#include <pharos/kernel/partition/declarations.h>
#include <pharos/kernel/partition/ipcall.h>
#include <pharos/kernel/core/syscall.h>
#include <pharos/kernel/assert/assert.h>


#define ARM_MOV_R0_R1               (0x4608U)
#define ARM_MOV_R1_R2               (0x4611U)
#define ARM_PUSH_R0                 (0xb401U)
#define ARM_LDR_R2_PC_PLUS(x)       (0x2000f8dfU + ((x) << 16))
#define ARM_LDR_R3_PC_PLUS(x)       (0x3000f8dfU + ((x) << 16))
#define ARM_MRS_R0_IPSR             (0x8005f3efU)
#define ARM_CMP_R0(x)               (0x2800 + (x))
#define ARM_BNE_VAL(x)              (0xd100 + (x) - 1)
#define ARM_MRS_R0_CONTROL          (0x8014f3ef)
#define ARM_AND_R0_R0_VAL(x)        (0x0000f000 + ((x) << 16))
#define ARM_CMP_R0_VAL(x)           (0x2800 + (x))
#define ARM_BEQ_VAL                 (0xd002)
#define ARM_POP_R0                  (0xbc01)
#define ARM_LDR_PC_PC_PLUS(x)       (0xf000f8df + ((x) << 16))
#define ARM_SWI(x)                  (0xdf00U + (x))
#define ARM_BX_LR                   (0x4770U)


#define THUMB_32_BIT_COND_1         (0b1110100000000000)
#define THUMB_32_BIT_COND_2         (0b1111000000000000)


bool pharosCpuThumbIs16(uint32_t opcode)
{
    /* return value of the function */
    bool result;

    /* get the first part of the instruction */
    uint16_t firstHalf = (uint16_t) (opcode & 0xffff);


    /* if the bit is set */
    if((firstHalf & THUMB_32_BIT_COND_2) == THUMB_32_BIT_COND_2)
    {
        /* then it is a 32-bit instruction */
        result = FALSE;
    }
        /* if the second bit is set */
    else if((firstHalf & THUMB_32_BIT_COND_1) == THUMB_32_BIT_COND_1)
    {
        /* then it is a 32-bit instruction */
        result = FALSE;
    }
    else
    {
        /* otherwise is a 16-bit instruction */
        result = TRUE;
    }

    /* returns TRUE if a 16-bit instruction and FALSE if a 32-bit instruction */
    return result;
}


void pharosCpuExecuteMemAddress(ptrThreadCpuContext context , uint8_t *interruptedInstruction)
{
    /* convert the instruction to a 32 bit address */
    uint32_t *instruction = (uint32_t *) interruptedInstruction;

    /* opcode as 32-bit thumb instruction */
    uint32_t opcode = * (uint32_t *) instruction;

    /* opcode as 16-bit thumb instruction */
    uint16_t opcode16 = (opcode & 0xffff);

    /* get the IO access */
    ptrThreadIoMemAccess access = &context->ioAccess;


    /* check if the executed instruction is 16-bit or 32-bit instruction */
    if(pharosCpuThumbIs16(opcode) == TRUE)
    {
        /* copy the 16-bit opcode and add a nop operation (0s) */
        access->originalInstruction = opcode16;

        /* should return to the instruction following the interrupted instruction, in thumb mode */
        access->returnAddress = ((uint32_t) interruptedInstruction + 2U) & ~((uint32_t) 1U);
    }
    else
    {
        /* copy the 32-bit opcode */
        access->originalInstruction = opcode;

        /* should return to the instruction following the interrupted instruction, in thumb mode */
        access->returnAddress = ((uint32_t) interruptedInstruction + 4U) & ~((uint32_t) 1U);
    }

    /* invalidate the instruction cache */
    pharosCpuInvalidateICache();
}


void pharosCpuIpCallPreambleInit(const ptrIpCall call , ptrIpCallMinimum minimum , uint32_t callNumber , PartitionNumber partition)
{
    /* get the preamble */
    ptrIpCallPreamble preamble = &minimum->preamble;


    /* set the entry point of the IPC call to the first assembly instruction */
    minimum->ipcFunctionPointer = (void *) (((uint32_t) & preamble->mov_r0_r1) | 1);

    /* update the function pointer to be called in thumb mode */
    call->functionPointer = (void *) (((uint32_t) call->functionPointer) | 1);

    /* init the preamble */

    /* fill the alignment with something */
    preamble->alignment = 0;

    /* move the r1 to r0 (input) */
    preamble->mov_r0_r1 = ARM_MOV_R0_R1;

    /* move the r2 to r1 (output) */
    preamble->mov_r1_r2 = ARM_MOV_R1_R2;

    /* set the r2 with the syscall number to execute */
    preamble->mov_r2_pc_plus = ARM_LDR_R2_PC_PLUS((uint8_t *) & preamble->call_number - (uint8_t *) & preamble->mov_r2_pc_plus - 2U);

    /* set the r3 with the partition number */
    preamble->mov_r3_pc_plus = ARM_LDR_R3_PC_PLUS((uint8_t *) & preamble->partition_number - (uint8_t *) & preamble->mov_r3_pc_plus - 2U);

    /* save r0 to stack */
    preamble->push_r0 = ARM_PUSH_R0;

    /* get the CPSR to r0 */
    preamble->mrs_r0_ipsr = ARM_MRS_R0_IPSR;

    /* check if priv mode */
    preamble->cmp_r0_0 = ARM_CMP_R0(0U);

    /* jump to non priv mode */
    preamble->bne_priv_mode = ARM_BNE_VAL((uint8_t *) & preamble->pop2_r0 - (uint8_t *) & preamble->bne_priv_mode - 8U - 6U);

    /* reads the control register to r0 */
    preamble->mrs_r0_control = ARM_MRS_R0_CONTROL;

    /* clear all the bits except for the privilege */
    preamble->and_r0_r0_priv_bit = ARM_AND_R0_R0_VAL((1 << CONTROL_PRIV_BIT));

    /* check if we are in the privileged mode */
    preamble->cmp_r0_non_priv = ARM_CMP_R0_VAL(CONTROL_PRIV_NON_PRIV);

    /* if we are in user mode then call the syscall */
    preamble->beq_user_mode = ARM_BEQ_VAL;

    /* restore r0 to its original value */
    preamble->pop_r0 = ARM_POP_R0;

    /* if not in user mode then jump directly to the IPC pharosSIpCall method */
    preamble->ldr_pc_pc_plus = ARM_LDR_PC_PC_PLUS((uint8_t *) & preamble->pharosSIpCallAddress - (uint8_t *) & preamble->ldr_pc_pc_plus - 2U);

    /* restore r0 */
    preamble->pop2_r0 = ARM_POP_R0;

    /* call the syscall */
    preamble->swi_OS_CUSTOM_SYSCALL = ARM_SWI(PHAROS_SYSCALL_IP_CALL);

    /* leave the IPC call */
    preamble->bx_lr = ARM_BX_LR;

    /* set the call number */
    preamble->call_number = callNumber;

    /* set the partition number of the call */
    preamble->partition_number = partition;

    /* set the address of the pharosSIpCall function */
    preamble->pharosSIpCallAddress = (uint32_t) pharosSIpCall;

}


void pharosArmIoAccessInitialize(ptrThreadIoMemAccess ioAccess)
{
    /* set the instruction to call the swi to restore the context */
    ioAccess->swi_return_direct_io = ARM_SWI(PHAROS_SYSCALL_RESTORE_TLB);
}
