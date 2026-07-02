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


#include <pharos/kernel/thread/aperiodicthread.h>
#include <pharos/kernel/thread/thread.h>
#include <pharos/kernel/scheduler/scheduler.h>
#include <pharos/kernel/thread/dispatcher.h>
#include <pharos/kernel/partition/partition.h>
#include <pharos/hal/boardApi.h>
#include <pharos/kernel/thread/idle.h>
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/kernel/clock/clock.h>
#include <pharos/kernel/thread/threadinline.h>


extern uint8_t pharosAperiodicThreadAddress;


/**
 * Ends the aperiodic thread
 */
static void pharosIAperiodicThreadEnd();


void pharosIAperiodicThreadBody(void)
{
    /* when we reach here we are at supervisor mode and with full memory access and interrupts disabled */

    /* get the running thread (it must be an aperiodic thread) */
    ptrAperiodicThread running;

    /* get the running partition */
    ptrPartition partition;


    /* enable the interrupts right away */
    pharosCpuInterruptForceEnable();

    /* get the running thread (it must be an aperiodic thread) */
    running = (ptrAperiodicThread) pharosISchedGetRunningThread();

    /* get the running partition */
    partition = pharosIPartitionGetRunning();

    /* check if the partition of the thread has supervisor permissions */
    if(partition->partitionFlags.isSupervisor == TRUE)
    {
        /* run directly the function without switching to user mode */
        running->body(partition->data);

        /* end the aperiodic thread */
        pharosIAperiodicThreadEnd();
    }
    else
    {
        /* now switch to user mode and go to the running thread memory access */
        pharosCpuExecuteUserModeAper(partition->data , &running->info);
    }

    /* should never return */
}


void pharosIAperiodicThreadEnd()
{
    /* get the aperiodic thread that is running */
    ptrThreadInfo thread = pharosISchedGetRunningThread();


    /* if we get here then the thread left the main body and should not be executed again */
    pharosISchedRemoveReadyThread(thread);

    /* place the thread state as deleted */
    pharosIThreadStateDelete(&thread->state);

    /* dispatch to next thread */
    pharosIDispatchThread();
    
    /* it is possible that the thread will reach here, specially in ArmV7-M */
}


void pharosIAperiodicThreadEnded(const uint8_t *address)
{
    /* check if the address is correct */
    if(address == &pharosAperiodicThreadAddress && pharosIThreadIsAperiodic(pharosISchedGetRunningThread()) == TRUE)
    {
        /* end the aperiodic thread */
        pharosIAperiodicThreadEnd();
    }
}


void pharosIAperiodicThreadInit(ptrAperiodicThread thread , ptrPartition partition , ThreadNumber number , bool restart)
{
    /* set the user allocated stack size */
    thread->info.userStackAllocateSize = sizeof (ThreadRegularStackUserSpace);

    /* initialize common thread data */
    pharosIThreadInitialize(&thread->info , partition , partition->scheduler , FALSE , FALSE , TRUE , number ,
                            (uintptr_t) & pharosIAperiodicThreadBody , (uintptr_t) thread->body , restart);

    /* set the thread as started */
    pharosIThreadStateSetStarted(&thread->info.state);

    /* make the thread ready */
    pharosISchedAddReadyThread(&thread->info);
}


PharosThreadAperiodicCreateR pharosSThreadAperiodicCreate(const ptrPharosThreadAperiodicProperties properties , ptrThreadId id)
{
    /* result of this method */
    PharosThreadAperiodicCreateR result;

    /* pointer to thread info */
    ptrThreadInfo thread;

    /* pointer to aperiodic thread (equal to thread but with a cast) */
    ptrAperiodicThread aperiodic;

    /* get the running partition */
    ptrPartition partition = pharosIPartitionGetRunning();

    /* thread number of the new thread */
    ThreadNumber number;

    /* interrupt level */
    PharosInterruptLevel level;


    /* validate the basic properties */
    result = pharosIThreadPropertiesValidate(&properties->basic , id , sizeof (PharosThreadAperiodicProperties));

    /* if the properties are correct */
    if(result == PHAROS_THREAD_APERIODIC_CREATE_SUCCESS)
    {
        /* create memory for all the threads stacks and the TCB */
        result = pharosIThreadCreate(&thread , sizeof (AperiodicThread) , &properties->basic ,
                                     sizeof (ThreadRegularStackUserSpace));

        /* if successfully created (not given out of memory error) */
        if(result == PHAROS_THREAD_APERIODIC_CREATE_SUCCESS)
        {
            /* cast to an aperiodic thread (we created with sizeof(AperiodicThread) */
            aperiodic = (ptrAperiodicThread) thread;

            /* place the entry point */
            aperiodic->body = properties->entry;

            /* disable interrupts: cannot allow an interrupt or another thread to change the _threadsReady at the same time */
            pharosCpuDisableInterrupts(&level);

            /* get the thread number to be used */
            number = partition->aperiodicIterator;

            /* increment the aperiodic iterator for the next thread to be created */
            partition->aperiodicIterator++;

            /* re-enable interrupts */
            pharosCpuEnableInterrupts(&level);

            /* initialize the thread */
            pharosIThreadInitialize(&aperiodic->info , partition , partition->scheduler , FALSE , FALSE , TRUE ,
                                    number , (uintptr_t) & pharosIAperiodicThreadBody , (uintptr_t) aperiodic->body , FALSE);

            /* add the thread to the partition */
            partitionAddAperiodicThread(partition , aperiodic);

            /* copy the id */
            *id = aperiodic->info.threadId;

            /* but do not place it in the ready queue */
        }
        /* else, just return the error */
    }
    /* else, just return the error */


    /* return the result */
    return result;
}
