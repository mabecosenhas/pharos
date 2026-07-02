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


#include <pharos/hal/arm.h>
#include <pharos/hal/board.h>
#include <pharos/hal/hal.h>
#include <pharos/hal/cpu/cpu.h>
#include <pharos/hal/boardApi.h>
#include <pharos/kernel/thread/stack.h>
#include <pharos/hal/cpu/asm.h>
#include <pharos/kernel/core/syscall.h>
#include <pharos/kernel/partition/ipcall.h>
#include <pharos/hal/fpu.h>
#include <pharos/kernel/object/protection.h>
#include <pharos/hal/memory/pmsa.h>
#include <pharos/kernel/core/core.h>
#include <pharos/kernel/thread/thread.h>

#include "pharos/hal/cpu/context.h"


#define ARM_PUSH_R0                 (0xe52d0004U)
#define ARM_LDR_R0_PC_PLUS(X)       (0xe59f0000U + (X))
#define ARM_LDR_R2_PC_PLUS(X)       (0xe59f2000U + (X))
#define ARM_LDR_R3_PC_PLUS(X)       (0xe59f3000U + (X))
#define ARM_MRC_P15_0_R0_C2_C0_0    (0xee020f10U)
#define ARM_MSR_CPSR_C_USER_MODE    (0xe321f00aU)
#define ARM_POP_R0                  (0xe49d0004U)
#define ARM_POP_PC                  (0xe49df004U)
#define ARM_POP_R0_PC               (0xe83d8001U)
#define ARM_MRC_P15_0_R0_C8_C7_0    (0xee080f17U)
#define ARM_NOP                     (0xe1a00000U)
#define ARM_SWI(X)                  (0xef000000U + (X))
#define ARM_BX_LR                   (0xe12fff1eU)
#define ARM_PUSH_R0_R1_R2           (0xe92d0007U)
#define ARM_PUSH_R0_R1              (0xe92d0003U)
#define ARM_PUSH_R0_R1_LR           (0xe92d4003U)
#define ARM_POP_R2_R3_LR            (0xe8bd400cU)
#define ARM_ADD_SP_SP(X)            (0xe28dd00U + (X))
#define ARM_PUSH_R0_R4_R12_LR       (0xe92d5ff1U)
#define ARM_POP_R2_R4_R12_LR        (0xe8bd5ff4U)

#define INTERRUPT_STACK_SECTION_BSS __attribute__ ((section (".interruptStackBss"))) 

/**
 * stack location of the interrupts running in the supervisor mode
 */
INTERRUPT_STACK_SECTION_BSS uint8_t pharosIrqSupervisorStack[IRQ_SUPERVIOSR_STACK_SIZE];

/**
 * stack location of the IRQ mode
 */
INTERRUPT_STACK_SECTION_BSS uint8_t pharosCpuIrqStack[IRQ_STACK_SIZE];

/**
 * stack location of the FIQ mode
 */
INTERRUPT_STACK_SECTION_BSS uint8_t fiqStack[FIQ_STACK_SIZE];

/**
 * stack location of the abort mode
 */
INTERRUPT_STACK_SECTION_BSS uint8_t abortStack[ABORT_STACK_SIZE];

/**
 * stack location of the undefined mode
 */
INTERRUPT_STACK_SECTION_BSS uint8_t undefinedStack[UNDEFINED_STACK_SIZE];

/**
 * stack location of the supervisor mode
 */
INTERRUPT_STACK_SECTION_BSS uint8_t supervisorStack[SUPERVISOR_STACK_SIZE];

/**
 * stack location of the system mode
 */
INTERRUPT_STACK_SECTION_BSS uint8_t systemStack[SYSTEM_STACK_SIZE];


void pharosCpuRestoreHeir(ptrThreadInfo heir)
{
    /* if not the idle thread */
    if(heir->partition != NULL)
    {
        /* restore the MPU registers of the heir thread */
        pharosCpuMpuRestore(heir);
    }

    /* perform the context switch (low level registers) */
    pharosCpuRestoreHeirLow(&heir->cpuContext);
}


void pharosCpuInterruptStackGet(uint8_t **interruptStackStart , uint8_t **interrruptStackEnd)
{
    /* set the start address */
    *interruptStackStart = pharosIrqSupervisorStack;

    /* set the end address */
    *interrruptStackEnd = pharosIrqSupervisorStack + sizeof (pharosIrqSupervisorStack);
}


void pharosCpuContextSwitch(ptrThreadInfo current , ptrThreadInfo heir)
{
    /* if not the idle thread */
    if(heir->partition != NULL)
    {
        /* restore the MPU registers of the heir thread */
        pharosCpuMpuRestore(heir);
    }

    /* perform the context switch (low level registers) */
    pharosCpuContextSwitchLow(&current->cpuContext , &heir->cpuContext);
}


