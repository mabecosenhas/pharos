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


#ifndef PHAROS_RCC_H
#define	PHAROS_RCC_H

#include <pharos/hal/cpu/declarations.h>

#define PHAROS_CPU_RCC                          ((ptrPharosCpuRcc) PHAROS_CPU_RCC_ADDRESS)



#define PHAROS_CPU_RCC_AHB1ENR_GPIOA_EN         (1U << 0U)      
#define PHAROS_CPU_RCC_AHB1ENR_GPIOB_EN         (1U << 1U)      
#define PHAROS_CPU_RCC_AHB1ENR_GPIOC_EN         (1U << 2U)      
#define PHAROS_CPU_RCC_AHB1ENR_GPIOD_EN         (1U << 3U)      
#define PHAROS_CPU_RCC_AHB1ENR_GPIOE_EN         (1U << 4U)      
#define PHAROS_CPU_RCC_AHB1ENR_GPIOF_EN         (1U << 5U)      
#define PHAROS_CPU_RCC_AHB1ENR_GPIOG_EN         (1U << 6U)      
#define PHAROS_CPU_RCC_AHB1ENR_GPIOH_EN         (1U << 7U)      
#define PHAROS_CPU_RCC_AHB1ENR_GPIOI_EN         (1U << 8U)      
#define PHAROS_CPU_RCC_AHB1ENR_GPIOJ_EN         (1U << 9U)      
#define PHAROS_CPU_RCC_AHB1ENR_GPIOK_EN         (1U << 10U)      
#define PHAROS_CPU_RCC_AHB1ENR_CRC_EN           (1U << 12U)      
#define PHAROS_CPU_RCC_AHB1ENR_BKP_EN           (1U << 18U)
#define PHAROS_CPU_RCC_AHB1ENR_CCM_EN           (1U << 20U)
#define PHAROS_CPU_RCC_AHB1ENR_DMA1_EN          (1U << 21U)
#define PHAROS_CPU_RCC_AHB1ENR_DMA2_EN          (1U << 22U)
#define PHAROS_CPU_RCC_AHB1ENR_DMA2D_EN         (1U << 23U)
#define PHAROS_CPU_RCC_AHB1ENR_ETH_EN           (1U << 25U)
#define PHAROS_CPU_RCC_AHB1ENR_ETH_TX_EN        (1U << 26U)
#define PHAROS_CPU_RCC_AHB1ENR_ETH_RX_EN        (1U << 27U)
#define PHAROS_CPU_RCC_AHB1ENR_ETH_PT_EN        (1U << 28U)
#define PHAROS_CPU_RCC_AHB1ENR_OTG_EN           (1U << 29U)
#define PHAROS_CPU_RCC_AHB1ENR_OTG_ULPI_EN      (1U << 30U)


#define PHAROS_CPU_RCC_AHB3ENR_FMCEN_DIS        (0U << 0U)
#define PHAROS_CPU_RCC_AHB3ENR_FMCEN_EN         (1U << 0U)


#define PHAROS_CPU_RCC_APB1ENR_TIM2_EN          (1U << 0U)
#define PHAROS_CPU_RCC_APB1ENR_TIM3_EN          (1U << 1U)
#define PHAROS_CPU_RCC_APB1ENR_TIM4_EN          (1U << 2U)
#define PHAROS_CPU_RCC_APB1ENR_TIM5_EN          (1U << 3U)
#define PHAROS_CPU_RCC_APB1ENR_TIM6_EN          (1U << 4U)
#define PHAROS_CPU_RCC_APB1ENR_TIM7_EN          (1U << 5U)
#define PHAROS_CPU_RCC_APB1ENR_TIM12_EN         (1U << 6U)
#define PHAROS_CPU_RCC_APB1ENR_TIM13_EN         (1U << 7U)
#define PHAROS_CPU_RCC_APB1ENR_TIM14_EN         (1U << 8U)
#define PHAROS_CPU_RCC_APB1ENR_WWDG_EN          (1U << 11U)
#define PHAROS_CPU_RCC_APB1ENR_SPI2_EN          (1U << 14U)
#define PHAROS_CPU_RCC_APB1ENR_SPI3_EN          (1U << 15U)
#define PHAROS_CPU_RCC_APB1ENR_USART2_EN        (1U << 17U)
#define PHAROS_CPU_RCC_APB1ENR_USART3_EN        (1U << 18U)
#define PHAROS_CPU_RCC_APB1ENR_UART4_EN         (1U << 19U)
#define PHAROS_CPU_RCC_APB1ENR_UART5_EN         (1U << 20U)
#define PHAROS_CPU_RCC_APB1ENR_I2C1_EN          (1U << 21U)
#define PHAROS_CPU_RCC_APB1ENR_I2C2_EN          (1U << 22U)
#define PHAROS_CPU_RCC_APB1ENR_I2C3_EN          (1U << 23U)
#define PHAROS_CPU_RCC_APB1ENR_CAN1_EN          (1U << 25U)
#define PHAROS_CPU_RCC_APB1ENR_CAN2_EN          (1U << 26U)
#define PHAROS_CPU_RCC_APB1ENR_PWR_EN           (1U << 28U)
#define PHAROS_CPU_RCC_APB1ENR_DAC_EN           (1U << 29U)
#define PHAROS_CPU_RCC_APB1ENR_UART7_EN         (1U << 30U)
#define PHAROS_CPU_RCC_APB1ENR_UART8_EN         (1U << 31U)


