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


#include <pharos/hal/memory/mpuV7.h>
#include <pharos/hal/performance/performance.h>
#include <pharos/hal/memory/pmsa.h>
#include <pharos/hal/boardApi.h>
#include <pharos/kernel/partition/declarations.h>
#include <pharos/kernel/partition/partition.h>
#include <pharos/kernel/core/core.h>
#include <pharos/channel.h>
#include <pharos/kernel/core/coreconfinline.h>


extern uint8_t pharosCpuTextSizeLog2;
extern uint8_t pharosCpuSharedSizeLog2;
extern uint8_t pharosCpuIpCallSizeLog2;


void pharosCpuMmuInitialize(void)
{
    /* setup the text region */

    /* set the first region */
    pharosCpuPmsaRgnrSet(PHAROS_CPU_PMSA_RGNR_REGION(PHAROS_CPU_MPU_REGION_TEXT));

    /* set the address of the text area */
    pharosCpuPmsaDrbarSet(PHAROS_CPU_PMSA_DRBAR_ADDR((uint32_t) & pharosStartText));

    /* set the execute permissions and read-only memory (unless in privileged mode in which case can also write) */
    pharosCpuPmsaDracrSet(PHAROS_CPU_PMSA_DRACR_EX_EN | PHAROS_CPU_PMSA_DRACR_AP_RONLY | PHAROS_CPU_PMSA_DRACR_TEX_NOR |
                          PHAROS_CPU_PMSA_DRACR_S_NOR | PHAROS_CPU_PMSA_DRACR_C_NOR | PHAROS_CPU_PMSA_DRACR_B_NOR);

    /* set the size of the text region for the first region and enable it (and enable all subregions) */
    pharosCpuPmsaDrsrSet(PHAROS_CPU_PMSA_DRSR_SR_EN_ALL | PHAROS_CPU_PMSA_DRSR_EN | PHAROS_CPU_PMSA_DRSR_SIZE((uint32_t) & pharosCpuTextSizeLog2));

    /* setup the shared section region */

    /* set the region */
    pharosCpuPmsaRgnrSet(PHAROS_CPU_PMSA_RGNR_REGION(PHAROS_CPU_MPU_REGION_SHARED));

    /* set the address of the text area */
    pharosCpuPmsaDrbarSet(PHAROS_CPU_PMSA_DRBAR_ADDR((uint32_t) & pharosSharedStart));

    /* set the no-execute permissions and read/write for all modes */
    pharosCpuPmsaDracrSet(PHAROS_CPU_PMSA_DRACR_EX_DIS | PHAROS_CPU_PMSA_DRACR_AP_FULL | PHAROS_CPU_PMSA_DRACR_TEX_NOR |
                          PHAROS_CPU_PMSA_DRACR_S_NOR | PHAROS_CPU_PMSA_DRACR_C_NOR | PHAROS_CPU_PMSA_DRACR_B_NOR);

    /* set the size of the shared region and enable it (and enable all subregions) */
    pharosCpuPmsaDrsrSet(PHAROS_CPU_PMSA_DRSR_SR_EN_ALL | PHAROS_CPU_PMSA_DRSR_EN | PHAROS_CPU_PMSA_DRSR_SIZE((uint32_t) & pharosCpuSharedSizeLog2));

    /* setup the IP calls region */

    /* if there is an IP call region to set */
    if(pharosIpCallEnd - pharosIpCallStart > 0)
    {
        /* set the third  region for the IP-calls */
        pharosCpuPmsaRgnrSet(PHAROS_CPU_PMSA_RGNR_REGION(PHAROS_CPU_MPU_REGION_IPCALL));

        /* set the address of the IP calls area */
        pharosCpuPmsaDrbarSet(PHAROS_CPU_PMSA_DRBAR_ADDR((uint32_t) & pharosIpCallStart));

        /* set the execute permissions and read-only for all modes */
        pharosCpuPmsaDracrSet(PHAROS_CPU_PMSA_DRACR_EX_EN | PHAROS_CPU_PMSA_DRACR_AP_RONLY | PHAROS_CPU_PMSA_DRACR_TEX_NOR |
                              PHAROS_CPU_PMSA_DRACR_S_NOR | PHAROS_CPU_PMSA_DRACR_C_NOR | PHAROS_CPU_PMSA_DRACR_B_NOR);

        /* set the size of the IP calls region and enable it (and enable all subregions) */
        pharosCpuPmsaDrsrSet(PHAROS_CPU_PMSA_DRSR_SR_EN_ALL | PHAROS_CPU_PMSA_DRSR_EN | PHAROS_CPU_PMSA_DRSR_SIZE((uint32_t) & pharosCpuIpCallSizeLog2));
    }

    /* set the region for the shared stack */
    pharosCpuPmsaRgnrSet(PHAROS_CPU_PMSA_RGNR_REGION(PHAROS_CPU_MPU_REGION_SHR_STACK));

    /* set the no-execute permissions and read/write for all modes */
    pharosCpuPmsaDracrSet(PHAROS_CPU_PMSA_DRACR_EX_DIS | PHAROS_CPU_PMSA_DRACR_AP_FULL | PHAROS_CPU_PMSA_DRACR_TEX_NOR |
                          PHAROS_CPU_PMSA_DRACR_S_NOR | PHAROS_CPU_PMSA_DRACR_C_NOR | PHAROS_CPU_PMSA_DRACR_B_NOR);

    /* divide by zero should NOT raise exception (divide by 0 returns 0) */
    pharosCpuPmsaSctlrSet((pharosCpuPmsaSctlrGet() & PHAROS_CPU_PMSA_SCTRL_DZ_MASK));

    /* enable the MPU and enable the background region so that in privileged mode we can access all the memory without having to change any MPU register */
    pharosCpuPmsaSctlrSet(pharosCpuPmsaSctlrGet() | PHAROS_CPU_PMSA_SCTRL_BR_EN | PHAROS_CPU_PMSA_SCTRL_MPU_EN);
}


