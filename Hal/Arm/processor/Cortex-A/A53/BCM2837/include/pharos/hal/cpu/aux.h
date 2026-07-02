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


#ifndef PHAROS_AUX_H
#define	PHAROS_AUX_H

#include <pharos/hal/cpu/declarations.h>
#include <pharos/hal/cpu/conf.h>


#define PHAROS_CPU_AUX_PER_MAP       ((ptrPharosCpuAuxiliaryPerMap) PHAROS_CPU_AUX_PER_MAP_ADDRESS)

/**
 * Compact the size of the I/O registers
 */
#pragma pack(push ,4)


struct PharosCpuAuxiliaryPerMap
{
    /**
     * Auxiliary Interrupt status (AUX_IRQ)
     * 
     * offset: 0x0
     */
    volatile uint32_t auxiliaryInterruptStatus;

    /**
     * Auxiliary enables (AUX_ENABLES)
     * 
     * offset: 0x4
     */
    volatile uint32_t auxiliaryEnables;

    /**
     * Reserved
     * 
     * offset: 0x8-3c
     */
    uint32_t reserved1[14];

    /**
     * Mini Uart I/O Data  (AUX_MU_IO_REG)
     * 
     * offset: 0x40
     */
    volatile uint32_t miniUartIoData;

    /**
     * Mini Uart Interrupt Enable (AUX_MU_IER_REG)
     * 
     * offset: 0x44
     */
    volatile uint32_t miniUartInterruptEnable;

    /**
     * Mini Uart Interrupt Identify (AUX_MU_IIR_REG)
     * 
     * offset: 0x48
     */
    volatile uint32_t miniUartInterruptIdentify;

    /**
     * Mini Uart Line Control (AUX_MU_LCR_REG)
     * 
     * offset: 0x4c
     */
    volatile uint32_t miniUartLineControl;

    /**
     * Mini Uart Modem Control (AUX_MU_MCR_REG)
     * 
     * offset: 0x50
     */
    volatile uint32_t miniUartModemControl;

    /**
     * Mini Uart Line Status (AUX_MU_LSR_REG)
     * 
     * offset: 0x54
     */
    volatile uint32_t miniUartLineStatus;

    /**
     * Mini Uart Modem Status (AUX_MU_MSR_REG)
     * 
     * offset: 0x58
     */
    volatile uint32_t miniUartModemStatus;

    /**
     * Mini Uart Scratch (AUX_MU_SCRATCH)
     * 
     * offset: 0x5c
     */
    volatile uint32_t miniUartScratch;

    /**
     * Mini Uart Extra Control (AUX_MU_CNTL_REG)
     * 
     * offset: 0x60
     */
    volatile uint32_t miniUartExtraControl;

    /**
     * Mini Uart Extra Status (AUX_MU_STAT_REG)
     * 
     * offset: 0x64
     */
    volatile uint32_t miniUartExtraStatus;

    /**
     * Mini Uart Baudrate (AUX_MU_BAUD_REG)
     * 
     * offset: 0x68
     */
    volatile uint32_t miniUartBaudrate;

    /**
     * Reserved
     * 
     * offset: 0x6c-7c
     */
    uint32_t reserved2[5];

    /**
     * SPI 1 Control register 0 (AUX_SPI0_CNTL0_REG)
     * 
     * offset: 0x80
     */
    volatile uint32_t spi1Control0;

    /**
     * SPI 1 Control register 1 (AUX_SPI0_CNTL1_REG)
     * 
     * offset: 0x84
     */
    volatile uint32_t spi1Control1;

    /**
     *  SPI 1 Status (AUX_SPI0_STAT_REG)
     * 
     * offset: 0x88
     */
    volatile uint32_t spi1Status;

    /**
     * Reserved
     * 
     * offset: 0x8c
     */
    volatile uint32_t reserved3;

    /**
     * SPI 1 Data (AUX_SPI0_IO_REG)     
     * 
     * offset: 0x90
     */
    volatile uint32_t spi1Data;

    /**
     * SPI 1 Peek (AUX_SPI0_PEEK_REG)
     * 
     * offset: 0x94
     */
    volatile uint32_t spi1Peek;

    /**
     * Reserved
     * 
     * offset: 0x98-2c
     */
    uint32_t reserved4[10];

    /**
     * SPI 2 Control register 0 (AUX_SPI1_CNTL0_REG)
     * 
     * offset: 0xc0
     */
    volatile uint32_t spi2Control0;

    /**
     * SPI 2 Control register 1 (AUX_SPI1_CNTL1_REG)
     * 
     * offset: 0xc4
     */
    volatile uint32_t spi2Control1;

    /**
     * SPI 2 Status (AUX_SPI1_STAT_REG)
     * 
     * offset: 0xc8
     */
    volatile uint32_t spi2Status;

    /**
     * Reserved
     * 
     * offset: 0xcc
     */
    uint32_t reserved5;

    /**
     * SPI 2 Data (AUX_SPI1_IO_REG)
     * 
     * offset: 0xd0
     */
    volatile uint32_t spi2Data;

    /**
     * SPI 2 Peek (AUX_SPI1_PEEK_REG)
     * 
     * offset: 0xd4
     */
    volatile uint32_t spi2Peek;

};

/**
 * restore the pack definition
 */
#pragma pack(pop)


/* PHAROS_AUX_H */
#endif	
