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


#include <pharos/kernel/core/syscall.h>
#include <pharos/hal/cpu/asm.h>


.type pharosWait,                           %function
.type pharosBulkGet,                        %function
.type pharosBulkGive,                       %function
.type pharosChannelSend,                    %function
.type pharosChannelReceiveLocal,            %function
.type pharosChannelReceive,                 %function
.type pharosChannelBulkQueueGet,            %function
.type pharosChannelBulkFree,                %function
.type pharosClockGetDate,                   %function
.type pharosClockSetDate,                   %function
.type pharosClockGetTicks,                  %function
.type pharosClockGetSinceBoot,              %function
.type pharosShutdown,                       %function
.type pharosFatalErrorAnnounce,             %function
.type pharosErrorAnnounce,                  %function
.type pharosEventSend,                      %function
.type pharosEventReceive,                   %function
.type pharosLwQueueSend,                    %function
.type pharosLwQueueReceive,                 %function
.type pharosLwQueueReceiveTimeout,          %function
.type pharosHwQueueSend,                    %function
.type pharosHwQueueReceive,                 %function
.type pharosResourceAccess,                 %function
.type pharosSemCeilObtain,                  %function
.type pharosSemCeilRelease,                 %function
.type pharosSemMutexObtain,                 %function
.type pharosSemMutexObtainTimeout,          %function
.type pharosSemMutexRelease,                %function
.type pharosSemCountObtain,                 %function
.type pharosSemCountObtainTimeout,          %function
.type pharosSemCountRelease,                %function
.type pharosThreadSuspend,                  %function
.type pharosThreadSuspendSelf,              %function
.type pharosThreadResume,                   %function
.type pharosTimerStart,                     %function
.type pharosTimerStop,                      %function
.type pharosInterruptInstall,               %function
.type pharosInterruptRemove,                %function
.type pharosCriticalSectionExecute,         %function
.type outputChar,                           %function
.type pharosDriverIoWrite,                  %function
.type pharosFutureGet,                      %function
.type pharosFutureFree,                     %function
.type pharosFutureWait,                     %function
.type pharosFutureReady,                    %function
.type pharosFutureQueueAdd,                 %function
.type pharosFutureQueueRemove,              %function
.type pharosFutureQueueWait,                %function
.type pharosMalloc,                         %function
.type pharosFree,                           %function
.type pharosThreadAperiodicCreate,          %function
.type pharosThreadPeriodicCreate,           %function
.type pharosThreadSporadicCreate,           %function
.type pharosThreadStart,                    %function
.type pharosThreadStartDelay,               %function
.type pharosThreadGetIdSelf,                %function
.type pharosThreadGetId,                    %function
.type pharosThreadGetStatus,                %function
.type pharosThreadPrioritySet,              %function
.type pharosThreadStop,                     %function
.type pharosThreadStopSelf,                 %function
.type pharosThreadRestart,                  %function
.type pharosPartitionRestart,               %function
.type pharosThreadStack,                    %function


