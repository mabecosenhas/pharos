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


#ifndef PHAROS_GPIO_H
#define	PHAROS_GPIO_H

#include <pharos/hal/cpu/declarations.h>
#include <pharos/hal/cpu/conf.h>


#define PHAROS_CPU_GPIOA                        ((ptrPharosCpuGpio) PHAROS_CPU_GPIOA_ADDRESS)
#define PHAROS_CPU_GPIOB                        ((ptrPharosCpuGpio) PHAROS_CPU_GPIOB_ADDRESS)
#define PHAROS_CPU_GPIOC                        ((ptrPharosCpuGpio) PHAROS_CPU_GPIOC_ADDRESS)
#define PHAROS_CPU_GPIOD                        ((ptrPharosCpuGpio) PHAROS_CPU_GPIOD_ADDRESS)
#define PHAROS_CPU_GPIOE                        ((ptrPharosCpuGpio) PHAROS_CPU_GPIOE_ADDRESS)
#define PHAROS_CPU_GPIOF                        ((ptrPharosCpuGpio) PHAROS_CPU_GPIOF_ADDRESS)
#define PHAROS_CPU_GPIOG                        ((ptrPharosCpuGpio) PHAROS_CPU_GPIOG_ADDRESS)
#define PHAROS_CPU_GPIOH                        ((ptrPharosCpuGpio) PHAROS_CPU_GPIOH_ADDRESS)
#define PHAROS_CPU_GPIOI                        ((ptrPharosCpuGpio) PHAROS_CPU_GPIOI_ADDRESS)
#define PHAROS_CPU_GPIOJ                        ((ptrPharosCpuGpio) PHAROS_CPU_GPIOJ_ADDRESS)
#define PHAROS_CPU_GPIOK                        ((ptrPharosCpuGpio) PHAROS_CPU_GPIOK_ADDRESS)


#define PHAROS_CPU_GPIO_MODER_ALT0              (0b10U << 0U)
#define PHAROS_CPU_GPIO_MODER_ALT1              (0b10U << 2U)
#define PHAROS_CPU_GPIO_MODER_ALT2              (0b10U << 4U)
#define PHAROS_CPU_GPIO_MODER_ALT3              (0b10U << 6U)
#define PHAROS_CPU_GPIO_MODER_ALT4              (0b10U << 8U)
#define PHAROS_CPU_GPIO_MODER_ALT5              (0b10U << 10U)
#define PHAROS_CPU_GPIO_MODER_ALT6              (0b10U << 12U)
#define PHAROS_CPU_GPIO_MODER_ALT7              (0b10U << 14U)
#define PHAROS_CPU_GPIO_MODER_ALT8              (0b10U << 16U)
#define PHAROS_CPU_GPIO_MODER_ALT9              (0b10U << 18U)
#define PHAROS_CPU_GPIO_MODER_ALT10             (0b10U << 20U)
#define PHAROS_CPU_GPIO_MODER_ALT11             (0b10U << 22U)
#define PHAROS_CPU_GPIO_MODER_ALT12             (0b10U << 24U)
#define PHAROS_CPU_GPIO_MODER_ALT13             (0b10U << 26U)
#define PHAROS_CPU_GPIO_MODER_ALT14             (0b10U << 28U)
#define PHAROS_CPU_GPIO_MODER_ALT15             (0b10U << 30U)

#define PHAROS_CPU_GPIO_OSPEEDR_VERYH0          (0b11U << 0U)
#define PHAROS_CPU_GPIO_OSPEEDR_VERYH1          (0b11U << 2U)
#define PHAROS_CPU_GPIO_OSPEEDR_VERYH2          (0b11U << 4U)
#define PHAROS_CPU_GPIO_OSPEEDR_VERYH3          (0b11U << 6U)
#define PHAROS_CPU_GPIO_OSPEEDR_VERYH4          (0b11U << 8U)
#define PHAROS_CPU_GPIO_OSPEEDR_VERYH5          (0b11U << 10U)
#define PHAROS_CPU_GPIO_OSPEEDR_VERYH6          (0b11U << 12U)
#define PHAROS_CPU_GPIO_OSPEEDR_VERYH7          (0b11U << 14U)
#define PHAROS_CPU_GPIO_OSPEEDR_VERYH8          (0b11U << 16U)
#define PHAROS_CPU_GPIO_OSPEEDR_VERYH9          (0b11U << 18U)
#define PHAROS_CPU_GPIO_OSPEEDR_VERYH10         (0b11U << 20U)
#define PHAROS_CPU_GPIO_OSPEEDR_VERYH11         (0b11U << 22U)
#define PHAROS_CPU_GPIO_OSPEEDR_VERYH12         (0b11U << 24U)
#define PHAROS_CPU_GPIO_OSPEEDR_VERYH13         (0b11U << 26U)
#define PHAROS_CPU_GPIO_OSPEEDR_VERYH14         (0b11U << 28U)
#define PHAROS_CPU_GPIO_OSPEEDR_VERYH15         (0b11U << 30U)

