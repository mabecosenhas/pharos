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
#include <pharos/hal/halapi.h>
#include <pharos/declarations.h>
#include <pharos/kernel/core/rsp.h>
#include <pharos/hal/cpu/fmc.h>
#include <pharos/hal/cpu/rcc.h>
#include <pharos/hal/cpu/sdram.h>
#include <pharos/hal/cpu/gpio.h>
#include <pharos/hal/boardApi.h>


void pharosCpuInitSdram();


void pharosCpuBoot(void)
{
    /* initialize the oscillators */
    pharosCpuOscInit();

    /* enable the SRAM Backup and AHB 1 GPIO B clocks */
    PHAROS_CPU_RCC->ahb1PeripheralClockEnable |= PHAROS_CPU_RCC_AHB1ENR_BKP_EN |
        PHAROS_CPU_RCC_AHB1ENR_GPIOA_EN |
        PHAROS_CPU_RCC_AHB1ENR_GPIOB_EN |
        PHAROS_CPU_RCC_AHB1ENR_GPIOC_EN |
        PHAROS_CPU_RCC_AHB1ENR_GPIOD_EN |
        PHAROS_CPU_RCC_AHB1ENR_GPIOE_EN |
        PHAROS_CPU_RCC_AHB1ENR_GPIOF_EN |
        PHAROS_CPU_RCC_AHB1ENR_GPIOG_EN |
        PHAROS_CPU_RCC_AHB1ENR_GPIOH_EN |
        PHAROS_CPU_RCC_AHB1ENR_GPIOI_EN |
        PHAROS_CPU_RCC_AHB1ENR_GPIOJ_EN |
        PHAROS_CPU_RCC_AHB1ENR_GPIOK_EN;

    /* enable the APB1 USART clock */
    PHAROS_CPU_RCC->apb1PeripheralClockEnable |= PHAROS_CPU_RCC_APB1ENR_USART3_EN;

    /* initialize GPIO */
    pharosCpuGpioInit();

    /* set pin 10 of GPIOB as alternate function 7 USART 3 Tx */
    pharosCpuGpioConfAlternate(PHAROS_CPU_GPIOB , PHAROS_CPU_GPIO_PIN_10 , PHAROS_CPU_GPIO_AF7);

    /* set pin 10 of GPIOB as alternate function 7 USART 3 Rx */
    pharosCpuGpioConfAlternate(PHAROS_CPU_GPIOB , PHAROS_CPU_GPIO_PIN_11 , PHAROS_CPU_GPIO_AF7);

    pharosCpuGpioConfMode(PHAROS_CPU_GPIOB , PHAROS_CPU_GPIO_PIN_10 , PHAROS_CPU_GPIO_MODE_ALTERNATE);
    pharosCpuGpioConfMode(PHAROS_CPU_GPIOB , PHAROS_CPU_GPIO_PIN_11 , PHAROS_CPU_GPIO_MODE_ALTERNATE);

    pharosCpuGpioConfSpeed(PHAROS_CPU_GPIOB , PHAROS_CPU_GPIO_PIN_10 , PHAROS_CPU_GPIO_SPEED_VERY_HIGH);
    pharosCpuGpioConfSpeed(PHAROS_CPU_GPIOB , PHAROS_CPU_GPIO_PIN_11 , PHAROS_CPU_GPIO_SPEED_VERY_HIGH);

    pharosCpuGpioConfOutputType(PHAROS_CPU_GPIOB , PHAROS_CPU_GPIO_PIN_10 , PHAROS_CPU_GPIO_OUTPUT_TYPE_PP);
    pharosCpuGpioConfOutputType(PHAROS_CPU_GPIOB , PHAROS_CPU_GPIO_PIN_11 , PHAROS_CPU_GPIO_OUTPUT_TYPE_PP);

    pharosCpuGpioConfPull(PHAROS_CPU_GPIOB , PHAROS_CPU_GPIO_PIN_10 , PHAROS_CPU_GPIO_PULL_UP);
    pharosCpuGpioConfPull(PHAROS_CPU_GPIOB , PHAROS_CPU_GPIO_PIN_11 , PHAROS_CPU_GPIO_PULL_UP);

    /* initialize the UART */
    pharosCpuUartInit(PHAROS_CPU_USART3);

    /* initialize the CPU */
    pharosCpuInitialize();

    /* initialize the SDRAM before copying data to it */
    pharosCpuInitSdram();

    pharosSingleCoreInitialize();
}


void outputSChar(char c)
{
    /* wait until the UART is ready */
    while((PHAROS_CPU_USART3->status & PHAROS_CPU_USART_SR_TXE_RDY) == 0);

    /* write the char to the UART */
    PHAROS_CPU_USART3->data = c;
}
