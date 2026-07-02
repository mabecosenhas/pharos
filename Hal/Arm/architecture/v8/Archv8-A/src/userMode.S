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

.type pharosCpuExecuteUserModeAper,         %function
.type pharosCpuExecuteUserModePer,          %function
.type pharosCpuExecuteUserModeEvent,        %function
.type pharosCpuExecuteUserModeMutex,        %function
.type pharosCpuExecuteUserModeLwQueue,      %function
.type pharosCpuExecuteUserModeHwQueue,      %function
.type pharosCpuExecuteUserModeChannel,      %function
.type pharosCpuExecuteUserModeIdle,         %function

.global pharosCpuExecuteUserModeAper
.global pharosCpuExecuteUserModePer
.global pharosCpuExecuteUserModeEvent
.global pharosCpuExecuteUserModeMutex
.global pharosCpuExecuteUserModeLwQueue
.global pharosCpuExecuteUserModeHwQueue
.global pharosCpuExecuteUserModeChannel
.global pharosCpuExecuteUserModeIdle

/**********************************************************************************************************/
/* labels that the syscall internal handlers will use to make sure the syscall is made at the right place */
/**********************************************************************************************************/

.global pharosAperiodicThreadAddress
.global pharosPeriodicThreadAddress
.global pharosSporadicThreadEndChannel
.global pharosSporadicThreadEndHwQueue
.global pharosSporadicThreadEndLwQueue
.global pharosSporadicThreadEndMutex
.global pharosSporadicThreadEndEvent


.text

/**
 * void pharosCpuExecuteUserModeIdle();
 */
