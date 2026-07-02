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


#include <pharos/kernel/partition/partition.h>
#include <pharos/kernel/thread/dispatcher.h>
#include <pharos/kernel/thread/thread.h>
#include <pharos/kernel/scheduler/scheduler.h>
#include <pharos/kernel/interrupt/interrupt.h>
#include <pharos/hal/boardApi.h>
#include <pharos/kernel/clock/clock.h>
#include <pharos/kernel/thread/rtthread.h>
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/kernel/thread/aperiodicthread.h>
#include <pharos/kernel/scheduler/schedulerinline.h>
#include <pharos/kernel/assert/assert.h>


INLINE bool updateWcet(ptrThreadInfo running , ptrThreadInfo heir)
{
    bool result = TRUE;

    /* flag indicating if the running thread has a WCET */
    bool shouldDisableWcet = FALSE;

    /* get the number of microseconds since boot */
    volatile uint64_t microseconds = pharosIClockGetSinceBoot();

    /* get the current clock tick */
    volatile ClockTick t = pharosIClockTicksGet(pharosICore()->runningFlatScheduler);


    /* if the running thread has a WCET */
    if(running->wcet != PHAROS_WCET_UNLIMITED)
    {
        /* update the execution time of the running thread (must be lower than wcet -> we read the microseconds 
         * a little bit after the thread stopped executing so this makes sure the next time the thread will run it will 
         * have at least one microseconds to execute) */
        running->executionTime = MIN(running->wcet - 1 , running->executionTime + microseconds - running->lastDispatchTime);

        /* save to flag that we are switching from a thread with WCET */
        shouldDisableWcet = TRUE;
    }

    /* if the heir thread has a WCET */
    if(heir->wcet != PHAROS_WCET_UNLIMITED)
    {
        /* if the replenishment instant for the heir thread has already elapsed */
        if(heir->lastReplenishmentInstant + heir->replenishBudgetInterval >= t)
        {
            /* calculate the difference between the current time and the last replenish */
            uint32_t difference = (uint32_t) (t - heir->lastReplenishmentInstant);

            /* calculate the clock tick where the budget was supposed to be replenished */
            heir->lastReplenishmentInstant += (difference / heir->replenishBudgetInterval) * heir->replenishBudgetInterval;

            /* replenish the budget */
            heir->executionTime = 0;
        }

        /* update the last dispatch instant of the heir thread */
        heir->lastDispatchTime = microseconds;

        /* should not disable WCET */
        shouldDisableWcet = FALSE;

        /* set the clock interrupt when the WCET expires */
        pharosCpuWcetClockFire((uint32_t) (heir->wcet - heir->executionTime));
    }

    /* if the heir does not have a WCET and we are switching from a real-time thread with WCET */
    if(shouldDisableWcet == TRUE)
    {
        /* this core is no longer waiting on any WCET timer (this is used to prevent race conditions if a pending WCET interrupt was already set -> since this flag is FALSE the pending interrupt will be ignored) vfg-uç. nh n*/
        pharosICore()->wcetIsActive = FALSE;

        /* then disable the WCET clock interrupt */
        pharosCpuWcetClockDisable();
    }
    else
    {
        pharosICore()->wcetIsActive = TRUE;
    }

    return result;
}


bool pharosIContextSwitchPossible(ptrThreadInfo heir)
{
    /* result of this method */
    bool result;


    /* if idle thread (check it first to make sure heir->partition is not NULL) */
    if(heir->partition == NULL)
    {
        /* then can switch */
        result = TRUE;
    }
        /* check the thread is ready. The thread could be "unready", for example, when stopping/suspending a thread. 
         * Between the thread state is placed with "stopped" (or "suspended") and the thread is taken from the ready 
         * list, an interrupt could have occurred which would activate that thread. This "if" prevents that thread
         * from executing and allows the original thread (that was stopping/suspending the thread) to complete its 
         * process */
    else if(pharosIThreadStateIsReady(&heir->state) == FALSE)
    {
        /* then cannot switch to it */
        result = FALSE;
    }
        /* if the heir thread partition is restarting and it is not the restart thread and the thread is not "inside" another partition */
    else if((heir->partition->state == PHAROS_PARTITION_STATE_RESTARTING) &&
            (&heir->partition->restartThread->info != heir) &&
            (heir->state.interPartitionNest == 0U))
    {
        /* then cannot switch to it */
        result = FALSE;
    }
    else
    {
        /* can switch to the heir thread */
        result = TRUE;
    }

    /* return TRUE if can switch to the heir thread and FALSE otherwise */
    return result;
}