.global pharosWait
.global pharosBulkGet
.global pharosBulkGive
.global pharosChannelSend
.global pharosChannelReceiveLocal
.global pharosChannelReceive
.global pharosChannelBulkQueueGet
.global pharosChannelBulkFree
.global pharosClockGetDate
.global pharosClockSetDate
.global pharosClockGetTicks
.global pharosClockGetSinceBoot
.global pharosShutdown
.global pharosFatalErrorAnnounce
.global pharosErrorAnnounce
.global pharosEventSend
.global pharosEventReceive
.global pharosLwQueueSend
.global pharosLwQueueReceive
.global pharosLwQueueReceiveTimeout
.global pharosHwQueueSend
.global pharosHwQueueReceive
.global pharosResourceAccess
.global pharosSemCeilObtain
.global pharosSemCeilRelease
.global pharosSemMutexObtain
.global pharosSemMutexObtainTimeout
.global pharosSemMutexRelease
.global pharosSemCountObtain
.global pharosSemCountObtainTimeout
.global pharosSemCountRelease
.global pharosThreadSuspend
.global pharosThreadSuspendSelf
.global pharosThreadResume
.global pharosTimerStart
.global pharosTimerStop
.global pharosInterruptInstall
.global pharosInterruptRemove
.global pharosCriticalSectionExecute
.global pharosDriverIoWrite
.global outputChar
.global pharosFutureGet
.global pharosFutureFree
.global pharosFutureWait
.global pharosFutureReady
.global pharosFutureQueueAdd
.global pharosFutureQueueRemove
.global pharosFutureQueueWait
.global pharosMalloc
.global pharosFree
.global pharosThreadAperiodicCreate
.global pharosThreadPeriodicCreate
.global pharosThreadSporadicCreate
.global pharosThreadStart
.global pharosThreadStartDelay
.global pharosThreadGetIdSelf
.global pharosThreadGetId
.global pharosThreadGetStatus
.global pharosThreadSetPriority
.global pharosThreadStop
.global pharosThreadStopSelf
.global pharosThreadRestart
.global pharosPartitionRestart
.global pharosThreadStack


.syntax unified
.text
.arm


.func outputChar
outputChar:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     outputCharS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       outputSChar
    
outputCharS:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4 - %r12 , %lr}

    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_DEBUG_PRINT_CHAR
   
    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2 , %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc


/**
 * PharosWaitResult pharosWait(ClockTick clockTicks)
 */
.func pharosWait
pharosWait:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosWaitS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSWait

pharosWaitS:
    
    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4 - %r12 , %lr}

    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_WAIT

    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2 , %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc


/**
 * PharosBulkGetResult pharosBulkGet(BulkQueueNumber number , ptrBulk *bulkDestination , ptrBuffer buffer)
 */
.func pharosBulkGet
pharosBulkGet:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosBulkGetS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSBulkGet

pharosBulkGetS:
   
    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4-%r12 , %lr}

    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_BULK_GET

    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2, %r4 - %r12 , %lr}
 
    /* return to caller */ 
    bx      %lr

.endfunc


/**
 * PharosBulkGiveResult pharosBulkGive(ptrBulk bulk)
 */
.func pharosBulkGive
pharosBulkGive:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosBulkGiveS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSBulkGive

pharosBulkGiveS:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4-%r12 , %lr}

    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_BULK_GIVE

    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2, %r4 - %r12 , %lr}
 
    /* return to caller */ 
    bx      %lr

.endfunc


.func pharosShutdown
pharosShutdown:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosShutdownS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSShutdown

pharosShutdownS:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4-%r12 , %lr}

    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_SHUTDOWN

    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2, %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc


/**
 * PharosChannelBulkQueueGetResult pharosChannelBulkQueueGet(ChannelBulkQueueNumber number , ptrChannelBulkKernel *bulk , ptrBuffer buffer);
 */
.func pharosChannelBulkQueueGet
pharosChannelBulkQueueGet:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosChannelBulkQueueGetS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSChannelBulkQueueGet

pharosChannelBulkQueueGetS:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4-%r12 , %lr}
    
    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_CHANNEL_BULK_QUEUE_GET

    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2, %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc


/**
 * PharosChannelBulkFreeResult pharosChannelBulkFree(ChannelNumber number);
 */
.func pharosChannelBulkFree
pharosChannelBulkFree:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosChannelFreeBulkS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSChannelBulkFree

pharosChannelFreeBulkS:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4-%r12 , %lr}
    
    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_CHANNEL_FREE_BULK

    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2, %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc


/**
 * pharosChannelReceiveLocalR pharosChannelReceiveLocal(ChannelNumber id , ptrChannelBulkKernel *bulk , ptrBuffer bulkMessage ,
 *                                                      ptrBuffer dataMessage , ptrPharosPriority priority ,
 *                                                      ptrFutureId future , Timeout timeout);
 */
