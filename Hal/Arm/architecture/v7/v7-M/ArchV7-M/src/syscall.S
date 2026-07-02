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
.type pharosFutureGet                       %function
.type pharosFutureFree                      %function
.type pharosFutureWait                      %function
.type pharosFutureReady                     %function
.type pharosFutureQueueAdd                  %function
.type pharosFutureQueueRemove               %function
.type pharosFutureQueueWait                 %function
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
.type pharosThreadSetPriority,              %function
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
.thumb


.func outputChar
outputChar:

    /* save r0 */
    push    {%r0}
    
    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     outputCharPriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

    /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     outputCharS

    /* syscall done in privilege mode */
outputCharPriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       outputSChar
    
outputCharS:

    /* restore r0 */
    pop     {%r0}

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_DEBUG_PRINT_CHAR

    /* return to caller */
    bx      %lr

.endfunc


/**
 * PharosWaitResult pharosWait(ClockTick clockTicks)
 */
.func pharosWait
pharosWait:

    /* save r0 */
    push    {%r0}

    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosWaitPriv
    
    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosWaitS
   
    /* syscall done in privilege mode */
pharosWaitPriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSWait

pharosWaitS:
    
    /* restore r0 */
    pop     {%r0}

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_WAIT
 
    /* return to caller */
    bx      %lr

.endfunc


/**
 * PharosBulkGetResult pharosBulkGet(BulkQueueNumber number , ptrBulk *bulkDestination , ptrBuffer buffer)
 */
.func pharosBulkGet
pharosBulkGet:

    /* save r0 */
    push    {%r0}

    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosBulkGetPriv
    
    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosBulkGetS
   
    /* syscall done in privilege mode */
pharosBulkGetPriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSBulkGet

pharosBulkGetS:
   
    /* restore r0 */
    pop     {%r0}

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_BULK_GET
 
    /* return to caller */ 
    bx      %lr

.endfunc


/**
 * PharosBulkGiveResult pharosBulkGive(ptrBulk bulk)
 */
.func pharosBulkGive
pharosBulkGive:

    /* save r0 */
    push    {%r0}
    
    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosBulkGivePriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosBulkGiveS
   
    /* syscall done in privilege mode */
pharosBulkGivePriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSBulkGive

pharosBulkGiveS:

    /* restore r0 */
    pop     {%r0}

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_BULK_GIVE
 
    /* return to caller */ 
    bx      %lr

.endfunc


.func pharosShutdown
pharosShutdown:

    /* save r0 */
    push    {%r0}
    
    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosShutdownPriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosShutdownS
   
    /* syscall done in privilege mode */
pharosShutdownPriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSShutdown

pharosShutdownS:

    /* restore r0 */
    pop     {%r0}

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_SHUTDOWN

    /* return to caller */
    bx      %lr

.endfunc


/**
 * PharosChannelBulkQueueGetResult pharosChannelBulkQueueGet(ChannelBulkQueueNumber number , ptrChannelBulkKernel *bulk , ptrBuffer buffer);
 */
.func pharosChannelBulkQueueGet
pharosChannelBulkQueueGet:

    /* save r0 */
    push    {%r0}
    
    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosChannelBulkQueueGetPriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosChannelBulkQueueGetS
   
    /* syscall done in privilege mode */
pharosChannelBulkQueueGetPriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSChannelBulkQueueGet

pharosChannelBulkQueueGetS:

    /* restore r0 */
    pop     {%r0}
    
    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_CHANNEL_BULK_QUEUE_GET

    /* return to caller */
    bx      %lr

.endfunc


/**
 * PharosChannelBulkFreeResult pharosChannelBulkFree(ChannelNumber number);
 */
.func pharosChannelBulkFree
pharosChannelBulkFree:

    /* save r0 */
    push    {%r0}
    
    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosChannelFreeBulkPriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosChannelFreeBulkS
   
    /* syscall done in privilege mode */
pharosChannelFreeBulkPriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSChannelBulkFree

pharosChannelFreeBulkS:

    /* restore r0 */
    pop     {%r0}
    
    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_CHANNEL_FREE_BULK

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
    push    {%r0}
    
    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosChannelReceiveLocalPriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosChannelReceiveLocalS
   
    /* syscall done in privilege mode */
pharosChannelReceiveLocalPriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSChannelReceiveLocal

