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


#include <pharos/hal/cpu/asm.h>
#include <pharos/hal/cpu/conf.h>

.section .textStart 

.global pharosCpuStart
.global pharosCpuCoreHalt
.global pharosICoreRunning
.global pharosCpuBoot
.global pharosCpuSpGet
.global pharosCpuMTimerInterruptDisable
.global pharosCpuMTimerInterruptEnable
.global pharosCpuMipGet
.global pharosCpuMieSetBit


.func pharosCpuStart
pharosCpuStart:


    /***************************************************/
    /* Initialize all registers                        */
    /***************************************************/
    li      x1 , 0
    li      x2 , 0
    li      x3 , 0
    li      x4 , 0
    li      x5 , 0
    li      x6 , 0
    li      x7 , 0
    li      x8 , 0
    li      x9 , 0
    li      x10 , 0
    li      x11 , 0
    li      x13 , 0
    li      x14 , 0
    li      x15 , 0
    li      x16 , 0
    li      x17 , 0
    li      x18 , 0
    li      x19 , 0
    li      x20 , 0
    li      x21 , 0
    li      x22 , 0
    li      x23 , 0
    li      x24 , 0
    li      x25 , 0
    li      x26 , 0
    li      x27 , 0
    li      x28 , 0
    li      x29 , 0
    li      x30 , 0

#ifdef PHAROS_CPU_FP

    li      t0 , PHAROS_CPU_MSTATUS_FS_MASK | PHAROS_CPU_MSTATUS_XS_MASK

    csrs    mstatus , t0

    fssr    x0

    fmv.s.x f0 , x0
    fmv.s.x f1 , x0
    fmv.s.x f2 , x0
    fmv.s.x f3 , x0
    fmv.s.x f4 , x0
    fmv.s.x f5 , x0
    fmv.s.x f6 , x0
    fmv.s.x f7 , x0
    fmv.s.x f8 , x0
    fmv.s.x f9 , x0
    fmv.s.x f10 , x0
    fmv.s.x f11 , x0
    fmv.s.x f12 , x0
    fmv.s.x f13 , x0
    fmv.s.x f14 , x0
    fmv.s.x f15 , x0
    fmv.s.x f16 , x0
    fmv.s.x f17 , x0
    fmv.s.x f18 , x0
    fmv.s.x f19 , x0
    fmv.s.x f20 , x0
    fmv.s.x f21 , x0
    fmv.s.x f22 , x0
    fmv.s.x f23 , x0
    fmv.s.x f24 , x0
    fmv.s.x f25 , x0
    fmv.s.x f26 , x0
    fmv.s.x f27 , x0
    fmv.s.x f28 , x0
    fmv.s.x f29 , x0
    fmv.s.x f30 , x0
    fmv.s.x f31 , x0

#endif

    /***************************************************/
    /* Set the trap table                              */
    /***************************************************/

    /* load the address of the trap table */
    la      t0 , pharosCpuTrapHandler

    /* set the mtvec to the trap table (MODE = 0 which means don't use the vectored approach) */
    csrw    mtvec , t0

    /***************************************************/
    /* Set the stack pointer based on the running core */
    /***************************************************/

    /* get the running core number */
    csrr    t0 , mhartid

    /* increment the core number by one */
    add     t0 , t0 , 1
 
    /* store the base stack address (stack grows down) for core 0 */
    la      t1 , pharosVStartupStack 

    /* t0 = (core + 1) * stack size */
    /* if core == 0 => t0 = 1 * PHAROS_CPU_STARTUP_STACK_SIZE */
    /* if core == 1 => t0 = 2 * PHAROS_CPU_STARTUP_STACK_SIZE */
    slli    t0 , t0 , PHAROS_CPU_STARTUP_STACK_SIZE_LOG2
    
    /* t0 = (core + 1) * stack size */
    /* if core == 0 => t0 = pharosVStartupStack + 1 * PHAROS_CPU_STARTUP_STACK_SIZE */
    /* if core == 1 => t0 = pharosVStartupStack + 2 * PHAROS_CPU_STARTUP_STACK_SIZE */
    add     sp , t0 , t1
 
    /***************************************************/
    /* Initialize MIE register                         */
    /***************************************************/

    /* enable the external interrupt and timer interrupt (software interrupt remains disabled) in all machine mode */
    li      t0 , PHAROS_CPU_MIE_MEIE | PHAROS_CPU_MIE_MTIE 

    /* set those bits on the mie register */
    csrs    mie , t0

    /***************************************************/
    /* Jump to boot                                    */
    /***************************************************/
  
    /* jump to C code */
    j     pharosCpuBoot

.endfunc


.func pharosCpuCoreHalt
pharosCpuCoreHalt:

     /* wait for interrupt */
    wfi

    /* loop forever */
    j       pharosCpuCoreHalt

.endfunc



.func pharosICoreRunning
pharosICoreRunning:

    /* get the runnning core to a0 */
    csrr    a0, mhartid

    /* leave the function */
    ret

.endfunc


.func pharosCpuSpGet
pharosCpuSpGet:

    /* return the stack pointer */
    mv  a0 , sp

    /* leave the function */
    ret

.endfunc



.func pharosCpuMipGet
pharosCpuMipGet:

    /* return the MIP register */
    csrr  a0 , mip

    /* leave the function */
    ret

.endfunc


.func pharosCpuMieSetBit
pharosCpuMieSetBit:

    /* set the bits on the MIE register */
    csrs  mie , a0 

    /* leave the function */
    ret

.endfunc


.func pharosCpuMTimerInterruptDisable
pharosCpuMTimerInterruptDisable:

    /* place the MTIE bit */
    li      a0 , 1 << 7

    /* clear the MTIE bit in the mie register */
    csrc    mie , a0

    /* leave the function */
    ret

.endfunc


.func pharosCpuMTimerInterruptEnable
pharosCpuMTimerInterruptEnable:

    /* place the MTIE bit */
    li      a0 , 1 << 7

    /* set the MTIE bit in the mie register */
    csrs    mie , a0

    /* leave the function */
    ret

.endfunc
