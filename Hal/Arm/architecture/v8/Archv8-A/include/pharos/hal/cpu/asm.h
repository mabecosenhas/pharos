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


#ifndef PHAROS_ASM_H
#define	PHAROS_ASM_H

#include <pharos/hal/def.h>


/******************************************************/
/* Offset of the registers for the context switch     */
/******************************************************/

#define OFFSET_CONTEXT_SWITCH_SP_EL1            ( 0 * 8)
#define OFFSET_CONTEXT_SWITCH_SP_EL0            ( 1 * 8)
#define OFFSET_CONTEXT_SWITCH_ERL_EL1           ( 2 * 8)
#define OFFSET_CONTEXT_SWITCH_SPSR_EL1          ( 3 * 8)
#define OFFSET_CONTEXT_SWITCH_X8                ( 4 * 8)
#define OFFSET_CONTEXT_SWITCH_X9                ( 5 * 8)
#define OFFSET_CONTEXT_SWITCH_X10               ( 6 * 8)
#define OFFSET_CONTEXT_SWITCH_X11               ( 7 * 8)
#define OFFSET_CONTEXT_SWITCH_X12               ( 8 * 8)
#define OFFSET_CONTEXT_SWITCH_X13               ( 9 * 8)
#define OFFSET_CONTEXT_SWITCH_X14               (10 * 8)
#define OFFSET_CONTEXT_SWITCH_X15               (11 * 8)
#define OFFSET_CONTEXT_SWITCH_X16               (12 * 8)
#define OFFSET_CONTEXT_SWITCH_X17               (13 * 8)
#define OFFSET_CONTEXT_SWITCH_X18               (14 * 8)
#define OFFSET_CONTEXT_SWITCH_X19               (15 * 8)
#define OFFSET_CONTEXT_SWITCH_X20               (16 * 8)
#define OFFSET_CONTEXT_SWITCH_X21               (17 * 8)
#define OFFSET_CONTEXT_SWITCH_X22               (18 * 8)
#define OFFSET_CONTEXT_SWITCH_X23               (19 * 8)
#define OFFSET_CONTEXT_SWITCH_X24               (20 * 8)
#define OFFSET_CONTEXT_SWITCH_X25               (21 * 8)
#define OFFSET_CONTEXT_SWITCH_X26               (22 * 8)
#define OFFSET_CONTEXT_SWITCH_X27               (23 * 8)
#define OFFSET_CONTEXT_SWITCH_X28               (24 * 8)
#define OFFSET_CONTEXT_SWITCH_X29               (25 * 8)
#define OFFSET_CONTEXT_SWITCH_X30               (26 * 8)

#define SIZE_THREAD_CPU_BASE_CONTEXT            (OFFSET_CONTEXT_SWITCH_X30 + (1 * 8))

