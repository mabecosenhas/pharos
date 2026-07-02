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
#include <pharos/kernel/interrupt/interrupt.h>
#include <pharos/error.h>
#include <pharos/kernel/thread/thread.h>
#include <pharos/kernel/partition/partition.h>
#include <pharos/kernel/thread/thread.h>
#include <pharos/hal/boardApi.h>
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/hal/cpu/sections.h>
#include <pharos/kernel/core/core.h>


/**
 * Get the partition interrupt configuration 
 * 
 * @param partition partition
 * @param index index on the partition interrupt configuration of the partition
 * 
 * @return returns the partition interrupt configuration or NULL if index is invalid
 */
static ptrPartitionIsrConf getPartitionInterruptConf(ptrPartition partition , uint32_t index);


/**
 * Get the interrupt configuration of the partition core and partition interrupt configuration
 * 
 * @param partition partition to get the core from
 * @param conf partition interrupt configuration
 * 
 * @return returns the interrupt configuration or NULL if invalid
 */
static ptrInterruptConfiguration getInterruptConfiguration(ptrPartition partition , const ptrPartitionIsrConf conf);


/**
 * Installs the interrupt 
 * 
 * @param scheduler scheduler where the interrupt is installed to
 * @param partConf interrupt partition configuration 
 * @param conf interrupt core configuration
 */
void pharosIInterruptInstall(ptrFlatScheduler scheduler , ptrPartitionIsrConf partConf);


/**
 * Removes the interrupt 
 * 
 * @constraint it is assumed that interrupts cannot be installed while this method is running
 * 
 * @param scheduler scheduler where the interrupt is removed from
 * @param partConf interrupt partition configuration 
 * @param conf interrupt core configuration
 */
void pharosIInterruptRemove(ptrFlatScheduler scheduler , ptrPartitionIsrConf partConf , ptrInterruptConfiguration conf);


void pharosIInterruptInstallInternal(ptrPartitionIsrConf partConf)
{
    /* get the interrupt configuration from the table */
    ptrInterruptConfiguration conf = pharosICore()->pharosVIsrTable[partConf->interruptConf->number];


    /* check if the interrupt configuration is valid */
    PHAROS_ASSERT(conf != NULL , "invalid interrupt configuration for interrupt %d" , partConf->interruptNumber);

    /* check if the partition has a handler */
    PHAROS_ASSERT(partConf->isrHandler != NULL , "Invalid isr handler");

    /* save new interrupt handler */
    linkedListAppendLast(&conf->partitionInterrupts , partConf);

    /* reset the partition */
    partConf->partition = NULL;

    /* check if the interrupt is not already installed */
    if(conf->isInstalled == FALSE)
    {
        /* set it to be installed */
        conf->isInstalled = TRUE;

        /* install the interrupt on the CPU */
        pharosCpuInterruptInstall(conf);
    }
}


void pharosIInterruptInstall(ptrFlatScheduler scheduler , ptrPartitionIsrConf partConf)
{
    /* interrupt level */
    PharosInterruptLevel interruptLevel;

    /* interrupt configuration */
    ptrInterruptConfiguration conf;


    /* check the ISR handler is valid */
    PHAROS_ASSERT(partConf->isrHandler != NULL , "Invalid isr handler");

    /* update to installed */
    partConf->flags.isInstalled = TRUE;

    /* don't allow anyone inside threads or interrupts to install an interrupt at the same time */
    pharosCpuDisableInterrupts(&interruptLevel);

    /* get the interrupt configuration */
    conf = pharosICore()->pharosVIsrTable[partConf->interruptNumber];

    /* make sure there is a valid configuration */
    PHAROS_ASSERT(conf != NULL , "interrupt configuration (vector = %d) is invalid" , conf->number);

    /* add the ISR to the list of scheduler interrupts */
    linkedListAppendLast(&scheduler->interruptsInstalled , partConf);

    /* save new interrupt handler */
    linkedListAppendLast(&conf->partitionInterrupts , partConf);

    /* if the interrupt is not installed */
    if(conf->isInstalled == FALSE)
    {
        /* set it to be installed */
        conf->isInstalled = TRUE;

        /* install the interrupt on the CPU */
        pharosCpuInterruptInstall(conf);
    }
    /* else, the interrupt is already installed so no need to do anything else 
     * (already added to the list of handlers to execute) */

    /* interrupts can now be enabled */
    pharosCpuEnableInterrupts(&interruptLevel);
}