#define PHAROS_CPU_RCC_CR_HSE_ON            (1 << 16)
#define PHAROS_CPU_RCC_CR_HSE_RDY           (1 << 17)
#define PHAROS_CPU_RCC_CR_PLL_ON            (1 << 24)
#define PHAROS_CPU_RCC_CR_PLL_RDY           (1 << 25)


#define PHAROS_CPU_RCC_PLLCFGR_HSE          (1 << 22)
#define PHAROS_CPU_RCC_PLLCFGR_M(x)         (x << 0)
#define PHAROS_CPU_RCC_PLLCFGR_N(x)         (x << 6)
#define PHAROS_CPU_RCC_PLLCFGR_P(x)         (((x >> 1) - 1) << 16)
#define PHAROS_CPU_RCC_PLLCFGR_Q(x)         (x << 24)
#define PHAROS_CPU_RCC_PLLCFGR_R(x)         (x << 28)

#define PHAROS_CPU_RCC_CFGR_HPRE(x)             (x << 4)
#define PHAROS_CPU_RCC_CFGR_HPRE_DIV_1          (0b000)
#define PHAROS_CPU_RCC_CFGR_PPRE1(x)            (x << 10) 
#define PHAROS_CPU_RCC_CFGR_PPRE1_DIV_4         (0b101)
#define PHAROS_CPU_RCC_CFGR_PPRE1_DIV_16        (0b111)
#define PHAROS_CPU_RCC_CFGR_PPRE2(x)            (x << 13)
#define PHAROS_CPU_RCC_CFGR_PPRE2_DIV_2         (0b100)
#define PHAROS_CPU_RCC_CFGR_PPRE2_DIV_16        (0b111)
#define PHAROS_CPU_RCC_CFGR_SW(x)               (x << 0)
#define PHAROS_CPU_RCC_CFGR_SW_PLL_P            (0b10)


struct PharosCpuRcc
{
    /**
     * RCC clock control register (RCC_CR)
     * 
     * offset:        0x00
     * Reset value: 0x0000
     */
    volatile uint32_t clockControl;

    /**
     * RCC PLL configuration register (RCC_PLLCFGR)
     * 
     * offset:        0x04
     * Reset value: 0x0000
     */
    volatile uint32_t pllConfiguration;

    /**
     * RCC clock configuration register (RCC_CFGR)
     * 
     * offset:        0x08
     * Reset value: 0x0000
     */
    volatile uint32_t clockConfiguration;

    /**
     * RCC clock interrupt register (RCC_CIR)
     * 
     * offset:        0x0c
     * Reset value: 0x0000
     */
    volatile uint32_t clockInterrupt;

    /**
     * RCC AHB1 peripheral reset register (RCC_AHB1RSTR)
     * 
     * offset:        0x10
     * Reset value: 0x0000
     */
    volatile uint32_t ahb1PeripheralReset;

    /**
     * RCC AHB2 peripheral reset register (RCC_AHB2RSTR)
     * 
     * offset:        0x14
     * Reset value: 0x0000
     */
    volatile uint32_t ahb2PeripheralReset;

    /**
     * RCC AHB3 peripheral reset register (RCC_AHB3RSTR)
     * 
     * offset:        0x18
     * Reset value: 0x0000
     */
    volatile uint32_t ahb3PeripheralReset;

    /**
     * Reserved
     */
    uint32_t reserved0;
    
    /**
     * RCC APB1 peripheral reset register (RCC_APB1RSTR)
     * 
     * offset:        0x20
     * Reset value: 0x0000
     */
    volatile uint32_t apb1PeripheralReset;