#define OFFSET_CONTEXT_SWITCH_Q0                (OFFSET_CONTEXT_SWITCH_X30 + 16)
#define OFFSET_CONTEXT_SWITCH_Q1                (OFFSET_CONTEXT_SWITCH_Q0  + 1 * 16)
#define OFFSET_CONTEXT_SWITCH_Q2                (OFFSET_CONTEXT_SWITCH_Q0  + 2 * 16)
#define OFFSET_CONTEXT_SWITCH_Q3                (OFFSET_CONTEXT_SWITCH_Q0  + 3 * 16)
#define OFFSET_CONTEXT_SWITCH_Q4                (OFFSET_CONTEXT_SWITCH_Q0  + 4 * 16)
#define OFFSET_CONTEXT_SWITCH_Q5                (OFFSET_CONTEXT_SWITCH_Q0  + 5 * 16)
#define OFFSET_CONTEXT_SWITCH_Q6                (OFFSET_CONTEXT_SWITCH_Q0  + 6 * 16)
#define OFFSET_CONTEXT_SWITCH_Q7                (OFFSET_CONTEXT_SWITCH_Q0  + 7 * 16)
#define OFFSET_CONTEXT_SWITCH_Q8                (OFFSET_CONTEXT_SWITCH_Q0  + 8 * 16)
#define OFFSET_CONTEXT_SWITCH_Q9                (OFFSET_CONTEXT_SWITCH_Q0  + 9 * 16)
#define OFFSET_CONTEXT_SWITCH_Q10               (OFFSET_CONTEXT_SWITCH_Q0  + 10 * 16)
#define OFFSET_CONTEXT_SWITCH_Q11               (OFFSET_CONTEXT_SWITCH_Q0  + 11 * 16)
#define OFFSET_CONTEXT_SWITCH_Q12               (OFFSET_CONTEXT_SWITCH_Q0  + 12 * 16)
#define OFFSET_CONTEXT_SWITCH_Q13               (OFFSET_CONTEXT_SWITCH_Q0  + 13 * 16)
#define OFFSET_CONTEXT_SWITCH_Q14               (OFFSET_CONTEXT_SWITCH_Q0  + 14 * 16)
#define OFFSET_CONTEXT_SWITCH_Q15               (OFFSET_CONTEXT_SWITCH_Q0  + 15 * 16)
#define OFFSET_CONTEXT_SWITCH_Q16               (OFFSET_CONTEXT_SWITCH_Q0  + 16 * 16)
#define OFFSET_CONTEXT_SWITCH_Q17               (OFFSET_CONTEXT_SWITCH_Q0  + 17 * 16)
#define OFFSET_CONTEXT_SWITCH_Q18               (OFFSET_CONTEXT_SWITCH_Q0  + 18 * 16)
#define OFFSET_CONTEXT_SWITCH_Q19               (OFFSET_CONTEXT_SWITCH_Q0  + 19 * 16)
#define OFFSET_CONTEXT_SWITCH_Q20               (OFFSET_CONTEXT_SWITCH_Q0  + 20 * 16)
#define OFFSET_CONTEXT_SWITCH_Q21               (OFFSET_CONTEXT_SWITCH_Q0  + 21 * 16)
#define OFFSET_CONTEXT_SWITCH_Q22               (OFFSET_CONTEXT_SWITCH_Q0  + 22 * 16)
#define OFFSET_CONTEXT_SWITCH_Q23               (OFFSET_CONTEXT_SWITCH_Q0  + 23 * 16)
#define OFFSET_CONTEXT_SWITCH_Q24               (OFFSET_CONTEXT_SWITCH_Q0  + 24 * 16)
#define OFFSET_CONTEXT_SWITCH_Q25               (OFFSET_CONTEXT_SWITCH_Q0  + 25 * 16)
#define OFFSET_CONTEXT_SWITCH_Q26               (OFFSET_CONTEXT_SWITCH_Q0  + 26 * 16)
#define OFFSET_CONTEXT_SWITCH_Q27               (OFFSET_CONTEXT_SWITCH_Q0  + 27 * 16)
#define OFFSET_CONTEXT_SWITCH_Q28               (OFFSET_CONTEXT_SWITCH_Q0  + 28 * 16)
#define OFFSET_CONTEXT_SWITCH_Q29               (OFFSET_CONTEXT_SWITCH_Q0  + 29 * 16)
#define OFFSET_CONTEXT_SWITCH_Q30               (OFFSET_CONTEXT_SWITCH_Q0  + 30 * 16)
#define OFFSET_CONTEXT_SWITCH_Q31               (OFFSET_CONTEXT_SWITCH_Q0  + 31 * 16)



