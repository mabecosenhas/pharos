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


#include <pharos/kernel/thread/thread.h>
#include <pharos/hal/boardApi.h>
#include <pharos/kernel/interrupt/interrupt.h>
#include <pharos/kernel/object/protection.h>
#include <pharos/kernel/object/object.h>
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/kernel/clock/clock.h>
#include <pharos/kernel/thread/rtthread.h>
#include <pharos/kernel/thread/dispatcher.h>


void pharosIThreadRestart(ptrThreadInfo thread , ptrClockTick start)
{
    /* stop the thread (state is re-initialized) */
    pharosIThreadStop(thread);

    /* and start the thread (will re-enable interrupts) */
    pharosIThreadStart(thread , 0U , start);
}


PharosThreadRestartR pharosSThreadRestart(ptrThreadId id)
{
    /* return value of this method */
    PharosThreadRestartR result;

    /* thread with the specified id from the partition thread map */
    ptrThreadInfo thread;

    /* interrupt level */
    PharosInterruptLevel interrupt;

    /* clock tick when the thread starts (unused on this function) */
    ClockTick start;


    /* check that the id is at a valid address */
    if(pharosCpuBufferIsInsideProtection((uint8_t *) id , sizeof (ThreadId) , pharosIPartitionRunningMemProt()) == FALSE)
    {
        /* return invalid id */
        result = PHAROS_THREAD_RESTART_INVALID_ID_ADDRESS;
    }
    else
    {
        /* get the thread with the specified id from the partition thread map */
        thread = pharosIThreadGet(id);

        /* check that the thread exists */
        if(thread != NULL)
        {
            /* disable interrupts so nothing can change the value of the thread state */
            pharosCpuDisableInterrupts(&interrupt);

            /* check if the thread is executing on its own partition */
            if(pharosIThreadStateIsInterPartition(thread) == FALSE)
            {
                /* stop the thread (won't be scheduled again) */
                pharosIThreadStateSetStopped(&thread->state);

                /* re-enable interrupts*/
                pharosCpuEnableInterrupts(&interrupt);

                /* restart the thread */
                pharosIThreadRestart(thread , &start);

                /*return success */
                result = PHAROS_THREAD_RESTART_SUCESS;
            }
            else
            {
                /* re-enable interrupts*/
                pharosCpuEnableInterrupts(&interrupt);

                /* return holding resources */
                result = PHAROS_THREAD_RESTART_USING_INTER_PARTITION;
            }
        }
            /* else, the thread is not valid */
        else
        {
            /* return invalid id */
            result = PHAROS_THREAD_RESTART_INVALID_ID;
        }
    }

    /* return success or the error */
    return result;
}
