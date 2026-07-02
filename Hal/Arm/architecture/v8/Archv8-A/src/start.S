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


/**
 * @file
 *
 * @brief Boot and system start code in ARM mode.
 */

/* External symbols */
#include <pharos/hal/cpu/asm.h>

#define OSC_FREQ		19200000

.type pharosCpuStart,                           %function 
.type pharosICoreRunning,                       %function
.type pharosCpuCoreHalt,                        %function
.type pharosCpuRestart,                        %function


.global pharosCpuStart
.global pharosICoreRunning
.global pharosCpuCoreHalt
.global pharosCpuRestart


.section .textStart

.func pharosCpuStart
pharosCpuStart:

    /* initialize registers */
    mov     x0 , #0
    mov     x1 , #0
    mov     x2 , #0
    mov     x3 , #0
    mov     x4 , #0
    mov     x5 , #0
    mov     x6 , #0
    mov     x7 , #0
    mov     x8 , #0
    mov     x9 , #0
    mov     x10 , #0
    mov     x11 , #0
    mov     x13 , #0
    mov     x14 , #0
    mov     x15 , #0
    mov     x16 , #0
    mov     x17 , #0
    mov     x18 , #0
    mov     x19 , #0
    mov     x20 , #0
    mov     x21 , #0
    mov     x22 , #0
    mov     x23 , #0
    mov     x24 , #0
    mov     x25 , #0
    mov     x26 , #0
    mov     x27 , #0
    mov     x28 , #0
    mov     x29 , #0
    mov     x30 , #0

    //ldr x0 , =19200000
    //msr CNTFRQ_EL0, x0

    /* load the exception vector table to x0 */
    ldr     x0, =pharosCpuExceptionVectorTable

    /* set the exception table */
    msr     VBAR_EL2, x0

    /***************************************************/
    /* Set the stack pointer based on the running core */
    /***************************************************/

    /* get the runnning core to x0 */
    mrs     x0 , mpidr_el1

    /* load the mask to x1 to leave only the running core number */
    mov     x1 , PHAROS_CPU_MPIDR_AFFINITY_MASK

    /* clear all but the running core */
    bic     x0 , x0 , x1

    /* increment the core number by one */
    add     x0 , x0 , 1

    /* get to x2 the stack size */
    ldr     x2 , =PHAROS_CPU_STARTUP_STACK_SIZE_LOG2

    /* x0 = (core + 1) * stack size */
    /* if core == 0 => x0 = 1 * PHAROS_CPU_STARTUP_STACK_SIZE */
    /* if core == 1 => x0 = 2 * PHAROS_CPU_STARTUP_STACK_SIZE */
    lsl     x0 , x0 , x2
    
    /* load the start of the stack */
    ldr     x1 , =pharosVStartupStack

    /* x0 = (core + 1) * stack size */
    /* if core == 0 => x0 = pharosVStartupStack + 1 * PHAROS_CPU_STARTUP_STACK_SIZE */
    /* if core == 1 => x0 = pharosVStartupStack + 2 * PHAROS_CPU_STARTUP_STACK_SIZE */
    add     sp , x0 , x1
    
    /***************************************************/
    /* Switch to EL2                                   */
    /***************************************************/

    /* read the ID_AA64PFR0_EL1 register */
    mrs     x0 , ID_AA64PFR0_EL1

    /* get to x1 the bits stating if EL3 is supported */
    lsr     x1 , x0 , #12

    /* clear remaining bits */
    and     x1 , x1 , #0xf

    /* check if EL3 is supported */
    cmp     x1 , #0

    /* if x1 == 0 then EL3 is not supported */
    beq     checkEl2
  
    /* get current execution level (EL) */
    mrs     x0 , CurrentEL

    /* shift by 2 bits the current el register */
    lsr     x0 , x0, #2

    /* check if running on EL3 */
    cmp     x0 , #3
    
    /* not running at EL3, check if running at EL2 */
    bne     checkEl2
    
    /* if we are here we are running at EL3 with the highest EL level */
   
    /* read the ID_AA64PFR0_EL1 register */
    mrs     x0 , ID_AA64PFR0_EL1

    /* get current execution level (EL) */
    mrs     x1 , CurrentEL

    /* disable MMU for EL2 */
    msr     SCTLR_EL2, xzr

    /* clear x1 */
    mov     x1, xzr

    /* set x1 with aarch64 and non-secure state */
    orr     x1 , x1 , #(1 << 10)

    orr     x1 , x1 , #(1 << 0)
    
    /* set the state of EL3 */
    msr     SCR_EL3, x1

    /* clear x1 again */
    mov     x1, xzr

    /* SErrors, IRQs, and FIQs will all be masked, Return to AArch64 state, Return to EL2,  Use EL2's dedicated stack pointer */
    orr     x1, x1, #(7 << 6) 
    orr     x1, x1, #(1 << 3)
    orr     x1, x1, #(1 << 0)
  
    /* set EL3 SPSR return (we will do an ERET instruction next so we must set up the SPSR of EL3 so we return from the exception to EL2) */
    msr     SPSR_EL3, x1

    /* load the address of checkEl2 into x1 */
    adr     x1 , checkEl2

    /* set the exception LR register of EL3 to go to checkEl2 */
    msr     ELR_EL3, x1
    
    /* return from exception which will cause to go to EL2 and to instruction checkEl2 in A64 */
    eret

