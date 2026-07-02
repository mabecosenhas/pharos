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


#ifndef PHAROS_CPU_H
#define	PHAROS_CPU_H

#include <pharos/hal/v7m/declarations.h>
#include <pharos/hal/v7m/v7m.h>
#include <pharos/hal/arm/declarations.h>
#include <pharos/hal/v7m/isr.h>
#include <pharos/hal/cpu/context.h>
#include <pharos/hal/thumb.h>
#include <pharos/hal/memory/mpuV7.h>
#include <pharos/interrupt.h>


#define PHAROS_CPU_SYSTEM_CONTROL                   ((ptrPharosCpuSystemControl) 0x400FE000U)


/************************************************************
 * MOSCCTL register definitions                             *
 ************************************************************/

/* Oscillator Range */
#define PHAROS_CPU_SYSCTL_MOSCCTL_OSCRNG            (0x00000010)

/* Power Down */
#define PHAROS_CPU_SYSCTL_MOSCCTL_PWRDN             (0x00000008)  

/* No Crystal Connected */
#define PHAROS_CPU_SYSCTL_MOSCCTL_NOXTAL            (0x00000004)  


/************************************************************
 * PLLFREQ0 register definitions                            *
 ************************************************************/

#define PHAROS_CPU_SYSCTL_PLLFREQ0_PWR              (0x00800000U)

/************************************************************
 * PLLSTAT register definitions                             *
 ************************************************************/

#define PHAROS_CPU_SYSCTL_PLLSTAT_LOCK              (0x00000001U)  

/************************************************************
 * RSCLKCFG register definitions                            *
 ************************************************************/

/* memory timing register update */
#define PHAROS_CPU_SYSCTL_RSCLKCFG_MEMTIMU          (0x80000000)  

/* use PLL as source */
#define PHAROS_CPU_SYSCTL_RSCLKCFG_USEPLL           (0x10000000)  

/* use MOSC as the PLL source */
#define PHAROS_CPU_SYSCTL_RSCLKCFG_PLLSRC_MOSC      (0x03000000)  

/* PLL source mask */
#define PHAROS_CPU_SYSCTL_RSCLKCFG_PLLSRC_M         (0x0F000000)  

/* select the MOSC as the source */
#define PHAROS_CPU_SYSCTL_RSCLKCFG_OSCSRC_MOSC      (0x00300000)  

/* oscillator source mask */
#define PHAROS_CPU_SYSCTL_RSCLKCFG_OSCSRC_M         (0x00F00000)  

/* System clock divisor mask */
#define PHAROS_CPU_SYSCTL_RSCLKCFG_PSYSDIV_MASK     (0x000003FF)  

/* offset bit of the PLL System clock divisor */
#define PHAROS_CPU_SYSCTL_RSCLKCFG_PSYSDIV_OFFSET   (0U)

/************************************************************
 * MEMTIM0 register definitions                             *
 ************************************************************/

/* 1.5 system clock periods */
#define PHAROS_CPU_SYSCTL_MEMTIM0_EBCHT_1_5         (0x00800000U) 

/* 3.5 system clock periods */
#define PHAROS_CPU_SYSCTL_MEMTIM0_EBCHT_3_5         (0x01800000U)  

/* 1.5 system clock periods */
#define PHAROS_CPU_SYSCTL_MEMTIM0_FBCHT_1_5         (0x00000080U)

/* 3.5 system clock periods */
#define PHAROS_CPU_SYSCTL_MEMTIM0_FBCHT_3_5         (0x00000180U)

#define PHAROS_CPU_SYSCTL_MEMTIM0_EWS_S             (16U)
#define PHAROS_CPU_SYSCTL_MEMTIM0_FWS_S             (0U)

#define PHAROS_CPU_SYSCTL_MEMTIM0_MB1               (0x00100010U)


struct CpuCoreConfiguration
{
};


/**
 * Compact the size of the I/O registers
 */
#pragma pack(push ,4)


struct PharosCpuSystemControl
{
    /**
     * Device Identification 0 (DID0)
     * 
     * offset: 0x000
     */
    volatile uint32_t deviceIdentification0;

    /**
     * Device Identification 0 (DID0)
     * 
     * offset: 0x004
     */
    volatile uint32_t deviceIdentification1;

    /**
     * Reserved
     */
    volatile uint32_t reserved1[12];

    /**
     * Power-Temp Brown Out Control (PTBOCTL)
     * 
     * offset: 0x038
     */
    volatile uint32_t powerTempBrownOutControl;

    /**
     * Reserved
     */
    volatile uint32_t reserved2[5];

    /**
     * Raw Interrupt Status (RIS)
     * 
     * offset: 0x050
     */
    volatile uint32_t rawInterruptStatus;

    /**
     * Interrupt Mask Control (IMC)
     * 
     * offset: 0x054
     */
    volatile uint32_t interruptMaskControl;

    /**
     * Masked Interrupt Status and Clear (MISC)
     * 
     * offset: 0x058
     */
    volatile uint32_t maskedInterruptStatusClear;

    /**
     * Reset Cause (RESC)
     * 
     * offset: 0x05C
     */
    volatile uint32_t resetCause;

    /**
     * Power-Temperature Cause (PWRTC)
     * 
     * offset: 0x060
     */
    volatile uint32_t powerTemperatureCause;

