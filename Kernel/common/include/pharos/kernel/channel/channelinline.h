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


#ifndef PHAROS_CHANNELINLINE_H
#define PHAROS_CHANNELINLINE_H

#include <pharos/channel.h>


INLINE void pharosChannelIdInitialize(ptrChannelId id , PartitionNumber partition , ChannelNumber channel)
{
    /* set the partition */
    id->partition = partition;

    /* set the channel */
    id->channel = channel;
}


INLINE ptrBuffer pharosChannelBulkBuffer(ptrChannelBulk bulk)
{
    /* return the message buffer */
    return &bulk->bufferMessage;
}


INLINE ptrBuffer pharosChannelBulkBufferExtra(ptrChannelBulk bulk)
{
    /* return the extra buffer */
    return &bulk->bufferExtra;
}


INLINE ptrBuffer pharosChannelBulkBufferExtraInit(ptrChannelBulk bulk , size_t size)
{
    /* the extra buffer is initialized at the end of the bulk */
    bulk->bufferExtra.buffer = bulk->bufferFull.buffer + bulk->bufferFull.size - size;

    /* set the size */
    bulk->bufferExtra.size = size;

    /* decrease the size of the buffer message */
    bulk->bufferMessage.size -= size;

    /* return the address of the extra buffer */
    return &bulk->bufferExtra;
}


INLINE bool pharosChannelBulkIsValid(ptrChannelBulk bulk)
{
    /* is valid if there is a valid kernel bulk */
    return bulk->kernelBulk == NULL ? FALSE : TRUE;
}


INLINE void pharosChannelBulkSetInvalid(ptrChannelBulk bulk)
{
    /* mark the kernel bulk with an invalid value so that pharosChannelBulkIsValid can check it */
    bulk->kernelBulk = NULL;
}


INLINE ptrBuffer pharosChannelBulkBufferExtraCopy(ptrChannelBulk bulk , const uint8_t *array , size_t size)
{
    /* the extra buffer is initialized at the end of the bulk */
    bulk->bufferExtra.buffer = bulk->bufferFull.buffer + bulk->bufferFull.size - size;

    /* set the size */
    bulk->bufferExtra.size = size;

    /* copy the arrays*/
    memcpy(bulk->bufferExtra.buffer , array , size);

    /* return the address of the extra buffer */
    return &bulk->bufferExtra;
}


INLINE ptrBuffer pharosChannelBulkBufferCopy(ptrChannelBulk bulk , const uint8_t *array , size_t size)
{
    /* set the size */
    bulk->bufferMessage.size = size;

    /* copy the array */
    memcpy(bulk->bufferMessage.buffer , array , size);

    /* return the address of the buffer */
    return &bulk->bufferMessage;
}


#endif /* PHAROS_CHANNELINLINE_H */
