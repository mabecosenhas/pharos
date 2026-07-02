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


#ifndef PHAROS_TIMER_H
#define PHAROS_TIMER_H


#include <pharos/kernel/timer/declarations.h>
#include <pharos/kernel/collection/treemap.h>
#include <pharos/kernel/time/time.h>
#include <pharos/kernel/object/object.h>
#include <pharos/declarations.h>

struct Timer
{
    /**
     * @brief node on the map of timers activated at the same instant
     */
    LinkedListNode node;

    /**
     * pointer to the time tick handler (if in use)
     */
    ptrTimeTickHandler timeTickHandler;

    /**
     * @brief time tick handler used to awake this timer (and potentially others as well)
     */
    TimeTickHandler tickHandler;

    /**
     * Minimum period of the timer. A timer cannot be started with a period smaller than this number
     */
    ClockTick minimumPeriod;

    /**
     * Current period
     */
    ClockTick currentPeriod;

    /**
     * body of the timer to be called when the awake time is reached 
     */
    TimerHandler body;

    /**
     * argument to pass to the body
     */
    uint64_t argument;

    /**
     * partition that holds the timer.
     * This is used to determine which partition is running when the timer is executing
     */
    ptrPartition partition;

    struct
    {
        /**
         * flag indicating if the timer is periodic 
         */
        unsigned int isPeriodic : 1;
    } flags;

};


/**
 * Initialize all timers of the partition 
 * 
 * @param table timer table of the partition
 * @param partition partition that owns the timers
 * @param restart flag indicating if initializing due to a restart of the partition or a normal start (first time start)
 */
void pharosITimersInitialize(const ptrTimerTable table , ptrPartition partition , bool restart);


/**
 * Executes the specified timer
 * 
 * @contraint should be called once the timer is fired
 * 
 * @param timer timer to execute
 */
void pharosIExecuteTimer(ptrTimer timer);


/**
 * Syscall for the pharosTimerStart
 * 
 * Starts a timer with the specified number inside the calling thread partition.
 * 
 * @param number timer index on the calling partition table of timers
 * @param period period of the timer (must be larger than or equal to the timer configured period)
 * @param argument argument to pass to the timer handler
 * @param isPeriodic boolean flag to indicate if the timer should be automatically fired periodically or is a 
 * one-shot timer
 * 
 * @return returns success or the error while attempting to start the timer
 */
PharosTimerStartR pharosSTimerStart(TimerNumber number , ClockTick period , uint64_t argument , bool isPeriodic);


/**
 * Syscall for the pharosTimerStop
 * 
 * Stops the timer 
 * 
 * @param timerId timer number in the calling thread partition to stop
 * 
 * @return returns success or the error while attempting to stop the timer
 */
PharosTimerStopR pharosSTimerStop(TimerNumber timerId);


/**
 * Executes a timer 
 * 
 * @param timer timer to execute 
 */
void pharosITimerExecute(ptrTimer timer);


/**
 * gets the running timer partition 
 * 
 * @contraint assumes that a timer is running
 * 
 * @return returns the running timer partition
 */
ptrPartition pharosITimerRunningPartition(void);


/**
 * Reset the timers of the partition 
 * 
 * @param p partition
 */
void pharosIPartitionTimersReset(ptrPartition p);

#endif /* TIMER_H */
