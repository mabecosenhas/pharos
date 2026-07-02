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


#ifndef PHAROS_MTIMERCOMP_H
#define	PHAROS_MTIMERCOMP_H

#include <pharos/hal/cpu/clint.h>

 
/**
 * Get the mtimercmp register for the running core
 * 
 * @return returns the mtimercmp for the running core
 */
INLINE uint64_t pharosCpuMTimerCmp()
{
    return PHAROS_CPU_CLINT->mtimecmp[pharosICoreRunning()];
}


/**
 * Get the mtime register
 * 
 * @return returns the mtime register value
 */
INLINE uint64_t pharosCpuMTime()
{
    return PHAROS_CPU_CLINT->mtime;
}


/* PHAROS_MTIMERCOMP_H */
#endif	
