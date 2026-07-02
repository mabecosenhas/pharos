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
#include <pharos/hal/cpu/conf.h>
#include <pharos/channel.h>
#include <pharos/kernel/partition/partition.h>
#include <pharos/kernel/core/core.h>
#include <pharos/kernel/thread/stack.h>
#include <pharos/hal/v7m/misc.h>
#include <pharos/hal/v7m/scb.h>
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/hal/cpu/linker.h>


void pharosMpuHandler(void)
{
    /* get the stack saved */
    ptrPharosCpuExceptionFrame ef = pharosCpuPspGet();

    /* get the running thread */
    ptrThreadInfo runningThread = pharosISchedGetRunningThread();


    /* first check if we could get the exception frame (if the thread was overloading the stack to a memory area not allowed, then 
     * it could not have possibly written to the ef nothing) */
    if(pharosIStackIsAddressInsideRunningStack(runningThread , (uint8_t *) ef) == FALSE)
    {
        /* announce the invalid memory error -> thread should be restarted or suspended, so a dispatch could be needed */
        pharosIAnnounceInvalidMemoryError(runningThread , NULL , (uint8_t *) ef);

        /* generate a pendable interrupt (it will perform the context switch) */
        PHAROS_CPU_SCB->interruptControl = PHAROS_CPU_SCB_ICSR_PENDSVSET;
    }
        /* announce an invalid memory access and handle it */
    else if(pharosIInvalidMemAccess((uint8_t *) ef->register_pc , (uint8_t *) PHAROS_CPU_SCB->memMngFaultAddress) == TRUE)
    {
        /* if the access was valid then execute the I/O direct area code */

        /* add privileges to thread mode (when we return) */
        pharosCpuControlAddPriv();

        /* use the same exception frame but replace the pc */

        /* leave to the io access when this exception ends */
        ef->register_pc = (uint16_t *) (((uint32_t) & runningThread->cpuContext.ioAccess) + 1);
    }
    else
    {
        /* dispatch to another thread */

        /* generate a pendable interrupt (it will perform the context switch) */
        PHAROS_CPU_SCB->interruptControl = PHAROS_CPU_SCB_ICSR_PENDSVSET;
    }

    /* clear the IACCVIOL and DACCVIOL memory violations */
    PHAROS_CPU_SCB->configurableFaultStatus = (PHAROS_CPU_SCB->configurableFaultStatus & 0xffU);
}


