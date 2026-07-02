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
 * File:   asm.h
 *
 * Created on September 24, 2017
 */

#ifndef PHAROS_ARM_ASM_H
#define	PHAROS_ARM_ASM_H

#include <pharos/hal/def.h>
#include <pharos/hal/cpu/conf.h>

#define PHAROS_CPU_CONTEXT_MPU_SIZE             (12 * (PHAROS_CPU_MPU_NUMBER_REGIONS-2) + 1)

/******************************************************/
/* Offset of the registers for the context switch     */
/******************************************************/

#define OFFSET_CONTEXT_SWITCH_CPSR              ( 0 * 4)
#define OFFSET_CONTEXT_SWITCH_R4                ( 1 * 4)
#define OFFSET_CONTEXT_SWITCH_R5                ( 2 * 4)
#define OFFSET_CONTEXT_SWITCH_R6                ( 3 * 4)
#define OFFSET_CONTEXT_SWITCH_R7                ( 4 * 4)
#define OFFSET_CONTEXT_SWITCH_R8                ( 5 * 4)
#define OFFSET_CONTEXT_SWITCH_R9                ( 6 * 4)
#define OFFSET_CONTEXT_SWITCH_R10               ( 7 * 4)
#define OFFSET_CONTEXT_SWITCH_R11               ( 8 * 4)
#define OFFSET_CONTEXT_SWITCH_R13_USER          ( 9 * 4)
#define OFFSET_CONTEXT_SWITCH_R14_USER          ( 10 * 4)
#define OFFSET_CONTEXT_SWITCH_R13_SUPERVISOR    ( 11 * 4)
#define OFFSET_CONTEXT_SWITCH_R14_SUPERVISOR    ( 12 * 4)
#define OFFSET_CONTEXT_SWITCH_S0                ( 13 * 4)
#define OFFSET_CONTEXT_SWITCH_S31               ( 44 * 4)
#define OFFSET_CONTEXT_SWITCH_FPEXC             ( 45 * 4)
#define OFFSET_CONTEXT_SWITCH_FPSCR             ( 46 * 4)

/******************************************************/
/* Offset of the kernel stack pointer                 */
/******************************************************/

#define OFFSET_KERNEL_STACK_POINTER             (OFFSET_CONTEXT_SWITCH_FPSCR + (1 * 4))

/******************************************************/
/* Offset of the thread entry point                   */
/******************************************************/

#define OFFSET_ENTRY_POINT                      (OFFSET_KERNEL_STACK_POINTER + (1 * 4))


/******************************************************/
/* Offset of the registers for an IRQ                 */
/******************************************************/

#define OFFSET_IRQ_FRAME                        ((OFFSET_ENTRY_POINT + 4))

#define OFFSET_IRQ_FRAME_SPSR                   ( 0 * 4)
#define OFFSET_IRQ_FRAME_R3                     ( 1 * 4)
#define OFFSET_IRQ_FRAME_R4                     ( 2 * 4)
#define OFFSET_IRQ_FRAME_R5                     ( 3 * 4)
#define OFFSET_IRQ_FRAME_R6                     ( 4 * 4)
#define OFFSET_IRQ_FRAME_R7                     ( 5 * 4)
#define OFFSET_IRQ_FRAME_R8                     ( 6 * 4)
#define OFFSET_IRQ_FRAME_R9                     ( 7 * 4)
#define OFFSET_IRQ_FRAME_R10                    ( 8 * 4)
#define OFFSET_IRQ_FRAME_R11                    ( 9 * 4)
#define OFFSET_IRQ_FRAME_R12                    (10 * 4)
#define OFFSET_IRQ_FRAME_LR                     (11 * 4)
#define OFFSET_IRQ_FRAME_R0                     (12 * 4)
#define OFFSET_IRQ_FRAME_R1                     (13 * 4)
#define OFFSET_IRQ_FRAME_R2                     (14 * 4)
#define OFFSET_IRQ_FRAME_SP_SUPERVISOR          (15 * 4)
#define OFFSET_IRQ_FRAME_LR_SUPERVISOR          (16 * 4)
#define OFFSET_IRQ_FRAME_SPSR_SUPERVISOR        (17 * 4)
#define OFFSET_IRQ_FRAME_S0                     (18 * 4)
#define OFFSET_IRQ_FRAME_S31                    (49 * 4)
#define OFFSET_IRQ_FRAME_FPEXC                  (50 * 4)
#define OFFSET_IRQ_FRAME_FPSCR                  (51 * 4)

