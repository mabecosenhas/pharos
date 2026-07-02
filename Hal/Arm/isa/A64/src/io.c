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
#include <pharos/hal/aarch64.h>
#include <pharos/kernel/partition/declarations.h>
#include <pharos/kernel/partition/ipcall.h>
#include <pharos/kernel/core/syscall.h>
#include <pharos/hal/cpu/cache.h>

#define ARM_MOV_X0_X1               (0xaa0103e0U)
#define ARM_MOV_X1_X2               (0xaa0203e1U)

#define ARM_ADR_X2                  (0x100000c2U)
#define ARM_ADR_X3                  (0x100000c3U)
#define ARM_LDR_W2_X2               (0xb9400042U)
#define ARM_LDR_W3_X3               (0xb9400063U)
#define ARM_SVC_IP_CALL             (0xd4000521U)
#define ARM_SVC_RESTORE_TLB         (0xd4000001U)
#define ARM_RET                     (0xd65f03c0U)


void pharosCpuExecuteMemAddress(ptrThreadCpuContext context , uint8_t *interruptedInstruction)
{
    /* convert the instruction to a 32 bit address */
    uint32_t *instruction = (uint32_t *) interruptedInstruction;

    /* get the IO access */
    ptrThreadIoMemAccess access = &context->ioAccess;

    /* get the IO access */
    /* copy the original instruction */
    access->originalInstruction = *instruction;

    /* should return to the instruction following the interrupted instruction */
    access->returnAddress = interruptedInstruction + 4U;

    /* invalidate the instruction cache since we just changed some instructions */
    pharosCpuInvalidateICache();
}


void pharosArmIoAccessInitialize(ptrThreadIoMemAccess ioAccess)
{
    /* set the swi PHAROS_SYSCALL_RESTORE_TLB instruction */
    ioAccess->swi_return_direct_io = ARM_SVC_RESTORE_TLB;
}


void pharosCpuIpCallPreambleInit(const ptrIpCall call , ptrIpCallMinimum minimum , uint32_t callNumber , PartitionNumber partition)
{
    /* get the preamble */
    ptrIpCallPreamble preamble = &minimum->preamble;

    /* init the preamble */

    /* mov x1 to x0 */
    preamble->mov_x0_x1 = ARM_MOV_X0_X1;
    
    /* mov x2 to x1 */
    preamble->mov_x1_x2 = ARM_MOV_X1_X2;

    /* set the x2 with the address of the function number to execute */
    preamble->adr_x2_8 = ARM_ADR_X2;

    /* set the x3 with the address of the partition number of the function to execute */
    preamble->adr_x3_24 = ARM_ADR_X3;

    /* set in w2 the function number to execute */
    preamble->ldr_w2_x2 = ARM_LDR_W2_X2;

    /* set in w3 the function number to execute */
    preamble->ldr_w3_x3 = ARM_LDR_W3_X3;

    /* call the syscall */
    preamble->swi_OS_CUSTOM_SYSCALL = ARM_SVC_IP_CALL;

    /* return of the function */
    preamble->ret = ARM_RET;

    /* set the call number */
    preamble->call_number = callNumber;

    /* set the partition number of the call */
    preamble->partition_number = partition;

    /* set the preamble */
    minimum->ipcFunctionPointer = preamble;
}