pharosChannelReceiveLocalS:

    /* restore r0 */
    pop     {%r0}
    
    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_CHANNEL_LOCAL_RECEIVE

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
    push    {%r0}
    
    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosChannelReceivePriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosChannelReceiveS
   
    /* syscall done in privilege mode */
pharosChannelReceivePriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSChannelReceive

pharosChannelReceiveS:

    /* restore r0 */
    pop     {%r0}
    
    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_CHANNEL_RECEIVE

    /* return to caller */
    bx      %lr

.endfunc


/**
 * PharosChannelSendResult pharosChannelSend(ptrChannelId id , ptrChannelBulkKernel bulk , uint32_t offset , uint32_t size , PharosPriority priority);
 */
.func pharosChannelSend
pharosChannelSend:

    /* save r0 */
    push    {%r0}
    
    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosChannelSendPriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosChannelSendS
   
    /* syscall done in privilege mode */
pharosChannelSendPriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSChannelSend

pharosChannelSendS:

    /* restore r0 */
    pop     {%r0}
    
    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_CHANNEL_SEND

    /* return to caller */
    bx      %lr

.endfunc
    

/**
 * PharosClockGetResult pharosClockGetDate(volatile ptrDate date);
 */
.func pharosClockGetDate
pharosClockGetDate:

    /* save r0 */
    push    {%r0}
    
    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosClockGetDatePriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosClockGetDateS
   
    /* syscall done in privilege mode */
pharosClockGetDatePriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSClockGetDate

pharosClockGetDateS:

    /* restore r0 */
    pop     {%r0}
    
    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_CLOCK_GET_DATE

    /* return to caller */
    bx      %lr

.endfunc
    


/**
 * PharosClockSetResult pharosClockSetDate(ptrDate date);
 */
.func pharosClockSetDate
pharosClockSetDate:

    /* save r0 */
    push    {%r0}
    
    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosClockSetDatePriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosClockSetDateS
   
    /* syscall done in privilege mode */
pharosClockSetDatePriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSClockSetDate

pharosClockSetDateS:

    /* restore r0 */
    pop     {%r0}
    
    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_CLOCK_SET_DATE

    /* return to caller */
    bx      %lr

.endfunc


.func pharosClockGetTicks
pharosClockGetTicks:

    /* save r0 */
    push    {%r0}
    
    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosClockGetTicksPriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosClockGetTicksS
   
    /* syscall done in privilege mode */
pharosClockGetTicksPriv:

    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSClockGetTicks

pharosClockGetTicksS:

    /* restore r0 */
    pop     {%r0}
    
    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_CLOCK_GET_TICKS

    /* return to caller */
    bx      %lr

.endfunc

/**
 * uint64_t pharosClockGetSinceBoot();
 */
.func pharosClockGetSinceBoot
pharosClockGetSinceBoot:

    /* save r0 */
    push    {%r0}
    
    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosClockGetSinceBootPriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosClockGetSinceBootS
   
    /* syscall done in privilege mode */
pharosClockGetSinceBootPriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSClockGetSinceBoot

pharosClockGetSinceBootS:

    /* restore r0 */
    pop     {%r0}
    
    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_CLOCK_GET_SINCE_BOOT

    /* return to caller */
    bx      %lr

.endfunc
    
/**
 * void pharosFatalErrorAnnounce(PharosFatalError error , const char *file , const char *func , uint32_t line);
 */
.func pharosFatalErrorAnnounce
pharosFatalErrorAnnounce:

    /* save r0 */
    push    {%r0}
    
    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosFatalErrorAnnouncePriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosFatalErrorAnnounceS
   
    /* syscall done in privilege mode */
pharosFatalErrorAnnouncePriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSFatalErrorAnnounce

pharosFatalErrorAnnounceS:

    /* restore r0 */
    pop     {%r0}
    
    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_FATAL_ERROR_ANNOUNCE

    /* return to caller */
    bx      %lr

.endfunc
    

/**
 * void pharosErrorAnnounce(PharosError error , uint8_t *interruptedInstruction);
 */
.func pharosErrorAnnounce
pharosErrorAnnounce:

    /* save r0 */
    push    {%r0}
    
    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosErrorAnnouncePriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosErrorAnnounceS
   
    /* syscall done in privilege mode */
pharosErrorAnnouncePriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSErrorAnnounce

