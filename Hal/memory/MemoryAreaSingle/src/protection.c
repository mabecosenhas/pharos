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


#include <pharos/kernel/object/declarations.h>
#include <pharos/kernel/object/protection.h>
#include <pharos/hal/boardApi.h>
#include <pharos/hal/cpu/linker.h>
#include <pharos/channel.h>
#include <pharos/kernel/interrupt/interrupt.h>
#include <pharos/kernel/partition/partition.h>
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/kernel/core/coreinlines.h>
#include <pharos/hal/cpu/sections.h>
#include <pharos/kernel/channel/channelbulkkernel.h>


RONLY_SECTION static const uintptr_t partitionsDataSize[] = {
    (uintptr_t) & partition0DataSize ,
    (uintptr_t) & partition1DataSize ,
    (uintptr_t) & partition2DataSize ,
    (uintptr_t) & partition3DataSize ,
    (uintptr_t) & partition4DataSize ,
    (uintptr_t) & partition5DataSize ,
    (uintptr_t) & partition6DataSize ,
    (uintptr_t) & partition7DataSize ,
    (uintptr_t) & partition8DataSize ,
    (uintptr_t) & partition9DataSize ,
    (uintptr_t) & partition10DataSize ,
    (uintptr_t) & partition11DataSize ,
    (uintptr_t) & partition12DataSize ,
    (uintptr_t) & partition13DataSize ,
    (uintptr_t) & partition14DataSize ,
    (uintptr_t) & partition15DataSize ,
    (uintptr_t) & partition16DataSize ,
    (uintptr_t) & partition17DataSize ,
    (uintptr_t) & partition18DataSize ,
    (uintptr_t) & partition19DataSize ,
    (uintptr_t) & partition20DataSize ,
    (uintptr_t) & partition21DataSize ,
    (uintptr_t) & partition22DataSize ,
    (uintptr_t) & partition23DataSize ,
    (uintptr_t) & partition24DataSize ,
    (uintptr_t) & partition25DataSize ,
    (uintptr_t) & partition26DataSize ,
    (uintptr_t) & partition27DataSize ,
    (uintptr_t) & partition28DataSize ,
    (uintptr_t) & partition29DataSize ,
    (uintptr_t) & partition30DataSize ,
    (uintptr_t) & partition31DataSize ,
    (uintptr_t) & partition32DataSize ,
    (uintptr_t) & partition33DataSize ,
    (uintptr_t) & partition34DataSize ,
    (uintptr_t) & partition35DataSize ,
    (uintptr_t) & partition36DataSize ,
    (uintptr_t) & partition37DataSize ,
    (uintptr_t) & partition38DataSize ,
    (uintptr_t) & partition39DataSize ,
    (uintptr_t) & partition40DataSize ,
    (uintptr_t) & partition41DataSize ,
    (uintptr_t) & partition42DataSize ,
    (uintptr_t) & partition43DataSize ,
    (uintptr_t) & partition44DataSize ,
    (uintptr_t) & partition45DataSize ,
    (uintptr_t) & partition46DataSize ,
    (uintptr_t) & partition47DataSize ,
    (uintptr_t) & partition48DataSize ,
    (uintptr_t) & partition49DataSize ,
    (uintptr_t) & partition50DataSize ,
    (uintptr_t) & partition51DataSize ,
    (uintptr_t) & partition52DataSize ,
    (uintptr_t) & partition53DataSize ,
    (uintptr_t) & partition54DataSize ,
    (uintptr_t) & partition55DataSize ,
    (uintptr_t) & partition56DataSize ,
    (uintptr_t) & partition57DataSize ,
    (uintptr_t) & partition58DataSize ,
    (uintptr_t) & partition59DataSize ,
    (uintptr_t) & partition60DataSize ,
    (uintptr_t) & partition61DataSize ,
    (uintptr_t) & partition62DataSize ,
    (uintptr_t) & partition63DataSize ,
};


