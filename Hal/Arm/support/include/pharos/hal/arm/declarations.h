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
 * File:   declarations.h
 *
 * Created on September 25, 2017
 */

#ifndef PHAROS_HAL_ARM_DECLARATIONS_H
#define	PHAROS_HAL_ARM_DECLARATIONS_H

#ifdef	__cplusplus
extern "C"
{
#endif

#include <pharos/hal/hal.h>

    typedef uint32_t CpuModeChangeContext;


    typedef uint32_t PharosInterruptLevel VARIABLE_ALIGNMENT_X(8);


    typedef PharosInterruptLevel *ptrPharosInterruptLevel;

    extern const uint32_t pharosVCpuSyscallStackSize[];


#ifdef	__cplusplus
}
#endif

#endif	/* DECLARATIONS_H */
