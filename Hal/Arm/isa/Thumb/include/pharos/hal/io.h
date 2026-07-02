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


/* 
 * File:   io.h
 *
 * Created on September 25, 2017
 */

#ifndef PHAROS_ARM_IO_H
#define PHAROS_ARM_IO_H


#include <pharos/hal/hal.h>
#include <pharos/hal/declarations.h>



/**
 * Make sure this call is packed to 1 byte 
 */
#pragma pack(push ,1)

struct IpCallPreamble
{
    /* alignment required for Thumb instructions */
    uint8_t alignment;

    /* move the r1 to r0 (input) */
    uint16_t mov_r0_r1;

    /* move the r2 to r1 (output) */
    uint16_t mov_r1_r2;

    /* r2 has the call number to execute */
    uint32_t mov_r2_pc_plus;

    /* r3 will have the partition number */
    uint32_t mov_r3_pc_plus;

    /* save r0 */
    uint16_t push_r0;

    /* read the IPSR to r0 */
    uint32_t mrs_r0_ipsr;

    /* check if priv mode */
    uint16_t cmp_r0_0;

    /* jump to non priv mode */
    uint16_t bne_priv_mode;

    /* reads the control register to r0 */
    uint32_t mrs_r0_control;

    /* clear all the bits except for the privilege */
    uint32_t and_r0_r0_priv_bit;

    /* check if we are in the privileged mode */
    uint16_t cmp_r0_non_priv;

    /* if we are in user mode then call the syscall */
    uint16_t beq_user_mode;

    /* we are in priv mode here */

    /* restore r0 */
    uint16_t pop_r0;

    /* call the pharosSIpCall directly  */
    uint32_t ldr_pc_pc_plus;

    /* when we return, we return directly to the caller */

    /* we are in user mode here */

    /* restore r0 */
    uint16_t pop2_r0;

    /* invoke the syscall */
    uint16_t swi_OS_CUSTOM_SYSCALL;

    /* leave the function */
    uint16_t bx_lr;

    /* place where the call number is specified */
    uint32_t call_number;

    /* place where the partition number is specified */
    uint32_t partition_number;

    /* address of pharosSIpCall function */
    uint32_t pharosSIpCallAddress;
};

/**
 * Self-modifying code in the Kernel area that executes a direct I/O access.
 * 
 * When this code is executed, the CPU is in privileged mode, so has access to execute the instruction as if the MPU was disabled.
 * We then remove the privileges because doing this here has plenty of restrictions regarding
 * changing the TLB, waiting until the TLB has been changed and switching to user mode (must make sure the TLB has been changed to allow interrupts)
 * but once the TLB has been changed we cannot execute any more instructions inside this memory area since this memory area is inside the kernel.
 */
struct ThreadIoMemAccess
{
    /* at this point all registers are equal */

    /* issue the original instruction */
    uint32_t originalInstruction;

    /* call the function to return from direct I/O (remove privileges and restore the pc to next instruction) */
    uint16_t swi_return_direct_io;

    /* return address once this piece of code is executed */
    uint32_t returnAddress;
};

/**
 * restore the pragma definition
 */
#pragma pack(pop)


/**
 * Initializes the IO memory access
 * 
 * @param ioAccess io access to initialize
 */
void pharosArmIoAccessInitialize(ptrThreadIoMemAccess ioAccess);


/**
 * Determines if the opcode is a 16-bit or 32-bit instruction
 * 
 * @param opcode opcode to determine 
 * 
 * @return returns TRUE if the opcode is coded into 16 bits and FALSE if 32 bits
 */
bool pharosCpuThumbIs16(uint32_t opcode);


#endif /* IO_H */
