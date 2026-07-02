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


#include <pharos/hal/cpu/cpu.h>
#include <pharos/hal/cpu/rom.h>
#include <pharos/hal/cpu/peripheral.h>


typedef void (*pharosCpuSysCtlAltClkConfig) (uint32_t ui32Config);
typedef uint32_t(*pharosCpuSysCtlClockFreqSet)(uint32_t ui32Config , uint32_t ui32SysClock);
typedef void (*pharosCpuSysCtlClockOutConfig) (uint32_t ui32Config , uint32_t ui32Div);
typedef void (*pharosCpuSysCtlDeepSleep) (void);
typedef void (*pharosCpuSysCtlDeepSleepClockConfigSet) (uint32_t ui32Div , uint32_t ui32Config);
typedef void (*pharosCpuSysCtlDelay) (uint32_t ui32Count);
typedef uint32_t(*pharosCpuSysCtlFlashSectorSizeGet)(void);
typedef uint32_t(*pharosCpuSysCtlFlashSizeGet)(void);
typedef void (*pharosCpuSysCtlIntClear) (uint32_t ui32Ints);
typedef void (*pharosCpuSysCtlIntDisable) (uint32_t ui32Ints);
typedef void (*pharosCpuSysCtlIntEnable) (uint32_t ui32Ints);
typedef uint32_t(*pharosCpuSysCtlIntStatus)(bool bMasked);
typedef void (*pharosCpuSysCtlLPCLowPowerConfigSet)(uint32_t ui32Config);
typedef uint32_t(*pharosCpuSysCtlLPCLowPowerStatusGet)(void);
typedef void (*pharosCpuSysCtlMOSCConfigSet)(uint32_t ui32Config);
typedef void (*pharosCpuSysCtlNMIClear)(uint32_t ui32Ints);
typedef uint32_t(*pharosCpuSysCtlNMIStatus)(void);
typedef void (*pharosCpuSysCtlPeripheralClockGating)(bool bEnable);
typedef void (*pharosCpuSysCtlPeripheralDeepSleepDisable)(uint32_t ui32Peripheral);
typedef void (*pharosCpuSysCtlPeripheralDeepSleepEnable)(uint32_t ui32Peripheral);
typedef void (*pharosCpuSysCtlPeripheralDisable)(uint32_t ui32Peripheral);
typedef void (*pharosCpuSysCtlPeripheralEnable)(uint32_t ui32Peripheral);
typedef void (*pharosCpuSysCtlPeripheralPowerOff)(uint32_t ui32Peripheral);
typedef void (*pharosCpuSysCtlPeripheralPowerOn)(uint32_t ui32Peripheral);
typedef bool(*pharosCpuSysCtlPeripheralPresent)(uint32_t ui32Peripheral);
typedef bool(*pharosCpuSysCtlPeripheralReady)(uint32_t ui32Peripheral);
typedef void (*pharosCpuSysCtlPeripheralReset)(uint32_t ui32Peripheral);
typedef void (*pharosCpuSysCtlPeripheralSleepDisable)(uint32_t ui32Peripheral);
typedef void (*pharosCpuSysCtlPeripheralSleepEnable)(uint32_t ui32Peripheral);
typedef uint32_t(*pharosCpuSysCtlPIOSCCalibrate)(uint32_t ui32Type);
typedef void (*pharosCpuSysCtlReset)(void);
typedef uint32_t(*pharosCpuSysCtlResetBehaviorGet)(void);
typedef void (*pharosCpuSysCtlResetBehaviorSet)(uint32_t ui32Behavior);
typedef void (*pharosCpuSysCtlResetCauseClear)(uint32_t ui32Causes);
typedef uint32_t(*pharosCpuSysCtlResetCauseGet)(void);
typedef void (*pharosCpuSysCtlSleep)(void);
typedef uint32_t(*pharosCpuSysCtlSRAMSizeGet)(void);
typedef void (*pharosCpuSysCtlVoltageEventClear)(uint32_t ui32Status);
typedef void (*pharosCpuSysCtlVoltageEventConfig)(uint32_t ui32Config);
typedef uint32_t(*pharosCpuSysCtlVoltageEventStatus)(void);


void pharosCpuPeripheralEnable(PharosCpuPeripheralType address)
{
    /* use the ROM function to enable the peripherals */
    ((pharosCpuSysCtlPeripheralEnable) ROM_SYSCTLTABLE[6])(address);
}
