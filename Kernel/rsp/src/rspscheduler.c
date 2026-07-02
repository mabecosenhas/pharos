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


#include <pharos/kernel/scheduler/scheduler.h>
#include <pharos/kernel/thread/queue.h>


void pharosISchedAddReadyThreadRmp(ptrThreadInfo thread)
{
    /* just call the ready thread */
    pharosISchedAddReadyThread(thread);
}


void pharosIThreadQueueUpdateThreadPrio(ptrThreadInfo thread , ThreadPriority newPriority)
{
    /* update the thread priority on the scheduler core (this one, since there is only one) */
    pharosIThreadQueueUpdateThreadPrioOnSchedulerCore(thread , newPriority);

    /* update the thread priority on the thread core */
    pharosIThreadQueueUpdateThreadPrioOnThreadCore(thread , newPriority);
}


void pharosITimeActivateThreadRmp(ptrThreadInfo thread)
{
    /* just call the normal activation */
    pharosITimeActivateThread(thread);
}