void pharosCpuThreadContextReInit(ptrThreadInfo thread)
{
    /* get the CPU context */
    ptrThreadCpuContext context = &thread->cpuContext;

    /* thread application stack */
    ptrStack applicationStack = &thread->applicationStack;

    /* thread kernel stack */
    ptrStack kernelStack = &thread->kernelStack;

    /* thread kernel space entry point */
    uintptr_t kernelEntryPoint = thread->kernelEntryPoint;

    /* initialize all registers with default values */
    context->baseContext.r4 = 0U;
    context->baseContext.r5 = 0U;
    context->baseContext.r6 = 0U;
    context->baseContext.r7 = 0U;
    context->baseContext.r8 = 0U;
    context->baseContext.r9 = 0U;
    context->baseContext.r10 = 0U;
    context->baseContext.r11 = 0U;

    /* set the thread sp pointer (stack grows down) */
    context->baseContext.r13_user = ((uint32_t) applicationStack->stack) + applicationStack->size;

    /* set the thread entry point (will be executed once the context switch routine returns) */
    context->baseContext.r14_user = kernelEntryPoint;

    /* initialize the supervisor stack */
    context->baseContext.r13_supervisor = ((uint32_t) kernelStack->stack) + kernelStack->size;

    /* set the thread entry point (will be executed once the context switch routine returns) */
    /* this is different from the context->entryPoint since this the _pharosAperiodicThreadBody or _pharosPeriodicThreadBody, etc */
    context->baseContext.r14_supervisor = kernelEntryPoint;

    /* initialize the kernel stack pointer (stack grows down) */
    context->kernelStack = ((uint32_t) kernelStack->stack) + kernelStack->size;

    /* thread will start in supervisor mode (with IRQ and FIQ enabled) and the on the entry will switch to user mode */
    context->baseContext.cpsr = CPSR_SUPERVISOR_MODE | CPSR_ENABLE_IRQ | CPSR_ENABLE_FIQ;

    /* setup the MPU for the partition data+bss (region 3 of the MPU) */

    /* mark the shared stack as invalid */
    context->sharedStackMpuRegion.size = PHAROS_CPU_PMSA_DRSR_DIS;
}


void pharosCpuThreadContextInit(ptrPartition partition , ptrThreadInfo thread , ptrEnvMemoryProtection memProtection)
{
    /* get the CPU context */
    ptrThreadCpuContext context = &thread->cpuContext;

    /* thread application stack */
    ptrStack applicationStack = &thread->applicationStack;

    /* thread kernel stack */
    ptrStack kernelStack = &thread->kernelStack;

    /* thread user space entry point */
    uintptr_t userEntryPoint = thread->userEntryPoint;

    /* thread kernel space entry point */
    uintptr_t kernelEntryPoint = thread->kernelEntryPoint;

    /* initialize all registers with default values */
    context->baseContext.r4 = 0U;
    context->baseContext.r5 = 0U;
    context->baseContext.r6 = 0U;
    context->baseContext.r7 = 0U;
    context->baseContext.r8 = 0U;
    context->baseContext.r9 = 0U;
    context->baseContext.r10 = 0U;
    context->baseContext.r11 = 0U;

    /* set the thread sp pointer (stack grows down) */
    context->baseContext.r13_user = ((uint32_t) applicationStack->stack) + applicationStack->size;

    /* set the thread entry point (will be executed once the context switch routine returns) */
    context->baseContext.r14_user = (uint32_t) kernelEntryPoint;

    /* initialize the supervisor stack */
    context->baseContext.r13_supervisor = ((uint32_t) kernelStack->stack) + kernelStack->size;

    /* set the thread entry point (will be executed once the context switch routine returns) */
    /* this is different from the context->entryPoint since this the _pharosAperiodicThreadBody or _pharosPeriodicThreadBody, etc */
    context->baseContext.r14_supervisor = (uint32_t) kernelEntryPoint;

    /* save a copy of the entry point to the CPU context for quick lookup of the function to execute when switching from supervisor to user mode */
    context->entryPoint = (uint32_t) userEntryPoint;

    /* initialize the kernel stack pointer (stack grows down) */
    context->kernelStack = ((uint32_t) kernelStack->stack) + kernelStack->size;

    /* thread will start in supervisor mode (with IRQ and FIQ enabled) and the on the entry will switch to user mode */
    context->baseContext.cpsr = CPSR_SUPERVISOR_MODE | CPSR_ENABLE_IRQ | CPSR_ENABLE_FIQ;

    /* mark the shared stack as invalid */
    context->sharedStackMpuRegion.size = PHAROS_CPU_PMSA_DRSR_DIS;

#ifdef PHAROS_CPU_HAS_VFP

    /* initialize the FPEXC FPU register */
    context->FPEXC = pharosCpuFpuGetFpexc();

    /* initialize the FPSCR FPU register */
    context->FPSCR = pharosCpuFpuGetFpscr();

#endif

    /* initialize the IO memory access */
    pharosArmIoAccessInitialize(&context->ioAccess);
}