checkEl2:

    /* read the ID_AA64PFR0_EL1 register */
    mrs     x0 , ID_AA64PFR0_EL1

    /* get to x1 the bits stating if EL2 is supported */
    lsr     x1 , x0 , #8

    /* clear remaining bits */
    and     x1 , x1 , #0xf

    /* check if EL2 is supported */
    cmp     x1 , #0
 
    /* if EL2 is not supported then go to EL1 */
    beq     runningEl1

    /* get current execution level (EL) */
    mrs     x0 , CurrentEL

    /* shift by 2 bits the current el register */
    lsr     x0 , x0, #2

    /* check if running on EL2 */
    cmp     x0 , #2

    /* not running at EL2, must be running at EL1 */
    bne     runningEl1

    /* if we are here then we are running at EL2 */

    /* clear the virtual counter offset */
    msr     CNTVOFF_EL2, xzr

    /* load the address of pharosCpuStart into x1 */
    adr     x1 , pharosCpuStart

    /* read the ID_AA64PFR0_EL1 register */
    mrs     x0 , ID_AA64PFR0_EL1

    /* get current execution level (EL) */
    mrs     x1 , CurrentEL

    /* clear x1 */
    mov     x1, xzr

    /* set x1 with aarch64 and secure state */
    orr     x1 , x1 , #(1 << 31)
    orr     x1 , x1 , #(1 << 1)         
    
    /* set the state of EL2 */
    msr     HCR_EL2 , x1

    /* clear x1 again to store x1 in the SPSR_EL2 */
    mov     x1, xzr

    /* disable IRQ */
    orr     x1, x1, #(1 << 7) 
    
    /* disable FIQ */
    orr     x1, x1, #(1 << 6) 
    
    /* return to EL1 */
    orr     x1, x1, #(1 << 2) 

    /* use SP of EL1 */
    orr     x1, x1, #(1 << 0) 

    /* set EL2 SPSR return (we will do an ERET instruction next so we must set up the SPSR of EL2 so we return from the exception to EL1) */
    msr     SPSR_EL2, x1
 
    /* load the exception vector table to x0 */
    ldr     x0, =pharosCpuExceptionVectorTable

    /* set the exception table for EL1 */
    msr     VBAR_EL2, x0

    /* setup SCTLR access */
    mov     x2, #0x0800
    movk    x2, #0x30d0, lsl #16
    msr     sctlr_el1, x2

    
    /* load the address of checkEl2 into x1 */
    adr     x1 , runningEl1

    /* set the exception LR register of EL2 to go to runningEl1 */
    msr     ELR_EL2, x1

    /* return from exception which will cause to go to EL1 and to instruction runningEl1 in A64 */
    eret

