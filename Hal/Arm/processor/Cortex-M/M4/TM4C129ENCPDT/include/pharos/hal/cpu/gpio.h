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
#define PHAROS_GPIO_H


/**************************************************************************************/
/*                                                                                    */
/* The following values define the bit field for the argument to several of the APIs  */
/*                                                                                    */
/**************************************************************************************/
#define PHAROS_CPU_GPIO_PIN_0              (0x00000001U)
#define PHAROS_CPU_GPIO_PIN_1              (0x00000002U)
#define PHAROS_CPU_GPIO_PIN_2              (0x00000004U)
#define PHAROS_CPU_GPIO_PIN_3              (0x00000008U)
#define PHAROS_CPU_GPIO_PIN_4              (0x00000010U)
#define PHAROS_CPU_GPIO_PIN_5              (0x00000020U)
#define PHAROS_CPU_GPIO_PIN_6              (0x00000040U)
#define PHAROS_CPU_GPIO_PIN_7              (0x00000080U)

/**************************************************************************************/
/*                                                                                    */
/* The following are defines for the GPIO register offsets                            */
/*                                                                                    */
/**************************************************************************************/

/* GPIO Data */
#define PHAROS_CPU_GPIO_O_DATA             (0x00000000U)

/* GPIO Direction */
#define PHAROS_CPU_GPIO_O_DIR              (0x00000400U)

/* GPIO Interrupt Sense */
#define PHAROS_CPU_GPIO_O_IS               (0x00000404U)

/* GPIO Interrupt Both Edges */
#define PHAROS_CPU_GPIO_O_IBE              (0x00000408U)

/* GPIO Interrupt Event */
#define PHAROS_CPU_GPIO_O_IEV              (0x0000040CU)

/* GPIO Interrupt Mask */
#define PHAROS_CPU_GPIO_O_IM               (0x00000410U)

/* GPIO Raw Interrupt Status */
#define PHAROS_CPU_GPIO_O_RIS              (0x00000414U)

/* GPIO Masked Interrupt Status */
#define PHAROS_CPU_GPIO_O_MIS              (0x00000418U)

/* GPIO Interrupt Clear */
#define PHAROS_CPU_GPIO_O_ICR              (0x0000041CU)

/* GPIO Alternate Function Select */
#define PHAROS_CPU_GPIO_O_AFSEL            (0x00000420U)

/* GPIO 2-mA Drive Select */
#define PHAROS_CPU_GPIO_O_DR2R             (0x00000500U)

/* GPIO 4-mA Drive Select */
#define PHAROS_CPU_GPIO_O_DR4R             (0x00000504U)

/* GPIO 8-mA Drive Select */
#define PHAROS_CPU_GPIO_O_DR8R             (0x00000508U)

/* GPIO Open Drain Select */
#define PHAROS_CPU_GPIO_O_ODR              (0x0000050CU)

/* GPIO Pull-Up Select */
#define PHAROS_CPU_GPIO_O_PUR              (0x00000510U)

/* GPIO Pull-Down Select */
#define PHAROS_CPU_GPIO_O_PDR              (0x00000514U)

/* GPIO Slew Rate Control Select */
#define PHAROS_CPU_GPIO_O_SLR              (0x00000518U)

/* GPIO Digital Enable */
#define PHAROS_CPU_GPIO_O_DEN              (0x0000051CU)

/* GPIO Lock */
#define PHAROS_CPU_GPIO_O_LOCK             (0x00000520U)
/* GPIO Commit */
#define PHAROS_CPU_GPIO_O_CR               (0x00000524U)
/* GPIO Analog Mode Select */
#define PHAROS_CPU_GPIO_O_AMSEL            (0x00000528U)
/* GPIO Port Control */
#define PHAROS_CPU_GPIO_O_PCTL             (0x0000052CU)
/* GPIO ADC Control */
#define PHAROS_CPU_GPIO_O_ADCCTL           (0x00000530U)
/* GPIO DMA Control */
#define PHAROS_CPU_GPIO_O_DMACTL           (0x00000534U)
/* GPIO Select Interrupt */
#define PHAROS_CPU_GPIO_O_SI               (0x00000538U)
/* GPIO 12-mA Drive Select */
#define PHAROS_CPU_GPIO_O_DR12R            (0x0000053CU)
/* GPIO Wake Pin Enable */
#define PHAROS_CPU_GPIO_O_WAKEPEN          (0x00000540U)
/* GPIO Wake Level */
#define PHAROS_CPU_GPIO_O_WAKELVL          (0x00000544U)
/* GPIO Wake Status */
#define PHAROS_CPU_GPIO_O_WAKESTAT         (0x00000548U)
/* GPIO Peripheral Property */
#define PHAROS_CPU_GPIO_O_PP               (0x00000FC0U)
/* GPIO Peripheral Configuration */
#define PHAROS_CPU_GPIO_O_PC               (0x00000FC4U)



