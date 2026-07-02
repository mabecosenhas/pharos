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


#ifndef PHAROS_HAL_V7M_DECLARATIONS_H
#define	PHAROS_HAL_V7M_DECLARATIONS_H

#include <pharos/hal/hal.h>
#include <pharos/hal/cpu/declarations.h>
#include <pharos/hal/cpu/conf.h>

typedef struct PharosCpuIsrTable PharosCpuIsrTable , *ptrPharosCpuIsrTable;
typedef struct PharosCpuScb PharosCpuScb , *ptrPharosCpuScb;
typedef struct PharosCpuMpu PharosCpuMpu , *ptrPharosCpuMpu;
typedef struct PharosCpuNvic PharosCpuNvic , *ptrPharosCpuNvic;
typedef struct PharosCpuInterruptTable PharosCpuInterruptTable , *ptrPharosCpuInterruptTable;
typedef struct PharosCpuFpu PharosCpuFpu , *ptrPharosCpuFpu;
typedef struct PharosCpuSysTick PharosCpuSysTick , *ptrPharosCpuSysTick;
typedef struct PharosCpuCache PharosCpuCache , *ptrPharosCpuCache;
typedef struct PharosCpuExceptionFrame PharosCpuExceptionFrame , *ptrPharosCpuExceptionFrame;
typedef struct PharosCpuCpuid PharosCpuCpuid , *ptrPharosCpuCpuid;

typedef uint8_t PharosInterruptPriority;

/**
 * Handler for an exception (only used inside Pharos)
 */
typedef void (*PharosExceptionHandler)(void);

/**
 * Raw interrupt handler (only used inside Pharos)
 */
typedef void (*PharosRawInterruptHandler)(void);

#endif	/* DECLARATIONS_H */
