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
#include <pharos/kernel/core/syscall.h>


.type pharosCpuIrqHandler,                           %function 

.global pharosCpuIrqHandler
.global pharosCpuInterruptStack


.section .text


.func pharosCpuIrqHandler
pharosCpuIrqHandler:

    /* when we reach here we are running at EL1, 
     * stack pointer is currently pointing to the running thread kernel stack 
     * interrupts are disable */

    /* save the register that might change when we call a C function to the current stack 
     * (at this point may be the running thread stack or the interrupt stack) */
    stp		x0 , x1 , [sp, #-16]!
    stp		x2 , x3 , [sp, #-16]!
    stp		x4 , x5 , [sp, #-16]!
    stp		x6 , x7 , [sp, #-16]!  

    /* also save some registers that are maintained accross C calls to store the stack pointer */
    stp		x9 , x10 , [sp, #-16]!  

    /* check if nested interrupt */

    /* load the nest level address */
    ldr     x0, =pharosVInterruptNestingLevel
    
    /* load to x0 the nest level value */
    ldr     w1, [x0]

    /* increment the nest level */
    add     w1 , w1 , #1

    /* store it back in memory */
    str     w1 , [x0]

    /* compare if not 1 */
    cmp     w1 , #1

    /* if the nest level is != 1 then we are in a nested interrupt */
    bne     nestedInterrupt
    
    /* if not nested interrupt then switch to the interrupt stack */

    /* save the current stack pointer (to a local variable that will be saved when we call a C function) */
    mov     x9 , sp

    /* switch the supervisor stack to the IRQ stack */
    ldr     x2 , =pharosCpuInterruptStack + PHAROS_CPU_INTERRUPT_STACK_SIZE

    /* switch the stack */
    mov     sp , x2

    /* save SPSR and ELR to interrupt stack */

    /* get the SPSR EL1 to x2 */
    mrs		x2 , SPSR_EL1

    /* get the ELR EL1 to x3 */
	mrs		x3 , ELR_EL1

    /* push SPSR and ELR to interrupt stack */
    stp     x2 , x3 , [sp , #-16]!

    /* get the interrupt that was triggered */
    mrs 	x0 , ICC_AP1R0_EL1

    /* get interrupt source */
    

    /* clear interrupt */


    /* enable interrupts */


    /* process */


    /* disable interrupts */

    /* load the nest level address */
    ldr     x0, =pharosVInterruptNestingLevel
    
    /* load to x0 the nest level value */
    ldr     w1, [x0]

    /* decrement the nest level */
    sub     w1 , w1 , #1

    /* store it back in memory */
    str     w1 , [x0]

    /* compare if 0 */
    cmp     w1 , #0

    

    /* context switch if needed */

    /* restore SPSR and ELR from stack */

    /* restore to thread kernel stack */

    /* return */
    eret
    

nestedInterrupt:

    ldr     x0, =pharosVInterruptNestingLevel
    
    /* load to x0 the nest level value */
    ldr     w1, [x0]

    /* decrement the nest level */
    sub     w1 , w1 , #1

    /* store it back in memory */
    str     w1 , [x0]

    /* compare if 0 */
    cmp     w1 , #0

    
    /* return */
    eret
    

.endfunc
