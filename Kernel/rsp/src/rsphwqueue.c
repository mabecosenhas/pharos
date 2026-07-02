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


#include <pharos/kernel/core/multi.h>
#include <pharos/kernel/thread/dispatcher.h>
#include <pharos/kernel/thread/sporadicthread.h>
#include <pharos/kernel/queue/queue.h>


PharosHwQueueSendR pharosIHwQueueSend(ptrHeavyWeightQueue queue , ptrHeavyWeightQueueMessage message , PharosPriority priority)
{
    /* send the message to the local core */
    return pharosIHwQueueSendOnQueueCore(queue , message , priority);
}


void pharosIHwQueueFreeMessage(ptrHeavyWeightQueue queue , ptrHeavyWeightQueueMessage message)
{
    /* free on the local core a single message */
    pharosIHwQueueFreeMessageLocal(queue , message);
}