pharosErrorAnnounceS:

    /* restore r0 */
    pop     {%r0}
    
    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_ERROR_ANNOUNCE

    /* return to caller */
    bx      %lr

.endfunc
    

/**
 * PharosEventSendResult pharosEventSend(ThreadId thread, Event event);
 */
.func pharosEventSend
pharosEventSend:

    /* save r0 */
    push    {%r0}

    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosEventSendPriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosEventSendS
   
    /* syscall done in privilege mode */
pharosEventSendPriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSEventSend

pharosEventSendS:

    /* restore r0 */
    pop     {%r0}
    
    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_EVENT_SEND

    /* return to caller */
    bx      %lr

.endfunc
    

/**
 * PharosEventReceiveResult pharosEventReceive(Timeout timeout, ptrEvent event);
 */
.func pharosEventReceive
pharosEventReceive:

    /* save r0 */
    push    {%r0}
    
    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosEventReceivePriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosEventReceiveS
   
    /* syscall done in privilege mode */
pharosEventReceivePriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSEventReceive

pharosEventReceiveS:

    /* restore r0 */
    pop     {%r0}
    
    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_EVENT_RECEIVE

    /* return to caller */
    bx      %lr

.endfunc
    

/**
 * PharosLwQueueSendResult pharosLwQueueSend(LightWeightQueueNumber id , ptrBulk message , uint32_t size);
 */
.func pharosLwQueueSend
pharosLwQueueSend:

    /* save r0 */
    push    {%r0}
    
    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosLwQueueSendPriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosLwQueueSendS
   
    /* syscall done in privilege mode */
pharosLwQueueSendPriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSLwQueueSend

pharosLwQueueSendS:

    /* restore r0 */
    pop     {%r0}
    
    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_LW_QUEUE_SEND

    /* return to caller */
    bx      %lr

.endfunc
    

/**
 * PharosLwQueueReceiveResult pharosLwQueueReceive(LightWeightQueueNumber id , ptrBulk *message , ptrBuffer buffer);
 */
.func pharosLwQueueReceive
pharosLwQueueReceive:

    /* save r0 */
    push    {%r0}
    
    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosLwQueueReceivePriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosLwQueueReceiveS
   
    /* syscall done in privilege mode */
pharosLwQueueReceivePriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSLwQueueReceive

pharosLwQueueReceiveS:

    /* restore r0 */
    pop     {%r0}
    
    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_LW_QUEUE_RECEIVE

    /* return to caller */
    bx      %lr

.endfunc
    

/**
 * PharosLwQueueReceiveTimeoutResult pharosLwQueueReceiveTimeout(LightWeightQueueNumber id , ptrBulk *message , ptrBuffer buffer , Timeout timeout);
 */
.func pharosLwQueueReceiveTimeout
pharosLwQueueReceiveTimeout:

    /* save r0 */
    push    {%r0}
    
    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosLwQueueReceiveTimeoutPriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosLwQueueReceiveTimeoutS
   
    /* syscall done in privilege mode */
pharosLwQueueReceiveTimeoutPriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSLwQueueReceiveTimeout

pharosLwQueueReceiveTimeoutS:

    /* restore r0 */
    pop     {%r0}

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_LW_QUEUE_RECEIVE_TIMEOUT

    /* return to caller */
    bx      %lr

.endfunc


/**
 * PharosHwSendResult pharosHwQueueSend(ptrHwQueueId id , const uint8_t *buffer , uint32_t size , PharosPriority priority);
 */
.func pharosHwQueueSend
pharosHwQueueSend:

    /* save r0 */
    push    {%r0}
    
    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosHwQueueSendPriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosHwQueueSendS
   
    /* syscall done in privilege mode */
pharosHwQueueSendPriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSHwQueueSend

pharosHwQueueSendS:

    /* restore r0 */
    pop     {%r0}
    
    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_HW_QUEUE_SEND

    /* return to caller */
    bx      %lr

.endfunc


/**
 * PharosHwReceiveResult pharosHwQueueReceive(HwQueueNumber number , uint8_t *buffer , uint32_t *size , ptrPharosPriority priority , Timeout timeout);
 */
.func pharosHwQueueReceive
pharosHwQueueReceive:

    /* save r0 */
    push    {%r0}

    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosHwQueueReceivePriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosHwQueueReceiveS
   
    /* syscall done in privilege mode */
pharosHwQueueReceivePriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSHwQueueReceive

