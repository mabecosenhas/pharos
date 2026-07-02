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


#ifndef PHAROS_KERNEL_CHANNEL_CHANNELBULK_H
#define PHAROS_KERNEL_CHANNEL_CHANNELBULK_H


#include <pharos/kernel/channel/declarations.h>
#include <pharos/kernel/buffer/buffer.h>

struct ChannelBulk
{
    /**
     * Pointer to the kernel bulk. Do not change this
     */
    ptrChannelBulkKernel kernelBulk;

    /**
     * Buffer that contains the whole bulk (should not be changed)
     */
    Buffer bufferFull;

    /**
     * Buffer containing the bulk message to be transmitted
     * Example: the message to be transmitted to a CAN, Ethernet, TCP, etc
     */
    Buffer bufferMessage;

    /**
     * Buffer containing an optional extra message 
     * Example: information of the socket in which the message is transmitted
     */
    Buffer bufferExtra;
};


#endif /* PHAROS_KERNEL_CHANNEL_CHANNELBULK_H */