#define PHAROS_CPU_GPIO_PA0_U0RX           (0x00000001U)
#define PHAROS_CPU_GPIO_PA0_I2C9SCL        (0x00000002U)
#define PHAROS_CPU_GPIO_PA0_T0CCP0         (0x00000003U)
#define PHAROS_CPU_GPIO_PA0_CAN0RX         (0x00000007U)

#define PHAROS_CPU_GPIO_PA1_U0TX           (0x00000401U)
#define PHAROS_CPU_GPIO_PA1_I2C9SDA        (0x00000402U)
#define PHAROS_CPU_GPIO_PA1_T0CCP1         (0x00000403U)
#define PHAROS_CPU_GPIO_PA1_CAN0TX         (0x00000407U)

#define PHAROS_CPU_GPIO_PA2_U4RX           (0x00000801U)
#define PHAROS_CPU_GPIO_PA2_I2C8SCL        (0x00000802U)
#define PHAROS_CPU_GPIO_PA2_T1CCP0         (0x00000803U)
#define PHAROS_CPU_GPIO_PA2_SSI0CLK        (0x0000080FU)

#define PHAROS_CPU_GPIO_PA3_U4TX           (0x00000C01U)
#define PHAROS_CPU_GPIO_PA3_I2C8SDA        (0x00000C02U)
#define PHAROS_CPU_GPIO_PA3_T1CCP1         (0x00000C03U)
#define PHAROS_CPU_GPIO_PA3_SSI0FSS        (0x00000C0FU)

#define PHAROS_CPU_GPIO_PA4_U3RX           (0x00001001U)
#define PHAROS_CPU_GPIO_PA4_T2CCP0         (0x00001003U)
#define PHAROS_CPU_GPIO_PA4_I2C7SCL        (0x00001002U)
#define PHAROS_CPU_GPIO_PA4_SSI0XDAT0      (0x0000100FU)

#define PHAROS_CPU_GPIO_PA5_U3TX           (0x00001401U)
#define PHAROS_CPU_GPIO_PA5_T2CCP1         (0x00001403U)
#define PHAROS_CPU_GPIO_PA5_I2C7SDA        (0x00001402U)
#define PHAROS_CPU_GPIO_PA5_SSI0XDAT1      (0x0000140FU)

#define PHAROS_CPU_GPIO_PA6_U2RX           (0x00001801U)
#define PHAROS_CPU_GPIO_PA6_I2C6SCL        (0x00001802U)
#define PHAROS_CPU_GPIO_PA6_T3CCP0         (0x00001803U)
#define PHAROS_CPU_GPIO_PA6_USB0EPEN       (0x00001805U)
#define PHAROS_CPU_GPIO_PA6_SSI0XDAT2      (0x0000180DU)
#define PHAROS_CPU_GPIO_PA6_EPI0S8         (0x0000180FU)

#define PHAROS_CPU_GPIO_PA7_U2TX           (0x00001C01U)
#define PHAROS_CPU_GPIO_PA7_I2C6SDA        (0x00001C02U)
#define PHAROS_CPU_GPIO_PA7_T3CCP1         (0x00001C03U)
#define PHAROS_CPU_GPIO_PA7_USB0PFLT       (0x00001C05U)
#define PHAROS_CPU_GPIO_PA7_USB0EPEN       (0x00001C0BU)
#define PHAROS_CPU_GPIO_PA7_SSI0XDAT3      (0x00001C0DU)
#define PHAROS_CPU_GPIO_PA7_EPI0S9         (0x00001C0FU)

