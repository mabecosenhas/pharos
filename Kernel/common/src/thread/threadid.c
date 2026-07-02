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


PharosThreadGetIdSelfR pharosSThreadGetIdSelf(ptrThreadId id)
{
    /* return value of this method */
    PharosThreadGetIdSelfR result;


    /* check if is running inside an interrupt */
    if(pharosIInterruptIsRunning() == TRUE)
    {
        /* inside an interrupt cannot get an id of a thread */
        result = PHAROS_THREAD_GET_ID_SELF_INSIDE_INTERRUPT;
    }
        /* check if the id address is valid */
    else if(pharosCpuBufferIsInsideProtection((uint8_t *) id , sizeof (ThreadId) , pharosIPartitionRunningMemProt()) == FALSE)
    {
        /* address of id is invalid */
        result = PHAROS_THREAD_GET_ID_SELF_INVALID_ID;
    }
    else
    {
        /* copy the thread id of the running thread (id is fixed so no need to disable interrupts) */
        *id = pharosISchedGetRunningThread()->threadId;

        /* return success */
        result = PHAROS_THREAD_GET_ID_SELF_SUCCESS;
    }

    /* return the method result */
    return result;
}


PharosThreadGetIdR pharosSThreadGetId(const char *name , ptrThreadId id)
{
    /* return value of this method */
    PharosThreadGetIdR result;

    /* interrupt level */
    PharosInterruptLevel interrupt;

    /* get the running partition */
    ptrPartition partition = pharosIPartitionGetRunning();

    /* thread with the specified name (if correct name) */
    ptrThreadInfo thread;


    /* check if the name is valid */
    if(pharosCpuStringIsInsideProtection(name , PHAROS_THREAD_NAME_MAX , pharosIPartitionRunningMemProt()) == FALSE)
    {
        /* name is invalid */
        result = PHAROS_THREAD_GET_ID_INVALID_NAME;
    }
        /* check if the id address is valid */
    else if(pharosCpuBufferIsInsideProtection((uint8_t *) id , sizeof (ThreadId) , pharosIPartitionRunningMemProt()) == FALSE)
    {
        /* address of id is invalid */
        result = PHAROS_THREAD_GET_ID_INVALID_ID;
    }
    else
    {
        /* disable interrupts so nothing can update the tree map of the partition names */
        pharosCpuDisableInterrupts(&interrupt);

        thread = (ptrThreadInfo) treeMapGet(&partition->threadsByName , name);

        /* re-enable interrupts*/
        pharosCpuEnableInterrupts(&interrupt);

        if(thread != NULL)
        {
            /* copy the thread id */
            *id = thread->threadId;

            /* return success */
            result = PHAROS_THREAD_GET_ID_SUCCESS;
        }
        else
        {
            /* could not find a thread with the corresponding name */
            result = PHAROS_THREAD_GET_ID_UNKNOWN;
        }
    }

    /* return the method result */
    return result;
}