    /**
     * NMI Cause Register (NMIC)
     * 
     * offset: 0x064
     */
    volatile uint32_t nmiCause;

    /**
     * Reserved
     */
    volatile uint32_t reserved3[5];

    /**
     * Main Oscillator Control (MOSCCTL)
     * 
     * offset: 0x07C
     */
    volatile uint32_t mainOscillatorControl;

    /**
     * Reserved
     */
    volatile uint32_t reserved4[12];

    /**
     * Run and Sleep Mode Configuration Register (RSCLKCFG)
     * 
     * offset: 0x0b0
     */
    volatile uint32_t runSleepModeConf;

    /**
     * Reserved
     */
    volatile uint32_t reserved5[3];

    /**
     * Memory Timing Parameter Register 0 for Main Flash and EEPROM (MEMTIM0)
     * 
     * offset: 0x0c0
     */
    volatile uint32_t memTimParFlashEeprom0;

    /**
     * Reserved
     */
    volatile uint32_t reserved6[29];

    /**
     * Alternate Clock Configuration (ALTCLKCFG)
     * 
     * offset: 0x138
     */
    volatile uint32_t alternateClockConf;

    /**
     * Reserved
     */
    volatile uint32_t reserved7[2];

    /**
     * Deep Sleep Clock Configuration Register (DSCLKCFG)
     * 
     * offset: 0x144
     */
    volatile uint32_t deepSleepClkConf;

    /**
     * Divisor and Source Clock Configuration (DIVSCLK)
     * 
     * offset: 0x148
     */
    volatile uint32_t divisorSourceClockConf;

    /**
     * System Properties (SYSPROP)
     * 
     * offset: 0x14C
     */
    volatile uint32_t systemProperties;

    /**
     * Precision Internal Oscillator Calibration (PIOSCCAL)
     * 
     * offset: 0x150
     */
    volatile uint32_t precisionInternalOscCal;

    /**
     * Precision Internal Oscillator Statistics (PIOSCSTAT)
     * 
     * offset: 0x154
     */
    volatile uint32_t precisionInternalOscStat;

    /**
     * Reserved
     */
    volatile uint32_t reserved8[2];

    /**
     * PLL Frequency 0 (PLLFREQ0)
     * 
     * offset: 0x160
     */
    volatile uint32_t pllFreq0;

    /**
     * PLL Frequency 1 (PLLFREQ1)
     * 
     * offset: 0x164
     */
    volatile uint32_t pllFreq1;

    /**
     * PLL Status (PLLSTAT)
     * 
     * offset: 0x168
     */
    volatile uint32_t pllStat;

    /**
     * Reserved
     */
    volatile uint32_t reserved9[7];

    /**
     * Sleep Power Configuration (SLPPWRCFG)
     * 
     * offset: 0x188
     */
    volatile uint32_t sleepPowerConf;

    /**
     * Deep-Sleep Power Configuration (DSLPPWRCFG)
     * 
     * offset: 0x18C
     */
    volatile uint32_t deepSleepPowerConf;

    /**
     * Reserved
     */
    volatile uint32_t reserved10[4];

    /**
     * Non-Volatile Memory Information (NVMSTAT)
     * 
     * offset: 0x1A0
     */
    volatile uint32_t nonVolatileMemInfo;

    /**
     * Reserved
     */
    volatile uint32_t reserved11[4];

    /**
     * LDO Sleep Power Control (LDOSPCTL)
     * 
     * offset: 0x1B4
     */
    volatile uint32_t ldoSleepPowerCtrl;

    /**
     * Reserved
     */
    volatile uint32_t reserved12;

    /**
     * LDO Deep-Sleep Power Control (LDODPCTL)
     * 
     * offset: 0x1BC
     */
    volatile uint32_t ldoDeepSleepPowerCtrl;

    /**
     * LDO Deep-Sleep Power Calibration (LDODPCAL)
     * 
     * offset: 0x1C0
     */
    volatile uint32_t ldoDeepSleepPowerCalibration;

    /**
     * Reserved
     */
    volatile uint32_t reserved14[2];

    /**
     * Sleep / Deep-Sleep Power Mode Status (SDPMST)
     * 
     * offset: 0x1CC
     */
    volatile uint32_t sleepDeepPowerModeStatus;

    /**
     * Reserved
     */
    volatile uint32_t reserved15[2];

    /**
     * Reset Behavior Control Register (RESBEHAVCTL)
     * 
     * offset: 0x1D8
     */
    volatile uint32_t resetBehaviourControl;

    /**
     * Reserved
     */
    volatile uint32_t reserved16[6];

    /**
     * Hardware System Service Request (HSSR)
     * 
     * offset: 0x1F4
     */
    volatile uint32_t hwSystemServiceRequest;

    /**
     * Reserved
     */
    volatile uint32_t reserved17[3];

    /**
     * Cryptographic Modules Clock Gating Request (CCMCGREQ)
     * 
     * offset: 0x204
     */
    volatile uint32_t cryptClkGatRequest;

    /**
     * Reserved
     */
    volatile uint32_t reserved18[30];

    /**
     * USB Power Domain Status (USBPDS)
     * 
     * offset: 0x280
     */
    volatile uint32_t usbPowerDomainStatus;