void pharosIInterruptRemoveInternal(ptrPartitionIsrConf partConf)
{
    /* get the interrupt configuration */
    ptrInterruptConfiguration conf = pharosICore()->pharosVIsrTable[partConf->interruptNumber];


    /* check if the interrupt configuration is valid */
    PHAROS_ASSERT(conf != NULL , "invalid interrupt configuration for interrupt %d" , partConf->interruptNumber);

    /* remove the interrupt handler */
    linkedListRemoveObject(&conf->partitionInterrupts , partConf);

    /* if there are no more handlers for the same interrupt */
    if(linkedListIsEmpty(&conf->partitionInterrupts) == TRUE)
    {
        /* remove the interrupt on the CPU */
        pharosCpuInterruptRemove(conf);

        /* interrupt is no longer installed */
        conf->isInstalled = FALSE;
    }
}


void pharosIInterruptRemove(ptrFlatScheduler scheduler , ptrPartitionIsrConf partConf , ptrInterruptConfiguration conf)
{
    /* check if there is an already installed interrupt configuration */
    PHAROS_ASSERT(pharosICore()->pharosVIsrTable[conf->number] != NULL ,
                  "No interrupt configuration installed at vector %d" , conf->number);

    /* update to not installed */
    partConf->flags.isInstalled = FALSE;

    /* then use it */
    conf = pharosICore()->pharosVIsrTable[conf->number];

    /* remove the interrupt from the list of installed interrupts */
    linkedListRemoveObject(&scheduler->interruptsInstalled , partConf);

    /* remove the interrupt handler */
    linkedListRemoveObject(&conf->partitionInterrupts , partConf);

    /* if there are no more handlers for the same interrupt */
    if(linkedListIsEmpty(&conf->partitionInterrupts) == TRUE)
    {
        /* remove the interrupt on the CPU */
        pharosCpuInterruptRemove(conf);

        /* interrupt is no longer installed */
        conf->isInstalled = FALSE;
    }
}


void pharosIInterruptExecute(ptrInterruptConfiguration conf , uintptr_t preInterruptHandlerReturn ,
                             const uint8_t *interruptedAddress)
{
    /* iterator of the interrupt handler for each partition */
    ptrPartitionIsrConf iterator;

    /* interrupt level */
    PharosInterruptLevel interruptLevel;

    /* address where the running core interrupt partition is */
    ptrPartition *runningInterruptPartition = &pharosICore()->pharosVRunningInterruptEnv;


    /* don't allow anyone inside threads or interrupts to install an interrupt at the same time */
    pharosCpuDisableInterrupts(&interruptLevel);

    /* get the first element of the list */
    iterator = linkedListFirst(&conf->partitionInterrupts);

    /* interrupts can now be enabled */
    pharosCpuEnableInterrupts(&interruptLevel);

    /* execute all nodes of the list */
    while(iterator != NULL)
    {
        /* update the pointer to the running interrupt environment */
        *runningInterruptPartition = iterator->partition;

        /* check if the ISR handler is valid */
        PHAROS_ASSERT(iterator->isrHandler != NULL , "Invalid isr handler");

        /* only execute the interrupt if the partition is in the "normal" state (or there is no partition) */
        if((iterator->partition == NULL) || (iterator->partition->state == PHAROS_PARTITION_STATE_NOMINAL))
        {
            /* call the application routine */
            iterator->isrHandler(iterator->argument , preInterruptHandlerReturn , conf->number , interruptedAddress);
        }

        /* else, ignore the interrupt */

        /* don't allow anyone inside threads or interrupts to install an interrupt at the same time */
        pharosCpuDisableInterrupts(&interruptLevel);

        /* get the next iterator */
        iterator = linkedListNext(&conf->partitionInterrupts , iterator);

        /* interrupts can now be enabled */
        pharosCpuEnableInterrupts(&interruptLevel);
    }

    /* if there is a routine to execute after the interrupt is processed */
    if(conf->postInterruptHandler != NULL)
    {
        /* clear the interrupt using the clear routine */
        conf->postInterruptHandler(conf->argument , preInterruptHandlerReturn);
    }
}