void pharosIContextSwitch(ptrThreadInfo running)
{
    /* heir thread to dispatch to */
    ptrThreadInfo heir;

    /* interrupt level */
    PharosInterruptLevel interrupt;


    /* make sure dispatching is enable */
    PHAROS_ASSERT(pharosIIsDispatchingEnabled() == TRUE , "dispatching disabled");

    /* check that context switch is needed */
    PHAROS_ASSERT(pharosINeedsThreadContextSwitch() == TRUE , "dispatching is not needed");

    /* check if not inside an interrupt 
     * (although we could context switch inside an interrupt, the nest level should be 0) */
    PHAROS_ASSERT(pharosIInterruptNest() == 0 , "interrupt nesting not 0 in core %d, it is %d" , pharosICoreRunning() ,
                  (uint32_t) pharosIInterruptNest());

    /* dispatch no longer needed (at least until interrupts are enabled again) */
    pharosIThreadContextSwitchNeeded(FALSE);

    /* awake next thread (if any) */
    pharosITimeTickAwakeNext(running);

    /* loop until we have dispatch to another thread (if the heir thread is being restarted, remove it and loop again) */
    while(TRUE)
    {
        /* re-disable interrupts while we are searching for the next thread to dispatch to */
        pharosCpuDisableInterrupts(&interrupt);

        /* get the highest priority ready thread */
        heir = pharosISchedGetHighestReady(pharosICore()->runningFlatScheduler);

        /* awake next thread (if any) */
        pharosITimeTickAwakeNext(heir);

        /* if the heir partition is being restarted and the thread is not inside an inter partition object */
        /* from a schedulability point of view this "loop" should be account only once. This is because any thread 
         * that is not possible to switch to (e.g. is restarting) would take more time than an extra loop, so from
         * a schedulability point of view the loop could be seen as executing only once. */
        if(pharosIContextSwitchPossible(heir) == FALSE)
        {
            /* remove the thread from the thread ready list */
            pharosISchedRemoveReadyThread(heir);

            /* re-enable interrupts */
            pharosCpuEnableInterrupts(&interrupt);

            /* get the next heir thread */
            continue;
        }
        /* else, if using a resource let it continue using the resource (of another partition) */

        /* if context switch is required */
        if(running != heir)
        {
            /* update the WCET and schedule the timer if needed */
            updateWcet(running , heir);

            /* re-enable interrupts */
            pharosCpuEnableInterrupts(&interrupt);

            /* update the running thread pointer */
            pharosISchedUpdateRunningThread(heir);

            /* perform the context switch to the heir thread, saving the context of the running thread */
            pharosCpuContextSwitch(running , heir);
        }
        else
        {
            /* re-enable interrupts */
            pharosCpuEnableInterrupts(&interrupt);
        }

        break;
    }
}


void pharosIDispatchHeirThread(void)
{
    /* heir thread to dispatch to */
    ptrThreadInfo heir;

    /* interrupt level */
    PharosInterruptLevel interrupt;

    /* get the running thread */
    ptrThreadInfo running = pharosISchedGetRunningThread();


    /* dispatch no longer needed (at least until interrupts are enabled again) */
    pharosIThreadContextSwitchNeeded(FALSE);

    /* re-disable interrupts while we are searching for the next thread to dispatch to */
    pharosCpuDisableInterrupts(&interrupt);

    /* get the highest priority ready thread */
    heir = pharosISchedGetHighestReady(pharosICore()->runningFlatScheduler);

    /* awake next thread (if any) */
    pharosITimeTickAwakeNext(heir);

    /* if context switch is required */

    /* update the WCET and schedule the timer if needed */
    updateWcet(running , heir);

    /* update the running thread pointer */
    pharosISchedUpdateRunningThread(heir);

    /* switch to the heir thread and DO NOT save the context of the running thread. This way its current context 
     * (that has already been re-initialized) is not overridden */
    pharosCpuRestoreHeir(heir);

    /* check that the restore heir never reaches here */
    PHAROS_ASSERT(FALSE , "Should not have reached here");
}


void pharosIDispatchThread(void)
{
    /* interrupt level */
    PharosInterruptLevel interrupt;

    /* running thread */
    ptrThreadInfo running;


    /* disable interrupts while we are searching if there is a need to dispatch to another thread */
    pharosCpuDisableInterrupts(&interrupt);

    /* only dispatch if allowed and a dispatch is needed */
    while((pharosIIsDispatchingEnabled() == TRUE) && (pharosINeedsThreadContextSwitch() == TRUE))
    {
        /* get the running thread */
        running = pharosISchedGetRunningThread();

        /* context switch to the heir thread */
        pharosIContextSwitch(running);
    }

    /* re-enable interrupts */
    pharosCpuEnableInterrupts(&interrupt);
}
