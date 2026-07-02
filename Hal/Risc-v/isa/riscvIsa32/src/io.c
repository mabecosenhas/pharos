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
#include <pharos/kernel/partition/declarations.h>
#include <pharos/kernel/partition/ipcall.h>
#include <pharos/kernel/core/syscall.h>
#include <pharos/hal/cpu/cache.h>
#include <pharos/kernel/thread/thread.h>
#include <pharos/kernel/assert/assert.h>



#define MV_A0_A1            (0x852e)
#define MV_A1_A2            (0x85b2)

#define NOP                 (0x0001U)

#define ECALL               (0x00000073U)

#define LI_GP(x)            (0x00000193U + ((x) << 20))

#define RET                 (0x8082U)

#define LI_A2(x)            (0x00000613U + ((x) << 20))
#define LI_A3(x)            (0x00000693U + ((x) << 20))

#define DIFF(x , base)      ((x) - (base))
#define BIT11(x , base)     ((DIFF(x , base) >> 11) & 0x1U)
#define BIT10_1(x , base)   ((DIFF(x , base) >> 1 ) & 0x3ffU)
#define BIT19_12(x , base)  ((DIFF(x , base) >> 12) & 0xffU)
#define BIT20(x , base)     ((DIFF(x , base) >> 20) & 0x1U)

#define JUMP(x , base)      ((0x06fU) + (BIT19_12(x,base) << 12) + (BIT11(x,base)<<20) + (BIT10_1(x,base) << 21) + (BIT20(x,base) << 31) )

#define AUIPC_BASE(x,base)  (((DIFF(x,base) >> 12) + 1) << 12)

#define AUIPC(x , base)     ((0x717U) + AUIPC_BASE(x,base))

#define JR(x , base)        ((0x70067U) + (((x - AUIPC_BASE(x,base) - (x & 0xfffU)) & 0xfffU)<<20))


/**
 * Create the opcodes for the AUIPC and JR instructions
 * 
 * @param dest destination address that we want to go to 
 * @param pc current PC address
 * @param auipc address where to place the AUIPC opcode
 * @param jr address where to place the JR opcode
 */
static void opcodeAuipcAndJr(uintptr_t dest , uintptr_t pc , uint32_t *auipc , uint32_t *jr);


void opcodeAuipcAndJr(uintptr_t dest , uintptr_t pc , uint32_t *auipc , uint32_t *jr)
{
    intptr_t d = dest;
    intptr_t p = pc;
    uint32_t pSmall = p & 0xfffU;
    intptr_t diff = d - p;
    int32_t auipcImm = (diff >> 12);

    /* if the distance is bigger than the 2 KiB (jalr instruction can only jump at maximum +2 KiB to -2 KiB */
    if((dest - pc) - (auipcImm << 12) > VALUE_2KiB)
    {
        /* have to increase the value in the auipc by one, which will decrement the value of the jalr */
        auipcImm++;
    }

    /* calculate the address that we would jump if only used auipc instruction (if jr had a 0 immediate offset) */
    uint32_t auipcImmAddress = (auipcImm << 12);

    /* place the opcode of the auipc instruction */
    *auipc = (0x717U) + auipcImmAddress;

    /* calculate the offset between the destination and the value placed on the auipc (account also for the 12 lower bits of the pc) */
    int32_t aux = d - (auipcImmAddress + pSmall);

    /* place the opcode of the jr instruction */
    *jr = (0x70067U) + ((aux & 0xfffU) << 20);
}


bool pharosCpuInstructionIs16(uint32_t opcode)
{
    /* check if the first 2 bits are set to 1, if so then it is not an opcode of 16 bits */
    return (opcode & 0x3) == 0x3 ? FALSE : TRUE;
}


void pharosCpuExecuteMemAddress(ptrThreadCpuContext context , uint8_t *interruptedInstruction)
{
    /* convert the instruction to a 32 bit address */
    uint32_t *instruction = (uint32_t *) interruptedInstruction;

    uint32_t opcode = *instruction;

    /* get the IO access */
    ptrThreadIoMemAccess access = &context->ioAccess;

    if(pharosCpuInstructionIs16(opcode) == TRUE)
    {
        /* get the first instruction */
        uint16_t opcode16 = opcode & 0xffff;

        /* copy the 16-bit opcode and add a nop operation */
        access->originalInstruction = opcode16 | (NOP << 16);

        /* should return to the instruction following the interrupted instruction */
        access->returnAddress = interruptedInstruction + 2U;

    }
    else
    {
        /* copy the 32-bit opcode */
        access->originalInstruction = opcode;

        /* should return to the instruction following the interrupted instruction */
        access->returnAddress = interruptedInstruction + 4U;
    }

    /* load a7 register with the restore tlb syscall */
    access->li_gp_SYSCALL_NUMBER = LI_GP(PHAROS_SYSCALL_RESTORE_TLB << 3);

    /* perform the syscall instruction */
    access->ecall = ECALL;

    /* invalidate the instruction cache since we just changed some instructions */
    pharosCpuInvalidateICache();
}


void pharosCpuIpCallPreambleInit(const ptrIpCall call , ptrIpCallMinimum minimum , uint32_t callNumber , PartitionNumber partition)
{
    uint8_t * aux = (void *) pharosSIpCall;

    /* get the preamble */
    ptrIpCallPreamble preamble = &minimum->preamble;
    intptr_t base = (intptr_t) & preamble->auipcIpScall;
    intptr_t dest = (intptr_t) aux;

    /* init the preamble */

    /* move the a1 to a0 (input) */
    preamble->mov_a0_a1 = MV_A0_A1;

    /* move the a2 to a1 (output) */
    preamble->mov_a1_a2 = MV_A1_A2;

    /* set the a2 with the address of the function number to execute */
    preamble->li_a2_call_number = LI_A2(callNumber);

    /* set the x3 with the address of the partition number of the function to execute */
    preamble->li_a3_partition_number = LI_A3(partition);

    /* load a7 register with the IPC syscall */
    preamble->li_gp_SYSCALL_NUMBER = LI_GP(PHAROS_SYSCALL_IP_CALL << 3);

    /* call the syscall */
    preamble->ecall = ECALL;

    /* create the opcodes for AUIPC and JR */
    opcodeAuipcAndJr(dest , base , &preamble->auipcIpScall , &preamble->jIpSCall);

    /* set the call number */
    preamble->call_number = callNumber;

    /* set the partition number of the call */
    preamble->partition_number = partition;

    /* set the preamble */
    minimum->ipcFunctionPointer = preamble;
}
