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


#include <pharos/hal/hal.h>
#include <pharos/hal/cpu/cpu.h>
#include <pharos/hal/cpuapi/arm.h>



void pharosCpuRestart(void)
{
    /* TODO */
    while(1);
}


StackPointer pharosCpuInitializeStackPointer(StackPointer originalPointer, uint32_t stackSize)
{
    /* on ARM the stack grows down, so the correct stack pointer is at the end of the stack array */
    return originalPointer + stackSize;
}