    /**
     * USB Memory Power Control (USBMPC)
     * 
     * offset: 0x284
     */
    volatile uint32_t usbMemPowerCtrl;

    /**
     * Ethernet MAC Power Domain Status (EMACPDS)
     * 
     * offset: 0x288
     */
    volatile uint32_t ethernetMacPowerDomainStatus;

    /**
     * Ethernet MAC Memory Power Control (EMACMPC)
     * 
     * offset: 0x28C
     */
    volatile uint32_t ethernetMacMemPowerCtrl;

    /**
     * Reserved
     */
    volatile uint32_t reserved19[2];

    /**
     * CAN 0 Power Domain Status (CAN0PDS)
     * 
     * offset: 0x298
     */
    volatile uint32_t can0PowerDomainStatus;

    /**
     * CAN 0 Memory Power Control (CAN0MPC)
     * 
     * offset: 0x29C
     */
    volatile uint32_t canMemPowerCtrl;

    /**
     * CAN 1 Power Domain Status (CAN1PDS)
     * 
     * offset: 0x2A0
     */
    volatile uint32_t can1PowerDomainStatus;

    /**
     * CAN 1 Memory Power Control (CAN1MPC)
     * 
     * offset: 0x2A4
     */
    volatile uint32_t can1MemPowerCtrl;

    /**
     * Reserved
     */
    volatile uint32_t reserved20[22];

    /**
     * Watchdog Timer Peripheral Present (PPWD)
     * 
     * offset: 0x300
     */
    volatile uint32_t watchdogPerPresent;

    /**
     * 16/32-Bit General-Purpose Timer Peripheral Present (PPTIMER)
     * 
     * offset: 0x304
     */
    volatile uint32_t gptPerPresent;

    /**
     * General-Purpose Input/Output Peripheral Present (PPGPIO)
     * 
     * offset: 0x308
     */
    volatile uint32_t gpioPerPresent;

    /**
     * Micro Direct Memory Access Peripheral Present (PPDMA)
     * 
     * offset: 0x30C
     */
    volatile uint32_t microDmaPerPresent;

    /**
     * EPI Peripheral Present (PPEPI)
     * 
     * offset: 0x310
     */
    volatile uint32_t epiPerPresent;

    /**
     * Hibernation Peripheral Present (PPHIB)
     * 
     * offset: 0x314
     */
    volatile uint32_t hibernationPerPresent;

    /**
     * Universal Asynchronous Receiver/Transmitter Peripheral Present (PPUART)
     * 
     * offset: 0x318
     */
    volatile uint32_t uartPerPresent;

    /**
     * Synchronous Serial Interface Peripheral Present (PPSSI)
     * 
     * offset: 0x31C
     */
    volatile uint32_t ssiPerPresent;

    /**
     * Inter-Integrated Circuit Peripheral Present (PPI2C)
     * 
     * offset: 0x320
     */
    volatile uint32_t interIntegPerPresent;

    /**
     * Reserved
     */
    volatile uint32_t reserved21;

    /**
     * Universal Serial Bus Peripheral Present (PPUSB)
     * 
     * offset: 0x328
     */
    volatile uint32_t usbPerPresent;

    /**
     * Reserved
     */
    volatile uint32_t reserved22;

    /**
     * Ethernet PHY Peripheral Present (PPEPHY)
     * 
     * offset: 0x330
     */
    volatile uint32_t ethernetPhyPerPresent;

    /**
     * Controller Area Network Peripheral Present (PPCAN)
     * 
     * offset: 0x334
     */
    volatile uint32_t canPerPresent;

    /**
     * Analog-to-Digital Converter Peripheral Present (PPADC)
     * 
     * offset: 0x338
     */
    volatile uint32_t a2dPerPresent;

    /**
     * Analog Comparator Peripheral Present (PPACMP)
     * 
     * offset: 0x33C
     */
    volatile uint32_t analogCompPerPresent;

    /**
     * Pulse Width Modulator Peripheral Present (PPPWM)
     * 
     * offset: 0x340
     */
    volatile uint32_t pwmPerPresent;

    /**
     * Quadrature Encoder Interface Peripheral Present (PPQEI)
     * 
     * offset: 0x344
     */
    volatile uint32_t quadEncPerPresent;

    /**
     * Low Pin Count Interface Peripheral Present (PPLPC)
     * 
     * offset: 0x348
     */
    volatile uint32_t lowPinCountPerPresent;

    /**
     * Reserved
     */
    volatile uint32_t reserved23;

    /**
     * Platform Environment Control Interface Peripheral Present (PPPECI)
     * 
     * offset: 0x350
     */
    volatile uint32_t platEnvControlPerPresent;

    /**
     * Fan Control Peripheral Present (PPFAN)
     * 
     * offset: 0x354
     */
    volatile uint32_t fanControlPerPresent;

    /**
     * EEPROM Peripheral Present (PPEEPROM)
     * 
     * offset: 0x358
     */
    volatile uint32_t eepromPerPresent;

    /**
     * 32/64-Bit Wide General-Purpose Timer Peripheral Present (PPWTIMER)
     * 
     * offset: 0x35C
     */
    volatile uint32_t gpt3264PerPresent;

    /**
     * Reserved
     */
    volatile uint32_t reserved24[4];

