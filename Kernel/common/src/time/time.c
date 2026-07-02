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


#include <pharos/kernel/time/time.h>
#include <pharos/kernel/timer/timer.h>
#include <pharos/kernel/clock/clock.h>
#include <pharos/kernel/thread/aperiodicthread.h>
#include <pharos/error.h>
#include <pharos/kernel/thread/dispatcher.h>
#include <pharos/kernel/thread/periodicthread.h>
#include <pharos/kernel/thread/sporadicthread.h>
#include <pharos/hal/boardApi.h>
#include <pharos/kernel/error/error.h>
#include <pharos/kernel/scheduler/scheduler.h>
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/kernel/core/multi.h>
#include <pharos/kernel/scheduler/schedulerinline.h>
#include <pharos/kernel/assert/assert.h>
#include <pharos/kernel/core/core.h>
#include <pharos/kernel/core/coreinlines.h>
#include <pharos/kernel/clock/clockinline.h>

#include "pharos/kernel/assert/declarations.h"


void pharosITimeInitializeTickHandler(ptrFlatScheduler scheduler , ptrTimeTickHandler handler , ClockTick awakeTime)
{
    /* next tick tick to process */
    ClockTick next = pharosIClockTickNextGet(scheduler);


    /* check that the handler is valid */
    PHAROS_ASSERT(handler != NULL , "Handler is invalid");

    /* set the awake time */
    handler->awakeTime = awakeTime;

    /* add the time tick to the map of time ticks at the same instant */
    treeMapPut(pharosIClockTickHandlersGet(scheduler) , &handler->awakeTime , handler);

    /* if need to update the instant of the closest clock tick */
    if(next > awakeTime)
    {
        /* then update it */
        pharosIClockTickNextSet(scheduler , awakeTime);
    }
}


void pharosITimeResetNextTimeTick(ptrFlatScheduler scheduler)
{
    /* interrupt level */
    PharosInterruptLevel interruptLevel;

    /* first time tick handler to be executed */
    ptrTimeTickHandler first;


    /* don't allow anyone inside threads or interrupts to change the timer */
    pharosCpuDisableInterrupts(&interruptLevel);

    /* get the first to be executed */
    first = (ptrTimeTickHandler) treeMapGetFirst(pharosIClockTickHandlersGet(scheduler));

    /* if there is no first (map is empty) */
    if(first == NULL)
    {
        /* reset the first clock tick */
        pharosIClockTickNextSet(scheduler , PHAROS_CLOCK_TICK_MAXIMUM);
    }
        /* otherwise */
    else
    {
        /* then update it with the first item on the tree map */
        pharosIClockTickNextSet(scheduler , first->awakeTime);
    }

    /* interrupts can now be enabled */
    pharosCpuEnableInterrupts(&interruptLevel);
}


void pharosITimeUpdateNextClockTick(ptrFlatScheduler scheduler , ClockTick currentClockTick)
{
    /* first time tick handler to be executed */
    ptrTimeTickHandler first;

    /* interrupt level */
    PharosInterruptLevel level;


    /* don't allow anyone inside threads or interrupts to change the handlers */
    pharosCpuDisableInterrupts(&level);

    /* if need to update the instant of the closest clock tick */
    if(pharosIClockTickNextGet(scheduler) == currentClockTick)
    {
        /* get the first to be executed */
        first = (ptrTimeTickHandler) treeMapGetFirst(pharosIClockTickHandlersGet(scheduler));

        /* if there is no first (map is empty) */
        if(first == NULL)
        {
            /* reset the first clock tick */
            pharosIClockTickNextSet(scheduler , PHAROS_CLOCK_TICK_MAXIMUM);
        }
            /* otherwise */
        else
        {
            /* then update it with the first item on the tree map */
            pharosIClockTickNextSet(scheduler , first->awakeTime);
        }
    }

    /* interrupts can now be enabled */
    pharosCpuEnableInterrupts(&level);
}


