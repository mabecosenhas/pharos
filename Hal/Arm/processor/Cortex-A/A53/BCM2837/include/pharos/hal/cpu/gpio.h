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

#define PHAROS_CPU_GPIO                     ((ptrPharosCpuGpio) PHAROS_CPU_GPIO_ADDRESS)


#define PHAROS_CPU_GPFSEL1_MASK(x)          (0b111 << (3U*(x-10U)))

#define PHAROS_CPU_GPFSEL1_INPUT(x)         (0b000 << (3U*(x-10U)))
#define PHAROS_CPU_GPFSEL1_OUTPUT(x)        (0b001 << (3U*(x-10U)))
#define PHAROS_CPU_GPFSEL1_ALTER0(x)        (0b100 << (3U*(x-10U)))
#define PHAROS_CPU_GPFSEL1_ALTER1(x)        (0b101 << (3U*(x-10U)))
#define PHAROS_CPU_GPFSEL1_ALTER2(x)        (0b110 << (3U*(x-10U)))
#define PHAROS_CPU_GPFSEL1_ALTER3(x)        (0b111 << (3U*(x-10U)))
#define PHAROS_CPU_GPFSEL1_ALTER4(x)        (0b011 << (3U*(x-10U)))
#define PHAROS_CPU_GPFSEL1_ALTER5(x)        (0b010 << (3U*(x-10U)))


#define PHAROS_CPU_GPPUDCLK0_EN(x)      (1U << x)


/**
 * Compact the size of the I/O registers
 */
#pragma pack(push ,4)


struct PharosCpuGpio
{
    /**
     * GPIO Function Select 0 (GPFSEL0)
     * 
     * offset: 0x0
     */
    volatile uint32_t functionSelect0;

    /**
     * GPIO Function Select 1 (GPFSEL1)
     * 
     * offset: 0x4
     */
    volatile uint32_t functionSelect1;

    /**
     * GPIO Function Select 2 (GPFSEL2)
     * 
     * offset: 0x8
     */
    volatile uint32_t functionSelect2;

    /**
     * GPIO Function Select 3 (GPFSEL3)
     * 
     * offset: 0xc
     */
    volatile uint32_t functionSelect3;

    /**
     * GPIO Function Select 4 (GPFSEL4)
     * 
     * offset: 0x10
     */
    volatile uint32_t functionSelect4;

    /**
     * GPIO Function Select 5 (GPFSEL5)
     * 
     * offset: 0x14
     */
    volatile uint32_t functionSelect5;

    /**
     * Reserved
     * 
     * offset: 0x18
     */
    volatile uint32_t reserved1;

    /**
     * GPIO Pin Output Set 0 (GPSET0)
     * 
     * offset: 0x1c
     */
    volatile uint32_t pinOutputSet0;

    /**
     * GPIO Pin Output Set 1 (GPSET1)
     * 
     * offset: 0x20
     */
    volatile uint32_t pinOutputSet1;

    /**
     * Reserved
     * 
     * offset: 0x24
     */
    volatile uint32_t reserved2;

    /**
     * GPIO Pin Output Clear 0 (GPCLR0)
     * 
     * offset: 0x28
     */
    volatile uint32_t pinOutputClear0;

    /**
     * GPIO Pin Output Clear 1 (GPCLR1)
     * 
     * offset: 0x2c
     */
    volatile uint32_t pinOutputClear1;

    /**
     * Reserved
     * 
     * offset: 0x30
     */
    volatile uint32_t reserved3;

    /**
     * GPIO Pin Level 0 (GPLEV0)
     * 
     * offset: 0x34
     */
    volatile uint32_t pinLevel0;

    /**
     * GPIO Pin Level 1 (GPLEV1)
     * 
     * offset: 0x38
     */
    volatile uint32_t pinLevel1;

    /**
     * Reserved
     * 
     * offset: 0x3c
     */
    volatile uint32_t reserved4;

    /**
     * GPIO Pin Event Detect Status 0 (GPEDS0)
     * 
     * offset: 0x40
     */
    volatile uint32_t pinEventDetectStatus0;