#define PHAROS_CPU_GPIO_PUPDR_PULL_UP0          (0b01 << 0U)
#define PHAROS_CPU_GPIO_PUPDR_PULL_UP1          (0b01 << 2U)
#define PHAROS_CPU_GPIO_PUPDR_PULL_UP2          (0b01 << 4U)
#define PHAROS_CPU_GPIO_PUPDR_PULL_UP3          (0b01 << 6U)
#define PHAROS_CPU_GPIO_PUPDR_PULL_UP4          (0b01 << 8U)
#define PHAROS_CPU_GPIO_PUPDR_PULL_UP5          (0b01 << 10U)
#define PHAROS_CPU_GPIO_PUPDR_PULL_UP6          (0b01 << 12U)
#define PHAROS_CPU_GPIO_PUPDR_PULL_UP7          (0b01 << 14U)
#define PHAROS_CPU_GPIO_PUPDR_PULL_UP8          (0b01 << 16U)
#define PHAROS_CPU_GPIO_PUPDR_PULL_UP9          (0b01 << 18U)
#define PHAROS_CPU_GPIO_PUPDR_PULL_UP10         (0b01 << 20U)
#define PHAROS_CPU_GPIO_PUPDR_PULL_UP11         (0b01 << 22U)
#define PHAROS_CPU_GPIO_PUPDR_PULL_UP12         (0b01 << 24U)
#define PHAROS_CPU_GPIO_PUPDR_PULL_UP13         (0b01 << 26U)
#define PHAROS_CPU_GPIO_PUPDR_PULL_UP14         (0b01 << 28U)
#define PHAROS_CPU_GPIO_PUPDR_PULL_UP15         (0b01 << 30U)


typedef enum
{
    PHAROS_CPU_GPIO_PIN_0 = 0U ,
    PHAROS_CPU_GPIO_PIN_1 = 1U ,
    PHAROS_CPU_GPIO_PIN_2 = 2U ,
    PHAROS_CPU_GPIO_PIN_3 = 3U ,
    PHAROS_CPU_GPIO_PIN_4 = 4U ,
    PHAROS_CPU_GPIO_PIN_5 = 5U ,
    PHAROS_CPU_GPIO_PIN_6 = 6U ,
    PHAROS_CPU_GPIO_PIN_7 = 7U ,
    PHAROS_CPU_GPIO_PIN_8 = 8U ,
    PHAROS_CPU_GPIO_PIN_9 = 9U ,
    PHAROS_CPU_GPIO_PIN_10 = 10U ,
    PHAROS_CPU_GPIO_PIN_11 = 11U ,
    PHAROS_CPU_GPIO_PIN_12 = 12U ,
    PHAROS_CPU_GPIO_PIN_13 = 13U ,
    PHAROS_CPU_GPIO_PIN_14 = 14U ,
    PHAROS_CPU_GPIO_PIN_15 = 15U ,
} PharosCpuGpioPin;


typedef enum
{
    PHAROS_CPU_GPIO_AF0 = 0U ,
    PHAROS_CPU_GPIO_AF1 = 1U ,
    PHAROS_CPU_GPIO_AF2 = 2U ,
    PHAROS_CPU_GPIO_AF3 = 3U ,
    PHAROS_CPU_GPIO_AF4 = 4U ,
    PHAROS_CPU_GPIO_AF5 = 5U ,
    PHAROS_CPU_GPIO_AF6 = 6U ,
    PHAROS_CPU_GPIO_AF7 = 7U ,
    PHAROS_CPU_GPIO_AF8 = 8U ,
    PHAROS_CPU_GPIO_AF9 = 9U ,
    PHAROS_CPU_GPIO_AF10 = 10U ,
    PHAROS_CPU_GPIO_AF11 = 11U ,
    PHAROS_CPU_GPIO_AF12 = 12U ,
    PHAROS_CPU_GPIO_AF13 = 13U ,
    PHAROS_CPU_GPIO_AF14 = 14U ,
    PHAROS_CPU_GPIO_AF15 = 15U ,
} PharosCpuGpioAf;


typedef enum
{
    PHAROS_CPU_GPIO_MODE_INPUT = 0U ,
    PHAROS_CPU_GPIO_MODE_OUTPUT = 1U ,
    PHAROS_CPU_GPIO_MODE_ALTERNATE = 2U ,
    PHAROS_CPU_GPIO_MODE_ANALOG = 3U ,
} PharosCpuGpioMode;


typedef enum
{
    PHAROS_CPU_GPIO_SPEED_LOW = 0U ,
    PHAROS_CPU_GPIO_SPEED_MEDIUM = 1U ,
    PHAROS_CPU_GPIO_SPEED_HIGH = 2U ,
    PHAROS_CPU_GPIO_SPEED_VERY_HIGH = 3U ,
} PharosCpuGpioSpeed;


typedef enum
{
    /**
     * Push-pull 
     */
    PHAROS_CPU_GPIO_OUTPUT_TYPE_PP = 0U ,

    /**
     * Open-drain
     */
    PHAROS_CPU_GPIO_OUTPUT_TYPE_OD = 1U ,
} PharosCpuGpioOutputType;


