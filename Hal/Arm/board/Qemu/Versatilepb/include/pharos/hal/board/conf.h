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


#ifndef PHAROS_VERSATILE_PB_CONF_H
#define PHAROS_VERSATILE_PB_CONF_H


#define PHAROS_VERSATILE_PB_SYSTEM_REGISTERS_ADDRESS                    (0x10000000U)

#define PHAROS_VERSATILE_PB_SERIAL_BUS_ADDRESS                          (0x10002000U)

/**
 * Secondary interrupt controller
 */
#define PHAROS_VERSATILE_PB_SIC_ADDRESS                                 (0x10003000U)


#define PHAROS_VERSATILE_PB_ADVANCED_AUDIO_CODEC_ADDRESS                (0x10004000U)

#define PHAROS_VERSATILE_PB_MULTIMEDIA_CARD_0_ADDRESS                   (0x10005000U)

#define PHAROS_VERSATILE_PB_KEYBOARD_MOUSE_0_ADDRESS                    (0x10006000U)

#define PHAROS_VERSATILE_PB_KEYBOARD_MOUSE_1_ADDRESS                    (0x10007000U)

/**
 * LAN91c111 address
 */
#define PHAROS_VERSATILE_PB_ETHERNET_ADDRESS                            (0x10010000U)

#define PHAROS_VERSATILE_PB_USB_ADDRESS                                 (0x10020000U)

#define PHAROS_VERSATILE_PB_SYNC_STATIC_MEMORY_CTRL_ADDRESS             (0x10100000U)

#define PHAROS_VERSATILE_PB_MULTIPORT_MEMORY_CTRL_ADDRESS               (0x10110000U)

#define PHAROS_VERSATILE_PB_COLOR_LCD_ADDRESS                           (0x10120000U)

#define PHAROS_VERSATILE_PB_DMA_CTRL_ADDRESS                            (0x10130000U)

#define PHAROS_VERSATILE_PB_PIC_ADDRESS                                 (0x10140000U)

#define PHAROS_VERSATILE_PB_SYSTEM_CONTROLLER_ADDRESS                   (0x101E0000U)

#define PHAROS_VERSATILE_PB_WATCHDOG_ADDRESS                            (0x101E1000U)

#define PHAROS_VERSATILE_PB_TIMER0_ADDRESS                              (0x101E2000U)
#define PHAROS_VERSATILE_PB_TIMER1_ADDRESS                              (0x101E2020U)

#define PHAROS_VERSATILE_PB_TIMER2_ADDRESS                              (0x101E3000U)
#define PHAROS_VERSATILE_PB_TIMER3_ADDRESS                              (0x101E3020U)

#define PHAROS_VERSATILE_PB_GPIO0_ADDRESS                               (0x101E4000U)
#define PHAROS_VERSATILE_PB_GPIO1_ADDRESS                               (0x101E5000U)
#define PHAROS_VERSATILE_PB_GPIO2_ADDRESS                               (0x101E6000U)
#define PHAROS_VERSATILE_PB_GPIO3_ADDRESS                               (0x101E7000U)
#define PHAROS_VERSATILE_PB_RTC_ADDRESS                                 (0x101E8000U)
#define PHAROS_VERSATILE_PB_SMART_CARD_ADDRESS                          (0x101F0000U)
#define PHAROS_VERSATILE_PB_UART0_ADDRESS                               (0x101F1000U)
#define PHAROS_VERSATILE_PB_UART1_ADDRESS                               (0x101F2000U)
#define PHAROS_VERSATILE_PB_UART2_ADDRESS                               (0x101F3000U)
#define PHAROS_VERSATILE_PB_SERIAL_SYNC_PORT_ADDRESS                    (0x101F4000U)
#define PHAROS_VERSATILE_PB_MBX_ADDRESS                                 (0x40000000U)

/**
 * Number of interrupts that the Pharos Kernel will have (on its interrupt table pharosVIsrTable)
 */
#define PHAROS_NUMBER_INTERRUPTS                                        (59U)

#endif /* PHAROS_VERSATILE_PB_CONF_H */