#define PHAROS_CPU_GPIO_PB0_U1RX           (0x00010001U)
#define PHAROS_CPU_GPIO_PB0_I2C5SCL        (0x00010002U)
#define PHAROS_CPU_GPIO_PB0_CAN1RX         (0x00010007U)
#define PHAROS_CPU_GPIO_PB0_T4CCP0         (0x00010003U)

#define PHAROS_CPU_GPIO_PB1_U1TX           (0x00010401U)
#define PHAROS_CPU_GPIO_PB1_I2C5SDA        (0x00010402U)
#define PHAROS_CPU_GPIO_PB1_CAN1TX         (0x00010407U)
#define PHAROS_CPU_GPIO_PB1_T4CCP1         (0x00010403U)

#define PHAROS_CPU_GPIO_PB2_T5CCP0         (0x00010803U)
#define PHAROS_CPU_GPIO_PB2_I2C0SCL        (0x00010802U)
#define PHAROS_CPU_GPIO_PB2_USB0STP        (0x0001080EU)
#define PHAROS_CPU_GPIO_PB2_EPI0S27        (0x0001080FU)

#define PHAROS_CPU_GPIO_PB3_I2C0SDA        (0x00010C02U)
#define PHAROS_CPU_GPIO_PB3_T5CCP1         (0x00010C03U)
#define PHAROS_CPU_GPIO_PB3_USB0CLK        (0x00010C0EU)
#define PHAROS_CPU_GPIO_PB3_EPI0S28        (0x00010C0FU)

#define PHAROS_CPU_GPIO_PB4_U0CTS          (0x00011001U)
#define PHAROS_CPU_GPIO_PB4_I2C5SCL        (0x00011002U)
#define PHAROS_CPU_GPIO_PB4_SSI1FSS        (0x0001100FU)

#define PHAROS_CPU_GPIO_PB5_U0RTS          (0x00011401U)
#define PHAROS_CPU_GPIO_PB5_I2C5SDA        (0x00011402U)
#define PHAROS_CPU_GPIO_PB5_SSI1CLK        (0x0001140FU)

#define PHAROS_CPU_GPIO_PC0_TCK            (0x00020001U)
#define PHAROS_CPU_GPIO_PC0_SWCLK          (0x00020001U)

#define PHAROS_CPU_GPIO_PC1_TMS            (0x00020401U)
#define PHAROS_CPU_GPIO_PC1_SWDIO          (0x00020401U)

#define PHAROS_CPU_GPIO_PC2_TDI            (0x00020801U)

#define PHAROS_CPU_GPIO_PC3_SWO            (0x00020C01U)
#define PHAROS_CPU_GPIO_PC3_TDO            (0x00020C01U)

#define PHAROS_CPU_GPIO_PC4_U7RX           (0x00021001U)
#define PHAROS_CPU_GPIO_PC4_EPI0S7         (0x0002100FU)

#define PHAROS_CPU_GPIO_PC5_U7TX           (0x00021401U)
#define PHAROS_CPU_GPIO_PC5_RTCCLK         (0x00021407U)
#define PHAROS_CPU_GPIO_PC5_EPI0S6         (0x0002140FU)

#define PHAROS_CPU_GPIO_PC6_U5RX           (0x00021801U)
#define PHAROS_CPU_GPIO_PC6_EPI0S5         (0x0002180FU)

#define PHAROS_CPU_GPIO_PC7_U5TX           (0x00021C01U)
#define PHAROS_CPU_GPIO_PC7_EPI0S4         (0x00021C0FU)

#define PHAROS_CPU_GPIO_PD0_I2C7SCL        (0x00030002U)
#define PHAROS_CPU_GPIO_PD0_T0CCP0         (0x00030003U)
#define PHAROS_CPU_GPIO_PD0_C0O            (0x00030005U)
#define PHAROS_CPU_GPIO_PD0_SSI2XDAT1      (0x0003000FU)

#define PHAROS_CPU_GPIO_PD1_I2C7SDA        (0x00030402U)
#define PHAROS_CPU_GPIO_PD1_T0CCP1         (0x00030403U)
#define PHAROS_CPU_GPIO_PD1_C1O            (0x00030405U)
#define PHAROS_CPU_GPIO_PD1_SSI2XDAT0      (0x0003040FU)