    /**
     * Remote Temperature Sensor Peripheral Present (PPRTS)
     * 
     * offset: 0x370
     */
    volatile uint32_t remoteTempSensorPerPresent;

    /**
     * CRC and Cryptographic Modules Peripheral Present (PPCCM)
     * 
     * offset: 0x374
     */
    volatile uint32_t crcPerPresent;

    /**
     * Reserved
     */
    volatile uint32_t reserved25[6];

    /**
     * LCD Peripheral Present (PPLCD)
     * 
     * offset: 0x390
     */
    volatile uint32_t lcdPerPresent;

    /**
     * Reserved
     */
    volatile uint32_t reserved26;

    /**
     * 1-Wire Peripheral Present (PPOWIRE)
     * 
     * offset: 0x398
     */
    volatile uint32_t oneWirePerPresent;

    /**
     * Ethernet MAC Peripheral Present (PPEMAC)
     * 
     * offset: 0x39C
     */
    volatile uint32_t ethernetMacPerPresent;

    /**
     * Power Regulator Bus Peripheral Present (PPPRB)
     * 
     * offset: 0x3A0
     */
    volatile uint32_t powerRegBusPerPresent;

    /**
     * Human Interface Master Peripheral Present (PPHIM)
     * 
     * offset: 0x3A4
     */
    volatile uint32_t hmiMasterPerPresent;

    /**
     * Reserved
     */
    volatile uint32_t reserved27[86];

    /**
     * Watchdog Timer Software Reset (SRWD)
     * 
     * offset: 0x500
     */
    volatile uint32_t watchdogSwReset;

    /**
     * 16/32-Bit General-Purpose Timer Software Reset (SRTIMER)
     * 
     * offset: 0x504
     */
    volatile uint32_t gptSwReset;

    /**
     * General-Purpose Input/Output Software Reset (SRGPIO)
     * 
     * offset: 0x508
     */
    volatile uint32_t gpioSwReset;

    /**
     * Micro Direct Memory Access Software Reset (SRDMA)
     * 
     * offset: 0x50C
     */
    volatile uint32_t microDmaSwReset;

    /**
     * EPI Software Reset (SREPI)
     * 
     * offset: 0x510
     */
    volatile uint32_t epiSwReset;

    /**
     * Hibernation Software Reset (SRHIB)
     * 
     * offset: 0x514
     */
    volatile uint32_t hibernationSwReset;

    /**
     * Universal Asynchronous Receiver/Transmitter Software Reset (SRUART)
     * 
     * offset: 0x518
     */
    volatile uint32_t uartSwReset;

    /**
     * Synchronous Serial Interface Software Reset (SRSSI)
     * 
     * offset: 0x51C
     */
    volatile uint32_t ssiSwReset;

    /**
     * Inter-Integrated Circuit Software Reset (SRI2C)
     * 
     * offset: 0x520
     */
    volatile uint32_t interIntegSwReset;

    /**
     * Reserved
     */
    volatile uint32_t reserved28;

    /**
     * Universal Serial Bus Software Reset (SRUSB)
     * 
     * offset: 0x528
     */
    volatile uint32_t usbSwReset;

    /**
     * Reserved
     */
    volatile uint32_t reserved29;

    /**
     * Ethernet PHY Software Reset (SREPHY)
     * 
     * offset: 0x530
     */
    volatile uint32_t ethernetPhySwReset;

    /**
     * Controller Area Network Software Reset (SRCAN)
     * 
     * offset: 0x534
     */
    volatile uint32_t canSwReset;

    /**
     * Analog-to-Digital Converter Software Reset (SRADC)
     * 
     * offset: 0x538
     */
    volatile uint32_t a2dSwReset;

    /**
     * Analog Comparator Software Reset (SRACMP)
     * 
     * offset: 0x53C
     */
    volatile uint32_t analogCompSwReset;

    /**
     * Pulse Width Modulator Software Reset (SRPWM)
     * 
     * offset: 0x540
     */
    volatile uint32_t pwmSwReset;

    /**
     * Quadrature Encoder Interface Software Reset (SRQEI)
     * 
     * offset: 0x544
     */
    volatile uint32_t quadEncSwReset;

    /**
     * Reserved
     */
    volatile uint32_t reserved30[4];

    /**
     * EEPROM Software Reset (SREEPROM)
     * 
     * offset: 0x558
     */
    volatile uint32_t eepromSwReset;

    /**
     * Reserved
     */
    volatile uint32_t reserved31[6];

    /**
     * CRC and Cryptographic Modules Software Reset (SRCCM)
     * 
     * offset: 0x574
     */
    volatile uint32_t crcSwReset;

    /**
     * Reserved
     */
    volatile uint32_t reserved32[9];

    /**
     * Ethernet MAC Software Reset (SREMAC)
     * 
     * offset: 0x59C
     */
    volatile uint32_t ethernetMacSwReset;

    /**
     * Reserved
     */
    volatile uint32_t reserved13[24];

    /**
     * Watchdog Timer Run Mode Clock Gating Control (RCGCWD)
     * 
     * offset: 0x600
     */
    volatile uint32_t watchdogRunClkGatCtrl;

    /**
     * 16/32-Bit General-Purpose Timer Run Mode Clock Gating Control (RCGCTIMER)
     * 
     * offset: 0x604
     */
    volatile uint32_t gptRunClkGatCtrl;