#define OFFSET_LR_IPC                           (OFFSET_IRQ_FRAME + OFFSET_IRQ_FRAME_FPSCR + (1 * 4))
#define OFFSET_IO_ACCESS                        (OFFSET_LR_IPC + (1 * 4))


/******************************************************/
/* CPU CPSR register definitions                      */
/******************************************************/

/**
 * CPU CPSR:
 * 
 * | N | Z | C | V | . . . . . . . . . . . . . . . | I | F | T | Mode |
 *   31  30  29  28  27                          8   7   6   5  4    0 
 * N - negative/less than
 * Z - zero
 * C - carry/borrow/extend
 * V - overflow
 * I - IRQ disable (1 means disabled)
 * F - FIQ disable (1 means disabled)
 * T - Thumb/arm state
 * Mode - CPU mode (e.g. user, system, irq, abort, supervisor, etc)
 */

#define CPSR_USER_MODE          (0x10U)
#define CPSR_IRQ_MODE           (0x12U)
#define CPSR_SUPERVISOR_MODE    (0x13U)
#define CPSR_SYSTEM_MODE        (0x1fU)
#define CPSR_ABORT_MODE         (0x17U)
#define CPSR_UNDEFINED_MODE     (0x1bU)

#define CPSR_ENABLE_IRQ         (0x00U)
#define CPSR_DISABLE_IRQ        (0x80U)

#define CPSR_ENABLE_FIQ         (0x00U)
#define CPSR_DISABLE_FIQ        (0x40U)

#define CPSR_CPU_MODE_MASK      (0xffffffe0U)

#define CPSR_IRQ_MASK           (0x80U)

#define FPEXC_ENABLE_MASK       (0x40000000U)

#define ARM_PSR_N (1 << 31)
#define ARM_PSR_Z (1 << 30)
#define ARM_PSR_C (1 << 29)
#define ARM_PSR_V (1 << 28)
#define ARM_PSR_Q (1 << 27)
#define ARM_PSR_J (1 << 24)
#define ARM_PSR_GE_SHIFT 16
#define ARM_PSR_GE_MASK (0xf << ARM_PSR_GE_SHIFT)
#define ARM_PSR_E (1 << 9)
#define ARM_PSR_A (1 << 8)
#define ARM_PSR_I (1 << 7)
#define ARM_PSR_F (1 << 6)
#define ARM_PSR_T (1 << 5)
#define ARM_PSR_M_SHIFT 0
#define ARM_PSR_M_MASK (0x1f << ARM_PSR_M_SHIFT)
#define ARM_PSR_M_USR 0x10
#define ARM_PSR_M_FIQ 0x11
#define ARM_PSR_M_IRQ 0x12
#define ARM_PSR_M_SUPERVISOR 0x13
#define ARM_PSR_M_ABORT 0x17
#define ARM_PSR_M_UNDEFINED 0x1b
#define ARM_PSR_M_SYSTEM 0x1f

#define PHAROS_CPU_INTERRUPT_STACK_MIN_SIZE     (VALUE_2KiB)

#define IRQ_SUPERVIOSR_STACK_SIZE               (VALUE_16KiB)
#define IRQ_STACK_SIZE                          64
#define FIQ_STACK_SIZE                          256
#define ABORT_STACK_SIZE                        64
#define UNDEFINED_STACK_SIZE                    64
#define SYSTEM_STACK_SIZE                       64
#define SUPERVISOR_STACK_SIZE                   64


/**
 * @brief Stack size when starting up. 
 * 
 * Also corresponds to the idle thread stack size
 */
#define PHAROS_CPU_STARTUP_STACK_SIZE   (1024U*4U)



#endif	/* ASM_H */
