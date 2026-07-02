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


#ifndef PHAROS_CPU_LINKER_H
#define	PHAROS_CPU_LINKER_H

#include <pharos/hal/hal.h>
#include <pharos/kernel/memory/memory.h>
#include <pharos/hal/cpu/memory.h>


extern uint8_t pharosStartText;

extern uint8_t pharosIpCallStart;
extern uint8_t pharosIpCallEnd;

/**
 * Linker script variable whose address indicates the start of the read-only data
 */
extern uint8_t pharosRodataStart;

/**
 * Linker script variable whose address indicates the end of the read-only data
 */
extern uint8_t pharosRodataEnd;

extern uint8_t pharosCpuTextSizeLog2;
extern uint8_t pharosCpuIpCallSizeLog2;

extern const EnvMemoryProtection pharosVSharedMemProtection;
extern const EnvMemoryProtection pharosVBulkMemProtection;

extern const PharosMemoryArea pharosCpuKernelMemoryArea;
extern const PharosMemoryArea pharosCpuSharedMemoryArea;
extern const PharosMemoryArea pharosCpuIpCallMemoryArea;

extern const PharosMemoryArea pharosCpuPartitionsMemoryArea[];

uint32_t pharosCpuMemoryAreasNumber(ptrEnvMemoryProtection memProtection);

/* PHAROS_CPU_LINKER_H */
#endif	
