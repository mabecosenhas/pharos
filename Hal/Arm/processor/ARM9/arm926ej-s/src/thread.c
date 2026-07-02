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


#include <pharos/hal/cpu/cpu.h>
#include <pharos/hal/boardApi.h>
#include <pharos/kernel/thread/stack.h>
#include <pharos/kernel/core/syscall.h>
#include <pharos/kernel/partition/ipcall.h>
#include <pharos/hal/io.h>
#include <pharos/hal/fpu.h>
#include <pharos/hal/cpu/asm.h>
#include <pharos/kernel/thread/thread.h>


#define INTERRUPT_STACK_SECTION_BSS __attribute__ ((section (".interruptStackBss"))) 



/**
 * stack location of the IRQ mode
 */
INTERRUPT_STACK_SECTION_BSS uint8_t pharosCpuIrqStack[IRQ_STACK_SIZE];

/**
 * stack location of the interrupts running in the supervisor mode
 */
INTERRUPT_STACK_SECTION_BSS uint8_t pharosIrqSupervisorStack[IRQ_SUPERVISOR_STACK_SIZE];

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


void pharosCpuInterruptStackGet(uint8_t **interruptStackStart , uint8_t **interrruptStackEnd)
{
    /* set the start address */
    *interruptStackStart = pharosIrqSupervisorStack;

    /* set the end address */
    *interrruptStackEnd = pharosIrqSupervisorStack + sizeof (pharosIrqSupervisorStack);
}


void *pharosICpuStackAllocate(ptrThreadCpuContext context , size_t size)
{
    /* address that will be allocated on the stack */
    void *result;

    /* stack address */
    uint8_t *stack = (uint8_t *) context->baseContext.r13_user;


    /* reserve the space (stack grows down) */
    stack -= size;

    /* mark the address that was reserved */
    result = stack;

    /* align the stack */
    stack -= pharosIAlignmentNextUnbound(stack , PHAROS_STACK_ALIGNMENT);

    /* set the thread to start at this address */
    context->baseContext.r13_user = (uintptr_t) stack;

    /* return the new stack pointer which is the address of the allocated space */
    return result;
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

    /* but will start with TLB set to kernel memory area (beginning of each thread requires full access) */
    context->baseContext.tlb = (uint32_t) & kernelSectionTable;
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

    /* TLB when in user mode */
    context->tlb_user = pharosCpuMmuGetTlb(partition);

    /* but will start with TLB set to kernel memory area (beginning of each thread requires full access) */
    context->baseContext.tlb = (uint32_t) & kernelSectionTable;

#ifdef PHAROS_CPU_HAS_VFP

    /* initialize the FPEXC FPU register */
    context->FPEXC = pharosCpuFpuGetFpexc();

    /* initialize the Fpscr FPU register */
    context->FPSCR = pharosCpuFpuGetFpscr()

            /* initialize the Fpinst FPU register */;
    context->FPINST = pharosCpuFpuGetFpinst();

    /* initialize the Fpinst2 FPU register */
    context->FPINST2 = pharosCpuFpuGetFpinst2();

#endif 

    /* initialize the IO memory access */
    pharosArmIoAccessInitialize(&context->ioAccess);
}
