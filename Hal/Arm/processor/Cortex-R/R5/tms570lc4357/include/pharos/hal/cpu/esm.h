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


/* 
 * File:   esm.h
 *
 * Created on September 27, 2017
 */

#ifndef PHAROS_CPU_ESM_H
#define PHAROS_CPU_ESM_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <pharos/hal/cpu/declarations.h>
#include <pharos/hal/cpu/conf.h>


#define PHAROS_CPU_ESM ((ptrPharosCpuEsm) PHAROS_CPU_ESM_ADDRESS)

    /**
     * ESM Channel Assignments
     * 
     * Retrieved from SPNS195C – FEBRUARY 2014 – REVISED JUNE 2016, 
     * "TMS570LC4357 HerculesTM Microcontroller Based on the ARM® Cortex®-R Core" Table 6-45.
     * 
     * Group1 are configurable, maskable, low or high priority interrupts
     */
    typedef enum
    {
        /**
         * MibADC2 - parity
         */
        PHAROS_ESM_GR1_MIBADC2_PAR = 1U ,

        /**
         * DMA - MPU error for CPU (DMAOCP_MPVINT(0))
         */
        PHAROS_ESM_GR1_DMA_MPU = 2U ,

        /**
         * DMA - ECC uncorrectable error
         */
        PHAROS_ESM_GR1_DMA_ECC_UNC = 3U ,

        /**
         * EPC - Correctable Error
         */
        PHAROS_ESM_GR1_EPC_CORR = 4U ,

        /**
         * L2FMC - correctable error (implicit OTP read).
         */
        PHAROS_ESM_GR1_L2FMC_CORR = 6U ,

        /**
         * NHET1 - parity
         */
        PHAROS_ESM_GR1_NHET1_PAR = 7U ,

        /**
         * HET TU1/HET TU2 - parity
         */
        PHAROS_ESM_GR1_HET_PAR = 8U ,

        /**
         * HET TU1/HET TU2 - MPU
         */
        PHAROS_ESM_GR1_HET_MPU = 9U ,

        /**
         * PLL1 - slip
         */
        PHAROS_ESM_GR1_PLL1_SLIP = 10U ,

        /**
         * LPO Clock Monitor - interrupt
         */
        PHAROS_ESM_GR1_LPO_INT = 11U ,

        /**
         * FlexRay RAM - ECC uncorrectable error
         */
        PHAROS_ESM_GR1_FRAY_ECC_UNC = 12U ,

        /**
         * FlexRay TU RAM - ECC uncorrectable error (TU_UCT_err)
         */
        PHAROS_ESM_GR1_FRAY_TU_ECC_UNC = 14U ,

        /**
         * VIM RAM - ECC uncorrectable error
         */
        PHAROS_ESM_GR1_VIM_ECC_UNC = 15U ,

        /**
         * FlexRay TU - MPU violation (TU_MPV_err)
         */
        PHAROS_ESM_GR1_FRAY_MPU = 16U ,

        /**
         * MibSPI1 - ECC uncorrectable error
         */
        PHAROS_ESM_GR1_SPI1_ECC_UNC = 17U ,

        /**
         * MibSPI3 - ECC uncorrectable error
         */
        PHAROS_ESM_GR1_SPI3_ECC_UNC = 18U ,

        /**
         * MibADC1 - parity
         */
        PHAROS_ESM_GR1_ADC1_PAR = 19U ,

        /**
         * DMA - Bus Error
         */
        PHAROS_ESM_GR1_DMA_BUS_ERR = 20U ,

        /**
         * DCAN1 - ECC uncorrectable error
         */
        PHAROS_ESM_GR1_DCAN1_ECC_UNC = 21U ,

        /**
         * DCAN3 - ECC uncorrectable error
         */
        PHAROS_ESM_GR1_DCAN3_ECC_UNC = 22U ,

        /**
         * DCAN2 - ECC uncorrectable error
         */
        PHAROS_ESM_GR1_DCAN2_ECC_UNC = 23U ,

        /**
         * MibSPI5 - ECC uncorrectable error
         */
        PHAROS_ESM_GR1_SPI5_ECC_UNC = 24U ,

        /**
         * L2RAMW - correctable error
         */
        PHAROS_ESM_GR1_L2RAMW_COR = 26U ,

        /**
         * Cortex-R5F CPU - self-test
         */
        PHAROS_ESM_GR1_CPU_SF_TST = 27U ,

        /**
         * DCC1 - error
         */
        PHAROS_ESM_GR1_DCC1_ERR = 30U ,

        /**
         * CCM-R5F - self-test
         */
        PHAROS_ESM_GR1_CCM_SF_TST = 31U ,

        /**
         * NHET2 - parity
         */
        PHAROS_ESM_GR1_NHET2_PAR = 34U ,

        /**
         * IOMM - Mux configuration error
         */
        PHAROS_ESM_GR1_IOMM_CONF_ERR = 37U ,

        /**
         * Power domain compare error
         */
        PHAROS_ESM_GR1_PWR_COMP_ERR = 38U ,

        /**
         * Power domain self-test error
         */
        PHAROS_ESM_GR1_PWR_SF_TST_ERR = 39U ,

        /**
         * eFuse farm – EFC error
         */
        PHAROS_ESM_GR1_EFUSE_EFC_ERR = 40U ,

        /**
         * eFuse farm - self-test error
         */
        PHAROS_ESM_GR1_EFUSE_SF_TST_ERR = 41U ,

        /**
         * PLL2 - slip
         */
        PHAROS_ESM_GR1_PLL2_SLIP = 42U ,

        /**
         * Ethernet Controller master interface
         */
        PHAROS_ESM_GR1_ETHER_INTER = 43U ,

        /**
         * Cortex-R5F Core - cache correctable error event
         */
        PHAROS_ESM_GR1_CACHE_CORR_EVT = 46U ,

        /**
         * ACP d-cache invalidate
         */
        PHAROS_ESM_GR1_ACP_DCACHE_INV = 47U ,

        /**
         * MibSPI2 - ECC uncorrectable error
         */
        PHAROS_ESM_GR1_SPI2_ECC_UNC = 49U ,

        /**
         * MibSPI4 - ECC uncorrectable error
         */
        PHAROS_ESM_GR1_SPI4_ECC_UNC = 50U ,

        /**
         * DCAN4 - ECC uncorrectable error
         */
        PHAROS_ESM_GR1_CAN4_ECC_UNC = 51U ,

        /**
         * CPU Interconnect Subsystem - Global error
         */
        PHAROS_ESM_GR1_CPU_INTER_ERR = 52U ,

        /**
         * CPU Interconnect Subsystem - Global Parity Error
         */
        PHAROS_ESM_GR1_CPU_INTER_PAR_ER = 53U ,

        /**
         * NHET1/2 - self-test error
         */
        PHAROS_ESM_GR1_NHET_SF_TEST_ERR = 54U ,

        /**
         * NMPU - EMAC MPU Error
         */
        PHAROS_ESM_GR1_EMAC_MPU_ERR = 55U ,

        /**
         * NMPU - PS_SCR_S MPU Error
         */
        PHAROS_ESM_GR1_MPU_ERR = 61U ,

        /**
         * DCC2 - error
         */
        PHAROS_ESM_GR1_DCC2_ERR = 62U ,

        /**
         * NMPU - DMA Port A MPU Error
         */
        PHAROS_ESM_GR1_DMA_A_MPU_ERR = 69U ,

        /**
         * DMA - Transaction Bus Parity Error
         */
        PHAROS_ESM_GR1_DMA_BUS_PAR_ERR = 70U ,

        /**
         * FlexRay TU RAM- ECC single bit error (TU_SBE_err)
         */
        PHAROS_ESM_GR1_FRAY_TU_ECC_SBE = 71U ,

        /**
         * FlexRay - ECC single bit error
         */
        PHAROS_ESM_GR1_FRAY_ECC_SBE = 72U ,

        /**
         * DCAN1 - ECC single bit error
         */
        PHAROS_ESM_GR1_CAN1_SBE = 73U ,

        /**
         * DCAN2 - ECC single bit error
         */
        PHAROS_ESM_GR1_CAN2_SBE = 74U ,

        /**
         * DCAN3 - ECC single bit error
         */
        PHAROS_ESM_GR1_CAN3_SBE = 75U ,

        /**
         * DCAN4 - ECC single bit error
         */
        PHAROS_ESM_GR1_CAN4_SBE = 76U ,

        /**
         * MIBSPI1 - ECC single bit error
         */
        PHAROS_ESM_GR1_SPI1_SBE = 77U ,

        /**
         * MIBSPI2 - ECC single bit error
         */
        PHAROS_ESM_GR1_SPI2_SBE = 78U ,

        /**
         * MIBSPI3 - ECC single bit error
         */
        PHAROS_ESM_GR1_SPI3_SBE = 79U ,

        /**
         * MIBSPI4 - ECC single bit error
         */
        PHAROS_ESM_GR1_SPI4_SBE = 80U ,

        /**
         * MIBSPI5 - ECC single bit error
         */
        PHAROS_ESM_GR1_SPI5_SBE = 81U ,

        /**
         * DMA - ECC single bit error
         */
        PHAROS_ESM_GR1_DMA_ECC_SBE = 82U ,

        /**
         * VIM - ECC single bit error
         */
        PHAROS_ESM_GR1_VIM_SBE = 83U ,

        /**
         * EMIF 64-bit Bridge I/F ECC uncorrectable error
         */
        PHAROS_ESM_GR1_EMIF_BR_ECC_UNC = 84U ,

        /**
         * EMIF 64-bit Bridge I/F ECC single bit error
         */
        PHAROS_ESM_GR1_EMIF_BR_ECC_SBE = 85U ,

        /**
         * DMA - Register Soft Error
         */
        PHAROS_ESM_GR1_DMA_SOFT_ERR = 88U ,

        /**
         * L2FMC - Register Soft Error
         */
        PHAROS_ESM_GR1_L2FMC_SOFT_ERR = 89U ,

        /**
         * SYS - Register Soft Error
         */
        PHAROS_ESM_GR1_SYS_SOFT_ERR = 90U ,

        /**
         * SCM - Time-out Error
         */
        PHAROS_ESM_GR1_SCM_TIMEOUT = 91U ,

        /**
         * CCM-R5F - Operating status
         */
        PHAROS_ESM_GR1_CPU_OP_STAT = 92U ,

    } PharosCpuEsmChannelGroup1;

    typedef enum
    {
        /**
         * CCM-R5F - CPU compare error
         */
        PHAROS_ESM_GR2_CPU_CMP_ERR = 2U ,

        /**
         * Cortex-R5F Core - All fatal bus error events. 
         * [Commonly caused by improper or incomplete ECC values in Flash.]
         */
        PHAROS_ESM_GR2_BUS_FATAL = 3U ,

        /**
         * L2RAMW - Uncorrectable error type B
         */
        PHAROS_ESM_GR2_L2RAMW_UNC_B = 7U ,

        /**
         * L2FMC - parity error
         *  - Mcmd parity error on Idle command
         *  - POM idle state parity error 
         *  - Port A/B Idle state parity error
         */
        PHAROS_ESM_GR2_L2FMC_PAR_ERR = 17U ,

        /**
         * L2FMC - double bit ECC error-error due to implicit OTP reads
         */
        PHAROS_ESM_GR2_L2FMC_ECC_DBE = 19U ,

        /**
         * EPC - Uncorrectable Error
         */
        PHAROS_ESM_GR2_EPC_UNC = 21U ,

        /**
         * RTI_WWD_NMI
         */
        PHAROS_ESM_GR2_RTI_WWD_NMI = 24U ,

        /**
         * CCM-R5F VIM compare error
         */
        PHAROS_ESM_GR2_VIM_CMP_ERR = 25U ,

        /**
         * CPU1 AXIM Bus Monitor failure
         */
        PHAROS_ESM_GR2_CPU1_BUS_MON_FAI = 26U ,

        /**
         * CCM-R5F - Power Domain monitor error
         */
        PHAROS_ESM_GR2_PWR_MON_ERR = 28U ,

    } PharosCpuEsmChannelGroup2;

    typedef enum
    {
        /**
         * eFuse Farm - autoload error
         */
        PHAROS_ESM_GR3_EFUSE_AULOAD_ERR = 1U ,

        /**
         * L2RAMW - double bit ECC uncorrectable error
         */
        PHAROS_ESM_GR3_L2RAMW_ECC_DBE = 3U ,

        /**
         * Cortex-R5F Core - All fatal events
         */
        PHAROS_ESM_GR3_FATAL_EVENTS = 9U ,

        /**
         * CPU Interconnect Subsystem - Diagnostic Error
         */
        PHAROS_ESM_GR3_CPU_INTER_DIAG = 12U ,

        /**
         * L2FMC - uncorrectable error due to:
         *  - address parity/internal parity error
         *  - address tag
         *  - internal switch time-out
         */
        PHAROS_ESM_GR3_L2FMC_UNC = 13U ,

        /**
         * L2RAMW - Uncorrectable error Type A
         */
        PHAROS_ESM_GR3_L2RAMW_UNC_A = 14U ,

        /**
         * L2RAMW - Address/Control parity error
         */
        PHAROS_ESM_GR3_L2RAMW_PAR_ERR = 15U ,

    } PharosCpuEsmChannelGroup3;


    /************************************************************
     * ESMEPSR register definitions                             *
     ************************************************************/