runningEl1:

    /* we are running in EL1 */
    
    /* set the FP trap disable for EL1 and EL0 */
    mov     x0 , 3 << 20

    /* set the FP trap disable for EL1 and EL0 */
    msr     CPACR_EL1, x0 
    
    /* disable MMU for EL1 */
    msr     SCTLR_EL1, xzr
 

    /* get the runnning core to x0 */
    mrs     x0 , mpidr_el1

    /* load the mask to x1 to leave only the running core number */
    mov     x1 , PHAROS_CPU_MPIDR_AFFINITY_MASK

    /* clear all but the running core */
    bic     x0 , x0 , x1

    /* get to x2 the stack size */
    ldr     x2 , =PHAROS_CPU_STARTUP_STACK_SIZE

    /* increment the core number by one */
    add     x0 , x0 , 1

    /* x0 = (core + 1) * stack size */
    /* if core == 0 => x0 = 1 * PHAROS_CPU_STARTUP_STACK_SIZE */
    /* if core == 1 => x0 = 2 * PHAROS_CPU_STARTUP_STACK_SIZE */
    mul     x0 , x0 , x2
    
    /* load the start of the stack */
    ldr     x1 , =pharosVStartupStack

    /* x0 = (core + 1) * stack size */
    /* if core == 0 => x0 = pharosVStartupStack + 1 * PHAROS_CPU_STARTUP_STACK_SIZE */
    /* if core == 1 => x0 = pharosVStartupStack + 2 * PHAROS_CPU_STARTUP_STACK_SIZE */
    add     x0 , x0 , x1
    
    /* place the stack pointer */
    mov     sp , x0

    /* load the exception vector table to x0 */
    ldr     x0, =pharosCpuExceptionVectorTable

    /* set the exception table for EL1 */
    msr     VBAR_EL1, x0

    /* jump to C code */
    bl      pharosCpuBoot

    /* if somehow CPU boot returns, just call the shutdown */
    bl      pharosCpuShutdown

.endfunc


.func pharosICoreRunning
pharosICoreRunning:

    /* get the runnning core to x0 */
    mrs     x0 , mpidr_el1

    /* load the mask to x1 to leave only the running core number */
    mov     x1 , PHAROS_CPU_MPIDR_AFFINITY_MASK

    /* clear all but the running core */
    bic     x0 , x0 , x1

    /* leave the function */
    ret

.endfunc


.func pharosCpuCoreHalt
pharosCpuCoreHalt:

    /* wait for interrupt */
    wfe

    /* loop forever */
    b pharosCpuCoreHalt

.endfunc


    .align 11
pharosCpuExceptionVectorTable:
    
    /**********************************************/
    /* Current EL with SP0                        */
    /**********************************************/
    
    /* synchronous exception */
    .align  7
    b       pharosCpuSyscallHandler

    /* asynchronous IRQ exception */
    .align  7
    b       pharosCpuIrqHandler
  
    /* asynchronous FIQ exception */
    .align  7
    b       pharosCpuIrqHandler
  
    /* System error */
    .align  7
    b       pharosCpuSystemErrorHandler
 
    /**********************************************/
    /* Current EL with SPx                        */
    /**********************************************/
    
    /* synchronous exception */
    .align  7
    b       pharosCpuSyscallHandler
   
    /* asynchronous IRQ exception */
    .align  7
    b       pharosCpuIrqHandler
   
    /* asynchronous FIQ exception */
    .align  7
    b       pharosCpuIrqHandler

    /* System error */
    .align  7
    b       pharosCpuSystemErrorHandler
  
    /**********************************************/
    /* Lower EL using AArch64                     */
    /**********************************************/
    
    /* synchronous exception */
    .align  7
    b       pharosCpuSyscallHandler
   
    /* asynchronous IRQ exception */
    .align  7
    b       pharosCpuIrqHandler
  
    /* asynchronous FIQ exception */
    .align  7
    b       pharosCpuIrqHandler
  
    /* System error */
    .align  7
    b       pharosCpuSystemErrorHandler
  
    /**********************************************/
    /* Lower EL using AArch32                     */
    /**********************************************/
    
    /* synchronous exception */
    .align  7
    b       pharosCpuSyscallHandler
  
    /* asynchronous IRQ exception */
    .align  7
    b       pharosCpuIrqHandler
  
    /* asynchronous FIQ exception */
    .align  7
    b       pharosCpuIrqHandler
  
    /* System error */
    .align  7
    b       pharosCpuSystemErrorHandler

    /* synchronous exception */
    .align  7
    b       pharosCpuSyscallHandler
  
    /* asynchronous IRQ exception */
    .align  7
    b       pharosCpuIrqHandler
  
    /* asynchronous FIQ exception */
    .align  7
    b       pharosCpuIrqHandler
  
    /* System error */
    .align  7
    b       pharosCpuSystemErrorHandler
  
    /* synchronous exception */
    .align  7
    b       pharosCpuSyscallHandler
  
    /* asynchronous IRQ exception */
    .align  7
    b       pharosCpuIrqHandler
  
    /* asynchronous FIQ exception */
    .align  7
    b       pharosCpuIrqHandler
  
    /* System error */
    .align  7
    b       pharosCpuSystemErrorHandler
  

.func pharosCpuRestart
pharosCpuRestart:
    /* TODO: this does not actually restart all cores, even the running core is not correctly restarted */
    b       pharosCpuStart
