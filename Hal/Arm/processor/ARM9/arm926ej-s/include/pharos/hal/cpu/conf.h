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
 * File:   conf.h
 *
 * Created on September 25, 2017
 */

#ifndef PHAROS_ARM_CONF_H
#define PHAROS_ARM_CONF_H


/**
 * Define the CPU arm926ej-s
 */
#define PHAROS_CPU_ARM926EJS


/**
 * The arm926ej-s uses always FPU
 */
#define PHAROS_CPU_HAS_VFP

/**
 * The arm926ej-s uses always MMU
 */
#define PHAROS_CPU_HAS_MMU


/**
 * Alignment required to copy 8 bytes at a time
 */
#define PHAROS_CPU_COPY8_ALIGN                      (8U)


/**
 * Alignment required to copy 4 bytes at a time
 */
#define PHAROS_CPU_COPY4_ALIGN                      (4U)


/**
 * all ARM926 have only one core
 */
#define PHAROS_NUMBER_CORES                         (1U)


/**
 * Number of memory areas of this CPU (only SDRAM). When we have multiple boards with ARM9 then this definition
 * should be moved to the board component (each board has its own number of memory areas)
 */
#define PHAROS_NUMBER_MEMORY_AREAS                  (1U)


/**
 * Minimum stack size for a thread (running in user space). This really depends on the application itself.
 */
#define PHAROS_STACK_MIN_SIZE                       (VALUE_2KiB)


/**
 * Minimum stack size for the thread kernel (running in supervisor state)
 * Tested with 1KiB and was OK, but this CPU has enough memory to allow a little extra
 */
#define PHAROS_STACK_KERNEL_MIN_SIZE                (VALUE_2KiB)


/**
 * Cache line size
 */
#define PHAROS_CPU_CACHE_LINE_SIZE                  (64U)


#endif /* CONF_H */
