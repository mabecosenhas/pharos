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


#ifndef PHAROS_CPU_CONF_H
#define PHAROS_CPU_CONF_H

#include <pharos/hal/cpufamily/conf.h>

/**
 *Number of memory areas of this CPU
 */
#define PHAROS_NUMBER_MEMORY_AREAS              (1U)

/**
 * Cache line size
 */
#define PHAROS_CPU_CACHE_LINE_SIZE              (64U)

/**
 * address of the RTI
 */
#define PHAROS_CPU_RTI_ADDRESS                  (0xFFFFFC00U)

/**
 * address of the ESM
 */
#define PHAROS_CPU_ESM_ADDRESS                  (0xFFFFF500U)

/**
 * address of the VIM interrupt manager
 */
#define PHAROS_VIM_ADDRESS                      (0xFFFFFD00U)

/**
 * Address of the VIM interrupt table 
 */
#define PHAROS_VIM_TABLE_ADDRESS                (0xFFF82000U)


/**
 * Number of interrupt channels supported by this CPU
 */
#define PHAROS_CPU_VIM_CHANNEL_SIZE             (128U)

/**
 * Number of interrupts supported by this CPU
 */
#define PHAROS_CPU_VIM_ISR_NUMBER               (128U)


/**
 * Number of interrupts that the Pharos Kernel will have (on its interrupt table pharosVIsrTable)
 */
#define PHAROS_NUMBER_INTERRUPTS                (PHAROS_CPU_VIM_CHANNEL_SIZE)

/**
 * Number of VIM channels
 */
#define PHAROS_CPU_VIM_CHANNEL_MAX              (126U)

#define PHAROS_CPU_SYSTEM_PRIMARY_ADDRESS       (0xFFFFFF00U)
#define PHAROS_CPU_SYSTEM_SECUNDARY_ADDRESS     (0xFFFFE100U)


#define PHAROS_CPU_PCR1_ADDRESS                 (0xFFFF1000U)
#define PHAROS_CPU_PCR2_ADDRESS                 (0xFCFF1000U)
#define PHAROS_CPU_PCR3_ADDRESS                 (0xFFF78000U)    

#define PHAROS_CPU_IOMM_ADDRESS                 (0xFFFF1C00U)    


#define PHAROS_CPU_MPU_NUMBER_REGIONS           (16U)

/**
 * 4 MPU regions are fixed (text, ipcall, shared and shared stack)
 */
#define PHAROS_CPU_MPU_NBR_RGI_FIX              (4U)

/**
 * Number of MPU regions that can be used in the partition to protect the memory areas that the partition uses
 */
#define PHAROS_CPU_MPU_NBR_RGI_VAR              (PHAROS_CPU_MPU_NUMBER_REGIONS - PHAROS_CPU_MPU_NBR_RGI_FIX)

/**
 * Text MPU area
 */
#define PHAROS_CPU_MPU_REGION_TEXT              (0U)

/**
 * IP call MPU area
 */
#define PHAROS_CPU_MPU_REGION_IPCALL            (1U)   

/**
 * Shared region MPU area
 */
#define PHAROS_CPU_MPU_REGION_SHARED            (2U)

/**
 * Shared stack MPU area (a thread only accesses one shared stack at each moment)
 */
#define PHAROS_CPU_MPU_REGION_SHR_STACK         (3U)


#define PHAROS_CPU_L2FMC_ADDRESS                (0xFFF87000U)



#define PHAROS_CPU_SCI_REG1                     (0xFFF7E400U)
#define PHAROS_CPU_SCI_REG2                     (0xFFF7E600U)    
#define PHAROS_CPU_SCI_REG3                     (0xFFF7E500U)    
#define PHAROS_CPU_SCI_REG4                     (0xFFF7E700U)    


#define PHAROS_CPU_SCI_PORT1                    (0xFFF7E440U)
#define PHAROS_CPU_SCI_PORT2                    (0xFFF7E640U)
#define PHAROS_CPU_SCI_PORT3                    (0xFFF7E540U)
#define PHAROS_CPU_SCI_PORT4                    (0xFFF7E740U)

#define PHAROS_CPU_LPO_TRIM_ADDRESS             (0xF00801B4U)

#endif /* CONF_H */
