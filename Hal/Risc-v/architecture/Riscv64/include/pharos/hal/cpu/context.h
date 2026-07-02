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

#include <pharos/hal/declarations.h>
#include <pharos/hal/cpu/conf.h>
#include <pharos/hal/io.h>


#define PHAROS_CPU_SATP_MODE_SV32               (1UL  << 60U)
#define PHAROS_CPU_SATP_MODE_SV39               (8UL  << 60U)
#define PHAROS_CPU_SATP_MODE_SV48               (9UL  << 60U)
#define PHAROS_CPU_SATP_MODE_SV57               (10UL << 60U)
#define PHAROS_CPU_SATP_MODE_SV64               (11UL << 60U)


/**
 * Make sure this context is packed to 8 bytes 
 */
#pragma pack(push , 8)

struct ThreadCpuBaseContext
{
    uint64_t mstatusContext;
    uint64_t satp;
    uint64_t raContext;
    uint64_t spContext;
    uint64_t t0Context;
    uint64_t t1Context;
    uint64_t t2Context;
    uint64_t t3Context;
    uint64_t t4Context;
    uint64_t t5Context;
    uint64_t t6Context;
    uint64_t s0Context;
    uint64_t s1Context;
    uint64_t s2Context;
    uint64_t s3Context;
    uint64_t s4Context;
    uint64_t s5Context;
    uint64_t s6Context;
    uint64_t s7Context;
    uint64_t s8Context;
    uint64_t s9Context;
    uint64_t s10Context;
    uint64_t s11Context;
};

struct ThreadCpuContext
{
    /*********************************************************************/
    /* context of the thread when calling a thread dispatch              */
    /*********************************************************************/
    ThreadCpuBaseContext baseContext;

    /*********************************************************************/
    /* context of the thread when an interrupt occurs                    */
    /*********************************************************************/
    uint64_t irqRa;
    uint64_t irqSp;
    uint64_t irqGp;
    uint64_t irqTp;
    uint64_t irqT0;
    uint64_t irqT1;
    uint64_t irqT2;
    uint64_t irqS0;
    uint64_t irqS1;
    uint64_t irqA0;
    uint64_t irqA1;
    uint64_t irqA2;
    uint64_t irqA3;
    uint64_t irqA4;
    uint64_t irqA5;
    uint64_t irqA6;
    uint64_t irqA7;
    uint64_t irqS2;
    uint64_t irqS3;
    uint64_t irqS4;
    uint64_t irqS5;
    uint64_t irqS6;
    uint64_t irqS7;
    uint64_t irqS8;
    uint64_t irqS9;
    uint64_t irqS10;
    uint64_t irqS11;
    uint64_t irqT3;
    uint64_t irqT4;
    uint64_t irqT5;
    uint64_t irqT6;
    uint64_t irqMepc;
    uint64_t irqMstatus;

    uint64_t irqFpuContext[32U] VARIABLE_ALIGNMENT_X(16);
    uint64_t irqFcsrContext;

    /*********************************************************************/
    /* thread information                                                */
    /*********************************************************************/
    uint64_t entryPoint;

    /**
     * original user stack base 
     */
    uint64_t userStack;

    /**
     * Stack saved before issuing an inter-partition call and restored when it gets backs
     */
    uint64_t userStackCurrent;

    /**
     * Stack saved when the thread exits from machine mode to user mode and restored when it gets back to machine mode 
     */
    uint64_t kernelStackCurrent;

    uint64_t raIpc;

    uint64_t fpuContext[32U] VARIABLE_ALIGNMENT_X(16);

    uint64_t fcsrContext;

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
 * Get the current stack pointer
 * 
 * @return returns the current stack pointer
 */
uint8_t *pharosCpuSpGet(void);


/**
 * Get the MIP register
 * 
 * @return returns the MIP register
 */
uint64_t pharosCpuMipGet(void);


/**
 * Set the specified bits on the MIE register
 * 
 * @param bits bits to set
 */
void pharosCpuMieSetBit(uint64_t bits);


/* PHAROS_CONTEXT_H */
#endif 