pharosHwQueueReceiveS:

    /* restore r0 */
    pop     {%r0}
    
    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_HW_QUEUE_RECEIVE

    /* return to caller */
    bx      %lr

.endfunc


/**
 * PharosResourceAccessResult pharosResourceAccess(ptrResourceId id , uint32_t input , uint32_t *output , Timeout timeout);
 */
.func pharosResourceAccess
pharosResourceAccess:

    /* save r0 */
    push    {%r0}
    
    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosResourceAccessPriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosResourceAccessS
   
    /* syscall done in privilege mode */
pharosResourceAccessPriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSResourceAccess

pharosResourceAccessS:

    /* restore r0 */
    pop     {%r0}

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_RESOURCE_ACCESS

    /* return to caller */
    bx      %lr

.endfunc



/**
 * PharosSemCeilObtainResult pharosSemCeilObtain(SemaphoreCeilingNumber number, Timeout timeout);
 */
.func pharosSemCeilObtain
pharosSemCeilObtain:

    /* save r0 */
    push    {%r0}
    
    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosSemCeilObtainPriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosSemCeilObtainS
   
    /* syscall done in privilege mode */
pharosSemCeilObtainPriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSSemCeilObtain

pharosSemCeilObtainS:

    /* restore r0 */
    pop     {%r0}
    
    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_SEMAPHORE_CEILING_OBTAIN

    /* return to caller */
    bx      %lr

.endfunc


/**
 * PharosSemCeilReleaseResult pharosSemCeilRelease(SemaphoreCeilingNumber id);
 */
.func pharosSemCeilRelease
pharosSemCeilRelease:

    /* save r0 */
    push    {%r0}
    
    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosSemCeilReleasePriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosSemCeilReleaseS
   
    /* syscall done in privilege mode */
pharosSemCeilReleasePriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSSemCeilRelease

pharosSemCeilReleaseS:

    /* restore r0 */
    pop     {%r0}
    
    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_SEMAPHORE_CEILING_RELEASE

    /* return to caller */
    bx      %lr

.endfunc


/**
 * PharosSemMutexObtainResult pharosSemMutexObtain(SemaphoreMutexNumber id);
 */
.func pharosSemMutexObtain
pharosSemMutexObtain:

    /* save r0 */
    push    {%r0}
    
    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosSemMutexObtainPriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosSemMutexObtainS
   
    /* syscall done in privilege mode */
pharosSemMutexObtainPriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSSemMutexObtain

pharosSemMutexObtainS:

    /* restore r0 */
    pop     {%r0}
    
    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_SEMAPHORE_MUTEX_OBTAIN

    /* return to caller */
    bx      %lr

.endfunc


/**
 * PharosSemMutexObtainTimeoutResult pharosSemMutexObtainTimeout(SemaphoreMutexNumber id, Timeout timeout);
 */
.func pharosSemMutexObtainTimeout
pharosSemMutexObtainTimeout:

    /* save r0 */
    push    {%r0}
    
    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosSemMutexObtainTimeoutPriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosSemMutexObtainTimeoutS
   
    /* syscall done in privilege mode */
pharosSemMutexObtainTimeoutPriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSSemMutexObtainTimeout

pharosSemMutexObtainTimeoutS:

    /* restore r0 */
    pop     {%r0}
    
    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_SEMAPHORE_MUTEX_OBTAIN_TIMEOUT

    /* return to caller */
    bx      %lr

.endfunc


/**
 * PharosSemMutexReleaseResult pharosSemMutexRelease(SemaphoreMutexNumber id);
 */
.func pharosSemMutexRelease
pharosSemMutexRelease:

    /* save r0 */
    push    {%r0}
    
    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosSemMutexReleasePriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosSemMutexReleaseS
   
    /* syscall done in privilege mode */
pharosSemMutexReleasePriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSSemMutexRelease

pharosSemMutexReleaseS:

    /* restore r0 */
    pop     {%r0}
    
    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_SEMAPHORE_MUTEX_RELEASE

    /* return to caller */
    bx      %lr

.endfunc


/**
 * PharosSemCountObtainResult pharosSemCountObtain(SemaphoreCountingNumber id);
 */
.func pharosSemCountObtain
pharosSemCountObtain:

    /* save r0 */
    push    {%r0}
    
    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosSemCountObtainPriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosSemCountObtainS
   
    /* syscall done in privilege mode */
pharosSemCountObtainPriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSSemCountObtain

pharosSemCountObtainS:

    /* restore r0 */
    pop     {%r0}

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_SEMAPHORE_COUNTING_OBTAIN

    /* return to caller */
    bx      %lr

.endfunc


/**
 * PharosSemCountObtainTimeoutResult pharosSemCountObtainTimeout(SemaphoreCountingNumber id, Timeout timeout);
 */
.func pharosSemCountObtainTimeout
pharosSemCountObtainTimeout:

    /* save r0 */
    push    {%r0}
    
    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosSemCountObtainTimeoutPriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosSemCountObtainTimeoutS
   
    /* syscall done in privilege mode */
pharosSemCountObtainTimeoutPriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSSemCountObtainTimeout

pharosSemCountObtainTimeoutS:

    /* restore r0 */
    pop     {%r0}
    
    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_SEMAPHORE_COUNTING_OBTAIN_TIMEOUT

    /* return to caller */
    bx      %lr

.endfunc


/**
 * PharosSemCountReleaseResult pharosSemCountRelease(SemaphoreCountingNumber id);
 */
.func pharosSemCountRelease
pharosSemCountRelease:

    /* save r0 */
    push    {%r0}
    
    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosSemCountReleasePriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosSemCountReleaseS
   
    /* syscall done in privilege mode */
pharosSemCountReleasePriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSSemCountRelease

pharosSemCountReleaseS:

    /* restore r0 */
    pop     {%r0}
    
    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_SEMAPHORE_COUNTING_RELEASE

    /* return to caller */
    bx      %lr

.endfunc


/**
 * PharosThreadSuspendResult pharosThreadSuspend(ThreadId id);
 */
.func pharosThreadSuspend
pharosThreadSuspend:

    /* save r0 */
    push    {%r0}
    
    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosThreadSuspendPriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosThreadSuspendS
   
    /* syscall done in privilege mode */
pharosThreadSuspendPriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSThreadSuspend

pharosThreadSuspendS:

    /* restore r0 */
    pop     {%r0}

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_THREAD_SUSPEND

    /* return to caller */
    bx      %lr

.endfunc


/**
 * PharosThreadSuspendSelfResult pharosThreadSuspendSelf();
 */
.func pharosThreadSuspendSelf
pharosThreadSuspendSelf:

    /* save r0 */
    push    {%r0}
    
    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosThreadSuspendSelfPriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosThreadSuspendSelfS
   
    /* syscall done in privilege mode */
pharosThreadSuspendSelfPriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSThreadSuspendSelf

pharosThreadSuspendSelfS:

    /* restore r0 */
    pop     {%r0}

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_THREAD_SUSPEND_SELF

    /* return to caller */
    bx      %lr

.endfunc


/**
 * PharosThreadResumeResult pharosThreadResume(ThreadId id);
 */
.func pharosThreadResume
pharosThreadResume:

    /* save r0 */
    push    {%r0}
    
    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosThreadResumePriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosThreadResumeS
   
    /* syscall done in privilege mode */
pharosThreadResumePriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSThreadResume

pharosThreadResumeS:

    /* restore r0 */
    pop     {%r0}

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_THREAD_RESUME

    /* return to caller */
    bx      %lr

.endfunc


/**
 * PharosTimerStartResult pharosTimerStart(TimerNumber number , uint64_t argument);
 */
.func pharosTimerStart
pharosTimerStart:

    /* save r0 */
    push    {%r0}
    
    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosTimerStartPriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosTimerStartS
   
    /* syscall done in privilege mode */
pharosTimerStartPriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSTimerStart

pharosTimerStartS:

    /* restore r0 */
    pop     {%r0}
    
    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_TIMER_START

    /* return to caller */
    bx      %lr

.endfunc

   
/**
 * PharosTimerStop pharosTimerStop(TimerNumber timerId);
 */
.func pharosTimerStop
pharosTimerStop:

    /* save r0 */
    push    {%r0}
    
    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosTimerStopPriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosTimerStopS
   
    /* syscall done in privilege mode */
pharosTimerStopPriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSTimerStop

pharosTimerStopS:

    /* restore r0 */
    pop     {%r0}
    
    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_TIMER_STOP

    /* return to caller */
    bx      %lr

.endfunc


