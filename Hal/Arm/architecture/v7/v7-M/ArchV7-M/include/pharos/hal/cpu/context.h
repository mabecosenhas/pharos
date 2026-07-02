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
#define	PHAROS_CONTEXT_H


#include <pharos/hal/cpu/conf.h>
#include <pharos/kernel/collection/linkedlist.h>
#include <pharos/hal/io.h>
#include <pharos/hal/v7m/declarations.h>
#include <pharos/hal/v7m/conf.h>
#include <pharos/hal/memory/mpuV7.h>




/**
 * Make sure this call is packed to 4 bytes 
 */
#pragma pack(push ,4)


struct ThreadCpuBaseContext
{
    /*********************************************************************/
    /* context of the thread when calling a thread dispatch              */
    /*********************************************************************/

    uint32_t r4;
    uint32_t r5;
    uint32_t r6;
    uint32_t r7;
    uint32_t r8;
    uint32_t r9;
    uint32_t r10;
    uint32_t r11;
    uint32_t r12;
    uint32_t lr;
    uint32_t psp;
    uint32_t psr;
    uint32_t control;
    uint32_t fpscr;
    uint32_t primask;
};


struct ThreadCpuContext
{
    ThreadCpuBaseContext baseContext;

    /* floating point context */
    uint64_t d[16];

    /*********************************************************************/
    /* kernel stack pointer                                              */
    /*********************************************************************/

    /* current kernel stack */
    uint8_t *currentKernelStack;

    /* original kernel stack */
    uint8_t *kernelStack;

    /* previous stack before the syscall was performed (used on the swi exception) */
    uint32_t previousSvcStack;

    /* previous stack before the exception was performed (used in the pendable exception) */
    uint32_t previousPendableStack;

    /****************************************************************************/
    /* Entry point of the thread (copy here to use when executing in user mode) */
    /****************************************************************************/

    uint32_t entryPoint;

    /****************************************************************************/
    /* Stack information                                                        */
    /****************************************************************************/

    /* current user stack */
    uint32_t userStack;

    uintptr_t lrIpc;

    /* flag indicating if running inside the kernel */
    uint32_t insideKernel;

    /****************************************************************************/
    /* Direct I/O memory area                                                   */
    /****************************************************************************/

    /**
     * Additional memory that is written when the thread wants to execute a direct I/O memory area
     */
    ThreadIoMemAccess ioAccess;

    /****************************************************************************/
    /* MPU                                                                      */
    /****************************************************************************/

    /* MPU region of the thread for the stack (application or shared) */
    PharosMpuV7MRegion stackMpuRegion;

    /* flag indicating if the pendable interrupt has been processed */
    uint32_t pendableDone;
};


/**
 * restore the previous pack definition
 */
#pragma pack(pop)


/**
 * Performs the context switch at low level (without changing the MPU registers)
 * 
 * @param current current thread
 * @param heir heir thread
 */
void pharosCpuContextSwitchLow(ptrThreadCpuContext current , ptrThreadCpuContext heir);


/**
 * Perform the context switch to the first thread (without changing the MPU registers)
 * 
 * @param heir first thread to run
 */
void pharosCpuRestoreHeirLow(ptrThreadCpuContext heir) __attribute__((noreturn));

/* PHAROS_CONTEXT_H */
#endif	
