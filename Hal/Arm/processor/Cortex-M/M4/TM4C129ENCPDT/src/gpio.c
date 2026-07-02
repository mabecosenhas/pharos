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
#include <pharos/hal/cpu/gpio.h>
#include <pharos/hal/cpu/rom.h>

/**
 * Auxiliary define to access a hardware register
 */
#define HWREG(x)                            (*((volatile uint32_t *)(x)))

/**
 * GPIO High-Performance Bus
 */
#define PHAROS_CPU_SYSCTL_GPIOHBCTL         0x400FE06C  

typedef void (*inTypeUart) (uint32_t ui32Port , uint8_t ui8Pins);
typedef void (*PinTypeOutput) (uint32_t ui32Port , uint8_t ui8Pins);


/*****************************************************************************
 * The base addresses of all the GPIO modules 
 * Both the APB and AHB aperture are provided.
 *****************************************************************************/
static const uint32_t g_pui32GPIOBaseAddrs[] = {
    PHAROS_CPU_GPIO_PORTA_BASE ,
    PHAROS_CPU_GPIO_PORTA_AHB_BASE ,
    PHAROS_CPU_GPIO_PORTB_BASE ,
    PHAROS_CPU_GPIO_PORTB_AHB_BASE ,
    PHAROS_CPU_GPIO_PORTC_BASE ,
    PHAROS_CPU_GPIO_PORTC_AHB_BASE ,
    PHAROS_CPU_GPIO_PORTD_BASE ,
    PHAROS_CPU_GPIO_PORTD_AHB_BASE ,
    PHAROS_CPU_GPIO_PORTE_BASE ,
    PHAROS_CPU_GPIO_PORTE_AHB_BASE ,
    PHAROS_CPU_GPIO_PORTF_BASE ,
    PHAROS_CPU_GPIO_PORTF_AHB_BASE ,
    PHAROS_CPU_GPIO_PORTG_BASE ,
    PHAROS_CPU_GPIO_PORTG_AHB_BASE ,
    PHAROS_CPU_GPIO_PORTH_BASE ,
    PHAROS_CPU_GPIO_PORTH_AHB_BASE ,
    PHAROS_CPU_GPIO_PORTJ_BASE ,
    PHAROS_CPU_GPIO_PORTJ_AHB_BASE ,
    PHAROS_CPU_GPIO_PORTK_BASE ,
    PHAROS_CPU_GPIO_PORTK_BASE ,
    PHAROS_CPU_GPIO_PORTL_BASE ,
    PHAROS_CPU_GPIO_PORTL_BASE ,
    PHAROS_CPU_GPIO_PORTM_BASE ,
    PHAROS_CPU_GPIO_PORTM_BASE ,
    PHAROS_CPU_GPIO_PORTN_BASE ,
    PHAROS_CPU_GPIO_PORTN_BASE ,
    PHAROS_CPU_GPIO_PORTP_BASE ,
    PHAROS_CPU_GPIO_PORTP_BASE ,
    PHAROS_CPU_GPIO_PORTQ_BASE ,
    PHAROS_CPU_GPIO_PORTQ_BASE ,
    PHAROS_CPU_GPIO_PORTR_BASE ,
    PHAROS_CPU_GPIO_PORTR_BASE ,
    PHAROS_CPU_GPIO_PORTS_BASE ,
    PHAROS_CPU_GPIO_PORTS_BASE ,
    PHAROS_CPU_GPIO_PORTT_BASE ,
    PHAROS_CPU_GPIO_PORTT_BASE ,
};


void pharosCpuGpioPinConfigure(uint32_t ui32PinConfig)
{
    /* base address */
    uint32_t ui32Base;

    /* shift */
    uint32_t ui32Shift;


    /* extract the base address index from the input value */
    ui32Base = (ui32PinConfig >> 16) & 0xff;

    /* get the base address of the GPIO module*/
    if(HWREG(PHAROS_CPU_SYSCTL_GPIOHBCTL) & (1 << ui32Base))
    {
        /* use the base address */
        ui32Base = g_pui32GPIOBaseAddrs[(ui32Base << 1) + 1];
    }
    else
    {
        /* use the AHB base address */
        ui32Base = g_pui32GPIOBaseAddrs[ui32Base << 1];
    }

    /* Extract the shift from the input value */
    ui32Shift = (ui32PinConfig >> 8) & 0xff;

    /* Write the requested pin muxing value for this GPIO pin */
    HWREG(ui32Base + PHAROS_CPU_GPIO_O_PCTL) = ((HWREG(ui32Base + PHAROS_CPU_GPIO_O_PCTL) & ~(0xf << ui32Shift)) |
                                                ((ui32PinConfig & 0xf) << ui32Shift));
}


void pharosCpuGpioPinTypeUart(uint32_t ui32Port , uint8_t ui8Pins)
{
    /* use the ROM function to enable the peripherals */
    ((inTypeUart) ROM_GPIOTABLE[21])(ui32Port , ui8Pins);
}


void pharosCpuGpioPinTypeOutput(uint32_t ui32Port , uint8_t ui8Pins)
{
    ((PinTypeOutput) ROM_GPIOTABLE[15])(ui32Port , ui8Pins);
}