    /**
     * GPIO Pin Event Detect Status 1 (GPEDS1)
     * 
     * offset: 0x44
     */
    volatile uint32_t pinEventDetectStatus1;

    /**
     * Reserved
     * 
     * offset: 0x48
     */
    volatile uint32_t reserved5;

    /**
     * GPIO Pin Rising Edge Detect Enable 0  (GPREN0)
     * 
     * offset: 0x4c
     */
    volatile uint32_t pinRisingEdgeDetectEnable0;

    /**
     * GPIO Pin Rising Edge Detect Enable 1  (GPREN1)
     * 
     * offset: 0x50
     */
    volatile uint32_t pinRisingEdgeDetectEnable1;

    /**
     * Reserved
     * 
     * offset: 0x54
     */
    volatile uint32_t reserved6;

    /**
     * GPIO Pin Falling Edge Detect Enable 0 (GPFEN0)
     * 
     * offset: 0x58
     */
    volatile uint32_t pinFallingEdgeDetectEnable0;

    /**
     * GPIO Pin Falling Edge Detect Enable 1 (GPFEN1)
     * 
     * offset: 0x5c
     */
    volatile uint32_t pinFallingEdgeDetectEnable1;

    /**
     * Reserved
     * 
     * offset: 0x60
     */
    volatile uint32_t reserved7;

    /**
     * GPIO Pin High Detect Enable 0 (GPHEN0)
     * 
     * offset: 0x64
     */
    volatile uint32_t pinHighDetectEnable0;

    /**
     * GPIO Pin High Detect Enable 1 (GPHEN1)
     * 
     * offset: 0x68
     */
    volatile uint32_t pinHighDetectEnable1;

    /**
     * Reserved
     * 
     * offset: 0x6c
     */
    volatile uint32_t reserved8;

    /**
     * GPIO Pin Low Detect Enable 0 (GPLEN0)
     * 
     * offset: 0x70
     */
    volatile uint32_t pinLowDetectEnable0;

    /**
     * GPIO Pin Low Detect Enable 1 (GPLEN1)
     * 
     * offset: 0x74
     */
    volatile uint32_t pinLowDetectEnable1;

    /**
     * Reserved
     * 
     * offset: 0x78
     */
    volatile uint32_t reserved9;

    /**
     * GPIO Pin Asynchronous Rising Edge Detect 0 (GPAREN0)
     * 
     * offset: 0x7c
     */
    volatile uint32_t pinAsyncRisingEdgeDetect0;

    /**
     * GPIO Pin Asynchronous Rising Edge Detect 1 (GPAREN1)
     * 
     * offset: 0x80
     */
    volatile uint32_t pinAsyncRisingEdgeDetect1;

    /**
     * Reserved
     * 
     * offset: 0x84
     */
    volatile uint32_t reserved10;

    /**
     * GPIO Pin Asynchronous Falling Edge Detect 0 (GPAFEN0)
     * 
     * offset: 0x88
     */
    volatile uint32_t pinAsyncFallingEdgeDetect0;

    /**
     * GPIO Pin Asynchronous Falling Edge Detect 1 (GPAFEN1)
     * 
     * offset: 0x8c
     */
    volatile uint32_t pinAsyncFallingEdgeDetect1;

    /**
     * Reserved
     * 
     * offset: 0x90
     */
    volatile uint32_t reserved11;

    /**
     * GPIO Pin Pull-up/down Enable (GPPUD)
     * 
     * offset: 0x94
     */
    volatile uint32_t pinPullUpDownEnable;

    /**
     * GPIO Pin Pull-up/down Enable Clock 0 (GPPUDCLK0)
     * 
     * offset: 0x98
     */
    volatile uint32_t pinPullUpDownEnableClock0;

    /**
     * GPIO Pin Pull-up/down Enable Clock 1 (GPPUDCLK1)
     * 
     * offset: 0x9c
     */
    volatile uint32_t pinPullUpDownEnableClock1;
};


/**
 * restore the pack definition
 */
#pragma pack(pop)


void pharosCpuUartInit(void);

/* PHAROS_GPIO_H */
#endif	