void pharosIInterruptRemoveAll(ptrPartition partition , const ptrPartitionIsrConfTable table)
{
    /* iterator on the table of partition ISRs */
    uint32_t i;

    /* partition ISR configuration */
    ptrPartitionIsrConf c;

    /* interrupt level */
    PharosInterruptLevel interruptLevel;


    /* go through all the ISRs defined by the partition */
    for(i = 0U; i < table->size; i++)
    {
        /* get the partition ISR configuration */
        c = &table->configurations[i];

        /* reset the flag */
        c->flags.savedIsInstalled = FALSE;

        /* don't allow anyone inside threads or interrupts to install an interrupt at the same time */
        pharosCpuDisableInterrupts(&interruptLevel);

        /* if the interrupt is currently installed */
        if(c->flags.isInstalled == TRUE)
        {
            /* then remove it */
            pharosIInterruptRemove(partition->scheduler , c , c->interruptConf);
        }

        /* interrupts can now be enabled */
        pharosCpuEnableInterrupts(&interruptLevel);
    }
}


void pharosIInterruptInit(ptrCore core)
{
    /* iterator on the table of partition ISRs */
    uint32_t i;

    /* interrupt configuration iterator */
    ptrInterruptConfiguration iterator;


    /* for each interrupt available on the CPU */
    for(i = 0U; i < PHAROS_NUMBER_INTERRUPTS; i++)
    {
        /* reset the interrupt routine */
        core->pharosVIsrTable[i] = NULL;
    }

    /* for each interrupt configured */
    for(i = 0; i < core->interrupts.size; i++)
    {
        /* get the interrupt configuration to install */
        iterator = &core->interrupts.interrupts[i];

        /* check the interrupt number is valid */
        PHAROS_ASSERT(iterator->number < PHAROS_NUMBER_INTERRUPTS , "Incorrect interrupt to install %d, max = %d" ,
                      iterator->number , PHAROS_NUMBER_INTERRUPTS);

        /* set the interrupt configuration */
        core->pharosVIsrTable[iterator->number] = iterator;

        /* initialize the linked list that holds all interrupts needed to be executed for each interrupt number */
        linkedListInitialize(&iterator->partitionInterrupts , OFFSETOF(PartitionIsrConf , node));

        /* set it to not be installed */
        iterator->isInstalled = FALSE;
    }
}


void pharosIInterruptInstallInit(ptrInterruptConfiguration conf , ptrPartitionIsrConf partConf)
{
    /* initialize the linked list that holds all interrupts needed to be executed for each interrupt number */
    linkedListInitialize(&conf->partitionInterrupts , OFFSETOF(PartitionIsrConf , node));

    /* replace any application interrupt configuration (or set a new one) */
    pharosICore()->pharosVIsrTable[conf->number] = conf;

    /* set the partition interrupt configuration */
    partConf->interruptConf = conf;

    /* install the interrupt */
    pharosIInterruptInstallInternal(partConf);
}


void pharosIInterruptPartitionInitialize(ptrPartition partition , const ptrPartitionIsrConfTable table)
{
    /* iterator on the table of partition ISRs */
    uint32_t i;

    /* interrupt configuration */
    ptrInterruptConfiguration interruptConf;

    /* partition ISR configuration */
    ptrPartitionIsrConf c;


    /* go through all the ISRs defined by the partition */
    for(i = 0U; i < table->size; i++)
    {
        /* get the partition ISR configuration */
        c = &table->configurations[i];

        /* set the partition */
        c->partition = partition;

        /* get the configuration */
        interruptConf = getInterruptConfiguration(partition , c);

        /* set the cache for the configuration */
        c->interruptConf = interruptConf;

        /* reset the flag */
        c->flags.savedIsInstalled = FALSE;

        /* cache for the interrupt number */
        c->interruptNumber = interruptConf->number;

        /* check to see if the interrupt should be installed at startup */
        if((c->flags.isInstalledStartup == TRUE) && (interruptConf != NULL))
        {
            /* catch the interrupt with its associated configuration */
            pharosIInterruptInstall(partition->scheduler , c);
        }
    }

}


