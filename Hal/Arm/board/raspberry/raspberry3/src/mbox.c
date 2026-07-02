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


#include <pharos/hal/mbox.h>


#define PHAROS_CPU_MBOX_STATUS_FULL         (0x80000000U)
#define PHAROS_CPU_MBOX_STATUS_EMPTY        (0x40000000U)
#define PHAROS_CPU_MBOX_READ_RESPONSE       (0x80000000U)


int32_t pharosCpuMboxCall(PharosCpuMboxChannel channel, uint32_t mbox[])
{
    uint32_t response;
    uint8_t channelResponse;
    uint32_t responseAddress;
    
    /* address of the mailbox buffer. This is a 32-bit variable */
    uint32_t address = (uint32_t) (uint64_t) & mbox[0];

    /* loop until the mailbox is not full */
    while(PHAROS_CPU_MBOX->status & PHAROS_CPU_MBOX_STATUS_FULL);

    /* write the address of the message plus the channel */
    PHAROS_CPU_MBOX->write = address | channel;
    
    do
    {
        /* wait for the response */
        while(PHAROS_CPU_MBOX->status & PHAROS_CPU_MBOX_STATUS_EMPTY);

        /* read the response */
        response = PHAROS_CPU_MBOX->read;

        /* get the channel the response is related to */
        channelResponse = (uint8_t) (response & 0xF);
        
        /* get the response address it is related to */
        responseAddress = response & ~0xF;

        /* loop until we get a response for our message */
    }
    while(channelResponse != channel || responseAddress != address);

    /* return the response */
    return mbox[1] == PHAROS_CPU_MBOX_READ_RESPONSE;
}