void pharosCpuMmuInitialize(void)
{
    uint32_t sharedSizeLog2;
    uint32_t sharedSize;
    uint32_t sharedStart;

    /* get the are of the shared section (only one is selected) */
    pharosCpuSharedSectionStart(&sharedStart , &sharedSize);

    /* calculate the log2 size of the shared section (minimum is 8 for 256 bytes) */
    sharedSizeLog2 = MAX(8U , pharosUint32Log2Ceil(sharedSize));

    /* set the address of the text area */
    PHAROS_CPU_MPU->regionBaseAddress = PHAROS_CPU_MPU_RBAR_ADDR((uint32_t) & pharosStartText) | PHAROS_CPU_MPU_RBAR_VALID | PHAROS_CPU_MPU_RBAR_REGION(PHAROS_CPU_MPU_REGION_TEXT);

    /* set the execute permissions and read-only memory (unless in privileged mode in which case can also write) */
    PHAROS_CPU_MPU->regionAttributeAndSize = PHAROS_CPU_MPU_RASR_XN_EN | PHAROS_CPU_MPU_RASR_AP_RONLY |
            PHAROS_CPU_MPU_RASR_TEX_NOR | PHAROS_CPU_MPU_RASR_S_NOR | PHAROS_CPU_MPU_RASR_C_NOR |
            PHAROS_CPU_MPU_RASR_B_NOR | PHAROS_CPU_MPU_RASR_SRD_EN | PHAROS_CPU_MPU_RASR_SIZE((uint32_t) & pharosCpuTextSizeLog2) | PHAROS_CPU_MPU_RASR_EN;

    /* if there is an IP call region to set */
    if(&pharosIpCallEnd - &pharosIpCallStart > 0)
    {
        /* set the address of the IP call area */
        PHAROS_CPU_MPU->regionBaseAddress = PHAROS_CPU_MPU_RBAR_ADDR((uint32_t) & pharosIpCallStart) | PHAROS_CPU_MPU_RBAR_VALID | PHAROS_CPU_MPU_RBAR_REGION(PHAROS_CPU_MPU_REGION_IPCALL);

        /* set the execute permissions and read-only for all modes */
        PHAROS_CPU_MPU->regionAttributeAndSize = PHAROS_CPU_MPU_RASR_XN_EN | PHAROS_CPU_MPU_RASR_AP_RONLY |
                PHAROS_CPU_MPU_RASR_TEX_NOR | PHAROS_CPU_MPU_RASR_S_NOR | PHAROS_CPU_MPU_RASR_C_NOR |
                PHAROS_CPU_MPU_RASR_B_NOR | PHAROS_CPU_MPU_RASR_SRD_EN | PHAROS_CPU_MPU_RASR_SIZE((uint32_t) & pharosCpuIpCallSizeLog2) | PHAROS_CPU_MPU_RASR_EN;
    }

    /* set the address of the shared area */
    if(sharedSize != 0)
    {
        PHAROS_CPU_MPU->regionBaseAddress = PHAROS_CPU_MPU_RBAR_ADDR(sharedStart) | PHAROS_CPU_MPU_RBAR_VALID | PHAROS_CPU_MPU_RBAR_REGION(PHAROS_CPU_MPU_REGION_SHARED);

        /* set the no-execute permissions and read/write for all modes */
        PHAROS_CPU_MPU->regionAttributeAndSize = PHAROS_CPU_MPU_RASR_XN_DIS | PHAROS_CPU_MPU_RASR_AP_FULL |
                PHAROS_CPU_MPU_RASR_TEX_NOR | PHAROS_CPU_MPU_RASR_S_NOR | PHAROS_CPU_MPU_RASR_C_NOR |
                PHAROS_CPU_MPU_RASR_B_NOR | PHAROS_CPU_MPU_RASR_SRD_EN | PHAROS_CPU_MPU_RASR_SIZE(sharedSizeLog2) | PHAROS_CPU_MPU_RASR_EN;
    }

    /* enable the MPU exception (and bus and usage as well) */
    PHAROS_CPU_SCB->systemHandlerControlState |= PHAROS_CPU_SHCSR_MEMFAULTENA;
    PHAROS_CPU_SCB->systemHandlerControlState |= PHAROS_CPU_SHCSR_USGFAULTENA;
    PHAROS_CPU_SCB->systemHandlerControlState |= PHAROS_CPU_SHCSR_BUSFAULTENA;

    /* enable the MPU and enable the background region so that in privileged mode we can access all the memory without having to change any MPU register */
    PHAROS_CPU_MPU->control = PHAROS_CPU_MPU_CTRL_PRIVDEF_ENA | PHAROS_CPU_MPU_CTRL_HFNMI_DIS | PHAROS_CPU_MPU_CTRL_ENABLE;
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

    /* setup the MPU for the partition data+bss+uss */

    /* reset the number of partition and IO regions the thread has */
    context->numberPartitionAndIoRegions = 0U;

    /* go through each partition memory area and while there are MPU regions to use */
    for(i = 0; i < PHAROS_NUMBER_MEMORY_AREAS && context->numberPartitionAndIoRegions < PHAROS_CPU_MPU_NBR_RGI_VAR; i++)
    {
        /* only add non-empty memory areas */
        if(memProtection->endAddress[i] != memProtection->startAddress[i])
        {
            context->tableRegions[context->numberPartitionAndIoRegions].region.base = PHAROS_CPU_MPU_RBAR_ADDR((uint32_t) memProtection->startAddress[i]) |
                    PHAROS_CPU_MPU_RBAR_VALID | PHAROS_CPU_MPU_RBAR_REGION(context->numberPartitionAndIoRegions + PHAROS_CPU_MPU_NBR_RGI_FIX);

            /* set the size and the attributes of the partition memory area to no execute, read/write permissions and cacheable and buffereable */
            context->tableRegions[context->numberPartitionAndIoRegions].region.size = PHAROS_CPU_MPU_RASR_XN_DIS | PHAROS_CPU_MPU_RASR_AP_FULL |
                    PHAROS_CPU_MPU_RASR_TEX_NOR | PHAROS_CPU_MPU_RASR_S_NOR | PHAROS_CPU_MPU_RASR_C_NOR |
                    PHAROS_CPU_MPU_RASR_B_NOR | PHAROS_CPU_MPU_RASR_SRD_EN |
                    PHAROS_CPU_MPU_RASR_SIZE(pharosUint32Log2Ceil((uint32_t) (memProtection->endAddress[i] - memProtection->startAddress[i]))) | PHAROS_CPU_MPU_RASR_EN;

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
        /* initialize the index */
        context->tableRegions[i].index = PHAROS_CPU_MPU_NBR_RGI_FIX + i;

        /* disable the MPU region */
        context->tableRegions[i].region.base = PHAROS_CPU_MPU_RBAR_INVALID |
                PHAROS_CPU_MPU_RBAR_VALID | PHAROS_CPU_MPU_RBAR_REGION(context->tableRegions[i].index);

        /* disable the region */
        context->tableRegions[i].region.size = PHAROS_CPU_MPU_RASR_DIS;

        /* add the region to the list of unused regions */
        linkedListAppendLast(&context->freeRegions , &context->tableRegions[i]);
    }

    /* go to each direct I/O memory and set it to an MPU region (but don't add too many, reserve at least 1 for channel) */
    for(i = 0U , ioArea = 0U;
        (ioArea < partition->ioTable.numberDirectAreas) &&
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
            freeRegion->region.base = PHAROS_CPU_MPU_RBAR_ADDR((uint32_t) partition->ioTable.directAreas[ioArea].start) |
                    PHAROS_CPU_MPU_RBAR_VALID | PHAROS_CPU_MPU_RBAR_REGION(freeRegion->index);

            /* set the size and attributes (no execute, give full access and place the attributes the application selected */
            freeRegion->region.size = PHAROS_CPU_MPU_RASR_XN_DIS | PHAROS_CPU_MPU_RASR_AP_FULL |
                    PHAROS_CPU_MPU_RASR_TEX(partition->ioTable.directAreas[ioArea].attributes.tex) |
                    PHAROS_CPU_MPU_RASR_S(partition->ioTable.directAreas[ioArea].attributes.s) |
                    PHAROS_CPU_MPU_RASR_C(partition->ioTable.directAreas[ioArea].attributes.c) |
                    PHAROS_CPU_MPU_RASR_B(partition->ioTable.directAreas[ioArea].attributes.b) |
                    PHAROS_CPU_MPU_RASR_SRD_EN |
                    PHAROS_CPU_MPU_RASR_SIZE(pharosUint32Log2Ceil(partition->ioTable.directAreas[ioArea].size)) |
                    PHAROS_CPU_MPU_RASR_EN;

            /* increase the number of regions used */
            i++;
        }
        /* else, the region needs supervisor attributes and therefore must be executed within a "data abort IO direct call" */
    }

    /* increment the number of I/O regions used */
    context->numberPartitionAndIoRegions += i;
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
    if(pharosCpuAddressInsideSharedSection(memAddress) == TRUE)
    {
        /* it is */
        result = TRUE;
    }
        /* check to see if it is inside the any memory area of the partition */
    else if(pharosIAddressInsideMemoryArea(&partition->memProtection , memAddress) == TRUE)
    {
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
            context->base[context->number] = PHAROS_CPU_MPU_RBAR_ADDR((uint32_t) memProtection->startAddress[i]) |
                    PHAROS_CPU_MPU_RBAR_VALID | PHAROS_CPU_MPU_RBAR_REGION(PHAROS_CPU_MPU_NBR_RGI_FIX + context->number);

            /* set the size with:
             *  - no execution permissions
             *  - read and write permissions for user mode 
             *  - normal memory attributes
             *  - all sub-regions enabled */
            context->size[context->number] = PHAROS_CPU_MPU_RASR_XN_DIS | PHAROS_CPU_MPU_RASR_AP_FULL |
                    PHAROS_CPU_MPU_RASR_TEX_NOR | PHAROS_CPU_MPU_RASR_S_NOR | PHAROS_CPU_MPU_RASR_C_NOR |
                    PHAROS_CPU_MPU_RASR_B_NOR | PHAROS_CPU_MPU_RASR_SRD_EN |
                    PHAROS_CPU_MPU_RASR_SIZE(pharosUint32Log2Ceil((uint32_t) (memProtection->endAddress[i] - memProtection->startAddress[i]))) | PHAROS_CPU_MPU_RASR_EN;

            /* increment the number of MPU regions */
            context->number++;
        }
    }
}


