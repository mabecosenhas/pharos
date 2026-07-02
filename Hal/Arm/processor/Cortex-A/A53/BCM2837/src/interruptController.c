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


#include <pharos/hal/cpu/interruptController.h>
#include <pharos/kernel/interrupt/interrupt.h>
#include <pharos/error.h>
#include <pharos/hal/boardApi.h>
#include <pharos/hal/cpu/cache.h>
#include <pharos/hal/cpu/gptimer.h>
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/hal/cpu/cpu.h>
#include <pharos/hal/cpu/sections.h>
#include <pharos/kernel/interrupt/interruptinline.h>


static void enableInterrupt(PharosInterruptNumber number , ptrInterruptConfiguration conf);
static void disableInterrupt(PharosInterruptNumber number);


/**
 * C interrupt handler. Called from the assembly interrupt handler 
 * 
 * @param interruptedAddress address that was being executed when the interrupt occurred 
 * 
 * @return returns the thread dispatch disable level for the running core
 */
uintptr_t pharosCpuCInterruptHandler(const uint8_t *interruptedAddress);


uintptr_t pharosCpuCInterruptHandler(const uint8_t *interruptedAddress)
{
    /* iterator of the interrupt number that needs to be processed */
    PharosInterruptNumber iterator;

    /* interrupt configuration */
    ptrInterruptConfiguration conf;

    /* return of the pre-handler */
    uintptr_t preInterruptHandlerReturn;

    /* running core */
    CoreNumber core = pharosICoreRunning();

    /* get the pending interrupts */
    uint32_t source = PHAROS_CPU_REGISTERS->coreIrqSources[core];

    /* interrupted partition running */
    ptrPartition oldRunning = pharosIInterruptRunningEnvGet();


    /* increase the interrupt nest level */
    pharosVRmpCore[core].interruptLevel++;

    /* increment dispatch level */
    pharosVRmpCore[core].dispatchLevel++;

    /* while there are interrupts to process*/
    while(source != 0)
    {
        /* get the first interrupt still to be processed */
        iterator = __builtin_ctz(source);

        /* get the interrupt configuration */
        conf = pharosIInterruptConfigurationGet(iterator + PHAROS_CPU_ISR_CNTPS);

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

        /* now we can enable interrupts since the source that triggered the interrupt has been dealt with */
        pharosCpuInterruptNestedEnable();

        /* execute the interrupt */
        pharosIInterruptExecute(conf , preInterruptHandlerReturn , interruptedAddress);

        /* remove the interrupt number from the list of interrupts to execute */
        source &= ~(1U << iterator);
    }

    /* decrement dispatch level */
    pharosVRmpCore[core].dispatchLevel--;

    /* decrease the nest level */
    pharosVRmpCore[core].interruptLevel--;

    /* restore the previous running interrupt partition */
    pharosIInterruptRunningEnvSet(oldRunning);

    /* return the dispatch level (the assembly code can use this to determine if a context switch may be done) */
    return pharosVRmpCore[core].dispatchLevel;
}


void pharosCpuIcInitialize(ptrInterruptTable table)
{
    /* nothing needs to be done */
}


