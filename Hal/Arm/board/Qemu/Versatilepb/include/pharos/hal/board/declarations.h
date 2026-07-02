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


#ifndef PHAROS_BOARD_DECLARATIONS_H
#define PHAROS_BOARD_DECLARATIONS_H

typedef enum
{
    PHAROS_INTERRUPT_NUMBER_WATCHDOG = 0U ,
    PHAROS_INTERRUPT_NUMBER_SOFTWARE = 1U ,
    PHAROS_INTERRUPT_NUMBER_COMMS_RX = 2U ,
    PHAROS_INTERRUPT_NUMBER_COMMS_TX = 3U ,
    PHAROS_INTERRUPT_NUMBER_TIMER_01 = 4U ,
    PHAROS_INTERRUPT_NUMBER_TIMER_23 = 5U ,
    PHAROS_INTERRUPT_NUMBER_GPIO0 = 6U ,
    PHAROS_INTERRUPT_NUMBER_GPIO1 = 7U ,
    PHAROS_INTERRUPT_NUMBER_GPIO2 = 8U ,
    PHAROS_INTERRUPT_NUMBER_GPIO3 = 9U ,
    PHAROS_INTERRUPT_NUMBER_RTC = 10U ,
    PHAROS_INTERRUPT_NUMBER_SSP = 11U ,
    PHAROS_INTERRUPT_NUMBER_UART0 = 12U ,
    PHAROS_INTERRUPT_NUMBER_UART1 = 13U ,
    PHAROS_INTERRUPT_NUMBER_UART2 = 14U ,
    PHAROS_INTERRUPT_NUMBER_SCI0 = 15U ,
    PHAROS_INTERRUPT_NUMBER_CLCD = 16U ,
    PHAROS_INTERRUPT_NUMBER_DMA = 17U ,
    PHAROS_INTERRUPT_NUMBER_PWRFAIL = 18U ,
    PHAROS_INTERRUPT_NUMBER_MBX = 19U ,
    PHAROS_INTERRUPT_NUMBER_DISK_ON_CHIP = 21U ,
    PHAROS_INTERRUPT_NUMBER_MCI0A = 22U ,
    PHAROS_INTERRUPT_NUMBER_AACI = 24U ,
    PHAROS_INTERRUPT_NUMBER_ETHERNET = 25U ,
    PHAROS_INTERRUPT_NUMBER_USB = 26U ,
    PHAROS_INTERRUPT_NUMBER_VIC_INT_SOURCE_27 = 27U ,
    PHAROS_INTERRUPT_NUMBER_VIC_INT_SOURCE_28 = 28U ,

    /**
     * Secondary interrupt controller 
     */
    PHAROS_INTERRUPT_NUMBER_SIC = 31U ,
    PHAROS_INTERRUPT_NUMBER_SIC_SOFTWARE = 32U ,
    PHAROS_INTERRUPT_NUMBER_SIC_MMCI0B = 33U ,
    PHAROS_INTERRUPT_NUMBER_SIC_KMI0 = 35U ,
    PHAROS_INTERRUPT_NUMBER_SIC_KMI1 = 36U ,
    PHAROS_INTERRUPT_NUMBER_SIC_TOUCHSCREEN = 40U ,
    PHAROS_INTERRUPT_NUMBER_SIC_KEYPAD = 41U ,
    PHAROS_INTERRUPT_NUMBER_SIC_DISK_ON_CHIP = 53U ,
    PHAROS_INTERRUPT_NUMBER_SIC_MMCI0A = 54U ,
    PHAROS_INTERRUPT_NUMBER_SIC_AACI = 56U ,
    PHAROS_INTERRUPT_NUMBER_SIC_ETHERNET = 57U ,
    PHAROS_INTERRUPT_NUMBER_SIC_USB = 58U ,

} PharosInterruptNumber;

typedef struct ArmInterruptControllerSic ArmInterruptControllerSic , *ptrArmInterruptControllerSic;


#endif /* PHAROS_BOARD_DECLARATIONS_H */
