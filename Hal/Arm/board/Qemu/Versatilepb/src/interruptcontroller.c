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


#include <pharos/hal/board.h>
#include <pharos/hal/interrupt.h>
#include <pharos/hal/vicPl190.h>
#include <pharos/kernel/core/core.h>
#include <pharos/hal/cpu/sections.h>
#include <pharos/hal/board/interruptcontroller.h>
#include <pharos/kernel/assert/assert.h>
#include <pharos/hal/clockSp804.h>
#include <pharos/kernel/interrupt/interrupt.h>
#include <pharos/kernel/interrupt/interruptinline.h>


static void pharosCpuSicInterruptInstall(ptrInterruptConfiguration conf , ptrArmInterruptControllerSic sic);

static void pharosCpuSicInterruptRemove(ptrInterruptConfiguration conf , ptrArmInterruptControllerSic sic);

static void pharosInterruptHandler(uint32_t numbers , uint32_t offset , const uint8_t *interruptedAddress);


void pharosCpuInterruptControllerInitialize(ptrInterruptTable table)
{
    /* initialize the PL190 interrupt controller */
    armVicPl190Initialize(VIC_PIC_BASE_ADDRESS);

    /* reset all interrupts */
    VIC_SIC_BASE_ADDRESS->enableClear = 0xffffffffU;

    /* enable the secondary interrupts on PL190 */
    armVicPl190UnmaskInterrupt(PHAROS_INTERRUPT_NUMBER_SIC , VIC_PIC_BASE_ADDRESS);
}


void pharosCpuInterruptInstall(ptrInterruptConfiguration conf)
{
    /* if the interrupt should go to the primary interrupt controller */
    if(conf->number <= 31)
    {
        /* then use the PL190 to install it */
        pharosCpuPl190InterruptInstall(conf , VIC_PIC_BASE_ADDRESS);
    }
    else
    {
        /* otherwise use the SIC interrupt controller */
        pharosCpuSicInterruptInstall(conf , VIC_SIC_BASE_ADDRESS);
    }
}


void pharosCpuInterruptRemove(ptrInterruptConfiguration conf)
{
    /* if the interrupt should go to the primary interrupt controller */
    if(conf->number <= 31)
    {
        /* then use the PL190 to remove it */
        pharosCpuPl190InterruptRemove(conf , VIC_PIC_BASE_ADDRESS);
    }
    else
    {
        /* otherwise use the SIC interrupt controller */
        pharosCpuSicInterruptRemove(conf , VIC_SIC_BASE_ADDRESS);
    }
}


void pharosInterruptHandler(uint32_t numbers , uint32_t offset , const uint8_t *interruptedAddress)
{
    /* interrupt number executing (without offset) */
    uint32_t number;

    /* interrupt number (with offset) */
    PharosInterruptNumber isrNumber;

    /* interrupt configuration */
    ptrInterruptConfiguration conf;

    /* return of the pre-handler */
    uintptr_t preInterruptHandlerReturn;


    /* loop through all the different sources of interrupts that occurred (only if default will have more than one) */
    while(numbers != 0U)
    {
        /* get the first interrupt number to execute (numbers contains the bits of each interrupt number supposed to be executed) */
        number = armbit2Integer(numbers);

        /* calculate the ISR number given the offset (could be the PIC or SIC) */
        isrNumber = number + offset;

        /* get the interrupt configuration */
        conf = pharosIInterruptConfigurationGet(isrNumber);

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
        pharosIInterruptExecute(pharosIInterruptConfigurationGet(isrNumber) , preInterruptHandlerReturn , interruptedAddress);

        /* remove the interrupt number from the list of interrupts to execute */
        numbers &= ~(1U << number);
    }
}


void pharosArmCInterruptHandler(const uint8_t *interruptedAddress)
{
    /* get the interrupts triggered on the PIC */
    uint32_t pic = pharosCpuPl190ActiveInterupts(VIC_PIC_BASE_ADDRESS);

    /* get the interrupts triggered on the SIC */
    uint32_t sic = VIC_SIC_BASE_ADDRESS->status;

    /* interrupted partition running */
    ptrPartition oldRunning = pharosIInterruptRunningEnvGet();


    /* enable interrupts (if interrupt stack is big enough). In this CPU we don't have to clear the interrupts before
     * enabling them since they will not be triggered again once we enable the interrupts */
    pharosCpuInterruptNestedEnable();

    /* handle the interrupts of the PIC */
    pharosInterruptHandler(pic , 0U , interruptedAddress);

    /* handle the interrupts of the SIC */
    pharosInterruptHandler(sic , 32U , interruptedAddress);

    /* if the PIC activate any interrupt */
    if(pic != 0)
    {
        /* clear the active interrupts */
        pharosCpuPl190ClearActiveInterupts(VIC_PIC_BASE_ADDRESS);
    }

    /* disable interrupts */
    pharosCpuInterruptNestedDisable();
    
    /* restore the previous running interrupt partition */
    pharosIInterruptRunningEnvSet(oldRunning);
}


void pharosCpuSicInterruptInstall(ptrInterruptConfiguration conf , ptrArmInterruptControllerSic sic)
{
    sic->enableSet = 1 << (conf->number - 32U);
}


void pharosCpuSicInterruptRemove(ptrInterruptConfiguration conf , ptrArmInterruptControllerSic sic)
{
    sic->enableClear = 1 << (conf->number - 32U);
}