void pharosPmsrBusEventExportEnable(void)
{
    register uint32_t pmcr = armPmcrGet();
    pmcr |= 0x10U;
    armPmcrSet(pmcr);
}


void pharosCpuVicInitialize(void)
{
    /* enable vectored FIQ and IRQ interrupt handling (will use the VIM TABLE instead of the standard interrupt table) */
    pharosCpuPmsaSctlrSet(pharosCpuPmsaSctlrGet() | PHAROS_CPU_PMSA_SCTRL_VE_EN);
}


void pharosCpuPartitionInitialize(ptrPartition partition)
{
    uint32_t i;

    uint32_t ioArea = 0;

    ptrPharosCpuMpuRegion freeRegion;

    /* get the MPU context */
    ptrPharosCpuMpuPartitionContext context = &partition->cpuInfo.partitionMpuContext;

    /* get the memory protection of the partition */
    ptrEnvMemoryProtection memProtection = pharosIPartitionMemoryProt(partition);

    /* initialize the simple list with all the bulks the partition has access to */
    linkedListInitialize(&partition->cpuInfo.channelBulks , OFFSETOF(ChannelBulkKernel , cpuInfo.node));

    /* initialize the list of free regions */
    linkedListInitialize(&context->freeRegions , OFFSETOF(PharosCpuMpuRegion , node));

    /* initialize the list of used regions */
    linkedListInitialize(&context->usedRegions , OFFSETOF(PharosCpuMpuRegion , node));

    /* setup the MPU for the partition data+bss (region 3 of the MPU) */

    /* reset the number of partition and IO regions the thread has */
    context->numberPartitionAndIoRegions = 0U;

    /* mark the shared stack as invalid */
    context->sharedStackRegion.region.size = PHAROS_CPU_PMSA_DRSR_DIS;

    /* for each MPU region */
    for(i = 0; i < PHAROS_NUMBER_MEMORY_AREAS && context->numberPartitionAndIoRegions < PHAROS_CPU_MPU_NBR_RGI_VAR; i++)
    {
        /* only add non-empty memory areas */
        if(memProtection->endAddress[i] != memProtection->startAddress[i])
        {

            /* set the start address */
            context->tableRegions[context->numberPartitionAndIoRegions].region.base = PHAROS_CPU_PMSA_DRBAR_ADDR((uint32_t) memProtection->startAddress[i]);

            /* set the size */
            context->tableRegions[context->numberPartitionAndIoRegions].region.size = PHAROS_CPU_PMSA_DRSR_SR_EN_ALL | PHAROS_CPU_PMSA_DRSR_EN |
                    PHAROS_CPU_PMSA_DRSR_SIZE(pharosUint32Log2Ceil((uint32_t) (memProtection->endAddress[i] - memProtection->startAddress[i])));


            /* set the attributes of the partition memory area to no execute, read/write permissions and cacheable and buffereable */
            context->tableRegions[context->numberPartitionAndIoRegions].region.attributes = PHAROS_CPU_PMSA_DRACR_EX_DIS | PHAROS_CPU_PMSA_DRACR_AP_FULL |
                    PHAROS_CPU_PMSA_DRACR_TEX_NOR |
                    PHAROS_CPU_PMSA_DRACR_S_NOR | PHAROS_CPU_PMSA_DRACR_C_NOR | PHAROS_CPU_PMSA_DRACR_B_NOR;

            /* initialize the index */
            context->tableRegions[context->numberPartitionAndIoRegions].index = context->numberPartitionAndIoRegions + PHAROS_CPU_MPU_NBR_RGI_FIX;

            /* jump to next region */
            context->numberPartitionAndIoRegions++;
        }
        /* else, the memory area is empty so we won't initialize it for this thread */
    }

    /* for every other region */
    for(i = context->numberPartitionAndIoRegions; i < PHAROS_CPU_MPU_NBR_RGI_VAR; i++)
    {
        /* initialize the number of the MPU region */
        context->tableRegions[i].index = PHAROS_CPU_MPU_NBR_RGI_FIX + i;

        /* disable the MPU region */
        context->tableRegions[i].region.size = PHAROS_CPU_PMSA_DRSR_DIS;

        /* add the region to the list of unused regions */
        linkedListAppendLast(&context->freeRegions , &context->tableRegions[i]);
    }

    /* if not on the kernel (idle thread) */
    if(partition != NULL)
    {
        /* go to each direct I/O memory and set it to an MPU region (but don't add too many, reserve at least 2 for channels) */
        for(i = 0U , ioArea = 0U;
            ioArea < partition->ioTable.numberDirectAreas &&
            (i <= PHAROS_CPU_MPU_NBR_RGI_VAR - context->numberPartitionAndIoRegions - 1);
            ioArea++)
        {
            /* if the IO region does not need supervisor attributes */
            if(partition->ioTable.directAreas[ioArea].attributes.supervisor == FALSE)
            {
                /* get a free region to be used for the I/O direct memory access */
                freeRegion = linkedListRemoveFirst(&context->freeRegions);

                /* don't add the I/O mem region to the used list. It will always be fixed to give the user some performance guarantees of what the system is doing */

                /* set the start address of the direct I/O area */
                freeRegion->region.base = PHAROS_CPU_PMSA_DRBAR_ADDR((uint32_t) partition->ioTable.directAreas[ioArea].start);

                /* set the attributes (no execute, give full access and place the attributes the application selected */
                freeRegion->region.attributes = PHAROS_CPU_PMSA_DRACR_EX_DIS | PHAROS_CPU_PMSA_DRACR_AP_FULL |
                        PHAROS_CPU_PMSA_DRACR_TEX(partition->ioTable.directAreas[ioArea].attributes.tex) |
                        PHAROS_CPU_PMSA_DRACR_S(partition->ioTable.directAreas[ioArea].attributes.s) |
                        PHAROS_CPU_PMSA_DRACR_C(partition->ioTable.directAreas[ioArea].attributes.c) |
                        PHAROS_CPU_PMSA_DRACR_B(partition->ioTable.directAreas[ioArea].attributes.b);

                /* set the size */
                freeRegion->region.size = PHAROS_CPU_PMSA_DRSR_SR_EN_ALL | PHAROS_CPU_PMSA_DRSR_EN |
                        PHAROS_CPU_PMSA_DRSR_SIZE(pharosUint32Log2Ceil(partition->ioTable.directAreas[ioArea].size));

                /* increase the number of regions used */
                i++;
            }
            /* else, the region needs supervisor attributes and therefore must be executed within a "data abort IO direct call" */
        }
    }


}


