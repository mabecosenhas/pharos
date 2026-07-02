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


#ifndef PHAROS_CPU_ASM_H
#define	PHAROS_CPU_ASM_H


/******************************************************/
/* Offset of the registers for the context switch     */
/******************************************************/

#define OFFSET_CONTEXT_SWITCH_R4                (0  * 4)
#define OFFSET_CONTEXT_SWITCH_R5                (1  * 4)
#define OFFSET_CONTEXT_SWITCH_R6                (2  * 4)
#define OFFSET_CONTEXT_SWITCH_R7                (3  * 4)
#define OFFSET_CONTEXT_SWITCH_R8                (4  * 4)
#define OFFSET_CONTEXT_SWITCH_R9                (5  * 4)
#define OFFSET_CONTEXT_SWITCH_R10               (6  * 4)
#define OFFSET_CONTEXT_SWITCH_R11               (7  * 4)
#define OFFSET_CONTEXT_SWITCH_R12               (8  * 4)
#define OFFSET_CONTEXT_SWITCH_LR                (9  * 4)
#define OFFSET_CONTEXT_SWITCH_PSP               (10 * 4)
#define OFFSET_CONTEXT_SWITCH_PSR               (11 * 4)
#define OFFSET_CONTEXT_SWITCH_CONTROL           (12 * 4)
#define OFFSET_CONTEXT_SWITCH_FPSCR             (13 * 4)
#define OFFSET_CONTEXT_SWITCH_PRIMASK           (14 * 4)
#define OFFSET_CONTEXT_SWITCH_S0                (15 * 4)
#define OFFSET_CONTEXT_SWITCH_S31               (46 * 4)

/******************************************************/
/* Offset of the kernel stack pointer                 */
/******************************************************/

#define OFFSET_CURRENT_KERNEL_STACK             (OFFSET_CONTEXT_SWITCH_S31 + (1 * 4))
#define OFFSET_ORIGINAL_KERNEL_STACK            (OFFSET_CURRENT_KERNEL_STACK + (1 * 4))
#define OFFSET_PREVIOUS_SVC_STACK               (OFFSET_ORIGINAL_KERNEL_STACK + (1 * 4))
#define OFFSET_PREVIOUS_PENDABLE_STACK          (OFFSET_PREVIOUS_SVC_STACK + (1 * 4))   


/******************************************************/
/* Offset of the thread entry point                   */
/******************************************************/

#define OFFSET_ENTRY_POINT                      (OFFSET_PREVIOUS_PENDABLE_STACK + (1 * 4))

#define OFFSET_USER_STACK                       (OFFSET_ENTRY_POINT + (1 * 4))

#define OFFSET_LR_IPC                           (OFFSET_USER_STACK + (1 * 4))

#define OFFSET_INSIDE_KERNEL                    (OFFSET_LR_IPC + (1 * 4))

#define OFFSET_IO_ACCESS                        (OFFSET_INSIDE_KERNEL + (1 * 4))




/******************************************************/
/* CPU PSR register definitions                       */
/******************************************************/

#define CONTROL_PSP_POINTER                 (0b10)
#define CONTROL_PRIV_BIT                    (0b0)
#define CONTROL_PRIV_NON_PRIV               (0b1)
#define CONTROL_PRIV_PRIV                   (0b0)
#define CONTROL_PRIV_BIT_MASK               (!CONTROL_PRIV_BIT)


#define FPEXC_ENABLE_MASK                   (0x40000000U)


/**
 * Interrupt stack size (in bytes)
 */
#define PHAROS_CPU_INTERRUPT_STACK_SIZE     (VALUE_16KiB)


/**
 * Don't accept more nested interrupts if the current stack only has 512 bytes left
 */
#define PHAROS_CPU_INTERRUPT_STACK_MIN_SIZE (512U)


/**
 * @brief Stack size when starting up. 
 * 
 * Also corresponds to the idle thread stack size
 */
#define PHAROS_CPU_STARTUP_STACK_SIZE       (1024U*4U)


#define PHAROS_CPU_CONTROL_PRIV             (0U << 0U)
#define PHAROS_CPU_CONTROL_UNPRIV           (1U << 0U)

#define PHAROS_CPU_CONTROL_MAIN_SP          (0U << 1U)
#define PHAROS_CPU_CONTROL_PROCESS_SP       (1U << 1U)

#define PHAROS_CPU_CONTROL_FP_EXT_ACT       (1U << 2U)
#define PHAROS_CPU_CONTROL_FP_EXT_DIS       (0U << 2U)


#endif	/* ASM_H */
