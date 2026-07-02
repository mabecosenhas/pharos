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


#ifndef PHAROS_MISC_H
#define	PHAROS_MISC_H

#include <pharos/hal/v7m/declarations.h>


/**
 * Remove from CONTROL register the privileged permissions
 */
void pharosCpuControlRemovePriv();


/**
 * Adds to CONTROL register the privileged permissions
 */
void pharosCpuControlAddPriv();


/**
 * Checks if CONTROL register is unprivileged
 * 
 * @return returns TRUE if CONTROL register is unprivileged and FALSE if is privileged
 */
bool pharosCpuControlIsUnpriv();


/**
 * Gets the PSP register value
 * 
 * @return returns the PSP register value
 */
ptrPharosCpuExceptionFrame pharosCpuPspGet(void);


/**
 * Sets the PSP register
 * 
 * @param newFrame new value of PSP to set
 */
void pharosCpuPspSet(uint32_t newFrame);


/**
 * Enable interrupts using the primask (places 0 on primask register)
 */
void pharosCpuPrimaskEnable(void);


/* PHAROS_MISC_H */
#endif	
