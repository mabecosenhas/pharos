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


#include <pharos/hal/cpu/exception.h>
#include <pharos/kernel/error/error.h>
#include <pharos/kernel/interrupt/interrupt.h>


void pharosCpuExceptionHandlerC(PharosCpuException exception , uint8_t *line , uint8_t *memAddress)
{
    /* check if the exception is for illegal instructions */
    if(exception <= PHAROS_CPU_EXCEPTION_ILLEGAL_INSTRUCTION)
    {
        /* trigger the illegal instruction */
        pharosIIllegalInstruction(line);
    }
        /* check if the exception is for misaligned addresses */
    else if((exception == PHAROS_CPU_EXCEPTION_LOAD_ADDRESS_MISALIGNED) || 
            (exception == PHAROS_CPU_EXCEPTION_STORE_ADDRESS_MISALIGNED))
    {
        /* trigger misaligned fault */
        pharosIInterruptMemNotAligned(line);
    }
        /* check if the exception is for memory accesses invalid */
    else if((exception == PHAROS_CPU_EXCEPTION_LOAD_PAGE_FAULT) ||
            (exception == PHAROS_CPU_EXCEPTION_STORE_PAGE_FAULT) ||
            (exception == PHAROS_CPU_EXCEPTION_LOAD_ACCESS_FAULT) ||
            (exception == PHAROS_CPU_EXCEPTION_STORE_ACCESS_FAULT) ||
            (exception == PHAROS_CPU_EXCEPTION_INSTRUCTION_PAGE_FAULT))
    {
        /* trigger the invalid memory access fault. This could allow or deny the access */
        pharosIInvalidMemAccess(line , memAddress);
    }
    else
    {
        /* if we are here then the exception is NOT a syscall and we can call an error */
        pharosSErrorAnnounce(PHAROS_ERR_CPU_START + exception , line);
    }
}
