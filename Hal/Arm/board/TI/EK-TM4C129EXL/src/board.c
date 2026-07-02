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


#include <pharos/hal/board.h>
#include <pharos/hal/v7m/fpu.h>
#include <pharos/hal/cpu/cpu.h>
#include <pharos/hal/cpu/uart.h>
#include <pharos/hal/cpu/gpio.h>
#include <pharos/hal/cpu/cpu.h>
#include <pharos/declarations.h>
#include <pharos/hal/cpu/peripheral.h>
#include <pharos/kernel/core/rsp.h>


void pharosCpuBoot(void)
{
    /* initialize the oscillators */
    pharosCpuOscInit();

    /* initialize the CPU */
    pharosCpuInitialize();

    /* initialize the peripherals for UART 0 */
    pharosCpuPeripheralEnable(PHAROS_CPU_SYSCTL_PERIPH_UART0);
    pharosCpuPeripheralEnable(PHAROS_CPU_SYSCTL_PERIPH_GPIOA);

    /* Set GPIO A0 and A1 as UART pins */
    pharosCpuGpioPinConfigure(PHAROS_CPU_GPIO_PA0_U0RX);
    pharosCpuGpioPinConfigure(PHAROS_CPU_GPIO_PA1_U0TX);

    /* configure the PIN0 and PIN1 to be used by UART */
    pharosCpuGpioPinTypeUart(PHAROS_CPU_GPIO_PORTA_BASE , PHAROS_CPU_GPIO_PIN_0 | PHAROS_CPU_GPIO_PIN_1);

    /* initialize the UART */
    pharosCpuUartInit();

    /* initialize Pharos in single core */
    pharosSingleCoreInitialize();
}