RONLY_SECTION static const uintptr_t partitionsBssSize[] = {
    (uintptr_t) & partition0BssSize ,
    (uintptr_t) & partition1BssSize ,
    (uintptr_t) & partition2BssSize ,
    (uintptr_t) & partition3BssSize ,
    (uintptr_t) & partition4BssSize ,
    (uintptr_t) & partition5BssSize ,
    (uintptr_t) & partition6BssSize ,
    (uintptr_t) & partition7BssSize ,
    (uintptr_t) & partition8BssSize ,
    (uintptr_t) & partition9BssSize ,
    (uintptr_t) & partition10BssSize ,
    (uintptr_t) & partition11BssSize ,
    (uintptr_t) & partition12BssSize ,
    (uintptr_t) & partition13BssSize ,
    (uintptr_t) & partition14BssSize ,
    (uintptr_t) & partition15BssSize ,
    (uintptr_t) & partition16BssSize ,
    (uintptr_t) & partition17BssSize ,
    (uintptr_t) & partition18BssSize ,
    (uintptr_t) & partition19BssSize ,
    (uintptr_t) & partition20BssSize ,
    (uintptr_t) & partition21BssSize ,
    (uintptr_t) & partition22BssSize ,
    (uintptr_t) & partition23BssSize ,
    (uintptr_t) & partition24BssSize ,
    (uintptr_t) & partition25BssSize ,
    (uintptr_t) & partition26BssSize ,
    (uintptr_t) & partition27BssSize ,
    (uintptr_t) & partition28BssSize ,
    (uintptr_t) & partition29BssSize ,
    (uintptr_t) & partition30BssSize ,
    (uintptr_t) & partition31BssSize ,
    (uintptr_t) & partition32BssSize ,
    (uintptr_t) & partition33BssSize ,
    (uintptr_t) & partition34BssSize ,
    (uintptr_t) & partition35BssSize ,
    (uintptr_t) & partition36BssSize ,
    (uintptr_t) & partition37BssSize ,
    (uintptr_t) & partition38BssSize ,
    (uintptr_t) & partition39BssSize ,
    (uintptr_t) & partition40BssSize ,
    (uintptr_t) & partition41BssSize ,
    (uintptr_t) & partition42BssSize ,
    (uintptr_t) & partition43BssSize ,
    (uintptr_t) & partition44BssSize ,
    (uintptr_t) & partition45BssSize ,
    (uintptr_t) & partition46BssSize ,
    (uintptr_t) & partition47BssSize ,
    (uintptr_t) & partition48BssSize ,
    (uintptr_t) & partition49BssSize ,
    (uintptr_t) & partition50BssSize ,
    (uintptr_t) & partition51BssSize ,
    (uintptr_t) & partition52BssSize ,
    (uintptr_t) & partition53BssSize ,
    (uintptr_t) & partition54BssSize ,
    (uintptr_t) & partition55BssSize ,
    (uintptr_t) & partition56BssSize ,
    (uintptr_t) & partition57BssSize ,
    (uintptr_t) & partition58BssSize ,
    (uintptr_t) & partition59BssSize ,
    (uintptr_t) & partition60BssSize ,
    (uintptr_t) & partition61BssSize ,
    (uintptr_t) & partition62BssSize ,
    (uintptr_t) & partition63BssSize ,
};


void pharosCpuSharedSectionStart(uintptr_t *start , uintptr_t *size)
{
    *start = (uintptr_t) & pharosSharedStart;
    *size = (uintptr_t) (&pharosSharedEnd - &pharosSharedStart);
}


uint32_t pharosCpuMemoryAreasNumber(ptrEnvMemoryProtection memProtection)
{
    return 1;
}


bool pharosCpuAddressInsideSharedSection(uint8_t *memAddress)
{
    return (&pharosSharedStart <= memAddress) && (&pharosSharedEnd > memAddress) != 0 ? TRUE : FALSE;
}


void pharosCpuMemProtectionInit(ptrEnvMemoryProtection memProtection , PartitionNumber partition)
{
    /* set the driver start address */
    memProtection->startAddress[0] = pharosVPartitionsStart[partition];

    /* set the driver end address */
    memProtection->endAddress[0] = pharosVPartitionsEnd[partition];
}


bool pharosCpuChannelBulkAddressValid(const ptrChannelBulkKernel address)
{
    /* result of the method */
    bool result = FALSE;

    /* calculate the end of the address of the bulk */
    uint8_t *endBulk = ((uint8_t *) address) + sizeof (ChannelBulkKernel);


    /* check if the buffer is inside the bulks area */
    if((&pharosBulkStart <= (uint64_t *) address) && (&pharosBulkEnd >= (uint64_t *) endBulk))
    {
        /* then the buffer is inside the bulks area */

        /* check if the bulk is aligned to its size */
        if(((uintptr_t) address - (uintptr_t) & pharosBulkStart) % (sizeof (ChannelBulkKernel)) == 0U)
        {
            /* it is */
            result = TRUE;
        }
    }

    /* return the result */
    return result;
}


bool pharosCpuAddressInsideKernel(const uint8_t *address)
{
    /* result of the method */
    bool result = FALSE;

    /* check if the buffer is inside the kernel area */
    if((&pharosKernelStart <= address) && (&pharosKernelEnd > address))
    {
        /* then the buffer is inside the kernel area */
        result = TRUE;
    }

    /* return the result */
    return result;
}


