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


#include <pharos/hal/hal.h>
#include <pharos/hal/v7m/nvic.h>
#include <pharos/hal/v7m/conf.h>
#include <pharos/hal/v7m/scb.h>
#include <pharos/hal/v7m/cache.h>
#include <pharos/kernel/interrupt/interrupt.h>
#include <pharos/kernel/thread/dispatcher.h>
#include <pharos/kernel/scheduler/scheduler.h>
#include <pharos/hal/v7m/misc.h>
#include <pharos/kernel/core/core.h>
#include <pharos/hal/v7m/mpu.h>
#include <pharos/hal/v7m/systick.h>
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/hal/cpu/sections.h>
#include <pharos/kernel/interrupt/interruptinline.h>


extern uint16_t criticalSectionExecuteEndCall;


/**
 * Interrupt stack
 */
KERNEL_SECTION_USS uint8_t pharosCpuInterruptStack[PHAROS_CPU_INTERRUPT_STACK_SIZE];


/**
 * Sets the priority associated with the interrupt number
 * 
 * @param number interrupt number
 * @param priority priority to set
 */
static void pharosCpuNvicSetPriority(PharosInterruptNumber number , PharosInterruptPriority priority);


static void pharosHardHandler(void);
static void pharosBusHandler(void);
static void pharosUsageHandler(void);
static void pharosDebugMonitor(void);
static void interruptHandler(void);
static void exceptionHandler(void);
static void generalInterruptHandler(PharosInterruptNumber vector);


PHAROS_SECTION(".vectorTable") PharosCpuInterruptTable pharosCpuInitVector = {

    /* initial Main stack pointer at reset */
    .initialStackPointer = pharosCpuInterruptStack + PHAROS_CPU_INTERRUPT_STACK_SIZE ,
    .resetHandler = pharosCpuStart ,
    .nmiHandler = exceptionHandler ,
    .hardFaultHandler = pharosHardHandler ,
    .mpuFaultHandler = pharosMpuHandler ,
    .busFaultHandler = pharosBusHandler ,
    .usageFaultHandler = pharosUsageHandler ,
    .syscallHandler = pharosCpuSyscallHandler ,
    .debugMonitorHandler = pharosDebugMonitor ,
    .pendableHandler = pharosCpuPendableHandler ,
    .systickHandler = exceptionHandler ,
    .interruptTable =
    {
        /* initialize all interrupts with the interrupt handler */
        .interruptVector =
        {[0 ... sizeof (PharosCpuIsrTable) / sizeof (PharosRawInterruptHandler) - 1] = interruptHandler} ,
    }
};


void pharosBusHandler(void)
{
    /* get the stack saved */
    ptrPharosCpuExceptionFrame ef = pharosCpuPspGet();

    /* announce a cpu bus error */
    pharosSErrorAnnounce(PHAROS_ERR_CPU_BUS , (uint8_t *) ef->register_pc);
}


void pharosUsageHandler(void)
{
    /* get the stack saved */
    ptrPharosCpuExceptionFrame ef = pharosCpuPspGet();

    /* announce a CPU error usage */
    pharosSErrorAnnounce(PHAROS_ERR_CPU_USAGE , (uint8_t *) ef->register_pc);
}


void pharosDebugMonitor()
{
    /* get the stack saved */
    ptrPharosCpuExceptionFrame ef = pharosCpuPspGet();

    /* announce a debug exception */
    pharosSErrorAnnounce(PHAROS_ERR_CPU_DEBUG , (uint8_t *) ef->register_pc);
}


void pharosHardHandler()
{
    /* get the previous stack (to get the  */
    ptrPharosCpuExceptionFrame f = pharosCpuPspGet();


    /* check if the error came from a trying to invoke a "end of critical section syscall" */
    if(f->register_pc == &criticalSectionExecuteEndCall)
    {
        /* returning from a critical section, can enable interrupts and return to same instruction (swi) */
        pharosCpuPrimaskEnable();

        /* re-execute the same instruction */
        f->register_pc--;

        /* note that application threads are supposed to run with interrupts enabled, so if they try to execute this code 
         * they will accomplish nothing */
    }
    else
    {
        /* Hint: if your program reaches here, you can try to debug it by checking the CFSR or SHCSR registers
         * (PHAROS_CPU_SCB->configurableFaultStatus) or (PHAROS_CPU_SCB->systemHandlerControlState) and check its value.
         * In GDB you could do "print/x *((ptrPharosCpuScb) 0xE000ED00UL)" to check all registers */

        /* announce the hard fault */
        pharosSErrorAnnounce(PHAROS_ERR_CPU_HARD , (uint8_t *) f->register_pc);
    }
}


