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


#ifndef PHAROS_API_EVENT_H
#define PHAROS_API_EVENT_H


#include <pharos/declarations.h>
#include <pharos/kernel/thread/thread.h>
#include <pharos/kernel/event/event.h>


/**
 * Send an event to a specified thread 
 * 
 * @param thread thread to receive the event
 * @param event event to send
 * 
 * @return returns success or the error
 */
PharosEventSendR pharosEventSend(ptrThreadId thread , Event event);


/**
 * Receive an event with a specified timeout
 * 
 * @constraint cannot be called from an interrupt
 * 
 * @param event event that was received
 * @param timeout maximum timeout for the thread to be blocked for the event
 * 
 * @return returns the result of the function
 */
PharosEventReceiveR pharosEventReceive(ptrEvent event , Timeout timeout);


#endif /* PHAROS_API_EVENT_H */
