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


#ifndef PHAROS_MPU_H
#define	PHAROS_MPU_H

#include <pharos/hal/v7m/declarations.h>
#include <pharos/hal/v7m/conf.h>

#define PHAROS_CPU_MPU                              ((ptrPharosCpuMpu) PHAROS_CPU_MPU_ADDRESS)

#define PHAROS_CPU_MPU_CTRL_PRIVDEF_ENA             (1U << 2U)
#define PHAROS_CPU_MPU_CTRL_HFNMI_DIS               (0U << 1U)
#define PHAROS_CPU_MPU_CTRL_ENABLE                  (1U << 0U)
#define PHAROS_CPU_MPU_CTRL_DISABLE                 (0U << 0U)



#define PHAROS_CPU_MPU_RBAR_ADDR(x)                 ((x) & 0xFFFFFFE0U)
#define PHAROS_CPU_MPU_RBAR_VALID                   (1 << 4U)
#define PHAROS_CPU_MPU_RBAR_INVALID                 (0 << 4U)
#define PHAROS_CPU_MPU_RBAR_REGION(x)               ((x) << 0U)

#define PHAROS_CPU_MPU_RASR_XN_EN                   (0 << 28U)
#define PHAROS_CPU_MPU_RASR_XN_DIS                  (1 << 28U)

#define PHAROS_CPU_MPU_RASR_AP_RONLY                (0b110 << 24U)
#define PHAROS_CPU_MPU_RASR_AP_FULL                 (0b011 << 24U)

#define PHAROS_CPU_MPU_RASR_TEX_NOR                 (0b000 << 19U)
#define PHAROS_CPU_MPU_RASR_TEX(x)                  ((x) << 19U)

#define PHAROS_CPU_MPU_RASR_TEX_DEV                 (0b000 << 19U)

#define PHAROS_CPU_MPU_RASR_S_NOR                   (0 << 18U)
#define PHAROS_CPU_MPU_RASR_C_NOR                   (1 << 17U)
#define PHAROS_CPU_MPU_RASR_B_NOR                   (0 << 16U)

#define PHAROS_CPU_MPU_RASR_S_DEV                   (0 << 18U)
#define PHAROS_CPU_MPU_RASR_C_DEV                   (0 << 17U)
#define PHAROS_CPU_MPU_RASR_B_DEV                   (1 << 16U)


#define PHAROS_CPU_MPU_RASR_S(x)                    ((x) << 18U)
#define PHAROS_CPU_MPU_RASR_C(x)                    ((x) << 17U)
#define PHAROS_CPU_MPU_RASR_B(x)                    ((x) << 16U)

#define PHAROS_CPU_MPU_RASR_SRD_EN                  (0x00 << 8U)

#define PHAROS_CPU_MPU_RASR_SIZE(x)                 (((x)-1U) << 1U)

#define PHAROS_CPU_MPU_RASR_EN                      (1U << 0U)
#define PHAROS_CPU_MPU_RASR_DIS                     (0U << 0U)


#define PHAROS_IO_MEM_DIRECT_TEX_NORMAL             (0b000)
#define PHAROS_IO_MEM_DIRECT_S_NORMAL               (0)
#define PHAROS_IO_MEM_DIRECT_B_NORMAL               (0)
#define PHAROS_IO_MEM_DIRECT_C_NORMAL               (1)    

#define PHAROS_IO_MEM_DIRECT_TEX_DEVICE             (0b000)
#define PHAROS_IO_MEM_DIRECT_S_DEVICE               (0)
#define PHAROS_IO_MEM_DIRECT_B_DEVICE               (1)
#define PHAROS_IO_MEM_DIRECT_C_DEVICE               (0)


/**
 * Compact the size of the I/O registers
 */
#pragma pack(push ,4)


struct PharosCpuMpu
{
    /**
     * MPU Type (MPUTYPE)
     * 
     * The MPUTYPE register indicates whether the MPU is present, and if so, how many regions it supports
     * 
     * Offset: 0x00
     */
    volatile uint32_t type;

    /**
     * MPU Control (MPUCTRL)
     * 
     * The MPUCTRL register enables the MPU, enables the default memory map background region, and enables use of the 
     * MPU when in the hard fault, Non-maskable Interrupt (NMI), and Fault Mask Register (FAULTMASK) escalated handlers
     * 
     * Offset: 0x04
     */
    volatile uint32_t control;

    /**
     * MPU Region Number (MPUNUMBER or MPU_RNR)
     * 
     * The MPUNUMBER register selects which memory region is referenced by the MPU Region Base 
     * Address (MPUBASE) and MPU Region Attribute and Size (MPUATTR) registers
     * 
     * Offset: 0x08
     */
    volatile uint32_t regionNumber;

    /**
     * MPU Region Base Address (MPUBASE or MPU_RBAR)
     * 
     * The MPUBASE register defines the base address of the MPU region selected by the MPU Region 
     * Number (MPUNUMBER) register and can update the value of the MPUNUMBER register
     * 
     * Offset: 0x0c
     */
    volatile uint32_t regionBaseAddress;

    /**
     * MPU Region Attribute and Size (MPUATTR or MPU_RASR)
     * 
     * The MPUATTR register defines the region size and memory attributes of the MPU region specified 
     * by the MPU Region Number (MPUNUMBER) register and enables that region and any subregions
     * Offset: 0x10
     */
    volatile uint32_t regionAttributeAndSize;
};


/**
 * restore the previous pack definition
 */
#pragma pack(pop)


void pharosMpuHandler(void);


/* PHAROS_MPU_H */
#endif	