void pharosITimeTickIsr(void)
{
    /* in hierarchical scheduler has the number of pending ticks that a new partition will have to process */
    ClockTick pendingTicks;

    /* clock tick iterator for the number of pending ticks */
    ClockTick i;

    /* current clock tick */
    ClockTick currentTick;

    /* get the running core */
    ptrCore core = pharosICore();


    /* check that the thread dispatch is disabled. 
     * Since this function should be called from inside an ISR this seams natural */
    PHAROS_ASSERT(pharosIIsDispatchingEnabled() == FALSE , "Dispatching is enabled while processing the time tick");

    /* increase the number of clock ticks (first clock tick to occur is "0") */
    currentTick = pharosIClockTicksInc(1U , core);

    /* increment the date by one clock tick */
    pharosIRmpIncDateOneClockTick(core);

    /* check if the core is running an hierarchical scheduler */
    if(core->scheduler->type == PHAROS_SCHEDULER_HIERARCHICAL)
    {
        /* if so, update the running partition (if needed) */
        pendingTicks = pharosISchedHierarchicalUpdate(core , (ptrHierarchicalScheduler) core->scheduler , currentTick);

        /* increment the clock tick by the number of pending ticks 
         * (not executed since the running partition was not scheduled) */
        pharosIClockTicksInc(pendingTicks , core);

        /* execute the scheduler for any pending interrupt (at least once) and after the partition has possibly been 
         * switched */
        pharosITimeTickFlatScheduler(core->runningFlatScheduler);

        /* for every pending clock tick that the heir scheduler did not execute */
        for(i = 0U; i < pendingTicks; i++)
        {
            /* schedule one tick */
            pharosITimeTickFlatScheduler(core->runningFlatScheduler);
        }
    }
    else
    {
        /* schedule one tick */
        pharosITimeTickFlatScheduler(core->runningFlatScheduler);
    }
}


void pharosIWcetHandlerIsr(const uint8_t *interruptedAddress)
{
    /* get the running thread whose WCET just expired */
    ptrThreadInfo thread = pharosISchedGetRunningThread();

    /* interrupt level */
    PharosInterruptLevel interruptLevel;

    /* variable where to save the clock ticks */
    volatile ClockTick t;


    /* if the WCET is canceled on this core */
    if(pharosICore()->wcetIsActive == FALSE)
    {
        /* then don't do anything (this was a pending interrupt that is no longer applicable) */
        return;
    }

    /* remove any active handler (shouldn't do anything in 99% of the times but if we just placed the tick handler then 
     * this removes it) */
    pharosITimeTickRemoveThread(thread);

    /* don't allow anyone inside threads or interrupts to change the timer */
    pharosCpuDisableInterrupts(&interruptLevel);

    t = pharosIClockTicksGet(thread->scheduler);

    /* interrupts can now be enabled */
    pharosCpuEnableInterrupts(&interruptLevel);


    /* check to see if the WCET instant occurred after the budget was supposed to be replenished */
    if(thread->lastReplenishmentInstant + thread->replenishBudgetInterval <= t)
    {
        /* calculate the difference between the current time and the last replenish */
        uint32_t difference = (uint32_t) (t - thread->lastReplenishmentInstant);

        /* calculate the clock tick where the budget was supposed to be replenished */
        thread->lastReplenishmentInstant += (difference / thread->replenishBudgetInterval) * thread->replenishBudgetInterval;

        /* replenish the budget */
        thread->executionTime = 0;
    }
    else
    {
        /* if thread is periodic */
        if(thread->threadId.flags.isPeriodic == TRUE)
        {
            /* announce error (WCET) */
            pharosSThreadErrorAnnounce(thread , PHAROS_THREAD_ERROR_WCET ,
                                       interruptedAddress , NULL);

            /* calculate next activation instant */
            thread->lastReplenishmentInstant += thread->replenishBudgetInterval;

            /* disable interrupts when calling pharosITimeTickAddThread */
            pharosCpuDisableInterrupts(&interruptLevel);

            /* only on next period will it be re-activated at the same point it left */
            pharosITimeTickAddThread(thread , thread->lastReplenishmentInstant ,
                                     PHAROS_TIME_TICK_AWAKEN_PERIODIC_THREAD , thread->scheduler);

            /* interrupts can now be enabled */
            pharosCpuEnableInterrupts(&interruptLevel);
        }
            /* else, check if it is sporadic */
        else if(thread->threadId.flags.isSporadic == TRUE)
        {
            /* announce error (WCET) */
            pharosSThreadErrorAnnounce(thread , PHAROS_THREAD_ERROR_WCET ,
                                       interruptedAddress , NULL);

            /* calculate the next instant the sporadic thread should awake */
            thread->lastReplenishmentInstant = MAX(thread->replenishBudgetInterval + thread->lastReplenishmentInstant ,
                                                   t + 1);

            /* disable interrupts when calling pharosITimeTickAddThread */
            pharosCpuDisableInterrupts(&interruptLevel);

            /* wake up at next MIT and execute at the same point it left at */
            pharosITimeTickAddThread(thread , thread->lastReplenishmentInstant ,
                                     PHAROS_TIME_TICK_AWAKEN_SPORADIC_THREAD_MIT , thread->scheduler);

            /* interrupts can now be enabled */
            pharosCpuEnableInterrupts(&interruptLevel);
        }
            /* must be aperiodic */
        else
        {
            /* calculate the next replenish instant */
            thread->lastReplenishmentInstant = MAX(thread->replenishBudgetInterval + thread->lastReplenishmentInstant ,
                                                   t + 1);

            /* disable interrupts when calling pharosITimeTickAddThread */
            pharosCpuDisableInterrupts(&interruptLevel);

            /* wake up at next MIT and execute at the same point it left at */
            pharosITimeTickAddThread(thread , thread->lastReplenishmentInstant ,
                                     PHAROS_TIME_TICK_AWAKEN_APERIODIC_THREAD_MIT , thread->scheduler);

            /* interrupts can now be enabled */
            pharosCpuEnableInterrupts(&interruptLevel);
        }

        /* set waiting on activation */
        pharosIThreadStateWaitActivation(&thread->state);

        /* remove running thread from ready list */
        pharosISchedRemoveReadyThread(thread);

        /* when interrupt leaves it will dispatch to highest priority ready thread */
    }
}


