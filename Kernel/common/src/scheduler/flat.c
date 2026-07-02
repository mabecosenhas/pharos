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
#include <pharos/kernel/thread/rtthread.h>
#include <pharos/kernel/scheduler/scheduler.h>
#include <pharos/kernel/thread/priority.h>
#include <pharos/kernel/core/core.h>
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/hal/cpu/sections.h>
#include <pharos/kernel/scheduler/schedulerinline.h>
#include <pharos/kernel/assert/assert.h>


/**
 * execute all timers that are supposed to be activated at the specified time tick
 *
 * @param handler time tick
 */
static void pharosIFlatSchedulerExecuteTimers(ptrTimeTickHandler handler);



/**
 * Execute the deadlines associated with the time tick handler
 * 
 * @param handler time tick handler
 */
static void pharosIFlatSchedulerExecuteDeadlines(ptrTimeTickHandler handler);


/**
 * lookup table of which is the highest bit set for each 256 possible bits
 */
const uint8_t pharosVhighestBit[] = {
    0 , 0 , 1 , 0 , 2 , 0 , 1 , 0 , 3 , 0 , 1 , 0 , 2 , 0 , 1 , 0 ,
    4 , 0 , 1 , 0 , 2 , 0 , 1 , 0 , 3 , 0 , 1 , 0 , 2 , 0 , 1 , 0 ,
    5 , 0 , 1 , 0 , 2 , 0 , 1 , 0 , 3 , 0 , 1 , 0 , 2 , 0 , 1 , 0 ,
    4 , 0 , 1 , 0 , 2 , 0 , 1 , 0 , 3 , 0 , 1 , 0 , 2 , 0 , 1 , 0 ,
    6 , 0 , 1 , 0 , 2 , 0 , 1 , 0 , 3 , 0 , 1 , 0 , 2 , 0 , 1 , 0 ,
    4 , 0 , 1 , 0 , 2 , 0 , 1 , 0 , 3 , 0 , 1 , 0 , 2 , 0 , 1 , 0 ,
    5 , 0 , 1 , 0 , 2 , 0 , 1 , 0 , 3 , 0 , 1 , 0 , 2 , 0 , 1 , 0 ,
    4 , 0 , 1 , 0 , 2 , 0 , 1 , 0 , 3 , 0 , 1 , 0 , 2 , 0 , 1 , 0 ,
    7 , 0 , 1 , 0 , 2 , 0 , 1 , 0 , 3 , 0 , 1 , 0 , 2 , 0 , 1 , 0 ,
    4 , 0 , 1 , 0 , 2 , 0 , 1 , 0 , 3 , 0 , 1 , 0 , 2 , 0 , 1 , 0 ,
    5 , 0 , 1 , 0 , 2 , 0 , 1 , 0 , 3 , 0 , 1 , 0 , 2 , 0 , 1 , 0 ,
    4 , 0 , 1 , 0 , 2 , 0 , 1 , 0 , 3 , 0 , 1 , 0 , 2 , 0 , 1 , 0 ,
    6 , 0 , 1 , 0 , 2 , 0 , 1 , 0 , 3 , 0 , 1 , 0 , 2 , 0 , 1 , 0 ,
    4 , 0 , 1 , 0 , 2 , 0 , 1 , 0 , 3 , 0 , 1 , 0 , 2 , 0 , 1 , 0 ,
    5 , 0 , 1 , 0 , 2 , 0 , 1 , 0 , 3 , 0 , 1 , 0 , 2 , 0 , 1 , 0 ,
    4 , 0 , 1 , 0 , 2 , 0 , 1 , 0 , 3 , 0 , 1 , 0 , 2 , 0 , 1 , 0 ,
};


/**
 * idle threads (one for each core)
 */
KERNEL_SECTION_USS ptrThreadInfo idleThreads[PHAROS_NUMBER_CORES];


/**
 * Get the list of free time tick handlers of the scheduler
 * 
 * @param scheduler flat scheduler
 * 
 * @return returns the list of free time tick handlers of the scheduler
 */
INLINE ptrLinkedList pharosITimeTickFreeHandlersGet(ptrFlatScheduler scheduler)
{
    /* get the simple list of free time tick handlers on the running core */
    return &scheduler->freeTimeTickHandlers;
}


