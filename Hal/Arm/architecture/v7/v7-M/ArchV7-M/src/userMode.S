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
#include <pharos/hal/def.h>


.type pharosCpuExecuteUserModeAper,         %function
.type pharosCpuExecuteUserModePer,          %function
.type pharosCpuExecuteUserModeEvent,        %function
.type pharosCpuExecuteUserModeMutex,        %function
.type pharosCpuExecuteUserModeLwQueue,      %function
.type pharosCpuExecuteUserModeHwQueue,      %function
.type pharosCpuExecuteUserModeChannel,      %function


.global pharosCpuExecuteUserModeAper
.global pharosCpuExecuteUserModePer
.global pharosCpuExecuteUserModeEvent
.global pharosCpuExecuteUserModeMutex
.global pharosCpuExecuteUserModeLwQueue
.global pharosCpuExecuteUserModeHwQueue
.global pharosCpuExecuteUserModeChannel

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


.syntax unified
.text
.thumb


/**
 * void pharosCpuExecuteUserModeAper(void *partitionData , ptrThreadInfo thread);
 */
.func pharosCpuExecuteUserModeAper
pharosCpuExecuteUserModeAper:

    /* save all the registers (might be changed inside the user mode function even though they should not if written in C) */
    push    {%r4 - %r12 , %lr}

    /* get the entry point to r3 (when we leave this function) */
    ldr     %r3 , [%r1 , #OFFSET_ENTRY_POINT]

    /* get the user stack */
    ldr     %r4 , [%r1 , #OFFSET_USER_STACK]

    /* place 0 in r8 */
    mov     %r8 , #FALSE

    /* store the FALSE in the "insideKernel" flag */
    str     %r8 , [%r1 , #OFFSET_INSIDE_KERNEL]

    /* switch stack to user stack (instead of being in kernel stack) */
    msr     PSP , %r4

    /* read the current control register */
    mrs     %r4 , control

    /* set the thread mode to unpriviledged */
    orr     %r4 , %r4 , #1

    /* switch to unpriviledged mode */
    msr     control , %r4

    /* jump to the entry point of the thread (r0 already has the right content) */
    blx     %r3

    /* make the syscall to restore supervisor mode */
    swi     PHAROS_SYSCALL_APERIODIC_THREAD_ENDED

     /* define the label position just after the swi call */
pharosAperiodicThreadAddress:

    /* at this point we are running on priviledged mode */
 
    /* restore all the registers */
    pop     {%r4 - %r12 , %lr}

    /* leave and restore ARM/Thumb mode */
    bx      lr

.endfunc


/**
 * void pharosCpuExecuteUserModePer(void *partitionData , ptrThreadInfo thread);
 */
.func pharosCpuExecuteUserModePer
pharosCpuExecuteUserModePer:
    
    /* save all the registers (might be changed inside the user mode function even though they should not if written in C) */
    push    {%r4 - %r12 , %lr}

    /* get the entry point to r3 (when we leave this function) */
    ldr     %r3 , [%r1 , #OFFSET_ENTRY_POINT]
    
    /* get the user stack */
    ldr     %r4 , [%r1 , #OFFSET_USER_STACK]

    /* get the previous syscall stack */
    ldr     %r7 , [%r1 , #OFFSET_PREVIOUS_SVC_STACK]

    /* get the previous current kernel stack */
    ldr     %r8 , [%r1 , #OFFSET_CURRENT_KERNEL_STACK]

    /* push the previous syscall stack and the current stack into the current thread kernel stack */
    push    {%r7 , %r8}

    /* store the PSP in the current kernel stack (to be restored later on) */
    str     %sp , [%r1 , #OFFSET_CURRENT_KERNEL_STACK]

    /* place 0 in r8 */
    mov     %r8 , #FALSE

    /* store the FALSE in the "insideKernel" flag */
    str     %r8 , [%r1 , #OFFSET_INSIDE_KERNEL]

    /* switch stack to user stack (instead of being in kernel stack) */
    mov     %sp , %r4

    /* read the current control register */
    mrs     %r4 , control

    /* set the thread mode to unpriviledged */
    orr     %r4 , %r4 , #1

    /* switch to unpriviledged mode */
    msr     control , %r4
    
    /* jump to the entry point of the thread (no arguments needed) */
    blx     %r3

    /* make the syscall to restore supervisor mode */
    swi     PHAROS_SYSCALL_PERIODIC_THREAD_ENDED

     /* define the label position just after the swi call */
pharosPeriodicThreadAddress:

    /* get the running thread variable address */
    ldr     %r4 , =pharosVRunningThread

    /* get the running thread address */
    ldr     %r4 , [%r4]

    /* restore the PSP stack */
    ldr     %sp , [%r4 , #OFFSET_CURRENT_KERNEL_STACK]

    /* get to r7 the previous syscall stack */
    pop     {%r7 , %r8}

    /* set the previous syscall stack to the current thread */
    str     %r7 , [%r4 , #OFFSET_PREVIOUS_SVC_STACK]

    /* restore the current kernel thread stack of the current thread */
    str     %r8 , [%r4 , #OFFSET_CURRENT_KERNEL_STACK]

    /* restore the used registers */
    pop     {%r4 - %r12 , %lr}

    /* return from function */
	bx      %lr

.endfunc


/**
 * void pharosCpuExecuteUserModeMutex(void *partitionData , SemaphoreMutexNumber ssId , ptrThreadInfo thread);
 */
.func pharosCpuExecuteUserModeMutex
pharosCpuExecuteUserModeMutex:
    
    /* save all the registers (might be changed inside the user mode function even though they should not if written in C) */
    push    {%r4 - %r12 , %lr}

    /* get the entry point to r3 (when we leave this function) */
    ldr     %r3 , [%r2, #OFFSET_ENTRY_POINT]

    /* get the user stack */
    ldr     %r4 , [%r2 , #OFFSET_USER_STACK]

    /* store the PSP in the current kernel stack (to be restored later on) */
    str     %sp , [%r2 , #OFFSET_CURRENT_KERNEL_STACK]

    /* place 0 in r8 */
    mov     %r8 , #FALSE

    /* store the FALSE in the "insideKernel" flag */
    str     %r8 , [%r2 , #OFFSET_INSIDE_KERNEL]

    /* switch stack to user stack (instead of being in kernel stack) */
    mov     %sp , %r4

    /* read the current control register */
    mrs     %r4 , control

    /* set the thread mode to unpriviledged */
    orr     %r4 , %r4 , #1

    /* switch to unpriviledged mode */
    msr     control , %r4
    
    /* jump to the entry point of the thread (no arguments needed) */
    blx     %r3

    /* make the syscall to restore supervisor mode */
    swi     PHAROS_SYSCALL_SPORADIC_THREAD_ENDED

     /* define the label position just after the swi call */
pharosSporadicThreadEndMutex:

    /* at this point we are running on priviledged mode */

    /* get the running thread variable address */
    ldr     %r0 , =pharosVRunningThread

    /* get the running thread address */
    ldr     %r0 , [%r0]

    /* restore the PSP stack */
    ldr     %sp , [%r0 , #OFFSET_CURRENT_KERNEL_STACK]

    /* restore all the registers */
    pop     {%r4 - %r12 , %lr}

    /* leave and restore ARM/Thumb mode */
    bx      lr

.endfunc


/**
 * void pharosCpuExecuteUserModeEvent(void *partitionData , Event event , ptrThreadInfo thread);
 * r0 - partitionData
 * r1 - nothing
 * r2 - event
 * r3 - event
 * [sp +  0] - thread
 */
.func pharosCpuExecuteUserModeEvent
pharosCpuExecuteUserModeEvent:
    
    /* save all the registers (might be changed inside the user mode function even though they should not if written in C) */
    push    {%r4 - %r12 , %lr}

    /* get to r1 the thread */
    ldr     %r1, [%sp , #40]

    /* get the entry point to r5 */
    ldr     %r5 , [%r1, #OFFSET_ENTRY_POINT]

    /* get the user stack */
    ldr     %r4 , [%r1 , #OFFSET_USER_STACK]

    /* store the PSP in the current kernel stack (to be restored later on) */
    str     %sp , [%r1 , #OFFSET_CURRENT_KERNEL_STACK]

    /* place 0 in r8 */
    mov     %r8 , #FALSE

    /* store the FALSE in the "insideKernel" flag */
    str     %r8 , [%r1 , #OFFSET_INSIDE_KERNEL]

    /* switch stack to user stack (instead of being in kernel stack) */
    mov     %sp , %r4

    /* read the current control register */
    mrs     %r4 , control

    /* set the thread mode to unpriviledged */
    orr     %r4 , %r4 , #1

    /* switch to unpriviledged mode */
    msr     control , %r4

    /* jump to the entry point of the thread (%r0 and %r2 and %r3 were left unchanged) */
    blx     %r5

    /* make the syscall to restore supervisor mode */
    swi     PHAROS_SYSCALL_SPORADIC_THREAD_ENDED

     /* define the label position just after the swi call */
pharosSporadicThreadEndEvent:

    /* at this point we are running on priviledged mode */

    /* get the running thread variable address */
    ldr     %r0 , =pharosVRunningThread

    /* get the running thread address */
    ldr     %r0 , [%r0]

    /* restore the PSP stack */
    ldr     %sp , [%r0 , #OFFSET_CURRENT_KERNEL_STACK]

    /* restore all the registers */
    pop     {%r4 - %r12 , %lr}

    /* leave and restore ARM/Thumb mode */
    bx      lr

.endfunc


/**
 * void pharosCpuExecuteUserModeLwQueue(void *partitionData , LightWeightQueueNumber number, ptrBulk message, uint8_t *buffer , uint32_t size , ptrThreadInfo thread);
 *
 * r0 - partitionData
 * r1 - number
 * r2 - message
 * r3 - buffer
 * [sp +  0] - size
 * [sp +  4] - thread
 */
.func pharosCpuExecuteUserModeLwQueue
pharosCpuExecuteUserModeLwQueue:
    
    /* save all the registers (might be changed inside the user mode function even though they should not if written in C) */
    push    {%r4 - %r12 , %lr}

    /* get to r9 the size (will be pushed to the user stack later on) */
    ldr     %r9, [%sp , #40]

    /* get to r5 the thread */
    ldr     %r5, [%sp , #44]

    /* get the entry point to r6 (when we leave this function) */
    ldr     %r6 , [%r5, #OFFSET_ENTRY_POINT]

    /* get the user stack */
    ldr     %r4 , [%r5 , #OFFSET_USER_STACK]

    /* store the PSP in the current kernel stack (to be restored later on) */
    str     %sp , [%r5 , #OFFSET_CURRENT_KERNEL_STACK]

    /* place 0 in r8 */
    mov     %r8 , #FALSE

    /* store the FALSE in the "insideKernel" flag */
    str     %r8 , [%r5 , #OFFSET_INSIDE_KERNEL]

    /* switch stack to user stack (instead of being in kernel stack) */
    mov     %sp , %r4

    /* read the current control register */
    mrs     %r4 , control

    /* set the thread mode to unpriviledged */
    orr     %r4 , %r4 , #1

    /* switch to unpriviledged mode */
    msr     control , %r4
    
    /* place in the user stack the arguments (save an extra register to keep the stack correctly aligned) */
    push    {%r9 , %r10}
    
    /* jump to the entry point of the thread (the arguments are already in r0-r3) */
    blx     %r6

    /* restore the previous stack position */
    pop     {%r9 , %r10}

    /* make the syscall to restore supervisor mode */
    swi     PHAROS_SYSCALL_SPORADIC_THREAD_ENDED

     /* define the label position just after the swi call */
pharosSporadicThreadEndLwQueue:

    /* at this point we are running on priviledged mode */

    /* get the running thread variable address */
    ldr     %r0 , =pharosVRunningThread

    /* get the running thread address */
    ldr     %r0 , [%r0]

    /* restore the PSP stack */
    ldr     %sp , [%r0 , #OFFSET_CURRENT_KERNEL_STACK]

    /* restore all the registers */
    pop     {%r4 - %r12 , %lr}

    /* leave and restore ARM/Thumb mode */
    bx      lr

.endfunc


/**
 * void pharosCpuExecuteUserModeHwQueuee(void *partitionData , HwQueueNumber queueId , uint8_t *buffer ,
 *                                       uint32_t size , PharosPriority priority , ptrFutureId future ,
 *                                       ptrThreadInfo thread);
 *
 * r0 - partitionData
 * r1 - queueId
 * r2 - buffer
 * r3 - size
 * [sp +  0] - priority
 * [sp +  4] - priority
 * [sp +  8] - future
 * [sp +  12] - thread
 */
.func pharosCpuExecuteUserModeHwQueue
pharosCpuExecuteUserModeHwQueue:
    
    /* save all the registers (might be changed inside the user mode function even though they should not if written in C) */
    push    {%r4 - %r12 , %lr}

    /* get to %r9 the priority (will be copied to the user stack later on) */
    ldr     %r9 , [%sp , #40]

    /* get to %r10 the priority (will be copied to the user stack later on) */
    ldr     %r10 , [%sp , #44]

    /* get to %r11 the future (will be copied to the user stack later on) */
    ldr     %r11 , [%sp , #48]

    /* get to r5 the thread */
    ldr     %r5, [%sp , #52]

    /* get the entry point to r6 */
    ldr     %r6 , [%r5, #OFFSET_ENTRY_POINT]

    /* get the user stack */
    ldr     %r4 , [%r5 , #OFFSET_USER_STACK]

    /* store the PSP in the current kernel stack (to be restored later on) */
    str     %sp , [%r5 , #OFFSET_CURRENT_KERNEL_STACK]

    /* place 0 in r8 */
    mov     %r8 , #FALSE

    /* store the FALSE in the "insideKernel" flag */
    str     %r8 , [%r5 , #OFFSET_INSIDE_KERNEL]

    /* switch stack to user stack (instead of being in kernel stack) */
    mov     %sp , %r4

    /* read the current control register */
    mrs     %r4 , control

    /* set the thread mode to unpriviledged */
    orr     %r4 , %r4 , #1

    /* switch to unpriviledged mode */
    msr     control , %r4
    
    /* place in the user stack the priority and future number */
    push    {%r9 , %r10 , %r11 , %r12}
    
    /* jump to the entry point of the thread (the arguments are already in r0-r3) */
    blx     %r6

    /* restore the previous stack position */
    pop     {%r9 , %r10 , %r11 , %r12}

    /* make the syscall to restore supervisor mode */
    swi     PHAROS_SYSCALL_SPORADIC_THREAD_ENDED

     /* define the label position just after the swi call */
pharosSporadicThreadEndHwQueue:

    /* at this point we are running on priviledged mode */

    /* get the running thread variable address */
    ldr     %r0 , =pharosVRunningThread

    /* get the running thread address */
    ldr     %r0 , [%r0]

    /* restore the PSP stack */
    ldr     %sp , [%r0 , #OFFSET_CURRENT_KERNEL_STACK]

    /* restore all the registers */
    pop     {%r4 - %r12 , %lr}

    /* leave and restore ARM/Thumb mode */
    bx      lr

.endfunc


/**
 * void pharosCpuExecuteUserModeChannel(void *partitionData , ptrChannelBulk bulk , 
 *                                      ChannelId channelId , PharosPriority priority , ptrFutureId number ,
 *                                      ptrThreadInfo thread);
 *
 * r0 - partitionData
 * r1 - bulk
 * r2 - channelId
 * r3 - channelId
 * [sp +  0] - priority
 * [sp +  4] - priority
 * [sp +  8] - number
 * [sp + 12] - thread
 */
.func pharosCpuExecuteUserModeChannel
pharosCpuExecuteUserModeChannel:
    
    /* save all the registers (might be changed inside the user mode function even though they should not if written in C) */
    push    {%r4 - %r12 , %lr}

    /* get to %r7 the priority (will be copied to the user stack later on) */
    ldr     %r7 , [%sp, #(40 + 0)]

    /* get to r4 the thread */
    ldr     %r4, [%sp , #(40 + 12)]

    /* get to %r8 the priority (will be copied to the user stack later on) */
    ldr     %r8 , [%sp, #(40 + 4)]

    /* get to %r9 the future number (will be copied to the user stack later on) */
    ldr     %r9 , [%sp, #(40 + 8)]

    /* get the user stack */
    ldr     %r5 , [%r4 , #OFFSET_USER_STACK]

    /* store the PSP in the current kernel stack (to be restored later on) */
    str     %sp , [%r4 , #OFFSET_CURRENT_KERNEL_STACK]

    /* place 0 in r6 */
    mov     %r6 , #FALSE

    /* store the FALSE in the "insideKernel" flag */
    str     %r6 , [%r4 , #OFFSET_INSIDE_KERNEL]

    /* switch stack to user stack (instead of being in kernel stack) */
    mov     %sp , %r5
   
    /* read the current control register */
    mrs     %r6 , control

    /* set the thread mode to unpriviledged */
    orr     %r6 , %r6 , #1

    /* get the entry point to r5 (when we leave this function) */
    ldr     %r5 , [%r4, #OFFSET_ENTRY_POINT]

    /* switch to unpriviledged mode */
    msr     control , %r6

    /* place in the user stack the arguments (save an extra register to keep the stack correctly aligned) */
    push    {%r7-%r10}

    /* jump to the entry point of the thread (the arguments are already in r0-r3 and stack) */
    blx     %r5
    
    /* restore the previous stack position */
    pop     {%r7-%r10}

    /* make the syscall to restore supervisor mode */
    swi     PHAROS_SYSCALL_SPORADIC_THREAD_ENDED

     /* define the label position just after the swi call */
pharosSporadicThreadEndChannel:

    /* at this point we are running on priviledged mode */

    /* get the running thread variable address */
    ldr     %r0 , =pharosVRunningThread

    /* get the running thread address */
    ldr     %r0 , [%r0]

    /* restore the PSP stack */
    ldr     %sp , [%r0 , #OFFSET_CURRENT_KERNEL_STACK]

    /* restore all the registers */
    pop     {%r4 - %r12 , %lr}

    /* leave and restore ARM/Thumb mode */
    bx      lr

.endfunc
