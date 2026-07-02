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


#ifndef PHAROS_V7M_CONF_H
#define	PHAROS_V7M_CONF_H

#include <pharos/hal/cpu/conf.h>

/**
 * Minimum stack size for a thread (running in user space)
 */
#define PHAROS_STACK_MIN_SIZE                       (VALUE_1KiB)

/**
 * Minimum stack size for the thread kernel (running in supervisor state)
 */
#define PHAROS_STACK_KERNEL_MIN_SIZE                (VALUE_1KiB + VALUE_512B)

/**
 * Minimum stack size of the shared stack. This is actually very dependent on the application itself. 
 * But don't place a value smaller than this to account for stuff Pharos needs
 */
#define PHAROS_SHARED_STACK_SIZE_MINIMUM_SIZE       (VALUE_1KiB)


#define PHAROS_ERR_CPU_BUS                          (PHAROS_ERR_CPU_START)
#define PHAROS_ERR_CPU_USAGE                        (PHAROS_ERR_CPU_START+1)
#define PHAROS_ERR_CPU_DEBUG                        (PHAROS_ERR_CPU_START+2)
#define PHAROS_ERR_CPU_HARD                         (PHAROS_ERR_CPU_START+3)

/**
 * Frequency of the CPU in MHz
 */
#define PHAROS_CPU_FREQ_MHZ                         (PHAROS_CPU_FREQ / VALUE_1M)

/**
 * ARM V7-M SCS address
 */
#define PHAROS_CPU_SCS_ADDRESS                      (0xE000E000UL)                            

/**
 * ARM V7-M NVIC address
 */
#define PHAROS_CPU_NVIC_ADDRESS                     (0xE000E100UL)

/**
 * ARM V7-M SCB address
 */
#define PHAROS_CPU_SCB_ADDRESS                      (0xE000ED00UL)                            

/**
 * ARM V7-M MPU address
 */
#define PHAROS_CPU_MPU_ADDRESS                      (0xE000ED90UL)

/**
 * ARM V7-M NVIC address
 */
#define PHAROS_CPU_FPU_ADDRESS                      (0xE000EF34UL)

/**
 * ARM V7-M Systick address
 */
#define PHAROS_CPU_SYSTICK_ADDRESS                  (0xE000E010UL)

/**
 * ARM V7-M CACHE address
 */
#define PHAROS_CPU_CACHE_ADDRESS                    (0xE000EF50UL)

/**
 * ARM V7-M CPUID address
 */
#define PHAROS_CPU_CPUID_ADDRESS                    (0xE000ED00UL)

/**
 * Number of MPU regions that can be used in the partition to protect the memory areas that the partition uses
 */
#define PHAROS_CPU_MPU_NBR_RGI_VAR                  (PHAROS_CPU_MPU_NUMBER_REGIONS - PHAROS_CPU_MPU_NBR_RGI_FIX)

/**
 * 4 MPU regions are fixed (text, ipcall, shared and shared stack)
 */
#define PHAROS_CPU_MPU_NBR_RGI_FIX                  (4U)

/**
 * Text MPU area
 */
#define PHAROS_CPU_MPU_REGION_TEXT                  (0U)

/**
 * IP call MPU area
 */
#define PHAROS_CPU_MPU_REGION_IPCALL                (1U)   

/**
 * Shared region MPU area
 */
#define PHAROS_CPU_MPU_REGION_SHARED                (2U)

/**
 * Stack MPU area
 * a thread only accesses one stack (user or shared) at each moment
 * The kernel stack is not needed to be placed since with supervisor permissions the CPU has access to any address
 */
#define PHAROS_CPU_MPU_REGION_STACK                 (3U)

/**
 * Alignment required to copy 8 bytes at a time
 */
#define PHAROS_CPU_COPY8_ALIGN                      (8U)

/**
 * Alignment required to copy 4 bytes at a time
 */
#define PHAROS_CPU_COPY4_ALIGN                      (4U)

/**
 * Number of interrupts that the Pharos Kernel will have (on its interrupt table pharosVIsrTable)
 * Already accounts for the number of exceptions (16)
 */
#define PHAROS_NUMBER_INTERRUPTS                    (16U + PHAROS_CPU_NUMBER_INTERRUPTS)


/* PHAROS_CONF_H */
#endif	