void pharosISchedFlatInit(ptrFlatScheduler scheduler , ptrCore core)
{
    /* priority iterator */
    uint16_t i;


    /* initialize the core */
    scheduler->scheduler.core = core;

    /* set the type of scheduler */
    scheduler->scheduler.type = PHAROS_SCHEDULER_FLAT;

    /* for every priority */
    for(i = 0U; i < MAXIMUM_NUMBER_PRIORITIES; i++)
    {
        /* initialize the linked list of threads */
        linkedListInitialize(&scheduler->pharosVThreadsReady[i] , OFFSETOF(ThreadInfo , readyNode));
    }

    /* initialize the tree map of internal handlers */
    treeMapInitialize(&scheduler->handlers , (ComparatorMethod) pharosIClockTickHandlerComp , OFFSETOF(TimeTickHandler , node));

    /* initialize the list of free time tick handlers */
    linkedListInitialize(&scheduler->freeTimeTickHandlers , OFFSETOF(TimeTickHandler , usedNode));

    /* initialize the priority bit map */
    pharosIPriorityBitMapInit(&scheduler->pharosVbitMap);

    /* initialize the next tick to schedule a thread/timer */
    scheduler->nextTicks = PHAROS_CLOCK_TICK_MAXIMUM;

    /* reset the clock tick */
    scheduler->ticks = 0U;

    /* set the latest processed tick */
    scheduler->lastProcessedTick = 0U;

    /* add the extra tick handler to make sure when re-scheduling timers there is one spare */
    pharosIFlatSchedulerTimeTickInit(scheduler , &scheduler->tickHandler);

    /* initialize the list of installed interrupts */
    linkedListInitialize(&scheduler->interruptsInstalled , OFFSETOF(PartitionIsrConf , nodeInstalled));
}


void pharosIFlatSchedulerTimeTickInit(ptrFlatScheduler scheduler , ptrTimeTickHandler handler)
{
    /* initialize the thread queue (sorted by priority always) */
    pharosIPqInitialize(&handler->threadQueue);

    /* initialize the list of timers that awake at this instant */
    linkedListInitialize(&handler->timers , OFFSETOF(Timer , node));

    /* initialize the list of deadlines that occur at this instant */
    linkedListInitialize(&handler->deadlines , OFFSETOF(RtThread , deadlineTickNode));

    /* mark it as being on the free list */
    handler->flags.isOnFreeList = TRUE;

    /* and add it to the list of free handlers */
    linkedListAppendFirst(pharosITimeTickFreeHandlersGet(scheduler) , handler);
}


/**
 * find the priority map module and the shift
 * 
 * @param priority priority to find out the module
 * @param module module of the bit map
 * @param shift number of bits to shift inside the bit module
 */
INLINE void schedulerFindPriorityMapNumber(ThreadPriority priority , uint8_t *module , uint8_t *shift)
{
    /* if the 0 < priority < 64 */
    if(priority < PRIORITY_BIT_MAP_NUMBER_BITS)
    {
        /* set the module to 0 (index is 0 on the 64 bit priority map) */
        *module = 0U;

        /* the shift inside the index is the same as the priority */
        *shift = (uint8_t) priority;
    }
        /* if the 64 <= priority < 127 */
    else if(priority < 2U * PRIORITY_BIT_MAP_NUMBER_BITS)
    {
        /* set the module to 1 (index is 1 on the 64 bit priority map) */
        *module = 1U;

        /* the shift inside the index is the same as the priority minus 64 */
        *shift = (uint8_t) (priority - 64U);
    }
        /* if the 127 <= priority < 191 */
    else if(priority < 3U * PRIORITY_BIT_MAP_NUMBER_BITS)
    {
        /* set the module to 2 (index is 2 on the 64 bit priority map) */
        *module = 2U;

        /* the shift inside the index is the same as the priority minus 2*64 */
        *shift = (uint8_t) (priority - 128U);
    }
        /* else then its 3 */
    else
    {
        /* set the module to 3 (index is 3 on the 64 bit priority map) */
        *module = 3U;

        /* the shift inside the index is the same as the priority minus 3*64 */
        *shift = (uint8_t) (priority - 192U);
    }
}


INLINE void checkDispatchNeeded(const ptrThreadInfo threadUpdated)
{
    /* check if the thread updated has a higher priority than the executing thread */
    if((threadUpdated != pharosISchedGetRunningThread()) &&
       (threadUpdated->currentPriority < pharosISchedGetRunningThread()->currentPriority))
    {
        /* then a context switch is required */
        pharosIThreadContextSwitchNeeded(TRUE);
    }
}


