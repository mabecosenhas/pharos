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


#ifndef PHAROS_TM4C129ENCPDT_DECLARATIONS_H
#define	PHAROS_TM4C129ENCPDT_DECLARATIONS_H

#include <pharos/hal/hal.h>

typedef struct PharosCpuRomSysCltTable PharosCpuRomSysCltTable , *ptrPharosCpuRomSysCltTable;

typedef struct PharosCpuSystemControl PharosCpuSystemControl , *ptrPharosCpuSystemControl;

/****************************************************************************
 * Fatal errors definitions for this CPU                                    *
 ****************************************************************************/

/**
 * Invalid interrupt number
 * 
 * the interrupt number is not valid
 */
#define PHAROS_FATAL_CPU_ISR_INV_NUM        (PHAROS_FATAL_CPU_START + 0U)

/**
 * Invalid interrupt priority
 * 
 * the interrupt priority is not valid 
 */
#define PHAROS_FATAL_CPU_ISR_INV_PRI        (PHAROS_FATAL_CPU_START + 1U)

/**
 * PLL could not lock
 * 
 * The main PLL used as a clock source to the CPU could not lock and is not available
 */
#define PHAROS_FATAL_CPU_PLL_NOT_LOCKED        (PHAROS_FATAL_CPU_START + 1U)