    /**
     * General-Purpose Input/Output Run Mode Clock Gating Control (RCGCGPIO)
     * 
     * offset: 0x608
     */
    volatile uint32_t gpioRunClkGatCtrl;

    /**
     * Micro Direct Memory Access Run Mode Clock Gating Control (RCGCDMA)
     * 
     * offset: 0x60c
     */
    volatile uint32_t microDmaRunClkGatCtrl;

    /**
     * EPI Run Mode Clock Gating Control (RCGCEPI)
     * 
     * offset: 0x610
     */
    volatile uint32_t epiRunClkGatCtrl;

    /**
     * Hibernation Run Mode Clock Gating Control (RCGCHIB)
     * 
     * offset: 0x614
     */
    volatile uint32_t hibernationRunClkGatCtrl;

    /**
     * Universal Asynchronous Receiver/Transmitter Run Mode Clock Gating Control (RCGCUART)
     * 
     * offset: 0x618
     */
    volatile uint32_t uartRunClkGatCtrl;

    /**
     * Synchronous Serial Interface Run Mode Clock Gating Control (RCGCSSI)
     * 
     * offset: 0x61C
     */
    volatile uint32_t ssiRunClkGatCtrl;

    /**
     * Inter-Integrated Circuit Run Mode Clock Gating Control (RCGCI2C)
     * 
     * offset: 0x620
     */
    volatile uint32_t interIntegRunClkGatCtrl;

    /**
     * Reserved
     */
    volatile uint32_t reserved33;

    /**
     * Universal Serial Bus Run Mode Clock Gating Control (RCGCUSB)
     * 
     * offset: 0x628
     */
    volatile uint32_t usbRunClkGatCtrl;

    /**
     * Reserved
     */
    volatile uint32_t reserved34;

    /**
     * Ethernet PHY Run Mode Clock Gating Control (RCGCEPHY)
     * 
     * offset: 0x630
     */
    volatile uint32_t ethernetPhyRunClkGatCtrl;

    /**
     * Controller Area Network Run Mode Clock Gating Control (RCGCCAN)
     * 
     * offset: 0x634
     */
    volatile uint32_t canRunClkGatCtrl;

    /**
     * Analog-to-Digital Converter Run Mode Clock Gating Control (RCGCADC)
     * 
     * offset: 0x638
     */
    volatile uint32_t a2dRunClkGatCtrl;

    /**
     * Analog Comparator Run Mode Clock Gating Control (RCGCACMP)
     * 
     * offset: 0x63C
     */
    volatile uint32_t analCompRunClkGatCtrl;

    /**
     * Pulse Width Modulator Run Mode Clock Gating Control (RCGCPWM)
     * 
     * offset: 0x640
     */
    volatile uint32_t pwmRunClkGatCtrl;

    /**
     * Quadrature Encoder Interface Run Mode Clock Gating Control (RCGCQEI)
     * 
     * offset: 0x644
     */
    volatile uint32_t quadEncRunClkGatCtrl;

    /**
     * Reserved
     */
    volatile uint32_t reserved35[4];

    /**
     * EEPROM Run Mode Clock Gating Control (RCGCEEPROM)
     * 
     * offset: 0x658
     */
    volatile uint32_t eepromRunClkGatCtrl;

    /**
     * Reserved
     */
    volatile uint32_t reserved36[6];

    /**
     * CRC and Cryptographic Modules Run Mode Clock Gating Control (RCGCCCM)
     * 
     * offset: 0x674
     */
    volatile uint32_t crcRunClkGatCtrl;

    /**
     * Reserved
     */
    volatile uint32_t reserved37[9];

    /**
     * Ethernet MAC Run Mode Clock Gating Control (RCGCEMAC)
     * 
     * offset: 0x69C
     */
    volatile uint32_t ethernetMacRunClkGatCtrl;

    /**
     * Reserved
     */
    volatile uint32_t reserved60[24];

    /**
     * Watchdog Timer Sleep Mode Clock Gating Control (SCGCWD)
     * 
     * offset: 0x700
     */
    volatile uint32_t watchdogSleepClkGatCtrl;

    /**
     * 16/32-Bit General-Purpose Timer Sleep Mode Clock Gating Control (SCGCTIMER)
     * 
     * offset: 0x704
     */
    volatile uint32_t gptSleepClkGatCtrl;

    /**
     * General-Purpose Input/Output Sleep Mode Clock Gating (SCGCGPIO)
     * 
     * offset: 0x708
     */
    volatile uint32_t gpioSleepClkGatCtrl;

    /**
     * Micro Direct Memory Access Sleep Mode Clock Gating Control (SCGCDMA)
     * 
     * offset: 0x70C
     */
    volatile uint32_t microDmaSleepClkGatCtrl;

    /**
     * EPI Sleep Mode Clock Gating Control (SCGCEPI)
     * 
     * offset: 0x710
     */
    volatile uint32_t epiSleepClkGatCtrl;

    /**
     * Hibernation Sleep Mode Clock Gating Control (SCGCHIB)
     * 
     * offset: 0x714
     */
    volatile uint32_t hibernationSleepClkGatCtrl;

    /**
     * Universal Asynchronous Receiver/Transmitter Sleep Mode Clock Gating Control (SCGCUART)
     * 
     * offset: 0x718
     */
    volatile uint32_t uartSleepClkGatCtrl;

