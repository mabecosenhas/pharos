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


#ifndef PHAROS_MULTI_H
#define PHAROS_MULTI_H

#include <pharos/hal/hal.h>
#include <pharos/kernel/core/declarations.h>
#include <pharos/kernel/time/declarations.h>
#include <pharos/kernel/collection/treemap.h>
#include <pharos/kernel/thread/declarations.h>
#include <pharos/kernel/channel/declarations.h>


INLINE bool pharosIShouldIncDate(void);



/**
 * Interrupt handler for multi-core interrupts
 */
void pharosIMulticoreInterruptHandler(void);


/**
 * Trigger an interrupt on the specified core number
 * 
 * @param number core number to trigger the IPI interrupt
 */
void pharosCpuMulticoreTriggerInterrupt(CoreNumber number);


/**
 * Determine if the multicore interrupt is pending
 * 
 * @return returns TRUE if the multicore interrupt is pending and FALSE otherwise 
 */
bool pharosCpuMulticoreIntIsPending(void);


/**
 * Clear any pending multicore interrupt 
 */
void pharosCpuMulticoreClearPending();


/**
 * Enable the multicore interrupts
 */
void pharosCpuMulticoreEnableInt();


/**
 * Receives a channel bulk on the channel core. The channel core is the core that decides which thread will get the 
 * bulk. It controls the channel itself.
 * 
 * @param thread thread that wants to receive a bulk
 * @param channel channel where the thread will block on
 * @param priority address where to place the priority (or if a minimum priority is set, contains the minimum priority)
 * @param timeout timeout on the channel core 
 */
void pharosIChannelReceiveOnChannelCore(ptrThreadInfo thread , ptrChannel channel , ptrPharosPriority priority ,
                                        Timeout timeout);


/**
 * Function invoked when the channel bulk is received on the receiver core. It runs after 
 * pharosIChannelReceiveOnChannelCore, gives to the receiver thread the MMU permissions to access the bulk and 
 * dispatches to the unblocked thread (if needed)
 * 
 * @param unblocked thread that received the bulk
 * @param bulk bulk received by the thread
 * @param returnValue return value of the reception (timeout or success)
 */
void pharosIChannelReceiveReturnOnReceiverCore(ptrThreadInfo unblocked , ptrChannelBulkKernel bulk ,
                                               PharosChannelReceiveR returnValue);


/**
 * Sends a bulk to a local or remote core channel
 * 
 * @param channel channel where to send the bulk to
 * @param bulk bulk to send
 */
void pharosIChannelSendRmp(ptrChannel channel , ptrChannelBulkKernel bulk);

/* PHAROS_MULTI_H */
#endif 
