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

/******************************************************/
/* Offset of the registers for the context switch     */
/******************************************************/

#define OFFSET_CONTEXT_SWITCH_CPSR              ( 0 * 4)
#define OFFSET_CONTEXT_SWITCH_TLB               ( 1 * 4)
#define OFFSET_CONTEXT_SWITCH_R4                ( 2 * 4)
#define OFFSET_CONTEXT_SWITCH_R5                ( 3 * 4)
#define OFFSET_CONTEXT_SWITCH_R6                ( 4 * 4)
#define OFFSET_CONTEXT_SWITCH_R7                ( 5 * 4)
#define OFFSET_CONTEXT_SWITCH_R8                ( 6 * 4)
#define OFFSET_CONTEXT_SWITCH_R9                ( 7 * 4)
#define OFFSET_CONTEXT_SWITCH_R10               ( 8 * 4)
#define OFFSET_CONTEXT_SWITCH_R11               ( 9 * 4)
#define OFFSET_CONTEXT_SWITCH_R13_USER          ( 10 * 4)
#define OFFSET_CONTEXT_SWITCH_R14_USER          ( 11 * 4)
#define OFFSET_CONTEXT_SWITCH_R13_SUPERVISOR    ( 12 * 4)
#define OFFSET_CONTEXT_SWITCH_R14_SUPERVISOR    ( 13 * 4)
#define OFFSET_CONTEXT_SWITCH_S0                ( 14 * 4)
#define OFFSET_CONTEXT_SWITCH_S31               ( 45 * 4)
#define OFFSET_CONTEXT_SWITCH_FPEXC             ( 46 * 4)
#define OFFSET_CONTEXT_SWITCH_FPSCR             ( 47 * 4)
#define OFFSET_CONTEXT_SWITCH_FPINST            ( 48 * 4)
#define OFFSET_CONTEXT_SWITCH_FPINST2           ( 49 * 4)

/******************************************************/
/* Offset of the registers for an IRQ                 */
/******************************************************/

#define OFFSET_IRQ_FRAME                        (50 * 4)

#define OFFSET_IRQ_FRAME_SPSR                   ( 0 * 4)
#define OFFSET_IRQ_FRAME_TLB                    ( 1 * 4)
#define OFFSET_IRQ_FRAME_R3                     ( 2 * 4)
#define OFFSET_IRQ_FRAME_R4                     ( 3 * 4)
#define OFFSET_IRQ_FRAME_R5                     ( 4 * 4)
#define OFFSET_IRQ_FRAME_R6                     ( 5 * 4)
#define OFFSET_IRQ_FRAME_R7                     ( 6 * 4)
#define OFFSET_IRQ_FRAME_R8                     ( 7 * 4)
#define OFFSET_IRQ_FRAME_R9                     ( 8 * 4)
#define OFFSET_IRQ_FRAME_R10                    ( 9 * 4)
#define OFFSET_IRQ_FRAME_R11                    (10 * 4)
#define OFFSET_IRQ_FRAME_R12                    (11 * 4)
#define OFFSET_IRQ_FRAME_LR                     (12 * 4)
#define OFFSET_IRQ_FRAME_R0                     (13 * 4)
#define OFFSET_IRQ_FRAME_R1                     (14 * 4)
#define OFFSET_IRQ_FRAME_R2                     (15 * 4)
#define OFFSET_IRQ_FRAME_SP_SUPERVISOR          (16 * 4)
#define OFFSET_IRQ_FRAME_LR_SUPERVISOR          (17 * 4)
#define OFFSET_IRQ_FRAME_SPSR_SUPERVISOR        (18 * 4)
#define OFFSET_IRQ_FRAME_S0                     (19 * 4)
#define OFFSET_IRQ_FRAME_S31                    (50 * 4)
#define OFFSET_IRQ_FRAME_FPEXC                  (51 * 4)
#define OFFSET_IRQ_FRAME_FPSCR                  (52 * 4)
#define OFFSET_IRQ_FRAME_FPINST                 (53 * 4)
#define OFFSET_IRQ_FRAME_FPINST2                (54 * 4)

