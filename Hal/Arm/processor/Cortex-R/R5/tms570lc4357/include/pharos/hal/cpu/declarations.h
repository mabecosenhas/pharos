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


#ifndef PHAROS_CPU_DECLARATIONS_H
#define	PHAROS_CPU_DECLARATIONS_H

#ifdef	__cplusplus
extern "C"
{
#endif

#include <pharos/hal/hal.h>
#include <pharos/declarations.h>

    typedef struct PharosCpuEsm PharosCpuEsm , *ptrPharosCpuEsm;
    typedef struct PharosCpuSystemPrimary PharosCpuSystemPrimary , *ptrPharosCpuSystemPrimary;
    typedef struct PharosCpuSystemSecundary PharosCpuSystemSecundary , *ptrPharosCpuSystemSecundardy;
    typedef struct PharosCpuPcr PharosCpuPcr , *ptrPharosCpuPcr;
    typedef struct PharosCpuIomm PharosCpuIomm , *ptrPharosCpuIomm;
    typedef struct PharosCpuL2Fmc PharosCpuL2Fmc , *ptrPharosCpuL2fmc;
    typedef struct PharosCpuSci PharosCpuSci , *ptrPharosCpuSci;
    typedef struct PharosCpuRti PharosCpuRti , *ptrPharosCpuRti;


    /****************************************************************************
     * Errors definitions for this CPU                                          *
     ****************************************************************************/

#define PHAROS_ERROR_CPU_RST_OSC            (PHAROS_ERR_CPU_START + 0U)
#define PHAROS_ERROR_CPU_RST_WATCHDOG       (PHAROS_ERR_CPU_START + 1U)
#define PHAROS_ERROR_CPU_RST_WATCHDOG2      (PHAROS_ERR_CPU_START + 2U)
#define PHAROS_ERROR_CPU_RST_INTERCON       (PHAROS_ERR_CPU_START + 3U)
#define PHAROS_ERROR_CPU_RST_SW             (PHAROS_ERR_CPU_START + 4U)

    /**
     * Single Bit Error found by the ECC on the VIM Interrupt Vector Table and was corrected. 
     * This is not a fatal error since the ECC could correct it.
     */
#define PHAROS_ERROR_CPU_VIM_ECC_SBE        (PHAROS_ERR_CPU_START + 5U)

    /**
     * Double Bit Error found by the ECC on the VIM Interrupt Vector Table and could not be corrected. 
     * This is not a fatal error since the the VIM Interrupt Vector Table is not used.
     */
#define PHAROS_ERROR_CPU_VIM_ECC_DBE        (PHAROS_ERR_CPU_START + 6U)

    /**
     * Error on the ESM
     */
#define PHAROS_ERROR_CPU_VIM_ESM            (PHAROS_ERR_CPU_START + 7U)
    

    /****************************************************************************
     * Fatal errors definitions for this CPU                                    *
     ****************************************************************************/

    /**
     * Invalid interrupt number
     * 
     * The interrupt number configured is not valid
     */
#define PHAROS_FATAL_CPU_ISR_INV_NUM        (PHAROS_FATAL_CPU_START + 0U)

    /**
     * Invalid interrupt channel
     * 
     * the interrupt channel is not valid (does not belong to PharosInterruptChannel)
     */
#define PHAROS_FATAL_CPU_ISR_INV_CHA        (PHAROS_FATAL_CPU_START + 1U)


    typedef enum
    {
        /**
         * ESM high
         */
        PHAROS_CPU_ISR_ESM_HI = 0U ,

        /**
         * RTI compare 0
         */
        PHAROS_CPU_ISR_RTI0_CMP = 2U ,

        /**
         * RTI compare 1
         */
        PHAROS_CPU_ISR_RTI1_CMP = 3U ,

        /**
         * RTI compare 2
         */
        PHAROS_CPU_ISR_RTI2_CMP = 4U ,

        /**
         * RTI compare 3
         */
        PHAROS_CPU_ISR_RTI3_CMP = 5U ,

        /**
         * RTI overflow 0
         */
        PHAROS_CPU_ISR_RTI0_OVER = 6U ,

        /**
         * RTI overflow 1 
         */
        PHAROS_CPU_ISR_RTI1_OVER = 7U ,

        /**
         * RTI timebase 1 
         */
        PHAROS_CPU_ISR_RTI1_TIMEBASE = 8U ,

        /**
         * GIO High
         */
        PHAROS_CPU_ISR_GIO_HI = 9U ,

        /**
         * HET1 High
         */
        PHAROS_CPU_ISR_HET1_HI = 10U ,

        /**
         * HET1 TU High
         */
        PHAROS_CPU_ISR_HET1_TU_HI = 11U ,

        /**
         * MIBSPI1 High
         */
        PHAROS_CPU_ISR_SPI1_HI = 12U ,

        /**
         * LIN1 High
         */
        PHAROS_CPU_ISR_LIN1_HI = 13U ,

        /**
         * ADC1 Event
         */
        PHAROS_CPU_ISR_ADC1_EVT = 14U ,

        /**
         * ADC1 Group 1
         */
        PHAROS_CPU_ISR_ADC1_GR1 = 15U ,

        /**
         * CAN1 High
         */
        PHAROS_CPU_ISR_CAN1_HI = 16U ,

        /**
         * MIBSPI2 High
         */
        PHAROS_CPU_ISR_SPI2_HI = 17U ,

        /**
         * FlexRay High
         */
        PHAROS_CPU_ISR_FLEXRAY_HI = 18U ,

        /**
         * CRC 1
         */
        PHAROS_CPU_ISR_CRC1 = 19U ,

        /**
         * ESM Low
         */
        PHAROS_CPU_ISR_ESM_LOW = 20U ,

        /**
         * SSI
         */
        PHAROS_CPU_ISR_SSI = 21U ,

        /**
         * PMU TAP
         */
        PHAROS_CPU_ISR_PMU_TAP = 22U ,

        /**
         * GIO Low
         */
        PHAROS_CPU_ISR_GIO = 23U ,

        /**
         * HET1 Low
         */
        PHAROS_CPU_ISR_HET1_LOW = 24U ,

        /**
         * HET1 TU1 Low
         */
        PHAROS_CPU_ISR_HET1_TU1_LOW = 25U ,

        /**
         * MIBSPI1 Low
         */
        PHAROS_CPU_ISR_SPI1_LOW = 26U ,

        /**
         * LIN1 Low
         */
        PHAROS_CPU_ISR_LIN1_LOW = 27U ,

        /**
         * ADC1 Group 2
         */
        PHAROS_CPU_ISR_ADC1_GR2 = 28U ,

        /**
         * CAN1 Low
         */
        PHAROS_CPU_ISR_CAN1_LOW = 29U ,

        /**
         * MIBSPI2 Low
         */
        PHAROS_CPU_ISR_SPI2_LOW = 30U ,

        /**
         * ADC1 Mag
         */
        PHAROS_CPU_ISR_ADC1_MAG = 31U ,

        /**
         * FlexRay Low
         */
        PHAROS_CPU_ISR_FLEXRAY_LOW = 32U ,

        /**
         * DMA FTCA
         */
        PHAROS_CPU_ISR_DMA_FTCA = 33U ,

        /**
         * DMA LFSA
         */
        PHAROS_CPU_ISR_DMA_LFSA = 34U ,

        /**
         * CAN2 High
         */
        PHAROS_CPU_ISR_CAN2_HI = 35U ,

        /** 
         * DMM High
         */
        PHAROS_CPU_ISR_DMM_HI = 36U ,

        /**
         * MIBSPI3 High
         */
        PHAROS_CPU_ISR_SPI3_HI = 37U ,

        /**
         * MIBSPI3 Low
         */
        PHAROS_CPU_ISR_SPI3_LOW = 38U ,

        /**
         * DMA HBCA
         */
        PHAROS_CPU_ISR_DMA_HBCA = 39U ,

        /**
         * DMA HTCA
         */
        PHAROS_CPU_ISR_DMA_HTCA = 40U ,

        /**
         * AEMIFINT
         */
        PHAROS_CPU_ISR_AEMIFINT = 41U ,

        /**
         * CAN2 Low
         */
        PHAROS_CPU_ISR_CAN2_LOW = 42U ,

        /**
         * DMM Low
         */
        PHAROS_CPU_ISR_DMM_LOW = 43U ,

        /**
         * CAN1 IF3
         */
        PHAROS_CPU_ISR_CAN_IF3 = 44U ,

        /**
         * CAN3 High
         */
        PHAROS_CPU_ISR_CAN3_HI = 45U ,

        /**
         * CAN2 IF3
         */
        PHAROS_CPU_ISR_IF3 = 46U ,

        /**
         * FPU
         */
        PHAROS_CPU_ISR_FPU = 47U ,

        /**
         * FlexRay TU
         */
        PHAROS_CPU_ISR_FLEXRAY_TU = 48U ,

        /**
         * MIBSPI4 High
         */
        PHAROS_CPU_ISR_SPI4_HI = 49U ,

        /**
         * ADC2 High
         */
        PHAROS_CPU_ISR_ADC2_HI = 50U ,

        /**
         * ADC2 Group 1
         */
        PHAROS_CPU_ISR_ADC2_GR1 = 51U ,

        /**
         * FlexRay TOC
         */
        PHAROS_CPU_ISR_FLEXRAY_TOC = 52U ,

        /**
         * MIBSPI5 high
         */
        PHAROS_CPU_ISR_SPI5_HI = 53U ,

        /**
         * MIBSPI4 Low
         */
        PHAROS_CPU_ISR_SPI4_LOW = 54U ,

        /**
         * CAN3 Low
         */
        PHAROS_CPU_ISR_CAN3_LOW = 55U ,

        /**
         * MIBSPI5 Low
         */
        PHAROS_CPU_ISR_SPI5_LOW = 56U ,

        /**
         * ADC2 Group 2
         */
        PHAROS_CPU_ISR_ADC2_GR2 = 57U ,

        /**
         * FlexRay TU Error
         */
        PHAROS_CPU_ISR_FLEXRAY_TU_ERR = 58U ,

        /**
         * ADC2 Mag
         */
        PHAROS_CPU_ISR_ADC2_MAG = 59U ,

        /**
         * CAN3 IF3
         */
        PHAROS_CPU_ISR_CAN3_IF3 = 60U ,

        /**
         * FSM DONE
         */
        PHAROS_CPU_ISR_FSM = 61U ,

        /**
         * FlexRay T1C
         */
        PHAROS_CPU_ISR_FLEXRAY_T1C = 62U ,

        /**
         * HET2 level 0
         */
        PHAROS_CPU_ISR_HET2_L0 = 63U ,

        /**
         * SCI3 High
         */
        PHAROS_CPU_ISR_SCI3_HI = 64U ,

        /**
         * HET TU2 level 0
         */
        PHAROS_CPU_ISR_HET_TU2_L0 = 65U ,

        /**
         * I2C1 
         */
        PHAROS_CPU_ISR_I2C1 = 66U ,

        /**
         * HET2 level 1
         */
        PHAROS_CPU_ISR_HET2_L1 = 73U ,

        /**
         * SCI3 low
         */
        PHAROS_CPU_ISR_SCI3_LOW = 74U ,

        /**
         * HET TU2 level 1
         */
        PHAROS_CPU_ISR_HET_TU2_L1 = 75U ,

        /**
         * C0 MISC PULSE
         */
        PHAROS_CPU_ISR_C0_MIST_PULSE = 76U ,

        /**
         * C0 TX PULSE
         */
        PHAROS_CPU_ISR_C0_TX_PULSE = 77U ,

        /**
         * C0 THRESHOLD PULSE
         */
        PHAROS_CPU_ISR_C0_THRSH_PULSE = 78U ,

        /**
         * C0 RX PULSE
         */
        PHAROS_CPU_ISR_C0_RX_PULSE = 79U ,

        /**
         * HWAG1 High
         */
        PHAROS_CPU_ISR_HWAG1_HI = 80U ,

        /**
         * HWAG2 High
         */
        PHAROS_CPU_ISR_HWAG2_HI = 81U ,

        /**
         * DCC1 done
         */
        PHAROS_CPU_ISR_DCC1 = 82U ,

        /**
         * DCC2 done
         */
        PHAROS_CPU_ISR_DCC2 = 83U ,

        /**
         * PBIST done
         */
        PHAROS_CPU_ISR_PBIST = 85U ,

        /**
         * IMM 
         */
        PHAROS_CPU_ISR_IMM = 86U ,

        /**
         * IMM interrupt 1
         */
        PHAROS_CPU_ISR_IMM1 = 87U ,

        /**
         * HWAG1 low
         */
        PHAROS_CPU_ISR_HWAG1_LOW = 88U ,

        /**
         * HWAG2 low
         */
        PHAROS_CPU_ISR_HWAG2_LOW = 89U ,

        /**
         * eTPWM1
         */
        PHAROS_CPU_ISR_ETPWM = 90U ,

        /**
         * eTPWM1 trip zone
         */
        PHAROS_CPU_ISR_ETPWM1_TZ = 91U ,

        /**
         * eTPWM 2
         */
        PHAROS_CPU_ISR_ETPWM2 = 92U ,

        /**
         * eTPWM 2 trip zone
         */
        PHAROS_CPU_ISR_ETPWM2_TZ = 93U ,

        /**
         * eTPWM 3
         */
        PHAROS_CPU_ISR_ETPWM3 = 94U ,

        /**
         * eTPWM 3 trip zone
         */
        PHAROS_CPU_ISR_ETPWM3_TZ = 95U ,

        /**
         * eTPWM 4
         */
        PHAROS_CPU_ISR_ETPWM4 = 96U ,

        /**
         * eTPWM 4 trip zone
         */
        PHAROS_CPU_ISR_ETPWM4_TZ = 97U ,

        /**
         * eTPWM 5
         */
        PHAROS_CPU_ISR_ETPWM5 = 98U ,

        /**
         * eTPWM 5 trip zone
         */
        PHAROS_CPU_ISR_ETPWM5_TZ = 99U ,

        /**
         * eTPWM 6
         */
        PHAROS_CPU_ISR_ETPWM6 = 100U ,

        /**
         * eTPWM 6 trip zone
         */
        PHAROS_CPU_ISR_ETPWM6_TZ = 101U ,

        /**
         * eTPWM 7
         */
        PHAROS_CPU_ISR_ETPWM7 = 102U ,

        /**
         * eTPWM 7 trip zone
         */
        PHAROS_CPU_ISR_ETPWM7_TZ = 103U ,

        /**
         * eCAP1
         */
        PHAROS_CPU_ISR_ECAP1 = 104U ,

        /**
         * eCAP2
         */
        PHAROS_CPU_ISR_ECAP2 = 105U ,

        /**
         * eCAP3
         */
        PHAROS_CPU_ISR_ECAP3 = 106U ,

        /**
         * eCAP4
         */
        PHAROS_CPU_ISR_ECAP4 = 107U ,

        /**
         * eCAP5
         */
        PHAROS_CPU_ISR_ECAP5 = 108U ,

        /**
         * eCAP6
         */
        PHAROS_CPU_ISR_ECAP6 = 109U ,

        /**
         * eQEP1
         */
        PHAROS_CPU_ISR_EQEP1 = 110U ,

        /**
         * eQEP2
         */
        PHAROS_CPU_ISR_EQEP2 = 111U ,

        /**
         * CAN4 High
         */
        PHAROS_CPU_ISR_CAN_HI = 113U ,

        /**
         * I2C2
         */
        PHAROS_CPU_ISR_I2C2 = 114U ,

        /**
         * LIN2
         */
        PHAROS_CPU_ISR_LIN2 = 115U ,

        /**
         * SCI4 High
         */
        PHAROS_CPU_ISR_SCI4_HI = 116U ,

        /**
         * CAN4 Low
         */
        PHAROS_CPU_ISR_CAN4_LOW = 117U ,

        /**
         * LIN2 Low
         */
        PHAROS_CPU_ISR_LIN2_LOW = 118U ,

        /**
         * SCI4 Low
         */
        PHAROS_CPU_ISR_SCI4_LOW = 119U ,

        /**
         * CAN4 IF3
         */
        PHAROS_CPU_ISR_CAN4_IF3 = 120U ,

        /**
         * CRC 2
         */
        PHAROS_CPU_ISR_CRC2 = 121U ,

        /**
         * EPC FIFO/CAM full
         */
        PHAROS_CPU_ISR_EPC_FULL = 124U ,

        /**
         * Phantom interrupt with lowest priority
         */
        PHAROS_CPU_ISR_PHANTOM = 126U ,

    } PharosInterruptNumber;


    /**
     * Channel
     */
    typedef enum
    {
        /**
         * Highest priority channel
         */
        PHAROS_INTERRUPT_CHANNEL_0 = 0 ,
        PHAROS_INTERRUPT_CHANNEL_1 ,
        PHAROS_INTERRUPT_CHANNEL_2 ,
        PHAROS_INTERRUPT_CHANNEL_3 ,
        PHAROS_INTERRUPT_CHANNEL_4 ,
        PHAROS_INTERRUPT_CHANNEL_5 ,
        PHAROS_INTERRUPT_CHANNEL_6 ,
        PHAROS_INTERRUPT_CHANNEL_7 ,
        PHAROS_INTERRUPT_CHANNEL_8 ,
        PHAROS_INTERRUPT_CHANNEL_9 ,
        PHAROS_INTERRUPT_CHANNEL_10 ,
        PHAROS_INTERRUPT_CHANNEL_11 ,
        PHAROS_INTERRUPT_CHANNEL_12 ,
        PHAROS_INTERRUPT_CHANNEL_13 ,
        PHAROS_INTERRUPT_CHANNEL_14 ,
        PHAROS_INTERRUPT_CHANNEL_15 ,
        PHAROS_INTERRUPT_CHANNEL_16 ,
        PHAROS_INTERRUPT_CHANNEL_17 ,
        PHAROS_INTERRUPT_CHANNEL_18 ,
        PHAROS_INTERRUPT_CHANNEL_19 ,
        PHAROS_INTERRUPT_CHANNEL_20 ,
        PHAROS_INTERRUPT_CHANNEL_21 ,
        PHAROS_INTERRUPT_CHANNEL_22 ,
        PHAROS_INTERRUPT_CHANNEL_23 ,
        PHAROS_INTERRUPT_CHANNEL_24 ,
        PHAROS_INTERRUPT_CHANNEL_25 ,
        PHAROS_INTERRUPT_CHANNEL_26 ,
        PHAROS_INTERRUPT_CHANNEL_27 ,
        PHAROS_INTERRUPT_CHANNEL_28 ,
        PHAROS_INTERRUPT_CHANNEL_29 ,
        PHAROS_INTERRUPT_CHANNEL_30 ,
        PHAROS_INTERRUPT_CHANNEL_31 ,
        PHAROS_INTERRUPT_CHANNEL_32 ,
        PHAROS_INTERRUPT_CHANNEL_33 ,
        PHAROS_INTERRUPT_CHANNEL_34 ,
        PHAROS_INTERRUPT_CHANNEL_35 ,
        PHAROS_INTERRUPT_CHANNEL_36 ,
        PHAROS_INTERRUPT_CHANNEL_37 ,
        PHAROS_INTERRUPT_CHANNEL_38 ,
        PHAROS_INTERRUPT_CHANNEL_39 ,
        PHAROS_INTERRUPT_CHANNEL_40 ,
        PHAROS_INTERRUPT_CHANNEL_41 ,
        PHAROS_INTERRUPT_CHANNEL_42 ,
        PHAROS_INTERRUPT_CHANNEL_43 ,
        PHAROS_INTERRUPT_CHANNEL_44 ,
        PHAROS_INTERRUPT_CHANNEL_45 ,
        PHAROS_INTERRUPT_CHANNEL_46 ,
        PHAROS_INTERRUPT_CHANNEL_47 ,
        PHAROS_INTERRUPT_CHANNEL_48 ,
        PHAROS_INTERRUPT_CHANNEL_49 ,
        PHAROS_INTERRUPT_CHANNEL_50 ,
        PHAROS_INTERRUPT_CHANNEL_51 ,
        PHAROS_INTERRUPT_CHANNEL_52 ,
        PHAROS_INTERRUPT_CHANNEL_53 ,
        PHAROS_INTERRUPT_CHANNEL_54 ,
        PHAROS_INTERRUPT_CHANNEL_55 ,
        PHAROS_INTERRUPT_CHANNEL_56 ,
        PHAROS_INTERRUPT_CHANNEL_57 ,
        PHAROS_INTERRUPT_CHANNEL_58 ,
        PHAROS_INTERRUPT_CHANNEL_59 ,
        PHAROS_INTERRUPT_CHANNEL_60 ,
        PHAROS_INTERRUPT_CHANNEL_61 ,
        PHAROS_INTERRUPT_CHANNEL_62 ,
        PHAROS_INTERRUPT_CHANNEL_63 ,
        PHAROS_INTERRUPT_CHANNEL_64 ,
        PHAROS_INTERRUPT_CHANNEL_65 ,
        PHAROS_INTERRUPT_CHANNEL_66 ,
        PHAROS_INTERRUPT_CHANNEL_67 ,
        PHAROS_INTERRUPT_CHANNEL_68 ,
        PHAROS_INTERRUPT_CHANNEL_69 ,
        PHAROS_INTERRUPT_CHANNEL_70 ,
        PHAROS_INTERRUPT_CHANNEL_71 ,
        PHAROS_INTERRUPT_CHANNEL_72 ,
        PHAROS_INTERRUPT_CHANNEL_73 ,
        PHAROS_INTERRUPT_CHANNEL_74 ,
        PHAROS_INTERRUPT_CHANNEL_75 ,
        PHAROS_INTERRUPT_CHANNEL_76 ,
        PHAROS_INTERRUPT_CHANNEL_77 ,
        PHAROS_INTERRUPT_CHANNEL_78 ,
        PHAROS_INTERRUPT_CHANNEL_79 ,
        PHAROS_INTERRUPT_CHANNEL_80 ,
        PHAROS_INTERRUPT_CHANNEL_81 ,
        PHAROS_INTERRUPT_CHANNEL_82 ,
        PHAROS_INTERRUPT_CHANNEL_83 ,
        PHAROS_INTERRUPT_CHANNEL_84 ,
        PHAROS_INTERRUPT_CHANNEL_85 ,
        PHAROS_INTERRUPT_CHANNEL_86 ,
        PHAROS_INTERRUPT_CHANNEL_87 ,
        PHAROS_INTERRUPT_CHANNEL_88 ,
        PHAROS_INTERRUPT_CHANNEL_89 ,
        PHAROS_INTERRUPT_CHANNEL_90 ,
        PHAROS_INTERRUPT_CHANNEL_91 ,
        PHAROS_INTERRUPT_CHANNEL_92 ,
        PHAROS_INTERRUPT_CHANNEL_93 ,
        PHAROS_INTERRUPT_CHANNEL_94 ,
        PHAROS_INTERRUPT_CHANNEL_95 ,
        PHAROS_INTERRUPT_CHANNEL_96 ,
        PHAROS_INTERRUPT_CHANNEL_97 ,
        PHAROS_INTERRUPT_CHANNEL_98 ,
        PHAROS_INTERRUPT_CHANNEL_99 ,
        PHAROS_INTERRUPT_CHANNEL_100 ,
        PHAROS_INTERRUPT_CHANNEL_101 ,
        PHAROS_INTERRUPT_CHANNEL_102 ,
        PHAROS_INTERRUPT_CHANNEL_103 ,
        PHAROS_INTERRUPT_CHANNEL_104 ,
        PHAROS_INTERRUPT_CHANNEL_105 ,
        PHAROS_INTERRUPT_CHANNEL_106 ,
        PHAROS_INTERRUPT_CHANNEL_107 ,
        PHAROS_INTERRUPT_CHANNEL_108 ,
        PHAROS_INTERRUPT_CHANNEL_109 ,
        PHAROS_INTERRUPT_CHANNEL_110 ,
        PHAROS_INTERRUPT_CHANNEL_111 ,
        PHAROS_INTERRUPT_CHANNEL_112 ,
        PHAROS_INTERRUPT_CHANNEL_113 ,
        PHAROS_INTERRUPT_CHANNEL_114 ,
        PHAROS_INTERRUPT_CHANNEL_115 ,
        PHAROS_INTERRUPT_CHANNEL_116 ,
        PHAROS_INTERRUPT_CHANNEL_117 ,
        PHAROS_INTERRUPT_CHANNEL_118 ,
        PHAROS_INTERRUPT_CHANNEL_119 ,
        PHAROS_INTERRUPT_CHANNEL_120 ,
        PHAROS_INTERRUPT_CHANNEL_121 ,
        PHAROS_INTERRUPT_CHANNEL_122 ,
        PHAROS_INTERRUPT_CHANNEL_123 ,
        PHAROS_INTERRUPT_CHANNEL_124 ,
        PHAROS_INTERRUPT_CHANNEL_125 ,
        PHAROS_INTERRUPT_CHANNEL_126 ,
        PHAROS_INTERRUPT_CHANNEL_MAX = PHAROS_INTERRUPT_CHANNEL_126 ,
    } PharosInterruptChannel , *ptrPharosInterruptChannel;


#ifdef	__cplusplus
}
#endif

#endif	/* DECLARATIONS_H */
