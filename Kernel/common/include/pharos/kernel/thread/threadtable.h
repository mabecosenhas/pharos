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


#ifndef PHAROS_THREADTABLE_H
#define PHAROS_THREADTABLE_H


#include <pharos/kernel/thread/declarations.h>

/**
 * @brief Table of application threads
 */
struct ThreadTable
{
    /**
     * @brief pointer to array of periodic threads in the system
     */
    PeriodicThread *periodicThreads;

    /**
     * @brief pointer to array of sporadic threads in the system
     */
    SporadicThread *sporadicThreads;

    /**
     * @brief pointer to array of aperiodic threads in the system
     */
    AperiodicThread *aperiodicThreads;

    /**
     * @brief number of periodic threads in the system
     */
    ObjectNumber sizePeriodicThreads;

    /**
     * @brief number of sporadic threads in the system
     */
    ObjectNumber sizeSporadicThreads;

    /**
     * @brief number of aperiodic threads in the system
     */
    ObjectNumber sizeAperiodicThreads;
};

#endif /* THREADTABLE_H */
