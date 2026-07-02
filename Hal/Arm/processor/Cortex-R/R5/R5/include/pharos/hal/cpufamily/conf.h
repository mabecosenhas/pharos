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


#ifndef PHAROS_CPU_FAMILY_CONF_H
#define	PHAROS_CPU_FAMILY_CONF_H

#ifdef	__cplusplus
extern "C"
{
#endif


    /**
     * Alignment required to copy 8 bytes at a time
     */
#define PHAROS_CPU_COPY8_ALIGN                  (8U)


    /**
     * Alignment required to copy 4 bytes at a time
     */
#define PHAROS_CPU_COPY4_ALIGN                  (4U)


    /**
     * all Cortex-R5 have only one core
     */
#define PHAROS_NUMBER_CORES                     (1U)

    
    /**
     * The Cortex-R5 always has FPU (VFPv3-d16)
     */
#define PHAROS_CPU_HAS_VFP

    /**
     * The Cortex-R5 always has MPU
     * 
     * By the specification, with 0, 12 or 16 regions but we will always use a CPU with 12 or 16, since with 0 regions
     * there is no way to make the application safe
     */
#define CPU_HAS_MPU

    /**
     * Minimum recommended user area stack size for a thread
     */
#define PHAROS_STACK_MIN_SIZE                   (VALUE_512B)

    /**
     * Kernel stack size for a thread. This value allows any system call. 
     * 
     * If using filters/IP-calls that can make additional system calls, then additional stack may be required.
     */
#define PHAROS_STACK_KERNEL_MIN_SIZE            (VALUE_2KiB)


    /**
     *  @brief ensure that the compiler writes memory in correct order (does not change the order of certain operations)
     */
#define PHAROS_CPU_AND_COMP_MEM_BARRIER()       __asm volatile("dsb\n" "isb\n" ::: "memory")

#ifdef	__cplusplus
}
#endif

#endif	/* CONFR5_H */
