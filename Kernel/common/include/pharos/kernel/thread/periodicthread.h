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


#ifndef PHAROS_PERIODICTHREAD_H
#define PHAROS_PERIODICTHREAD_H


#include <pharos/kernel/thread/declarations.h>
#include <pharos/kernel/clock/clock.h>
#include <pharos/kernel/thread/thread.h>
#include <pharos/kernel/thread/rtthread.h>


/**
 * @brief thread job body method pointer
 */
typedef void (*PeriodicThreadJobBody)(void *partitionData);

struct PharosThreadPeriodicProperties
{
    PharosThreadProperties basic;
    PeriodicThreadJobBody entry;
    ClockTick deadline;
};

/**
 * Definition of structures
 */
struct PeriodicThread
{
    /**
     * @brief real-time thread 
     */
    RtThread rtThread;

    /**
     * @brief body of the job of a thread
     */
    PeriodicThreadJobBody body;

    /**
     * @brief release instant  of the thread
     * 
     * @note should be initialized by the application
     */
    ClockTick release;
};


/**
 * Initialize a periodic thread
 * 
 * @param thread thread to initialize 
 * @param env environment of the thread
 */
void pharosIPeriodicThreadInitialize(ptrPeriodicThread thread , ptrPartition partition , ThreadNumber number , bool restart);


/**
 * Announce that a periodic job has ended
 * 
 * @param address that triggered the syscall. If not correct, this function will not do anything and return the same context
 * @param context current mode of the CPU 
 * 
 * @return returns the CPU mode that should be switched to
 */
CpuModeChangeContext pharosIPeriodicThreadEnded(const uint8_t *address , CpuModeChangeContext context);


/**
 * Create an periodic thread 
 * 
 * @param properties periodic thread properties
 * 
 * @return returns success or the corresponding error
 */
PharosThreadPeriodicCreateR pharosSThreadPeriodicCreate(const ptrPharosThreadPeriodicProperties properties , ptrThreadId id);


#endif /* PERIODICTHREAD_H */
