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


#include <pharos/kernel/core/core.h>
#include <pharos/hal/v8a/context.h>

extern uintptr_t pharosCpuIpCallFilterExit;
extern uintptr_t pharosCpuIpCallIoExit;
extern uintptr_t pharosCpuIpCallOExit;


void pharosCpuPartitionIpcallFilterExit(ptrThreadInfo thread)
{
    /* make the thread leave to the pharosCpuIpCallFilterExit instructions once it is re-executed */
    thread->cpuContext.baseContext.x30 = pharosCpuIpCallFilterExit;

    /* clear the SPSR EL bits */
    thread->cpuContext.baseContext.spsr_l1 &= ~SPSR_EL_MASK;

    /* make the thread return in the EL1 mode */
    thread->cpuContext.baseContext.spsr_l1 |= SPSR_EL1_MODE;
}


void pharosCpuPartitionIpcallIoExit(ptrThreadInfo thread)
{
    /* make the thread leave to the pharosCpuIpCallIoExit instructions once it is re-executed */
    thread->cpuContext.baseContext.x30 = pharosCpuIpCallIoExit;

    /* clear the SPSR EL bits */
    thread->cpuContext.baseContext.spsr_l1 &= ~SPSR_EL_MASK;

    /* make the thread return in the EL1 mode */
    thread->cpuContext.baseContext.spsr_l1 |= SPSR_EL1_MODE;
}


void pharosCpuPartitionIpcallOExit(ptrThreadInfo thread)
{
    /* make the thread leave to the pharosCpuIpCallOExit instructions once it is re-executed */
    thread->cpuContext.baseContext.x30 = pharosCpuIpCallOExit;

    /* clear the SPSR EL bits */
    thread->cpuContext.baseContext.spsr_l1 &= ~SPSR_EL_MASK;

    /* make the thread return in the EL1 mode */
    thread->cpuContext.baseContext.spsr_l1 |= SPSR_EL1_MODE;
}