.func pharosChannelReceiveLocal
pharosChannelReceiveLocal:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosChannelReceiveLocalS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSChannelReceiveLocal

pharosChannelReceiveLocalS:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4-%r12 , %lr}
    
    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_CHANNEL_LOCAL_RECEIVE

    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2, %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc

/**
 * pharosChannelReceiveR pharosChannelReceive(ptrChannelId id , ptrChannelBulkKernel *bulk , ptrBuffer bulkMessage ,
 *                                            ptrBuffer dataMessage , ptrPharosPriority priority , ptrFutureId future ,
 *                                            Timeout timeout);
 */
.func pharosChannelReceive
pharosChannelReceive:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosChannelReceiveS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSChannelReceive

pharosChannelReceiveS:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4-%r12 , %lr}
    
    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_CHANNEL_RECEIVE

    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2, %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc


/**
 * PharosChannelSendR pharosChannelSend(ptrChannelId id , ptrChannelBulk bulk , PharosPriority priority , 
 *                                      ptrFutureId future)
 */
.func pharosChannelSend
pharosChannelSend:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosChannelSendS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSChannelSend

pharosChannelSendS:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4-%r12 , %lr}
    
    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_CHANNEL_SEND

    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2, %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc
    

/**
 * PharosClockGetResult pharosClockGetDate(volatile ptrDate date);
 */
.func pharosClockGetDate
pharosClockGetDate:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosClockGetDateS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSClockGetDate

pharosClockGetDateS:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4-%r12 , %lr}
    
    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_CLOCK_GET_DATE

    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2, %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc
    


/**
 * PharosClockSetResult pharosClockSetDate(ptrDate date);
 */
.func pharosClockSetDate
pharosClockSetDate:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosClockSetDateS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSClockSetDate

pharosClockSetDateS:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4-%r12 , %lr}
    
    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_CLOCK_SET_DATE

    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2, %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc


.func pharosClockGetTicks
pharosClockGetTicks:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosClockGetTicksS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSClockGetTicks

pharosClockGetTicksS:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4-%r12 , %lr}
    
    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_CLOCK_GET_TICKS

    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2, %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc

/**
 * uint64_t pharosClockGetSinceBoot();
 */
.func pharosClockGetSinceBoot
pharosClockGetSinceBoot:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosClockGetSinceBootS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSClockGetSinceBoot

pharosClockGetSinceBootS:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4-%r12 , %lr}
    
    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_CLOCK_GET_SINCE_BOOT

    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2, %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc
    
/**
 * void pharosFatalErrorAnnounce(PharosFatalError error , const char *file , const char *func , uint32_t line);
 */
.func pharosFatalErrorAnnounce
pharosFatalErrorAnnounce:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosFatalErrorAnnounceS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSFatalErrorAnnounce

pharosFatalErrorAnnounceS:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4-%r12 , %lr}
    
    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_FATAL_ERROR_ANNOUNCE

    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2, %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc
    

/**
 * void pharosErrorAnnounce(PharosError error , uint8_t *interruptedInstruction);
 */
.func pharosErrorAnnounce
pharosErrorAnnounce:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosErrorAnnounceS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSErrorAnnounce

pharosErrorAnnounceS:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4-%r12 , %lr}
    
    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_ERROR_ANNOUNCE

    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2, %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc
    

/**
 * PharosEventSendResult pharosEventSend(ThreadId thread, Event event);
 */
.func pharosEventSend
pharosEventSend:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosEventSendS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSEventSend

pharosEventSendS:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4-%r12 , %lr}
    
    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_EVENT_SEND

    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2, %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc
    

/**
 * PharosEventReceiveResult pharosEventReceive(Timeout timeout, ptrEvent event);
 */
.func pharosEventReceive
pharosEventReceive:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosEventReceiveS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSEventReceive

