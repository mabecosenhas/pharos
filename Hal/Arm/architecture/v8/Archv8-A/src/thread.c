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
#include <pharos/hal/cpu/asm.h>
#include <pharos/hal/v8a/context.h>
#include <pharos/kernel/thread/thread.h>


void pharosCpuThreadContextReInit(ptrThreadInfo thread)
{
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
    context->baseContext.x8 = 0U;
    context->baseContext.x9 = 0U;
    context->baseContext.x10 = 0U;
    context->baseContext.x11 = 0U;
    context->baseContext.x12 = 0U;
    context->baseContext.x13 = 0U;
    context->baseContext.x14 = 0U;
    context->baseContext.x15 = 0U;
    context->baseContext.x16 = 0U;
    context->baseContext.x17 = 0U;
    context->baseContext.x18 = 0U;
    context->baseContext.x19 = 0U;
    context->baseContext.x20 = 0U;
    context->baseContext.x21 = 0U;
    context->baseContext.x22 = 0U;
    context->baseContext.x23 = 0U;
    context->baseContext.x24 = 0U;
    context->baseContext.x25 = 0U;
    context->baseContext.x26 = 0U;
    context->baseContext.x27 = 0U;
    context->baseContext.x28 = 0U;
    context->baseContext.x29 = 0U;

    context->baseContext.x30 = kernelEntryPoint;

    /* set the EL0 pointer (stack grows down) */
    context->baseContext.sp_el0 = ((uintptr_t) applicationStack->stack) + applicationStack->size;

    /* initialize the EL1 stack */
    context->baseContext.sp_el1 = ((uintptr_t) kernelStack->stack) + kernelStack->size;

    /* set the thread entry point (will be executed once the context switch routine returns) */
    context->baseContext.elr_l1 = kernelEntryPoint;

    context->entryPoint = (uintptr_t) userEntryPoint;

    /* thread will start in EL1 mode with IRQ and FIQ enabled. On the entry will switch to EL0 mode */
    context->baseContext.spsr_l1 = SPSR_EL1_MODE | SPSR_IRQ_ENABLE | SPSR_FIQ_ENABLE | SPSR_SP_MODE;
}


void pharosCpuThreadContextInit(ptrPartition partition , ptrThreadInfo thread , ptrEnvMemoryProtection memProtection)
{
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
    context->baseContext.x8 = 0U;
    context->baseContext.x9 = 0U;
    context->baseContext.x10 = 0U;
    context->baseContext.x11 = 0U;
    context->baseContext.x12 = 0U;
    context->baseContext.x13 = 0U;
    context->baseContext.x14 = 0U;
    context->baseContext.x15 = 0U;
    context->baseContext.x16 = 0U;
    context->baseContext.x17 = 0U;
    context->baseContext.x18 = 0U;
    context->baseContext.x19 = 0U;
    context->baseContext.x20 = 0U;
    context->baseContext.x21 = 0U;
    context->baseContext.x22 = 0U;
    context->baseContext.x23 = 0U;
    context->baseContext.x24 = 0U;
    context->baseContext.x25 = 0U;
    context->baseContext.x26 = 0U;
    context->baseContext.x27 = 0U;
    context->baseContext.x28 = 0U;
    context->baseContext.x29 = 0U;

    if(partition == NULL)
    {
        context->baseContext.x30 = (uintptr_t) pharosCpuExecuteUserModeIdle;
    }
    else
    {
        context->baseContext.x30 = (uintptr_t) kernelEntryPoint;
    }

    /* set the EL0 pointer (stack grows down) */
    context->baseContext.sp_el0 = ((uintptr_t) applicationStack->stack) + applicationStack->size;

    /* initialize the EL1 stack */
    context->baseContext.sp_el1 = ((uintptr_t) kernelStack->stack) + kernelStack->size;

    /* set the thread entry point (will be executed once the context switch routine returns) */
    context->baseContext.elr_l1 = (uintptr_t) kernelEntryPoint;

    /* save a copy of the entry point to the CPU context for quick lookup of the function to execute when switching from supervisor to user mode */
    context->entryPoint = (uintptr_t) userEntryPoint;

    /* thread will start in EL1 mode with IRQ and FIQ enabled. On the entry will switch to EL0 mode */
    context->baseContext.spsr_l1 = SPSR_EL1_MODE | SPSR_IRQ_ENABLE | SPSR_FIQ_ENABLE | SPSR_SP_MODE;

    /* TLB of the thread */
    context->baseContext.tlb_el0 = pharosCpuMmuGetTlb(partition);

    /* initialize the IO memory access */
    pharosArmIoAccessInitialize(&context->ioAccess);
}


CpuModeChangeContext pharosCpuSwitchSupervisorMode(CpuModeChangeContext context)
{
    /* clear the mode from the SPSR -> this will place it at EL0 and set the SPSR return mode to EL1 */
    context.spsr = (context.spsr & SPSR_EL_MASK) | SPSR_SP_MODE | SPSR_EL1_MODE;

    /* return the modified context */
    return context;
}