INLINE void schedulerAddPriority(ThreadPriority priority , ptrFlatScheduler scheduler)
{
    /* index in the 64 bit map */
    uint8_t mapNumber;

    /* shit inside the index of the 64 bit map */
    uint8_t shift;


    /* find the index and shit inside the 64 bit map for the priority */
    schedulerFindPriorityMapNumber(priority , &mapNumber , &shift);

    /* add the bit to the bit map */
    scheduler->pharosVbitMap.map64[mapNumber] |= (((uint64_t) 1U << (shift)));
}


INLINE void schedulerRemovePriority(ptrFlatScheduler scheduler , ThreadPriority priority)
{
    /* index in the 64 bit map */
    uint8_t mapNumber;

    /* shit inside the index of the 64 bit map */
    uint8_t shift;


    /* find the index and shit inside the 64 bit map for the priority */
    schedulerFindPriorityMapNumber(priority , &mapNumber , &shift);

    /* remove the bit from the bit map */
    scheduler->pharosVbitMap.map64[mapNumber] &= ~(((uint64_t) 1U << (shift)));
}


INLINE uint8_t schedulerFindLowestBit(uint64_t module)
{
    uint32_t aux;
    uint16_t aux2;
    uint8_t aux3;
    uint8_t delta;


    /* if the first bit is on the lowest 32 bits out of 64 */
    if((uint32_t) (module & 0xFFFFFFFFU) != 0U)
    {
        /* get the first 32 bits  */
        aux = (uint32_t) (module & 0xFFFFFFFFU);

        /* there is no delta */
        delta = 0U;
    }
    else
    {
        /* shift by 32 */
        aux = (uint32_t) (module >> 32U);

        /* set the delta to 32 */
        delta = 32U;
    }

    /* if the first bit is on the lowest 16 bits out of 32 */
    if((uint16_t) (aux & 0xFFFFU) != 0U)
    {
        /* first bit is in the lowest 16 bits */
        aux2 = (uint16_t) (aux & 0xFFFFU);
    }
    else
    {
        /* search in the highest 16 bits */
        aux2 = (uint16_t) (aux >> 16U);

        /* increase the delta */
        delta += 16U;
    }

    /* if the first bit is on the lowest 8 bits out of 16 */
    if((uint8_t) (aux2 & 0xFFU) != 0U)
    {
        /* first bit is in the lowest 8 bits */
        aux3 = (uint8_t) (aux2 & 0xFFU);
    }
    else
    {
        /* first bit is in the highest 8 bits */
        aux3 = (uint8_t) (aux2 >> 8U);

        /* increase the delta */
        delta += 8U;
    }

    /* check which bit is set */
    return pharosVhighestBit[aux3] + delta;
}


int32_t schedulerGetHighestPriority(ptrFlatScheduler flat)
{
    /* highest priority */
    int32_t result;


    /* check if there is a bit set on the highest priority module */
    if(flat->pharosVbitMap.map64[0] != 0U)
    {
        /* there is, get the highest priority bit */
        result = schedulerFindLowestBit(flat->pharosVbitMap.map64[0]);
    }
        /* check if a bit is set on the second 64 priorities */
    else if(flat->pharosVbitMap.map64[1] != 0U)
    {
        /* there is, get the highest priority bit */
        result = schedulerFindLowestBit(flat->pharosVbitMap.map64[1]) + 64U;
    }
        /* check if a bit is set on the third 64 priorities */
    else if(flat->pharosVbitMap.map64[2] != 0U)
    {
        /* there is, get the highest priority bit */
        result = schedulerFindLowestBit(flat->pharosVbitMap.map64[2]) + 128U;
    }
        /* check if a bit is set on the fourth 64 priorities */
    else if(flat->pharosVbitMap.map64[3] != 0U)
    {
        /* must be in the lowest priority bit map */
        result = schedulerFindLowestBit(flat->pharosVbitMap.map64[3]) + 192U;
    }
    else
    {
        /* did not find any ready thread (should go idle thread) */
        result = -1;
    }

    /* return the highest priority on the map */
    return result;
}


void pharosISchedIdleThreadSet(CoreNumber runningCore , ptrThreadInfo idle)
{
    /* check that the function is running on the core of the current scheduler */
    PHAROS_ASSERT_CORE(runningCore , "Incorrect core running");

    /* set the idle thread for the running core */
    idleThreads[runningCore] = idle;
}