void pharosITimeActivateRemoteThread(ptrThreadInfo thread)
{
    /* interrupt level */
    PharosInterruptLevel level;


    /* if the thread is waiting for a timeout */
    if(pharosITimeTickAwakenIsTimeout(thread->type) == TRUE)
    {
        /* then set the timeout to TRUE */
        thread->threadFlags.unblockDueToTimeout = TRUE;

        /* disable interrupts so nothing can change the blocked queue */
        pharosCpuDisableInterrupts(&level);

        /* if waiting for a timeout then the thread is blocked. Remove the thread from the blocked queue.
         * An interesting scenario could occur here:
         * suppose a clock tick should awake two threads blocked on the same mutex with a timeout. The first thread (highest priority) will then release the
         * mutex. Since the second thread did not yet activate, it is still on the priority queue of the mutex and will get the mutex. As it gets the mutex
         * it will leave the mutex priority queue. When the first thread dispatches to the second one and the second is activated by this method, it will no
         * longer be on a blocked queue and this method will not do anything. */
        pharosIThreadQueueRemoveBlocked(thread);

        /* re-enable interrupts */
        pharosCpuEnableInterrupts(&level);

        /* unblock the thread (thread is no longer blocked on whatever item it was blocked on - semaphore, event, queue, etc) */
        pharosIThreadStateUnblock(&thread->state);
    }

    /* if waiting for activation (sporadic thread) */
    if(pharosIThreadStateIsWaitAct(&thread->state) == TRUE)
    {
        /* reset the execution time for next job */
        thread->executionTime = 0U;

        /* thread is also no longer waiting for an activation (if it was) */
        pharosIThreadStateClearWaitActivation(&thread->state);
    }

    /* thread is no longer waiting */
    pharosIThreadStateUnwait(&thread->state);

    /* make the thread ready on the core it was waiting on */
    pharosISchedAddReadyThreadRmp(thread);
}