bool pharosCpuIoMemAddressIsValid(const ptrPartition partition , uint8_t *memAddress)
{
    /* result of the method */
    bool result = FALSE;

    /* get the I/O table */
    const ptrIoMemAreaTable table = &partition->ioTable;

    /* iterator through the direct areas. The MMU areas are not needed to search since they will trigger an exception */
    uint32_t i;

    /* check if the address is inside the shared data */
    if((&pharosSharedStart <= memAddress) && (&pharosSharedEnd >= memAddress))
    {
        /* it is */
        result = TRUE;
    }
        /* check if the address is on any direct io memory area */
    else
    {
        /* go through all direct areas */
        for(i = 0U; i < table->numberDirectAreas; i++)
        {
            /* and check if the address is present in one of them */
            if((memAddress >= table->directAreas[i].start) && (memAddress <= table->directAreas[i].start + table->directAreas[i].size))
            {
                /* it is present */
                result = TRUE;

                /* stop searching */
                break;
            }
        }

        /* iterate through the list of channels hold by the partition */
        ptrChannelBulkKernel iterator = (ptrChannelBulkKernel) linkedListGetFirst(&partition->cpuInfo.channelBulks);

        /* while there are bulks hold by the partition continue to search */
        while(iterator != NULL)
        {
            /* check if the address is inside the channel bulk buffer 
             * (unlike MMU, here we can limit the search to the buffer, instead of the whole MMU page) */
            if((iterator->bufferMessage.buffer <= memAddress) &&
               (iterator->bufferMessage.buffer + iterator->bufferMessage.size >= memAddress))
            {
                /* it is */
                result = TRUE;

                /* stop searching */
                break;
            }

            /* search next channel bulk */
            iterator = (ptrChannelBulkKernel) linkedListNext(&partition->cpuInfo.channelBulks , iterator);
        }


    }

    /* return TRUE if found and FALSE otherwise */
    return result;
}