void pharosISchedAddReadyThread(ptrThreadInfo thread)
{
    /* interrupt nest number */
    PharosInterruptLevel level;

    /* get the thread scheduler (can only make a thread ready on its own scheduler) */
    ptrFlatScheduler scheduler = thread->scheduler;

    /* list of threads with the same priority */
    ptrLinkedList list;


    /* check that the function is running on the core of the current scheduler */
    PHAROS_ASSERT_CORE(thread->partition->core->number , "Incorrect core running");

    /* disable interrupts: cannot allow an interrupt or another thread to change the _threadsReady at the same time */
    pharosCpuDisableInterrupts(&level);

    /* get the list of threads with the same priority as the thread (inside the critical section to make sure the 
     * current priority does not change) */
    list = &scheduler->pharosVThreadsReady[thread->currentPriority];

    /* check if the thread is not suspended (if is suspended then don't add to ready queue) */
    if(pharosIThreadStateIsReady(&thread->state) == TRUE)
    {
        /* check if thread is already in the ready list */
        if(thread->threadFlags.isOnReadyList == FALSE)
        {
            /* set the thread has being on the ready list */
            thread->threadFlags.isOnReadyList = TRUE;

            /* if there is no ready thread with the same priority */
            if(linkedListIsEmpty(list) == TRUE)
            {
                /* add the priority to the bit map */
                schedulerAddPriority(thread->currentPriority , scheduler);
            }

            /* add the thread to the last position on the list */
            linkedListAppendLast(list , thread);

            /* check if the new ready thread has a higher priority than the executing thread */
            checkDispatchNeeded(thread);
        }
        /* else, don't anything since the thread is already on the ready list */
    }

    /* re-enable interrupts */
    pharosCpuEnableInterrupts(&level);
}


void pharosISchedUpdateThreadPrio(ptrThreadInfo thread , ThreadPriority newPriority)
{
    /* save the current priority to be compared later on */
    ThreadPriority oldPriority = thread->currentPriority;


    /* if the priority is the same then don't bother updating it */
    if(oldPriority != newPriority)
    {
        /* remove the thread with the current priority from the list */
        pharosISchedRemoveReadyThread(thread);

        /* update the thread priority */
        thread->currentPriority = newPriority;

        /* add the thread on the correct position */
        pharosISchedAddReadyThread(thread);

        /* update the priority */
        checkDispatchNeeded(thread);

        /* if the thread changing its priority is the running thread */
        if(thread == pharosISchedGetRunningThread() && oldPriority > newPriority)
        {
            /* if the running thread increased its priority then there is no need to do a context switch */
            pharosIThreadContextSwitchNeeded(FALSE);
        }
    }
}


ptrThreadInfo pharosISchedGetHighestReady(ptrFlatScheduler flat)
{
    /* highest priority ready thread */
    ptrThreadInfo highest;

    /* get the highest priority ready thread */
    int32_t highestPriority = schedulerGetHighestPriority(flat);


    /* if there are no "standard" threads ready */
    if(highestPriority < 0)
    {
        /* then return the idle thread */
        highest = idleThreads[pharosICoreRunning()];
    }
    else
    {
        /* get the thread with the highest priority */
        highest = (ptrThreadInfo) linkedListGetFirst(&flat->pharosVThreadsReady[highestPriority]);
    }

    /* get the first thread on list of threads with the highest priority which are ready */
    return highest;
}


void pharosISchedRemoveReadyThread(ptrThreadInfo thread)
{
    /* interrupt level */
    PharosInterruptLevel level;

    /* get the scheduler where the thread could be ready */
    ptrFlatScheduler scheduler = thread->scheduler;


    /* disable interrupts: cannot allow an interrupt or another thread to change the scheduler->pharosVThreadsReady
     * at the same time */
    pharosCpuDisableInterrupts(&level);

    /* if the thread is on the thread ready list */
    if(thread->threadFlags.isOnReadyList == TRUE)
    {
        /* then update its state */
        thread->threadFlags.isOnReadyList = FALSE;

        /* remove the thread from its list */
        linkedListRemoveObject(&scheduler->pharosVThreadsReady[thread->currentPriority] , thread);

        /* if there are no more threads with the same priority */
        if(linkedListIsEmpty(&scheduler->pharosVThreadsReady[thread->currentPriority]) == TRUE)
        {
            /* then remove the priority from the map */
            schedulerRemovePriority(scheduler , thread->currentPriority);
        }

        /* if the thread removed from ready is the running thread */
        if(thread == pharosISchedGetRunningThread())
        {
            /* then a context switch is required */
            pharosIThreadContextSwitchNeeded(TRUE);
        }
    }
    /* else, don't do anything */

    /* re-enable interrupts */
    pharosCpuEnableInterrupts(&level);
}