/******************************************************/
/* IRQ frame                                          */
/******************************************************/
#define OFFSET_IRQ_FRAME                        (OFFSET_CONTEXT_SWITCH_Q31 + (1 * 16))
#define OFFSET_IRQ_FRAME_X2                     (OFFSET_IRQ_FRAME)
#define OFFSET_IRQ_FRAME_X3                     (OFFSET_IRQ_FRAME + (1*8))
#define OFFSET_IRQ_FRAME_X4                     (OFFSET_IRQ_FRAME + (2*8))
#define OFFSET_IRQ_FRAME_X5                     (OFFSET_IRQ_FRAME + (3*8))
#define OFFSET_IRQ_FRAME_X6                     (OFFSET_IRQ_FRAME + (4*8))
#define OFFSET_IRQ_FRAME_X7                     (OFFSET_IRQ_FRAME + (5*8))
#define OFFSET_IRQ_FRAME_X8                     (OFFSET_IRQ_FRAME + (6*8))
#define OFFSET_IRQ_FRAME_X9                     (OFFSET_IRQ_FRAME + (7*8))
#define OFFSET_IRQ_FRAME_X10                    (OFFSET_IRQ_FRAME + (8*8))
#define OFFSET_IRQ_FRAME_X11                    (OFFSET_IRQ_FRAME + (9*8))
#define OFFSET_IRQ_FRAME_X12                    (OFFSET_IRQ_FRAME + (10*8))
#define OFFSET_IRQ_FRAME_X13                    (OFFSET_IRQ_FRAME + (11*8))
#define OFFSET_IRQ_FRAME_X14                    (OFFSET_IRQ_FRAME + (12*8))
#define OFFSET_IRQ_FRAME_X15                    (OFFSET_IRQ_FRAME + (13*8))
#define OFFSET_IRQ_FRAME_X29                    (OFFSET_IRQ_FRAME + (14*8))
#define OFFSET_IRQ_FRAME_X30                    (OFFSET_IRQ_FRAME + (15*8))
#define OFFSET_IRQ_FRAME_SPSR                   (OFFSET_IRQ_FRAME + (16*8))
#define OFFSET_IRQ_FRAME_ELR                    (OFFSET_IRQ_FRAME + (17*8))
#define OFFSET_IRQ_FRAME_X0                     (OFFSET_IRQ_FRAME + (18*8))
#define OFFSET_IRQ_FRAME_X1                     (OFFSET_IRQ_FRAME + (19*8))
#define OFFSET_IRQ_FRAME_SP                     (OFFSET_IRQ_FRAME + (20*8))
#define OFFSET_IRQ_FRAME_SP_EL1                 (OFFSET_IRQ_FRAME + (21*8))
#define OFFSET_IRQ_FRAME_Q0                     (OFFSET_IRQ_FRAME + (22*8))
#define OFFSET_IRQ_FRAME_Q1                     (OFFSET_IRQ_FRAME_Q0 + 1 * 16)
#define OFFSET_IRQ_FRAME_Q2                     (OFFSET_IRQ_FRAME_Q0 + 2 * 16)
#define OFFSET_IRQ_FRAME_Q3                     (OFFSET_IRQ_FRAME_Q0 + 3 * 16)
#define OFFSET_IRQ_FRAME_Q4                     (OFFSET_IRQ_FRAME_Q0 + 4 * 16)
#define OFFSET_IRQ_FRAME_Q5                     (OFFSET_IRQ_FRAME_Q0 + 5 * 16)
#define OFFSET_IRQ_FRAME_Q6                     (OFFSET_IRQ_FRAME_Q0 + 6 * 16)
#define OFFSET_IRQ_FRAME_Q7                     (OFFSET_IRQ_FRAME_Q0 + 7 * 16)
#define OFFSET_IRQ_FRAME_Q8                     (OFFSET_IRQ_FRAME_Q0 + 8 * 16)
#define OFFSET_IRQ_FRAME_Q9                     (OFFSET_IRQ_FRAME_Q0 + 9 * 16)
#define OFFSET_IRQ_FRAME_Q10                    (OFFSET_IRQ_FRAME_Q0 + 10 * 16)
#define OFFSET_IRQ_FRAME_Q11                    (OFFSET_IRQ_FRAME_Q0 + 11 * 16)
#define OFFSET_IRQ_FRAME_Q12                    (OFFSET_IRQ_FRAME_Q0 + 12 * 16)
#define OFFSET_IRQ_FRAME_Q13                    (OFFSET_IRQ_FRAME_Q0 + 13 * 16)
#define OFFSET_IRQ_FRAME_Q14                    (OFFSET_IRQ_FRAME_Q0 + 14 * 16)
#define OFFSET_IRQ_FRAME_Q15                    (OFFSET_IRQ_FRAME_Q0 + 15 * 16)
#define OFFSET_IRQ_FRAME_Q16                    (OFFSET_IRQ_FRAME_Q0 + 16 * 16)
#define OFFSET_IRQ_FRAME_Q17                    (OFFSET_IRQ_FRAME_Q0 + 17 * 16)
#define OFFSET_IRQ_FRAME_Q18                    (OFFSET_IRQ_FRAME_Q0 + 18 * 16)
#define OFFSET_IRQ_FRAME_Q19                    (OFFSET_IRQ_FRAME_Q0 + 19 * 16)
#define OFFSET_IRQ_FRAME_Q20                    (OFFSET_IRQ_FRAME_Q0 + 20 * 16)
#define OFFSET_IRQ_FRAME_Q21                    (OFFSET_IRQ_FRAME_Q0 + 21 * 16)
#define OFFSET_IRQ_FRAME_Q22                    (OFFSET_IRQ_FRAME_Q0 + 22 * 16)
#define OFFSET_IRQ_FRAME_Q23                    (OFFSET_IRQ_FRAME_Q0 + 23 * 16)
#define OFFSET_IRQ_FRAME_Q24                    (OFFSET_IRQ_FRAME_Q0 + 24 * 16)
#define OFFSET_IRQ_FRAME_Q25                    (OFFSET_IRQ_FRAME_Q0 + 25 * 16)
#define OFFSET_IRQ_FRAME_Q26                    (OFFSET_IRQ_FRAME_Q0 + 26 * 16)
#define OFFSET_IRQ_FRAME_Q27                    (OFFSET_IRQ_FRAME_Q0 + 27 * 16)
#define OFFSET_IRQ_FRAME_Q28                    (OFFSET_IRQ_FRAME_Q0 + 28 * 16)
#define OFFSET_IRQ_FRAME_Q29                    (OFFSET_IRQ_FRAME_Q0 + 29 * 16)
#define OFFSET_IRQ_FRAME_Q30                    (OFFSET_IRQ_FRAME_Q0 + 30 * 16)
#define OFFSET_IRQ_FRAME_Q31                    (OFFSET_IRQ_FRAME_Q0 + 31 * 16)

