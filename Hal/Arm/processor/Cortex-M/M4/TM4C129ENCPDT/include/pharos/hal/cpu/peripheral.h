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


#ifndef PHAROS_PERIPHERAL_H
#define	PHAROS_PERIPHERAL_H


typedef enum
{
    /**
     * ADC 0
     */
    PHAROS_CPU_SYSCTL_PERIPH_ADC0 = 0xf0003800U ,
    /**
     * ADC 1
     */
    PHAROS_CPU_SYSCTL_PERIPH_ADC1 = 0xf0003801 ,

    PHAROS_CPU_SYSCTL_PERIPH_CAN0 = 0xf0003400 ,

    PHAROS_CPU_SYSCTL_PERIPH_CAN1 = 0xf0003401 ,

    PHAROS_CPU_SYSCTL_PERIPH_COMP0 = 0xf0003c00 ,

    PHAROS_CPU_SYSCTL_PERIPH_EMAC0 = 0xf0009c00 ,

    PHAROS_CPU_SYSCTL_PERIPH_EPHY0 = 0xf0003000 ,

    PHAROS_CPU_SYSCTL_PERIPH_EPI0 = 0xf0001000 ,

    PHAROS_CPU_SYSCTL_PERIPH_GPIOA = 0xf0000800 ,

    PHAROS_CPU_SYSCTL_PERIPH_GPIOB = 0xf0000801 ,

    PHAROS_CPU_SYSCTL_PERIPH_GPIOC = 0xf0000802 ,

    PHAROS_CPU_SYSCTL_PERIPH_GPIOD = 0xf0000803 ,

    PHAROS_CPU_SYSCTL_PERIPH_GPIOE = 0xf0000804 ,

    PHAROS_CPU_SYSCTL_PERIPH_GPIOF = 0xf0000805 ,

    PHAROS_CPU_SYSCTL_PERIPH_GPIOG = 0xf0000806 ,

    PHAROS_CPU_SYSCTL_PERIPH_GPIOH = 0xf0000807 ,

    PHAROS_CPU_SYSCTL_PERIPH_GPIOJ = 0xf0000808 ,

    PHAROS_CPU_SYSCTL_PERIPH_HIBERNATE = 0xf0001400 ,

    PHAROS_CPU_SYSCTL_PERIPH_CCM0 = 0xf0007400 ,

    PHAROS_CPU_SYSCTL_PERIPH_EEPROM0 = 0xf0005800 ,

    PHAROS_CPU_SYSCTL_PERIPH_FAN0 = 0xf0005400 ,

    PHAROS_CPU_SYSCTL_PERIPH_FAN1 = 0xf0005401 ,

    PHAROS_CPU_SYSCTL_PERIPH_GPIOK = 0xf0000809 ,

    PHAROS_CPU_SYSCTL_PERIPH_GPIOL = 0xf000080a ,

    PHAROS_CPU_SYSCTL_PERIPH_GPIOM = 0xf000080b ,

    PHAROS_CPU_SYSCTL_PERIPH_GPION = 0xf000080c ,

    PHAROS_CPU_SYSCTL_PERIPH_GPIOP = 0xf000080d ,

    PHAROS_CPU_SYSCTL_PERIPH_GPIOQ = 0xf000080e ,

    PHAROS_CPU_SYSCTL_PERIPH_GPIOR = 0xf000080f ,

    PHAROS_CPU_SYSCTL_PERIPH_GPIOS = 0xf0000810 ,

    PHAROS_CPU_SYSCTL_PERIPH_GPIOT = 0xf0000811 ,

    PHAROS_CPU_SYSCTL_PERIPH_I2C0 = 0xf0002000 ,

    PHAROS_CPU_SYSCTL_PERIPH_I2C1 = 0xf0002001 ,

    PHAROS_CPU_SYSCTL_PERIPH_I2C2 = 0xf0002002 ,

    PHAROS_CPU_SYSCTL_PERIPH_I2C3 = 0xf0002003 ,

    PHAROS_CPU_SYSCTL_PERIPH_I2C4 = 0xf0002004 ,

    PHAROS_CPU_SYSCTL_PERIPH_I2C5 = 0xf0002005 ,

    PHAROS_CPU_SYSCTL_PERIPH_I2C6 = 0xf0002006 ,

    PHAROS_CPU_SYSCTL_PERIPH_I2C7 = 0xf0002007 ,

    PHAROS_CPU_SYSCTL_PERIPH_I2C8 = 0xf0002008 ,

    PHAROS_CPU_SYSCTL_PERIPH_I2C9 = 0xf0002009 ,

    PHAROS_CPU_SYSCTL_PERIPH_LCD0 = 0xf0009000 ,

    PHAROS_CPU_SYSCTL_PERIPH_ONEWIRE0 = 0xf0009800 ,

    PHAROS_CPU_SYSCTL_PERIPH_PWM0 = 0xf0004000 ,

    PHAROS_CPU_SYSCTL_PERIPH_PWM1 = 0xf0004001 ,

    PHAROS_CPU_SYSCTL_PERIPH_QEI0 = 0xf0004400 ,

    PHAROS_CPU_SYSCTL_PERIPH_QEI1 = 0xf0004401 ,

    PHAROS_CPU_SYSCTL_PERIPH_SSI0 = 0xf0001c00 ,

    PHAROS_CPU_SYSCTL_PERIPH_SSI1 = 0xf0001c01 ,

    PHAROS_CPU_SYSCTL_PERIPH_SSI2 = 0xf0001c02 ,

    PHAROS_CPU_SYSCTL_PERIPH_SSI3 = 0xf0001c03 ,

    PHAROS_CPU_SYSCTL_PERIPH_TIMER0 = 0xf0000400 ,

    PHAROS_CPU_SYSCTL_PERIPH_TIMER1 = 0xf0000401 ,

    PHAROS_CPU_SYSCTL_PERIPH_TIMER2 = 0xf0000402 ,

    PHAROS_CPU_SYSCTL_PERIPH_TIMER3 = 0xf0000403 ,

    PHAROS_CPU_SYSCTL_PERIPH_TIMER4 = 0xf0000404 ,

    PHAROS_CPU_SYSCTL_PERIPH_TIMER5 = 0xf0000405 ,

    PHAROS_CPU_SYSCTL_PERIPH_TIMER6 = 0xf0000406 ,

    PHAROS_CPU_SYSCTL_PERIPH_TIMER7 = 0xf0000407 ,

    PHAROS_CPU_SYSCTL_PERIPH_UART0 = 0xf0001800 ,

    PHAROS_CPU_SYSCTL_PERIPH_UART1 = 0xf0001801 ,

    PHAROS_CPU_SYSCTL_PERIPH_UART2 = 0xf0001802 ,

    PHAROS_CPU_SYSCTL_PERIPH_UART3 = 0xf0001803 ,

    PHAROS_CPU_SYSCTL_PERIPH_UART4 = 0xf0001804 ,

    PHAROS_CPU_SYSCTL_PERIPH_UART5 = 0xf0001805 ,

    PHAROS_CPU_SYSCTL_PERIPH_UART6 = 0xf0001806 ,

    PHAROS_CPU_SYSCTL_PERIPH_UART7 = 0xf0001807 ,

    PHAROS_CPU_SYSCTL_PERIPH_UDMA = 0xf0000c00 ,

    PHAROS_CPU_SYSCTL_PERIPH_USB0 = 0xf0002800 ,

    PHAROS_CPU_SYSCTL_PERIPH_WDOG0 = 0xf0000000 ,

    PHAROS_CPU_SYSCTL_PERIPH_WDOG1 = 0xf0000001 ,

    PHAROS_CPU_SYSCTL_PERIPH_WTIMER0 = 0xf0005c00 ,

    PHAROS_CPU_SYSCTL_PERIPH_WTIMER1 = 0xf0005c01 ,

    PHAROS_CPU_SYSCTL_PERIPH_WTIMER2 = 0xf0005c02 ,

    PHAROS_CPU_SYSCTL_PERIPH_WTIMER3 = 0xf0005c03 ,

    PHAROS_CPU_SYSCTL_PERIPH_WTIMER4 = 0xf0005c04 ,

    PHAROS_CPU_SYSCTL_PERIPH_WTIMER5 = 0xf0005c05 ,

} PharosCpuPeripheralType;


/**
 * Enable the CPU peripherals
 */
void pharosCpuPeripheralEnable(PharosCpuPeripheralType address);


/* PHAROS_PERIPHERAL_H */
#endif	