    /**
     * Synchronous Serial Interface Sleep Mode Clock Gating Control (SCGCSSI)
     * 
     * offset: 0x71C
     */
    volatile uint32_t ssiSleepClkGatCtrl;

    /**
     * Inter-Integrated Circuit Sleep Mode Clock Gating Control (SCGCI2C)
     * 
     * offset: 0x720
     */
    volatile uint32_t interIntegSleepClkGatCtrl;

    /**
     * Reserved
     */
    volatile uint32_t reserved38;

    /**
     * Universal Serial Bus Sleep Mode Clock Gating Control (SCGCUSB)
     * 
     * offset: 0x728
     */
    volatile uint32_t usbSleepClkGatCtrl;

    /**
     * Reserved
     */
    volatile uint32_t reserved39;

    /**
     * Ethernet PHY Sleep Mode Clock Gating Control (SCGCEPHY)
     * 
     * offset: 0x730
     */
    volatile uint32_t ethernetPhySleepClkGatCtrl;

    /**
     * Controller Area Network Sleep Mode Clock Gating Control (SCGCCAN)
     * 
     * offset: 0x734
     */
    volatile uint32_t canSleepClkGatCtrl;

    /**
     * Analog-to-Digital Converter Sleep Mode Clock Gating Control (SCGCADC)
     * 
     * offset: 0x738
     */
    volatile uint32_t a2dSleepClkGatCtrl;

    /**
     * Analog Comparator Sleep Mode Clock Gating Control (SCGCACMP)
     * 
     * offset: 0x73C
     */
    volatile uint32_t analogCompSleepClkGatCtrl;

    /**
     * Pulse Width Modulator Sleep Mode Clock Gating Control (SCGCPWM)
     * 
     * offset: 0x740
     */
    volatile uint32_t pwmSleepClkGatCtrl;

    /**
     * Quadrature Encoder Interface Sleep Mode Clock Gating Control (SCGCQEI)
     * 
     * offset: 0x744
     */
    volatile uint32_t quadEncSleepClkCtrl;

    /**
     * Reserved
     */
    volatile uint32_t reserved40[4];

    /**
     * EEPROM Sleep Mode Clock Gating Control (SCGCEEPROM)
     * 
     * offset: 0x758
     */
    volatile uint32_t eepromSleepClkGatCtrl;

    /**
     * Reserved
     */
    volatile uint32_t reserved41[6];

    /**
     * CRC and Cryptographic Modules Sleep Mode Clock Gating Control (SCGCCCM)
     * 
     * offset: 0x774
     */
    volatile uint32_t crcSleepModeClkGatCtr;

    /**
     * Reserved
     */
    volatile uint32_t reserved42[9];

    /**
     * Ethernet MAC Sleep Mode Clock Gating Control (SCGCEMAC)
     * 
     * offset: 0x79C
     */
    volatile uint32_t ethMacSleepModeClkGatCtrl;

    /**
     * Reserved
     */
    volatile uint32_t reserved61[24];

    /**
     * Watchdog Timer Deep-Sleep Mode Clock Gating Control (DCGCWD)
     * 
     * offset: 0x800
     */
    volatile uint32_t watchdogDeepSleepClkGatCtrl;

    /**
     * 16/32-Bit General-Purpose Timer Deep-Sleep Mode Clock Gating Control (DCGCTIMER)
     * 
     * offset: 0x804
     */
    volatile uint32_t gptClkGatCtrl;

    /**
     * General-Purpose Input/Output Deep-Sleep Mode Clock Gating Control (DCGCGPIO)
     * 
     * offset: 0x808
     */
    volatile uint32_t gpioDeepSleepClkGatCtrl;

    /**
     * Micro Direct Memory Access Deep-Sleep Mode Clock Gating Control (DCGCDMA)
     * 
     * offset: 0x80C
     */
    volatile uint32_t microDmaDeepSleepClkGatCtrl;

    /**
     * EPI Deep-Sleep Mode Clock Gating Control (DCGCEPI)
     * 
     * offset: 0x810
     */
    volatile uint32_t epiDeepSleepClkGatCtrl;

    /**
     * Hibernation Deep-Sleep Mode Clock Gating Control (DCGCHIB)
     * 
     * offset: 0x814
     */
    volatile uint32_t hibernationDeepSleepClkGatCtrl;

    /**
     * Reserved
     */
    volatile uint32_t reserved43;

    /**
     * Synchronous Serial Interface Deep-Sleep Mode Clock Gating Control (DCGCSSI)
     * 
     * offset: 0x81C
     */
    volatile uint32_t ssiDeepSleepClkGatCtrl;

    /**
     * Inter-Integrated Circuit Deep-Sleep Mode Clock Gating Control (DCGCI2C)
     * 
     * offset: 0x820
     */
    volatile uint32_t interIntegDeepSleepClkGatCtrl;

    /**
     * Reserved
     */
    volatile uint32_t reserved44;

    /**
     * Universal Serial Bus Deep-Sleep Mode Clock Gating Control (DCGCUSB)
     * 
     * offset: 0x828
     */
    volatile uint32_t usbDeepSleepClkGatCtrl;

    /**
     * Reserved
     */
    volatile uint32_t reserved45;