bool pharosCpuAddressInsideProtection(const uint8_t *address , const ptrEnvMemoryProtection prot)
{
    /* result of the method */
    bool result;

    /* check if the buffer is inside the protection environment start and end addresses */
    if((prot->startAddress[0] <= address) && (prot->endAddress[0] > address))
    {
        /* then the buffer is inside protection env */
        result = TRUE;
    }
        /* check if the buffer is inside the shared section */
    else if((&pharosSharedStart <= address) && (&pharosSharedEnd > address))
    {
        /* then the buffer is inside the shared section */
        result = TRUE;
    }
        /* check if inside the data "const" zone */
    else if((&pharosRodataStart <= address) && (&pharosRodataEnd > address))
    {
        /* then the buffer is inside the shared section */
        result = TRUE;
    }
        /* check if an interrupt is being executed at the moment */
    else if((pharosIInterruptIsRunning() == TRUE) || (pharosIPartitionHasSupervisor() == TRUE))
    {
        /* inside an interrupt all addresses are OK */
        result = TRUE;
    }
        /* otherwise */
    else
    {
        /* return address is not inside protection */
        result = FALSE;
    }

    /* return the result */
    return result;
}


bool pharosCpuBufferIsInsideProtection(const uint8_t *buffer , uint32_t size , const ptrEnvMemoryProtection prot)
{
    /* result of the method */
    bool result;

    /* calling thread */
    ptrThreadInfo thread;

    /* end position of the buffer */
    const uint8_t *end = &buffer[size];


    /* check if the buffer is inside the start and end */
    if((prot->startAddress[0] <= buffer) && (prot->endAddress[0] >= end))
    {
        /* then the buffer is inside */
        result = TRUE;
    }
        /* check if the buffer is inside the shared section */
    else if((&pharosSharedStart <= buffer) && (&pharosSharedEnd >= end))
    {
        /* then the buffer is inside the shared section */
        result = TRUE;
    }
        /* check if inside the data "const" zone */
    else if((&pharosRodataStart <= buffer) && (&pharosRodataEnd >= end))
    {
        /* then the buffer is inside the shared section */
        result = TRUE;
    }
        /* check if the running partition has supervisor privileges */
    else if(pharosIPartitionHasSupervisor() == TRUE)
    {
        /* a supervisor partition has access to all addresses */
        result = TRUE;
    }
        /* check if an interrupt is being executed at the moment and the address is in the interrupt stack */
    else if(pharosIInterruptIsRunning() == TRUE)
    {
        /* if the address is inside the interrupt stack */
        if(pharosIAddressInsideInterruptStack(buffer , size) == TRUE)
        {
            /* has permissions for interrupt stack */
            result = TRUE;
        }
            /* otherwise, not inside the interrupt stack */
        else
        {
            /* then does not have permissions */
            result = FALSE;
        }
    }
        /* if running a thread and is inside the thread application stack. This could be the case for the shared stack */

        /* otherwise */
    else
    {
        /* we are not executing inside an interrupt, so we can get the running thread */
        thread = pharosISchedGetRunningThread();

        /* check if the thread is running inside another partition */
        if(pharosIThreadStateIsInterPartition(thread) == TRUE)
        {
            /* if the buffer is inside the thread shared stack */
            if((pharosIStackIsBufferInside(&thread->sharedAccessStack->stack , buffer , size) == TRUE))
            {
                /* then it has permissions */
                result = TRUE;
            }
            else
            {
                /* no permissions */
                result = FALSE;
            }
        }
        else
        {
            /* return address is not inside protection */
            result = FALSE;
        }
    }


    /* return the result */
    return result;
}


void pharosIKernelDataInitialize()
{
    /* copy the kernel data */
    pharosICopy64(&pharosKernelDataStart , &pharosKernelDataLoadAddr , (uintptr_t) & pharosKernelDataSize);

    /* initialize the kernel BSS section */
    pharosIZeros64(&pharosKernelBssStart , (uintptr_t) & pharosKernelBssSize);
}


void pharosIIpCallDataInitialize(void)
{
    /* copy the inter-partition calls data */
    pharosICopy64(&pharosIpCallSectionStart , &pharosIpCallDataLoadAddr , (uintptr_t) & pharosIpCallDataSize);
}


void pharosISharedDataInitialize()
{
    /* copy the shared data */
    pharosICopy64(&pharosSharedDataStart , &pharosSharedDataLoadAddr , (uintptr_t) & pharosSharedDataSize);

    /* initialize the shared BSS section */
    pharosIZeros64(&pharosSharedBssStart , (uintptr_t) & pharosSharedBssSize);
}


void pharosIKernelBulksInitialize()
{
    /* initialize the kernel bulks section */
    pharosIZeros64(&pharosBulkStart , (uintptr_t) (&pharosBulkEnd - &pharosBulkStart));
}


void pharosIPartitionDataInitialize(PartitionNumber number)
{
    /* copy the data from flash to ram of partition */
    pharosICopy64((uint64_t *) partitionsDataStart[number] , partitionsDataAddr[number] , partitionsDataSize[number]);

    /* initialize the BSS of partition with zeros */
    pharosIZeros64((uint64_t *) partitionsBssStart[number] , partitionsBssSize[number]);
}