void pharosCpuMpuSave(ptrThreadInfo thread)
{
    /* region iterator */
    uint32_t i;

    /* interrupt level */
    PharosInterruptLevel level;

    /* get the MPU context of the partition in which the thread is running in */
    ptrPharosCpuMpuPartitionContext mpu = &thread->runningPartition->cpuInfo.partitionMpuContext;


    /* disable interrupts to prevent the regions to change (region number specially) */
    pharosCpuDisableInterrupts(&level);

    /* save the shared stack MPU region */
    PHAROS_CPU_MPU->regionNumber = PHAROS_CPU_MPU_REGION_STACK;

    /* get the base address from the MPUBASE register */
    thread->cpuContext.stackMpuRegion.base = PHAROS_CPU_MPU->regionBaseAddress;

    /* get the attributes from the MPUATTR register */
    thread->cpuContext.stackMpuRegion.size = PHAROS_CPU_MPU->regionAttributeAndSize;

    /* save every MPU region (except for the fixed MPU regions).
     * NOTE: this is actually needed when the clock is very high (see Test650 or Test652) */
    for(i = 0U; i < PHAROS_CPU_MPU_NBR_RGI_VAR; i++)
    {
        /* set the region number */
        PHAROS_CPU_MPU->regionNumber = i + PHAROS_CPU_MPU_NBR_RGI_FIX;

        /* save the base */
        mpu->tableRegions[i].region.base = PHAROS_CPU_MPU->regionBaseAddress;

        /* save the size */
        mpu->tableRegions[i].region.size = PHAROS_CPU_MPU->regionAttributeAndSize;
    }

    /* re-enable temporarily the interrupts */
    pharosCpuEnableInterrupts(&level);
}