    /**
     * Ethernet PHY Deep-Sleep Mode Clock Gating Control (DCGCEPHY)
     * 
     * offset: 0x830
     */
    volatile uint32_t ethernetPhyDeepSleepClkGatCtrl;

    /**
     * Controller Area Network Deep-Sleep Mode Clock Gating Control (DCGCCAN)
     * 
     * offset: 0x834
     */
    volatile uint32_t canDeepSleepClkGatCtrl;

    /**
     * Analog-to-Digital Converter Deep-Sleep Mode Clock Gating Control (DCGCADC)
     * 
     * offset: 0x838
     */
    volatile uint32_t a2dDeepSleppClkGatCtrl;

    /**
     * Analog Comparator Deep-Sleep Mode Clock Gating Control (DCGCACMP)
     * 
     * offset: 0x83C
     */
    volatile uint32_t analogCompDeepSleepClkGatCtrl;

    /**
     * Pulse Width Modulator Deep-Sleep Mode Clock Gating Control (DCGCPWM)
     * 
     * offset: 0x840
     */
    volatile uint32_t pwmDeepSleepClkGatCtrl;

    /**
     * Quadrature Encoder Interface Deep-Sleep Mode Clock Gating Control (DCGCQEI)
     * 
     * offset: 0x844
     */
    volatile uint32_t quadEncDeepSleepClkGatCtrl;

    /**
     * Reserved
     */
    volatile uint32_t reserved46[4];

    /**
     * EEPROM Deep-Sleep Mode Clock Gating Control (DCGCEEPROM)
     * 
     * offset: 0x858
     */
    volatile uint32_t eepromDeepSleepClkGatCtrl;

    /**
     * Reserved
     */
    volatile uint32_t reserved47[6];

    /**
     * CRC and Cryptographic Modules Deep-Sleep Mode Clock Gating Control (DCGCCCM)
     * 
     * offset: 0x874
     */
    volatile uint32_t crcDeepSleepClockGatCtrl;

    /**
     * Reserved
     */
    volatile uint32_t reserved64[9];

    /**
     * Ethernet MAC Deep-Sleep Mode Clock Gating Control (DCGCEMAC)
     * 
     * offset: 0x89C
     */
    volatile uint32_t ethernetMacDeepSleepClkGatCtrl;

    /**
     * Reserved
     */
    volatile uint32_t reserved62[24];

    /**
     * Watchdog Timer Power Control (PCWD)
     * 
     * offset: 0x900
     */
    volatile uint32_t watchdogTimer;

    /**
     * 16/32-Bit General-Purpose Timer Power Control (PCTIMER)
     * 
     * offset: 0x904
     */
    volatile uint32_t gptPowerControl;

    /**
     * General-Purpose Input/Output Power Control (PCGPIO)
     * 
     * offset: 0x908
     */
    volatile uint32_t gpioPowerControl;

    /**
     * Micro Direct Memory Access Power Control (PCDMA)
     * 
     * offset: 0x90C
     */
    volatile uint32_t microDmaPowerControl;

    /**
     * External Peripheral Interface Power Control (PCEPI)
     * 
     * offset: 0x910
     */
    volatile uint32_t externalPerInterPowerControl;

    /**
     * Hibernation Power Control (PCHIB)
     * 
     * offset: 0x914
     */
    volatile uint32_t hibernationPowerControl;

    /**
     * Universal Asynchronous Receiver/Transmitter Power Control (PCUART)
     * 
     * offset: 0x918
     */
    volatile uint32_t uartPowerControl;

    /**
     * Synchronous Serial Interface Power Control (PCSSI)
     * 
     * offset: 0x91C
     */
    volatile uint32_t ssiPowerControl;

    /**
     * Inter-Integrated Circuit Power Control (PCI2C)
     * 
     * offset: 0x920
     */
    volatile uint32_t interIntCircuitPowerControl;

    /**
     * Reserved
     */
    volatile uint32_t reserved48;

    /**
     * Universal Serial Bus Power Control (PCUSB)
     * 
     * offset: 0x928
     */
    volatile uint32_t usbPowerControl;

    /**
     * Reserved
     */
    volatile uint32_t reserved49;

    /**
     * Ethernet PHY Power Control (PCEPHY)
     * 
     * offset: 0x930
     */
    volatile uint32_t ethernetPhyPowerControl;

    /**
     * Controller Area Network Power Control (PCCAN)
     * 
     * offset: 0x934
     */
    volatile uint32_t canPowerControl;

    /**
     * Analog-to-Digital Converter Power Control (PCADC)
     * 
     * offset: 0x938
     */
    volatile uint32_t a2dConverterPowerControl;

    /**
     * Analog Comparator Power Control (PCACMP)
     * 
     * offset: 0x93C
     */
    volatile uint32_t analCompPowerControl;

    /**
     * Pulse Width Modulator Power Control (PCPWM)
     * 
     * offset: 0x940
     */
    volatile uint32_t pwmPowerControl;

    /**
     * Quadrature Encoder Interface Power Control (PCQEI)
     * 
     * offset: 0x944
     */
    volatile uint32_t quadEncInterPowerControl;

    /**
     * Reserved
     */
    volatile uint32_t reserved50[4];

    /**
     * EEPROM Power Control (PCEEPROM)
     * 
     * offset: 0x958
     */
    volatile uint32_t eepromPowerControl;