pharosEventReceiveS:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4-%r12 , %lr}
    
    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_EVENT_RECEIVE

    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2, %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc
    

/**
 * PharosLwQueueSendResult pharosLwQueueSend(LightWeightQueueNumber id , ptrBulk message , uint32_t size);
 */
.func pharosLwQueueSend
pharosLwQueueSend:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosLwQueueSendS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSLwQueueSend

pharosLwQueueSendS:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4-%r12 , %lr}
    
    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_LW_QUEUE_SEND

    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2, %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc
    

/**
 * PharosLwQueueReceiveResult pharosLwQueueReceive(LightWeightQueueNumber id , ptrBulk *message , ptrBuffer buffer);
 */
.func pharosLwQueueReceive
pharosLwQueueReceive:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosLwQueueReceiveS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSLwQueueReceive

pharosLwQueueReceiveS:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4-%r12 , %lr}
    
    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_LW_QUEUE_RECEIVE

    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2, %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc
    

/**
 * PharosLwQueueReceiveTimeoutResult pharosLwQueueReceiveTimeout(LightWeightQueueNumber id , ptrBulk *message , ptrBuffer buffer , Timeout timeout);
 */
.func pharosLwQueueReceiveTimeout
pharosLwQueueReceiveTimeout:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosLwQueueReceiveTimeoutS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSLwQueueReceiveTimeout

pharosLwQueueReceiveTimeoutS:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4-%r12 , %lr}

    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_LW_QUEUE_RECEIVE_TIMEOUT

    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2, %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc


/**
 * PharosHwQueueSendR pharosHwQueueSend(PartitionNumber partition , HwQueueNumber number , const uint8_t *buffer , uint32_t size , PharosPriority priority , ptrFutureId future, Timeout timeout);
 */
.func pharosHwQueueSend
pharosHwQueueSend:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosHwQueueSendS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSHwQueueSend

pharosHwQueueSendS:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4-%r12 , %lr}
    
    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_HW_QUEUE_SEND

    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2, %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc


/**
 * PharosHwReceiveResult pharosHwQueueReceive(HwQueueNumber number , uint8_t *buffer , uint32_t *size , ptrPharosPriority priority , Timeout timeout);
 */
.func pharosHwQueueReceive
pharosHwQueueReceive:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosHwQueueReceiveS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSHwQueueReceive

pharosHwQueueReceiveS:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4-%r12 , %lr}
    
    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_HW_QUEUE_RECEIVE

    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2, %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc


/**
 * PharosResourceAccessResult pharosResourceAccess(ptrResourceId id , uint32_t input , uint32_t *output , Timeout timeout);
 */
.func pharosResourceAccess
pharosResourceAccess:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosResourceAccessS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSResourceAccess

pharosResourceAccessS:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4-%r12 , %lr}
    
    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_RESOURCE_ACCESS

    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2, %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc



/**
 * PharosSemCeilObtainResult pharosSemCeilObtain(SemaphoreCeilingNumber number, Timeout timeout);
 */
.func pharosSemCeilObtain
pharosSemCeilObtain:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosSemCeilObtainS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSSemCeilObtain

pharosSemCeilObtainS:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4-%r12 , %lr}
    
    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_SEMAPHORE_CEILING_OBTAIN

    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2, %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc


/**
 * PharosSemCeilReleaseResult pharosSemCeilRelease(SemaphoreCeilingNumber id);
 */
.func pharosSemCeilRelease
pharosSemCeilRelease:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosSemCeilReleaseS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSSemCeilRelease

pharosSemCeilReleaseS:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4-%r12 , %lr}
    
    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_SEMAPHORE_CEILING_RELEASE

    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2, %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc


/**
 * PharosSemMutexObtainResult pharosSemMutexObtain(SemaphoreMutexNumber id);
 */
.func pharosSemMutexObtain
pharosSemMutexObtain:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosSemMutexObtainS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSSemMutexObtain

