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


#ifndef PHAROS_API_BUFFER_H
#define PHAROS_API_BUFFER_H

#include <pharos/declarations.h>
#include <pharos/kernel/buffer/buffer.h>
#include <pharos/kernel/buffer/bufferinline.h>


/**
 * Initialize a buffer with the address of the array and its size [byte]
 * 
 * @param b buffer to initialize
 * @param buffer pointer to the array to place in the buffer
 * @param size size of the buffer (in bytes)
 */
INLINE void pharosBufferInitialize(const ptrBuffer b , uint8_t *buffer , size_t size);


/**
 * Initializes the buffer with zeros
 * 
 * @param b buffer to initialize
 */
INLINE void pharosBufferInitialize0(const ptrBuffer b);


/**
 * Sets the size of the buffer 
 * 
 * @param b buffer to set the size of 
 * @param size size (in bytes) of the buffer 
 */
INLINE void pharosBufferSetSize(const ptrBuffer b , size_t size);


/**
 * Gets the size of the buffer 
 * 
 * @param b buffer to get the size from
 * 
 * @return returns the size of the buffer (in bytes)
 */
INLINE size_t pharosBufferSize(const ptrBuffer b);


/**
 * gets the array of the buffer
 * 
 * @param b buffer to get the array from
 * 
 * @return returns the array of the buffer
 */
INLINE uint8_t *pharosBufferArray(const ptrBuffer b);


/**
 * Copy the content of a buffer to another buffer 
 * 
 * @param dest destination buffer 
 * @param src source buffer
 */
INLINE void pharosBufferCopy(ptrBuffer dest , const ptrBuffer src);


/**
 * Checks if the smaller buffer is completely inside the bigger buffer
 * 
 * @param bigger bigger buffer
 * @param smaller smaller buffer
 * 
 * @return returns TRUE if the smaller buffer is completely inside the bigger buffer and FALSE otherwise
 */
INLINE bool pharosBufferIsInside(ptrBuffer bigger , ptrBuffer smaller);


/**
 * Compares two buffers
 * 
 * @param a first buffer 
 * @param b second buffer
 * 
 * @return return <0, =0, >0 if a<b , a=b and a>b, respectively
 */
intptr_t pharosBufferCompare(ptrBuffer a , ptrBuffer b);



#endif /* BUFFER_H */
