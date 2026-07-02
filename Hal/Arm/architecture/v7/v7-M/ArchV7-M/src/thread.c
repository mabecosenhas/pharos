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


#include <pharos/hal/v7m/declarations.h>
#include <pharos/hal/v7m/isr.h>
#include <pharos/hal/cpu/cpu.h>
#include <pharos/hal/thumb.h>
#include <pharos/hal/v7m/systick.h>
#include <pharos/hal/cpu/context.h>
#include <pharos/kernel/thread/stack.h>
#include <pharos/hal/v7m/mpu.h>
#include <pharos/hal/v7m/cache.h>
#include <pharos/hal/v7m/fpu.h>
#include <pharos/hal/memory/mpuV7.h>
#include <pharos/hal/cpu/conf.h>
#include <pharos/kernel/object/protection.h>
#include <pharos/kernel/core/core.h>
#include <pharos/kernel/clock/clock.h>
#include <pharos/hal/v7m/nvic.h>
#include <pharos/hal/cpu/timer.h>
#include <pharos/kernel/thread/thread.h>
#include <pharos/kernel/assert/assert.h>


PHAROS_WEAK void pharosCpuClockInitialize(void)
{
    /* initialize the systick */
    pharosCpuSystickInit();

    /* initialize the timer 0 to operate as the WCET timer */
    pharosCpuTimerWcetInit();
}


PHAROS_WEAK void pharosCpuShutdown(void)
{
    while(1);
}


