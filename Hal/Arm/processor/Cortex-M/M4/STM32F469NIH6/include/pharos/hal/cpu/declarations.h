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


#ifndef PHAROS_CPU_STM32_DECLARATIONS_H
#define	PHAROS_CPU_STM32_DECLARATIONS_H

#include <pharos/hal/hal.h>

typedef struct PharosCpuGpTimer2_5 PharosCpuGpTimer2_5 , *ptrPharosCpuGpTimer2_5;
typedef struct PharosCpuAdvCtrTimer PharosCpuAdvCtrTimer , *ptrPharosCpuAdvCtrTimer;
typedef struct PharosCpuGpio PharosCpuGpio , *ptrPharosCpuGpio;
typedef struct PharosCpuRcc PharosCpuRcc , *ptrPharosCpuRcc;
typedef struct PharosCpuPwr PharosCpuPwr, *ptrPharosCpuPwr;
typedef struct PharosCpuFlash PharosCpuFlash, *ptrPharosCpuFlash;
typedef struct PharosCpuFmc PharosCpuFmc , *ptrPharosCpuFmc;
typedef struct PharosCpuUsart PharosCpuUsart , *ptrPharosCpuUsart;


/****************************************************************************
 * Fatal errors definitions for this CPU                                    *
 ****************************************************************************/


/**
 * STM32F469NIH6 CPU Interrupt Table
 * 
 * Note that the numbering does not match exactly the documentation (STM32F469xx and STM32F479xx
 * advanced Arm ® -based 32-bit MCUs Reference Manual). It contains an offset that makes the internal
 * Pharos programming simpler and faster. Please use the following enumeration when installing an interrupt in Pharos.
 */