/**
 * PharosInterruptCatchResult pharosInterruptInstall(uint32_t index);
 */
.func pharosInterruptInstall
pharosInterruptInstall:

    /* save r0 */
    push    {%r0}
    
    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosInterruptInstallPriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosInterruptInstallS
   
    /* syscall done in privilege mode */
pharosInterruptInstallPriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSInterruptInstall

pharosInterruptInstallS:

    /* restore r0 */
    pop     {%r0}
    
    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_INTERRUPT_INSTALL

    /* return to caller */
    bx      %lr

.endfunc


/**
 * PharosInterruptCatchResult pharosInterruptRemove(uint32_t index);
 */
.func pharosInterruptRemove
pharosInterruptRemove:

    /* save r0 */
    push    {%r0}
    
    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosInterruptRemovePriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosInterruptRemoveS
   
    /* syscall done in privilege mode */
pharosInterruptRemovePriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSInterruptRemove

pharosInterruptRemoveS:

    /* restore r0 */
    pop     {%r0}
    
    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_INTERRUPT_REMOVE

    /* return to caller */
    bx      %lr

.endfunc


/**
 * PharosCriticalSectionExecuteResult pharosCriticalSectionExecute(uint32_t index , uint32_t arg1 , uint32_t arg2 , uint32_t arg3 , uint32_t arg4);
 */
.func pharosCriticalSectionExecute
pharosCriticalSectionExecute:

    /* save r0 */
    push    {%r0}
    
    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosCriticalSectionExecutePriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosCriticalSectionExecuteS
   
    /* syscall done in privilege mode */
pharosCriticalSectionExecutePriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSCriticalSectionExecute

pharosCriticalSectionExecuteS:

    /* restore r0 */
    pop     {%r0}

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_CRITICAL_SECTION_EXECUTE

    /* return to caller */
    bx      %lr

.endfunc


.func pharosFutureGet
pharosFutureGet:

    /* save r0 */
    push    {%r0}
    
    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosFutureGetPriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosFutureGetS
   
    /* syscall done in privilege mode */
pharosFutureGetPriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSFutureGet

pharosFutureGetS:

    /* restore r0 */
    pop     {%r0}

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_FUTURE_GET

    /* return to caller */
    bx      %lr

.endfunc


.func pharosFutureFree
pharosFutureFree:

    /* save r0 */
    push    {%r0}
    
    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosFutureFreePriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosFutureFreeS
   
    /* syscall done in privilege mode */
pharosFutureFreePriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSFutureFree

pharosFutureFreeS:

    /* restore r0 */
    pop     {%r0}

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_FUTURE_FREE

    /* return to caller */
    bx      %lr

.endfunc


.func pharosFutureWait
pharosFutureWait:

    /* save r0 */
    push    {%r0}
    
    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosFutureWaitPriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosFutureWaitS
   
    /* syscall done in privilege mode */
pharosFutureWaitPriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSFutureWait

pharosFutureWaitS:

    /* restore r0 */
    pop     {%r0}

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_FUTURE_WAIT

    /* return to caller */
    bx      %lr

.endfunc


.func pharosFutureReady
pharosFutureReady:

    /* save r0 */
    push    {%r0}
    
    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosFutureReadyPriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosFutureReadyS
   
    /* syscall done in privilege mode */
pharosFutureReadyPriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSFutureReady

pharosFutureReadyS:

    /* restore r0 */
    pop     {%r0}

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_FUTURE_READY

    /* return to caller */
    bx      %lr

.endfunc


.func pharosFutureQueueAdd
pharosFutureQueueAdd:

    /* save r0 */
    push    {%r0}
    
    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosFutureQueueAddPriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosFutureQueueAddS
   
    /* syscall done in privilege mode */
pharosFutureQueueAddPriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSFutureQueueAdd

pharosFutureQueueAddS:

    /* restore r0 */
    pop     {%r0}

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_FUTURE_QUEUE_ADD

    /* return to caller */
    bx      %lr

.endfunc


.func pharosFutureQueueRemove
pharosFutureQueueRemove:

    /* save r0 */
    push    {%r0}
    
    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosFutureQueueRemovePriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosFutureQueueRemoveS
   
    /* syscall done in privilege mode */
pharosFutureQueueRemovePriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSFutureQueueRemove

pharosFutureQueueRemoveS:

    /* restore r0 */
    pop     {%r0}

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_FUTURE_QUEUE_REMOVE

    /* return to caller */
    bx      %lr

