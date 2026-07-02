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


#include <pharos/kernel/thread/periodicthread.h>
#include <pharos/kernel/thread/dispatcher.h>
#include <pharos/thread.h>
#include <pharos/kernel/partition/partition.h>
#include <pharos/hal/boardApi.h>
#include <pharos/kernel/core/syscall.h>
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/kernel/scheduler/schedulerinline.h>


extern uint8_t pharosPeriodicThreadAddress;


void pharosIPeriodicThreadEnd()
{
    ptrThreadInfo thread;

    /* interrupt level */
    PharosInterruptLevel interrupt;

    /* running periodic thread */
    ptrPeriodicThread periodic;

    /* scheduler that schedules the periodic thread */
    ptrFlatScheduler scheduler;


    /* get the running thread */
    thread = pharosISchedGetRunningThread();

    /* get the running periodic thread */
    /* MISRA2004 11.4: if address is pharosPeriodicThreadAddress then this thread must be a periodic thread */
    periodic = (ptrPeriodicThread) thread;

    /* get the scheduler */
    scheduler = thread->scheduler;

    /* if there is a deadline defined and it was not missed */
    if(periodic->rtThread.deadline != PHAROS_DEADLINE_IGNORED)
    {
        /* disable interrupts so we can safely remove the thread from the ready list */
        pharosCpuDisableInterrupts(&interrupt);

        /* deactivate the deadline handler (if one is active) */
        pharosITimeTickRemoveDeadline(&periodic->rtThread);

        /* calculate when the deadline for the next job will take place */
        periodic->rtThread.nextDeadline = thread->lastReplenishmentInstant + thread->replenishBudgetInterval +
                periodic->rtThread.deadline;

        /* check the deadline instant did not yet occur (Test2185) */
        if(periodic->rtThread.nextDeadline > pharosIClockTicksGet(scheduler))
        {
            /* add the deadline handler to be executed at the next deadline */
            pharosITimeTickAddDeadline(&periodic->rtThread , periodic->rtThread.nextDeadline);
        }
        else
        {
            /* announce that the deadline occurred */
            pharosSThreadErrorAnnounce(thread , PHAROS_THREAD_ERROR_DEADLINE_MISS , NULL , NULL);
        }

        /* re-enable interrupts */
        pharosCpuEnableInterrupts(&interrupt);
    }

    /* set waiting for activation */
    pharosIThreadStateWaitActivation(&periodic->rtThread.thread.state);

    /* remove thread from ready list */
    pharosISchedRemoveReadyThread(&periodic->rtThread.thread);

    /* loop to activate the next period (and validate that the thread could be scheduled on the next period) */
    while(TRUE)
    {
        /* calculate the next period start instant */
        thread->lastReplenishmentInstant += thread->replenishBudgetInterval;

        /* disable interrupts so we can safely add the thread to wait for a tick and it remove it from the ready list */
        pharosCpuDisableInterrupts(&interrupt);

        /* if the period already elapsed */
        if(thread->lastReplenishmentInstant <= pharosIClockTicksGet(thread->scheduler))
        {
            /* announce error (a full period elapsed and the thread did not even start to execute) */
            pharosSThreadErrorAnnounce(thread , PHAROS_THREAD_ERROR_PERIOD_MISS , NULL , NULL);
        }
        else
        {
            /* sleep till next period */
            pharosITimeTickAddThread(&periodic->rtThread.thread , thread->lastReplenishmentInstant ,
                                     PHAROS_TIME_TICK_AWAKEN_PERIODIC_THREAD , thread->scheduler);

            /* re-enable interrupts */
            pharosCpuEnableInterrupts(&interrupt);

            /* no need to be on the loop anymore */
            break;
        }

        /* re-enable interrupts */
        pharosCpuEnableInterrupts(&interrupt);
    }

    /* dispatch to other thread */
    pharosIDispatchThread();

    /* reset the execution time for next job */
    periodic->rtThread.thread.executionTime = 0U;
}


CpuModeChangeContext pharosIPeriodicThreadEnded(const uint8_t *address , CpuModeChangeContext context)
{
    /* if the return address is set to the periodic thread return address then this is a valid call */
    if(address == &pharosPeriodicThreadAddress && pharosIThreadIsPeriodic(pharosISchedGetRunningThread()) == TRUE)
    {
        /* end periodic thread period */
        pharosIPeriodicThreadEnd();

        /* when leaving switch to supervisor mode */
        context = pharosCpuSwitchSupervisorMode(context);
    }

    /* return the CPU mode that should be placed */
    return context;
}


void pharosIPeriodicThreadBody(void)
{
    /* when we reach here we are at supervisor mode and with full memory access and interrupts disabled */

    /* body of the thread to execute periodically */
    register PeriodicThreadJobBody body;

    /* running partition */
    ptrPartition partition;

    /* running thread */
    ptrThreadInfo thread;

    /* running periodic thread */
    ptrPeriodicThread periodic;


    /* enable the interrupts right away */
    pharosCpuInterruptForceEnable();

    /* get the running partition */
    partition = pharosIPartitionGetRunning();

    /* get the running thread */
    thread = pharosISchedGetRunningThread();

    /* get the running periodic thread */
    /* MISRA2004 11.4: This function is only invoked by a periodic thread */
    periodic = (ptrPeriodicThread) thread;

    /* get the body */
    body = periodic->body;

    /* loop forever */
    for(;;)
    {
        /* check if the partition should be run with supervisor permissions */
        if(partition->partitionFlags.isSupervisor == TRUE)
        {
            /* run directly the function without switching to user mode */
            body(partition->data);

            /* end this period */
            pharosIPeriodicThreadEnd();
        }
        else
        {
            /* now execute the thread entry point in user mode and with memory protection to only the thread memory space */
            pharosCpuExecuteUserModePer(partition->data , &periodic->rtThread.thread);
            /* when we reach here we are already with supervisor privileges */
        }
    }
}