void pharosITimeActivateThread(ptrThreadInfo thread)
{
    /* interrupt level */
    PharosInterruptLevel level;


    /* check that we are running on the correct core */
    PHAROS_ASSERT_CORE(thread->partition->core->number , "Activating thread on invalid core");

    /* if the thread is waiting for a timeout */
    if(pharosITimeTickAwakenIsTimeout(thread->type) == TRUE)
    {
        /* then set the timeout to TRUE */
        thread->threadFlags.unblockDueToTimeout = TRUE;

        /* disable interrupts so nothing can change the blocked queue */
        pharosCpuDisableInterrupts(&level);

        /* if waiting for a timeout then the thread is blocked. Remove the thread from the blocked queue 
         * (if waiting on a queue).
         * 
         * An interesting scenario could occur here:
         * suppose a clock tick should awake two threads blocked on the same mutex with a timeout. 
         * The first thread (highest priority) will then release the mutex. Since the second thread did not yet 
         * activate, it is still on the priority queue of the mutex and will get the mutex. As it gets the mutex
         * it will leave the mutex priority queue. When the first thread dispatches to the second one and the second 
         * is activated by this method, it will no longer be on a blocked queue and this method will not do anything. */
        pharosIThreadQueueRemoveBlocked(thread);

        /* unblock the thread (thread is no longer blocked on whatever item it was blocked on - semaphore, event, queue, etc) */
        pharosIThreadStateUnblock(&thread->state);

        /* re-enable interrupts */
        pharosCpuEnableInterrupts(&level);
    }

    /* if waiting for activation (sporadic thread) */
    if(pharosIThreadStateIsWaitAct(&thread->state) == TRUE)
    {
        /* reset the execution time for next job */
        thread->executionTime = 0U;

        /* thread is also no longer waiting for an activation (if it was) */
        pharosIThreadStateClearWaitActivation(&thread->state);
    }

    /* thread is no longer waiting */
    pharosIThreadStateUnwait(&thread->state);

    /* make the thread ready on the running core */
    pharosISchedAddReadyThread(thread);
}


void pharosITimeTickAdd(ptrFlatScheduler scheduler , ptrTimeTickHandler handler)
{
    /* interrupt level */
    PharosInterruptLevel level;


    /* don't allow anyone inside threads or interrupts to change the map */
    pharosCpuDisableInterrupts(&level);

    /* place the time tick handler on the free list */
    pharosITimeTickHandlerFree(scheduler , handler);

    /* check that the tree is empty (has been initialized) */
    PHAROS_ASSERT(treeMapIsEmpty(&handler->threadQueue.items) == TRUE , "Time tick handler tree not empty");

    /* interrupts can now be enabled */
    pharosCpuEnableInterrupts(&level);
}


void pharosITimeTickRemoveDeadline(ptrRtThread rtTthread)
{
    /* interrupt level */
    PharosInterruptLevel level;

    /* thread deadline handler*/
    ptrTimeTickHandler handler;


    /* don't allow anyone inside threads or interrupts to change the map */
    pharosCpuDisableInterrupts(&level);

    /* get the current thread deadline handler */
    handler = rtTthread->currentDeadlineHandler;

    /* reset the current handler */
    rtTthread->currentDeadlineHandler = NULL;


    /* if there is a deadline handler */
    if(handler != NULL)
    {
        /* remove the deadline from the handler */
        linkedListRemoveObject(&handler->deadlines , rtTthread);

        /* interrupts can now be enabled */
        pharosCpuEnableInterrupts(&level);

        /* remove the handler (if needed) */
        pharosIFlatSchedulerTimeTickHandlerRemove(rtTthread->thread.scheduler , &rtTthread->thread , handler);
    }
    else
    {
        /* interrupts can now be enabled */
        pharosCpuEnableInterrupts(&level);
    }
}


void pharosIExecuteDeadline(ptrRtThread thread)
{
    /* deadline handler to execute */
    HealthThreadErrorHandler handler;

    /* get the partition of the thread whose deadline was missed */
    ptrPartition p = thread->thread.partition;


    /* if the partition is initialized and the thread has started (is not restarting) */
    if((pharosIPartitionIsInitialized(p) == TRUE) && (pharosIThreadStateIsStarted(&thread->thread.state) == TRUE))
    {
        /* get the partition handler */
        handler = p->threadErrorHandler;

        /* if the handler exists */
        if(handler != NULL)
        {
            /* execute the application handler */
            handler(thread->thread.threadId , PHAROS_THREAD_ERROR_DEADLINE_MISS , NULL , NULL);



        }
    }
    /* else, partition or thread is restarting so don't execute the handler */

    /* remove the deadline handler */
    thread->currentDeadlineHandler = NULL;
}