.endfunc


.func pharosFutureQueueWait
pharosFutureQueueWait:

    /* save r0 */
    push    {%r0}
    
    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosFutureQueueWaitPriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosFutureQueueWaitS
   
    /* syscall done in privilege mode */
pharosFutureQueueWaitPriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSFutureQueueWait

pharosFutureQueueWaitS:

    /* restore r0 */
    pop     {%r0}

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_FUTURE_QUEUE_WAIT

    /* return to caller */
    bx      %lr

.endfunc


.func pharosMalloc
pharosMalloc:

    /* save r0 */
    push    {%r0}
    
    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosMallocPriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosMallocUser
   
    /* syscall done in privilege mode */
pharosMallocPriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSMalloc

pharosMallocUser:

    /* restore r0 */
    pop     {%r0}

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_MALLOC

    /* return to caller */
    bx      %lr

.endfunc


.func pharosFree
pharosFree:

    /* save r0 */
    push    {%r0}
    
    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosFreePriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosFreeUser
   
    /* syscall done in privilege mode */
pharosFreePriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosFreeS

pharosFreeUser:

    /* restore r0 */
    pop     {%r0}

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_FREE

    /* return to caller */
    bx      %lr

.endfunc


.func pharosThreadAperiodicCreate
pharosThreadAperiodicCreate:

    /* save r0 */
    push    {%r0}
    
    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosThreadAperiodicCreatePriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosThreadAperiodicCreateUser
   
    /* syscall done in privilege mode */
pharosThreadAperiodicCreatePriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSThreadAperiodicCreate

pharosThreadAperiodicCreateUser:

    /* restore r0 */
    pop     {%r0}

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_THREAD_APERIODIC_CREATE

    /* return to caller */
    bx      %lr

.endfunc



.func pharosThreadPeriodicCreate
pharosThreadPeriodicCreate:

    /* save r0 */
    push    {%r0}
    
    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosThreadPeriodicCreatePriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosThreadPeriodicCreateUser
   
    /* syscall done in privilege mode */
pharosThreadPeriodicCreatePriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSThreadPeriodicCreate

pharosThreadPeriodicCreateUser:

    /* restore r0 */
    pop     {%r0}

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_THREAD_PERIODIC_CREATE

    /* return to caller */
    bx      %lr

.endfunc



.func pharosThreadSporadicCreate
pharosThreadSporadicCreate:

    /* save r0 */
    push    {%r0}
    
    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosThreadSporadicCreatePriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosThreadSporadicCreateUser
   
    /* syscall done in privilege mode */
pharosThreadSporadicCreatePriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSThreadSporadicCreate

pharosThreadSporadicCreateUser:

    /* restore r0 */
    pop     {%r0}

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_THREAD_SPORADIC_CREATE

    /* return to caller */
    bx      %lr

.endfunc



.func pharosThreadStart
pharosThreadStart:

    /* save r0 */
    push    {%r0}
    
    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosThreadStartPriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosThreadStartUser
   
    /* syscall done in privilege mode */
pharosThreadStartPriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSThreadStart

pharosThreadStartUser:

    /* restore r0 */
    pop     {%r0}

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_THREAD_START

    /* return to caller */
    bx      %lr

.endfunc


.func pharosThreadStartDelay
pharosThreadStartDelay:

    /* save r0 */
    push    {%r0}
    
    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosThreadStartDelayPriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosThreadStartDelayUser
   
    /* syscall done in privilege mode */
pharosThreadStartDelayPriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSThreadStartDelay

pharosThreadStartDelayUser:

    /* restore r0 */
    pop     {%r0}

    /*  call the supervisor function */
    svc    PHAROS_SYSCALL_THREAD_START_DELAY

    /* return to caller */
    bx      %lr

.endfunc


.func pharosThreadGetIdSelf
pharosThreadGetIdSelf:

    /* save r0 */
    push    {%r0}
    
    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosThreadGetIdSelfPriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosThreadGetIdSelfUser
   
    /* syscall done in privilege mode */
pharosThreadGetIdSelfPriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSThreadGetIdSelf

pharosThreadGetIdSelfUser:

    /* restore r0 */
    pop     {%r0}

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_THREAD_GET_ID_SELF

    /* return to caller */
    bx      %lr

