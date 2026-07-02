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


#ifndef PHAROS_V7M_H
#define	PHAROS_V7M_H

#include <pharos/hal/v7m/declarations.h>
#include <pharos/hal/v7m/nvic.h>
#include <pharos/hal/cpu/asm.h>


void pharosCpuIsb(void);
void pharosCpuDsb(void);
void pharosCpuDmb(void);

void pharosCpuStart(void);

void pharosCpuPendableHandler(void);
void pharosCpuSyscallHandler(void);


void pharosCpuArmv7mSyscall(void);
/*void pharosCpuArmv7mSyscallLastCall(void);*/
void pharosCpuArmv7mDispatch(void);

PharosInterruptNumber pharosCpuIpsrGet(void);
uint32_t pharosCpuControlGet(void);

void pharosCpuSharedSectionStart(uintptr_t *start , uintptr_t *size);


bool pharosCpuAddressInsideSharedSection(uint8_t *address);


/* PHAROS_V7M_H */
#endif	
