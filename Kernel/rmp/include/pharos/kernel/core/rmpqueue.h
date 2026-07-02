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


#ifndef PHAROS_RMPQUEUE_H
#define PHAROS_RMPQUEUE_H

#include <pharos/kernel/thread/queue.h>


/**
 * Add the thread remote thread queue nodes to the running core. Only adds if not already added. It is safe to call
 * this method multiple times for the same thread since it will only do something in the first call
 *  
 * @param thread thread to add the remote nodes from
 */
void pharosIThreadQueueThreadRemoteAdd(ptrThreadInfo thread);


#endif /* PHAROS_RMPQUEUE_H */
