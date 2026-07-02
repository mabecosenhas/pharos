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


#include <pharos/buffer.h>
#include <pharos/kernel/buffer/buffer.h>


intptr_t pharosBufferCompare(ptrBuffer a , ptrBuffer b)
{
    /* return value */
    intptr_t result;


    /* check the address first */
    if(a->buffer != b->buffer)
    {
        /* return the comparison of the buffers */
        result = COMPARE(a->buffer , b->buffer);
    }
    else
    {
        /* return the comparison of the sizes */
        result = COMPARE(a->size , b->size);
    }

    /* return the result of the comparison */
    return result;
}


bool pharosBufferArrayInside(uint8_t *buffer , uint32_t size , uint8_t *address)
{
    bool result = FALSE;

    /* end position of the buffer */
    uint8_t *end = &buffer[size];


    /* check if it has access */
    if((buffer <= address) && (address <= end))
    {
        /* then the buffer is inside protection env */
        result = TRUE;
    }


    return result;
}
