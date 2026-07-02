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


#ifndef PHAROS_TIME_DECLARATIONS_H
#define PHAROS_TIME_DECLARATIONS_H


#include <pharos/hal/hal.h>

/**
 * Maximum possible value for a clock tick (note that placing this value is the same as stating that the timeout is 
 * infinite (in practical terms, if an application places this value it would have to wait for thousands of years, 
 * even with a very fast clock tick)
 */
#define PHAROS_CLOCK_TICK_MAXIMUM       (0xffffffffffffffffU)


/**
 * value to place on the timeout to not block
 */
#define PHAROS_TIMEOUT_ZERO             (0U)

/**
 * value to place on the timeout to block forever
 */
#define PHAROS_TIMEOUT_INFINITE         (PHAROS_CLOCK_TICK_MAXIMUM)


typedef struct TimeTickHandler TimeTickHandler , *ptrTimeTickHandler;


/**
 * A clock tick measures the passage of time since boot. Being a 64-bit variable, it can last for thousands of years, 
 * even with a very fast clock tick (e.g. 1 us).
 */
typedef uint64_t ClockTick , *ptrClockTick;


/**
 * The timeout is measured in clock ticks 
 */
typedef ClockTick Timeout;

/**
 * When a clock tick occurs and awaken a thread, this type holds the reason of the awakening
 */
typedef enum
{
    /**
     * periodic thread is waiting for the next replenishment instant
     */
    PHAROS_TIME_TICK_AWAKEN_PERIODIC_THREAD = 0 ,

    /**
     * sporadic thread is waiting for the next replenishment instant
     */
    PHAROS_TIME_TICK_AWAKEN_SPORADIC_THREAD_MIT = 1 ,

    /**
     * aperiodic thread is waiting for the next replenishment instant
     */
    PHAROS_TIME_TICK_AWAKEN_APERIODIC_THREAD_MIT = 2 ,

    /**
     * thread is waiting for a pharosWait to end
     */
    PHAROS_TIME_TICK_AWAKEN_SLEEP = 3 ,

    /************************************************************
     * From here to the end of the enum only have timeouts
     ***********************************************************/

    /**
     * timeout waiting for an event
     */
    PHAROS_TIME_TICK_AWAKEN_THREAD_EVENT_RECEIVE_TIMEOUT = 4 ,

    /**
     * Timeout waiting for a mutex obtain
     */
    PHAROS_TIME_TICK_AWAKEN_THREAD_SEMAPHORE_MUTEX_OBTAIN_TIMEOUT = 5 ,

    /**
     * Timeout waiting for a lw message queue receive
     */
    PHAROS_TIME_TICK_AWAKEN_THREAD_LIGHTWEIGHT_QUEUE_RECEIVE_TIMEOUT = 6 ,

    /**
     * Timeout waiting for a semaphore counting obtain
     */
    PHAROS_TIME_TICK_AWAKEN_THREAD_SEMAPHORE_COUNTING_OBTAIN_TIMEOUT = 7 ,

    /**
     * Timeout waiting for a semaphore ceiling obtain
     */
    PHAROS_TIME_TICK_AWAKEN_THREAD_SEMAPHORE_CEILING_OBTAIN_TIMEOUT = 8 ,

    /**
     * Timeout waiting for a hw queue receive
     */
    PHAROS_TIME_TICK_AWAKEN_THREAD_HEAVYWEIGHT_QUEUE_RECEIVE_TIMEOUT = 9 ,

    /**
     * Timeout wait for a hw queue send
     */
    PHAROS_TIME_TICK_AWAKEN_THREAD_HEAVYWEIGHT_QUEUE_SEND_TIMEOUT = 10 ,

    /**
     * Timeout waiting for a channel receive
     */
    PHAROS_TIME_TICK_AWAKEN_THREAD_CHANNEL_RECEIVE_TIMEOUT = 11 ,

    /**
     * Timeout waiting for a future wait
     */
    PHAROS_TIME_TICK_AWAKEN_THREAD_FUTURE_WAIT_TIMEOUT = 12 ,

    /**
     * Timeout waiting for a future queue wait
     */
    PHAROS_TIME_TICK_AWAKEN_THREAD_FUTURE_QUEUE_WAIT_TIMEOUT = 13 ,

    /**
     * Timeout waiting for a bulk get
     */
    PHAROS_TIME_TICK_AWAKEN_THREAD_BULK_TIMEOUT = 14 ,

    /**
     * Timeout waiting for a channel bulk get
     */
    PHAROS_TIME_TICK_AWAKEN_THREAD_CHANNEL_BULK_GET_TIMEOUT = 15 ,

    /**
     * Timeout waiting for a channel bulk receive 
     */
    PHAROS_TIME_TICK_AWAKEN_THREAD_CHANNEL_BULK_RECEIVE_TIMEOUT = 16 ,

    /**
     * Timeout waiting for a channel bulk response
     */
    PHAROS_TIME_TICK_AWAKEN_THREAD_CHANNEL_RESPONSE_TIMEOUT = 17 ,
} TimeTickAwakenType;

#endif /* DECLARATIONS_H */