/******************************************************/
/* Offset of the kernel stack pointer                 */
/******************************************************/


#define OFFSET_KERNEL_STACK_POINTER             (OFFSET_IRQ_FRAME + (55 * 4))

/******************************************************/
/* Offset of the TLB in user mode                     */
/******************************************************/


#define OFFSET_TLB_USER_MODE                    (OFFSET_KERNEL_STACK_POINTER + (1 * 4))


#define OFFSET_ENTRY_POINT                      (OFFSET_TLB_USER_MODE + (1 * 4))

#define OFFSET_R14_IPC                          (OFFSET_ENTRY_POINT + (1 * 4))


#define OFFSET_IO_ACCESS                        (OFFSET_R14_IPC + (1 * 4))

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

#define CPSR_USER_MODE                  (0x10U)
#define CPSR_IRQ_MODE                   (0x12U)
#define CPSR_SUPERVISOR_MODE            (0x13U)
#define CPSR_SYSTEM_MODE                (0x1fU)
#define CPSR_ABORT_MODE                 (0x17U)
#define CPSR_UNDEFINED_MODE             (0x1bU)

#define CPSR_ENABLE_IRQ                 (0x00U)
#define CPSR_DISABLE_IRQ                (0x80U)

#define CPSR_ENABLE_FIQ                 (0x00U)
#define CPSR_DISABLE_FIQ                (0x40U)

#define CPSR_CPU_MODE_MASK              (0xffffffe0U)

#define CPSR_IRQ_MASK                   (0x80U)

#define FPEXC_ENABLE_MASK               (0x40000000U)

#define ARM_PSR_N                       (1U << 31U)
#define ARM_PSR_Z                       (1U << 30U)
#define ARM_PSR_C                       (1U << 29U)
#define ARM_PSR_V                       (1U << 28U)
#define ARM_PSR_Q                       (1U << 27U)
#define ARM_PSR_J                       (1U << 24U)
#define ARM_PSR_GE_SHIFT                (16U)
#define ARM_PSR_GE_MASK                 (0xfU << ARM_PSR_GE_SHIFT)
#define ARM_PSR_E                       (1U << 9U)
#define ARM_PSR_A                       (1U << 8U)
#define ARM_PSR_I                       (1U << 7U)
#define ARM_PSR_F                       (1U << 6U)
#define ARM_PSR_T                       (1U << 5U)
#define ARM_PSR_M_SHIFT                 (0U)
#define ARM_PSR_M_MASK                  (0x1fU << ARM_PSR_M_SHIFT)
#define ARM_PSR_M_USR                   (0x10U)
#define ARM_PSR_M_FIQ                   (0x11U)
#define ARM_PSR_M_IRQ                   (0x12U)
#define ARM_PSR_M_SUPERVISOR            (0x13U)
#define ARM_PSR_M_ABORT                 (0x17U)
#define ARM_PSR_M_UNDEFINED             (0x1bU)
#define ARM_PSR_M_SYSTEM                (0x1fU)


/**
 * Interrupt stack in supervisor mode. Since there are multiple sources of interrupts that can be nested, we give
 * a good stack margin for the interrupt stack
 */
#define IRQ_SUPERVISOR_STACK_SIZE               (VALUE_32KiB)

/**
 * Don't accept more nested interrupts if the interrupt stack is left down to less than 2KiB
 */
#define PHAROS_CPU_INTERRUPT_STACK_MIN_SIZE     (VALUE_2KiB)


#define IRQ_STACK_SIZE                          (64U)
#define FIQ_STACK_SIZE                          (64U)
#define ABORT_STACK_SIZE                        (64U)
#define UNDEFINED_STACK_SIZE                    (64U)
#define SYSTEM_STACK_SIZE                       (64U)
#define SUPERVISOR_STACK_SIZE                   (64U)

    /**
     * @brief Stack size when starting up. 
     * 
     * Also corresponds to the idle thread stack size
     */
#define PHAROS_CPU_STARTUP_STACK_SIZE           (VALUE_4KiB)


#endif	/* ASM_H */
