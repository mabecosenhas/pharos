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


#ifndef PHAROS_FMC_H
#define	PHAROS_FMC_H

#include <pharos/hal/cpu/declarations.h>
#include <pharos/hal/cpu/conf.h>


#define PHAROS_CPU_FMC          ((ptrPharosCpuFmc) PHAROS_CPU_FMC_ADDRESS)

#define PHAROS_CPU_FMC_SDCR_NC_8        (0b00 << 0)
#define PHAROS_CPU_FMC_SDCR_NR_11       (0b00 << 2)
#define PHAROS_CPU_FMC_SDCR_NR_12       (0b01 << 2)

#define PHAROS_CPU_FMC_SDCR_MWID_16     (0b01 << 4)

#define PHAROS_CPU_FMC_SDCR_NB_4        (0b1 << 6)
#define PHAROS_CPU_FMC_SDCR_CAS_1       (0b01 << 7)
#define PHAROS_CPU_FMC_SDCR_CAS_3       (0b11 << 7)

#define PHAROS_CPU_FMC_SDCR_WP_IGN      (0b1 << 9)
#define PHAROS_CPU_FMC_SDCR_WP_ALL      (0b0 << 9)
#define PHAROS_CPU_FMC_SDCR_SDCLK_0     (0b00 << 10)
#define PHAROS_CPU_FMC_SDCR_SDCLK_3     (0b11 << 10)
#define PHAROS_CPU_FMC_SDCR_RBURST_DIS  (0b0 << 12)
#define PHAROS_CPU_FMC_SDCR_RPIPE_0     (0b00 << 13)
#define PHAROS_CPU_FMC_SDCR_RPIPE_1     (0b01 << 13)


#define PHAROS_CPU_FMC_SDCMR_CTB1               (1U << 4U)
#define PHAROS_CPU_FMC_SDCMR_CTB2               (1U << 3U)


#define PHAROS_CPU_FMC_SDCMR_MODE_NORMAL        (0U << 0U)
#define PHAROS_CPU_FMC_SDCMR_MODE_CLOCK_ENABLE  (1U << 0U)
#define PHAROS_CPU_FMC_SDCMR_MODE_PALL          (2U << 0U)
#define PHAROS_CPU_FMC_SDCMR_MODE_AUTO_REFRESH  (3U << 0U)
#define PHAROS_CPU_FMC_SDCMR_MODE_LOAD_MODE     (4U << 0U)
#define PHAROS_CPU_FMC_SDCMR_MODE_SELF_REFRESH  (5U << 0U)
#define PHAROS_CPU_FMC_SDCMR_MODE_POWER_DOWN    (6U << 0U)

#define PHAROS_CPUFMC_SDCMR_NRFS(x)             (x << 5)
#define PHAROS_CPUFMC_SDCMR_MRD(x)              (x << 9)

#define PHAROS_CPU_FMC_SDSR_BUSY                (1U << 5U)


#define PHAROS_CPU_FMC_SDCR_NC_MASK             (0b11U << 0U)
#define PHAROS_CPU_FMC_SDCR_NR_MASK             (0b11U << 2U)
#define PHAROS_CPU_FMC_SDCR_MWID_MASK           (0b11U << 4U)
#define PHAROS_CPU_FMC_SDCR_NB_MASK             (0b1U  << 6U)
#define PHAROS_CPU_FMC_SDCR_CAS_MASK            (0b11U << 7U)
#define PHAROS_CPU_FMC_SDCR_WP_MASK             (0b1U  << 9U)
#define PHAROS_CPU_FMC_SDCR_SDCLK_MASK          (0b11U << 10U)
#define PHAROS_CPU_FMC_SDCR_RBURST_MASK         (0b1U  << 12U)
#define PHAROS_CPU_FMC_SDCR_RPIPE_MASK          (0b11U << 13U)


#define PHAROS_CPU_FMC_SDCR_CLEAR               (0xffff8000U)

#define PHAROS_CPU_FMC_SDCR_NC_8                (0b00 << 0)
#define PHAROS_CPU_FMC_SDCR_NR_12               (0b01 << 2)
#define PHAROS_CPU_FMC_SDCR_MWID_32             (0b10 << 4)
#define PHAROS_CPU_FMC_SDCR_NB_4                (0b1 << 6)
#define PHAROS_CPU_FMC_SDCR_CAS_3               (0b11 << 7)
#define PHAROS_CPU_FMC_SDCR_WP_DIS              (0b0 << 9)
#define PHAROS_CPU_FMC_SDCR_SDCLK_2             (0b10 << 10)
#define PHAROS_CPU_FMC_SDCR_RBURST_EN           (0b1 << 12)
#define PHAROS_CPU_FMC_SDCR_RPIPE_0             (0b00 << 13)



#define PHAROS_CPU_FMC_SDTR_TMRD_2              (1 << 0)
#define PHAROS_CPU_FMC_SDTR_TXSR_7              (6 << 4)
#define PHAROS_CPU_FMC_SDTR_TRAS_4              (3 << 8)
#define PHAROS_CPU_FMC_SDTR_TRC_7               (6 << 12)
#define PHAROS_CPU_FMC_SDTR_TWR_3               (2 << 16)
#define PHAROS_CPU_FMC_SDTR_TRP_2               (1 << 20)
#define PHAROS_CPU_FMC_SDTR_TRCD_2              (1 << 24)

#define PHAROS_CPU_FMC_SDRTR_COUNT(x)           ((x) << 1U)

struct PharosCpuFmc
{
    /**
     * SRAM/NOR-Flash chip-select control register for bank x (FMC_BCR1)
     * 
     * offset:      0x0000
     * Reset value: 0x0000 30DB
     */
    volatile uint32_t sramNorFlashChipSelectControl1;

