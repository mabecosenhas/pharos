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
.type pharosCpuExecuteSupervisorModeAper,   %function
.type pharosCpuExecuteSupervisorModePer,    %function
.type pharosCpuExecuteSupevisorModeEvent,   %function
.type pharosCpuExecuteSupevisorModeMutex,   %function
.type pharosCpuExecuteSupevisorModeLwQueue, %function
.type pharosCpuExecuteSupevisorModeHwQueue, %function
.type pharosCpuExecuteSupevisorModeChannel, %function

.global pharosCpuExecuteUserModeAper
.global pharosCpuExecuteUserModePer
.global pharosCpuExecuteUserModeEvent
.global pharosCpuExecuteUserModeMutex
.global pharosCpuExecuteUserModeLwQueue
.global pharosCpuExecuteUserModeHwQueue
.global pharosCpuExecuteUserModeChannel
.global pharosCpuExecuteSupervisorModeAper
.global pharosCpuExecuteSupervisorModePer
.global pharosCpuExecuteSupevisorModeEvent
.global pharosCpuExecuteSupevisorModeMutex
.global pharosCpuExecuteSupevisorModeLwQueue
.global pharosCpuExecuteSupevisorModeHwQueue
.global pharosCpuExecuteSupevisorModeChannel


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
.global pharosAperiodicThreadSuperAddress

.syntax unified
.text
.arm


/**
 * void pharosCpuExecuteUserModeAper(void *partitionData , ptrThreadInfo thread);
 */