void pharosCpuNvicInitialize(ptrInterruptTable table)
{
    /* interrupt iterator */
    PharosInterruptNumber i;


    /* set a high priority for the bus fault exception */
    pharosCpuNvicSetPriority(PHAROS_CPU_EXCEPTION_BUS_FAULT , PHAROS_INTERRUPT_PRIORITY_HIGHEST);

    /* set a high priority for the usage exception */
    pharosCpuNvicSetPriority(PHAROS_CPU_EXCEPTION_USAGE , PHAROS_INTERRUPT_PRIORITY_HIGHEST);

    /* set a high priority for the debug exception */
    pharosCpuNvicSetPriority(PHAROS_CPU_EXCEPTION_DEBUG_MON , PHAROS_INTERRUPT_PRIORITY_HIGHEST);

    /* set the a higher priority than MPU exception for the system tick */
    pharosCpuNvicSetPriority(PHAROS_CPU_EXCEPTION_SYSTICK , PHAROS_INTERRUPT_PRIORITY_LOWEST - 2);

    /* set the low priority to the MPU fault exception */
    pharosCpuNvicSetPriority(PHAROS_CPU_EXCEPTION_MPU , PHAROS_INTERRUPT_PRIORITY_LOWEST - 1);

    /* set the lowest priority to the system call exception */
    pharosCpuNvicSetPriority(PHAROS_CPU_EXCEPTION_SUPER_CALL , PHAROS_INTERRUPT_PRIORITY_LOWEST);

    /* set the lowest priority to the pendable exception */
    pharosCpuNvicSetPriority(PHAROS_CPU_EXCEPTION_PENDABLE , PHAROS_INTERRUPT_PRIORITY_LOWEST);

    /* set the priority for each interrupt*/
    for(i = 0U; i < PHAROS_CPU_NUMBER_INTERRUPTS; i++)
    {
        /* set the lowest priority for all interrupts */
        pharosCpuNvicSetPriority(i + PHAROS_CPU_EXCEPTION_SYSTICK + 1 , PHAROS_INTERRUPT_PRIORITY_LOWEST - 1);
    }
}


bool pharosCpuNvicIsException(PharosInterruptNumber number)
{
    /* return value */
    bool result;


    /* if the number is below 16 */
    if(number <= PHAROS_CPU_EXCEPTION_SYSTICK)
    {
        /* then it is an exception */
        result = TRUE;
    }
    else
    {
        /* it is an interrupt */
        result = FALSE;
    }

    /* return TRUE if the number corresponds to an exception and returns FALSE otherwise */
    return result;
}


uint32_t interruptNumber(PharosInterruptNumber number)
{
    /* return the interrupt number */
    return number - (PHAROS_CPU_EXCEPTION_SYSTICK + 1);
}


void pharosCpuClearPendingInterrupt(PharosInterruptNumber number)
{
    /* get the interrupt number */
    uint32_t interruptN = interruptNumber(number);


    /* clear the pending interrupt */
    PHAROS_CPU_NVIC->interruptClearPending[interruptN >> 5UL] = (uint32_t) (1UL << (interruptN & 0x1FUL));
}


bool pharosCpuInterruptIsPending(PharosInterruptNumber number)
{
    /* get the interrupt number */
    uint32_t interruptN = interruptNumber(number);


    /* check if the interrupt is pending */
    return PHAROS_CPU_NVIC->interrruptSetPending[interruptN >> 5UL] & ((1UL << interruptN & 0x1FUL) == 0) ? FALSE : TRUE;
}


