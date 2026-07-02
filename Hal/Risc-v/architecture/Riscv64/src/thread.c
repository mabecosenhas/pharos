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


#include <pharos/kernel/thread/stack.h>
#include <pharos/hal/cpu/context.h>
#include <pharos/hal/cpu/conf.h>
#include <pharos/kernel/object/protection.h>
#include <pharos/kernel/core/core.h>
#include <pharos/kernel/clock/clock.h>
#include <pharos/hal/cpu/timer.h>
#include <pharos/hal/memory/mmu.h>


void pharosCpuThreadContextReInit(ptrThreadInfo thread)
{
    /* iterator */
    uint32_t i;

    /* get the CPU context */
    ptrThreadCpuContext context = &thread->cpuContext;

    /* thread application stack */
    ptrStack applicationStack = &thread->applicationStack;

    /* thread kernel stack */
    ptrStack kernelStack = &thread->kernelStack;

    /* thread kernel space entry point */
    uintptr_t kernelEntryPoint = thread->kernelEntryPoint;


    /* set the entry point (aperiodic, periodic, sporadic thread entry point) */
    context->baseContext.raContext = kernelEntryPoint;

    /* initialize all registers */
    context->baseContext.t0Context = 0U;
    context->baseContext.t1Context = 0U;
    context->baseContext.t2Context = 0U;
    context->baseContext.t3Context = 0U;
    context->baseContext.t4Context = 0U;
    context->baseContext.t5Context = 0U;
    context->baseContext.t6Context = 0U;
    context->baseContext.s0Context = 0U;
    context->baseContext.s1Context = 0U;
    context->baseContext.s2Context = 0U;
    context->baseContext.s3Context = 0U;
    context->baseContext.s4Context = 0U;
    context->baseContext.s5Context = 0U;
    context->baseContext.s6Context = 0U;
    context->baseContext.s7Context = 0U;
    context->baseContext.s8Context = 0U;
    context->baseContext.s9Context = 0U;
    context->baseContext.s10Context = 0U;
    context->baseContext.s11Context = 0U;

    /* initialize the FPU registers */
    for(i = 0; i < 32U; i++)
    {
        context->fpuContext[i] = 0U;
    }

    /* set the mstatus for MPP = user */
    context->baseContext.mstatusContext = PHAROS_CPU_MSTATUS_MIE | PHAROS_CPU_MSTATUS_UIE | PHAROS_CPU_MSTATUS_SIE |
            PHAROS_CPU_MSTATUS_MPP_USER | PHAROS_CPU_MSTATUS_UPIE | PHAROS_CPU_MSTATUS_MPIE |
            PHAROS_CPU_MSTATUS_FS_MASK | PHAROS_CPU_MSTATUS_XS_MASK;

    /* initialize the user stack with the top of the application stack */
    context->userStack = ((uintptr_t) applicationStack->stack) + applicationStack->size;

    /* initialize the kerne stack with the top of the kernel stack */
    context->kernelStackCurrent = ((uintptr_t) kernelStack->stack) + kernelStack->size;

    /* the thread starts on machine mode, so initialize its stack to the kernel mode */
    context->baseContext.spContext = context->kernelStackCurrent;
    
    /* note: the thread->wcet is only required to be initialized once */
}


void pharosCpuThreadContextInit(ptrPartition partition , ptrThreadInfo thread , ptrEnvMemoryProtection memProtection)
{
    /* iterator */
    uint32_t i;

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


    /* set the mstatus to enable interrupts on all modes */
    if(partition != NULL)
    {
        /* set the mstatus for MPP = user */
        context->baseContext.mstatusContext = PHAROS_CPU_MSTATUS_MIE | PHAROS_CPU_MSTATUS_UIE | PHAROS_CPU_MSTATUS_SIE |
                PHAROS_CPU_MSTATUS_MPP_USER | PHAROS_CPU_MSTATUS_UPIE | PHAROS_CPU_MSTATUS_MPIE |
                PHAROS_CPU_MSTATUS_FS_MASK | PHAROS_CPU_MSTATUS_XS_MASK;
    }
    else
    {
        /* set the mstatus to enable interrupts */
        context->baseContext.mstatusContext = PHAROS_CPU_MSTATUS_MIE | PHAROS_CPU_MSTATUS_UIE | PHAROS_CPU_MSTATUS_SIE |
                PHAROS_CPU_MSTATUS_MPP_MACHINE | PHAROS_CPU_MSTATUS_UPIE | PHAROS_CPU_MSTATUS_MPIE |
                PHAROS_CPU_MSTATUS_FS_MASK | PHAROS_CPU_MSTATUS_XS_MASK;
    }

    /* shift the SATP register by 12 */
    context->baseContext.satp = pharosCpuMmuGetSatp(partition);

    /* set the entry point (aperiodic, periodic, sporadic thread entry point) */
    context->baseContext.raContext = kernelEntryPoint;

    /* initialize all registers */
    context->baseContext.t0Context = 0U;
    context->baseContext.t1Context = 0U;
    context->baseContext.t2Context = 0U;
    context->baseContext.t3Context = 0U;
    context->baseContext.t4Context = 0U;
    context->baseContext.t5Context = 0U;
    context->baseContext.t6Context = 0U;
    context->baseContext.s0Context = 0U;
    context->baseContext.s1Context = 0U;
    context->baseContext.s2Context = 0U;
    context->baseContext.s3Context = 0U;
    context->baseContext.s4Context = 0U;
    context->baseContext.s5Context = 0U;
    context->baseContext.s6Context = 0U;
    context->baseContext.s7Context = 0U;
    context->baseContext.s8Context = 0U;
    context->baseContext.s9Context = 0U;
    context->baseContext.s10Context = 0U;
    context->baseContext.s11Context = 0U;

    /* initialize the thread user mode entry point */
    context->entryPoint = userEntryPoint;

    /* initialize the FPU registers */
    for(i = 0; i < 32U; i++)
    {
        context->fpuContext[i] = 0U;
    }

    /* initialize the user stack with the top of the application stack */
    context->userStack = ((uintptr_t) applicationStack->stack) + applicationStack->size;

    /* initialize the kerne stack with the top of the kernel stack */
    context->kernelStackCurrent = ((uintptr_t) kernelStack->stack) + kernelStack->size;

    /* the thread starts on machine mode, so initialize its stack to the kernel mode */
    context->baseContext.spContext = context->kernelStackCurrent;

    /* if the thread has a WCET */
    if(thread->wcet != PHAROS_WCET_UNLIMITED)
    {
        /* then convert it to the number of number of that the mtimer precision base */
        thread->wcet = PHAROS_CPU_MICRO_2_TIMER(thread->wcet);
    }
}


CpuModeChangeContext pharosCpuSwitchSupervisorMode(CpuModeChangeContext context)
{
    /* make the MPP switch to machine mode when an mret is performed */
    return context | PHAROS_CPU_MSTATUS_MPP_MACHINE;
}


StackPointer pharosCpuInitializeStackPointer(StackPointer originalPointer , uint32_t stackSize)
{
    /* on ARM the stack grows down, so the correct stack pointer is at the end of the stack array */
    return originalPointer + stackSize;
}