/**
 * TM4C129ENCPDT CPU Interrupt Table
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
     *   TM4129ENCPDT specific Interrupt Numbers                *
     ************************************************************/
    PHAROS_CPU_ISR_GPIO_PORTA = 16U ,
    PHAROS_CPU_ISR_GPIO_PORTB = 17U ,
    PHAROS_CPU_ISR_GPIO_PORTC = 18U ,
    PHAROS_CPU_ISR_GPIO_PORTD = 19U ,
    PHAROS_CPU_ISR_GPIO_PORTE = 20U ,
    PHAROS_CPU_ISR_UARTA_RXTX = 21U ,
    PHAROS_CPU_ISR_UARTB_RXTX = 22U ,
    PHAROS_CPU_ISR_SSI0_RXTX = 23U ,
    PHAROS_CPU_ISR_I2C0 = 24U ,
    PHAROS_CPU_ISR_PWM_FAULT = 25U ,
    PHAROS_CPU_ISR_PWM_GEN0 = 26U ,
    PHAROS_CPU_ISR_PWM_GEN1 = 27U ,
    PHAROS_CPU_ISR_PWM_GEN2 = 28U ,
    PHAROS_CPU_ISR_QUAD_ENC0 = 29U ,
    PHAROS_CPU_ISR_ADC_SEQ0 = 30U ,
    PHAROS_CPU_ISR_ADC_SEQ1 = 31U ,
    PHAROS_CPU_ISR_ADC_SEQ2 = 32U ,
    PHAROS_CPU_ISR_ADC_SEQ3 = 33U ,
    PHAROS_CPU_ISR_WATCHDOG = 34U ,
    PHAROS_CPU_ISR_TIMER0A = 35U ,
    PHAROS_CPU_ISR_TIMER0B = 36U ,
    PHAROS_CPU_ISR_TIMER1A = 37U ,
    PHAROS_CPU_ISR_TIMER1B = 38U ,
    PHAROS_CPU_ISR_TIMER2A = 39U ,
    PHAROS_CPU_ISR_TIMER2B = 40U ,
    PHAROS_CPU_ISR_ANALOG0 = 41U ,
    PHAROS_CPU_ISR_ANALOG1 = 42U ,
    PHAROS_CPU_ISR_AMALOG2 = 43U ,
    PHAROS_CPU_ISR_SYSTEM = 44U ,
    PHAROS_CPU_ISR_FLASH_CTRL = 45U ,
    PHAROS_CPU_ISR_GPIO_PORTF = 46U ,
    PHAROS_CPU_ISR_GPIO_PORTG = 47U ,
    PHAROS_CPU_ISR_GPIO_PORTH = 48U ,
    PHAROS_CPU_ISR_UART2_RXTX = 49U ,
    PHAROS_CPU_ISR_SSI1_RXTX = 50U ,
    PHAROS_CPU_ISR_TIMER3A = 51U ,
    PHAROS_CPU_ISR_TIMER3B = 52U ,
    PHAROS_CPU_ISR_I2C1 = 53U ,
    PHAROS_CPU_ISR_CAN0 = 54U ,
    PHAROS_CPU_ISR_CAN1 = 55U ,
    PHAROS_CPU_ISR_ETHERNET = 56U ,
    PHAROS_CPU_ISR_HIBERNATE = 57U ,
    PHAROS_CPU_ISR_USB0 = 58U ,
    PHAROS_CPU_ISR_PWM_GEN3 = 59U ,
    PHAROS_CPU_ISR_UDMA_SWTRA = 60U ,
    PHAROS_CPU_ISR_UDMA_ERROR = 61U ,
    PHAROS_CPU_ISR_ADC1_SEQ0 = 62U ,
    PHAROS_CPU_ISR_ADC1_SEQ1 = 63U ,
    PHAROS_CPU_ISR_ADC1_SEQ2 = 64U ,
    PHAROS_CPU_ISR_ADC1_SEQ3 = 65U ,
    PHAROS_CPU_ISR_EXT_BUS0 = 66U ,
    PHAROS_CPU_ISR_GPIO_PORTJ = 67U ,
    PHAROS_CPU_ISR_GPIO_PORTK = 68U ,
    PHAROS_CPU_ISR_GPIO_PORTL = 69U ,
    PHAROS_CPU_ISR_SSI2_RXTX = 70U ,
    PHAROS_CPU_ISR_SSI3_RXTX = 71U ,
    PHAROS_CPU_ISR_UART3_RXTX = 72U ,
    PHAROS_CPU_ISR_UART4_RXTX = 73U ,
    PHAROS_CPU_ISR_UART5_RXTX = 74U ,
    PHAROS_CPU_ISR_UART6_RXTX = 75U ,
    PHAROS_CPU_ISR_UART7_RXTX = 76U ,
    PHAROS_CPU_ISR_I2C2 = 77U ,
    PHAROS_CPU_ISR_I2C3 = 78U ,
    PHAROS_CPU_ISR_TIMER4A = 79U ,
    PHAROS_CPU_ISR_TIMER4B = 80U ,
    PHAROS_CPU_ISR_TIMER5A = 81U ,
    PHAROS_CPU_ISR_TIMER5B = 82U ,
    PHAROS_CPU_ISR_FPU = 83U ,
    /* 85 and 86 are reserved */
    PHAROS_CPU_ISR_I2C4 = 86U ,
    PHAROS_CPU_ISR_I2C5 = 87U ,
    PHAROS_CPU_ISR_GPIO_PORTM = 88U ,
    PHAROS_CPU_ISR_GPIO_PORTN = 89U ,

    PHAROS_CPU_ISR_TAMPER = 91U ,
    PHAROS_CPU_ISR_GPIO_PORTP = 92U ,
    PHAROS_CPU_ISR_GPIO_PORTP1 = 93U ,
    PHAROS_CPU_ISR_GPIO_PORTP2 = 94U ,
    PHAROS_CPU_ISR_GPIO_PORTP3 = 95U ,
    PHAROS_CPU_ISR_GPIO_PORTP4 = 96U ,
    PHAROS_CPU_ISR_GPIO_PORTP5 = 97U ,
    PHAROS_CPU_ISR_GPIO_PORTP6 = 98U ,
    PHAROS_CPU_ISR_GPIO_PORTP7 = 99U ,
    PHAROS_CPU_ISR_GPIO_PORTQ = 100U ,
    PHAROS_CPU_ISR_GPIO_PORTQ1 = 101U ,
    PHAROS_CPU_ISR_GPIO_PORTQ2 = 102U ,
    PHAROS_CPU_ISR_GPIO_PORTQ3 = 103U ,
    PHAROS_CPU_ISR_GPIO_PORTQ4 = 104U ,
    PHAROS_CPU_ISR_GPIO_PORTQ5 = 105U ,
    PHAROS_CPU_ISR_GPIO_PORTQ6 = 106U ,
    PHAROS_CPU_ISR_GPIO_PORTQ7 = 107U ,
    PHAROS_CPU_ISR_GPIO_PORTR = 108U ,
    PHAROS_CPU_ISR_GPIO_PORTS = 109U ,
    PHAROS_CPU_ISR_SHAMD5 = 110U ,
    PHAROS_CPU_ISR_AES0 = 111U ,
    PHAROS_CPU_ISR_DES3DES = 112U ,
    PHAROS_CPU_ISR_LCD_CTRL = 113U ,
    PHAROS_CPU_ISR_TIMER6A = 114U ,
    PHAROS_CPU_ISR_TIMER6B = 115U ,
    PHAROS_CPU_ISR_TIMER7A = 116U ,
    PHAROS_CPU_ISR_TIMER7B = 117U ,
    PHAROS_CPU_ISR_I2C6 = 118U ,
    PHAROS_CPU_ISR_I2C7 = 119U ,
    PHAROS_CPU_ISR_HIM_SCAN_KEY0 = 120U ,
    PHAROS_CPU_ISR_ONEWIRE0 = 121U ,
    PHAROS_CPU_ISR_HIM_PS2 = 122U ,
    PHAROS_CPU_ISR_HIM_LED_SEQ0 = 123U ,
    PHAROS_CPU_ISR_HIM_CONSUMER_IR0 = 124U ,
    PHAROS_CPU_ISR_I2C8 = 125U ,
    PHAROS_CPU_ISR_I2C9 = 126U ,
    PHAROS_CPU_ISR_GPIO_PORTT = 127U ,
    PHAROS_CPU_ISR_FAN1 = 128U ,

} PharosInterruptNumber;


/* PHAROS_TM4C129ENCPDT_DECLARATIONS_H */
#endif	
