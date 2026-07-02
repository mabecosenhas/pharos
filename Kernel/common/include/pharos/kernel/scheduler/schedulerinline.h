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


#ifndef PHAROS_FLATINLINE_H
#define PHAROS_FLATINLINE_H

#include <pharos/kernel/scheduler/scheduler.h>
#include <pharos/kernel/assert/assert.h>
#include <pharos/kernel/core/coreconfinline.h>


INLINE ClockTick pharosIClockTickNextGet(ptrFlatScheduler scheduler)
{
    /* get the next clock tick on the scheduler */
    return scheduler->nextTicks;
}


INLINE void pharosIClockTickNextSet(ptrFlatScheduler scheduler , ClockTick nextTick)
{
    /* set the next clock tick on the scheduler */
    scheduler->nextTicks = nextTick;
}


INLINE ptrTreeMap pharosIClockTickHandlersGet(ptrFlatScheduler scheduler)
{
    /* get the tree map of clock tick handlers of the scheduler */
    return &scheduler->handlers;
}


INLINE bool pharosISchedulerIsHierarchical(void)
{
    /* check the scheduler type for the running core */
    return pharosICore()->scheduler->type == PHAROS_SCHEDULER_HIERARCHICAL ? TRUE : FALSE;
}


INLINE ClockTick pharosIClockTicksGet(ptrFlatScheduler scheduler)
{
    /* get the clock ticks elapsed on the running core */
    return scheduler->ticks;
}


INLINE ClockTick pharosIClockTicksInc(ClockTick increment , ptrCore core)
{
    /* get the running core scheduler */
    ptrFlatScheduler scheduler = core->runningFlatScheduler;

    
    PHAROS_ASSERT(scheduler != NULL , "Scheduler is incorrect");

    /* increment the clock ticks elapsed on the running core */
    scheduler->ticks += increment;

    /* return the current number of ticks */
    return scheduler->ticks;
}


#endif /* PHAROS_FLATINLINE_H */