typedef enum
{
    PHAROS_CPU_GPIO_PULL_NO = 0U ,
    PHAROS_CPU_GPIO_PULL_UP = 1U ,
    PHAROS_CPU_GPIO_PULL_DOWN = 2U ,
} PharosCpuGpioPull;


struct PharosCpuGpio
{
    /**
     * GPIO port mode register (GPIOx_MODER) (x = A to K)
     * 
     * offset:      0x00
     * Reset value: 0xA800 0000 for port A
     *              0x0000 0280 for port B
     *              0x0000 0000 for other ports
     */
    volatile uint32_t portMode;

    /**
     * GPIO port output type register (GPIOx_OTYPER) (x = A to K)
     * 
     * offset:      0x04
     * Reset value: 0x0000
     */
    volatile uint32_t portOutputType;

    /**
     * GPIO port output speed register (GPIOx_OSPEEDR) (x = A to K)
     * 
     * offset:      0x08
     * Reset value: 0x0C00 0000 for port A
     *              0x0000 00C0 for port B
     *              0x0000 0000 for other ports
     */
    volatile uint32_t portOutputSpeed;

    /**
     * GPIO port pull-up/pull-down register (GPIOx_PUPDR) (x = A to K)
     * 
     * offset:      0x0C
     * Reset value: 0x6400 0000 for port A
     *              0x0000 0100 for port B
     *              0x0000 0000 for other ports
     */
    volatile uint32_t portPullUpDown;

    /**
     * GPIO port input data register (GPIOx_IDR) (x = A to K)
     * 
     * offset:      0x10
     * Reset value: 0x0000 XXXX
     */
    volatile uint32_t portInpoutData;

    /**
     * GPIO port output data register (GPIOx_ODR) (x = A to K)
     * 
     * offset:      0x14
     * Reset value: 0x0000 0000
     */
    volatile uint32_t portOutputData;

    /**
     * GPIO port bit set/reset register (GPIOx_BSRR) (x = A to K)
     * 
     * offset:      0x18
     * Reset value: 0x0000
     */
    volatile uint32_t portBitSetReset;

    /**
     * GPIO port configuration lock register (GPIOx_LCKR) (x = A to K)
     * 
     * offset:      0x1C
     * Reset value: 0x0000
     */
    volatile uint32_t portConfigurationLock;

    /**
     * GPIO alternate function low and high registers (GPIOx_AFRL and GPIOx_AFRH) (x = A to K)
     * 
     * offset:      0x20
     * Reset value: 0x0000 0000 0000 0000
     */
    volatile uint32_t alternateFunction[2];
};


/**
 * Initialize the GPIO
 */
void pharosCpuGpioInit();


/**
 * Configure the specified GPIO pin to the push up/down/no mode
 * 
 * @param gpio GPIO to configure
 * @param pin pin to configure
 * @param pull push up/down/no mode to set
 */
void pharosCpuGpioConfPull(ptrPharosCpuGpio gpio , PharosCpuGpioPin pin , PharosCpuGpioPull pull);


/**
 * Configure the specified GPIO pin to the output type 
 * 
 * @param gpio GPIO to configure
 * @param pin pin to configure
 * @param type type to set
 */
void pharosCpuGpioConfOutputType(ptrPharosCpuGpio gpio , PharosCpuGpioPin pin , PharosCpuGpioOutputType type);


/**
 * Configure the specified GPIO pin to the speed
 * 
 * @param gpio GPIO to configure
 * @param pin pin to configure
 * @param speed speed to set
 */
void pharosCpuGpioConfSpeed(ptrPharosCpuGpio gpio , PharosCpuGpioPin pin , PharosCpuGpioSpeed speed);


/**
 * Configure the specified GPIO pin to the mode
 * 
 * @param gpio GPIO to configure
 * @param pin pin to configure
 * @param mode mode to set
 */
void pharosCpuGpioConfMode(ptrPharosCpuGpio gpio , PharosCpuGpioPin pin , PharosCpuGpioMode mode);


/**
 * Configure the specified GPIO pin to the alternate function 
 * 
 * @param gpio GPIO to configure
 * @param pin pin to configure
 * @param function alternate function to set
 */
void pharosCpuGpioConfAlternate(ptrPharosCpuGpio gpio , PharosCpuGpioPin pin , PharosCpuGpioAf function);


/**
 * Configure the GPIO pin in alternate mode and specify the alternate function and speed
 * 
 * @param gpio GPIO to configure
 * @param pin pin to configure
 * @param function alternate function to set
 * @param speed speed to set
 * @param type output type to set
 */
void pharosCpuGpioConfAlt(ptrPharosCpuGpio gpio , PharosCpuGpioPin pin , PharosCpuGpioAf function , PharosCpuGpioSpeed speed, PharosCpuGpioOutputType type);


/* PHAROS_GPIO_H */
#endif	
