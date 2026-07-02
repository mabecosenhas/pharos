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


#include <pharos/hal/cpu/asm.h>
#include <pharos/hal/v7m/declarations.h>
#include <pharos/hal/v7m/isr.h>
#include <pharos/hal/cpu/cpu.h>
#include <pharos/hal/thumb.h>
#include <pharos/hal/v7m/systick.h>
#include <pharos/hal/cpu/context.h>
#include <pharos/kernel/thread/stack.h>
#include <pharos/hal/v7m/mpu.h>
#include <pharos/hal/v7m/cache.h>
#include <pharos/hal/v7m/fpu.h>
#include <pharos/hal/memory/mpuV7.h>
#include <pharos/hal/cpu/conf.h>
#include <pharos/kernel/object/protection.h>
#include <pharos/kernel/core/core.h>
#include <pharos/kernel/clock/clock.h>
#include <pharos/hal/v7m/nvic.h>
#include <pharos/hal/cpu/timer.h>
#include <pharos/hal/cpu/gpio.h>




void pharosCpuInitialize(void)
{
    /* initialize the floating point */
    pharosCpuFpuInit();

    /* initialize the i-cache */
    pharosCpuCacheIEnable();

    /* initialize the d-cache */
    pharosCpuCacheDEnable();
}


void pharosCpuCoreInitialize(ptrCore core)
{
    /* initialize the interrupts */
    pharosCpuNvicInitialize(&core->interrupts);
}