void pharosCpuPartitionMmuInit(ptrPartitionCpuMmuContext context , ptrEnvMemoryProtection memProtection)
{
    /* memory area iterator */
    uint32_t i;


    /* reset the number of MPU regions */
    context->number = 0;

    /* go through every MPU region */
    for(i = 0; i < PHAROS_NUMBER_MEMORY_AREAS && i < PHAROS_CPU_MPU_NBR_RGI_VAR; i++)
    {
        /* only add non-empty memory areas */
        if(memProtection->endAddress[i] != memProtection->startAddress[i])
        {
            /* set the start address */
            context->base[context->number] = PHAROS_CPU_PMSA_DRBAR_ADDR((uint32_t) memProtection->startAddress[i]);

            /* set the size */
            context->size[context->number] = PHAROS_CPU_PMSA_DRSR_SR_EN_ALL | PHAROS_CPU_PMSA_DRSR_EN |
                    PHAROS_CPU_PMSA_DRSR_SIZE(pharosUint32Log2Ceil((uint32_t) (memProtection->endAddress[i] - memProtection->startAddress[i])));

            /* increment the number of MPU regions */
            context->number++;
        }
    }
}


void pharosCpuMpuRestore(ptrThreadInfo thread)
{
    /* region iterator */
    uint32_t i;

    /* interrupt level */
    PharosInterruptLevel level;

    /* get the MPU context of the partition in which the thread is running in */
    ptrPharosCpuMpuPartitionContext mpu = &thread->runningPartition->cpuInfo.partitionMpuContext;


    /* disable interrupts to prevent the regions to change (RGNR register) */
    pharosCpuDisableInterrupts(&level);

    /* if the running thread has a shared stack */
    if((thread->cpuContext.sharedStackMpuRegion.size & PHAROS_CPU_PMSA_DRSR_EN) != 0)
    {
        /* set the shared stack region */
        pharosCpuPmsaRgnrSet(PHAROS_CPU_PMSA_RGNR_REGION(PHAROS_CPU_MPU_REGION_SHR_STACK));

        /* set the base address with the DRBAR register */
        pharosCpuPmsaDrbarSet(thread->cpuContext.sharedStackMpuRegion.base);

        /* set the attributes with the DRACR register */
        pharosCpuPmsaDracrSet(thread->cpuContext.sharedStackMpuRegion.attributes);

        /* set the size and enable status with the DRSR register */
        pharosCpuPmsaDrsrSet(thread->cpuContext.sharedStackMpuRegion.size);
    }
        /* otherwise disable it */
    else
    {
        /* set the shared stack region */
        pharosCpuPmsaRgnrSet(PHAROS_CPU_PMSA_RGNR_REGION(PHAROS_CPU_MPU_REGION_SHR_STACK));

        /* disable the memory region */
        pharosCpuPmsaDrsrSet(PHAROS_CPU_PMSA_DRSR_DIS);
    }

    /* restore every MPU region (except for the fixed regions) */
    for(i = 0U; i < PHAROS_CPU_MPU_NBR_RGI_VAR; i++)
    {
        /* set the first changeable region  (regions 0 - 2 are set always to text and shared data and IP calls) */
        pharosCpuPmsaRgnrSet(PHAROS_CPU_PMSA_RGNR_REGION(i + PHAROS_CPU_MPU_NBR_RGI_FIX));

        /* set the base address with the DRBAR register */
        pharosCpuPmsaDrbarSet(mpu->tableRegions[i].region.base);

        /* set the attributes with the DRACR register */
        pharosCpuPmsaDracrSet(mpu->tableRegions[i].region.attributes);

        /* set the size and enable status with the DRSR register */
        pharosCpuPmsaDrsrSet(mpu->tableRegions[i].region.size);
    }

    /* re-enable temporarily the interrupts */
    pharosCpuEnableInterrupts(&level);
}