    /**
     * SRAM/NOR-Flash chip-select timing register for bank 1 (FMC_BTR1)
     * 
     * offset:      0x0004
     * Reset value: 0x0FFF FFFF
     */
    volatile uint32_t sramNorFlashChipSelectTiming1;

    /**
     * SRAM/NOR-Flash chip-select control register for bank 2 (FMC_BCR2)
     * 
     * offset:      0x0008
     * Reset value: 0x0000 30D2
     */
    volatile uint32_t sramNorFlashChipSelectControl2;

    /**
     * SRAM/NOR-Flash chip-select timing register for bank 2 (FMC_BTR2)
     * 
     * offset:      0x000c
     * Reset value: 0x0FFF FFFF
     */
    volatile uint32_t sramNorFlashChipSelectTiming2;

    /**
     * SRAM/NOR-Flash chip-select control register for bank 3 (FMC_BCR3)
     * 
     * offset:      0x0010
     * Reset value: 0x0000 30D2
     */
    volatile uint32_t sramNorFlashChipSelectControl3;

    /**
     * SRAM/NOR-Flash chip-select timing register for bank 3 (FMC_BTR3)
     * 
     * offset:      0x0014
     * Reset value: 0x0FFF FFFF
     */
    volatile uint32_t sramNorFlashChipSelectTiming3;

    /**
     * SRAM/NOR-Flash chip-select control register for bank 4 (FMC_BCR4)
     * 
     * offset:      0x0018
     * Reset value: 0x0000 30D2
     */
    volatile uint32_t sramNorFlashChipSelectControl4;

    /**
     * SRAM/NOR-Flash chip-select timing register for bank 4 (FMC_BTR4)
     * 
     * offset:      0x001c
     * Reset value: 0x0FFF FFFF
     */
    volatile uint32_t sramNorFlashChipSelectTiming4;

    /**
     * Reserved
     */
    uint32_t reserved1[24];

    /**
     * NAND Flash control registers (FMC_PCR)
     * 
     * offset:      0x0080
     * Reset value: 0x0000 0018 
     */
    volatile uint32_t nandFlashControl;

    /**
     * FIFO status and interrupt register (FMC_SR)
     * 
     * offset:      0x0084
     * Reset value: 0x0000 0040
     */
    volatile uint32_t fifoStatusInterrupt;

    /**
     * Common memory space timing register 2..4 (FMC_PMEM)
     * 
     * offset:      0x0088
     * Reset value: 0xFCFC FCFC 
     */
    volatile uint32_t commonMemorySpaceTiming;

    /**
     * Attribute memory space timing registers (FMC_PATT)
     * 
     * offset:      0x008c
     * Reset value: 0xFCFC FCFC
     */
    volatile uint32_t attributeMemorySpaceTiming;

    /**
     * Reserved
     */
    uint32_t reserved2;

    /**
     * ECC result register (FMC_ECCR)
     * 
     * offset:      0x0094
     * Reset value: 0x0000 0000 
     */
    volatile uint32_t eccResult;

    /**
     * Reserved
     */
    uint32_t reserved3[27];

    /**
     * SRAM/NOR-Flash write timing register 1 (FMC_BWTR1)
     * 
     * offset:      0x0104
     * Reset value: 0x000F FFFF
     */
    volatile uint32_t sramNorFlashWriteTiming1;

    /**
     * Reserved
     */
    uint32_t reserved4;

    /**
     * SRAM/NOR-Flash write timing register 2 (FMC_BWTR2)
     * 
     * offset:      0x010c
     * Reset value: 0x000F FFFF
     */
    volatile uint32_t sramNorFlashWriteTiming2;

    /**
     * Reserved
     */
    uint32_t reserved5;

    /**
     * SRAM/NOR-Flash write timing register 3 (FMC_BWTR3)
     * 
     * offset:      0x0114
     * Reset value: 0x000F FFFF
     */
    volatile uint32_t sramNorFlashWriteTiming3;

    /**
     * Reserved
     */
    uint32_t reserved6;

    /**
     * SRAM/NOR-Flash write timing register 4 (FMC_BWTR4)
     * 
     * offset:      0x011c
     * Reset value: 0x000F FFFF
     */
    volatile uint32_t sramNorFlashWriteTiming4;

    /**
     * Reserved
     */
    uint32_t reserved7[8];

    /**
     * SDRAM Control register 1 and 2 (FMC_SDCR1 and FMC_SDCR2)
     * 
     * offset:      0x0140 and 0x0144
     * Reset value: 0x0000 02D0 and 0x0000 02D0 
     */
    volatile uint32_t sdramControl[2];

    /**
     * SDRAM Timing register 1 and 2 (FMC_SDTR1 and FMC_SDTR2)
     * 
     * offset:      0x0148 and 0x014c
     * Reset value: 0x0FFF FFFF and 0x0FFF FFFF
     */
    volatile uint32_t sdramTiming[2];

    /**
     * SDRAM Command Mode register (FMC_SDCMR)
     * 
     * offset:      0x0150
     * Reset value: 0x0000 0000 
     */
    volatile uint32_t sdramCommandMode;

    /**
     * SDRAM Refresh Timer register (FMC_SDRTR)
     * 
     * offset:      0x0154
     * Reset value: 0x0000 0000
     */
    volatile uint32_t sdramRefreshTimer;

    /**
     * SDRAM Status register (FMC_SDSR)
     * 
     * offset:      0x0158
     * Reset value: 0x0000 0000
     */
    volatile uint32_t sdramStatus;

};

/* PHAROS_FMC_H */
#endif	
