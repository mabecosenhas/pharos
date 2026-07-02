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


#ifndef PHAROS_ARCHCONF_H
#define PHAROS_ARCHCONF_H

#define PHAROS_CPU_RISCV

#define PHAROS_CPU_RISCV64


/**
 * Alignment required to copy 8 bytes at a time
 */
#define PHAROS_CPU_COPY8_ALIGN                      (8U)


/**
 * Alignment required to copy 4 bytes at a time
 */
#define PHAROS_CPU_COPY4_ALIGN                      (4U)


/**
 * In RISCV64 stack must be always aligned to 16 bytes
 */
#define PHAROS_STACK_ALIGNMENT                      (16U)


/**
 * Alignment required for a generic structure
 */
#define STRUCTURE_ALIGNMENT                         (8U)


/**
 * Minimum stack size for a thread in user space
 */
#define PHAROS_STACK_MIN_SIZE                       (VALUE_4KiB)


/**
 * Minimum stack size for a thread in kernel space
 */
#define PHAROS_STACK_KERNEL_MIN_SIZE                (VALUE_4KiB)


/**
 * Minimum stack size for a shared stack
 */
#define PHAROS_SHARED_STACK_SIZE_MINIMUM_SIZE       (VALUE_4KiB)


/**
 * Calculate the correct size of the shared stack for dynamically created threads.
 * Must be the next multiple of 4 KiB of the original stack size and not below 4 KiB 
 */
#define PHAROS_SHARED_STACK_CORRECT_SIZE(size)      MAX_MACRO(VALUE_4KiB , VALUE_4KiB * PHAROS_CEILING(size , VALUE_4KiB))


/**
 * Alignment of the shared stack. Must be aligned to an MMU page
 */
#define PHAROS_DYNAMIC_STACK_ALIGN(size)   VALUE_4KiB


/**
 * Startup stack size in bytes
 */
#define PHAROS_CPU_STARTUP_STACK_SIZE               (VALUE_16KiB)

/**
 * Log2 of the startup stack size 
 */
#define PHAROS_CPU_STARTUP_STACK_SIZE_LOG2          (14U)


/* PHAROS_ARCHCONF_H */
#endif 
