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


#ifndef PHAROS_EXCEPTION_H
#define	PHAROS_EXCEPTION_H

#include <pharos/hal/hal.h>


typedef enum
{
    PHAROS_CPU_EXCEPTION_INSTRUCTION_MISALIGNED = 0U ,
    PHAROS_CPU_EXCEPTION_INSTRUCTION_ACCESS_FAULT = 1U ,
    PHAROS_CPU_EXCEPTION_ILLEGAL_INSTRUCTION = 2U ,
    PHAROS_CPU_EXCEPTION_BREAKPOINT = 3U ,
    PHAROS_CPU_EXCEPTION_LOAD_ADDRESS_MISALIGNED = 4U ,
    PHAROS_CPU_EXCEPTION_LOAD_ACCESS_FAULT = 5U ,
    PHAROS_CPU_EXCEPTION_STORE_ADDRESS_MISALIGNED = 6U ,
    PHAROS_CPU_EXCEPTION_STORE_ACCESS_FAULT = 7U ,
    PHAROS_CPU_EXCEPTION_ENV_CALL_U_MODE = 8U ,
    PHAROS_CPU_EXCEPTION_ENV_CALL_S_MODE = 9U ,
    PHAROS_CPU_EXCEPTION_ENV_CALL_M_MODE = 11U ,
    PHAROS_CPU_EXCEPTION_INSTRUCTION_PAGE_FAULT = 12U ,
    PHAROS_CPU_EXCEPTION_LOAD_PAGE_FAULT = 13U ,
    PHAROS_CPU_EXCEPTION_STORE_PAGE_FAULT = 15U ,
 } PharosCpuException;

/* PHAROS_EXCEPTION_H */
#endif	
