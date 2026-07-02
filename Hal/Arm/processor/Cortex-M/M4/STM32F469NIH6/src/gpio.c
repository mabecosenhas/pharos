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
#include <pharos/hal/cpu/gpio.h>
#include <pharos/hal/cpu/rcc.h>


void pharosCpuGpioInit()
{

}


void pharosCpuGpioConfPull(ptrPharosCpuGpio gpio , PharosCpuGpioPin pin , PharosCpuGpioPull pull)
{
    /* bit index on the GPIOx_PUPDR register */
    uint32_t bitIndex = pin * 2;

    /* calculate the mask to clear the current speed of the selected pin */
    uint32_t mask = 0x3 << bitIndex;

    /* read the current speed */
    uint32_t aux = gpio->portPullUpDown;


    /* clear the current pull for the selected pin */
    aux &= ~mask;

    /* set the new speed */
    aux |= pull << bitIndex;

    /* save it back to the CPU register */
    gpio->portPullUpDown = aux;
}


void pharosCpuGpioConfOutputType(ptrPharosCpuGpio gpio , PharosCpuGpioPin pin , PharosCpuGpioOutputType type)
{
    /* bit index on the GPIOx_OTYPER register */
    uint32_t bitIndex = pin;

    /* calculate the mask to clear the current type of the selected pin */
    uint32_t mask = 0x1 << bitIndex;

    /* read the current type */
    uint32_t aux = gpio->portOutputType;


    /* clear the current speed for the selected pin */
    aux &= ~mask;

    /* set the new speed */
    aux |= type << bitIndex;

    /* save it back to the CPU register */
    gpio->portOutputType = aux;
}


void pharosCpuGpioConfSpeed(ptrPharosCpuGpio gpio , PharosCpuGpioPin pin , PharosCpuGpioSpeed speed)
{
    /* bit index on the GPIOx_OSPEEDR register */
    uint32_t bitIndex = pin * 2;

    /* calculate the mask to clear the current speed of the selected pin */
    uint32_t mask = 0x3 << bitIndex;

    /* read the current speed */
    uint32_t aux = gpio->portOutputSpeed;


    /* clear the current speed for the selected pin */
    aux &= ~mask;

    /* set the new speed */
    aux |= speed << bitIndex;

    /* save it back to the CPU register */
    gpio->portOutputSpeed = aux;

}


void pharosCpuGpioConfMode(ptrPharosCpuGpio gpio , PharosCpuGpioPin pin , PharosCpuGpioMode mode)
{
    /* bit index on the GPIOx_MODER register */
    uint32_t bitIndex = pin * 2;

    /* calculate the mask to clear the current mode of the selected pin */
    uint32_t mask = 0x3 << bitIndex;

    /* read the current mode */
    uint32_t aux = gpio->portMode;


    /* clear the current mode for the selected pin */
    aux &= ~mask;

    /* set the new mode (alternate, input, output, analog) */
    aux |= mode << bitIndex;

    /* save it back to the CPU register */
    gpio->portMode = aux;
}


void pharosCpuGpioConfAlternate(ptrPharosCpuGpio gpio , PharosCpuGpioPin pin , PharosCpuGpioAf function)
{
    /* calculate the index on the array (2 positions on the array) */
    uint32_t arrayIndex = pin >> 3;

    /* calculate the bit offset inside the selected register (GPIOx_AFRH or GPIOx_AFRL) */
    uint32_t registerIndex = (pin & 0x7U) << 2;

    /* bits of the alternate function for the selected pin */
    uint32_t mask = 0xf << registerIndex;

    /* read the alternate function to a auxiliary variable */
    uint32_t aux = gpio->alternateFunction[arrayIndex];


    /* clear the current alternate function */
    aux &= ~mask;

    /* set the new alternate function */
    aux |= function << registerIndex;

    /* save it back to the CPU register */
    gpio->alternateFunction[arrayIndex] = aux;
}


void pharosCpuGpioConfAlt(ptrPharosCpuGpio gpio , PharosCpuGpioPin pin , PharosCpuGpioAf function , PharosCpuGpioSpeed speed , PharosCpuGpioOutputType type)
{
    /* configure the pin to alternate function */
    pharosCpuGpioConfMode(gpio , pin , PHAROS_CPU_GPIO_MODE_ALTERNATE);

    /* configure the alternate function */
    pharosCpuGpioConfAlternate(gpio , pin , function);

    /* configure the speed */
    pharosCpuGpioConfSpeed(gpio , pin , speed);

    /* set the output type */
    pharosCpuGpioConfOutputType(gpio , pin , type);
}
