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


#ifndef PHAROS_TMS570LC4357_CPU_H
#define PHAROS_TMS570LC4357_CPU_H

#ifdef __cplusplus
extern "C"
{
#endif


#include <pharos/hal/cpu/asm.h>
#include <pharos/hal/cpu/conf.h>
#include <pharos/kernel/core/declarations.h>
#include <pharos/hal/arm/declarations.h>
#include <pharos/hal/cpu/declarations.h>
#include <pharos/hal/arm.h>
#include <pharos/kernel/object/declarations.h>
#include <pharos/hal/boardApi.h>
#include <pharos/hal/io.h>
#include <pharos/hal/cpu/context.h>
#include <pharos/hal/memory/mpuV7.h>
#include <pharos/hal/halapi.h>
#include <pharos/hal/interrupt/vim.h>
#include <pharos/hal/cpu/esm.h>


    struct CpuCoreConfiguration
    {
    };



    /**
     * Initialize the CPU Memory
     */
    void pharosCpuMemoryInitialize(void);


   
    
    
#ifdef __cplusplus
}
#endif

#endif /* PHAROS_ARM926EJ_S_CPU_H */
