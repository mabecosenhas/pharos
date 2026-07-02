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


#ifndef PHAROS_APERIODICTHREAD_H
#define PHAROS_APERIODICTHREAD_H


#include <pharos/kernel/thread/declarations.h>
#include <pharos/kernel/thread/thread.h>
#include <pharos/kernel/timer/timer.h>


/**
 * @brief thread body method pointer
 */
typedef void (*AperiodicThreadBody)(void *partitionData);

/**
 * Definition of structures
 */

/**
 * @brief Aperiodic thread definitions
 */
struct AperiodicThread
{
    /**
     * @brief thread info
     */
    ThreadInfo info;

    /**
     * @brief body of the thread
     */
    AperiodicThreadBody body;
};

struct PharosThreadAperiodicProperties
{
    PharosThreadProperties basic;

    AperiodicThreadBody entry;
};

/**
 * Initializes an aperiodic thread 
 * 
 * @param thread
 * @param partition
 * @param restart restart flag
 */
void pharosIAperiodicThreadInit(ptrAperiodicThread thread , ptrPartition partition , ThreadNumber number , bool restart);


/**
 * Announce that an aperiodic thread has ended
 */
void pharosIAperiodicThreadEnded(const uint8_t *address);


/**
 * Starting point of aperiodic threads
 */
void pharosIAperiodicThreadBody(void);


PharosThreadAperiodicCreateR pharosSThreadAperiodicCreate(const ptrPharosThreadAperiodicProperties properties , ptrThreadId id);


#endif /* APERIODICTHREAD_H */
