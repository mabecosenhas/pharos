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


#ifndef PHAROS_EVENT_H
#define PHAROS_EVENT_H


#include <pharos/kernel/event/declarations.h>
#include <pharos/declarations.h>
#include <pharos/kernel/time/declarations.h>
#include <pharos/kernel/thread/declarations.h>


/**
 * receive an event 
 * 
 * @param timeout timeout of the receive
 * @param event address where to place the event received
 * @return 
 */
PharosEventReceiveR pharosIEventReceiveUnprotected(Timeout timeout , ptrEvent event);


/**
 * System call for pharosEventSend
 * 
 * @param threadId thread to send the id to
 * @param event event to send
 * 
 * @return returns success or the failure in sending the event
 */
PharosEventSendR pharosSEventSend(ptrThreadId threadId , Event event);


/**
 * System call for PharosEventReceive
 * 
 * @param event address where to place the event received
 * @param timeout timeout
 * 
 * @return returns success or the failure in receiving the event
 */
PharosEventReceiveR pharosSEventReceive(ptrEvent event , Timeout timeout);


#endif /* EVENT_H */
