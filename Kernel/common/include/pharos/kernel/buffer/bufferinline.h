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


#ifndef PHAROS_BUFFERINLINE_H
#define PHAROS_BUFFERINLINE_H

#include <pharos/kernel/buffer/buffer.h>
#include <pharos/kernel/object/declarations.h>


INLINE void pharosBufferInitialize(const ptrBuffer b , uint8_t *buffer , size_t size)
{
    /* set the buffer */
    b->buffer = buffer;

    /* set the size */
    b->size = size;
}


INLINE void pharosBufferSetSize(const ptrBuffer b , size_t size)
{
    /* set the size */
    b->size = size;
}


INLINE size_t pharosBufferSize(const ptrBuffer b)
{
    /* return the size */
    return b->size;
}


INLINE uint8_t *pharosBufferArray(const ptrBuffer b)
{
    /* return the buffer */
    return b->buffer;
}


INLINE void pharosBufferCopy(ptrBuffer dest , const ptrBuffer src)
{
    /* copy the buffer */
    memcpy(dest->buffer , src->buffer , src->size);

    /* copy the size */
    dest->size = src->size;
}


INLINE bool pharosBufferIsInside(ptrBuffer bigger , ptrBuffer smaller)
{
    /* end of the bigger array */
    uint8_t *end1 = bigger->buffer + bigger->size;
    
    /* end of the smaller array */
    uint8_t *end2 = smaller->buffer + smaller->size;

    
    /* returns TRUE if the start and end of the smaller are within the bigger buffer */
    return((bigger->buffer <= smaller->buffer) && (end2 <= end1)) ? TRUE : FALSE;
}


INLINE void pharosBufferInitialize0(const ptrBuffer b)
{
    /* set zeros */
    memset(b->buffer , 0 , b->size);
}



#endif /* PHAROS_BUFFERINLINE_H */
