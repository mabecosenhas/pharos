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


#ifndef PHAROS_THREAD_ID_H
#define PHAROS_THREAD_ID_H

#include <pharos/kernel/thread/declarations.h>

/**
 * Unique identifier of a thread inside a partition
 * 
 * Used to calculate the pointer of the thread
 */
struct ThreadId
{
    /**
     * index of the thread on its table
     */
    ThreadNumber number;

    struct
    {
        /**
         * flag set to TRUE if the thread is periodic
         */
        uint32_t isPeriodic : 1;

        /**
         * flag set to TRUE if the thread is sporadic
         */
        uint32_t isSporadic : 1;

        /**
         * flag set to TRUE if the thread is aperiodic
         */
        uint32_t isAperiodic : 1;
    } flags;
};


/**
 * Determines if the thread id is for a Real-Time thread or not 
 * 
 * @param id thread id
 * 
 * @return returns TRUE if the thread is a Real-Time thread and FALSE otherwise
 */
INLINE bool pharosIThreadIdIsRt(ptrThreadId id)
{
    /* if aperiodic then it is not a real-time thread */
    return id->flags.isAperiodic == FALSE ? TRUE : FALSE;
}


#endif /* ID_H */