void generalInterruptHandler(PharosInterruptNumber vector)
{
    /* get the interrupted instruction */
    uint8_t *interruptedInstruction = (uint8_t *) pharosCpuPspGet()->register_pc;

    /* interrupted partition running */
    ptrPartition oldRunning = pharosIInterruptRunningEnvGet();

    /* interrupt configuration */
    ptrInterruptConfiguration conf;

    /* return of the pre-handler */
    uintptr_t preInterruptHandlerReturn;


    /* increase the thread dispatch level (no context switch inside the interrupt) */
    pharosVDispatchLevel++;

    /* increase the interrupt nesting level */
    pharosVInterruptNestingLevel++;

    /* get the interrupt configuration */
    conf = pharosIInterruptConfigurationGet(vector);

    /* if there is a routine to clear the interrupt */
    if(conf->preInterruptHandler != NULL)
    {
        /* clear the interrupt using the clear routine */
        preInterruptHandlerReturn = conf->preInterruptHandler(conf->argument);
    }
    else
    {
        /* set the default return value */
        preInterruptHandlerReturn = 0U;
    }

    /* call the application routine */
    pharosIInterruptExecute(conf , preInterruptHandlerReturn , interruptedInstruction);

    /* decrease the interrupt nesting level */
    pharosVInterruptNestingLevel--;

    /* decrease the thread dispatch level */
    pharosVDispatchLevel--;

    /* restore the previous running interrupt partition */
    pharosIInterruptRunningEnvSet(oldRunning);

    /* if thread dispatch is enabled, running the outermost interrupt and a thread context switch is needed */
    if((pharosVDispatchLevel == 0U) &&
       (pharosVInterruptNestingLevel == 0U) &&
       (pharosINeedsThreadContextSwitch() == TRUE))
    {
        /* then generate a pendable interrupt (it will perform the context switch) */
        PHAROS_CPU_SCB->interruptControl = PHAROS_CPU_SCB_ICSR_PENDSVSET;
    }
}


void exceptionHandler(void)
{
    /* get the exception triggered */
    PharosInterruptNumber vector = pharosCpuIpsrGet();


    /* call the generic handler */
    generalInterruptHandler(vector);
}


void interruptHandler(void)
{
    /* get the interrupt vector triggered */
    PharosInterruptNumber vector = PHAROS_CPU_SCB->interruptControl & 0x1ffU;


    /* call the generic handler */
    generalInterruptHandler(vector);
}


void pharosCpuInterruptInstall(ptrInterruptConfiguration conf)
{
    /* set the priority */
    pharosCpuNvicSetPriority(conf->number , conf->cpu.priority);

    /* unmask the interrupt */
    pharosCpuInterruptUnmask(conf);
}


void pharosCpuInterruptRemove(ptrInterruptConfiguration conf)
{
    /* only when there is no handler can de-activate the interrupt */
    pharosCpuInterruptMask(conf);
}


void pharosCpuInterruptUnmask(ptrInterruptConfiguration conf)
{
    /* if the interrupt is not an exception */
    if(pharosCpuNvicIsException(conf->number) == FALSE)
    {
        /* get the interrupt number */
        uint32_t interruptN = interruptNumber(conf->number);


        /* enable the interrupt using the ISER register */
        PHAROS_CPU_NVIC->interruptSetEnable[interruptN >> 5UL] = (uint32_t) (1UL << (interruptN & 0x1FUL));
    }
}


void pharosCpuInterruptMask(ptrInterruptConfiguration conf)
{
    /* if the interrupt is not an exception */
    if(pharosCpuNvicIsException(conf->number) == FALSE)
    {
        /* get the interrupt number */
        uint32_t interruptN = interruptNumber(conf->number);


        /* disable the interrupt using the ICER register */
        PHAROS_CPU_NVIC->interruptClearEnable[interruptN >> 5UL] = (uint32_t) (1UL << (interruptN & 0x1FUL));
    }
}


void pharosCpuNvicSetPriority(PharosInterruptNumber number , PharosInterruptPriority priority)
{
    /* if the interrupt is an exception */
    if(pharosCpuNvicIsException(number) == TRUE)
    {
        /* use the SHPR registers to set the exception priority */
        PHAROS_CPU_SCB->systemHandlerPriority[number - 4U] = priority;
    }
        /* if the interrupt is not an exception */
    else
    {
        /* get the current priority and keep the bits that should be kept */
        uint32_t oldPriority = PHAROS_CPU_NVIC->interruptPriority[interruptNumber(number)] & PHAROS_CPU_INTERRUPT_PRIO_MASK;


        /* set the priority by maintaining the old priority bits that should be kept */
        PHAROS_CPU_NVIC->interruptPriority[interruptNumber(number)] = (priority << (8U - PHAROS_CPU_INTERRUPT_PRIO_BITS)) | oldPriority;
    }
}


void pharosCpuInterruptStackGet(uint8_t **interruptStackStart , uint8_t **interrruptStackEnd)
{
    /* set the start address */
    *interruptStackStart = pharosCpuInterruptStack;

    /* set the end address */
    *interrruptStackEnd = pharosCpuInterruptStack + sizeof (pharosCpuInterruptStack);
}
