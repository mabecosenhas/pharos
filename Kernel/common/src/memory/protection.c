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


#include <pharos/kernel/object/protection.h>
#include <pharos/hal/boardApi.h>


bool pharosCpuStringIsInsideProtection(const char *string , uint64_t maxSize , const ptrEnvMemoryProtection prot)
{
    return pharosCpuBufferIsInsideProtection((const uint8_t *) string , maxSize , prot);
}


bool pharosIAddressInsideInterruptStack(const uint8_t *start , uint32_t size)
{   
    /* return value of the method */
    bool result;
    
    /* start of the interrupt stack */
    uint8_t *interruptStackStart;

    /* end address of the interrupt stack */
    uint8_t *interrruptStackEnd;

    /* calculate the end of the requested buffer */
    const uint8_t *end = start  + size;


    /* get the start and end of the interrupt stack */
    pharosCpuInterruptStackGet(&interruptStackStart , &interrruptStackEnd);

    /* check if the buffer is inside the start and end of the interrupt stack */
    if((interruptStackStart <= start) && (interrruptStackEnd >= end))
    {
        /* then the buffer is inside */
        result = TRUE;
    }
        /* otherwise */
    else
    {
        /* requested buffer is outside (at least partially) of the interrupt stack */
        result = FALSE;
    }

    /* return TRUE if the requested buffer is inside the interrupt stack */
    return result;
}
