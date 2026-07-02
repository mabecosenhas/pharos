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


#include <pharos/kernel/scheduler/scheduler.h>
#include <pharos/kernel/thread/dispatcher.h>
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/kernel/scheduler/schedulerinline.h>
#include <pharos/kernel/assert/assert.h>
#include <pharos/kernel/partition/partition.h>
#include <pharos/kernel/core/core.h>


void pharosIThreadResumeOnSchedulerCore(ptrThreadInfo thread)
{
    /* interrupt level */
    PharosInterruptLevel interrupt;

   
    /* check the running core */
    PHAROS_ASSERT_CORE(thread->partition->core->number , "Resuming on invalid core");
    
    /* disable interrupts so nothing can change the value of the global variables */
    pharosCpuDisableInterrupts(&interrupt);

    /* get the queue where the thread was blocked when it was suspended */
    if(thread->suspendedBlockedQueue != NULL)
    {
        /* add the thread again to the blocked queue */
        pharosIThreadQueueAddBlocked(thread->suspendedBlockedQueue , thread);

        /* reset the suspended blocked queue */
        thread->suspendedBlockedQueue = NULL;
    }

    /* if the thread was waiting on a queue when it was suspended */
    if(thread->suspendedAwakeTime != PHAROS_CLOCK_TICK_MAXIMUM)
    {
        /* if the awake time already elapsed */
        if(thread->suspendedAwakeTime <= pharosIClockTicksGet(thread->scheduler))
        {
            /* activate the thread */
            pharosITimeActivateThreadRmp(thread);
        }
        else
        {
            /* now that it is resumed, add it to the queue again */
            pharosITimeTickAddThread(thread , thread->suspendedAwakeTime , thread->type , thread->scheduler);
        }

        /* reset the suspended awake time */
        thread->suspendedAwakeTime = PHAROS_CLOCK_TICK_MAXIMUM;
    }

    /* re-enable interrupts*/
    pharosCpuEnableInterrupts(&interrupt);
}


PharosThreadResumeR pharosSThreadResume(ptrThreadId id)
{
    /* return value of this method */
    PharosThreadResumeR result;

    /* get the thread from its id */
    ptrThreadInfo thread;

    /* interrupt level */
    PharosInterruptLevel interrupt;


    /* check if the id is at a valid address */
    if(pharosCpuBufferIsInsideProtection((uint8_t *) id , sizeof (ThreadId) , pharosIPartitionRunningMemProt()) == FALSE)
    {
        /* return invalid */
        result = PHAROS_THREAD_RESUME_INVALID_ID_ADDRESS;
    }
    else
    {
        /* try to get the thread on the running partition */
        thread = pharosIThreadGet(id);

        /* disable interrupts so nothing can change the value of the global variables */
        pharosCpuDisableInterrupts(&interrupt);

        /* if the id is invalid */
        if(thread == NULL)
        {
            /* re-enable interrupts*/
            pharosCpuEnableInterrupts(&interrupt);

            /* return invalid */
            result = PHAROS_THREAD_RESUME_INVALID_ID;
        }
        else
        {
            /* check if the thread is suspended (or waiting to be suspended) */
            if((pharosIThreadStateIsSuspended(&thread->state) == FALSE) &&
               (pharosIThreadStateIsSuspendWaiting(&thread->state) == FALSE))
            {
                /* re-enable interrupts*/
                pharosCpuEnableInterrupts(&interrupt);

                /* if the thread is not suspended then return an error */
                result = PHAROS_THREAD_RESUME_NOT_SUSPENDED;
            }
            else
            {
                /* remove the suspended state from the thread */
                pharosIThreadStateSetResume(&thread->state);

                /* add the ready thread (if the thread is ready) */
                pharosISchedAddReadyThread(thread);

                /* re-enable interrupts*/
                pharosCpuEnableInterrupts(&interrupt);

                /* resume the thread (on a possible remote core) */
                pharosIThreadResume(thread);

                /* dispatch to the resumed thread (if needed) */
                pharosIDispatchThread();

                /* return success */
                result = PHAROS_THREAD_RESUME_SUCCESS;
            }
        }
    }

    /* return the method result */
    return result;
}
