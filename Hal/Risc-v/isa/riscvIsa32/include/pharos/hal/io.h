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


#ifndef PHAROS_IO_H
#define PHAROS_IO_H


#include <pharos/declarations.h>

/**
 * Make sure this call is packed to 4 bytes 
 */
#pragma pack(push ,2)

struct IpCallPreamble
{
    /* move the a1 to a0 (input) */
    uint16_t mov_a0_a1;

    /* move the a2 to a1 (output) */
    uint16_t mov_a1_a2;

    /* a2 has the call number to execute */
    uint32_t li_a2_call_number;

    /* a3 will have the partition number */
    uint32_t li_a3_partition_number;

    /* load gp with the syscall number */
    uint32_t li_gp_SYSCALL_NUMBER;

    /* invoke the syscall */
    uint32_t ecall;

    uint32_t auipcIpScall;

    /* jump to the pharosSIpCall function */
    uint32_t jIpSCall;

    /* place where the call number is specified */
    uint32_t call_number;

    /* place where the partition number is specified */
    uint32_t partition_number;
};

/**
 * Self-modifying code in the Kernel area that executes a direct I/O access.
 * 
 * When this code is executed, the CPU is in USER mode but the TLB has access to every address.
 * We use the syscall to return to user TLB because doing this here has plenty of restrictions regarding
 * changing the TLB, waiting until the TLB has been changed and switching to user mode (must make sure the TLB has been changed to allow interrupts)
 * but once the TLB has been changed we cannot execute any more instructions inside this memory area since this memory area is inside the kernel.
 */
struct ThreadIoMemAccess
{
    /* at this point all registers are equal, the TLB = kernel and the CPU mode is SYSTEM */

    /* issue the original instruction */
    uint32_t originalInstruction;

    /* load gp with the syscall number */
    uint32_t li_gp_SYSCALL_NUMBER;

    /* invoke the syscall */
    uint32_t ecall;

    /* return address once this piece of code is executed */
    uint8_t * returnAddress;
};


/**
 * restore the previous pack definition
 */
#pragma pack(pop)

/* PHAROS_IO_H */
#endif 
