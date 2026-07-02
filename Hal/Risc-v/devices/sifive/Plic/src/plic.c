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


#include <pharos/hal/cpu/plic.h>
#include <pharos/hal/cpu/conf.h>
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/hal/cpu/timer.h>
#include <pharos/hal/cpu/clint.h>
#include <pharos/hal/cpu/sections.h>
#include <pharos/kernel/interrupt/interruptinline.h>


#define PHAROS_CPU_PLIC       ((ptrPharosCpuPlic) PHAROS_CPU_PLIC_ADDRESS)


/**
 * Enables the interrupt
 * 
 * @param number interrupt number
 * @param conf interrupt configuration
 */
static void pharosCpuEnableInterrupt(PharosInterruptNumber number , ptrInterruptConfiguration conf);


/**
 * Disables the interrupt 
 * 
 * @param number interrupt number
 */
static void pharosCpuDisableInterrupt(PharosInterruptNumber number);


/**
 * Set the priority of an interrupt 
 * 
 * @param number interrupt number
 * @param priority priority to set
 */
static void pharosCpuInterruptSetPriority(PharosInterruptNumber number , PharosInterruptPriority priority);


/**
 * Set the priority threshold of the running core (hart)
 * 
 * @param priority priority to set
 */
static void pharosCpuInterruptSetPriorityThreshold(PharosInterruptPriority priority);


/**
 * C interrupt handler. Called from the assembly interrupt handler 
 * 
 * @param mcause
 * @param interruptedAddress address that was being executed when the interrupt occurred 
 * 
 * @return returns the pointer to the running core to make the assembly interrupt handler life easier
 */
ptrPharosRmpCore pharosCpuCInterruptHandler(uint64_t mcause , const uint8_t *interruptedAddress);


void pharosCpuIcInitialize(ptrInterruptTable table)
{
    /* interrupt iterator */
    uint32_t i;


    /* for every interrupt configured to be installed */
    for(i = 0U; i < table->size; i++)
    {
        /* set the priority */
        pharosCpuInterruptSetPriority(table->interrupts[i].number , table->interrupts[i].cpu.priority);
    }

    /* set the priority threshold to the minimum value -> enable all interrupts with priority larger than 0 */
    pharosCpuInterruptSetPriorityThreshold(0U);
}


void pharosCpuInterruptInstall(ptrInterruptConfiguration conf)
{
    /* activate the interrupt */
    pharosCpuEnableInterrupt(conf->number , conf);
}


void pharosCpuInterruptRemove(ptrInterruptConfiguration conf)
{
    /* only when there is no handler can de-activate the interrupt */
    pharosCpuDisableInterrupt(conf->number);
}


ptrPharosRmpCore pharosCpuCInterruptHandler(uint64_t mcause , const uint8_t *interruptedAddress)
{
    /* get the highest priority interrupt on the running core and claim it */
    uint32_t claim;

    /* interrupt configuration */
    ptrInterruptConfiguration conf;

    /* return of the pre-handler */
    uintptr_t preInterruptHandlerReturn;

    /* running core */
    CoreNumber core = pharosICoreRunning();

    /* running core in the interrupt perspective */
    CoreNumber interruptCore = pharosCpuInterruptCoreCorrect(core);

    /* interrupted partition running */
    ptrPartition oldRunning = pharosIInterruptRunningEnvGet();


    /* increase the interrupt nest level */
    pharosVRmpCore[core].interruptLevel++;

    /* increment dispatch level */
    pharosVRmpCore[core].dispatchLevel++;

    /* if the cause is the interrupt timer */
    if(mcause == PHAROS_CPU_MCAUSE_TIMER)
    {
        /* process the timer interrupt */
        pharosCpuMtimerIsr(interruptedAddress);
    }
        /* check if it is an external interrupt */
    else if(mcause == PHAROS_CPU_MCAUSE_EXT)
    {
        /* process the highest priority interrupt */
        claim = PHAROS_CPU_PLIC->aux[interruptCore].claimComplete;

        /* get all interrupts until there are no more to process */
        while(claim != 0)
        {
            
            /* get the interrupt configuration */
            conf = pharosIInterruptConfigurationGet(claim);
            
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

            /* call the application routines */
            pharosIInterruptExecute(conf , preInterruptHandlerReturn , interruptedAddress);

            /* write to claim to complete the interrupt processing */
            PHAROS_CPU_PLIC->aux[interruptCore].claimComplete = claim;

            /* read for any more interrupts */
            claim = PHAROS_CPU_PLIC->aux[interruptCore].claimComplete;
        }
    }
        /* must be a software interrupt */
    else
    {
        /* clear the interrupt 
         * (before the interrupt handler is called, this way we won't loose any message being sent) */
        pharosCpuMulticoreClearInterrupt(core);

        /* now we can enable interrupts since the source that triggered the interrupt has been dealt with */
        pharosCpuInterruptNestedEnable();

        /* call the multi-core interrupt handler */
        pharosIMulticoreInterruptHandler();
    }

    /* disable interrupts */
    pharosCpuInterruptNestedDisable();

    /* decrement dispatch level */
    pharosVRmpCore[core].dispatchLevel--;

    /* decrease the nest level */
    pharosVRmpCore[core].interruptLevel--;

    /* restore the previous running interrupt partition */
    pharosIInterruptRunningEnvSet(oldRunning);

    /* returns the pointer to the running core to make the assembly interrupt handler life easier */
    return &pharosVRmpCore[core];
}