pharosSemMutexObtainS:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4-%r12 , %lr}
    
    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_SEMAPHORE_MUTEX_OBTAIN

    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2, %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc


/**
 * PharosSemMutexObtainTimeoutResult pharosSemMutexObtainTimeout(SemaphoreMutexNumber id, Timeout timeout);
 */
.func pharosSemMutexObtainTimeout
pharosSemMutexObtainTimeout:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosSemMutexObtainTimeoutS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSSemMutexObtainTimeout

pharosSemMutexObtainTimeoutS:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4-%r12 , %lr}
    
    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_SEMAPHORE_MUTEX_OBTAIN_TIMEOUT

    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2, %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc


/**
 * PharosSemMutexReleaseResult pharosSemMutexRelease(SemaphoreMutexNumber id);
 */
.func pharosSemMutexRelease
pharosSemMutexRelease:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosSemMutexReleaseS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSSemMutexRelease

pharosSemMutexReleaseS:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4-%r12 , %lr}
    
    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_SEMAPHORE_MUTEX_RELEASE

    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2, %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc


/**
 * PharosSemCountObtainResult pharosSemCountObtain(SemaphoreCountingNumber id);
 */
.func pharosSemCountObtain
pharosSemCountObtain:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosSemCountObtainS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSSemCountObtain

pharosSemCountObtainS:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4-%r12 , %lr}
    
    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_SEMAPHORE_COUNTING_OBTAIN

    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2, %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc


/**
 * PharosSemCountObtainTimeoutResult pharosSemCountObtainTimeout(SemaphoreCountingNumber id, Timeout timeout);
 */
.func pharosSemCountObtainTimeout
pharosSemCountObtainTimeout:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosSemCountObtainTimeoutS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSSemCountObtainTimeout

pharosSemCountObtainTimeoutS:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4-%r12 , %lr}
    
    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_SEMAPHORE_COUNTING_OBTAIN_TIMEOUT

    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2, %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc


/**
 * PharosSemCountReleaseResult pharosSemCountRelease(SemaphoreCountingNumber id);
 */
.func pharosSemCountRelease
pharosSemCountRelease:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosSemCountReleaseS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSSemCountRelease

pharosSemCountReleaseS:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4-%r12 , %lr}
    
    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_SEMAPHORE_COUNTING_RELEASE

    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2, %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc


/**
 * PharosThreadSuspendResult pharosThreadSuspend(ThreadId id);
 */
.func pharosThreadSuspend
pharosThreadSuspend:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosThreadSuspendS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSThreadSuspend

pharosThreadSuspendS:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4-%r12 , %lr}
    
    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_THREAD_SUSPEND

    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2, %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc


/**
 * PharosThreadSuspendSelfResult pharosThreadSuspendSelf();
 */
.func pharosThreadSuspendSelf
pharosThreadSuspendSelf:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosThreadSuspendSelfS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSThreadSuspendSelf

pharosThreadSuspendSelfS:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4-%r12 , %lr}
    
    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_THREAD_SUSPEND_SELF

    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2, %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc


/**
 * PharosThreadResumeResult pharosThreadResume(ThreadId id);
 */
.func pharosThreadResume
pharosThreadResume:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosThreadResumeS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSThreadResume

pharosThreadResumeS:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4-%r12 , %lr}

    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_THREAD_RESUME

    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2, %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc


/**
 * PharosTimerStartResult pharosTimerStart(TimerNumber number , uint64_t argument);
 */
.func pharosTimerStart
pharosTimerStart:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosTimerStartS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSTimerStart

pharosTimerStartS:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4-%r12 , %lr}
    
    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_TIMER_START

    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2, %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc

   
/**
 * PharosTimerStop pharosTimerStop(TimerNumber timerId);
 */
.func pharosTimerStop
pharosTimerStop:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosTimerStopS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSTimerStop

pharosTimerStopS:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4-%r12 , %lr}
    
    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_TIMER_STOP

    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2, %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc


/**
 * PharosInterruptCatchResult pharosInterruptInstall(uint32_t index);
 */
.func pharosInterruptInstall
pharosInterruptInstall:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosInterruptInstallS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSInterruptInstall

pharosInterruptInstallS:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4-%r12 , %lr}
    
    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_INTERRUPT_INSTALL

    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2, %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc


/**
 * PharosInterruptCatchResult pharosInterruptRemove(uint32_t index);
 */
.func pharosInterruptRemove
pharosInterruptRemove:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosInterruptRemoveS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSInterruptRemove

pharosInterruptRemoveS:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4-%r12 , %lr}
    
    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_INTERRUPT_REMOVE

    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2, %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc


/**
 * PharosCriticalSectionExecuteResult pharosCriticalSectionExecute(uint32_t index , uint32_t arg1 , uint32_t arg2 , uint32_t arg3 , uint32_t arg4);
 */
.func pharosCriticalSectionExecute
pharosCriticalSectionExecute:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosCriticalSectionExecuteS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSCriticalSectionExecute

pharosCriticalSectionExecuteS:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4-%r12 , %lr}
    
    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_CRITICAL_SECTION_EXECUTE

    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2, %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc


.func pharosFutureGet
pharosFutureGet:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosFutureGetS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSFutureGet
    
pharosFutureGetS:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4 - %r12 , %lr}

    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_FUTURE_GET
   
    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2 , %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc


.func pharosFutureFree
pharosFutureFree:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosFutureFreeS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSFutureFree
    
pharosFutureFreeS:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4 - %r12 , %lr}

    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_FUTURE_FREE
   
    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2 , %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc


.func pharosFutureWait
pharosFutureWait:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosFutureWaitS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSFutureWait
    
pharosFutureWaitS:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4 - %r12 , %lr}

    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_FUTURE_WAIT
   
    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2 , %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc


.func pharosFutureReady
pharosFutureReady:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosFutureReadyS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSFutureReady
    
pharosFutureReadyS:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4 - %r12 , %lr}

    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_FUTURE_READY
   
    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2 , %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc


.func pharosFutureQueueAdd
pharosFutureQueueAdd:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosFutureQueueAddS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSFutureQueueAdd
    
pharosFutureQueueAddS:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4 - %r12 , %lr}

    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_FUTURE_QUEUE_ADD
   
    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2 , %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc


.func pharosFutureQueueRemove
pharosFutureQueueRemove:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosFutureQueueRemoveS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSFutureQueueRemove
    
pharosFutureQueueRemoveS:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4 - %r12 , %lr}

    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_FUTURE_QUEUE_REMOVE
   
    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2 , %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc


.func pharosFutureQueueWait
pharosFutureQueueWait:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosFutureQueueWaitS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSFutureQueueWait
    
pharosFutureQueueWaitS:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4 - %r12 , %lr}

    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_FUTURE_QUEUE_WAIT
   
    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2 , %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc


.func pharosMalloc
pharosMalloc:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     SpharosMalloc
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSMalloc
    
SpharosMalloc:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4 - %r12 , %lr}

    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_MALLOC
   
    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2 , %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc


.func pharosFree
pharosFree:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     SpharosFree
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosFreeS
    
SpharosFree:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4 - %r12 , %lr}

    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_FREE
   
    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2 , %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc


.func pharosThreadAperiodicCreate
pharosThreadAperiodicCreate:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosThreadAperiodicCreateS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSThreadAperiodicCreate
    
pharosThreadAperiodicCreateS:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4 - %r12 , %lr}

    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_THREAD_APERIODIC_CREATE
   
    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2 , %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc


.func pharosThreadPeriodicCreate
pharosThreadPeriodicCreate:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosThreadPeriodicCreateS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSThreadPeriodicCreate
    
pharosThreadPeriodicCreateS:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4 - %r12 , %lr}

    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_THREAD_PERIODIC_CREATE
   
    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2 , %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc



.func pharosThreadSporadicCreate
pharosThreadSporadicCreate:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosThreadSporadicCreateS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSThreadSporadicCreate
    
pharosThreadSporadicCreateS:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4 - %r12 , %lr}

    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_THREAD_SPORADIC_CREATE
   
    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2 , %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc



.func pharosThreadStart
pharosThreadStart:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosThreadStartS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSThreadStart
    
pharosThreadStartS:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4 - %r12 , %lr}

    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_THREAD_START
   
    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2 , %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc



.func pharosThreadStartDelay
pharosThreadStartDelay:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosThreadStartDelayS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSThreadStartDelay
    
pharosThreadStartDelayS:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4 - %r12 , %lr}

    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_THREAD_START_DELAY
   
    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2 , %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc



.func pharosThreadGetIdSelf
pharosThreadGetIdSelf:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosThreadGetIdSelfS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSThreadGetIdSelf
    
pharosThreadGetIdSelfS:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4 - %r12 , %lr}

    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_THREAD_GET_ID_SELF
   
    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2 , %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc



.func pharosThreadGetId
pharosThreadGetId:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosThreadGetIdS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSThreadGetId
    
pharosThreadGetIdS:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4 - %r12 , %lr}

    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_THREAD_GET_ID
   
    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2 , %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc



.func pharosThreadGetStatus
pharosThreadGetStatus:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosThreadGetStatusS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSThreadGetStatus
    
pharosThreadGetStatusS:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4 - %r12 , %lr}

    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_THREAD_GET_STATUS
   
    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2 , %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc



.func pharosThreadSetPriority
pharosThreadSetPriority:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosThreadSetPriorityS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSThreadSetPriority
    
pharosThreadSetPriorityS:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4 - %r12 , %lr}

    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_THREAD_SET_PRIORITY
   
    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2 , %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc



.func pharosThreadStop
pharosThreadStop:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosThreadStopS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSThreadStop
    
pharosThreadStopS:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4 - %r12 , %lr}

    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_THREAD_STOP
   
    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2 , %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc



.func pharosThreadStopSelf
pharosThreadStopSelf:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosThreadStopSelfS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSThreadStopSelf
    
pharosThreadStopSelfS:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4 - %r12 , %lr}

    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_THREAD_STOP_SELF
   
    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2 , %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc



.func pharosThreadRestart
pharosThreadRestart:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosThreadRestartS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSThreadRestart
    
pharosThreadRestartS:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4 - %r12 , %lr}

    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_THREAD_RESTART
   
    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2 , %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc


.func pharosPartitionRestart
pharosPartitionRestart:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosPartitionRestartS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSPartitionRestart
    
pharosPartitionRestartS:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4 - %r12 , %lr}

    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_PARTITION_RESTART
   
    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2 , %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc



.func pharosThreadStack
pharosThreadStack:

    /* save r0 */
    push    {r0}
    
    /* get the CPSR to r0 */
    mrs     %r0 , CPSR

    /* clear all the bits except for the mode */
    and     %r0 , %r0 , #0x1f

    /* check if we are in the user mode */
    cmp     %r0 , #CPSR_USER_MODE

    /* restore r0 */
    pop     {r0}

    /* if we are in user mode then call the syscall */
    beq     pharosThreadStackS
   
    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSThreadStack
    
pharosThreadStackS:

    /* save r0 and r1 because they will be overriden in the syscall. Save the lr so we know where to return when the syscall finishes */
    push    {%r0 , %r4 - %r12 , %lr}

    /*  call the supervisor function */
    swi     PHAROS_SYSCALL_THREAD_STACK
   
    /* restore the stack to its original position (use r2 and r3 since they can be overwritten) and restore lr */
    pop     {%r2 , %r4 - %r12 , %lr}

    /* return to caller */
    bx      %lr

.endfunc