void pharosCpuMpuSwitchPartition(ptrPharosCpuMpuPartitionContext newContext)
{
    uint32_t i;

    /* interrupt level */
    PharosInterruptLevel level;


    /* disable interrupts to prevent the regions to change (RGNR register) */
    pharosCpuDisableInterrupts(&level);

    /* restore remaining (except for shared stack) */
    for(i = 0U; i < PHAROS_CPU_MPU_NBR_RGI_VAR; i++)
    {
        /* set the first changeable region (region 0 is for text and 1 is for shared data) */
        pharosCpuPmsaRgnrSet(PHAROS_CPU_PMSA_RGNR_REGION(i + PHAROS_CPU_MPU_NBR_RGI_FIX));

        /* set the base address with the DRBAR register */
        pharosCpuPmsaDrbarSet(newContext->tableRegions[i].region.base);

        /* set the attributes with the DRACR register */
        pharosCpuPmsaDracrSet(newContext->tableRegions[i].region.attributes);

        /* set the size and enable status with the DRSR register */
        pharosCpuPmsaDrsrSet(newContext->tableRegions[i].region.size);
    }

    /* re-enable interrupts */
    pharosCpuEnableInterrupts(&level);
}


void pharosCpuAddSharedStack(ptrThreadInfo thread , ptrPartitionCpuMmuContext context)
{
    /* interrupt level */
    PharosInterruptLevel level;


    /* disable interrupts to prevent the regions to change (RGNR register) */
    pharosCpuDisableInterrupts(&level);

    /* set the first changeable region (region 0 is for text and 1 is for shared data) */
    pharosCpuPmsaRgnrSet(PHAROS_CPU_PMSA_RGNR_REGION(PHAROS_CPU_MPU_REGION_SHR_STACK));

    /* set the base address with the DRBAR register */
    pharosCpuPmsaDrbarSet((uint32_t) thread->sharedAccessStack->stack.stack);

    /* attributes were already set, no need to set them again */

    /* set the size and enable status with the DRSR register */
    pharosCpuPmsaDrsrSet(PHAROS_CPU_PMSA_DRSR_SR_EN_ALL | PHAROS_CPU_PMSA_DRSR_EN | PHAROS_CPU_PMSA_DRSR_SIZE(pharosUint32Log2Ceil(thread->sharedAccessStack->stack.size)));

    /* re-enable interrupts */
    pharosCpuEnableInterrupts(&level);
}

void pharosCpuRemoveSharedStack(ptrThreadInfo thread , ptrPartitionCpuMmuContext context)
{
    /* interrupt level */
    PharosInterruptLevel level;

    /* disable interrupts to prevent the regions to change (RGNR register) */
    pharosCpuDisableInterrupts(&level);

    /* set the first changeable region (region 0 is for text and 1 is for shared data) */
    pharosCpuPmsaRgnrSet(PHAROS_CPU_PMSA_RGNR_REGION(PHAROS_CPU_MPU_REGION_SHR_STACK));

    /* disable the MPU for the region */
    pharosCpuPmsaDrsrSet(PHAROS_CPU_PMSA_DRSR_DIS);

    /* re-enable interrupts */
    pharosCpuEnableInterrupts(&level);
}


void armCDataAbortHandler(uint8_t * instruction_address , uint8_t * mem_address)
{
    /* announce an invalid memory access and handle it */
    pharosIInvalidMemAccess(instruction_address , mem_address);
}


