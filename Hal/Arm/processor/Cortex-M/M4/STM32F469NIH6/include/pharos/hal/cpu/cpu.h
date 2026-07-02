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


#ifndef PHAROS_CPU_H
#define	PHAROS_CPU_H

#include <pharos/hal/v7m/declarations.h>
#include <pharos/hal/v7m/v7m.h>
#include <pharos/hal/arm/declarations.h>
#include <pharos/hal/v7m/isr.h>
#include <pharos/hal/cpu/context.h>
#include <pharos/hal/thumb.h>
#include <pharos/hal/memory/mpuV7.h>
#include <pharos/interrupt.h>
#include <pharos/hal/cpu/linker.h>
#include <pharos/hal/cpu/memory.h>


struct CpuCoreConfiguration
{
};




/**
 * Initialize the CPU PLL to maximum frequency
 */
void pharosCpuOscInit(void);


/**
 * Initialize the CPU
 */
void pharosCpuInitialize(void);

/* PHAROS_CPU_H */
#endif	
