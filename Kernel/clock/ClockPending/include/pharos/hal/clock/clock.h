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


#ifndef PHAROS_TIMER_PENDING_CLOCK_H
#define PHAROS_TIMER_PENDING_CLOCK_H

#include <pharos/kernel/clock/clock.h>

/**
 * Read the HW time counter for the clock tick. 
 * 
 * Gets the number of microseconds to the next clock tick.
 * 
 * The HAL should provide this function 
 * 
 * @return returns the HW internal counter (in microseconds)
 */
uint32_t pharosCpuClockReadCounter(void);


/**
 * Determines if the clock interrupt is pending
 * 
 * The HAL should provide this function 
 *
 * @return returns TRUE if the clock interrupt is pending and FALSE otherwise
 */
bool pharosCpuClockIsrIsPending(void);


/**
 * Read the clock value in raw 
 * 
 * @note CPUs that have a clock pending must also implement this function
 * 
 * @return returns the number of microseconds since the last tick (raw)
 */
uint64_t pharosCpuClockReadCounterRaw(void);


/**
 * Get the number of microseconds per tick (raw)
 * 
 * @note CPUs that have a clock pending must also implement this function
 * 
 * @return returns the number of microseconds per tick (raw)
 */
uint64_t pharosCpuClockMicrosecondsPerTickRaw(void);


/* PHAROS_TIMER_PENDING_CLOCK_H */
#endif 