#define PHAROS_CPU_ESM_EPSR_ERR                     (0U << 0U)
#define PHAROS_CPU_ESM_EPSR_NO_ERR                  (1U << 0U)

    /************************************************************
     * ESMEKR register definitions                              *
     ************************************************************/
#define PHAROS_CPU_ESM_EKR_NORMAL                   (0x0U << 0U)
#define PHAROS_CPU_ESM_EKR_RESET                    (0x5U << 0U)
#define PHAROS_CPU_ESM_EKR_FORCE_ERR                (0xaU << 0U)


    /**
     * Compact the size of the I/O registers
     */
#pragma pack(push ,4)

    struct PharosCpuEsm
    {
        /**
         * ESM Enable ERROR Pin Action/Response Register 1 (ESMEEPAPR1)
         * 
         * Enable ERROR Pin Action/Response on Group 1 Channel[31:0].
         * 
         * Offset: 0x0
         */
        volatile uint32_t enableErrAction1;

        /**
         * ESM Disable ERROR Pin Action/Response Register 1 (ESMDEPAPR1)
         * 
         * Disable ERROR Pin Action/Response on Group 1 Channel[31:0].
         * 
         * Offset: 0x4
         */
        volatile uint32_t disableErrorAction1Ch1;

        /**
         * ESM Interrupt Enable Set/Status Register 1 (ESMIESR1)
         * 
         * Enables interrupts for Group 1 Channel[31:0].
         * 
         * Offset: 0x8
         */
        volatile uint32_t interruptEnableSet1Ch1;

        /**
         * ESM Interrupt Enable Clear/Status Register 1 (ESMIECR1)
         * 
         * Disables interrupts for Group 1 Channel[31:0].
         * 
         * Offset: 0xc
         */
        volatile uint32_t interruptEnableClear1Ch1;

        /**
         * ESM Interrupt Level Set/Status Register 1 (ESMILSR1)
         * 
         * Set Interrupt Priority to high priority (1 is high priority, 0 is low priority) for Group 1 Channel[31:0].
         * 
         * Offset: 0x10
         */
        volatile uint32_t interruptLevelSet1Ch1;

        /**
         * ESM Interrupt Level Clear/Status Register 1 (ESMILCR1)
         * 
         * Clears Interrupt Priority from high priority for Group 1 Channel[31:0]
         * 
         * Offset: 0x14
         */
        volatile uint32_t interrruptLevelClear1Ch1;

        /**
         * ESM Status Register 1 (ESMSR1)
         * 
         * Error Status Flag for Group 1. Provides status information on a pending error.
         * 
         * Note that the status register will get updated if an error condition occurs, regardless if the 
         * corresponding interrupt enable flag is set or not.
         * 
         * Offset: 0x18
         */
        volatile uint32_t status1Ch1;

        /**
         * ESM Status Register 2 (ESMSR2)
         * 
         * Error Status Flag for Group 2. Provides status information on a pending error.
         * 
         * Note that the status register will get updated if an error condition occurs, regardless if the 
         * corresponding interrupt enable flag is set or not.
         * 
         * Offset: 0x1c
         */
        volatile uint32_t status2;

        /**
         * ESM Status Register 3 (ESMSR3)
         * 
         * Error Status Flag for Group 3. Provides status information on a pending error.
         * 
         * Note that the status register will get updated if an error condition occurs, regardless if the 
         * corresponding interrupt enable flag is set or not.
         * 
         * Offset: 0x20
         */
        volatile uint32_t status3;

        /**
         * ESM ERROR Pin Status Register (ESMEPSR)
         * 
         * Provides status information for the ERROR Pin
         * 
         * Offset: 0x24
         */
        volatile uint32_t errorPinStatus;

        /**
         * ESM Interrupt Offset High Register (ESMIOFFHR)
         * 
         * This vector gives the channel number of the highest pending interrupt request for the high level 
         * interrupt line. For Group 1 and 2
         * 
         * Offset: 0x28
         */
        volatile uint32_t interruptHighOffset;

        /**
         * ESM Interrupt Offset Low Register (ESMIOFFLR)
         * 
         * This vector gives the channel number of the highest pending interrupt request for the low level 
         * interrupt line. For Group 1 (group 2 are fixed to high level interrupt)
         * 
         * Offset: 0x2c
         */
        volatile uint32_t interruptLowOffset;

        /**
         * ESM Low-Time Counter Register (ESMLTCR)
         * 
         * ERROR Pin Low-Time Counter
         * 
         * Offset: 0x30
         */
        volatile uint32_t errorLowTimeCounter;

        /**
         * ESM Low-Time Counter Preload Register (ESMLTCPR)
         * 
         * ERROR Pin Low-Time Counter Pre-load Value
         * 
         * Offset: 0x34
         */
        volatile uint32_t errorPinLowTimeCounterLoad;

        /**
         * ESM Error Key Register (ESMEKR)
         * 
         * The key to reset the ERROR pin or to force an error on the ERROR pin.
         * 
         * Offset: 0x38
         */
        volatile uint32_t errorKey;

        /**
         * ESM Status Shadow Register 2 (ESMSSR2)
         * 
         * Shadow register for status information on pending error for Group 2
         * 
         * Offset: 0x3c
         */
        volatile uint32_t statusShadow2;

        /**
         * ESM Influence ERROR Pin Set/Status Register 4 (ESMIEPSR4)
         * 
         * Set/check Influence on ERROR Pin for Group 1 Channel[63:32]
         * 
         * Offset: 0x40
         */
        volatile uint32_t errorInfluencePinSet1Ch2;

        /**
         * ESM Influence ERROR Pin Clear/Status Register 4 (ESMIEPCR4)
         * 
         * Clears Influence on ERROR Pin for Group 1 Channel[63:32]
         * 
         * Offset: 0x44
         */
        volatile uint32_t errorInfluencePinClear1Ch2;

        /**
         * ESM Interrupt Enable Set/Status Register 4 (ESMIESR4)
         * 
         * Interrupt enable for Group 1 Channel[63:32]
         * 
         * Offset: 0x48
         */
        volatile uint32_t interruptEnableSet1Ch2;

        /**
         * ESM Interrupt Enable Clear/Status Register 4 (ESMIECR4)
         * 
         * Interrupt disable for Group 1 Channel[63:32]
         * 
         * Offset: 0x4c
         */
        volatile uint32_t interruptEnableClear1Ch2;

        /**
         * ESM Interrupt Level Set/Status Register 4 (ESMILSR4)
         * 
         * Set the interrupt level to high (1 is high priority, 0 is low priority) for Group1 Channel[63:32]
         * 
         * Offset: 0x50
         */
        volatile uint32_t interruptLevelSet1Ch2;

        /**
         * ESM Interrupt Level Clear/Status Register 4 (ESMILCR4)
         * 
         * Clears Interrupt Priority from high priority for Group 1 Channel[63:32]
         * 
         * Offset: 0x54
         */
        volatile uint32_t interruptLevelClear1Ch2;

        /**
         * ESM Status Register 4 (ESMSR4)
         * 
         * Provides status information on a pending error for Group1 Channel[63:32].
         * 
         * Offset: 0x58
         */
        volatile uint32_t status1Ch2;

        /**
         * Reserved
         * 
         * Offset: 0x5c
         */
        volatile uint32_t reserved3[9U];

        /**
         * ESM Influence ERROR Pin Clear/Status Register 7 (ESMIEPCR7)
         * 
         * Set Influence on ERROR Pin for Group1 Channel[95:64]
         * 
         * Offset: 0x80
         */
        volatile uint32_t errorInfluencePinSet1Ch3;

        /**
         * ESM Influence ERROR Pin Clear/Status Register 7 (ESMIEPCR7)
         * 
         * Clear Influence on ERROR Pin for Group1 Channel[95:64]
         * 
         * Offset: 0x84
         */
        volatile uint32_t errorInfluencePinClear1Ch3;


        /**
         * ESM Interrupt Enable Set/Status Register 7 (ESMIESR7)
         * 
         * Interrupt enable for Group 1 Channel[95:64]
         * 
         * Offset: 0x88
         */
        volatile uint32_t interruptEnableSet1Ch3;

        /**
         * ESM Interrupt Enable Clear/Status Register 7 (ESMIECR7)
         * 
         * Interrupt disable for Group 1 Channel[95:64]
         * 
         * Offset: 0x8c
         */
        volatile uint32_t interruptEnableClear1Ch3;

        /**
         * ESM Interrupt Level Set/Status Register 7 (ESMILSR7)
         * 
         * Set the interrupt level to high (1 is high priority, 0 is low priority) for Group1 Channel[95:64]
         * 
         * Offset: 0x90
         */
        volatile uint32_t interruptLevelSet1Ch3;

        /**
         * ESM Interrupt Level Clear/Status Register 7 (ESMILCR7)
         * 
         * Clears Interrupt Priority from high priority for Group 1 Channel[95:64]
         * 
         * Offset: 0x94
         */
        volatile uint32_t interruptLevelClear1Ch3;


        /**
         * ESM Status Register 7 (ESMSR7)
         * 
         * Error Status Flag. Provides status information on a pending error for Group1 Channel[95:64]
         * 
         * Offset: 0x98
         */
        volatile uint32_t status1Ch3;
    };


    /**
     * restore the previous pack definition
     */
#pragma pack(pop)


    void pharosEsmInitialize(void);


    /**
     * Function that will be invoked after an ESM error has been detected.
     * 
     * The application can override this default function
     */
    void pharosEsmClearError(uint32_t fiqIndex);

#ifdef __cplusplus
}
#endif

#endif /* ESM_H */