    /**
     * RCC APB2 peripheral reset register (RCC_APB2RSTR)
     * 
     * offset:        0x24
     * Reset value: 0x0000
     */
    volatile uint32_t apb2PeripheralReset;

    /**
     * Reserved
     */
    uint32_t reserved1[2];

    /**
     * RCC AHB1 peripheral clock enable register (RCC_AHB1ENR)
     * 
     * offset:        0x30
     * Reset value: 0x0010 0000
     */
    volatile uint32_t ahb1PeripheralClockEnable;

    /**
     * RCC AHB2 peripheral clock enable register (RCC_AHB2ENR)
     * 
     * offset:        0x34
     * Reset value: 0x0000
     */
    volatile uint32_t ahb2PeripheralClockEnable;

    /**
     * RCC AHB3 peripheral clock enable register (RCC_AHB3ENR)
     * 
     * offset:        0x38
     * Reset value: 0x0000
     */
    volatile uint32_t ahb3PeripheralClockEnable;

    /**
     * Reserved
     */
    uint32_t reserved2;

    /**
     * RCC APB1 peripheral clock enable register (RCC_APB1ENR)
     * 
     * offset:        0x40
     * Reset value: 0x0000
     */
    volatile uint32_t apb1PeripheralClockEnable;

    /**
     * RCC APB2 peripheral clock enable register (RCC_APB2ENR)
     * 
     * offset:        0x44
     * Reset value: 0x0000
     */
    volatile uint32_t apb2PeripheralClockEnable;

    /**
     * Reserved
     */
    volatile uint32_t reserved3[2];

    /**
     * RCC AHB1 peripheral clock enable in low power mode register (RCC_AHB1LPENR)
     * 
     * offset:      0x50
     * Reset value: 0x7EEF 97FF
     */
    volatile uint32_t ahb1PeripheralClockEnableLowPower;

    /**
     * RCC AHB2 peripheral clock enable in low power mode register (RCC_AHB2LPENR)
     * 
     * offset:      0x54
     * Reset value: 0x0000 00F1
     */
    volatile uint32_t ahb2PeripheralClockEnableLowPower;

    /**
     * RCC AHB3 peripheral clock enable in low power mode register (RCC_AHB3LPENR)
     * 
     * offset:        0x58
     * Reset value: 0x0003
     */
    volatile uint32_t ahb3PeripheralClockEnableLowPower;

    /**
     * Reserved
     */
    volatile uint32_t reserved4;

    /**
     * RCC APB1  peripheral clock enable in low power mode register (RCC_APB1LPENR)
     * 
     * offset:      0x60
     * Reset value: 0xF6FE C9FF
     */
    volatile uint32_t apb1PeripheralClockEnableLowPower;

    /**
     * RCC APB2 peripheral clock enabled in low power mode register (RCC_APB2LPENR)
     * 
     * offset:      0x64
     * Reset value: 0x0C77 7F33
     */
    volatile uint32_t apb2PeripheralClockEnableLowPower;

    /**
     * Reserved
     */
    volatile uint32_t reserved5[2];

    /**
     * RCC Backup domain control register (RCC_BDCR)
     * 
     * offset:        0x70
     * Reset value: 0x0000
     */
    volatile uint32_t backupDomainControl;

    /**
     * RCC clock control & status register (RCC_CSR)
     * 
     * offset:      0x74
     * Reset value: 0x0E00 0000
     */
    volatile uint32_t clockControlStatus;

    /**
     * Reserved
     */
    volatile uint32_t reserved6[2];

    /**
     * RCC spread spectrum clock generation register (RCC_SSCGR)
     * 
     * offset:        0x80
     * Reset value: 0x0000
     */
    volatile uint32_t spreadSpectrumClockGeneration;

    /**
     * RCC PLLI2S configuration register (RCC_PLLI2SCFGR)
     * 
     * offset:      0x84
     * Reset value: 0x2400 3000
     */
    volatile uint32_t pllI2SConfiguration;

    /**
     * RCC PLL configuration register (RCC_PLLSAICFGR)
     * 
     * offset:        0x88
     * Reset value: 0x2400 3000
     */
    volatile uint32_t pllSaiConfiguration;

    /**
     * RCC Dedicated Clock Configuration Register (RCC_DCKCFGR)
     * 
     * offset:        0x8c
     * Reset value: 0x0000
     */
    volatile uint32_t dedicatedClockConfiguration;

};


/* PHAROS_RCC_H */
#endif	
