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


#ifndef PHAROS_SYSTEM_H
#define	PHAROS_SYSTEM_H

#ifdef	__cplusplus
extern "C"
{
#endif


#include <pharos/hal/cpu/declarations.h>
#include <pharos/hal/cpu/conf.h>



#define PHAROS_SYSTEM_PRIMARY       ((ptrPharosCpuSystemPrimary) PHAROS_CPU_SYSTEM_PRIMARY_ADDRESS)

#define PHAROS_SYSTEM_SECUNDARY     ((ptrPharosCpuSystemSecundardy) PHAROS_CPU_SYSTEM_SECUNDARY_ADDRESS)

    /***********************************************************************
     * Primary System register definitions                                 *
     **********************************************************************/

    /************************************************************
     * SYSPC1 register definitions                              *
     ************************************************************/
#define PHAROS_CPU_SYS1_SYSPC1_GIO                  (0U << 0U)
#define PHAROS_CPU_SYS1_SYSPC1_FUNC                 (1U << 0U)

    /************************************************************
     * SYSPC2 register definitions                              *
     ************************************************************/
#define PHAROS_CPU_SYS1_SYSPC2_INPUT                (0U << 0U)
#define PHAROS_CPU_SYS1_SYSPC2_OUTPUT               (1U << 0U)


    /************************************************************
     * SYSPC4 register definitions                              *
     ************************************************************/
#define PHAROS_CPU_SYS1_SYSPC4_LOW                  (0U << 0U)
#define PHAROS_CPU_SYS1_SYSPC4_HIGH                 (1U << 0U)


    /************************************************************
     * SYSPC7 register definitions                              *
     ************************************************************/
#define PHAROS_CPU_SYS1_SYSPC7_LOW                  (0U << 0U)
#define PHAROS_CPU_SYS1_SYSPC7_TRISTATE             (1U << 0U)

    /************************************************************
     * SYSPC8 register definitions                              *
     ************************************************************/
#define PHAROS_CPU_SYS1_SYSPC8_ACTIVE               (0U << 0U)
#define PHAROS_CPU_SYS1_SYSPC8_INACTIVE             (1U << 0U)

    /************************************************************
     * SYSPC9 register definitions                              *
     ************************************************************/
#define PHAROS_CPU_SYS1_SYSPC9_PULLDOWN             (0U << 0U)
#define PHAROS_CPU_SYS1_SYSPC9_PULLUP               (1U << 0U)


    /************************************************************
     * CDDIS register definitions                               *
     ************************************************************/
#define PHAROS_CPU_SYS1_CDDIS_VCLKA1_EN             (0U << 4U)
#define PHAROS_CPU_SYS1_CDDIS_VCLKA1_DI             (1U << 4U)

#define PHAROS_CPU_SYS1_CDDIS_VCLKA2_EN             (0U << 5U)
#define PHAROS_CPU_SYS1_CDDIS_VCLKA2_DI             (1U << 5U)

#define PHAROS_CPU_SYS1_CDDIS_VCLKA3_EN             (0U << 8U)
#define PHAROS_CPU_SYS1_CDDIS_VCLKA3_DI             (1U << 8U)

#define PHAROS_CPU_SYS1_CDDIS_VCLKA3_EN             (0U << 8U)

#define PHAROS_CPU_SYS1_CDDIS_VCLKA11_EN            (0U << 11U)

    /************************************************************
     * CSDIS register definitions                               *
     ************************************************************/
#define PHAROS_CPU_SYS1_CSDIS_X_DIS(x)              (1U << (x))
#define PHAROS_CPU_SYS1_CSDIS_X_EN(x)               (0U << (x))


    /************************************************************
     * CSDISSET register definitions                            *
     ************************************************************/
#define PHAROS_CPU_SYS1_CSDISSET_7_DIS              (1U << 7U)
#define PHAROS_CPU_SYS1_CSDISSET_7_EN               (0U << 7U)

#define PHAROS_CPU_SYS1_CSDISSET_6_DIS              (1U << 6U)
#define PHAROS_CPU_SYS1_CSDISSET_6_EN               (0U << 6U)

#define PHAROS_CPU_SYS1_CSDISSET_5_DIS              (1U << 5U)
#define PHAROS_CPU_SYS1_CSDISSET_5_EN               (0U << 5U)

#define PHAROS_CPU_SYS1_CSDISSET_4_DIS              (1U << 4U)
#define PHAROS_CPU_SYS1_CSDISSET_4_EN               (0U << 4U)

#define PHAROS_CPU_SYS1_CSDISSET_3_DIS              (1U << 3U)
#define PHAROS_CPU_SYS1_CSDISSET_3_EN               (0U << 3U)

#define PHAROS_CPU_SYS1_CSDISSET_1_DIS              (1U << 1U)
#define PHAROS_CPU_SYS1_CSDISSET_1_EN               (0U << 1U)

#define PHAROS_CPU_SYS1_CSDISSET_0_DIS              (1U << 0U)
#define PHAROS_CPU_SYS1_CSDISSET_0_EN               (0U << 0U)

#define PHAROS_CPU_SYS1_CSDISSET_X_DIS(x)           (1U << x)
#define PHAROS_CPU_SYS1_CSDISSET_X_EN(x)            (0U << x)

    /************************************************************
     * MINITGCR register definitions                            *
     ************************************************************/
#define PHAROS_CPU_MINITGCR_KEY                     (0xaU)


    /************************************************************
     * MSINENA register definitions                             *
     ************************************************************/
#define PHAROS_CPU_MSINENA_SELFTEST                 (0x1U)

    /************************************************************
     * MSTCGSTAT register definitions                           *
     ************************************************************/
#define PHAROS_CPU_MSTCGSTAT_MEM_DONE               (1U << 8U)


    /************************************************************
     * SYSESR register definitions                              *
     ************************************************************/
#define PHAROS_CPU_SYS1_SYSESR_PWR_RST              (1U << 15U)
#define PHAROS_CPU_SYS1_SYSESR_PWR_OSC              (1U << 14U)
#define PHAROS_CPU_SYS1_SYSESR_PWR_WTD              (1U << 13U)
#define PHAROS_CPU_SYS1_SYSESR_PWR_WTD2             (1U << 12U)
#define PHAROS_CPU_SYS1_SYSESR_PWR_DGB              (1U << 11U)
#define PHAROS_CPU_SYS1_SYSESR_PWR_ICT              (1U <<  7U)
#define PHAROS_CPU_SYS1_SYSESR_PWR_CPU              (1U <<  5U)
#define PHAROS_CPU_SYS1_SYSESR_PWR_SWR              (1U <<  4U)
#define PHAROS_CPU_SYS1_SYSESR_PWR_EXT              (1U <<  3U)

#define PHAROS_CPU_SYS1_SYSESR_MPU_EN               (1U <<  0U)

#define PHAROS_CPU_SYS1_SYSESR_MPU_UNMASK           (0xfffffffeU)



    /************************************************************
     * GLBSTAT register definitions                             *
     ************************************************************/
#define PHAROS_CPU_SYS1_GLBSTAT_OSCFAIL_CLR         (1U << 0U)
#define PHAROS_CPU_SYS1_GLBSTAT_RFSLIP_CLR          (1U << 8U)
#define PHAROS_CPU_SYS1_GLBSTAT_RBSLIP_CLR          (1U << 9U)


    /************************************************************
     * GHVSRC register definitions                              *
     ************************************************************/
#define PHAROS_CPU_SYS1_GHVSRC_WAKEUP(x)            ((x) << 24U)
#define PHAROS_CPU_SYS1_GHVSRC_NORMAL(x)            ((x) << 0U)
#define PHAROS_CPU_SYS1_GHVSRC_PWRDOWN(x)           ((x) << 16U)

    /************************************************************
     * PLLCTL1 register definitions                             *
     ************************************************************/
#define PHAROS_CPU_SYS1_PLLCTL1_ROS_EN              (1U << 31U)            
#define PHAROS_CPU_SYS1_PLLCTL1_DIV_MASK            (0xE0FFFFFFU)

#define PHAROS_CPU_SYS1_PLLCTL1_DIV(x)              ((x) << 24U)

    /************************************************************
     * PLLCTL2 register definitions                             *
     ************************************************************/
#define PHAROS_CPU_SYS1_PLLCTL2_MOD_EN              (1U << 31U)
#define PHAROS_CPU_SYS1_PLLCTL2_MOD_DIS             (0U << 31U)

#define PHAROS_CPU_SYS1_PLLCTL2_DIV(x)              ((x) << 24U)


    /************************************************************
     * PLLCTL3 register definitions                             *
     ************************************************************/
#define PHAROS_CPU_SYS1_PLLCTL3_2DIV_MASK           (0xE0FFFFFFU)

#define PHAROS_CPU_SYS1_PLLCTL3_DIV(x)              ((x) << 24U)

    /************************************************************
     * LPOMONCTL register definitions                           *
     ************************************************************/
#define PHAROS_CPU_SYS1_LPOMONCTL_BIAS_EN           (1U << 24U)
#define PHAROS_CPU_SYS1_LPOMONCTL_BIAS_DIS          (0U << 24U)

#define PHAROS_CPU_SYS1_LPOMONCTL_HFTRIM(x)         ((x) << 8U)
#define PHAROS_CPU_SYS1_LPOMONCTL_LFTRIM(x)         ((x) << 0U)


    /************************************************************
     * ECPCNTL register definitions                             *
     ************************************************************/
#define PHAROS_CPU_SYS1_ECPCNTL_VCLK                (0U << 24U)
#define PHAROS_CPU_SYS1_ECPCNTL_OSCIN               (1U << 24U)

#define PHAROS_CPU_SYS1_ECPCNTL_DIS                 (0U << 23U)
#define PHAROS_CPU_SYS1_ECPCNTL_EN                  (1U << 23U)

    /**
     * Controls the divider of the ECLK. The ECLK frequency is calculated by
     * 
     * ECLK_freq = ClockSource_freq / (DIV + 1)
     * 
     * where the ClockSource can be VCLK or OSCIN
     */
#define PHAROS_CPU_SYS1_ECPCNTL_DIV(x)              ((x-1) & 0xFFFFU)

    /************************************************************
     * RCLKSRC register definitions                             *
     ************************************************************/
#define PHAROS_CPU_SYS1_RCLKSRC_DIV(x)              ((x) << 8U)
#define PHAROS_CPU_SYS1_RCLKSRC_SRC(x)              ((x) << 0U)

    /************************************************************
     * RCLKSRC register definitions                             *
     ************************************************************/
#define PHAROS_CPU_SYS1_VCLKASRC_SRC2(x)            ((x) << 8U)
#define PHAROS_CPU_SYS1_VCLKASRC_SRC1(x)            ((x) << 0U)

    /************************************************************
     * CLKCNTL register definitions                             *
     ************************************************************/
#define PHAROS_CPU_SYS1_CLKCNTL_VCLK2R_MASK         (0xF0FFFFFFU)
#define PHAROS_CPU_SYS1_CLKCNTL_VCLK2R(x)           ((x) << 24U)

#define PHAROS_CPU_SYS1_CLKCNTL_VCLK1R_MASK         (0xFFF0FFFFU)
#define PHAROS_CPU_SYS1_CLKCNTL_VCLK1R(x)           ((x) << 16U)

    /************************************************************
     * PLLCTL1 register definitions                             *
     ************************************************************/
#define PHAROS_CPU_SYS1_PLLCTL1_BSLIPEN             (1U << 29U)
#define PHAROS_CPU_SYS1_PLLCTL1_BSLIPDS             (2U << 29U)
#define PHAROS_CPU_SYS1_PLLCTL1_PLLDIV(x)           ((x) << 24U)
#define PHAROS_CPU_SYS1_PLLCTL1_RCLKDIV(x)          ((x) << 16U)
#define PHAROS_CPU_SYS1_PLLCTL1_PLLMUL(x)           ((x) << 0U)

    /************************************************************
     * PLLCTL2 register definitions                             *
     ************************************************************/
#define PHAROS_CPU_SYS1_PLLCTL2_SNGRATE(x)          ((x) << 22U)
#define PHAROS_CPU_SYS1_PLLCTL2_MULMOD(x)           ((x) << 12U)
#define PHAROS_CPU_SYS1_PLLCTL2_ODPLL(x)            ((x) << 9U)
#define PHAROS_CPU_SYS1_PLLCTL2_SPR_AMT(x)          ((x) << 0U)

    /***********************************************************************
     * Secondary System register definitions                               *
     **********************************************************************/

    /************************************************************
     * HCLKCNT register definitions                             *
     ************************************************************/
#define PHAROS_CPU_SYS2_HCLKCNTL_DIV(x)             ((x) << 0U)


    /************************************************************
     * CLK2CNTRL register definitions                           *
     ************************************************************/
#define PHAROS_CPU_SYS2_CLK2CNTRL_VCLK3R_MASK       (0xFFFFFFF0U)
#define PHAROS_CPU_SYS2_CLK2CNTRL_VCLK3R(x)         ((x) << 0U)

    /************************************************************
     * VCLKACON1 register definitions                           *
     ************************************************************/
#define PHAROS_CPU_SYS2_VCLKACON1_4DIV(x)           ((x) << 24U)

#define PHAROS_CPU_SYS2_VCLKACON1_4DIV_DIS          (1 << 20U)
#define PHAROS_CPU_SYS2_VCLKACON1_4DIV_EN           (0 << 20U)    

#define PHAROS_CPU_SYS2_VCLKACON1_4SRC(x)           ((x) << 16U)    

#define PHAROS_CPU_SYS2_VCLKACON1_3DIV(x)           ((x) << 8U)    

#define PHAROS_CPU_SYS2_VCLKACON1_3DIV_DIS          (1 << 4U)
#define PHAROS_CPU_SYS2_VCLKACON1_3DIV_EN           (0 << 4U)    

#define PHAROS_CPU_SYS2_VCLKACON1_3SRC(x)           ((x) << 0U)    

    /************************************************************
     * PLLCTL3 register definitions                             *
     ************************************************************/
#define PHAROS_CPU_SYS2_PLLCTL3_ODPLL2(x)           ((x) << 29U)
#define PHAROS_CPU_SYS2_PLLCTL3_PLLDIV2(x)          ((x) << 24U)
#define PHAROS_CPU_SYS2_PLLCTL3_CLKDIV2(x)          ((x) << 16U)
#define PHAROS_CPU_SYS2_PLLCTL3_PLLMUL2(x)          ((x) << 0U)



    /**
     * Compact the size of the I/O registers
     */
#pragma pack(push ,4)


    /**
     * TMS570LC53x System primary control registers
     * 
     * Information retrieved from "TMS570LC43x 16/32-Bit RISC Flash Microcontroller, Technical Reference Manual, 
     * SPNU563, May 2014"
     */
    struct PharosCpuSystemPrimary
    {
        /**
         * System Pin Control 1 (SYSPC1)
         * 
         * Allows to enable the External Clock (ECLK) prescalar (ECP CLK FUN)
         * 
         * Offset: 0x0
         */
        volatile uint32_t pinControl1;

        /**
         * System Pin Control 2 (SYSPC2)
         * 
         * Control the ECLK data direction 
         * 
         * Offset: 0x4
         */
        volatile uint32_t pinControl2;

        /**
         * System Pin Control 3 (SYSPC3)
         * 
         * Offset: 0x8
         */
        volatile uint32_t pinControl3;

        /**
         * System Pin Control 4 (SYSPC4)
         * 
         * Offset: 0xc
         */
        volatile uint32_t pinControl4;

        /**
         * System Pin Control 5 (SYSPC5)
         * 
         * Controls the ECLK data out set when set in GIO output mode
         * 
         * Offset: 0x10
         */
        volatile uint32_t pinControl5;

        /**
         * System Pin Control 6 (SYSPC6)
         * 
         * Offset: 0x14
         */
        volatile uint32_t pinControl6;

        /**
         * System Pin Control 7 (SYSPC7)
         * 
         * Offset: 0x18
         */
        volatile uint32_t pinControl7;

        /**
         * System Pin Control 8 (SYSPC8)
         * 
         * Offset: 0x1c
         */
        volatile uint32_t pinControl8;

        /**
         * System Pin Control 9 (SYSPC9)
         * 
         * Offset: 0x20
         */
        volatile uint32_t pinControl9;

        /**
         * reserved
         * 
         * Offset: 0x24
         */
        volatile uint32_t reserved1[3];

        /**
         * Clock source disable register (CSDIS)
         * 
         * controls and displays the state of the device clock sources
         * 
         * Offset: 0x30
         */
        volatile uint32_t clockSourceDisable;

        /**
         * Clock source disable set register (CSDISSET)
         * 
         * sets clock sources to the disabled state
         * 
         * Offset: 0x34
         */
        volatile uint32_t clockSourceDisableSet;

        /**
         * Clock source disable clear register (CSDISCLR)
         * 
         * clears clock sources to the enabled state
         * 
         * Offset: 0x38
         */
        volatile uint32_t clockSourceDisableClear;

        /**
         * Clock domain disable register (CDDIS)
         * 
         * controls the state of the clock domains
         * 
         * Offset: 0x3c
         */
        volatile uint32_t clockDomainDisable;

        /**
         * Clock domain disable set register (CDDISSET)
         * 
         * sets clock domains to the disabled state
         * 
         * Offset: 0x40
         */
        volatile uint32_t clockDomainDisableSet;

        /**
         * Clock domain disable clear register (CDDISCLR)
         * 
         * clears clock domains to the enabled state
         * 
         * Offset: 0x44
         */
        volatile uint32_t clockDomainDisableClear;

        /**
         * GCLK, HCLK, VCLK, and VCLK2 Source Register (GHVSRC)
         * 
         * controls the clock source configuration for the GCLK, HCLK, VCLK and VCLK2 clock domains
         * 
         * Offset: 0x48
         */
        volatile uint32_t clockSourceConf;

        /**
         * Peripheral Asynchronous Clock Source Register (VCLKASRC)
         * 
         * sets the clock source for the asynchronous peripheral clock domains to be configured to run from a specific clock source
         * 
         * Offset: 0x4c
         */
        volatile uint32_t vClockAsynSource;

        /**
         * RTI Clock Source Register (RCLKSRC)
         * 
         * controls the RTI (Real Time Interrupt) clock source selection
         * 
         * Offset: 0x50
         */
        volatile uint32_t rtiClockSource;

        /**
         * Clock Source Valid Status Register (CSVSTAT)
         * 
         * indicates the status of usable clock sources
         * 
         * Offset: 0x54
         */
        volatile uint32_t clockSourceValidStatus;

        /**
         * Memory Self-Test Global Control Register (MSTGCR)
         * 
         * controls several aspects of the PBIST (Programmable Built-In Self Test) memory controller
         * 
         * Offset: 0x58
         */
        volatile uint32_t memorySelfTestGlobalControl;

        /**
         * Memory Hardware Initialization Global Control Register (MINITGCR)
         * 
         * enables automatic hardware memory initialization
         * 
         * Offset: 0x5c
         */
        volatile uint32_t memHwInitializationGlobalCtr;

        /**
         * Memory Self-Test/Initialization Enable Register (MSINENA)
         * 
         * enables PBIST controllers for memory self test and the memory modules initialized during automatic hardware
         * memory initialization
         * 
         * Offset: 0x60
         */
        volatile uint32_t memSelfTestInitEnable;

        /**
         * MSTFAIL
         * 
         * reserved
         * 
         * Offset: 0x64
         */
        volatile uint32_t mstFail;

        /**
         * Memory Hardware Initialization Status Register (MSTCGSTAT)
         * 
         * shows the status of the memory hardware initialization and the memory self-test
         * 
         * Offset: 0x68
         */
        volatile uint32_t memHwInitStatus;

        /**
         * Memory Self-Test Global Control Register (MINISTAT)
         * 
         * indicates the status of hardware memory initialization
         * 
         * Offset: 0x6c
         */
        volatile uint32_t memSelfTestGlobalControl;

        /**
         * PLL Control Register 1 (PLLCTL1)
         * 
         * controls the output frequency of PLL1 (Clock Source 1 - FMzPLL).
         * 
         * Offset: 0x70
         */
        volatile uint32_t pllControl1;

        /**
         * PLL Control Register 2 (PLLCTL2)
         * 
         * controls the modulation characteristics and the output divider of the PLL
         * 
         * Offset: 0x74
         */
        volatile uint32_t pllControl2;

        /**
         * SYS Pin Control Register 10 (SYSPC10)
         * 
         * controls the function of the ECPCLK slew mode          
         * 
         * Offset: 0x78
         */
        volatile uint32_t sysPinControl;

        /**
         * Die Identification Register Upper Word (DIEIDL)
         * 
         * contains information about the die lot number, wafer number and X, Y wafer coordinates
         * 
         * Offset: 0x7c
         */
        volatile uint32_t dieIdLowerWord;

        /**
         * Die Identification Register Upper Word (DIEIDH)
         * 
         * contains information about the die lot number
         * 
         * Offset: 0x80
         */
        volatile uint32_t dieIdUpperWord;

        /**
         * Reserved
         * 
         * Offset: 0x84
         */
        volatile uint32_t reserved2;

        /**
         * LPO/Clock Monitor Control Register (LPOMONCTL)
         * 
         * controls the Low Frequency (Clock Source 4) and High Frequency (Clock Source 5) Low Power Oscillator’s (LPO) 
         * trim values
         * 
         * Offset: 0x88
         */
        volatile uint32_t lpoMonitorControl;

        /**
         * Clock Test Register (CLKTEST)
         * 
         * controls the clock signal that is supplied to the ECLK pin for test and debug purposes
         * 
         * Offset: 0x8c
         */
        volatile uint32_t clockTest;

        /**
         * DFT Control Register (DFTCTRLREG)
         * 
         * control the register key (enable/disable)
         * 
         * Offset: 0x90
         */
        volatile uint32_t dftControl;

        /**
         * DFT Control Register 2 (DFTCTRLREG2)
         * 
         * test mode key
         * 
         * Offset: 0x94
         */
        volatile uint32_t dftControl2;

        /**
         * Reserved
         * 
         * Offset: 0x98
         */
        volatile uint32_t reserved3[2];

        /**
         * General Purpose Register (GPREG1)
         * 
         * Configures the system response when a FBSLIP is indicated by the PLL macro
         * 
         * Offset: 0xa0
         */
        volatile uint32_t generalPurpose;

        /**
         * Reserved
         * 
         * Offset: 0xa4
         */
        volatile uint32_t reserved4[3];

        /**
         * System Software Interrupt Request 1 Register (SSIR1)
         * 
         * used for software interrupt generation
         * 
         * Offset: 0xb0
         */
        volatile uint32_t swInterruptRequest1;

        /**
         * System Software Interrupt Request 2 Register (SSIR2)
         * 
         * used for software interrupt generation
         * 
         * Offset: 0xb4
         */
        volatile uint32_t swInterruptRequest2;

        /**
         * System Software Interrupt Request 3 Register (SSIR3)
         * 
         * used for software interrupt generation
         * 
         * Offset: 0xb8
         */
        volatile uint32_t swInterruptRequest3;

        /**
         * System Software Interrupt Request 4 Register (SSIR4)
         * 
         * used for software interrupt generation
         * 
         * Offset: 0xbc
         */
        volatile uint32_t swInterruptRequest4;

        /**
         * RAM Control Register (RAMGCR)
         * 
         * configure eSRAM data and address wait states
         * 
         * Offset: 0xc0
         */
        volatile uint32_t ramControl;

        /**
         * Bus Matrix Module Control Register 1 (BMMCR1)
         * 
         * allows RAM and Program (Flash) memory addresses to be swapped
         * 
         * Offset: 0xc4
         */
        volatile uint32_t busMatrixModuleControl1;

        /**
         * Reserved
         * 
         * Offset: 0xc8
         */
        volatile uint32_t reserved5;

        /**
         * CPU Reset Control Register (CPURSTCR)
         * 
         * allows the memory protection mode to be configured. It also allows a reset to the Cortex R4 CPU 
         * to be generated
         * 
         * Offset: 0xcc
         */
        volatile uint32_t cpuResetControl;

        /**
         * Clock Control Register (CLKCNTL)
         * 
         * controls peripheral reset and the peripheral clock divide ratios
         * 
         * Offset: 0xd0
         */
        volatile uint32_t clockControl;

        /**
         * ECP Control Register (ECPCNTL)
         * 
         * configures the ECLK pin in functional mode
         * 
         * Offset: 0xd4
         */
        volatile uint32_t ecpControl;

        /**
         * Reserved
         * 
         * Offset: 0xd8
         */
        volatile uint32_t reserved6;

        /**
         * DEV Parity Control Register 1 (DEVCR1)
         * 
         * used to determine the parity of the device (odd or even)
         * 
         * Offset: 0xdc
         */
        volatile uint32_t deviceParityControl1;

        /**
         * System Exception Control Register (SYSECR)
         * 
         * used to generate a software reset
         * 
         * Offset: 0xe0
         */
        volatile uint32_t systemExceptionControl;

        /**
         * System Exception Status Register (SYSESR)
         * 
         * shows the source for different resets encountered
         * 
         * Offset: 0xe4
         */
        volatile uint32_t systemExceptionStatus;

        /**
         * System Test Abort Status Register (SYSTASR)
         * 
         * Test Abort status flag
         * 
         * Offset: 0xe8
         */
        volatile uint32_t systemTestAbortStatus;

        /**
         * Global Status Register (GLBSTAT) 
         * 
         * indicates the FMzPLL (PLL1) slip status and the oscillator fail status
         * 
         * Offset: 0xec
         */
        volatile uint32_t globalStatus;

        /**
         * Device Identification Register (DEVID)
         * 
         * device-specific information that is hard-coded during device manufacture
         * 
         * Offset: 0xf0
         */
        volatile uint32_t deviceId;

        /**
         * Software Interrupt Vector Register (SSIVEC)
         * 
         * contains information about software interrupts
         * 
         * Offset: 0xf4
         */
        volatile uint32_t swInterruptVector;

        /**
         * System Software Interrupt Flag Register (SSIF)
         * 
         * contains software interrupt flag status information
         * 
         * Offset: 0xf8
         */
        volatile uint32_t swInterruptFlag;
    };


    /**
     * restore the previous pack definition
     */
#pragma pack(pop)


    /**
     * TMS570LC53x System secondary control registers
     * 
     * Information retrieved from "TMS570LC43x 16/32-Bit RISC Flash Microcontroller, Technical Reference Manual, 
     * SPNU563, May 2014"
     */
    struct PharosCpuSystemSecundary
    {
        /**
         * PLL Control Register 3 (PLLCTL3)
         * 
         * controls the settings of PLL2 (Clock Source 6 - FPLL)
         * 
         * Offset: 0x00
         */
        volatile uint32_t pllControl3;

        /**
         * Reserved
         * 
         * Offset: 0x04
         */
        volatile uint32_t reserved1;

        /**
         * CPU Logic BIST Clock Divider (STCCLKDIV)
         * 
         * used to set the internal Cortex R4 CPU Logic Built In Self-Test (LBIST) divider
         * 
         * Offset: 0x08
         */
        volatile uint32_t cpuBistClockDivider;

        /**
         * Reserved
         * 
         * Offset: 0x0c
         */
        volatile uint32_t reserved2[6U];

        /**
         * ECP Control Register (ECPCNTL)
         * 
         * configures the ECLK pin in functional mode
         * 
         * Offset: 0x24
         */
        volatile uint32_t ecpControl;

        /**
         * ECP Control Register 1 (ECPCNTL1)
         * 
         * configures the ECLK2 pin in functional mode
         * 
         * Offset: 0x28
         */
        volatile uint32_t ecpControl1;

        /**
         * Reserved
         * 
         * Offset: 0x2c
         */
        volatile uint32_t reserved3[4U];

        /**
         * Clock 2 Control Register (CLK2CNTRL)
         * 
         * Controls the VBUS clock3 ration
         * 
         * Offset: 0x3c
         */
        volatile uint32_t clock2Control;

        /**
         * Peripheral Asynchronous Clock Configuration 1 Register (VCLKACON1)
         * 
         * Controls the peripheral asynchronous Clock 
         * 
         * Offset: 0x40
         */
        volatile uint32_t peripheralAsynClockConf;

        /**
         * Reserved
         * 
         * Offset: 0x44
         */
        volatile uint32_t reserved4[4U];

        /**
         * HCLK Control Register (HCLKCNTL)
         * 
         * HCLK divider value
         * 
         * Offset: 0x54
         */
        volatile uint32_t hclkControl;

        /**
         * Reserved
         * 
         * Offset: 0x58
         */
        volatile uint32_t reserved5[6U];

        /**
         * Clock Slip Control Register (CLKSLIP)
         * 
         * Controls the PLL1 RFSLIP Filter count and key
         * 
         * Offset: 0x70
         */
        volatile uint32_t clockSlipControl;

        /**
         * Reserved
         * 
         * Offset: 0x74
         */
        volatile uint32_t reserved6;

        /**
         * IP ECC Error Enable Register (IP1ECCERREN)
         * 
         * ECC Error Enable Key for PS_SCR_M master and DMA port A master
         * 
         * Offset: 0x78
         */
        volatile uint32_t ipEccErrorEnable;

        /**
         * Reserved
         * 
         * Offset: 0x7c
         */
        volatile uint32_t reserved7[28U];

        /**
         * EFUSE Controller Control Register (EFC_CTLEN)
         * 
         * Controls the EFUSE
         * 
         * Offset: 0xec
         */
        volatile uint32_t efuseControllerControl;

        /**
         * Die Identification Register Lower Word (DIEIDL_REG0)
         * 
         * This read-only register contains the lower word (31:0) of the die ID information
         * 
         * Offset: 0xf0
         */
        volatile uint32_t dieIdRegisterLow0;

        /**
         * Die Identification Register Upper Word (DIEIDH_REG1)
         * 
         * This read-only register contains the upper word (63:32) of the die ID information
         * 
         * Offset: 0xf4
         */
        volatile uint32_t dieIdRegisterHigh0;

        /**
         * Die Identification Register Lower Word 2 (DIEIDL_REG2)
         * 
         * This read-only register contains the lower word (95:64) of the die ID information.
         * 
         * Offset: 0xf8
         */
        volatile uint32_t dieIdRegisterLow1;

        /**
         * Die Identification Register Upper Word 2 (DIEIDH_REG3)
         * 
         * This read-only register contains the upper word (127:97) of the die ID information.
         * 
         * Offset: 0xfc
         */
        volatile uint32_t dieIdRegisterHigh1;
    };


    /**
     * Cause of a reset
     */
    typedef enum
    {
        /**
         * no reset was made
         */
        PHAROS_CPU_RST_SRC_NO_RESET = 0U ,

        /**
         * reset is caused when a power-on reset occurs, either internally asserted by the VMON or externally 
         * asserted by the nPORRST pin 
         */
        PHAROS_CPU_RST_SRC_POWER_ON = 15U ,

        /**
         * reset is caused by an oscillator failure or PLL slip
         *
         * The action taken when an oscillator failure or PLL slip is detected must configured in the PLLCTL1 register
         */
        PHAROS_CPU_RST_SRC_OSC = 14U ,

        /**
         * reset is caused by the analog or digital watchdog
         */
        PHAROS_CPU_RST_SRC_WATCHDOG = 13U ,

        /**
         * reset is caused by the second analog or digital watchdog
         */
        PHAROS_CPU_RST_SRC_WATCHDOG2 = 12U ,

        /**
         * reset is caused when the last reset was caused by the debugger reset request 
         */
        PHAROS_CPU_RST_SRC_DEBUG = 11U ,

        /**
         * reset is caused when the last CPU reset was caused by the entering and exiting of interconnect 
         * self-test check
         */
        PHAROS_CPU_RST_SRC_INTERCONNECT = 7U ,

        /**
         * reset is caused by the CPU
         * 
         * A CPU reset can be initiated by the CPU self-test controller (LBIST) or by toggling the CPU RESET bit 
         * field in CPURSTCR register.
         */
        PHAROS_CPU_RST_SRC_CPU = 5U ,

        /**
         * reset is caused by a software system reset has occurred. 
         * 
         * A software system reset can be initiated by writing to the RESET bits in the SYSECR register.
         */
        PHAROS_CPU_RST_SRC_SW = 4U ,

        /**
         * reset is caused by the external reset pin nRST or by any reset that also asserts the nRST pin (PORST, 
         * OSCRST, WDRST, WD2RST, and SWRST)
         */
        PHAROS_CPU_RST_SRC_EXTERNAL = 3U ,

    } PharosCpuResetSource;


    /**
     * Gets the reset source
     * 
     * @return returns the reset source
     */
    PharosCpuResetSource pharosCpuResetSource(void);

    /**
     * Clears the reset source
     */
    void pharosCpuResetSourceClear(void);

#ifdef	__cplusplus
}
#endif

#endif	/* SYSTEM_H */