#define PHAROS_CPU_GPIO_PD2_I2C8SCL        (0x00030802U)
#define PHAROS_CPU_GPIO_PD2_T1CCP0         (0x00030803U)
#define PHAROS_CPU_GPIO_PD2_C2O            (0x00030805U)
#define PHAROS_CPU_GPIO_PD2_SSI2FSS        (0x0003080FU)

#define PHAROS_CPU_GPIO_PD3_I2C8SDA        (0x00030C02U)
#define PHAROS_CPU_GPIO_PD3_T1CCP1         (0x00030C03U)
#define PHAROS_CPU_GPIO_PD3_SSI2CLK        (0x00030C0FU)

#define PHAROS_CPU_GPIO_PD4_U2RX           (0x00031001U)
#define PHAROS_CPU_GPIO_PD4_T3CCP0         (0x00031003U)
#define PHAROS_CPU_GPIO_PD4_SSI1XDAT2      (0x0003100FU)

#define PHAROS_CPU_GPIO_PD5_U2TX           (0x00031401U)
#define PHAROS_CPU_GPIO_PD5_T3CCP1         (0x00031403U)
#define PHAROS_CPU_GPIO_PD5_SSI1XDAT3      (0x0003140FU)

#define PHAROS_CPU_GPIO_PD6_U2RTS          (0x00031801U)
#define PHAROS_CPU_GPIO_PD6_T4CCP0         (0x00031803U)
#define PHAROS_CPU_GPIO_PD6_USB0EPEN       (0x00031805U)
#define PHAROS_CPU_GPIO_PD6_SSI2XDAT3      (0x0003180FU)

#define PHAROS_CPU_GPIO_PD7_U2CTS          (0x00031C01U)
#define PHAROS_CPU_GPIO_PD7_T4CCP1         (0x00031C03U)
#define PHAROS_CPU_GPIO_PD7_USB0PFLT       (0x00031C05U)
#define PHAROS_CPU_GPIO_PD7_NMI            (0x00031C08U)
#define PHAROS_CPU_GPIO_PD7_SSI2XDAT2      (0x00031C0FU)

#define PHAROS_CPU_GPIO_PE0_U1RTS          (0x00040001U)

#define PHAROS_CPU_GPIO_PE1_U1DSR          (0x00040401U)

#define PHAROS_CPU_GPIO_PE2_U1DCD          (0x00040801U)

#define PHAROS_CPU_GPIO_PE3_U1DTR          (0x00040C01U)

#define PHAROS_CPU_GPIO_PE4_U1RI           (0x00041001U)
#define PHAROS_CPU_GPIO_PE4_SSI1XDAT0      (0x0004100FU)

#define PHAROS_CPU_GPIO_PE5_SSI1XDAT1      (0x0004140FU)

#define PHAROS_CPU_GPIO_PF0_EN0LED0        (0x00050005U)
#define PHAROS_CPU_GPIO_PF0_M0PWM0         (0x00050006U)
#define PHAROS_CPU_GPIO_PF0_SSI3XDAT1      (0x0005000EU)
#define PHAROS_CPU_GPIO_PF0_TRD2           (0x0005000FU)

#define PHAROS_CPU_GPIO_PF1_EN0LED2        (0x00050405
#define PHAROS_CPU_GPIO_PF1_M0PWM1         (0x00050406U)
#define PHAROS_CPU_GPIO_PF1_SSI3XDAT0      (0x0005040EU)
#define PHAROS_CPU_GPIO_PF1_TRD1           (0x0005040FU)

#define PHAROS_CPU_GPIO_PF2_M0PWM2         (0x00050806U)
#define PHAROS_CPU_GPIO_PF2_SSI3FSS        (0x0005080EU)
#define PHAROS_CPU_GPIO_PF2_TRD0           (0x0005080FU)

#define PHAROS_CPU_GPIO_PF3_M0PWM3         (0x00050C06U)
#define PHAROS_CPU_GPIO_PF3_SSI3CLK        (0x00050C0EU)
#define PHAROS_CPU_GPIO_PF3_TRCLK          (0x00050C0FU)

#define PHAROS_CPU_GPIO_PF4_EN0LED1        (0x00051005U)
#define PHAROS_CPU_GPIO_PF4_M0FAULT0       (0x00051006U)
#define PHAROS_CPU_GPIO_PF4_SSI3XDAT2      (0x0005100EU)
#define PHAROS_CPU_GPIO_PF4_TRD3           (0x0005100FU)