.func pharosCpuExecuteUserModeIdle
pharosCpuExecuteUserModeIdle:

    /* enable interrupts (IRQ + FIQ) */
    msr     DAIFClr , #0b0011

    /* save the local registers: don't assume the function will not change them even though it shouldn't 
     * (idle thread could be replaced by the application so we cannot trust it) */
    stp     x8  , x9  , [sp, #-16]!
    stp     x10 , x11 , [sp, #-16]!
    stp     x12 , x13 , [sp, #-16]!
    stp     x14 , x15 , [sp, #-16]!
    stp     x16 , x17 , [sp, #-16]!
    stp     x18 , x19 , [sp, #-16]!
    stp     x20 , x21 , [sp, #-16]!
    stp     x22 , x23 , [sp, #-16]!
    stp     x24 , x25 , [sp, #-16]!
    stp     x26 , x27 , [sp, #-16]!
    stp     x28 , x29 , [sp, #-16]!
    
    /* load the address of idle thread body into x1 */
    adr     x1 , pharosIIdleThreadBody

    /* jump to idle thread (this will not return) */
    blr     x1

.endfunc


/**
 * void pharosCpuExecuteUserModeAper(void *partitionData , ptrThreadInfo thread);
 */
.func pharosCpuExecuteUserModeAper
pharosCpuExecuteUserModeAper:
    
    /* when we arrive here we are at EL1 mode */

    /* save x30 to the stack (will be modified by the call) */
    str     x30 , [sp, #-16]!

    /* save the local registers: don't assume the handler will not change them even though it shouldn't */
    stp     x8  , x9  , [sp, #-16]!
    stp     x10 , x11 , [sp, #-16]!
    stp     x12 , x13 , [sp, #-16]!
    stp     x14 , x15 , [sp, #-16]!
    stp     x16 , x17 , [sp, #-16]!
    stp     x18 , x19 , [sp, #-16]!
    stp     x20 , x21 , [sp, #-16]!
    stp     x22 , x23 , [sp, #-16]!
    stp     x24 , x25 , [sp, #-16]!
    stp     x26 , x27 , [sp, #-16]!
    stp     x28 , x29 , [sp, #-16]!
    
    /*************************************************************************/
    /* switch to EL0                                                         */
    /*************************************************************************/
 
    /* disable interrupts (if an interrupt occurs it will override the SPSR and ELR and we must protect them */
    msr     DAIFSet , #0b0011

    /* set EL1 SPSR to go to EL0 (we will do an ERET instruction next so we must set up the SPSR of EL0 so we return from the exception to EL0) */
    msr     SPSR_EL1, xzr

    /* load the address of checkEl2 into z1 */
    adr     x7 , pharosCpuExecuteUserModeAperCall

    /* set the exception LR register of EL3 to go to pharosCpuExecuteUserModeAperCall */
    msr     ELR_EL1, x7

    /* get the entry point to x7 (when we leave this function) */
    ldr     x7 , [x1 , #OFFSET_ENTRY_POINT]

    /* return from exception which will cause to go to EL0 and to instruction pharosCpuExecuteUserModeAperCall in A64 */
    /* note: this will re-enable interrupts since SPSR == 0 */
    eret

pharosCpuExecuteUserModeAperCall:

    /* jump to the entry point of the thread (no arguments needed) */
    blr     x7

    /*************************************************************************/
    /* Switch to EL1 mode with a syscall made in a well-known address        */
    /*************************************************************************/
 
    /* make the syscall to restore supervisor mode */
    svc     PHAROS_SYSCALL_APERIODIC_THREAD_ENDED

     /* define the label position just after the swi call */
pharosAperiodicThreadAddress:

    /* now we are in EL1 again */

    /* restore the local registers */
    ldp     x28 , x29 , [sp], #16
    ldp     x26 , x27 , [sp], #16
    ldp     x24 , x25 , [sp], #16
    ldp     x22 , x23 , [sp], #16
    ldp     x20 , x21 , [sp], #16
    ldp     x18 , x19 , [sp], #16
    ldp     x16 , x17 , [sp], #16
    ldp     x14 , x15 , [sp], #16
    ldp     x12 , x13 , [sp], #16
    ldp     x10 , x11 , [sp], #16
    ldp     x8  , x9  , [sp], #16

    /* restore x30 from the stack */
    ldr     x30 , [sp], #16

    /* return from function */
    ret 

.endfunc


/**
 * void pharosCpuExecuteUserModePer(void *partitionData , ptrThreadInfo thread);
 */
.func pharosCpuExecuteUserModePer
pharosCpuExecuteUserModePer:
    
    /* when we arrive here we are at EL1 mode */

    /* save x30 to the stack (will be modified by the call) */
    str     x30 , [sp, #-16]!

    /* save the local registers: don't assume the handler will not change them even though it shouldn't */
    stp     x8  , x9  , [sp, #-16]!
    stp     x10 , x11 , [sp, #-16]!
    stp     x12 , x13 , [sp, #-16]!
    stp     x14 , x15 , [sp, #-16]!
    stp     x16 , x17 , [sp, #-16]!
    stp     x18 , x19 , [sp, #-16]!
    stp     x20 , x21 , [sp, #-16]!
    stp     x22 , x23 , [sp, #-16]!
    stp     x24 , x25 , [sp, #-16]!
    stp     x26 , x27 , [sp, #-16]!
    stp     x28 , x29 , [sp, #-16]!
    
    /*************************************************************************/
    /* switch to EL0                                                         */
    /*************************************************************************/

    /* disable interrupts (if an interrupt occurs it will override the SPSR and ELR and we must protect them */
    msr     DAIFSet , #0b0011

    /* set EL1 SPSR return (we will do an ERET instruction next so we must set up the SPSR of EL0 so we return from the exception to EL0) */
    msr     SPSR_EL1, xzr

    /* load the address of checkEl2 into z1 */
    adr     x7 , pharosCpuExecuteUserModePerCall

    /* set the exception LR register of EL3 to go to pharosCpuExecuteUserModePerCall */
    msr     ELR_EL1, x7

    /* get the entry point to x7 (when we leave this function) */
    ldr     x7 , [x1 , #OFFSET_ENTRY_POINT]

    /* return from exception which will cause to go to EL0 and to instruction pharosCpuExecuteUserModePerCall in A64 */
    /* note: this will re-enable interrupts since SPSR == 0 */
    eret

pharosCpuExecuteUserModePerCall:

    /* jump to the entry point of the thread (no arguments needed) */
    blr     x7

    /*************************************************************************/
    /* Switch to EL1 mode with a syscall made in a well-known address        */
    /*************************************************************************/
 
    /* make the syscall to restore supervisor mode */
    svc     PHAROS_SYSCALL_PERIODIC_THREAD_ENDED

     /* define the label position just after the swi call */
pharosPeriodicThreadAddress:

    /* now we are in EL1 again */
 
    /* restore the local registers */
    ldp     x28 , x29 , [sp], #16
    ldp     x26 , x27 , [sp], #16
    ldp     x24 , x25 , [sp], #16
    ldp     x22 , x23 , [sp], #16
    ldp     x20 , x21 , [sp], #16
    ldp     x18 , x19 , [sp], #16
    ldp     x16 , x17 , [sp], #16
    ldp     x14 , x15 , [sp], #16
    ldp     x12 , x13 , [sp], #16
    ldp     x10 , x11 , [sp], #16
    ldp     x8  , x9  , [sp], #16

    /* restore x30 from the stack */
    ldr     x30 , [sp], #16

    /* return from function */
    ret 


.endfunc


/**
 * void pharosCpuExecuteUserModeMutex(void *partitionData , SemaphoreMutexNumber ssId , ptrThreadInfo thread);
 */
.func pharosCpuExecuteUserModeMutex
pharosCpuExecuteUserModeMutex:
    
    /* when we arrive here we are at EL1 mode */

    /* save x30 to the stack (will be modified by the call) */
    str     x30 , [sp, #-16]!

    /* save the local registers: don't assume the handler will not change them even though it shouldn't */
    stp     x8  , x9  , [sp, #-16]!
    stp     x10 , x11 , [sp, #-16]!
    stp     x12 , x13 , [sp, #-16]!
    stp     x14 , x15 , [sp, #-16]!
    stp     x16 , x17 , [sp, #-16]!
    stp     x18 , x19 , [sp, #-16]!
    stp     x20 , x21 , [sp, #-16]!
    stp     x22 , x23 , [sp, #-16]!
    stp     x24 , x25 , [sp, #-16]!
    stp     x26 , x27 , [sp, #-16]!
    stp     x28 , x29 , [sp, #-16]!
   
    /*************************************************************************/
    /* switch to EL0                                                         */
    /*************************************************************************/

    /* disable interrupts (if an interrupt occurs it will override the SPSR and ELR and we must protect them */
    msr     DAIFSet , #0b0011

    /* set EL1 SPSR return (we will do an ERET instruction next so we must set up the SPSR of EL0 so we return from the exception to EL0) */
    msr     SPSR_EL1, xzr

    /* load the address of checkEl2 into z1 */
    adr     x7 , pharosCpuExecuteUserModeMutexCall

    /* set the exception LR register of EL3 to go to pharosCpuExecuteUserModeMutexCall */
    msr     ELR_EL1, x7

    /* get the entry point to x7 (when we leave this function) */
    ldr     x7 , [x2 , #OFFSET_ENTRY_POINT]

    /* return from exception which will cause to go to EL0 and to instruction pharosCpuExecuteUserModeMutexCall in A64 */
    /* note: this will re-enable interrupts since SPSR == 0 */
    eret

pharosCpuExecuteUserModeMutexCall:

    /* jump to the entry point of the thread (x0 and x1 already have the arguments) */
    blr     x7
 
    /*************************************************************************/
    /* Switch to EL1 mode with a syscall made in a well-known address        */
    /*************************************************************************/

    /* make the syscall to restore supervisor mode */
    svc     PHAROS_SYSCALL_SPORADIC_THREAD_ENDED

     /* define the label position just after the swi call */
pharosSporadicThreadEndMutex:

    /* now we are in EL1 again */
 
    /* restore the local registers */
    ldp     x28 , x29 , [sp], #16
    ldp     x26 , x27 , [sp], #16
    ldp     x24 , x25 , [sp], #16
    ldp     x22 , x23 , [sp], #16
    ldp     x20 , x21 , [sp], #16
    ldp     x18 , x19 , [sp], #16
    ldp     x16 , x17 , [sp], #16
    ldp     x14 , x15 , [sp], #16
    ldp     x12 , x13 , [sp], #16
    ldp     x10 , x11 , [sp], #16
    ldp     x8  , x9  , [sp], #16

    /* restore x30 from the stack */
    ldr     x30 , [sp], #16

    /* return from function */
    ret 

.endfunc


/**
 * void pharosCpuExecuteUserModeEvent(void *partitionData , Event event , ptrThreadInfo thread);
 */
.func pharosCpuExecuteUserModeEvent
pharosCpuExecuteUserModeEvent:

    /* when we arrive here we are at EL1 mode */

    /* save x30 to the stack (will be modified by the call) */
    str     x30 , [sp, #-16]!

    /* save the local registers: don't assume the handler will not change them even though it shouldn't */
    stp     x8  , x9  , [sp, #-16]!
    stp     x10 , x11 , [sp, #-16]!
    stp     x12 , x13 , [sp, #-16]!
    stp     x14 , x15 , [sp, #-16]!
    stp     x16 , x17 , [sp, #-16]!
    stp     x18 , x19 , [sp, #-16]!
    stp     x20 , x21 , [sp, #-16]!
    stp     x22 , x23 , [sp, #-16]!
    stp     x24 , x25 , [sp, #-16]!
    stp     x26 , x27 , [sp, #-16]!
    stp     x28 , x29 , [sp, #-16]!
   
    /*************************************************************************/
    /* switch to EL0                                                         */
    /*************************************************************************/

    /* disable interrupts (if an interrupt occurs it will override the SPSR and ELR and we must protect them */
    msr     DAIFSet , #0b0011

    /* set EL1 SPSR return (we will do an ERET instruction next so we must set up the SPSR of EL0 so we return from the exception to EL0) */
    msr     SPSR_EL1, xzr

    /* load the address of checkEl2 into z1 */
    adr     x7 , pharosCpuExecuteUserModeEventCall

    /* set the exception LR register of EL3 to go to pharosCpuExecuteUserModeEventCall */
    msr     ELR_EL1, x7

    /* get the entry point to x7 (when we leave this function) */
    ldr     x7 , [x2 , #OFFSET_ENTRY_POINT]

    /* return from exception which will cause to go to EL0 and to instruction pharosCpuExecuteUserModeEventCall in A64 */
    /* note: this will re-enable interrupts since SPSR == 0 */
    eret

pharosCpuExecuteUserModeEventCall:

    /* jump to the entry point of the thread (x0 and x1 already have the arguments) */
    blr     x7
 
    /*************************************************************************/
    /* Switch to EL1 mode with a syscall made in a well-known address        */
    /*************************************************************************/

    /* make the syscall to restore supervisor mode */
    svc     PHAROS_SYSCALL_SPORADIC_THREAD_ENDED

     /* define the label position just after the swi call */
pharosSporadicThreadEndEvent:

    /* now we are in EL1 again */
 
    /* restore the local registers */
    ldp     x28 , x29 , [sp], #16
    ldp     x26 , x27 , [sp], #16
    ldp     x24 , x25 , [sp], #16
    ldp     x22 , x23 , [sp], #16
    ldp     x20 , x21 , [sp], #16
    ldp     x18 , x19 , [sp], #16
    ldp     x16 , x17 , [sp], #16
    ldp     x14 , x15 , [sp], #16
    ldp     x12 , x13 , [sp], #16
    ldp     x10 , x11 , [sp], #16
    ldp     x8  , x9  , [sp], #16

    /* restore x30 from the stack */
    ldr     x30 , [sp], #16

    /* return from function */
    ret 

.endfunc


/**
 * void pharosCpuExecuteUserModeLwQueue(void *partitionData , LightWeightQueueNumber number, ptrBulk message, uint8_t *buffer , uint32_t size , ptrThreadInfo thread);
 *
 * x0 - partitionData
 * x1 - number
 * x2 - message
 * x3 - buffer
 * x4 - size
 * x5 - thread
 */
.func pharosCpuExecuteUserModeLwQueue
pharosCpuExecuteUserModeLwQueue:
    
    /* when we arrive here we are at EL1 mode */

    /* save x30 to the stack (will be modified by the call) */
    str     x30 , [sp, #-16]!

    /* save the local registers: don't assume the handler will not change them even though it shouldn't */
    stp     x8  , x9  , [sp, #-16]!
    stp     x10 , x11 , [sp, #-16]!
    stp     x12 , x13 , [sp, #-16]!
    stp     x14 , x15 , [sp, #-16]!
    stp     x16 , x17 , [sp, #-16]!
    stp     x18 , x19 , [sp, #-16]!
    stp     x20 , x21 , [sp, #-16]!
    stp     x22 , x23 , [sp, #-16]!
    stp     x24 , x25 , [sp, #-16]!
    stp     x26 , x27 , [sp, #-16]!
    stp     x28 , x29 , [sp, #-16]!
   
    /*************************************************************************/
    /* switch to EL0                                                         */
    /*************************************************************************/

    /* disable interrupts (if an interrupt occurs it will override the SPSR and ELR and we must protect them */
    msr     DAIFSet , #0b0011

    /* set EL1 SPSR return (we will do an ERET instruction next so we must set up the SPSR of EL0 so we return from the exception to EL0) */
    msr     SPSR_EL1, xzr

    /* load the address of checkEl2 into z1 */
    adr     x7 , pharosCpuExecuteUserModeLwQueueCall

    /* set the exception LR register of EL3 to go to pharosCpuExecuteUserModeLwQueueCall */
    msr     ELR_EL1, x7

    /* get the entry point to x7 (when we leave this function) */
    ldr     x7 , [x5 , #OFFSET_ENTRY_POINT]

    /* return from exception which will cause to go to EL0 and to instruction pharosCpuExecuteUserModeLwQueueCall in A64 */
    /* note: this will re-enable interrupts since SPSR == 0 */
    eret

pharosCpuExecuteUserModeLwQueueCall:

    /* jump to the entry point of the thread (x0 - x3 already have the arguments) */
    blr     x7
 
    /*************************************************************************/
    /* Switch to EL1 mode with a syscall made in a well-known address        */
    /*************************************************************************/

    /* make the syscall to restore supervisor mode */
    svc     PHAROS_SYSCALL_SPORADIC_THREAD_ENDED

     /* define the label position just after the swi call */
pharosSporadicThreadEndLwQueue:

    /* now we are in EL1 again */
 
    /* restore the local registers */
    ldp     x28 , x29 , [sp], #16
    ldp     x26 , x27 , [sp], #16
    ldp     x24 , x25 , [sp], #16
    ldp     x22 , x23 , [sp], #16
    ldp     x20 , x21 , [sp], #16
    ldp     x18 , x19 , [sp], #16
    ldp     x16 , x17 , [sp], #16
    ldp     x14 , x15 , [sp], #16
    ldp     x12 , x13 , [sp], #16
    ldp     x10 , x11 , [sp], #16
    ldp     x8  , x9  , [sp], #16

    /* restore x30 from the stack */
    ldr     x30 , [sp], #16

    /* return from function */
    ret 

.endfunc


/**
 * void pharosCpuExecuteUserModeHwQueue(void *partitionData , HeavyWeightQueueNumber queueId, uint8_t *buffer, uint32_t size , PharosPriority priority , FutureNumber future , ptrThreadInfo thread);
 *
 * x0 - partitionData
 * x1 - queueId
 * x2 - buffer
 * x3 - size
 * x4 - priority
 * x5 - future
 * x6 - thread
 */
.func pharosCpuExecuteUserModeHwQueue
pharosCpuExecuteUserModeHwQueue:

    /* when we arrive here we are at EL1 mode */

    /* save x30 to the stack (will be modified by the call) */
    str     x30 , [sp, #-16]!

    /* save the local registers: don't assume the handler will not change them even though it shouldn't */
    stp     x8  , x9  , [sp, #-16]!
    stp     x10 , x11 , [sp, #-16]!
    stp     x12 , x13 , [sp, #-16]!
    stp     x14 , x15 , [sp, #-16]!
    stp     x16 , x17 , [sp, #-16]!
    stp     x18 , x19 , [sp, #-16]!
    stp     x20 , x21 , [sp, #-16]!
    stp     x22 , x23 , [sp, #-16]!
    stp     x24 , x25 , [sp, #-16]!
    stp     x26 , x27 , [sp, #-16]!
    stp     x28 , x29 , [sp, #-16]!
   
    /*************************************************************************/
    /* switch to EL0                                                         */
    /*************************************************************************/

    /* disable interrupts (if an interrupt occurs it will override the SPSR and ELR and we must protect them */
    msr     DAIFSet , #0b0011

    /* set EL1 SPSR return (we will do an ERET instruction next so we must set up the SPSR of EL0 so we return from the exception to EL0) */
    msr     SPSR_EL1, xzr

    /* load the address of checkEl2 into z1 */
    adr     x7 , pharosCpuExecuteUserModeHwQueueCall

    /* set the exception LR register of EL3 to go to pharosCpuExecuteUserModeHwQueueCall */
    msr     ELR_EL1, x7

    /* get the entry point to x7 (when we leave this function) */
    ldr     x7 , [x6 , #OFFSET_ENTRY_POINT]

    /* return from exception which will cause to go to EL0 and to instruction pharosCpuExecuteUserModeHwQueueCall in A64 */
    /* note: this will re-enable interrupts since SPSR == 0 */
    eret

pharosCpuExecuteUserModeHwQueueCall:

    /* jump to the entry point of the thread (x0 - x4 already have the arguments) */
    blr     x7
 
    /*************************************************************************/
    /* Switch to EL1 mode with a syscall made in a well-known address        */
    /*************************************************************************/

    /* make the syscall to restore supervisor mode */
    svc     PHAROS_SYSCALL_SPORADIC_THREAD_ENDED

     /* define the label position just after the swi call */
pharosSporadicThreadEndHwQueue:

    /* now we are in EL1 again */
 
    /* restore the local registers */
    ldp     x28 , x29 , [sp], #16
    ldp     x26 , x27 , [sp], #16
    ldp     x24 , x25 , [sp], #16
    ldp     x22 , x23 , [sp], #16
    ldp     x20 , x21 , [sp], #16
    ldp     x18 , x19 , [sp], #16
    ldp     x16 , x17 , [sp], #16
    ldp     x14 , x15 , [sp], #16
    ldp     x12 , x13 , [sp], #16
    ldp     x10 , x11 , [sp], #16
    ldp     x8  , x9  , [sp], #16

    /* restore x30 from the stack */
    ldr     x30 , [sp], #16

    /* return from function */
    ret 

.endfunc


/**
 * void pharosCpuExecuteUserModeChannel(void *partitionData , ptrChannelBulk bulk , 
 *                                      ChannelId channelId , PharosPriority priority , ptrFutureId number ,
 *                                      ptrThreadInfo thread);
 *
 * x0 - partitionData
 * x1 - bulk
 * x2 - channelId
 * x3 - priority
 * x4 - number
 * x5 - thread
 */
.func pharosCpuExecuteUserModeChannel
pharosCpuExecuteUserModeChannel:

    /* when we arrive here we are at EL1 mode */

    /* save x30 to the stack (will be modified by the call) */
    str     x30 , [sp, #-16]!

    /* save the local registers: don't assume the handler will not change them even though it shouldn't */
    stp     x8  , x9  , [sp, #-16]!
    stp     x10 , x11 , [sp, #-16]!
    stp     x12 , x13 , [sp, #-16]!
    stp     x14 , x15 , [sp, #-16]!
    stp     x16 , x17 , [sp, #-16]!
    stp     x18 , x19 , [sp, #-16]!
    stp     x20 , x21 , [sp, #-16]!
    stp     x22 , x23 , [sp, #-16]!
    stp     x24 , x25 , [sp, #-16]!
    stp     x26 , x27 , [sp, #-16]!
    stp     x28 , x29 , [sp, #-16]!

    /*************************************************************************/
    /* switch to EL0                                                         */
    /*************************************************************************/

    /* disable interrupts (if an interrupt occurs it will override the SPSR and ELR and we must protect them */
    msr     DAIFSet , #0b0011

    /* set EL1 SPSR return (we will do an ERET instruction next so we must set up the SPSR of EL0 so we return from the exception to EL0) */
    msr     SPSR_EL1, xzr

    /* load the address of checkEl2 into z1 */
    adr     x8 , pharosCpuExecuteUserModeChannelCall

    /* set the exception LR register of EL3 to go to pharosCpuExecuteUserModeChannelCall */
    msr     ELR_EL1, x8

    /* get the entry point to x10 (when we leave this function) */
    ldr     x10 , [x5 , #OFFSET_ENTRY_POINT]

    /* return from exception which will cause to go to EL0 and to instruction pharosCpuExecuteUserModeChannelCall in A64 */
    /* note: this will re-enable interrupts since SPSR == 0 */
    eret

pharosCpuExecuteUserModeChannelCall:

    /* jump to the entry point of the thread (x0 - x7 already have the arguments) */
    blr     x10

    /*************************************************************************/
    /* Switch to EL1 mode with a syscall made in a well-known address        */
    /*************************************************************************/

    /* make the syscall to restore supervisor mode */
    svc     PHAROS_SYSCALL_SPORADIC_THREAD_ENDED

     /* define the label position just after the swi call */
pharosSporadicThreadEndChannel:

    /* now we are in EL1 again */
 
    /* restore the local registers */
    ldp     x28 , x29 , [sp], #16
    ldp     x26 , x27 , [sp], #16
    ldp     x24 , x25 , [sp], #16
    ldp     x22 , x23 , [sp], #16
    ldp     x20 , x21 , [sp], #16
    ldp     x18 , x19 , [sp], #16
    ldp     x16 , x17 , [sp], #16
    ldp     x14 , x15 , [sp], #16
    ldp     x12 , x13 , [sp], #16
    ldp     x10 , x11 , [sp], #16
    ldp     x8  , x9  , [sp], #16

    /* restore x30 from the stack */
    ldr     x30 , [sp], #16

    /* return from function */
    ret 

.endfunc