void pharosITimeTickAddDeadline(ptrRtThread rTthread , ClockTick deadlineInstant)
{
    /* time tick handler */
    ptrTimeTickHandler handler;

    /* scheduler where to add the deadline */
    ptrFlatScheduler scheduler = rTthread->thread.scheduler;


    /* check that the deadline instant did not yet elapsed */
    PHAROS_ASSERT(deadlineInstant > pharosIClockTicksGet(scheduler) , "Setting invalid deadline");

    /* check that the thread does not have already an active deadline handler */
    PHAROS_ASSERT(rTthread->currentDeadlineHandler == NULL , "Thread already with deadline");

    /* check if there is another object set to awake at the same instant */
    handler = (ptrTimeTickHandler) treeMapGet(pharosIClockTickHandlersGet(scheduler) , &deadlineInstant);

    /* if there isn't */
    if(handler == NULL)
    {
        /* then get a new one */
        handler = pharosITimeTickHandlerGetNew(rTthread->thread.scheduler);

        /* make sure there is a handler */
        PHAROS_ASSERT(handler != NULL , "Invalid handler");

        /* initialize the time tick handler */
        pharosITimeInitializeTickHandler(scheduler , handler , deadlineInstant);
    }

    /* update the handler so we can remove if needed */
    rTthread->currentDeadlineHandler = handler;

    /* place the thread on the list of deadlines */
    linkedListAppendLast(&handler->deadlines , rTthread);
}


void pharosITimeTickAddTimer(ptrFlatScheduler scheduler , ptrTimer timer , ClockTick awakeTime)
{
    /* time tick handler */
    ptrTimeTickHandler handler;


    /* check if there is another object set to awake at the same instant */
    handler = (ptrTimeTickHandler) treeMapGet(pharosIClockTickHandlersGet(scheduler) , &awakeTime);

    /* if there isn't */
    if(handler == NULL)
    {
        /* then get a new one */
        handler = pharosITimeTickHandlerGetNew(scheduler);

        /* initialize the time tick handler */
        pharosITimeInitializeTickHandler(scheduler , handler , awakeTime);
    }

    /* make the thread point to the handler (useful when removing it) */
    timer->timeTickHandler = handler;

    /* place the timer on the list of timers */
    linkedListAppendLast(&handler->timers , timer);

}


void pharosITimeTickAddThread(ptrThreadInfo thread , ClockTick awakeTime , TimeTickAwakenType type ,
                              ptrFlatScheduler scheduler)
{
    /* handler to get */
    ptrTimeTickHandler handler;


    /* check that the awakeTime did not yet elapsed */
    PHAROS_ASSERT(awakeTime > pharosIClockTicksGet(scheduler) , "Setting invalid awake time");

    /* check that the thread scheduler is the scheduler being invoked */
    PHAROS_ASSERT(thread->scheduler == scheduler , "Invalid scheduler");

    /* before adding a new time tick handler to the thread we must ensure that the current tick (if any) is 
     * fully processed */
    pharosITimeTickAwakeNext(thread);

    /* check if there is another object set to awake at the same instant */
    handler = (ptrTimeTickHandler) treeMapGet(pharosIClockTickHandlersGet(scheduler) , &awakeTime);

    /* if there isn't */
    if(handler == NULL)
    {
        /* then get a new one */
        handler = pharosITimeTickHandlerGetNew(scheduler);

        /* check that the handler is valid */
        PHAROS_ASSERT(handler != NULL , "Handler is invalid");

        /* initialize the time tick handler */
        pharosITimeInitializeTickHandler(scheduler , handler , awakeTime);
    }

    /* make the thread point to the handler (useful when removing it) */
    thread->timeTickHandler = handler;

    /* set the awake type */
    thread->type = type;

    /* reset the flag indicating if it is activated */
    thread->threadFlags.hasTimeTickElapsed = FALSE;

    /* add the thread to the thread queue */
    pharosIPqAddWait(&handler->threadQueue , thread);
}


