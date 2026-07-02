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


/* 
 * File:   fpu.h
 *
 * Created on September 24, 2017
 */

#ifndef PHAROS_ARM_FPU_H
#define	PHAROS_ARM_FPU_H

#ifdef	__cplusplus
extern "C"
{
#endif

#include <pharos/hal/cpu/asm.h>
    
#ifdef PHAROS_CPU_HAS_VFP
#include <pharos/declarations.h>

    void pharosCpuFpuInitialize(void);

    uint32_t pharosCpuFpuGetFpexc(void);
    uint32_t pharosCpuFpuGetFpscr(void);
    uint32_t pharosCpuFpuGetFpinst(void);
    uint32_t pharosCpuFpuGetFpinst2(void);


#endif

#ifdef	__cplusplus
}
#endif

#endif	/* FPU_H */
