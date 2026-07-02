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

.type pharosCpuSystemErrorHandler,          %function

.global pharosCpuSystemErrorHandler



.func pharosCpuSystemErrorHandler
pharosCpuSystemErrorHandler:

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

    /* store x30 (will be modified once we call a C function) */
    stp		x29 , x30 , [sp, #-16]!  

    /* get the runnning core to x10 */
    mrs     x10 , mpidr_el1

    /* load the mask to x1 to leave only the running core number */
    mov     x1 , PHAROS_CPU_MPIDR_AFFINITY_MASK

    /* clear all but the running core */
    bic     x10 , x10 , x1

    /* get the index on the array of the running core */
    lsl     x10 , x10 , PHAROS_CPU_CORE_RMP_SIZE_LOG2

    /* Increment thread dispatch disable level (disable dispatch inside the interrupt handler) */
    ldr     x2 , =pharosVRmpCore
 
    /* add the thread dispatch level offset */
    add     x2 , x2 , PHAROS_CORE_TABLE_DISPATCH_LEVEL_OFFSET

    /* calculate the final address where the thread dispatch level for the running core is on */
    add     x2 , x10 , x2
    
    /* get the dispatch level */
	ldr     x1 , [x2]

    /* increment the thread dispatch level */
	add     x1 , x1 , #1

    /* store it back */
	str     x1 , [x2]

    /* save SPSR and ELR to stack */

    /* get the ELR EL1 to x0 */
	mrs		x0 , ELR_EL1

    /* get the SPSR EL1 to x3 (will be the argument to pharosCpuCInterruptHandler) */
    mrs		x3 , SPSR_EL1

    /* read the FAR address to x1 (will be the memAddress argument to the pharosIInvalidMemAccess call) */
    mrs     x1 , FAR_EL1

    /* push SPSR and ELR to interrupt stack */
    stp     x0 , x3 , [sp , #-16]!
    
    /* enable interrupts (IRQ + FIQ) since we already saved the SPSR and ELR */
    msr     DAIFClr , #0b0011

    /* call the abort handler (x0 has already been set to the instruction address and x1 has the memAddress) */
	bl      pharosIInvalidMemAccess

    /* disable interrupts (IRQ + FIQ) to prevent an interrupt to mess with SPSR and ELR */
    msr     DAIFSet , #0b0011

    /* get the runnning core to x10 */
    mrs     x10 , mpidr_el1

    /* load the mask to x1 to leave only the running core number */
    mov     x1 , PHAROS_CPU_MPIDR_AFFINITY_MASK

    /* clear all but the running core */
    bic     x10 , x10 , x1

    /* get the index on the array of the running core */
    lsl     x10 , x10 , PHAROS_CPU_CORE_RMP_SIZE_LOG2

    /* decrement thread dispatch disable level (to restore it to its initial value) */
    ldr     x2 , =pharosVRmpCore
 
    /* add the thread dispatch level offset */
    add     x2 , x2 , PHAROS_CORE_TABLE_DISPATCH_LEVEL_OFFSET

    /* calculate the final address where the thread dispatch level for the running core is on */
    add     x2 , x10 , x2

    /* get the thread dispatch level */
	ldr     x1 , [x2]

    /* decrement thread dispatch level */
	sub     x1 , x1 , #1

    /* store the thread dispatch level */
	str     x1 , [x2]

    /* dispatch the thread inside the interrupt (leaving with interrupts disabled) */
	bl      pharosIDispatchThread

    /* will only reach here if the thread that originated the fault is using a direct I/O */

    /* restore the registers saved to the stack */
     
    /* get SPSR and ELR from stack  */
    ldp     x9 , x10 , [sp] , #16

    /*********************************************************************/
    /* Get the running thread of the running core to x0                  */
    /*********************************************************************/

    /* get the runnning core to x0 */
    mrs     x0 , mpidr_el1

    /* load the mask to x1 to leave only the running core number */
    mov     x1 , PHAROS_CPU_MPIDR_AFFINITY_MASK

    /* clear all but the running core */
    bic     x0 , x0 , x1

    /* get the index on the array of the running core */
    lsl     x0 , x0 , PHAROS_CPU_CORE_RMP_SIZE_LOG2

    /* get the address of the array of running threads */
	ldr     x1 , =pharosVRmpCore

    /* calculate the final address where the running thread for the running core is on */
    add     x0 , x0 , x1

    /* get the running thread */
    ldr     x0 , [x0]

    /****************************************************************/
    /* set the return address (ELR) to execute the IO direct memory */
    /****************************************************************/

    /* set the return address to the direct I/O memory access */
    add     x0 , x0 , #OFFSET_IO_ACCESS

    /* set the return address to the IO direct memory */
	msr		ELR_EL1 , x0

    /*********************************************************************/
    /* change SPSR to return in EL1 mode to execute the IO direct memory */
    /*********************************************************************/

    /* x3 contains the original SPSR at this point, we will set it to return to EL1 mode now */
    orr     x10 , x10 , #4

    /* set the SPSR to return from the exception to the EL1 mode (all remaining flags are unchanged) */
    msr		SPSR_EL1 , x10
   
    /* restore remaining registers from stack */
    ldp		x29 , x30 , [sp] , #16 
    ldp		x9 , x10 , [sp] , #16 
    ldp		x6 , x7 , [sp] , #16  
    ldp		x4 , x5 , [sp] , #16
    ldp		x2 , x3 , [sp] , #16
    ldp		x0 , x1 , [sp] , #16
    
    /* return from interrupt */
    eret

.endfunc