void pharosIPeriodicThreadInitialize(ptrPeriodicThread thread , ptrPartition partition , ThreadNumber number , bool restart)
{
    /* interrupt level */
    PharosInterruptLevel level;

    /* actual release of the periodic thread */
    ClockTick actualRelease;


    /* set the user allocated stack size */
    thread->rtThread.thread.userStackAllocateSize = sizeof (ThreadRegularStackUserSpace);

    /* initialize the thread info */
    pharosIThreadRtInitialize(&thread->rtThread , partition , TRUE , FALSE , number ,
                              (uintptr_t) pharosIPeriodicThreadBody , (uintptr_t) thread->body , restart);

    /* set the thread as started */
    pharosIThreadStateSetStarted(&thread->rtThread.thread.state);

    /* disable interrupts: cannot allow an interrupt or another thread to change the _threadsReady at the same time */
    pharosCpuDisableInterrupts(&level);

    /* copy the release instant (when restarting the thread the "release" should not be changed but the "actualRelease" 
     * can be changed) */
    actualRelease = thread->release;

    /* if restarting the thread */
    if(restart == TRUE)
    {
        /* increase the release time */
        actualRelease += pharosIClockTicksGet(thread->rtThread.thread.scheduler);
    }

    /* set the first release instant to the instant when the thread is released */
    thread->rtThread.thread.lastReplenishmentInstant = actualRelease;

    /* if the thread is not supposed to run right now */
    if(thread->release != 0U)
    {
        /* make it sleep till next period (don't add it to the list of ready threads) */
        pharosITimeTickAddThread(&thread->rtThread.thread ,
                                 thread->rtThread.thread.lastReplenishmentInstant ,
                                 PHAROS_TIME_TICK_AWAKEN_PERIODIC_THREAD ,
                                 thread->rtThread.thread.scheduler);
    }

    /* if the thread has a deadline */
    if(thread->rtThread.deadline != PHAROS_DEADLINE_IGNORED)
    {
        /* increase the last deadline by the period */
        thread->rtThread.nextDeadline = actualRelease + thread->rtThread.deadline;

        /* set the deadline to the last deadline plus the period  */
        pharosITimeTickAddDeadline(&thread->rtThread , thread->rtThread.nextDeadline);
    }

    /* re-enable interrupts */
    pharosCpuEnableInterrupts(&level);

    /* if the thread is supposed to run right now */
    if(thread->release == 0U)
    {
        /* make the thread ready */
        pharosISchedAddReadyThread(&thread->rtThread.thread);
    }
}


PharosThreadPeriodicCreateR pharosSThreadPeriodicCreate(const ptrPharosThreadPeriodicProperties properties , ptrThreadId id)
{
    /* return value */
    PharosThreadPeriodicCreateR result;

    /* thread being created */
    ptrThreadInfo thread;

    /* periodic thread being created (cast to thread) */
    ptrPeriodicThread periodic;

    /* running partition */
    ptrPartition partition = pharosIPartitionGetRunning();

    /* thread number that will be created */
    ThreadNumber number;

    /* interrupt level */
    PharosInterruptLevel level;


    /* validate the basic properties */
    result = pharosIThreadPropertiesValidate(&properties->basic , id , sizeof (PharosThreadPeriodicProperties));

    /* check if the properties are correct */
    if(result == PHAROS_THREAD_PERIODIC_CREATE_SUCCESS)
    {
        /* create memory for all the threads stacks and the TCB */
        result = pharosIThreadCreate(&thread , sizeof (PeriodicThread) , &properties->basic ,
                                     sizeof (ThreadRegularStackUserSpace));

        /* if successfully created (not given out of memory error) */
        if(result == PHAROS_THREAD_PERIODIC_CREATE_SUCCESS)
        {
            /* cast to an periodic thread (we created with sizeof(PeriodicThread) */
            periodic = (ptrPeriodicThread) thread;

            /* place the entry point */
            periodic->body = properties->entry;

            /* copy the deadline */
            periodic->rtThread.deadline = properties->deadline;
            
            /* reset the release (if an application wants a different release, it should use the pharosThreadStartDelay function */
            periodic->release = 0U;

            /* disable interrupts: cannot allow an interrupt or another thread to change the _threadsReady at the same time */
            pharosCpuDisableInterrupts(&level);

            /* get the thread number to be used */
            number = partition->periodicIterator;

            /* increment the aperiodic iterator for the next thread to be created */
            partition->periodicIterator++;

            /* re-enable interrupts */
            pharosCpuEnableInterrupts(&level);

            /* initialize the thread */
            pharosIThreadRtInitialize(&periodic->rtThread , partition , TRUE , FALSE , number ,
                                      (uintptr_t) pharosIPeriodicThreadBody , (uintptr_t) periodic->body , FALSE);

            /* add the thread to the partition */
            partitionAddPeriodicThread(partition , periodic);

            /* copy the id */
            *id = periodic->rtThread.thread.threadId;

            /* but do not place it in the ready queue */
        }
        /* else, just return the error */
    }
    /* else, just return the error */

    /* return the result */
    return result;
}