uint32_t pharosCpuChannelBulkAdd(ptrChannelBulkQueue queue , ptrChannelBulkKernel bulk , ptrPartition partition)
{
    /* interrupt level */
    PharosInterruptLevel level;

    ptrPharosCpuMpuRegion freeRegion;

    /* get the MPU context of the partition */
    ptrPharosCpuMpuPartitionContext context = &partition->cpuInfo.partitionMpuContext;


    /* disable interrupts to prevent the regions to change (RGNR register) */
    pharosCpuDisableInterrupts(&level);

    /* add the bulk to the list of bulks the partition has access to */
    linkedListAppendLast(&partition->cpuInfo.channelBulks , bulk);

    /* if possible, add the channel bulk to a region */
    freeRegion = linkedListRemoveFirst(&context->freeRegions);

    /* if there is a free region that we can assign to the region */
    if(freeRegion != NULL)
    {
        /* use the free region to give permissions to the channel */

        /* mark the region as used */
        linkedListAppendFirst(&context->usedRegions , freeRegion);

        /* set to the region number */
        pharosCpuPmsaRgnrSet(PHAROS_CPU_PMSA_RGNR_REGION(freeRegion->index));

        /* if the thread that is running is on the same partition */
        if(pharosISchedGetRunningThread()->runningPartition == partition)
        {
            /* then update the MPU region */

            /* set the base address with the DRBAR register with the address of the bulk */
            pharosCpuPmsaDrbarSet((uint32_t) bulk->bulkFullBuffer.buffer);

            /* set as "normal" memory attributes */
            pharosCpuPmsaDracrSet(PHAROS_CPU_PMSA_DRACR_EX_DIS | PHAROS_CPU_PMSA_DRACR_AP_FULL |
                                  PHAROS_CPU_PMSA_DRACR_TEX_NOR |
                                  PHAROS_CPU_PMSA_DRACR_S_NOR | PHAROS_CPU_PMSA_DRACR_C_NOR | PHAROS_CPU_PMSA_DRACR_B_NOR);

            /* set the size and enable status with the DRSR register */
            pharosCpuPmsaDrsrSet(PHAROS_CPU_PMSA_DRSR_SR_EN_ALL | PHAROS_CPU_PMSA_DRSR_EN | PHAROS_CPU_PMSA_DRSR_SIZE(pharosUint32Log2Ceil(queue->cpuInfo.size)));
        }

        /* make the bulk point to the region associated with it */
        bulk->cpuInfo.region = freeRegion;
    }
    else
    {
        /* reset the region associated with the bulk */
        bulk->cpuInfo.region = NULL;
    }

    /* re-enable interrupts */
    pharosCpuEnableInterrupts(&level);

    /* return the number of bytes of the channel bulk */
    return queue->cpuInfo.size;
}


void pharosCpuChannelBulkRemove(ptrChannelBulkKernel bulk , ptrPartition partition)
{
    /* interrupt level */
    PharosInterruptLevel level;

    /* get the MPU context of the partition */
    ptrPharosCpuMpuPartitionContext context = &partition->cpuInfo.partitionMpuContext;


    /* disable interrupts to prevent the regions to change (RGNR register) and the channel bulk list */
    pharosCpuDisableInterrupts(&level);

    /* remove the bulk from the list of bulks that the partition has access to */
    linkedListRemoveObject(&partition->cpuInfo.channelBulks , bulk);

    /* if there is a region guarding the channel */
    if(bulk->cpuInfo.region != NULL)
    {
        /* then remove the region from guarding it */

        /* if the thread that is running is on the same partition */
        if(pharosISchedGetRunningThread()->runningPartition == partition)
        {
            /* set to the region number */
            pharosCpuPmsaRgnrSet(PHAROS_CPU_PMSA_RGNR_REGION(bulk->cpuInfo.region->index));

            /* disable the MPU for the region */
            pharosCpuPmsaDrsrSet(PHAROS_CPU_PMSA_DRSR_DIS);
        }

        /* remove it from the list of used regions */
        linkedListRemoveObject(&context->usedRegions , bulk->cpuInfo.region);

        /* and return it to the list of free regions to be used in the future */
        linkedListAppendFirst(&context->freeRegions , bulk->cpuInfo.region);
    }
    /* else, nothing to do on the regions */


    /* re-enable interrupts */
    pharosCpuEnableInterrupts(&level);
}


uint8_t *pharosCpuChannelBulkQueueInit(ptrChannelBulkQueue queue , uint32_t *bulkSize)
{
    /* set the size of each bulk */
    *bulkSize = queue->cpuInfo.size;

    /* return the address of the first page */
    return queue->start;
}