void pharosIAnnounceInvalidStackError(ptrThreadInfo running , uint8_t *interruptedInstruction , uint8_t *memAddress)
{
    /* announce the invalid stack error */
    pharosSThreadErrorAnnounce(running , PHAROS_THREAD_ERROR_INVALID_STACK_ACCESS ,
                               interruptedInstruction , memAddress);

    /* stop the running thread */
    pharosISchedRemoveReadyThread(running);
}


void pharosIAnnounceInvalidMemoryError(ptrThreadInfo running , uint8_t *interruptedInstruction , uint8_t *memAddress)
{
    /* announce the invalid memory error */
    pharosSThreadErrorAnnounce(running , PHAROS_THREAD_ERROR_INVALID_MEM_ACCESS ,
                               interruptedInstruction , memAddress);

    /* stop the running thread */
    pharosISchedRemoveReadyThread(running);
}


bool pharosIInvalidMemAccess(uint8_t *interruptedInstruction , uint8_t *memAddress)
{
    /* return value (TRUE if the mem access is valid and FALSE otherwise) */
    bool result;

    /* get the running thread that caused this invalid memory access (interrupts do not cause the exception since
     * they run with all permissions) */
    ptrThreadInfo running = pharosISchedGetRunningThread();

    /* get the running partition */
    ptrPartition p = running->runningPartition;


    /* lets check if the memAddress is actually valid */
    if(pharosCpuIoMemAddressIsValid(p , memAddress) == TRUE)
    {
        /* mem access is valid */
        result = TRUE;

        /* then tell the CPU to execute the address (somehow) */
        pharosCpuExecuteMemAddress(&running->cpuContext , interruptedInstruction);
    }
        /* else, the address is not a valid IO mem address */
    else
    {
        /* mem access is invalid */
        result = FALSE;

        /* announce the invalid memory error */
        pharosIAnnounceInvalidMemoryError(running , interruptedInstruction , memAddress);
    }

    /* returns TRUE if the access is valid and FALSE otherwise */
    return result;
}


void pharosIIllegalInstruction(uint8_t *interruptedInstruction)
{
    /* get the running thread that caused this invalid memory access */
    ptrThreadInfo running = pharosISchedGetRunningThread();


    /* announce invalid memory error occurred */
    pharosSErrorAnnounce(PHAROS_ERR_ILLEGAL_INSTR , interruptedInstruction);

    /* stop the running thread */
    pharosISchedRemoveReadyThread(running);
}


void pharosIInterruptMemNotAligned(uint8_t *interruptedInstruction)
{
    /* get the running thread that caused this invalid memory access */
    ptrThreadInfo running = pharosISchedGetRunningThread();


    /* announce unaligned memory access occurred */
    pharosSErrorAnnounce(PHAROS_ERR_UNALIGNED_MEM_ACCESS , interruptedInstruction);

    /* stop the running thread */
    pharosISchedRemoveReadyThread(running);
}


PharosInterruptInstallR pharosSInterruptInstall(uint32_t index , void *argument)
{
    /* return value of this method*/
    PharosInterruptInstallR result;

    /* get the running partition */
    ptrPartition partition = pharosIPartitionGetRunning();

    /* get the partition interrupt configuration */
    ptrPartitionIsrConf conf = getPartitionInterruptConf(partition , index);

    /* interrupt configuration */
    ptrInterruptConfiguration interruptConf;

    /* interrupt level */
    PharosInterruptLevel interruptLevel;


    /* don't allow anyone inside threads or interrupts to install an interrupt at the same time */
    pharosCpuDisableInterrupts(&interruptLevel);

    /* if the interrupt configuration is invalid */
    if(conf == NULL)
    {
        /* interrupts can now be enabled */
        pharosCpuEnableInterrupts(&interruptLevel);

        /* return invalid index (no such configuration under the running partition) */
        result = PHAROS_INTERRUPT_INSTALL_INVALID_INDEX;
    }
        /* check if the partition handler is already installed */
    else if(conf->flags.isInstalled == TRUE)
    {
        /* interrupts can now be enabled */
        pharosCpuEnableInterrupts(&interruptLevel);

        /* return already installed */
        result = PHAROS_INTERRUPT_INSTALL_ALREADY_INSTALLED;
    }
    else
    {
        /* get the interrupt configuration */
        interruptConf = getInterruptConfiguration(partition , conf);

        /* if there is no interrupt configuration */
        if(interruptConf == NULL)
        {
            /* interrupts can now be enabled */
            pharosCpuEnableInterrupts(&interruptLevel);

            /* return the interrupt is incorrectly installed */
            result = PHAROS_INTERRUPT_INSTALL_INTERRUPT_INVALID;
        }
        else
        {
            /* copy the argument */
            conf->argument = argument;

            /* catch the interrupt with its associated configuration */
            pharosIInterruptInstall(partition->scheduler , conf);

            /* interrupts can now be enabled */
            pharosCpuEnableInterrupts(&interruptLevel);

            /* return success */
            result = PHAROS_INTERRUPT_INSTALL_SUCCESS;
        }
    }

    /* return success or the error */
    return result;
}


