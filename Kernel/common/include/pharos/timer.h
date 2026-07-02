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


#ifndef PHAROS_API_TIMER_H
#define PHAROS_API_TIMER_H


#include <pharos/declarations.h>
#include <pharos/kernel/timer/declarations.h>
#include <pharos/kernel/time/declarations.h>


/**
 * Schedules the specified timer to run (periodically or one-shot).
 * 
 * @param number timer index on the calling partition table of timers
 * @param period period of the timer (must be larger than or equal to the timer configured period)
 * @param argument argument to pass to the timer handler
 * @param isPeriodic boolean flag to indicate if the timer should be automatically fired periodically or is a 
 * one-shot timer
 * 
 * @return returns success or the appropriate error
 */
PharosTimerStartR pharosTimerStart(TimerNumber number , ClockTick period,  uint64_t argument , bool isPeriodic);


/**
 * Stops a timer from executing
 *
 * @param timer timer id of the timer to stop (corresponds to the index on the timer array)
 * 
 * @return returns success, already stopped or invalid timer id
 */
PharosTimerStopR pharosTimerStop(TimerNumber timer);


#endif /* TIMER_H */