void pharosCpuInterruptSetPriorityThreshold(PharosInterruptPriority priority)
{
    /* get the running core */
    CoreNumber core = pharosCpuInterruptCoreCorrect(pharosICoreRunning());

    /* auxiliary variable that contains the 32-bit address to change 
     * (requires another variable to force the compiler to access the word in a 32-bit instead of 8-bits plus 16 
     * or whatever optimization the compiler wants to make - even in O0) */
    uint32_t *aux = (uint32_t *) & PHAROS_CPU_PLIC->aux[core].priorityThreshold;

    
    /* update the priority */
    *aux = priority;
}


void pharosCpuInterruptSetPriority(PharosInterruptNumber number , PharosInterruptPriority priority)
{
    /* auxiliary variable that contains the 32-bit address to change 
     * (requires another variable to force the compiler to access the word in a 32-bit instead of 8-bits plus 16 
     * or whatever optimization the compiler wants to make - even in O0) 
     * (decrease one unit because interrupt source 0 does not exist and on the PLIC is marked with reserved) */
    uint32_t *aux = (uint32_t *) & PHAROS_CPU_PLIC->priorities[number - 1U];


    /* update the priority */
    *aux = priority;
}


void pharosCpuEnableInterrupt(PharosInterruptNumber number , ptrInterruptConfiguration conf)
{
    /* get the running core */
    CoreNumber core = pharosCpuInterruptCoreCorrect(pharosICoreRunning());

    /* auxiliary variable that contains the 32-bit address to change 
     * (requires another variable to force the compiler to access the word in a 32-bit instead of 8-bits plus 16 
     * or whatever optimization the compiler wants to make - even in O0) */
    uint32_t *aux = (uint32_t *) & PHAROS_CPU_PLIC->interruptEnable[core][number / 32U];


    /* set the bit to enable the interrupt */
    *aux |= 1U << (number % 32U);
}


void pharosCpuDisableInterrupt(PharosInterruptNumber number)
{
    /* get the running core */
    CoreNumber core = pharosCpuInterruptCoreCorrect(pharosICoreRunning());

    /* auxiliary variable that contains the 32-bit address to change 
     * (requires another variable to force the compiler to access the word in a 32-bit instead of 8-bits plus 16 
     * or whatever optimization the compiler wants to make - even in O0) */
    uint32_t *aux = (uint32_t *) & PHAROS_CPU_PLIC->interruptEnable[core][number / 32U];


    /* clear the bit to disable the interrupt */
    *aux &= ~(1U << (number % 32U));
}


bool pharosCpuInterruptIsPending(PharosInterruptNumber number)
{
    /* auxiliary variable that contains the 32-bit address to change 
     * (requires another variable to force the compiler to access the word in a 32-bit instead of 8-bits plus 16 
     * or whatever optimization the compiler wants to make - even in O0) */
    uint32_t *aux = (uint32_t *) & PHAROS_CPU_PLIC->pending[number / 32U];


    /* check if the bit is set (then the interrupt is pending) */
    return (*aux & (1U << (number % 32U))) != 0 ? TRUE : FALSE;
}