PharosInterruptRemoveR pharosSInterruptRemove(uint32_t index)
{
    /* return value of this method*/
    PharosInterruptRemoveR result;

    /* get the running partition */
    ptrPartition partition = pharosIPartitionGetRunning();

    /* get the interrupt configuration */
    ptrPartitionIsrConf conf = getPartitionInterruptConf(partition , index);

    /* interrupt configuration */
    ptrInterruptConfiguration interruptConf;

    /* interrupt level */
    PharosInterruptLevel interruptLevel;


    /* don't allow anyone inside threads or interrupts to install an interrupt at the same time */
    pharosCpuDisableInterrupts(&interruptLevel);

    /* if the interrupt configuration is invalid */
    if(conf == NULL)
    {
        /* interrupts can now be enabled */
        pharosCpuEnableInterrupts(&interruptLevel);

        /* return invalid index (no such configuration under the running partition) */
        result = PHAROS_INTERRUPT_REMOVE_INVALID_INDEX;
    }
        /* check if the handler is not installed */
    else if(conf->flags.isInstalled == FALSE)
    {
        /* interrupts can now be enabled */
        pharosCpuEnableInterrupts(&interruptLevel);

        /* return not installed */
        result = PHAROS_INTERRUPT_REMOVE_NOT_INSTALLED;
    }
    else
    {
        /* get the interrupt configuration */
        interruptConf = getInterruptConfiguration(partition , conf);

        /* if could not find the interrupt configuration */
        if(interruptConf == NULL)
        {
            /* interrupts can now be enabled */
            pharosCpuEnableInterrupts(&interruptLevel);

            /* return the interrupt is incorrectly installed */
            result = PHAROS_INTERRUPT_INSTALL_INTERRUPT_INVALID;
        }
        else
        {
            /* remove the interrupt with its associated configuration */
            pharosIInterruptRemove(partition->scheduler , conf , interruptConf);

            /* interrupts can now be enabled */
            pharosCpuEnableInterrupts(&interruptLevel);

            /* return success */
            result = PHAROS_INTERRUPT_INSTALL_SUCCESS;
        }
    }

    /* return success or the error */
    return result;
}


ptrPartitionIsrConf getPartitionInterruptConf(ptrPartition partition , uint32_t index)
{
    /* result of this method */
    ptrPartitionIsrConf result;


    /* if the index is valid */
    if(index < partition->interruptConfigurationTable.size)
    {
        /* return the configuration with the selected index */
        result = &partition->interruptConfigurationTable.configurations[index];
    }
    else
    {
        /* return error */
        result = NULL;
    }

    /* returns the selected configuration or NULL if does not exist */
    return result;
}


ptrInterruptConfiguration getInterruptConfiguration(ptrPartition partition , const ptrPartitionIsrConf conf)
{
    /* result of this method */
    ptrInterruptConfiguration result;


    /* if the index is valid */
    if(conf->number < partition->core->interrupts.size)
    {
        /* return the configuration with the selected index */
        result = &partition->core->interrupts.interrupts[conf->number];
    }
    else
    {
        /* return error */
        result = NULL;
    }

    /* returns the selected configuration or NULL if does not exist */
    return result;
}