void pharosCpuMpuRestore(ptrThreadInfo thread)
{
    /* region iterator */
    uint32_t i;

    /* interrupt level */
    PharosInterruptLevel level;

    /* get the MPU context of the partition in which the thread is running in */
    ptrPharosCpuMpuPartitionContext mpu = &thread->runningPartition->cpuInfo.partitionMpuContext;


    /* disable interrupts to prevent the regions to change (region number specially) */
    pharosCpuDisableInterrupts(&level);

    /* set the region and base address */
    PHAROS_CPU_MPU->regionBaseAddress = thread->cpuContext.stackMpuRegion.base | PHAROS_CPU_MPU_RBAR_VALID;

    /* set the region size and attributes */
    PHAROS_CPU_MPU->regionAttributeAndSize = thread->cpuContext.stackMpuRegion.size;

    /* restore every MPU region (except for the fixed MPU regions) */
    for(i = 0U; i < PHAROS_CPU_MPU_NBR_RGI_VAR; i++)
    {
        /* if the MPU region is enabled */
        if((mpu->tableRegions[i].region.size & PHAROS_CPU_MPU_RASR_EN) != 0)
        {
            /* set the base address (and enable the region) */
            PHAROS_CPU_MPU->regionBaseAddress = mpu->tableRegions[i].region.base | PHAROS_CPU_MPU_RBAR_VALID;

            /* set the region size and attributes */
            PHAROS_CPU_MPU->regionAttributeAndSize = mpu->tableRegions[i].region.size;
        }
        else
        {
            /* set the changeable region */
            PHAROS_CPU_MPU->regionNumber = i + PHAROS_CPU_MPU_NBR_RGI_FIX;

            /* disable the region */
            PHAROS_CPU_MPU->regionAttributeAndSize = PHAROS_CPU_MPU_RASR_DIS;
        }
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

    /* change the partition MPU regions */
    for(i = 0; i < PHAROS_CPU_MPU_NBR_RGI_VAR; i++)
    {
        /* set the base address with the DRBAR register */
        PHAROS_CPU_MPU->regionBaseAddress = newContext->tableRegions[i].region.base | PHAROS_CPU_MPU_RBAR_VALID;

        /* attributes were already set, no need to set them again */

        /* set the size and enable status with the DRSR register */
        PHAROS_CPU_MPU->regionAttributeAndSize = newContext->tableRegions[i].region.size;
    }

    /* re-enable interrupts */
    pharosCpuEnableInterrupts(&level);
}


void pharosCpuAddSharedStack(ptrThreadInfo thread , ptrPartitionCpuMmuContext context)
{
    /* interrupt level */
    PharosInterruptLevel level;

    /* get the shared stack to switch */
    ptrStack stack = &thread->sharedAccessStack->stack;


    /* check if the thread is the running thread */
    PHAROS_ASSERT(thread == pharosISchedGetRunningThread() , "Not called with running thread");

    /* don't need to save the shared stack to the runningThread->context.mpu. We can directly write it to the 
     * MPU registers and when a context switch is performed, they will be saved to the runningThread->context.mpu */

    /* disable interrupts to prevent the regions to change (RGNR register) */
    pharosCpuDisableInterrupts(&level);

    /* set the address of the stack area */
    PHAROS_CPU_MPU->regionBaseAddress = PHAROS_CPU_MPU_RBAR_ADDR((uint32_t) stack->stack) |
            PHAROS_CPU_MPU_RBAR_VALID | PHAROS_CPU_MPU_RBAR_REGION(PHAROS_CPU_MPU_REGION_STACK);

    /* set the no-execute permissions and read/write for all modes */
    PHAROS_CPU_MPU->regionAttributeAndSize = PHAROS_CPU_MPU_RASR_XN_DIS | PHAROS_CPU_MPU_RASR_AP_FULL |
            PHAROS_CPU_MPU_RASR_TEX_NOR | PHAROS_CPU_MPU_RASR_S_NOR | PHAROS_CPU_MPU_RASR_C_NOR |
            PHAROS_CPU_MPU_RASR_B_NOR | PHAROS_CPU_MPU_RASR_SRD_EN |
            PHAROS_CPU_MPU_RASR_SIZE(pharosUint32Log2Ceil(stack->size)) | PHAROS_CPU_MPU_RASR_EN;

    /* re-enable interrupts */
    pharosCpuEnableInterrupts(&level);
}


void pharosCpuRemoveSharedStack(ptrThreadInfo thread , ptrPartitionCpuMmuContext context)
{
    /* interrupt level */
    PharosInterruptLevel level;

    /* get the application stack to restore */
    ptrStack stack = &thread->applicationStack;


    /* disable interrupts to prevent the regions to change (RGNR register) */
    pharosCpuDisableInterrupts(&level);

    /* set the address of the stack area */
    PHAROS_CPU_MPU->regionBaseAddress = PHAROS_CPU_MPU_RBAR_ADDR((uint32_t) stack->stack) |
            PHAROS_CPU_MPU_RBAR_VALID | PHAROS_CPU_MPU_RBAR_REGION(PHAROS_CPU_MPU_REGION_STACK);

    /* set the no-execute permissions and read/write for all modes */
    PHAROS_CPU_MPU->regionAttributeAndSize = PHAROS_CPU_MPU_RASR_XN_DIS | PHAROS_CPU_MPU_RASR_AP_FULL |
            PHAROS_CPU_MPU_RASR_TEX_NOR | PHAROS_CPU_MPU_RASR_S_NOR | PHAROS_CPU_MPU_RASR_C_NOR |
            PHAROS_CPU_MPU_RASR_B_NOR | PHAROS_CPU_MPU_RASR_SRD_EN |
            PHAROS_CPU_MPU_RASR_SIZE(pharosUint32Log2Ceil(stack->size)) | PHAROS_CPU_MPU_RASR_EN;

    /* re-enable interrupts */
    pharosCpuEnableInterrupts(&level);
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

        /* if the thread that is running is on the same partition */
        if(pharosISchedGetRunningThread()->runningPartition == partition)
        {
            /* then update the MPU region */

            /* set the address of the bulk area */
            PHAROS_CPU_MPU->regionBaseAddress = PHAROS_CPU_MPU_RBAR_ADDR((uint32_t) bulk->bulkFullBuffer.buffer) |
                    PHAROS_CPU_MPU_RBAR_VALID | PHAROS_CPU_MPU_RBAR_REGION(freeRegion->index);

            /* set the no-execute permissions and read/write for all modes */
            PHAROS_CPU_MPU->regionAttributeAndSize = PHAROS_CPU_MPU_RASR_XN_DIS | PHAROS_CPU_MPU_RASR_AP_FULL |
                    PHAROS_CPU_MPU_RASR_TEX_NOR | PHAROS_CPU_MPU_RASR_S_NOR | PHAROS_CPU_MPU_RASR_C_NOR |
                    PHAROS_CPU_MPU_RASR_B_NOR | PHAROS_CPU_MPU_RASR_SRD_EN |
                    PHAROS_CPU_MPU_RASR_SIZE(pharosUint32Log2Ceil(queue->cpuInfo.size)) | PHAROS_CPU_MPU_RASR_EN;
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


    /* disable interrupts to prevent the regions to change (region number) and the channel bulk list */
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
            PHAROS_CPU_MPU->regionNumber = bulk->cpuInfo.region->index;

            /* disable the MPU for the region */
            PHAROS_CPU_MPU->regionAttributeAndSize = PHAROS_CPU_MPU_RASR_DIS;
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


uint8_t *pharosCpuChannelBulkQueueInit(ptrChannelBulkQueue queue , uint32_t * bulkSize)
{
    /* set the size of each bulk */
    *bulkSize = queue->cpuInfo.size;

    /* return the address of the first page */
    return queue->start;
}