typedef enum
{
    /************************************************************
     *   ARMV7-M general Interrupt Numbers                       *
     ************************************************************/
    PHAROS_CPU_EXCEPTION_RESET = 1U ,
    PHAROS_CPU_EXCEPTION_NMI = 2U ,
    PHAROS_CPU_EXCEPTION_HARD_FAULT = 3U ,
    PHAROS_CPU_EXCEPTION_MPU = 4U ,
    PHAROS_CPU_EXCEPTION_BUS_FAULT = 5U ,
    PHAROS_CPU_EXCEPTION_USAGE = 6U ,
    PHAROS_CPU_EXCEPTION_SUPER_CALL = 11U ,
    PHAROS_CPU_EXCEPTION_DEBUG_MON = 12U ,
    PHAROS_CPU_EXCEPTION_PENDABLE = 14U ,
    PHAROS_CPU_EXCEPTION_SYSTICK = 15U ,

    /************************************************************
     *   STM32F469NIH6 specific Interrupt Numbers               *
     ************************************************************/
    PHAROS_CPU_ISR_WATCHDOG = 16U ,
    PHAROS_CPU_ISR_PVD = 17U ,
    PHAROS_CPU_ISR_TAMPER_STAMP = 18U ,
    PHAROS_CPU_ISR_RTC_WAKEUP = 19U ,
    PHAROS_CPU_ISR_FLASH = 20U ,
    PHAROS_CPU_ISR_RCC = 21U ,
    PHAROS_CPU_ISR_EXT_LINE0 = 22U ,
    PHAROS_CPU_ISR_EXT_LINE1 = 23U ,
    PHAROS_CPU_ISR_EXT_LINE2 = 24U ,
    PHAROS_CPU_ISR_EXT_LINE3 = 25U ,
    PHAROS_CPU_ISR_EXT_LINE4 = 26U ,
    PHAROS_CPU_ISR_DMA1_STREAM0 = 27U ,
    PHAROS_CPU_ISR_DMA1_STREAM1 = 28U ,
    PHAROS_CPU_ISR_DMA1_STREAM2 = 29U ,
    PHAROS_CPU_ISR_DMA1_STREAM3 = 30U ,
    PHAROS_CPU_ISR_DMA1_STREAM4 = 31U ,
    PHAROS_CPU_ISR_DMA1_STREAM5 = 32U ,
    PHAROS_CPU_ISR_DMA1_STREAM6 = 33U ,
    PHAROS_CPU_ISR_ADC = 34U ,
    PHAROS_CPU_ISR_CAN1_TX = 35U ,
    PHAROS_CPU_ISR_CAN1_RX0 = 36U ,
    PHAROS_CPU_ISR_CAN1_RX1 = 37U ,
    PHAROS_CPU_ISR_CAN1_SCE = 38U ,
    PHAROS_CPU_ISR_EXT_LINE9_5 = 39U ,
    PHAROS_CPU_ISR_TIM1_BRK_TIMER9 = 40U ,
    PHAROS_CPU_ISR_TIMER1_UP_TIMER10 = 41U ,
    PHAROS_CPU_ISR_TIMER1_TRG_COMM_TIMER11 = 42U ,
    PHAROS_CPU_ISR_TIMER1_CPT_CMP = 43U ,
    PHAROS_CPU_ISR_TIMER2 = 44U ,
    PHAROS_CPU_ISR_TIMER3 = 45U ,
    PHAROS_CPU_ISR_TIMER4 = 46U ,
    PHAROS_CPU_ISR_I2C1_EVENT = 47U ,
    PHAROS_CPU_ISR_I2C1_ERROR = 48U ,
    PHAROS_CPU_ISR_I2C2_EVENT = 49U ,
    PHAROS_CPU_ISR_I2C2_ERROR = 50U ,
    PHAROS_CPU_ISR_SPI1 = 51U ,
    PHAROS_CPU_ISR_SPI2 = 52U ,
    PHAROS_CPU_ISR_USART1 = 53U ,
    PHAROS_CPU_ISR_USART2 = 54U ,
    PHAROS_CPU_ISR_USART3 = 55U ,
    PHAROS_CPU_ISR_EXT_LINE15_10 = 56U ,
    PHAROS_CPU_ISR_RTC_ALARM = 57U ,
    PHAROS_CPU_ISR_USB_OTG_FS_WKUP = 58U ,
    PHAROS_CPU_ISR_TIMER8_BRK_TIMER12 = 59U ,
    PHAROS_CPU_ISR_TIMER8_UP_TIMER13 = 60U ,
    PHAROS_CPU_ISR_TIMER8_TRG_COMM_TIMER14 = 61U ,
    PHAROS_CPU_ISR_TIMER8_CPT_CMP = 62U ,
    PHAROS_CPU_ISR_DMA1_STREAM7 = 63U ,
    PHAROS_CPU_ISR_FMC = 64U ,
    PHAROS_CPU_ISR_SDIO = 65U ,
    PHAROS_CPU_ISR_TIMER5 = 66U ,
    PHAROS_CPU_ISR_SPI3 = 67U ,
    PHAROS_CPU_ISR_UART4 = 68U ,
    PHAROS_CPU_ISR_UART5 = 69U ,
    PHAROS_CPU_ISR_TIMER6_DAC1_ERROR_DAC2_ERROR = 70U ,
    PHAROS_CPU_ISR_TIMER7 = 71U ,
    PHAROS_CPU_ISR_DMA2_STREAM0 = 72U ,
    PHAROS_CPU_ISR_DMA2_STREAM1 = 73U ,
    PHAROS_CPU_ISR_DMA2_STREAM2 = 74U ,
    PHAROS_CPU_ISR_DMA2_STREAM3 = 75U ,
    PHAROS_CPU_ISR_DMA2_STREAM4 = 76U ,
    PHAROS_CPU_ISR_ETHERNET = 77U ,
    PHAROS_CPU_ISR_ETHERNET_WKUP = 78U ,
    PHAROS_CPU_ISR_CAN2_TX = 79U ,
    PHAROS_CPU_ISR_CAN2_RX0 = 80U ,
    PHAROS_CPU_ISR_CAN2_RX1 = 81U ,
    PHAROS_CPU_ISR_CAN2_SCE = 82U ,
    PHAROS_CPU_ISR_USB_OTG_FS = 83U ,
    PHAROS_CPU_ISR_DMA2_STREAM5 = 84U ,
    PHAROS_CPU_ISR_DMA2_STREAM6 = 85U ,
    PHAROS_CPU_ISR_DMA2_STREAM7 = 86U ,
    PHAROS_CPU_ISR_USART6 = 87U ,
    PHAROS_CPU_ISR_I2C3_EVENT = 88U ,
    PHAROS_CPU_ISR_I2C3_ERROR = 89U ,
    PHAROS_CPU_ISR_USB_OTG_HS_ENDPOINT1_OUT = 90U ,
    PHAROS_CPU_ISR_USB_OTG_HS_ENDPOINT1_IN = 91U ,
    PHAROS_CPU_ISR_USB_OTG_HS_WKUP = 92U ,
    PHAROS_CPU_ISR_USB_OTG_HS = 93U ,
    PHAROS_CPU_ISR_DCMI = 94U ,
    PHAROS_CPU_ISR_CRYP = 95U ,
    PHAROS_CPU_ISR_HASH_RNG = 96U ,
    PHAROS_CPU_ISR_FPU = 97U ,
    PHAROS_CPU_ISR_UART7 = 98U ,
    PHAROS_CPU_ISR_UART8 = 99U ,
    PHAROS_CPU_ISR_SPI4 = 100U ,
    PHAROS_CPU_ISR_SPI5 = 101U ,
    PHAROS_CPU_ISR_SPI6 = 102U ,
    PHAROS_CPU_ISR_SAI1 = 103U ,
    PHAROS_CPU_ISR_LCD_TFT = 104U ,
    PHAROS_CPU_ISR_LCD_TFT_ERROR = 105U ,
    PHAROS_CPU_ISR_DMA2D = 106U ,
    PHAROS_CPU_ISR_QUAD_SPI = 107U ,
    PHAROS_CPU_ISR_DSI = 108U ,
} PharosInterruptNumber;

/* PHAROS_CPU_STM32_DECLARATIONS_H */
#endif	