    /**
     * Reserved
     */
    volatile uint32_t reserved51[6];

    /**
     * CRC and Cryptographic Modules Power Control (PCCCM)
     * 
     * offset: 0x974
     */
    volatile uint32_t crcPowerControl;

    /**
     * Reserved
     */
    volatile uint32_t reserved52[9];

    /**
     * Ethernet MAC Power Control (PCEMAC)
     * 
     * offset: 0x99C
     */
    volatile uint32_t ethernetMacPowerControl;

    /**
     * Reserved
     */
    volatile uint32_t reserved63[24];

    /**
     * Watchdog Timer Peripheral Ready (PRWD)
     * 
     * offset: 0xA00
     */
    volatile uint32_t watchdogPerReady;

    /**
     * 16/32-Bit General-Purpose Timer Peripheral Ready (PRTIMER)
     * 
     * offset: 0xA04
     */
    volatile uint32_t gptPerReady;

    /**
     * General-Purpose Input/Output Peripheral Ready (PRGPIO)
     * 
     * offset: 0xA08
     */
    volatile uint32_t gpioPerReady;

    /**
     * Micro Direct Memory Access Peripheral Ready (PRDMA)
     * 
     * offset: 0xA0C
     */
    volatile uint32_t microDmaPerReady;

    /**
     * EPI Peripheral Ready (PREPI)
     * 
     * offset: 0xA10
     */
    volatile uint32_t epiPerReady;

    /**
     * Hibernation Peripheral Ready (PRHIB)
     * 
     * offset: 0xA14
     */
    volatile uint32_t hibernationPerReady;

    /**
     * Universal Asynchronous Receiver/Transmitter Peripheral Ready (PRUART)
     * 
     * offset: 0xA18
     */
    volatile uint32_t uartRxTRxPerReady;

    /**
     * Synchronous Serial Interface Peripheral Ready (PRSSI)
     * 
     * offset: 0xA1C
     */
    volatile uint32_t ssiPerReady;

    /**
     * Inter-Integrated Circuit Peripheral Ready (PRI2C)
     * 
     * offset: 0xA20
     */
    volatile uint32_t interIntegratedCirPerReady;

    /**
     * Reserved
     */
    volatile uint32_t reserved53;

    /**
     * Universal Serial Bus Peripheral Ready (PRUSB)
     * 
     * offset: 0xA28
     */
    volatile uint32_t usbReady;

    /**
     * Reserved
     */
    volatile uint32_t reserved54;

    /**
     * Ethernet PHY Peripheral Ready (PREPHY)
     * 
     * offset: 0xA30
     */
    volatile uint32_t ethernetPhyPerReady;

    /**
     * Controller Area Network Peripheral Ready (PRCAN)
     * 
     * offset: 0xA34
     */
    volatile uint32_t canPerReady;

    /**
     * Analog-to-Digital Converter Peripheral Ready (PRADC)
     * 
     * offset: 0xA38
     */
    volatile uint32_t a2dConPerReady;

    /**
     * Analog Comparator Peripheral Ready (PRACMP)
     * 
     * offset: 0xA3C
     */
    volatile uint32_t analogCompPerReady;

    /**
     * Pulse Width Modulator Peripheral Ready (PRPWM)
     * 
     * offset: 0xA40
     */
    volatile uint32_t pwmPerReady;

    /**
     * Quadrature Encoder Interface Peripheral Ready (PRQEI)
     * 
     * offset: 0xA44
     */
    volatile uint32_t quadEncInterPerReady;

    /**
     * Reserved
     */
    volatile uint32_t reserved55[4];

    /**
     * EEPROM Peripheral Ready (PREEPROM)
     * 
     * offset: 0xA58
     */
    volatile uint32_t eepromPerReady;

    /**
     * Reserved
     */
    volatile uint32_t reserved56[6];

    /**
     * CRC and Cryptographic Modules Peripheral Ready (PRCCM)
     * 
     * offset: 0xA74
     */
    volatile uint32_t crcPerReady;

    /**
     * Reserved
     */
    volatile uint32_t reserved57[9];

    /**
     * Ethernet MAC Peripheral Ready (PREMAC)
     * 
     * offset: 0xA9C
     */
    volatile uint32_t ethernetMacPerReady;

    /**
     * Reserved
     */
    volatile uint32_t reserved58[288];

    /**
     * Unique ID 0 (UNIQUEID0)
     * 
     * offset: 0xF20
     */
    volatile uint32_t uniqueId0;

    /**
     * Unique ID 1 (UNIQUEID1)
     * 
     * offset: 0xF24
     */
    volatile uint32_t uniqueId1;

    /**
     * Unique ID 2 (UNIQUEID2)
     * 
     * offset: 0xF28
     */
    volatile uint32_t uniqueId2;

    /**
     * Unique ID 3 (UNIQUEID3)
     * 
     * offset: 0xF2C
     */
    volatile uint32_t uniqueId3;

};


/**
 * restore the previous pack definition
 */
#pragma pack(pop)


/**
 * Initialize the CPU
 */
void pharosCpuInitialize(void);


void pharosCpuFpuInitCpac(void);

/**
 * Initialize the CPU oscillator to provide 120MHz system clock
 */
void pharosCpuOscInit(void);


#endif	/* PHAROS_CPU_H */