#define PHAROS_CPU_GPIO_PG0_I2C1SCL        (0x00060002U)
#define PHAROS_CPU_GPIO_PG0_EN0PPS         (0x00060005U)
#define PHAROS_CPU_GPIO_PG0_M0PWM4         (0x00060006U)
#define PHAROS_CPU_GPIO_PG0_EPI0S11        (0x0006000FU)

#define PHAROS_CPU_GPIO_PG1_I2C1SDA        (0x00060402U)
#define PHAROS_CPU_GPIO_PG1_M0PWM5         (0x00060406U)
#define PHAROS_CPU_GPIO_PG1_EPI0S10        (0x0006040FU)

#define PHAROS_CPU_GPIO_PH0_U0RTS          (0x00070001U)
#define PHAROS_CPU_GPIO_PH0_EPI0S0         (0x0007000FU)

#define PHAROS_CPU_GPIO_PH1_U0CTS          (0x00070401U)
#define PHAROS_CPU_GPIO_PH1_EPI0S1         (0x0007040FU)

#define PHAROS_CPU_GPIO_PH2_U0DCD          (0x00070801U)
#define PHAROS_CPU_GPIO_PH2_EPI0S2         (0x0007080FU)

#define PHAROS_CPU_GPIO_PH3_U0DSR          (0x00070C01U)
#define PHAROS_CPU_GPIO_PH3_EPI0S3         (0x00070C0FU)

#define PHAROS_CPU_GPIO_PJ0_U3RX           (0x00080001U)
#define PHAROS_CPU_GPIO_PJ0_EN0PPS         (0x00080005U)

#define PHAROS_CPU_GPIO_PJ1_U3TX           (0x00080401U)

#define PHAROS_CPU_GPIO_PK0_U4RX           (0x00090001U)
#define PHAROS_CPU_GPIO_PK0_EPI0S0         (0x0009000FU)

#define PHAROS_CPU_GPIO_PK1_U4TX           (0x00090401U)
#define PHAROS_CPU_GPIO_PK1_EPI0S1         (0x0009040FU)

#define PHAROS_CPU_GPIO_PK2_U4RTS          (0x00090801U)
#define PHAROS_CPU_GPIO_PK2_EPI0S2         (0x0009080FU)

#define PHAROS_CPU_GPIO_PK3_U4CTS          (0x00090C01U)
#define PHAROS_CPU_GPIO_PK3_EPI0S3         (0x00090C0FU)

#define PHAROS_CPU_GPIO_PK4_I2C3SCL        (0x00091002U)
#define PHAROS_CPU_GPIO_PK4_EN0LED0        (0x00091005U)
#define PHAROS_CPU_GPIO_PK4_M0PWM6         (0x00091006U)
#define PHAROS_CPU_GPIO_PK4_EPI0S32        (0x0009100FU)

#define PHAROS_CPU_GPIO_PK5_I2C3SDA        (0x00091402U)
#define PHAROS_CPU_GPIO_PK5_EN0LED2        (0x00091405U)
#define PHAROS_CPU_GPIO_PK5_M0PWM7         (0x00091406U)
#define PHAROS_CPU_GPIO_PK5_EPI0S31        (0x0009140FU)

#define PHAROS_CPU_GPIO_PK6_I2C4SCL        (0x00091802U)
#define PHAROS_CPU_GPIO_PK6_EN0LED1        (0x00091805U)
#define PHAROS_CPU_GPIO_PK6_M0FAULT1       (0x00091806U)
#define PHAROS_CPU_GPIO_PK6_EPI0S25        (0x0009180FU)

#define PHAROS_CPU_GPIO_PK7_U0RI           (0x00091C01U)
#define PHAROS_CPU_GPIO_PK7_I2C4SDA        (0x00091C02U)
#define PHAROS_CPU_GPIO_PK7_RTCCLK         (0x00091C05U)
#define PHAROS_CPU_GPIO_PK7_M0FAULT2       (0x00091C06U)
#define PHAROS_CPU_GPIO_PK7_EPI0S24        (0x00091C0FU)

