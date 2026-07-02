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


#include <pharos/kernel/thread/rtthread.h>
#include <pharos/hal/boardApi.h>
#include <pharos/kernel/interrupt/interrupt.h>
#include <pharos/kernel/object/protection.h>
#include <pharos/kernel/object/object.h>
#include <pharos/kernel/core/coreconfinline.h>


PharosThreadGetStatusR pharosSThreadGetStatus(ptrThreadId id , ptrThreadStatus status)
{
    /* return value of this method */
    PharosThreadGetStatusR result;

    /* thread with the specified name (if correct name) */
    ptrThreadInfo thread;

    /* interrupt level */
    PharosInterruptLevel interrupt;


    /* check if the id address is valid */
    if(pharosCpuBufferIsInsideProtection((uint8_t *) id , sizeof (ThreadId) , pharosIPartitionRunningMemProt()) == FALSE)
    {
        /* address of id is invalid */
        result = PHAROS_THREAD_GET_STATUS_INVALID_ID;
    }
    else if(pharosCpuBufferIsInsideProtection((uint8_t *) status , sizeof (ThreadStatus) , pharosIPartitionRunningMemProt()) == FALSE)
    {
        /* address of status is invalid */
        result = PHAROS_THREAD_GET_STATUS_INVALID_STATUS;
    }
    else
    {
        /* try to get the thread */
        thread = pharosIThreadGet(id);

        /* if there is a thread with such an id on the running partition */
        if(thread != NULL)
        {
            /* disable interrupts so nothing can update the priority nor the state */
            pharosCpuDisableInterrupts(&interrupt);

            status->currentPriority = thread->currentPriority;

            /* copy the state */
            status->state = thread->state;

            /* set the kernel stack size */
            status->kernelStackSize = thread->kernelStack.size;

            /* set the user stack size */
            status->userStackSize = thread->applicationStack.size;

            /* if there is a shared stack */
            if(thread->sharedAccessStack != NULL)
            {
                /* set the shared stack size */
                status->sharedStackSize = thread->sharedAccessStack->stack.size;
            }
            else
            {
                /* if not, then place 0 */
                status->sharedStackSize = 0U;
            }

            /* re-enable interrupts (no need for interrupts to be disabled to get the deadline since it does not change) */
            pharosCpuEnableInterrupts(&interrupt);

            /* if the thread is a RT thread */
            if(pharosIThreadIdIsRt(&thread->threadId) == TRUE)
            {
                /* cast it to a RT thread (we checked) */
                ptrRtThread rtThread = (ptrRtThread) thread;

                /* copy the deadline */
                status->deadline = rtThread->deadline;
            }
                /* if not a real-time thread */
            else
            {
                /* set no deadline */
                status->deadline = PHAROS_DEADLINE_IGNORED;
            }

            /* copy the state */
            status->state = thread->state;

            /* return success */
            result = PHAROS_THREAD_GET_ID_SUCCESS;
        }
        else
        {
            /* could not find a thread with the corresponding id */
            result = PHAROS_THREAD_GET_STATUS_UNKNOWN;
        }
    }

    /* return the method result */
    return result;
}
