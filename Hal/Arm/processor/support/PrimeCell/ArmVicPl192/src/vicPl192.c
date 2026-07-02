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


#include <vicPl192.h>
#include <pharos/hal/halapi.h>
#include <pharos/kernel/interrupt/interrupt.h>
#include <pharos/hal/boardApi.h>

#define VIC_MAXIMUM_NUMBER_INTERRUPTS   (32U)

KERNEL_SECTION ptrArmVicPl192 pharosVVic = NULL;

/**
 * Table of application ISRs per priority
 */
static KERNEL_SECTION LinkedList pharosVIsrTable[VIC_MAXIMUM_NUMBER_INTERRUPTS];


static KERNEL_SECTION ptrPartition pharosVRunningInterruptEnv = NULL;


void armVicPl192Initialize(ptrArmVicPl192 address , ptrInterruptTable table)
{
    uint32_t i;

    /* set the address */
    pharosVVic = address;

    /* for each interrupt */
    for(i = 0U; i < VIC_MAXIMUM_NUMBER_INTERRUPTS; i++)
    {
        /* initialize the linked list that holds all interrupt handlers needed to be executed */
        linkedListInitialize(&pharosVIsrTable[i] , OFFSETOF(PartitionIsrConf , node));
    }

    /* for every interrupt */
    for(i = 0U; i < table->size; i++)
    {
        /* set it to not installed */
        table->interrupts[i].isInstalled = FALSE;

        /* set the priority of the interrupt */
        pharosVVic->vectorPriorityRegisters[table->interrupts[i].number] = table->interrupts[i].priority;
    }

    /* force all interrupts to be IRQ (not FIQ) */
    pharosVVic->interruptSelect = 0;
}


void armVicPl192UnmaskInterrupt(PharosInterruptNumber number)
{
    /* unmask the corresponding interrupt vector number */
    pharosVVic->interruptEnable = 1U << number;
}


void armVicPl192MaskInterrupt(PharosInterruptNumber number)
{
    /* mask the corresponding interrupt vector number */
    pharosVVic->interruptEnableClear = 1U << number;
}


void pharosCpuInterruptInstall(const ptrPartition partition , ptrPartitionIsrConf partConf , ptrInterruptConfiguration conf)
{
    /* interrupt level */
    PharosInterruptLevel interruptLevel;


    /* don't allow anyone inside threads or interrupts to change the interrupt handler table */
    pharosCpuDisableInterrupts(&interruptLevel);

    /* save new interrupt handler */
    linkedListAppendLast(&pharosVIsrTable[conf->number] , partConf);

    /* cache for the interrupt number */
    partConf->interruptNumber = conf->number;

    /* if the interrupt is not installed */
    if(conf->isInstalled == FALSE)
    {
        /* install the vector address with the priority 
         * because the control register only allows one interrupt number, each priority can only be associated with one interrupt number */
        pharosVVic->vectorAddressRegisters[conf->number] = conf->number;

        /* set it to be installed */
        conf->isInstalled = TRUE;

        /* flush instruction cache after installing an interrupt vector */
        pharosCpuInvalidateICache();

        /* flush data cache */
        pharosCpuFlushDataCache();

        /* activate the interrupt */
        armVicPl192UnmaskInterrupt(conf->number);
    }
    /* else, the interrupt is already installed so no need to do anything else (already added to the list of handlers to execute) */

    /* interrupts can now be enabled */
    pharosCpuEnableInterrupts(&interruptLevel);
}


void pharosCpuInterruptRemove(const ptrPartition partition , ptrPartitionIsrConf partConf , ptrInterruptConfiguration conf)
{
    /* interrupt level */
    PharosInterruptLevel interruptLevel;


    /* don't allow anyone inside threads or interrupts to change the interrupt handler table */
    pharosCpuDisableInterrupts(&interruptLevel);

    /* remove the interrupt handler */
    linkedListRemoveObject(&pharosVIsrTable[conf->number] , partConf);

    /* check if the interrupt number is installed */
    if(conf->isInstalled == TRUE)
    {
        /* if there are no more handlers for the same interrupt */
        if(linkedListIsEmpty(&pharosVIsrTable[conf->number]) == TRUE)
        {
            /* then we can remove the interrupt since no one will handle it */

            /* only when there is no handler can de-activate the interrupt */
            armVicPl192MaskInterrupt(conf->number);

            /* interrupt is no longer installed */
            conf->isInstalled = FALSE;

            /* flush instruction and data after removing an interrupt vector */
            pharosCpuInvalidateICache();

            /* flush data cache */
            pharosCpuFlushDataCache();
        }
    }

    /* interrupts can now be enabled */
    pharosCpuEnableInterrupts(&interruptLevel);
}


void pharosArmCInterruptHandler(const uint8_t *interruptedAddress , uint32_t numbers)
{
    /* interrupt number executing */
    uint32_t number;

    /* list of interrupt configurations for the interrupt number */
    ptrLinkedList list;

    /* iterator of the interrupt configurations for the interrupt number */
    ptrPartitionIsrConf iterator;

    /* interrupt configuration */
    ptrInterruptConfiguration conf;

    /* save the interrupted running partition */
    ptrPartition oldRunning = pharosVRunningInterruptEnv;


    /* loop through all the different sources of interrupts that occurred (only if default will have more than one) */
    while(numbers != 0U)
    {
        /* get the first interrupt number to execute (numbers contains the bits of each interrupt number supposed to be executed) */
        number = armbit2Integer(numbers);

        /* get the linked list containing the interrupt configurations to execute */
        list = &pharosVIsrTable[number];

        /* get the first element of the list */
        iterator = linkedListFirst(list);

        /* get the interrupt configuration so that we can clear it later */
        conf = iterator->interruptConf;

        /* execute all nodes of the list */
        while(iterator != NULL)
        {
            /* update the pointer to the running interrupt environment */
            pharosVRunningInterruptEnv = iterator->partition;

            /* call the application routine */
            iterator->isrHandler(iterator->interruptNumber , interruptedAddress);

            /* get the next iterator */
            iterator = linkedListNext(list , iterator);
        }

        /* if there is a routine to clear the interrupt */
        if(conf->clearInterrupt != NULL)
        {
            /* clear the interrupt using the clear routine */
            conf->clearInterrupt();
        }

        /* remove the interrupt number from the list of interrupts to execute */
        numbers &= ~(1U << number);
    }

    /* reset the pointer to the running interrupt environment */
    pharosVRunningInterruptEnv = oldRunning;
}
