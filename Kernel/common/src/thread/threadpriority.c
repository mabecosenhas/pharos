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


#include <pharos/kernel/thread/priority.h>
#include <pharos/kernel/thread/thread.h>
#include <pharos/hal/boardApi.h>
#include <pharos/kernel/interrupt/interrupt.h>
#include <pharos/kernel/object/protection.h>
#include <pharos/kernel/object/object.h>
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/kernel/scheduler/schedulerinline.h>


void pharosIPriorityBitMapInit(ptrPriorityBitMap bitMap)
{
    /* bit field iterator */
    uint32_t i;

    /* for each bit field */
    for(i = 0U; i < NUMBER_BIT_MAPS; i++)
    {
        /* reset it */
        bitMap->map64[i] = 0U;
    }
}


intptr_t pharosIThreadPriorityComparator(const ptrThreadPriority a , const ptrThreadPriority b)
{
    /* return the comparison result */
    return (intptr_t) (*a) - (intptr_t) (*b);
}


PharosThreadPrioritySetR pharosSThreadSetPriority(ptrThreadId id , ThreadPriority newPriority)
{
    /* return value of this method */
    PharosThreadPrioritySetR result;

    /* thread with the specified name (if correct name) */
    ptrThreadInfo thread;


    /* check if the id address is valid */
    if(pharosCpuBufferIsInsideProtection((uint8_t *) id , sizeof (ThreadId) , pharosIPartitionRunningMemProt()) == FALSE)
    {
        /* address of id is invalid */
        result = PHAROS_THREAD_PRIORITY_SET_INVALID_ID;
    }
    else
    {
        /* try to get the thread */
        thread = pharosIThreadGet(id);

        /* if there is a thread with such an id on the running partition */
        if(thread != NULL)
        {
            /* if the priority is too high (lower in value) - hierarchical schedulers allow any change of priority */
            if(pharosISchedulerIsHierarchical() == FALSE && newPriority < thread->priority)
            {
                /* not allowed to place a priority higher than the initial value */
                result = PHAROS_THREAD_PRIORITY_SET_TOO_HIGH;
            }
            else
            {

                /* update the thread priority on any priority queue it may be currently be on */
                pharosIThreadQueueUpdateThreadPrio(thread , newPriority);

                /* return success */
                result = PHAROS_THREAD_PRIORITY_SET_SUCESS;
            }
        }
        else
        {
            /* could not find a thread with the corresponding id */
            result = PHAROS_THREAD_SET_PRIORITY_UNKNOWN;
        }
    }

    /* return the method result */
    return result;
}