/******************************************************/
/* Offset of the thread entry point                   */
/******************************************************/


#define OFFSET_ENTRY_POINT                      (OFFSET_IRQ_FRAME_Q31       + (1 * 16))


#define OFFSET_X30_IPC                          (OFFSET_ENTRY_POINT  + (1 * 8))


#define OFFSET_DIRECT_IO_SAVE_ELR               (OFFSET_X30_IPC         + (1 * 8))


#define OFFSET_IO_ACCESS                        (OFFSET_DIRECT_IO_SAVE_ELR + (1 * 8))

/**
 * Mask to the MPIDR register to get the current running core
 */
#define PHAROS_CPU_MPIDR_AFFINITY_MASK          (0xC1000000U)


/**
 * @brief Stack size when starting up. 
 * 
 * Also corresponds to the idle thread stack size
 */
#define PHAROS_CPU_STARTUP_STACK_SIZE           (VALUE_4KiB)


/**
 * Logarithm value of PHAROS_CPU_STARTUP_STACK_SIZE. Needed to easily multiply by the stack size using only a left shift
 */
#define PHAROS_CPU_STARTUP_STACK_SIZE_LOG2      (12U)


/**
 * Separated "big" interrupt stack to support nested interrupts without stack overrun
 * Must be a power of 2 so that PHAROS_CPU_INTERRUPT_STACK_SIZE_LOG2 gives a "correct" value
 * If you change this value be sure to also change PHAROS_CPU_INTERRUPT_STACK_SIZE_LOG2.
 */
#define PHAROS_CPU_INTERRUPT_STACK_SIZE         (VALUE_64KiB)


/**
 * Logarithm value of PHAROS_CPU_INTERRUPT_STACK_SIZE. Needed to easily multiply by the stack size using only a left shift
 */
#define PHAROS_CPU_INTERRUPT_STACK_SIZE_LOG2    (16U)


/**
 * Don't accept higher priority interrupts if the interrupt stack is left down to 4 KiB
 */
#define PHAROS_CPU_INTERRUPT_STACK_MIN_SIZE     (VALUE_4KiB)


/**
 * Minimum stack size for a thread in user space
 */
#define PHAROS_STACK_MIN_SIZE                   (VALUE_4KiB)


/**
 * Minimum stack size for a thread in kernel space
 */
#define PHAROS_STACK_KERNEL_MIN_SIZE            (VALUE_4KiB)


/**
 * Minimum stack size for a shared stack 
 */
#define PHAROS_SHARED_STACK_SIZE_MINIMUM_SIZE   (VALUE_4KiB)


/**
 * SPSR register mask for the EL mode
 */
#define SPSR_EL_MASK        (0b1100)


/**
 * Value to place on the SPSR mode bits to place it on EL0
 */
#define SPSR_EL0_MODE       (0b0000)


/**
 * Value to place on the SPSR mode bits to place it on EL1
 */
#define SPSR_EL1_MODE       (0b0100)


/**
 * Value to place on the SPSR mode bits to place the SP used defined by the EL level the CPU is in
 */
#define SPSR_SP_MODE        (0b0001)


/**
 * Value to place on the SPSR mode bits to place it on EL2
 */
#define SPSR_EL2_MODE       (0b1000)


/**
 * Value to place on the SPSR mode bits to place it on EL3
 */
#define SPSR_EL3_MODE       (0b1100)


#define SPSR_RESTORE_EL1    (SPSR_EL1_MODE | SPSR_SP_MODE)


#define SPSR_IRQ_ENABLE     (0 << 7U)


#define SPSR_IRQ_DISABLE    (1 << 7U)

#define SPSR_FIQ_ENABLE     (0 << 6U)


#define SPSR_FIQ_DISABLE    (1 << 6U)


#define PHAROS_CORE_TABLE_RUNNING_THREAD_OFFSET     (0)
#define PHAROS_CORE_TABLE_DISPATCH_LEVEL_OFFSET     (PHAROS_CORE_TABLE_RUNNING_THREAD_OFFSET + __SIZEOF_POINTER__)
#define PHAROS_CORE_TABLE_INTERRUPT_LEVEL_OFFSET    (PHAROS_CORE_TABLE_DISPATCH_LEVEL_OFFSET + __SIZEOF_POINTER__)
#define PHAROS_CORE_TABLE_CONTEXT_NEEDED_OFFSET     (PHAROS_CORE_TABLE_INTERRUPT_LEVEL_OFFSET + __SIZEOF_POINTER__)


/* PHAROS_ASM_H */
#endif	
