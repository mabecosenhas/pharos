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
#include <pharos/hal/cpu/sections.h>
#include <pharos/kernel/core/coreinlines.h>


static bool pharosCpuAddressInsidePro(const uint8_t *address , ptrEnvMemoryProtection const prot);
static bool pharosCpuBufferInsidePro(const uint8_t *buffer , uint32_t size , const ptrEnvMemoryProtection prot);
static bool pharosCpuBufferInsidePro2(const uint8_t *buffer , uint32_t size , const ptrEnvMemoryProtection prot , const uint8_t **start);


uint32_t pharosCpuMemoryAreasNumber(ptrEnvMemoryProtection memProtection)
{
    return PHAROS_NUMBER_MEMORY_AREAS;
}


bool pharosCpuAddressInsidePro(const uint8_t *address , const ptrEnvMemoryProtection prot)
{
    bool result = FALSE;
    int i;

    /* for each memory area */
    for(i = 0; i < PHAROS_NUMBER_MEMORY_AREAS; i++)
    {
        /* check if it has access */
        if((prot->startAddress[i] <= address) && (prot->endAddress[i] > address))
        {
            /* then the buffer is inside protection env */
            result = TRUE;
            break;
        }
    }

    return result;
}


bool pharosCpuBufferInsidePro2(const uint8_t *buffer , uint32_t size , const ptrEnvMemoryProtection prot , const uint8_t **start)
{
    bool result = FALSE;
    int i;

    /* end position of the buffer */
    const uint8_t *end = &buffer[size];

    /* for each memory area */
    for(i = 0; i < PHAROS_NUMBER_MEMORY_AREAS; i++)
    {
        /* check if it has access */
        if((prot->startAddress[i] <= buffer) && (prot->endAddress[i] >= end))
        {
            /* then the buffer is inside protection env */
            result = TRUE;
            *start = (uint8_t *) prot->startAddress[i];
            break;
        }
    }

    return result;
}


bool pharosCpuBufferInsidePro(const uint8_t *buffer , uint32_t size , const ptrEnvMemoryProtection prot)
{
    /* return value */
    bool result = FALSE;

    /* iterator of the memory areas */
    uint32_t i;

    /* end position of the buffer */
    const uint8_t *end = &buffer[size];


    /* for each memory area */
    for(i = 0U; i < PHAROS_NUMBER_MEMORY_AREAS; i++)
    {
        /* check if it has access */
        if((prot->startAddress[i] <= buffer) && (prot->endAddress[i] >= end))
        {
            /* then the buffer is inside protection env */
            result = TRUE;

            /* stop searching */
            break;
        }
    }

    /* returns TRUE or FALSE, depending on whether there is a memory area that contains the buffer */
    return result;
}


bool pharosCpuAddressInsideSharedSection(uint8_t *address)
{
    return pharosCpuAddressInsidePro(address , (const ptrEnvMemoryProtection) &pharosVSharedMemProtection);
}


bool pharosCpuAddressInsideKernel(const uint8_t *address)
{
    return pharosCpuAddressInsidePro(address , (const ptrEnvMemoryProtection) &pharosVKernelMemProtection);
}