.endfunc


.func pharosThreadGetId
pharosThreadGetId:

    /* save r0 */
    push    {%r0}
    
    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosThreadGetIdPriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosThreadGetIdUser
   
    /* syscall done in privilege mode */
pharosThreadGetIdPriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSThreadGetId

pharosThreadGetIdUser:

    /* restore r0 */
    pop     {%r0}

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_THREAD_GET_ID

    /* return to caller */
    bx      %lr

.endfunc


.func pharosThreadGetStatus
pharosThreadGetStatus:

    /* save r0 */
    push    {%r0}
    
    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosThreadGetStatusPriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosThreadGetStatusUser
   
    /* syscall done in privilege mode */
pharosThreadGetStatusPriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSThreadGetStatus

pharosThreadGetStatusUser:

    /* restore r0 */
    pop     {%r0}

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_THREAD_GET_STATUS

    /* return to caller */
    bx      %lr

.endfunc


.func pharosThreadSetPriority
pharosThreadSetPriority:

    /* save r0 */
    push    {%r0}
    
    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosThreadSetPriorityPriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosThreadSetPriorityUser
   
    /* syscall done in privilege mode */
pharosThreadSetPriorityPriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSThreadSetPriority

pharosThreadSetPriorityUser:

    /* restore r0 */
    pop     {%r0}

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_THREAD_SET_PRIORITY

    /* return to caller */
    bx      %lr

.endfunc


.func pharosThreadStop
pharosThreadStop:

    /* save r0 */
    push    {%r0}
    
    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosThreadStopPriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosThreadStopUser
   
    /* syscall done in privilege mode */
pharosThreadStopPriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSThreadStop

pharosThreadStopUser:

    /* restore r0 */
    pop     {%r0}

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_THREAD_STOP

    /* return to caller */
    bx      %lr

.endfunc


.func pharosThreadStopSelf
pharosThreadStopSelf:

    /* save r0 */
    push    {%r0}
    
    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosThreadStopSelfPriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosThreadStopSelfUser
   
    /* syscall done in privilege mode */
pharosThreadStopSelfPriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSThreadStopSelf

pharosThreadStopSelfUser:

    /* restore r0 */
    pop     {%r0}

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_THREAD_STOP_SELF

    /* return to caller */
    bx      %lr

.endfunc


.func pharosThreadRestart
pharosThreadRestart:

    /* save r0 */
    push    {%r0}
    
    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosThreadRestartPriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosThreadRestartUser
   
    /* syscall done in privilege mode */
pharosThreadRestartPriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSThreadRestart

pharosThreadRestartUser:

    /* restore r0 */
    pop     {%r0}

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_THREAD_RESTART

    /* return to caller */
    bx      %lr

.endfunc


.func pharosPartitionRestart
pharosPartitionRestart:

    /* save r0 */
    push    {%r0}
    
    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosPartitionRestartPriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosPartitionRestartUser
   
    /* syscall done in privilege mode */
pharosPartitionRestartPriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSPartitionRestart

pharosPartitionRestartUser:

    /* restore r0 */
    pop     {%r0}

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_PARTITION_RESTART

    /* return to caller */
    bx      %lr

.endfunc




.func pharosThreadStack
pharosThreadStack:

    /* save r0 */
    push    {%r0}
    
    /* get the current executing exception */
    mrs     %r0 , IPSR

    /* if there is no current executing exception */
    cmp     %r0 , #0

    /* then we are in user mode then call the syscall */
    bne     pharosThreadStackPriv

    /* get the CONTROL register to r0 */
    mrs     %r0 , CONTROL

     /* clear all the bits except for the privilege */
    and     %r0 , %r0, #(1 << CONTROL_PRIV_BIT)

    /* check if we are in the privileged mode */
    cmp     %r0 , #CONTROL_PRIV_NON_PRIV

    /* if we are in user mode then call the syscall */
    beq     pharosThreadStackUser
   
    /* syscall done in privilege mode */
pharosThreadStackPriv:
    
    /* restore r0 */
    pop     {%r0}

    /* go directly to the syscall. Note that the lx has not been changed so when this syscall leaves, it will leave directly to the caller of this function */
    b       pharosSThreadStack

pharosThreadStackUser:

    /* restore r0 */
    pop     {%r0}

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_THREAD_STACK

    /* return to caller */
    bx      %lr

.endfunc
