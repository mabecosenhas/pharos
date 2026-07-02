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


#ifndef PHAROS_THREADINLINE_H
#define PHAROS_THREADINLINE_H

#include <pharos/kernel/thread/thread.h>
#include <pharos/kernel/partition/partitioninline.h>


INLINE void pharosThreadIdInitAperiodic(const ptrThreadId id , ThreadNumber number)
{
    /* set to aperiodic */
    id->flags.isAperiodic = TRUE;

    /* set to not periodic */
    id->flags.isPeriodic = FALSE;

    /* set to not sporadic */
    id->flags.isSporadic = FALSE;

    /* initialize the aperiodic thread number */
    id->number = number;
}


INLINE void pharosThreadIdInitPeriodic(const ptrThreadId id , ThreadNumber number)
{
    /* set to not aperiodic */
    id->flags.isAperiodic = FALSE;

    /* set to periodic */
    id->flags.isPeriodic = TRUE;

    /* set to not sporadic */
    id->flags.isSporadic = FALSE;

    /* initialize the aperiodic thread number */
    id->number = number;
}


INLINE void pharosThreadIdInitSporadic(const ptrThreadId id , ThreadNumber number)
{
    /* set to not aperiodic */
    id->flags.isAperiodic = FALSE;

    /* set to not periodic */
    id->flags.isPeriodic = FALSE;

    /* set to sporadic */
    id->flags.isSporadic = TRUE;

    /* initialize the aperiodic thread number */
    id->number = number;
}


INLINE ThreadPriority pharosIThreadGetPriority(const ptrThreadInfo thread)
{
    /* return the thread priority */
    return thread->currentPriority;
}


INLINE void pharosIThreadAddCeiling(const ptrThreadInfo thread , ptrSemaphoreCeiling semaphore)
{
    /* add the ceiling to the priority queue */
    pharosIPqAddSemCeil(&thread->ceilingSemaphoresHold.queue.priority , semaphore);
}


INLINE void pharosIThreadRemoveCeiling(const ptrThreadInfo thread , ptrSemaphoreCeiling semaphore)
{
    /* remove the semaphore from the priority queue */
    pharosIPqRemoveSemCeil(&thread->ceilingSemaphoresHold.queue.priority , semaphore);
}


INLINE ptrSemaphoreCeiling pharosIThreadGetHighestCeiling(const ptrThreadInfo thread)
{
    /* get the highest semaphore ceiling from the priority queue of semaphores hold */
    return pharosIPqGetHighestSemCeil(&thread->ceilingSemaphoresHold.queue.priority);
}


INLINE void pharosIThreadRestorePriority(const ptrThreadInfo thread)
{
    /* update the thread priority to the base value*/
    pharosISchedUpdateThreadPrio(thread , thread->priority);
}


INLINE void pharosIThreadResourceAdd(ptrThreadInfo thread , ptrResource resource)
{
    /* if there is a resource to add */
    if(resource != NULL)
    {
        /* append it to the thread list of resources hold */
        linkedListAppendFirst(&thread->resourcesHold , resource);
    }
}


INLINE void pharosIThreadResourceRemove(ptrThreadInfo thread , ptrResource resource)
{
    /* if there is a resource to remove */
    if(resource != NULL)
    {
        /* remove it from the thread list of resources */
        linkedListRemoveObject(&thread->resourcesHold , resource);
    }
}


INLINE bool pharosIThreadIsInsideResource(ptrThreadInfo thread)
{
    /* check if the thread holds any resource */
    return linkedListIsEmpty(&thread->resourcesHold) == TRUE ? FALSE : TRUE;
}


INLINE ptrResource pharosIThreadResourceLast(ptrThreadInfo thread)
{
    /* get the first element on the list (the last element to be added to the list) */
    return linkedListGetFirst(&thread->resourcesHold);
}


INLINE bool pharosIThreadIsPeriodic(const ptrThreadInfo thread)
{
    /* get the "isPeriodic" flag */
    return thread->threadId.flags.isPeriodic;
}


INLINE bool pharosIThreadIsAperiodic(const ptrThreadInfo thread)
{
    /* get the "isAperiodic" flag */
    return thread->threadId.flags.isAperiodic;
}


INLINE bool pharosIThreadIsRt(const ptrThreadInfo thread)
{
    /* return TRUE if either the periodic or sporadic threads are set */
    return thread->threadId.flags.isPeriodic + thread->threadId.flags.isSporadic;
}


INLINE bool pharosIThreadIsSporadic(const ptrThreadInfo thread)
{
    /* get the "isSporadic" flag */
    return thread->threadId.flags.isSporadic;
}


INLINE ptrPartition pharosIThreadGetPartition(const ptrThreadInfo thread)
{
    /* get the partition of the thread */
    return thread->partition;
}


INLINE bool pharosIThreadIsStarted(ptrThreadInfo thread)
{
    /* return value of the method */
    bool result;


    /* if the thread partition is not yet initialized (delay start or restarting) or the thread is not started */
    if((pharosIPartitionIsStarted(thread->partition) == FALSE) || (thread->state.flags.isStarted == FALSE))
    {
        /* then the thread is not started */
        result = FALSE;
    }
        /* otherwise */
    else
    {
        /* the thread has started */
        result = TRUE;
    }

    /* return TRUE if the thread has started and FALSE otherwise */
    return result;
}


#endif /* PHAROS_THREADINLINE_H */