#define PHAROS_CPU_GPIO_PL0_I2C2SDA        (0x000A0002U)
#define PHAROS_CPU_GPIO_PL0_M0FAULT3       (0x000A0006U)
#define PHAROS_CPU_GPIO_PL0_USB0D0         (0x000A000EU)
#define PHAROS_CPU_GPIO_PL0_EPI0S16        (0x000A000FU)

#define PHAROS_CPU_GPIO_PL1_I2C2SCL        (0x000A0402U)
#define PHAROS_CPU_GPIO_PL1_PHA0           (0x000A0406U)
#define PHAROS_CPU_GPIO_PL1_USB0D1         (0x000A040EU)
#define PHAROS_CPU_GPIO_PL1_EPI0S17        (0x000A040FU)

#define PHAROS_CPU_GPIO_PL2_C0O            (0x000A0805U)
#define PHAROS_CPU_GPIO_PL2_PHB0           (0x000A0806U)
#define PHAROS_CPU_GPIO_PL2_USB0D2         (0x000A080EU)
#define PHAROS_CPU_GPIO_PL2_EPI0S18        (0x000A080FU)

#define PHAROS_CPU_GPIO_PL3_C1O            (0x000A0C05U)
#define PHAROS_CPU_GPIO_PL3_IDX0           (0x000A0C06U)
#define PHAROS_CPU_GPIO_PL3_USB0D3         (0x000A0C0EU)
#define PHAROS_CPU_GPIO_PL3_EPI0S19        (0x000A0C0FU)

#define PHAROS_CPU_GPIO_PL4_T0CCP0         (0x000A1003U)
#define PHAROS_CPU_GPIO_PL4_USB0D4         (0x000A100EU)
#define PHAROS_CPU_GPIO_PL4_EPI0S26        (0x000A100FU)

#define PHAROS_CPU_GPIO_PL5_T0CCP1         (0x000A1403U)
#define PHAROS_CPU_GPIO_PL5_EPI0S33        (0x000A140FU)
#define PHAROS_CPU_GPIO_PL5_USB0D5         (0x000A140EU)

#define PHAROS_CPU_GPIO_PL6_T1CCP0         (0x000A1803U)

#define PHAROS_CPU_GPIO_PL7_T1CCP1         (0x000A1C03U)

#define PHAROS_CPU_GPIO_PM0_T2CCP0         (0x000B0003U)
#define PHAROS_CPU_GPIO_PM0_EPI0S15        (0x000B000FU)

#define PHAROS_CPU_GPIO_PM1_T2CCP1         (0x000B0403U)
#define PHAROS_CPU_GPIO_PM1_EPI0S14        (0x000B040FU)

#define PHAROS_CPU_GPIO_PM2_T3CCP0         (0x000B0803U)
#define PHAROS_CPU_GPIO_PM2_EPI0S13        (0x000B080FU)

#define PHAROS_CPU_GPIO_PM3_T3CCP1         (0x000B0C03U)
#define PHAROS_CPU_GPIO_PM3_EPI0S12        (0x000B0C0FU)

#define PHAROS_CPU_GPIO_PM4_U0CTS          (0x000B1001U)
#define PHAROS_CPU_GPIO_PM4_T4CCP0         (0x000B1003U)

#define PHAROS_CPU_GPIO_PM5_U0DCD          (0x000B1401U)
#define PHAROS_CPU_GPIO_PM5_T4CCP1         (0x000B1403U)

#define PHAROS_CPU_GPIO_PM6_U0DSR          (0x000B1801U)
#define PHAROS_CPU_GPIO_PM6_T5CCP0         (0x000B1803U)

#define PHAROS_CPU_GPIO_PM7_U0RI           (0x000B1C01U)
#define PHAROS_CPU_GPIO_PM7_T5CCP1         (0x000B1C03U)

#define PHAROS_CPU_GPIO_PN0_U1RTS          (0x000C0001U)

#define PHAROS_CPU_GPIO_PN1_U1CTS          (0x000C0401U)

#define PHAROS_CPU_GPIO_PN2_U1DCD          (0x000C0801U)
#define PHAROS_CPU_GPIO_PN2_U2RTS          (0x000C0802U)
#define PHAROS_CPU_GPIO_PN2_EPI0S29        (0x000C080FU)

#define PHAROS_CPU_GPIO_PN3_U1DSR          (0x000C0C01U)
#define PHAROS_CPU_GPIO_PN3_U2CTS          (0x000C0C02U)
#define PHAROS_CPU_GPIO_PN3_EPI0S30        (0x000C0C0FU)