.func pharosCpuExecuteUserModeAper
pharosCpuExecuteUserModeAper:
    
    /* save all the registers (might be changed inside the user mode function even though they should not if written in C) */
    push    {%r4 - %r12 , %lr}

    /* get the entry point to r3 (when we leave this function) */
    ldr     %r3 , [%r1 , #OFFSET_ENTRY_POINT]

    /* get the TLB in user mode */
    ldr     %r1 , [%r1 , #OFFSET_TLB_USER_MODE]

    /* get to %r2 the current TLB */
    mrc     p15, 0 , %r2 , c2 , c0 , 0

    /* save the current TLB to the supervisor stack */
    push    {%r2}

    /* pogram the TLB to the thread memory space */
    mcr     p15, 0 , %r1, c2 , c0 , 0
  
    /* flush the current TLB */
    mcr     p15, 0, %r1, c8, c7, 0

    /* switch to user mode registers */
    msr     cpsr_c , #(CPSR_USER_MODE)
    
    /* jump to the entry point of the thread (no arguments needed) */
    blx     %r3

    /* push the registers needed for the syscall */
    push    {%r0 , %r4-%r12 , %lr}

    /* make the syscall to restore supervisor mode */
    swi     PHAROS_SYSCALL_APERIODIC_THREAD_ENDED

     /* define the label position just after the swi call */
pharosAperiodicThreadAddress:

    /* at this point we are running on supervisor mode */

    /* load to r2 the address of the kernel TLB */
    ldr     %r2, =kernelSectionTable

    /* program the c2 register to place the kernel TBL address so that when we pop from the supervisor stack we have permissions */
    mcr     p15, 0 , %r2, c2 , c0 , 0
  
    /* flush the current TLB */
    mcr     p15, 0, %r2, c8, c7, 0

    /*  switch to system mode registers */
    msr     cpsr_c , #(CPSR_SYSTEM_MODE)
  
    /* restore the stack to the correct place */
    pop     {%r0 , %r4-%r12, %lr}

    /* switch back to supervisor mode registers */
    msr     cpsr_c , #(CPSR_SUPERVISOR_MODE)
  
    /* get the previous TLB to r2 (pop from the supervisor stack) */
    pop     {%r2}

    /* pogram the TLB to the thread memory space */
    mcr     p15, 0 , %r2, c2 , c0 , 0
  
    /* flush the current TLB */
    mcr     p15, 0, %r2, c8, c7, 0

    /* restore all the registers */
    pop     {%r4 - %r12 , %lr}

    /* leave and restore ARM/Thumb mode */
    bx      lr

.endfunc


/**
 * void pharosCpuExecuteUserModePer(void *partitionData , ptrThreadInfo thread);
 */
.arm
.func pharosCpuExecuteUserModePer
pharosCpuExecuteUserModePer:
    
    /* save all the registers (might be changed inside the user mode function even though they should not if written in C) */
    push    {%r4 - %r12 , %lr}

    /* get the entry point to r3 (when we leave this function) */
    ldr     %r3 , [%r1 , #OFFSET_ENTRY_POINT]

    /* get the TLB in user mode */
    ldr     %r1 , [%r1 , #OFFSET_TLB_USER_MODE]

    /* get to %r2 the current TLB */
    mrc     p15, 0 , %r2 , c2 , c0 , 0

    /* save the current TLB to the supervisor stack */
    push    {%r2}

    /* pogram the TLB to the thread memory space */
    mcr     p15, 0 , %r1, c2 , c0 , 0
  
    /* flush the current TLB */
    mcr     p15, 0, %r1, c8, c7, 0

    /* switch to user mode registers */
    msr     cpsr_c , #(CPSR_USER_MODE)
    
    /* jump to the entry point of the thread (no arguments needed) */
    blx     %r3

    /* push the registers needed for the syscall */
    push    {%r0 , %r4-%r12, %lr}

    /* make the syscall to restore supervisor mode */
    swi     PHAROS_SYSCALL_PERIODIC_THREAD_ENDED

     /* define the label position just after the swi call */
pharosPeriodicThreadAddress:

    /* at this point we are running on supervisor mode */

    /* load to r2 the address of the kernel TLB */
    ldr     %r2, =kernelSectionTable

    /* program the c2 register to place the kernel TBL address so that when we pop from the supervisor stack we have permissions */
    mcr     p15, 0 , %r2, c2 , c0 , 0
  
    /* flush the current TLB */
    mcr     p15, 0, %r2, c8, c7, 0

    /*  switch to system mode registers */
    msr     cpsr_c , #(CPSR_SYSTEM_MODE)
  
    /* restore the stack to the correct place */
    pop     {%r0 , %r4-%r12, %lr}

    /* switch back to supervisor mode registers */
    msr     cpsr_c , #(CPSR_SUPERVISOR_MODE)
  
    /* get the previous TLB to r2 (pop from the supervisor stack) */
    pop     {%r2}

    /* pogram the TLB to the thread memory space */
    mcr     p15, 0 , %r2, c2 , c0 , 0
  
    /* flush the current TLB */
    mcr     p15, 0, %r2, c8, c7, 0

    /* restore all the registers */
    pop     {%r4 - %r12 , %lr}

    /* leave and restore ARM/Thumb mode */
    bx      lr

.endfunc


/**
 * void pharosCpuExecuteUserModeMutex(void *partitionData , SemaphoreMutexNumber ssId , ptrThreadInfo thread);
 */
.arm
.func pharosCpuExecuteUserModeMutex
pharosCpuExecuteUserModeMutex:
    
    /* save all the registers (might be changed inside the user mode function even though they should not if written in C) */
    push    {%r4 - %r12 , %lr}

    /* get the entry point to r3 (when we leave this function) */
    ldr     %r3 , [%r2, #OFFSET_ENTRY_POINT]

    /* get the TLB in user mode */
    ldr     %r4 , [%r2 , #OFFSET_TLB_USER_MODE]

    /* get to %r2 the current TLB */
    mrc     p15, 0 , %r2 , c2 , c0 , 0

    /* save the current TLB to the supervisor stack */
    push    {%r2}

    /* pogram the TLB to the thread memory space */
    mcr     p15, 0 , %r4, c2 , c0 , 0
  
    /* flush the current TLB */
    mcr     p15, 0, %r4, c8, c7, 0

    /* switch to user mode registers */
    msr     cpsr_c , #(CPSR_USER_MODE)
    
    /* jump to the entry point of the thread (no arguments needed) */
    blx     %r3

    /* push the registers needed for the syscall */
    push    {%r0 , %r4-%r12, %lr}

    /* make the syscall to restore supervisor mode */
    swi     PHAROS_SYSCALL_SPORADIC_THREAD_ENDED

     /* define the label position just after the swi call */
pharosSporadicThreadEndMutex:

    /* at this point we are running on supervisor mode */

    /* load to r2 the address of the kernel TLB */
    ldr    %r2, =kernelSectionTable

    /* program the c2 register to place the kernel TBL address so that when we pop from the supervisor stack we have permissions */
    mcr     p15, 0 , %r2, c2 , c0 , 0
  
    /* flush the current TLB */
    mcr     p15, 0, %r2, c8, c7, 0

    /*  switch to system mode registers */
    msr     cpsr_c , #(CPSR_SYSTEM_MODE)
  
    /* restore the stack to the correct place */
    pop     {%r0 , %r4-%r12, %lr}

    /* switch back to supervisor mode registers */
    msr     cpsr_c , #(CPSR_SUPERVISOR_MODE)
  
    /* get the previous TLB to r2 (pop from the supervisor stack) */
    pop     {%r2}

    /* pogram the TLB to the thread memory space */
    mcr     p15, 0 , %r2, c2 , c0 , 0
  
    /* flush the current TLB */
    mcr     p15, 0, %r2, c8, c7, 0

    /* restore all the registers */
    pop     {%r4 - %r12 , %lr}

    /* leave and restore ARM/Thumb mode */
    bx      lr

.endfunc




/**
 * void pharosCpuExecuteUserModeEvent(void *partitionData , Event event , ptrThreadInfo thread);
 * r0        - partitionData
 * r2 and r3 - event
 * [sp + 0]  - thread
 */
.arm
.func pharosCpuExecuteUserModeEvent
pharosCpuExecuteUserModeEvent:
    
    /* save all the registers (might be changed inside the user mode function even though they should not if written in C) */
    push    {%r4 - %r12 , %lr}

    /* get to r6 the thread */
    ldr     %r6, [%sp , #40]

    /* get the entry point to r3 (when we leave this function) */
    ldr     %r5 , [%r6, #OFFSET_ENTRY_POINT]

    /* get the TLB in user mode */
    ldr     %r4 , [%r6 , #OFFSET_TLB_USER_MODE]

    /* get to %r6 the current TLB */
    mrc     p15, 0 , %r6 , c2 , c0 , 0

    /* save the current TLB to the supervisor stack */
    push    {%r6}

    /* pogram the TLB to the thread memory space */
    mcr     p15, 0 , %r4, c2 , c0 , 0
  
    /* flush the current TLB */
    mcr     p15, 0, %r4, c8, c7, 0

    /* switch to user mode registers */
    msr     cpsr_c , #(CPSR_USER_MODE)
    
    /* jump to the entry point of the thread (no arguments needed) */
    blx     %r5

    /* push the registers needed for the syscall */
    push    {%r0 , %r4-%r12, %lr}

    /* make the syscall to restore supervisor mode */
    swi     PHAROS_SYSCALL_SPORADIC_THREAD_ENDED

     /* define the label position just after the swi call */
pharosSporadicThreadEndEvent:

    /* at this point we are running on supervisor mode */

    /* load to r2 the address of the kernel TLB */
    ldr    %r2, =kernelSectionTable

    /* program the c2 register to place the kernel TBL address so that when we pop from the supervisor stack we have permissions */
    mcr     p15, 0 , %r2, c2 , c0 , 0
  
    /* flush the current TLB */
    mcr     p15, 0, %r2, c8, c7, 0

    /*  switch to system mode registers */
    msr     cpsr_c , #(CPSR_SYSTEM_MODE)
  
    /* restore the stack to the correct place */
    pop     {%r0 , %r4-%r12, %lr}

    /* switch back to supervisor mode registers */
    msr     cpsr_c , #(CPSR_SUPERVISOR_MODE)
  
    /* get the previous TLB to r2 (pop from the supervisor stack) */
    pop     {%r2}

    /* pogram the TLB to the thread memory space */
    mcr     p15, 0 , %r2, c2 , c0 , 0
  
    /* flush the current TLB */
    mcr     p15, 0, %r2, c8, c7, 0

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
.arm
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

    /* get the TLB in user mode */
    ldr     %r7 , [%r5 , #OFFSET_TLB_USER_MODE]

    /* get to %r8 the current TLB */
    mrc     p15, 0 , %r8 , c2 , c0 , 0

    /* save the current TLB to the supervisor stack */
    push    {%r8}

    /* program the TLB to the thread memory space */
    mcr     p15, 0 , %r7, c2 , c0 , 0
  
    /* flush the current TLB */
    mcr     p15, 0, %r7, c8, c7, 0

    /* switch to user mode registers */
    msr     cpsr_c , #(CPSR_USER_MODE)
    
    /* place in the user stack the arguments (save an extra register to keep the stack correctly aligned) */
    push    {%r9 , %r10}
    
    /* jump to the entry point of the thread (the arguments are already in r0-r3) */
    blx     %r6

    /* restore the previous stack position */
    pop     {%r9 , %r10}

    /* push the registers needed for the syscall */
    push    {%r0 , %r4-%r12, %lr}

    /* make the syscall to restore supervisor mode */
    swi     PHAROS_SYSCALL_SPORADIC_THREAD_ENDED

     /* define the label position just after the swi call */
pharosSporadicThreadEndLwQueue:

    /* at this point we are running on supervisor mode */

    /* load to r2 the address of the kernel TLB */
    ldr     %r2, =kernelSectionTable

    /* program the c2 register to place the kernel TBL address so that when we pop from the supervisor stack we have permissions */
    mcr     p15, 0 , %r2, c2 , c0 , 0
  
    /* flush the current TLB */
    mcr     p15, 0, %r2, c8, c7, 0

    /*  switch to system mode registers */
    msr     cpsr_c , #(CPSR_SYSTEM_MODE)
  
    /* restore the stack to the correct place */
    pop     {%r0 , %r4-%r12, %lr}

    /* switch back to supervisor mode registers */
    msr     cpsr_c , #(CPSR_SUPERVISOR_MODE)
  
    /* get the previous TLB to %r2 (pop from the supervisor stack) */
    pop     {%r2}

    /* pogram the TLB to the thread memory space */
    mcr     p15, 0 , %r2, c2 , c0 , 0
  
    /* flush the current TLB */
    mcr     p15, 0,%r2, c8, c7, 0

    /* restore all the registers */
    pop     {%r4 - %r12 , %lr}

    /* leave and restore ARM/Thumb mode */
    bx      lr

.endfunc


/**
 * void pharosCpuExecuteUserModeHwQueue(void *partitionData , HeavyWeightQueueNumber queueId, uint8_t *buffer, uint32_t size , PharosPriority priority , FutureNumber future , ptrThreadInfo thread);
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
.arm
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

    /* get the TLB in user mode */
    ldr     %r7 , [%r5 , #OFFSET_TLB_USER_MODE]

    /* get to %r2 the current TLB */
    mrc     p15, 0 , %r8 , c2 , c0 , 0

    /* save the current TLB to the supervisor stack */
    push    {%r8}

    /* program the TLB to the thread memory space */
    mcr     p15, 0 , %r7, c2 , c0 , 0
  
    /* flush the current TLB */
    mcr     p15, 0, %r7, c8, c7, 0

    /* switch to user mode registers */
    msr     cpsr_c , #(CPSR_USER_MODE)
    
    /* place in the user stack the priority and future number */
    push    {%r9 , %r10 , %r11 , %r12}
    
    /* jump to the entry point of the thread (the arguments are already in r0-r3) */
    blx     %r6

    /* restore the previous stack position */
    pop     {%r9 , %r10 , %r11 , %r12}

    /* push the registers needed for the syscall */
    push    {%r0 , %r4-%r12, %lr}

    /* make the syscall to restore supervisor mode */
    swi     PHAROS_SYSCALL_SPORADIC_THREAD_ENDED

     /* define the label position just after the swi call */
pharosSporadicThreadEndHwQueue:

    /* at this point we are running on supervisor mode */

    /* load to r2 the address of the kernel TLB */
    ldr     %r2, =kernelSectionTable

    /* program the c2 register to place the kernel TBL address so that when we pop from the supervisor stack we have permissions */
    mcr     p15, 0 , %r2, c2 , c0 , 0
  
    /* flush the current TLB */
    mcr     p15, 0, %r2, c8, c7, 0

    /*  switch to system mode registers */
    msr     cpsr_c , #(CPSR_SYSTEM_MODE)
  
    /* restore the stack to the correct place */
    pop     {%r0 , %r4-%r12, %lr}

    /* switch back to supervisor mode registers */
    msr     cpsr_c , #(CPSR_SUPERVISOR_MODE)
  
    /* get the previous TLB to r2 (pop from the supervisor stack) */
    pop     {%r2}

    /* pogram the TLB to the thread memory space */
    mcr     p15, 0 , %r2, c2 , c0 , 0
  
    /* flush the current TLB */
    mcr     p15, 0, %r2, c8, c7, 0

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
.arm
.func pharosCpuExecuteUserModeChannel
pharosCpuExecuteUserModeChannel:
    
    /* save all the registers 
     *(might be changed inside the user mode function even though they should not if written in C) */
    push    {%r4 - %r12 , %lr}

    /* get to %r6 the priority (will be copied to the user stack later on) */
    ldr     %r6 , [%sp, #(40 + 0)]

    /* get to %r7 the prioirty (will be copied to the user stack later on) */
    ldr     %r7 , [%sp, #(40 + 4)]

    /* get to %r8 the future number (will be copied to the user stack later on) */
    ldr     %r8 , [%sp, #(40 + 8)]

    /* get to r4 the thread */
    ldr     %r4, [%sp , #(40 + 12)]
   
    /* get to %r5 the current TLB */
    mrc     p15, 0 , %r5 , c2 , c0 , 0

    /* save the current TLB to the supervisor stack */
    push    {%r5}
   
    /* get the entry point to r5 */
    ldr     %r5 , [%r4, #OFFSET_ENTRY_POINT]

    /* get the TLB in user mode */
    ldr     %r4 , [%r4 , #OFFSET_TLB_USER_MODE]

    /* program the TLB to the thread memory space */
    mcr     p15, 0 , %r4, c2 , c0 , 0
  
    /* flush the current TLB */
    mcr     p15, 0, %r4, c8, c7, 0

    /* switch to user mode registers */
    msr     cpsr_c , #(CPSR_USER_MODE)

    /* place in the user stack the arguments (save an extra register to keep the stack correctly aligned) */
    push    {%r6-%r9}
    
    /* jump to the entry point of the thread (the arguments are already in r0-r3 and stack) */
    blx     %r5
    
    /* restore the previous stack position */
    pop     {%r6-%r9}

    /* push the registers needed for the syscall */
    push    {%r0 , %r4-%r12, %lr}

    /* make the syscall to restore supervisor mode */
    swi     PHAROS_SYSCALL_SPORADIC_THREAD_ENDED

     /* define the label position just after the swi call */
pharosSporadicThreadEndChannel:

    /* at this point we are running on supervisor mode */

    /* load to r2 the address of the kernel TLB */
    ldr     %r2, =kernelSectionTable

    /* program the c2 register to place the kernel TBL address so that when we pop from the supervisor stack we have permissions */
    mcr     p15, 0 , %r2, c2 , c0 , 0
  
    /* flush the current TLB */
    mcr     p15, 0, %r2, c8, c7, 0

    /*  switch to system mode registers */
    msr     cpsr_c , #(CPSR_SYSTEM_MODE)
  
    /* restore the stack to the correct place */
    pop     {%r0 , %r4-%r12, %lr}

    /* switch back to supervisor mode registers */
    msr     cpsr_c , #(CPSR_SUPERVISOR_MODE)
  
    /* get the previous TLB to r2 (pop from the supervisor stack) */
    pop     {%r2}

    /* pogram the TLB to the thread memory space */
    mcr     p15, 0 , %r2, c2 , c0 , 0
  
    /* flush the current TLB */
    mcr     p15, 0, %r2, c8, c7, 0

    /* restore all the registers */
    pop     {%r4 - %r12 , %lr}

    /* leave and restore ARM/Thumb mode */
    bx      lr

.endfunc