ptrTimeTickHandler pharosITimeTickHandlerGetNew(ptrFlatScheduler scheduler)
{
    /* time tick handler to get */
    ptrTimeTickHandler result;


    /* check if running in the scheduler core */
    PHAROS_ASSERT_CORE(scheduler->scheduler.core->number , "Invalid core");

    /* get the first item on the list */
    result = (ptrTimeTickHandler) linkedListRemoveFirst(pharosITimeTickFreeHandlersGet(scheduler));

    /* check (in debug mode) that the time tick handler is not NULL */
    PHAROS_ASSERT(result != NULL , "Could not get new time tick handler");

    /* check (in debug mode) that the time tick handler is not NULL */
    PHAROS_ASSERT(result->flags.isOnFreeList == TRUE , "New time tick handler is not free");

    /* mark the handler has not being on the free list */
    result->flags.isOnFreeList = FALSE;

    /* return the handler got */
    return result;
}


void pharosITimeTickHandlerFree(ptrFlatScheduler scheduler , ptrTimeTickHandler handler)
{
    /* check that the handler is valid */
    PHAROS_ASSERT(handler != NULL , "Handler is invalid");

    /* check if running in the scheduler core */
    PHAROS_ASSERT_CORE(scheduler->scheduler.core->number , "Invalid core");

    /* check if it not already free (it could be free if the handler of the thread->timeTickHandler was not yet reset, 
     * or the deadline tick handler already removed from the list of deadlines but not actually removed the handler) */
    if(handler->flags.isOnFreeList == FALSE)
    {
        /* mark it as being on the free list */
        handler->flags.isOnFreeList = TRUE;

        /* and add it to the list of free handlers */
        linkedListAppendFirst(pharosITimeTickFreeHandlersGet(scheduler) , handler);
    }

}


void pharosITimeTickFlatScheduler(ptrFlatScheduler scheduler)
{
    /* handler to execute */
    ptrTimeTickHandler handler;

    /* highest priority thread that activates at the clock tick */
    ptrThreadInfo activatedThread;


    /* if there is a handler waiting to be executed and its time to execute has already passed */
    if(pharosIClockTickNextGet(scheduler) <= pharosIClockTicksGet(scheduler))
    {
        /* remove the first handler (should be removed right away so the tick handler can be used by any timer) */
        handler = treeMapRemoveFirst(pharosIClockTickHandlersGet(scheduler));

        /* check that we obtained the handler */
        PHAROS_ASSERT(handler != NULL , "Handler is invalid");

        /* execute the timers */
        pharosIFlatSchedulerExecuteTimers(handler);

        /* execute the deadlines */
        pharosIFlatSchedulerExecuteDeadlines(handler);

        /* now activate the highest priority thread that is going to be made ready on this clock tick */
        activatedThread = pharosIFlatSchedulerTimeActivateHighestPriorityThread(handler);

        /* remove the time tick */
        if(pharosIFlatSchedulerTimeTickHandlerRemove(scheduler , activatedThread , handler) == TRUE)
        {
            /* no more threads to activate at this clock tick */

            /* if activated a thread (not only just timers or deadlines) */
            if(activatedThread != NULL)
            {
                /* reset the activated thread thread tick handler */
                activatedThread->timeTickHandler = NULL;

                /* reset its flag */
                activatedThread->threadFlags.hasTimeTickElapsed = FALSE;
            }
        }
        else
        {
            /* if activated a thread (not only just timers or deadlines) */
            if(activatedThread != NULL)
            {
                /* set that the time has elapsed (this will awaken other threads that are awaken at the same handler when 
                 * doing a context switch) */
                activatedThread->threadFlags.hasTimeTickElapsed = TRUE;
            }
        }
    }
}