void pharosITimeTickRemoveThread(ptrThreadInfo thread)
{
    /* interrupt level */
    PharosInterruptLevel interruptLevel;

    /* thread tick handler (if any) */
    ptrTimeTickHandler handler;


    /* check that the thread is valid */
    PHAROS_ASSERT(thread != NULL , "Thread is NULL");

    /* don't allow anyone inside threads or interrupts to change the map */
    pharosCpuDisableInterrupts(&interruptLevel);

    /* get the handler */
    handler = thread->timeTickHandler;

    /* remove the handler from the thread */
    thread->timeTickHandler = NULL;

    /* if the handler exists */
    if(handler != NULL)
    {
        /* remove the thread from the thread queue */
        pharosIPqRemoveWait(thread);

        /* remove the handler (if needed) */
        pharosIFlatSchedulerTimeTickHandlerRemove(thread->scheduler , thread , handler);
    }

    /* interrupts can now be enabled */
    pharosCpuEnableInterrupts(&interruptLevel);
}


bool pharosITimeTickHandlerIsEmpty(ptrTimeTickHandler handler)
{
    /* return value of this method */
    bool result;


    /* check if there are threads to awake at the clock tick of the handler */
    if(pharosIPqIsEmpty(&handler->threadQueue) == FALSE)
    {
        /* return not empty */
        result = FALSE;
    }
        /* check if there are timers to awake at the clock tick of the handler */
    else if(linkedListIsEmpty(&handler->timers) == FALSE)
    {
        /* return not empty */
        result = FALSE;
    }
        /* check if there are deadlines to awake at the clock tick of the handler */
    else if(linkedListIsEmpty(&handler->deadlines) == FALSE)
    {
        /* return not empty */
        result = FALSE;
    }
        /* there is nothing to awake */
    else
    {
        /* return is empty */
        result = TRUE;
    }

    /* return TRUE or FALSE */
    return result;
}


void pharosITimeTickRemoveTimer(ptrTimer timer)
{
    /* interrupt level */
    PharosInterruptLevel interruptLevel;

    /* get the time tick handler of the timer */
    ptrTimeTickHandler handler = timer->timeTickHandler;


    /* don't allow anyone inside threads or interrupts to change the map */
    pharosCpuDisableInterrupts(&interruptLevel);

    /* remove the timer from the handler */
    linkedListRemoveObject(&handler->timers , timer);

    /* remove the handler (if needed) */
    pharosIFlatSchedulerTimeTickHandlerRemove(timer->partition->scheduler , NULL , handler);

    /* remove the tick handler from the timer */
    timer->timeTickHandler = NULL;

    /* interrupts can now be enabled */
    pharosCpuEnableInterrupts(&interruptLevel);
}


void pharosITimeTickAwakeNext(ptrThreadInfo thread)
{
    /* get the handler */
    ptrTimeTickHandler handler = thread->timeTickHandler;

    /* next activated thread */
    ptrThreadInfo activatedThread;


    /* if the handler exists and the time tick has elapsed */
    if((handler != NULL) && (thread->threadFlags.hasTimeTickElapsed == TRUE))
    {
        /* then activate highest priority thread of the handler */
        activatedThread = pharosIFlatSchedulerTimeActivateHighestPriorityThread(handler);

        /* remove the time tick (activatedThread is never NULL here since the handler must have at least one thread to execute */
        if(pharosIFlatSchedulerTimeTickHandlerRemove(activatedThread->scheduler , activatedThread , handler) == FALSE)
        {
            /* set that the time has elapsed (this will awaken other threads that are awaken at the same handler when 
             * doing a context switch) */
            activatedThread->threadFlags.hasTimeTickElapsed = TRUE;
        }

        /* reset the time tick handler */
        thread->timeTickHandler = NULL;

        /* reset the is activated flag */
        thread->threadFlags.hasTimeTickElapsed = FALSE;
    }
}


intptr_t pharosIClockTickHandlerComp(const ptrClockTick a , const ptrClockTick b)
{
    /* result of the method */
    intptr_t result;


    /* if a < b */
    if(*a < *b)
    {
        /* then b is bigger */
        result = -1;
    }
        /* if a > b */
    else if(*a > *b)
    {
        /* then a is bigger */
        result = 1;
    }
        /* if a == b */
    else
    {
        /* they are equal */
        result = 0;
    }

    /* return the result */
    return result;
}


bool pharosITimeTickAwakenIsTimeout(TimeTickAwakenType type)
{
    /* if the type is bigger than the first timeout then it is timeout */
    return type >= PHAROS_TIME_TICK_AWAKEN_THREAD_EVENT_RECEIVE_TIMEOUT ? TRUE : FALSE;
}