bool pharosCpuAddressInsideProtection(const uint8_t *address , const ptrEnvMemoryProtection prot)
{
    /* result of the method */
    bool result;

    /* check if the buffer is inside the protection environment start and end addresses */
    if(pharosCpuAddressInsidePro(address , (const ptrEnvMemoryProtection) &pharosVSharedMemProtection) == TRUE)
    {
        /* then the buffer is inside protection env */
        result = TRUE;
    }
        /* check if the buffer is inside the shared section */
    else if(pharosCpuAddressInsidePro(address , prot) == TRUE)
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
        /* check if an interrupt is being executed at the moment or has partition has supervisor permissions */
    else if(pharosIInterruptIsRunning() == TRUE || pharosIPartitionHasSupervisor() == TRUE)
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


void pharosCpuSharedSectionStart(uint32_t *start , uint32_t *size)
{
    /* iterator for the memory areas of the shared section */
    uint32_t i;


    /* initialize the size with a minimum value */
    *size = 0;

    /* find the biggest memory area of the shared section */
    for(i = 0; i < PHAROS_NUMBER_MEMORY_AREAS; i++)
    {
        /* aux contains the size of the section */
        uint32_t aux = (uint32_t) pharosVSharedMemProtection.endAddress[i] - (uint32_t) pharosVSharedMemProtection.startAddress[i];

        /* if the area is bigger than the previous one */
        if(aux > *size)
        {
            /* update the size */
            *size = aux;

            /* and update the start address */
            *start = (uint32_t) pharosVSharedMemProtection.startAddress[i];
        }
    }
}


bool pharosCpuChannelBulkAddressValid(const ptrChannelBulkKernel address)
{
    /* result of the method */
    bool result = FALSE;

    const uint8_t *start;

    /* check if the address is inside the bulk memory areas */
    if(pharosCpuBufferInsidePro2((uint8_t *) address , sizeof (ChannelBulkKernel) ,
                                 (const ptrEnvMemoryProtection) &pharosVBulkMemProtection , &start) == TRUE)
    {
        /* check if the bulk is aligned */
        if(((uintptr_t) address - (uintptr_t) start) % (sizeof (ChannelBulkKernel)) == 0U)
        {
            /* it is */
            result = TRUE;
        }
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
    if(pharosCpuBufferInsidePro(buffer , size , prot) == TRUE)
    {
        /* then the buffer is inside  */
        result = TRUE;
    }
        /* check if the buffer is inside the shared section */
    else if(pharosCpuBufferInsidePro(buffer , size , (const ptrEnvMemoryProtection) &pharosVSharedMemProtection) == TRUE)
    {
        /* then the buffer is inside the shared section */
        result = TRUE;
    }
        /* check if inside the data "const" zone */
    else if((&pharosRodataStart <= buffer) && (&pharosRodataEnd > end))
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


void pharosCpuMemoryAreaInit(const ptrPharosMemoryArea prot)
{
    /* memory area iterator */
    uint32_t i;

    /* for each area */
    for(i = 0; i < PHAROS_NUMBER_MEMORY_AREAS; i++)
    {
        /* copy the initialize data values (from flash to RAM typically) */
        pharosICopy64(prot->startAddress[i] , prot->dataLoadAddress[i] , prot->dataSize[i]);

        /* initialize the BSS section to 0's */
        pharosIZeros64(prot->bssAddress[i] , prot->bssSize[i]);
    }
}


void pharosCpuMemProtectionInit(ptrEnvMemoryProtection memProtection , PartitionNumber partition)
{
    /* set the start address */
    memcpy(memProtection->startAddress , pharosCpuPartitionsMemoryArea[partition].dataRamAddress , sizeof (memProtection->startAddress));

    /* set the end addresses */
    memcpy(memProtection->endAddress , pharosCpuPartitionsMemoryArea[partition].endAddress , sizeof (memProtection->endAddress));
}


void pharosIIpCallDataInitialize(void)
{
    /* copy the inter-partition calls data */
    pharosICopy64(&pharosIpCallSectionStart , &pharosIpCallDataLoadAddr , (uintptr_t) & pharosIpCallDataSize);
}


void pharosIKernelDataInitialize()
{
    /* initialize the kernel data and bss areas */
    pharosCpuMemoryAreaInit((const ptrPharosMemoryArea) &pharosCpuKernelMemoryArea);
}


void pharosISharedDataInitialize()
{
    /* initialize the shared data and bss areas */
    pharosCpuMemoryAreaInit((const ptrPharosMemoryArea) &pharosCpuSharedMemoryArea);
}


void pharosIKernelBulksInitialize()
{
    /* memory area iterator */
    uint32_t i;

    /* for each area */
    for(i = 0; i < PHAROS_NUMBER_MEMORY_AREAS; i++)
    {
        /* initialize the BSS section to 0's */
        pharosIZeros64((uint64_t *) pharosVBulkMemProtection.startAddress[i] ,
                       (uintptr_t) (pharosVBulkMemProtection.endAddress[i] - pharosVBulkMemProtection.startAddress[i]));
    }
}


void pharosIPartitionDataInitialize(PartitionNumber number)
{
    pharosCpuMemoryAreaInit((const ptrPharosMemoryArea) &pharosCpuPartitionsMemoryArea[number]);
}