void pharosCpuThreadContextReInit(ptrThreadInfo thread)
{
    /* get the CPU context */
    ptrThreadCpuContext context = &thread->cpuContext;

    /* thread kernel space entry point */
    uintptr_t kernelEntryPoint = thread->kernelEntryPoint;

    /* thread application stack */
    ptrStack applicationStack = &thread->applicationStack;

    /* thread kernel stack */
    ptrStack kernelStack = &thread->kernelStack;

    /* get the application stack to restore */
    ptrStack stack = &thread->applicationStack;


    /* initialize all registers with default values */
    context->baseContext.r4 = 0U;
    context->baseContext.r5 = 0U;
    context->baseContext.r6 = 0U;
    context->baseContext.r7 = 0U;
    context->baseContext.r8 = 0U;
    context->baseContext.r9 = 0U;
    context->baseContext.r10 = 0U;
    context->baseContext.r11 = 0U;
    context->baseContext.r12 = 0U;


    /* set the thread sp pointer (stack grows down) */
    context->baseContext.psp = ((uint32_t) kernelStack->stack) + kernelStack->size;

    /* set the entry point to kernel mode */
    context->baseContext.lr = (uint32_t) kernelEntryPoint;

    /* initialize the kernel stack pointer (stack grows down) */
    context->kernelStack = kernelStack->stack + kernelStack->size;

    /* initialize the current kernel stack */
    context->currentKernelStack = context->kernelStack;

    /* initialize the user stack */
    context->userStack = ((uint32_t) applicationStack->stack) + applicationStack->size;

    /* maintain the thumb mode */
    context->baseContext.psr = 0x01000000U;

    /* start with the thread inside the kernel */
    context->insideKernel = TRUE;

    /* reset the pendable done flag */
    context->pendableDone = FALSE;

    /* initialize with interrupts enabled */
    context->baseContext.primask = 0;

    /* set the address of the stack area */
    context->stackMpuRegion.base = PHAROS_CPU_MPU_RBAR_ADDR((uint32_t) stack->stack) |
            PHAROS_CPU_MPU_RBAR_VALID | PHAROS_CPU_MPU_RBAR_REGION(PHAROS_CPU_MPU_REGION_STACK);

    /* set the no-execute permissions and read/write for all modes */
    context->stackMpuRegion.size = PHAROS_CPU_MPU_RASR_XN_DIS | PHAROS_CPU_MPU_RASR_AP_FULL |
            PHAROS_CPU_MPU_RASR_TEX_NOR | PHAROS_CPU_MPU_RASR_S_NOR | PHAROS_CPU_MPU_RASR_C_NOR |
            PHAROS_CPU_MPU_RASR_B_NOR | PHAROS_CPU_MPU_RASR_SRD_EN |
            PHAROS_CPU_MPU_RASR_SIZE(pharosUint32Log2Ceil(stack->size)) | PHAROS_CPU_MPU_RASR_EN;

    /* if the CPU has floating point */
#ifdef PHAROS_CPU_HAS_VFP

    /* thread will start in privileged mode and with FP extension activated */
    context->baseContext.control = PHAROS_CPU_CONTROL_PRIV | PHAROS_CPU_CONTROL_PROCESS_SP | PHAROS_CPU_CONTROL_FP_EXT_ACT;

    /* if the CPU does not have floating point */
#else

    /* thread will start in privileged mode and with FP extension de-activated */
    context->baseContext.control = PHAROS_CPU_CONTROL_PRIV | PHAROS_CPU_CONTROL_PROCESS_SP | PHAROS_CPU_CONTROL_FP_EXT_DIS;

#endif
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

    /* get the application stack to restore */
    ptrStack stack = &thread->applicationStack;


    /* initialize all registers with default values */
    context->baseContext.r4 = 0U;
    context->baseContext.r5 = 0U;
    context->baseContext.r6 = 0U;
    context->baseContext.r7 = 0U;
    context->baseContext.r8 = 0U;
    context->baseContext.r9 = 0U;
    context->baseContext.r10 = 0U;
    context->baseContext.r11 = 0U;
    context->baseContext.r12 = 0U;

    /* set the thread sp pointer (stack grows down) */
    context->baseContext.psp = ((uint32_t) kernelStack->stack) + kernelStack->size - 8U;

    /* set the entry point to kernel mode */
    context->baseContext.lr = (uint32_t) kernelEntryPoint;

    /* set the entry point for user mode */
    context->entryPoint = (uint32_t) userEntryPoint;

    /* initialize the kernel stack pointer (stack grows down) */
    context->kernelStack = kernelStack->stack + kernelStack->size - 8U;

    /* initialize the current kernel stack */
    context->currentKernelStack = context->kernelStack;

    /* initialize the user stack */
    context->userStack = ((uint32_t) applicationStack->stack) + applicationStack->size;

    /* maintain the thumb mode */
    context->baseContext.psr = 0x01000000U;

    /* start with the thread inside the kernel */
    context->insideKernel = TRUE;

    /* reset the pendable done flag */
    context->pendableDone = FALSE;

    /* initialize with interrupts enabled */
    context->baseContext.primask = 0;

    /* if the CPU has floating point */
#ifdef PHAROS_CPU_HAS_VFP

    /* thread will start in privileged mode and with FP extension activated */
    context->baseContext.control = PHAROS_CPU_CONTROL_PRIV | PHAROS_CPU_CONTROL_PROCESS_SP | PHAROS_CPU_CONTROL_FP_EXT_ACT;

    /* if the CPU does not have floating point */
#else

    /* thread will start in privileged mode and with FP extension de-activated */
    context->baseContext.control = PHAROS_CPU_CONTROL_PRIV | PHAROS_CPU_CONTROL_PROCESS_SP | PHAROS_CPU_CONTROL_FP_EXT_DIS;

#endif

    /* set the address of the stack area */
    context->stackMpuRegion.base = PHAROS_CPU_MPU_RBAR_ADDR((uint32_t) stack->stack) |
            PHAROS_CPU_MPU_RBAR_VALID | PHAROS_CPU_MPU_RBAR_REGION(PHAROS_CPU_MPU_REGION_STACK);

    /* set the no-execute permissions and read/write for all modes */
    context->stackMpuRegion.size = PHAROS_CPU_MPU_RASR_XN_DIS | PHAROS_CPU_MPU_RASR_AP_FULL |
            PHAROS_CPU_MPU_RASR_TEX_NOR | PHAROS_CPU_MPU_RASR_S_NOR | PHAROS_CPU_MPU_RASR_C_NOR |
            PHAROS_CPU_MPU_RASR_B_NOR | PHAROS_CPU_MPU_RASR_SRD_EN |
            PHAROS_CPU_MPU_RASR_SIZE(pharosUint32Log2Ceil(stack->size)) | PHAROS_CPU_MPU_RASR_EN;

    /* initialize the IO memory access */
    pharosArmIoAccessInitialize(&context->ioAccess);
}