bool pharosIFlatSchedulerTimeTickHandlerRemove(ptrFlatScheduler scheduler , ptrThreadInfo thread ,
                                               ptrTimeTickHandler handler)
{
    /* interrupt level */
    PharosInterruptLevel level;

    /* flag indicating that the handler is empty (or not) meaning it has no other threads left to activate */
    bool isEmpty = FALSE;

    /* map of time tick handlers */
    ptrTreeMap map = pharosIClockTickHandlersGet(scheduler);


    /* don't allow anyone inside threads or interrupts to change the map */
    pharosCpuDisableInterrupts(&level);

    /* if there is nothing left to awake at the handler clock tick (timers, deadlines and threads where executed and 
     * removed already, but maybe some threads are left to be activated) */
    if(pharosITimeTickHandlerIsEmpty(handler) == TRUE)
    {
        /* then remove the handler from the map of handlers to execute */
        treeMapRemove(map , &handler->awakeTime);

        /* update the instant of the next clock tick to execute */
        pharosITimeUpdateNextClockTick(scheduler , handler->awakeTime);

        /* flag that the handler is empty */
        isEmpty = TRUE;

        /* free the handler so it can be re-used 
         * will not free a handler that is the thread->otherPartitionTimeTickHandler since the thread is being removed */
        pharosITimeTickHandlerFree(scheduler , handler);
    }

    /* if removing a thread */
    if(thread != NULL)
    {
        /* reset the thread wait queue (thread no longer waiting to be awaken by a clock tick) */
        thread->waitQueue = NULL;
    }

    /* Update the next tick to be processed with the information of which handler was just removed */
    pharosITimeUpdateNextClockTick(scheduler , handler->awakeTime);

    /* interrupts can now be enabled */
    pharosCpuEnableInterrupts(&level);

    /* return TRUE if no more threads to execute at the clock tick */
    return isEmpty;
}


ptrThreadInfo pharosIFlatSchedulerTimeActivateHighestPriorityThread(ptrTimeTickHandler handler)
{
    /* first thread on the list of the highest priority threads to be activated */
    ptrThreadInfo thread;

    /* interrupt level */
    PharosInterruptLevel level;


    /* disable interrupts so nothing can change list of timers (a higher priority interrupt) */
    pharosCpuDisableInterrupts(&level);

    /* find the highest priority thread on the handler priority queue */
    thread = pharosIPqRemoveHighest(&handler->threadQueue);

    /* if there are threads to activate (if only timers are activated then this could be false) */
    if(thread != NULL)
    {
        /* re-enable interrupts */
        pharosCpuEnableInterrupts(&level);

        /* activate the thread */
        pharosITimeActivateThreadRmp(thread);
    }
    else
    {
        /* re-enable interrupts */
        pharosCpuEnableInterrupts(&level);
    }

    /* return the thread activated */
    return thread;
}


void pharosIFlatSchedulerExecuteDeadlines(ptrTimeTickHandler handler)
{
    /* get the first thread whose deadline was just triggered */
    ptrRtThread thread = (ptrRtThread) linkedListRemoveFirst(&handler->deadlines);


    /* loop for all threads with deadlines inside the handler */
    while(thread != NULL)
    {
        /* execute the deadline */
        pharosIExecuteDeadline(thread);

        /* get the next thread with a deadline to trigger */
        thread = linkedListRemoveFirst(&handler->deadlines);
    }
}


void pharosIFlatSchedulerExecuteTimers(ptrTimeTickHandler handler)
{
    /* interrupt level */
    PharosInterruptLevel level;

    /* timer iterator*/
    ptrTimer timer;


    /* disable interrupts so nothing can change list of timers (a higher priority interrupt) */
    pharosCpuDisableInterrupts(&level);

    /* get the first timer to execute */
    timer = (ptrTimer) linkedListRemoveFirst(&handler->timers);

    /* re-enable interrupts */
    pharosCpuEnableInterrupts(&level);

    /* loop for all timers inside the handler */
    while(timer != NULL)
    {
        /* reset the tick handler before executing the timer so it can be re-scheduled if wanted */
        timer->timeTickHandler = NULL;

        /* execute the timer */
        pharosITimerExecute(timer);

        /* disable interrupts so nothing can change list of timers (a higher priority interrupt) */
        pharosCpuDisableInterrupts(&level);

        /* get next timer */
        timer = (ptrTimer) linkedListRemoveFirst(&handler->timers);

        /* re-enable interrupts */
        pharosCpuEnableInterrupts(&level);
    }
}