#define PHAROS_CPU_GPIO_PN4_U1DTR          (0x000C1001U)
#define PHAROS_CPU_GPIO_PN4_U3RTS          (0x000C1002U)
#define PHAROS_CPU_GPIO_PN4_I2C2SDA        (0x000C1003U)
#define PHAROS_CPU_GPIO_PN4_EPI0S34        (0x000C100FU)

#define PHAROS_CPU_GPIO_PN5_U1RI           (0x000C1401U)
#define PHAROS_CPU_GPIO_PN5_U3CTS          (0x000C1402U)
#define PHAROS_CPU_GPIO_PN5_I2C2SCL        (0x000C1403U)
#define PHAROS_CPU_GPIO_PN5_EPI0S35        (0x000C140FU)

#define PHAROS_CPU_GPIO_PP0_U6RX           (0x000D0001U)
#define PHAROS_CPU_GPIO_PP0_SSI3XDAT2      (0x000D000FU)

#define PHAROS_CPU_GPIO_PP1_U6TX           (0x000D0401U)
#define PHAROS_CPU_GPIO_PP1_SSI3XDAT3      (0x000D040FU)

#define PHAROS_CPU_GPIO_PP2_U0DTR          (0x000D0801U)
#define PHAROS_CPU_GPIO_PP2_USB0NXT        (0x000D080EU)
#define PHAROS_CPU_GPIO_PP2_EPI0S29        (0x000D080FU)

#define PHAROS_CPU_GPIO_PP3_U1CTS          (0x000D0C01U)
#define PHAROS_CPU_GPIO_PP3_U0DCD          (0x000D0C02U)
#define PHAROS_CPU_GPIO_PP3_RTCCLK         (0x000D0C07U)
#define PHAROS_CPU_GPIO_PP3_USB0DIR        (0x000D0C0EU)
#define PHAROS_CPU_GPIO_PP3_EPI0S30        (0x000D0C0FU)

#define PHAROS_CPU_GPIO_PP4_U3RTS          (0x000D1001U)
#define PHAROS_CPU_GPIO_PP4_U0DSR          (0x000D1002U)
#define PHAROS_CPU_GPIO_PP4_USB0D7         (0x000D100EU)

#define PHAROS_CPU_GPIO_PP5_U3CTS          (0x000D1401U)
#define PHAROS_CPU_GPIO_PP5_I2C2SCL        (0x000D1402U)
#define PHAROS_CPU_GPIO_PP5_USB0D6         (0x000D140EU)

#define PHAROS_CPU_GPIO_PQ0_SSI3CLK        (0x000E000EU)
#define PHAROS_CPU_GPIO_PQ0_EPI0S20        (0x000E000FU)

#define PHAROS_CPU_GPIO_PQ1_SSI3FSS        (0x000E040EU)
#define PHAROS_CPU_GPIO_PQ1_EPI0S21        (0x000E040FU)

#define PHAROS_CPU_GPIO_PQ2_SSI3XDAT0      (0x000E080EU)
#define PHAROS_CPU_GPIO_PQ2_EPI0S22        (0x000E080FU)

#define PHAROS_CPU_GPIO_PQ3_SSI3XDAT1      (0x000E0C0EU)
#define PHAROS_CPU_GPIO_PQ3_EPI0S23        (0x000E0C0FU)

#define PHAROS_CPU_GPIO_PQ4_U1RX           (0x000E1001U)
#define PHAROS_CPU_GPIO_PQ4_DIVSCLK        (0x000E1007U)


/**
 * Configures the GPIO pins
 * 
 * Configure the pin mux that selecs the function of the peripheral associated with the pin.
 * 
 * @param pinConfiguration
 */
void pharosCpuGpioPinConfigure(uint32_t pinConfiguration);

/**
 * Configures pin(s) for use by the UART peripheral
 * 
 * @param port base address of the GPIO port
 * @param pins bit-packed representation of the pin(s)
 */
void pharosCpuGpioPinTypeUart(uint32_t port , uint8_t pins);

void pharosCpuGpioPinTypeOutput(uint32_t ui32Port , uint8_t ui8Pins);

/* PHAROS_GPIO_H */
#endif 
