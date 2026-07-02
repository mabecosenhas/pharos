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


/**
 * Gets the critical section given the partition and the index on the critical section table
 * 
 * @param partition partition 
 * @param index index on the critical section table of the partition
 * 
 * @return returns the critical section or NULL if the index is invalid
 */
static ptrCriticalSection getCriticalSection(ptrPartition partition , uint32_t index);


PharosCsExecuteR pharosSCriticalSectionExecute(uint32_t index , uint32_t arg1 , uint32_t arg2 , uint32_t arg3 ,
                                               uint32_t arg4 , uint32_t *output)
{
    /* return value of this method*/
    PharosCsExecuteR result;

    /* get the running partition */
    ptrPartition partition = pharosIPartitionGetRunning();

    /* get the critical section */
    ptrCriticalSection section = getCriticalSection(partition , index);


    /* if could not find the critical section */
    if(section == NULL)
    {
        /* return error */
        result = PHAROS_CRITICAL_SECTION_EXECUTE_INVALID_INDEX;
    }
        /* check if the output is in a valid location */
    else if(pharosCpuBufferIsInsideProtection((uint8_t *) output , sizeof (uint32_t) ,
                                              pharosIPartitionRunningMemProt()) == FALSE)
    {
        /* it is not, return invalid */
        result = PHAROS_CRITICAL_SECTION_EXECUTE_INVALID_OUTPUT;
    }
    else
    {
        /* execute the handler in user mode and with interrupts disabled */
        *output = pharosCpuCriticalSectionExecute(partition->data , arg1 , arg2 , arg3 , arg4 ,
                                                  pharosIPartitionMemoryContext(partition) , section->csHandler);

        /* return success */
        result = PHAROS_CRITICAL_SECTION_EXECUTE_SUCCESS;
    }

    /* return the error or success */
    return result;
}


ptrCriticalSection getCriticalSection(ptrPartition partition , uint32_t index)
{
    /* return value of this method */
    ptrCriticalSection result;


    /* if the index is valid */
    if(index < partition->criticalSectionTable.size)
    {
        /* return the configuration with the selected index */
        result = &partition->criticalSectionTable.sections[index];
    }
    else
    {
        /* return error */
        result = NULL;
    }

    /* returns the selected configuration or NULL if does not exist */
    return result;
}
