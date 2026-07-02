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
#define PHAROS_CPU_H


#include <pharos/hal/cpu/asm.h>
#include <pharos/hal/hal.h>
#include <pharos/hal/cpu/declarations.h>
#include <pharos/hal/cpu/context.h>
#include <pharos/hal/cpu/conf.h>
#include <pharos/hal/io.h>
#include <pharos/hal/memory/mmu.h>
#include <pharos/hal/interrupt.h>
#include <pharos/hal/cpu/interruptController.h>
#include <pharos/hal/arch/declarations.h>

struct CpuCoreConfiguration
{
};


/**
 * Correct the core number when dealing with interrupts 
 * 
 * @param number core number
 * 
 * @return corrected core number for interrupt handling with the PLIC
 */
INLINE CoreNumber pharosCpuInterruptCoreCorrect(CoreNumber number)
{
    /* don't know why, but the PLIC uses the core number (hart) * 2 */
    return number * 2U;
}

/* PHAROS_CPU_H */
#endif 
