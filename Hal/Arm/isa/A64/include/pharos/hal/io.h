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


#include <pharos/hal/hal.h>
#include <pharos/hal/declarations.h>


/**
 * Make sure this call is packed to 4 bytes 
 */
#pragma pack(push ,4)

struct IpCallPreamble
{
    /* move the x1 to x0 (input) */
    uint32_t mov_x0_x1;

    /* move the x2 to x1 (output) */
    uint32_t mov_x1_x2;

    /* r2 has the call number to execute */
    uint32_t adr_x2_8;

    /* r3 will have the partition number */
    uint32_t adr_x3_24;

    uint32_t ldr_w2_x2;

    uint32_t ldr_w3_x3;

    /* invoke the syscall */
    uint32_t swi_OS_CUSTOM_SYSCALL;

    uint32_t ret;

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

    /* use syscall to return to user mode and uses the following address as the lr once the syscall ends */
    uint32_t swi_return_direct_io;

    /* return address once this piece of code is executed */
    uint8_t * returnAddress;
};


/**
 * restore the previous pack definition
 */
#pragma pack(pop)


/**
 * Initializes the IO memory access
 * 
 * @param ioAccess io access to initialize
 */
void pharosArmIoAccessInitialize(ptrThreadIoMemAccess ioAccess);

/* PHAROS_IO_H */
#endif 