void enableInterrupt(PharosInterruptNumber number , ptrInterruptConfiguration conf)
{
    /* get the running core */
    CoreNumber core = pharosICoreRunning();


    /* check if the interrupt is related to the timers */
    if(number >= PHAROS_CPU_ISR_CNTPS && number <= PHAROS_CPU_ISR_CNTV)
    {
        if(conf->cpu.priority == PHAROS_CPU_INTERRUPT_IRQ)
        {
            /* clear FIQ bit (do not generate FIQ interrupt) */
            PHAROS_CPU_REGISTERS->coreTimerInterruptControl[core] &= ~(1 << (number + 4));

            /* generate an IRQ interrupt */
            PHAROS_CPU_REGISTERS->coreTimerInterruptControl[core] |= 1 << number;
        }
        else
        {
            /* clear IRQ bit (do not generate IRQ interrupt) */
            PHAROS_CPU_REGISTERS->coreTimerInterruptControl[core] &= ~(1 << number);

            /* generate an FIQ interrupt */
            PHAROS_CPU_REGISTERS->coreTimerInterruptControl[core] |= 1 << (number + 4);
        }
    }
        /* check if the interrupt is related to the mailboxes */
    else if(number >= PHAROS_CPU_ISR_MAILBOX0 && number <= PHAROS_CPU_ISR_MAILBOX3)
    {
        /* switch the number to 0...3 */
        number -= PHAROS_CPU_ISR_MAILBOX0;

        if(conf->cpu.priority == PHAROS_CPU_INTERRUPT_IRQ)
        {
            /* clear FIQ bit (do not generate FIQ interrupt) */
            PHAROS_CPU_REGISTERS->coreMailBoxesInterruptControl[core] &= ~(1 << (number + 4));

            /* generate an IRQ interrupt */
            PHAROS_CPU_REGISTERS->coreMailBoxesInterruptControl[core] |= 1 << number;
        }
        else
        {
            /* clear IRQ bit (do not generate IRQ interrupt) */
            PHAROS_CPU_REGISTERS->coreMailBoxesInterruptControl[core] &= ~(1 << number);

            /* generate an FIQ interrupt */
            PHAROS_CPU_REGISTERS->coreMailBoxesInterruptControl[core] |= 1 << (number + 4);
        }
    }
        /* check if the interrupt is related to the performance monitor */
    else if(number == PHAROS_CPU_ISR_PMU)
    {
        if(conf->cpu.priority == PHAROS_CPU_INTERRUPT_IRQ)
        {
            /* clear FIQ bit (do not generate FIQ interrupt) */
            PHAROS_CPU_REGISTERS->perfMonIntRoutingClear = 1 << (core + 4);

            /* generate an IRQ interrupt */
            PHAROS_CPU_REGISTERS->perfMonIntRoutingSet = 1 << core;
        }
        else
        {
            /* clear IRQ bit (do not generate IRQ interrupt) */
            PHAROS_CPU_REGISTERS->perfMonIntRoutingClear = 1 << core;

            /* generate an FIQ interrupt */
            PHAROS_CPU_REGISTERS->perfMonIntRoutingSet = 1 << (core + 4);
        }
    }
}


void disableInterrupt(PharosInterruptNumber number)
{
    /* get the running core */
    CoreNumber core = pharosICoreRunning();


    /* check if the interrupt is related to the timers */
    if(number >= PHAROS_CPU_ISR_CNTPS && number <= PHAROS_CPU_ISR_CNTV)
    {
        /* clear FIQ and IRQ interrupt */
        PHAROS_CPU_REGISTERS->coreTimerInterruptControl[core] &= ~((1 << number) | (1 << (number + 4)));
    }
        /* check if the interrupt is related to the mailboxes */
    else if(number >= PHAROS_CPU_ISR_MAILBOX0 && number <= PHAROS_CPU_ISR_MAILBOX3)
    {
        /* switch the number to 0...3 */
        number -= PHAROS_CPU_ISR_MAILBOX0;

        /* clear FIQ and IRQ interrupt */
        PHAROS_CPU_REGISTERS->coreMailBoxesInterruptControl[core] &= ~((1 << (number + 4)) | 1 << number);
    }
        /* check if the interrupt is related to the performance monitor */
    else if(number == PHAROS_CPU_ISR_PMU)
    {
        /* clear IRQ and FIQ bits */
        PHAROS_CPU_REGISTERS->perfMonIntRoutingClear = (1 << (core + 4)) | (1 << core);
    }
}


void pharosCpuInterruptInstall(ptrInterruptConfiguration conf)
{
    /* if a FIQ interrupt priority */
    if(conf->cpu.priority == PHAROS_CPU_INTERRUPT_FIQ)
    {
        /* then enable it on the FIQ control register */
        PHAROS_CPU_IC->fiqControl = PHAROS_CPU_IC_FIQ_EN | conf->number;
    }

    /* activate the interrupt */
    enableInterrupt(conf->number , conf);

    /* flush instruction after installing an interrupt vector */
    pharosCpuInvalidateICache();

    /* flush data cache */
    pharosCpuFlushDataCache();
}


void pharosCpuInterruptRemove(ptrInterruptConfiguration conf)
{
    /* then we can remove the interrupt since no one will handle it */

    /* only when there is no handler can de-activate the interrupt */
    disableInterrupt(conf->number);

    /* flush instruction and data after removing an interrupt vector */
    pharosCpuInvalidateICache();

    /* flush data cache */
    pharosCpuFlushDataCache();
}
