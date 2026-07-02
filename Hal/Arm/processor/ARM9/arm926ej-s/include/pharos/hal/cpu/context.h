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


#ifndef PHAROS_CONTEXT_H
#define PHAROS_CONTEXT_H


#include <pharos/hal/hal.h>

/**
 * Minimum shared stack size 
 */
#define PHAROS_SHARED_STACK_SIZE_MINIMUM_SIZE       VALUE_4KiB


/**
 * Calculate the correct size of the shared stack for dynamically created threads.
 * Must be the next multiple of 4 KiB of the original stack size and not below 4 KiB 
 */
#define PHAROS_SHARED_STACK_CORRECT_SIZE(size)      MAX_MACRO(VALUE_4KiB , VALUE_4KiB * PHAROS_CEILING(size , VALUE_4KiB))

/**
 * Alignment of the shared stack. Must be aligned to an MMU page
 */
#define PHAROS_DYNAMIC_STACK_ALIGN(size)            VALUE_4KiB

/**
 * Make sure this call is packed to 4 bytes 
 */
#pragma pack(push ,4)

struct ThreadCpuBaseContext
{
    uint32_t cpsr;

    /**
     * Address of the current TLB descriptor
     */
    uint32_t tlb;

    uint32_t r4;
    uint32_t r5;
    uint32_t r6;
    uint32_t r7;
    uint32_t r8;
    uint32_t r9;
    uint32_t r10;
    uint32_t r11;

    /**
     * register r13 is the same as the sp
     */
    uint32_t r13_user;

    /**
     * register r14 is the same as the lr
     */
    uint32_t r14_user;

    uint32_t r13_supervisor;

    uint32_t r14_supervisor;
};

struct ThreadCpuContext
{
    /*********************************************************************/
    /* context of the thread when calling a thread dispatch              */
    /*********************************************************************/
    ThreadCpuBaseContext baseContext;

    /* floating point context */
    uint32_t s[32];
    uint32_t FPEXC;
    uint32_t FPSCR;
    uint32_t FPINST;
    uint32_t FPINST2;

    /*******************************************************************************************************************/
    /* context of the thread when calling an interrupt occurred.                                                       */
    /* When an interrupt occurs it must save ALL registers, including r0-r3 and SPSR so a thread dispatch can occur    */
    /* and when the thread is restored again, the interrupt handler can go this part of the TCB and restore the thread */
    /* state fully.                                                                                                    */
    /*******************************************************************************************************************/

    uint32_t interrrupt_SPSR;
    uint32_t interrrupt_TLB;
    uint32_t interrrupt_r3;
    uint32_t interrrupt_r4;
    uint32_t interrrupt_r5;
    uint32_t interrrupt_r6;
    uint32_t interrrupt_r7;
    uint32_t interrrupt_r8;
    uint32_t interrrupt_r9;
    uint32_t interrrupt_r10;
    uint32_t interrrupt_r11;
    uint32_t interrrupt_r12;
    uint32_t interrrupt_lr;
    uint32_t interrrupt_r0;
    uint32_t interrrupt_r1;
    uint32_t interrrupt_r2;

    uint32_t interrupt_sp_supervisor;
    uint32_t interrupt_lr_supervisor;
    uint32_t interrupt_SPSR_supervisor;



    /* floating point context */
    uint32_t interrupt_s[32];
    uint32_t interrupt_FPEXC;
    uint32_t interrupt_FPSCR;
    uint32_t interrupt_FPINST;
    uint32_t interrupt_FPINST2;

    /*********************************************************************/
    /* kernel stack pointer                                              */
    /*********************************************************************/

    uint32_t kernelStack;

    /*********************************************************************/
    /* TLB pointer in user mode                                          */
    /*********************************************************************/

    uint32_t tlb_user;


    /****************************************************************************/
    /* Entry point of the thread (copy here to use when executing in user mode) */
    /****************************************************************************/

    uint32_t entryPoint;

    /* saved return address when doing an IPC (saved the context) */
    uint32_t r14Ipc;

    /****************************************************************************/
    /* Direct I/O memory area                                                   */
    /****************************************************************************/

    /**
     * Additional memory that is written when the thread wants to execute a direct I/O memory area
     */
    ThreadIoMemAccess ioAccess;
};


/**
 * restore the pack definition
 */
#pragma pack(pop)


extern uint8_t pharosIrqSupervisorStack[];


#endif /* PHAROS_CONTEXT_H */
