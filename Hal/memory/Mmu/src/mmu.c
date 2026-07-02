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


#include <pharos/hal/boardApi.h>
#include <pharos/kernel/partition/partition.h>

typedef struct IoMemAreaTable IoMemAreaTable , *ptrIoMemAreaTable;


bool pharosCpuIoMemAddressIsValid(const ptrPartition partition , uint8_t *memAddress)
{
    /* result of the method */
    bool result = FALSE;

    /* get the I/O table */
    const ptrIoMemAreaTable table = &partition->ioTable;

    /* iterator through the direct areas. The MMU areas are not needed to search since they will trigger an exception */
    uint32_t i;

    /* the start address of the IO area */
    uint8_t *start;

    /* the end address of the IO area */
    uint8_t *end;


    /* go through all direct areas */
    for(i = 0U; i < table->numberDirectAreas; i++)
    {
        /* calculate the start address of the IO area */
        start = table->directAreas[i].start;

        /* calculate the end address of the IO area */
        end = start + table->directAreas[i].size;

        /* and check if the address is present in one of them */
        if((memAddress >= start) && (memAddress <= end))
        {
            /* it is present */
            result = TRUE;

            /* stop searching */
            break;
        }
    }

    /* return TRUE if found and FALSE otherwise */
    return result;
}
