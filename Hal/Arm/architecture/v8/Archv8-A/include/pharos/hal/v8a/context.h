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


#ifndef PHAROS_V8A_CONTEXT_H
#define PHAROS_V8A_CONTEXT_H

#include <pharos/hal/hal.h>
#include <pharos/hal/io.h>
#include <pharos/hal/memory/mmu.h>
#include <pharos/hal/aarch64.h>


/**
 * Make sure this context is packed to 4 bytes 
 */
#pragma pack(push ,4)

struct ThreadCpuBaseContext
{
    uint64_t sp_el1;
    uint64_t sp_el0;
    uint64_t elr_l1;
    uint64_t spsr_l1;

    ptrPharosCpuMmuLevel0Table tlb_el0;
    /* local registers */
    uint64_t x8;
    uint64_t x9;
    uint64_t x10;

    uint64_t x11;
    uint64_t x12;
    uint64_t x13;
    uint64_t x14;

    uint64_t x15;
    uint64_t x16;
    uint64_t x17;
    uint64_t x18;

    uint64_t x19;
    uint64_t x20;
    uint64_t x21;
    uint64_t x22;

    uint64_t x23;
    uint64_t x24;
    uint64_t x25;
    uint64_t x26;

    uint64_t x27;
    uint64_t x28;
    uint64_t x29;
    uint64_t x30;
};

struct ThreadCpuContext
{
    /*********************************************************************/
    /* context of the thread when calling a thread dispatch              */
    /*********************************************************************/
    ThreadCpuBaseContext baseContext;


    /* floating point context */
    uint128_t q[32];

    /*********************************************************************/
    /* IRQ frame                                                         */
    /*********************************************************************/
    uint64_t x2_irq;
    uint64_t x3_irq;
    uint64_t x4_irq;
    uint64_t x5_irq;

    uint64_t x6_irq;
    uint64_t x7_irq;
    uint64_t x8_irq;
    uint64_t x9_irq;

    uint64_t x10_irq;
    uint64_t x11_irq;
    uint64_t x12_irq;
    uint64_t x13_irq;

    uint64_t x14_irq;
    uint64_t x15_irq;
    uint64_t x29_irq;
    uint64_t x30_irq;

    uint64_t SPSR_EL1_irq;
    uint64_t ELR_EL1_irq;
    uint64_t x0_irq;
    uint64_t x1_irq;

    uint64_t sp_irq;
    uint64_t sp_el0_irq;

    /* floating point context */
    uint128_t q_irq[32];

    /****************************************************************************/
    /* Entry point of the thread (copy here to use when executing in user mode) */
    /****************************************************************************/

    uintptr_t entryPoint;

    uint64_t x30Ipc;

    /* place where the original ELR is saved to */
    uintptr_t directIoSaveElr;

    /*********************************************************************/
    /* I/O Context                                                       */
    /*********************************************************************/

    ThreadIoMemAccess ioAccess;
};


/**
 * restore the previous pack definition
 */
#pragma pack(pop)


/**
 * Perform the assembly DSB instruction
 */
void pharosCpuDsb(void);


/**
 * Perform the assembly ISB instruction
 */
void pharosCpuIsb(void);


/**
 * Execute the idle thread (enables interrupts before jumping to idle thread body)
 */
void pharosCpuExecuteUserModeIdle(void);


/**
 * Get the current stack pointer value
 * 
 * @return returns the current stack pointer value
 */
uint8_t *pharosCpuSpGet(void);


/* PHAROS_V8A_CONTEXT_H */
#endif 
