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


.func outputChar
outputChar:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_DEBUG_PRINT_CHAR << 3

    /*  call the supervisor function */
    ecall    

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     outputSChar

    la      a2, outputSChar

1:  auipc   a4 , %pcrel_hi(outputSChar)
    jalr    x0 , a4 , %pcrel_lo(1b)

    jalr    x0 , a2, 0

.endfunc


/**
 * PharosWaitResult pharosWait(ClockTick clockTicks)
 */
.func pharosWait
pharosWait:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_WAIT << 3

    /*  call the supervisor function */
    ecall     
 
    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSWait

.endfunc


/**
 * PharosBulkGetResult pharosBulkGet(BulkQueueNumber number , ptrBulk *bulkDestination , ptrBuffer buffer)
 */
.func pharosBulkGet
pharosBulkGet:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_BULK_GET << 3

    /*  call the supervisor function */
    ecall     
 
    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSBulkGet

.endfunc


/**
 * PharosBulkGiveResult pharosBulkGive(ptrBulk bulk)
 */
.func pharosBulkGive
pharosBulkGive:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_BULK_GIVE << 3

    /*  call the supervisor function */
    ecall     

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSBulkGive

.endfunc


.func pharosShutdown
pharosShutdown:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_SHUTDOWN << 3

    /*  call the supervisor function */
    ecall     

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSShutdown

.endfunc


/**
 * PharosChannelBulkQueueGetResult pharosChannelBulkQueueGet(ChannelBulkQueueNumber number , ptrChannelBulkKernel *bulk , ptrBuffer buffer);
 */
.func pharosChannelBulkQueueGet
pharosChannelBulkQueueGet:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_CHANNEL_BULK_QUEUE_GET << 3

    /*  call the supervisor function */
    ecall     

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSChannelBulkQueueGet

.endfunc


/**
 * PharosChannelBulkFreeResult pharosChannelBulkFree(ChannelNumber number);
 */
.func pharosChannelBulkFree
pharosChannelBulkFree:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_CHANNEL_FREE_BULK << 3

    /*  call the supervisor function */
    ecall     

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSChannelBulkFree

.endfunc


/**
 * pharosChannelReceiveLocalR pharosChannelReceiveLocal(ChannelNumber id , ptrChannelBulkKernel *bulk , ptrBuffer bulkMessage ,
 *                                                      ptrBuffer dataMessage , ptrPharosPriority priority ,
 *                                                      ptrFutureId future , Timeout timeout);
 */
.func pharosChannelReceiveLocal
pharosChannelReceiveLocal:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_CHANNEL_LOCAL_RECEIVE << 3

    /*  call the supervisor function */
    ecall     

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSChannelReceiveLocal

.endfunc


/**
 * pharosChannelReceiveR pharosChannelReceive(ptrChannelId id , ptrChannelBulkKernel *bulk , ptrBuffer bulkMessage ,
 *                                            ptrBuffer dataMessage , ptrPharosPriority priority , ptrFutureId future ,
 *                                            Timeout timeout);
 */
.func pharosChannelReceive
pharosChannelReceive:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_CHANNEL_RECEIVE << 3

    /*  call the supervisor function */
    ecall     

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSChannelReceive

.endfunc


/**
 * PharosChannelSendR pharosChannelSend(ptrChannelId id , ptrChannelBulk bulk , PharosPriority priority ,
 *                                      ptrFutureId future)
 */
.func pharosChannelSend
pharosChannelSend:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_CHANNEL_SEND << 3

    /*  call the supervisor function */
    ecall     

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSChannelSend

.endfunc
    

/**
 * PharosClockGetResult pharosClockGetDate(volatile ptrDate date);
 */
.func pharosClockGetDate
pharosClockGetDate:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_CLOCK_GET_DATE << 3

    /*  call the supervisor function */
    ecall     

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSClockGetDate

.endfunc
    


/**
 * PharosClockSetResult pharosClockSetDate(ptrDate date);
 */
.func pharosClockSetDate
pharosClockSetDate:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_CLOCK_SET_DATE << 3

    /*  call the supervisor function */
    ecall     

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSClockSetDate

.endfunc


.func pharosClockGetTicks
pharosClockGetTicks:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_CLOCK_GET_TICKS << 3

    /*  call the supervisor function */
    ecall     
 
    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSClockGetTicks

.endfunc

/**
 * uint64_t pharosClockGetSinceBoot();
 */
.func pharosClockGetSinceBoot
pharosClockGetSinceBoot:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_CLOCK_GET_SINCE_BOOT << 3

    /*  call the supervisor function */
    ecall     

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSClockGetSinceBoot

.endfunc
    
/**
 * void pharosFatalErrorAnnounce(PharosFatalError error , const char *file , const char *func , uint32_t line);
 */
.func pharosFatalErrorAnnounce
pharosFatalErrorAnnounce:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_FATAL_ERROR_ANNOUNCE << 3

    /*  call the supervisor function */
    ecall     

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSFatalErrorAnnounce

.endfunc
    

/**
 * void pharosErrorAnnounce(PharosError error , uint8_t *interruptedInstruction);
 */
.func pharosErrorAnnounce
pharosErrorAnnounce:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_ERROR_ANNOUNCE << 3

    /*  call the supervisor function */
    ecall     

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSErrorAnnounce

.endfunc
    

/**
 * PharosEventSendResult pharosEventSend(ThreadId thread, Event event);
 */
.func pharosEventSend
pharosEventSend:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_EVENT_SEND << 3

    /*  call the supervisor function */
    ecall     

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSEventSend

.endfunc
    

/**
 * PharosEventReceiveResult pharosEventReceive(Timeout timeout, ptrEvent event);
 */
.func pharosEventReceive
pharosEventReceive:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_EVENT_RECEIVE << 3

    /*  call the supervisor function */
    ecall     

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSEventReceive

.endfunc
    

/**
 * PharosLwQueueSendResult pharosLwQueueSend(LightWeightQueueNumber id , ptrBulk message , uint32_t size);
 */
.func pharosLwQueueSend
pharosLwQueueSend:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_LW_QUEUE_SEND << 3

    /*  call the supervisor function */
    ecall     

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSLwQueueSend

.endfunc
    

/**
 * PharosLwQueueReceiveResult pharosLwQueueReceive(LightWeightQueueNumber id , ptrBulk *message , ptrBuffer buffer);
 */
.func pharosLwQueueReceive
pharosLwQueueReceive:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_LW_QUEUE_RECEIVE << 3

    /*  call the supervisor function */
    ecall     

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSLwQueueReceive

.endfunc
    

/**
 * PharosLwQueueReceiveTimeoutResult pharosLwQueueReceiveTimeout(LightWeightQueueNumber id , ptrBulk *message , ptrBuffer buffer , Timeout timeout);
 */
.func pharosLwQueueReceiveTimeout
pharosLwQueueReceiveTimeout:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_LW_QUEUE_RECEIVE_TIMEOUT << 3

    /*  call the supervisor function */
    ecall     

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSLwQueueReceiveTimeout

.endfunc


/**
 * PharosHwSendResult pharosHwQueueSend(ptrHwQueueId id , const uint8_t *buffer , uint32_t size , PharosPriority priority);
 */
.func pharosHwQueueSend
pharosHwQueueSend:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_HW_QUEUE_SEND << 3

    /*  call the supervisor function */
    ecall     

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSHwQueueSend

.endfunc


/**
 * PharosHwReceiveResult pharosHwQueueReceive(HwQueueNumber number , uint8_t *buffer , uint32_t *size , ptrPharosPriority priority , Timeout timeout);
 */
.func pharosHwQueueReceive
pharosHwQueueReceive:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_HW_QUEUE_RECEIVE << 3

    /*  call the supervisor function */
    ecall     

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSHwQueueReceive

.endfunc


/**
 * PharosResourceAccessResult pharosResourceAccess(ptrResourceId id , uint32_t input , uint32_t *output , Timeout timeout);
 */
.func pharosResourceAccess
pharosResourceAccess:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_RESOURCE_ACCESS << 3

    /*  call the supervisor function */
    ecall     

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSResourceAccess

.endfunc



/**
 * PharosSemCeilObtainResult pharosSemCeilObtain(SemaphoreCeilingNumber number, Timeout timeout);
 */
.func pharosSemCeilObtain
pharosSemCeilObtain:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_SEMAPHORE_CEILING_OBTAIN << 3

    /*  call the supervisor function */
    ecall     

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSSemCeilObtain

.endfunc


/**
 * PharosSemCeilReleaseResult pharosSemCeilRelease(SemaphoreCeilingNumber id);
 */
.func pharosSemCeilRelease
pharosSemCeilRelease:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_SEMAPHORE_CEILING_RELEASE << 3

    /*  call the supervisor function */
    ecall     

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSSemCeilRelease

.endfunc


/**
 * PharosSemMutexObtainResult pharosSemMutexObtain(SemaphoreMutexNumber id);
 */
.func pharosSemMutexObtain
pharosSemMutexObtain:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_SEMAPHORE_MUTEX_OBTAIN << 3

    /*  call the supervisor function */
    ecall     

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSSemMutexObtain

.endfunc


/**
 * PharosSemMutexObtainTimeoutResult pharosSemMutexObtainTimeout(SemaphoreMutexNumber id, Timeout timeout);
 */
.func pharosSemMutexObtainTimeout
pharosSemMutexObtainTimeout:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_SEMAPHORE_MUTEX_OBTAIN_TIMEOUT << 3

    /*  call the supervisor function */
    ecall     

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSSemMutexObtainTimeout

.endfunc


/**
 * PharosSemMutexReleaseResult pharosSemMutexRelease(SemaphoreMutexNumber id);
 */
.func pharosSemMutexRelease
pharosSemMutexRelease:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_SEMAPHORE_MUTEX_RELEASE << 3

    /*  call the supervisor function */
    ecall    

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSSemMutexRelease

.endfunc


/**
 * PharosSemCountObtainResult pharosSemCountObtain(SemaphoreCountingNumber id);
 */
.func pharosSemCountObtain
pharosSemCountObtain:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_SEMAPHORE_COUNTING_OBTAIN << 3

    /*  call the supervisor function */
    ecall     

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSSemCountObtain

.endfunc


/**
 * PharosSemCountObtainTimeoutResult pharosSemCountObtainTimeout(SemaphoreCountingNumber id, Timeout timeout);
 */
.func pharosSemCountObtainTimeout
pharosSemCountObtainTimeout:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_SEMAPHORE_COUNTING_OBTAIN_TIMEOUT << 3

    /*  call the supervisor function */
    ecall     

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSSemCountObtainTimeout

.endfunc


/**
 * PharosSemCountReleaseResult pharosSemCountRelease(SemaphoreCountingNumber id);
 */
.func pharosSemCountRelease
pharosSemCountRelease:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_SEMAPHORE_COUNTING_RELEASE << 3

    /*  call the supervisor function */
    ecall    

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSSemCountRelease

.endfunc


/**
 * PharosThreadSuspendResult pharosThreadSuspend(ThreadId id);
 */
.func pharosThreadSuspend
pharosThreadSuspend:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_THREAD_SUSPEND << 3

    /*  call the supervisor function */
    ecall     

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSThreadSuspend

.endfunc


/**
 * PharosThreadSuspendSelfResult pharosThreadSuspendSelf();
 */
.func pharosThreadSuspendSelf
pharosThreadSuspendSelf:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_THREAD_SUSPEND_SELF << 3

    /*  call the supervisor function */
    ecall     

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSThreadSuspendSelf

.endfunc


/**
 * PharosThreadResumeResult pharosThreadResume(ThreadId id);
 */
.func pharosThreadResume
pharosThreadResume:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_THREAD_RESUME << 3

    /*  call the supervisor function */
    ecall   

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSThreadResume

.endfunc


/**
 * PharosTimerStartResult pharosTimerStart(TimerNumber number , uint64_t argument);
 */
.func pharosTimerStart
pharosTimerStart:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_TIMER_START << 3

    /*  call the supervisor function */
    ecall   

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSTimerStart

.endfunc

   
/**
 * PharosTimerStop pharosTimerStop(TimerNumber timerId);
 */
.func pharosTimerStop
pharosTimerStop:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_TIMER_STOP << 3

    /*  call the supervisor function */
    ecall   

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSTimerStop

.endfunc


/**
 * PharosInterruptCatchResult pharosInterruptInstall(uint32_t index);
 */
.func pharosInterruptInstall
pharosInterruptInstall:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_INTERRUPT_INSTALL << 3

    /*  call the supervisor function */
    ecall   

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSInterruptInstall

.endfunc


/**
 * PharosInterruptCatchResult pharosInterruptRemove(uint32_t index);
 */
.func pharosInterruptRemove
pharosInterruptRemove:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_INTERRUPT_REMOVE << 3

    /*  call the supervisor function */
    ecall   

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSInterruptRemove

.endfunc


/**
 * PharosCriticalSectionExecuteResult pharosCriticalSectionExecute(uint32_t index , uint32_t arg1 , uint32_t arg2 , uint32_t arg3 , uint32_t arg4);
 */
.func pharosCriticalSectionExecute
pharosCriticalSectionExecute:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_CRITICAL_SECTION_EXECUTE << 3

    /*  call the supervisor function */
    ecall    

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSCriticalSectionExecute

.endfunc


.func pharosFutureGet
pharosFutureGet:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_FUTURE_GET << 3

    /*  call the supervisor function */
    ecall    

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSFutureGet

.endfunc


.func pharosFutureFree
pharosFutureFree:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_FUTURE_FREE << 3

    /*  call the supervisor function */
    ecall     

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSFutureFree

.endfunc


.func pharosFutureWait
pharosFutureWait:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_FUTURE_WAIT << 3

    /*  call the supervisor function */
    ecall    

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSFutureWait

.endfunc


.func pharosFutureReady
pharosFutureReady:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_FUTURE_READY << 3

    /*  call the supervisor function */
    ecall      

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSFutureReady

.endfunc


.func pharosFutureQueueAdd
pharosFutureQueueAdd:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_FUTURE_QUEUE_ADD << 3

    /*  call the supervisor function */
    ecall     

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSFutureQueueAdd

.endfunc


.func pharosFutureQueueRemove
pharosFutureQueueRemove:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_FUTURE_QUEUE_REMOVE << 3

    /*  call the supervisor function */
    ecall     

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSFutureQueueRemove

.endfunc


.func pharosFutureQueueWait
pharosFutureQueueWait:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_FUTURE_QUEUE_WAIT << 3

    /*  call the supervisor function */
    ecall     

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSFutureQueueWait

.endfunc


.func pharosMalloc
pharosMalloc:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_MALLOC << 3

    /*  call the supervisor function */
    ecall     

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSMalloc

.endfunc


.func pharosFree
pharosFree:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_FREE << 3

    /*  call the supervisor function */
    ecall     

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosFreeS

.endfunc


.func pharosThreadAperiodicCreate
pharosThreadAperiodicCreate:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_THREAD_APERIODIC_CREATE << 3

    /*  call the supervisor function */
    ecall     

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSThreadAperiodicCreate

.endfunc


.func pharosThreadPeriodicCreate
pharosThreadPeriodicCreate:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_THREAD_PERIODIC_CREATE << 3

    /*  call the supervisor function */
    ecall     

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSThreadPeriodicCreate

.endfunc


.func pharosThreadSporadicCreate
pharosThreadSporadicCreate:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_THREAD_SPORADIC_CREATE << 3

    /*  call the supervisor function */
    ecall     

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSThreadSporadicCreate

.endfunc


.func pharosThreadStart
pharosThreadStart:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_THREAD_START << 3

    /*  call the supervisor function */
    ecall     

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSThreadStart

.endfunc


.func pharosThreadStartDelay
pharosThreadStartDelay:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_THREAD_START_DELAY << 3

    /*  call the supervisor function */
    ecall     

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSThreadStartDelay

.endfunc
 

.func pharosThreadGetIdSelf
pharosThreadGetIdSelf:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_THREAD_GET_ID_SELF << 3

    /*  call the supervisor function */
    ecall     

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSThreadGetIdSelf

.endfunc


.func pharosThreadGetId
pharosThreadGetId:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_THREAD_GET_ID << 3

    /*  call the supervisor function */
    ecall     

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSThreadGetId

.endfunc


.func pharosThreadGetStatus
pharosThreadGetStatus:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_THREAD_GET_STATUS << 3

    /*  call the supervisor function */
    ecall     

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSThreadGetStatus

.endfunc


.func pharosThreadSetPriority
pharosThreadSetPriority:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_THREAD_SET_PRIORITY << 3

    /*  call the supervisor function */
    ecall     

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSThreadSetPriority

.endfunc
 

.func pharosThreadStop
pharosThreadStop:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_THREAD_STOP << 3

    /*  call the supervisor function */
    ecall     

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSThreadStop

.endfunc


.func pharosThreadStopSelf
pharosThreadStopSelf:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_THREAD_STOP_SELF << 3

    /*  call the supervisor function */
    ecall     

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSThreadStopSelf

.endfunc


.func pharosThreadRestart
pharosThreadRestart:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_THREAD_RESTART << 3

    /*  call the supervisor function */
    ecall     

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSThreadRestart

.endfunc


.func pharosPartitionRestart
pharosPartitionRestart:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_PARTITION_RESTART << 3

    /*  call the supervisor function */
    ecall     

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSPartitionRestart

.endfunc


.func pharosThreadStack
pharosThreadStack:

    /* load to the arguments last register with the syscall number to perform (will not be used since no syscall used 8 arguments) */ 
    li      gp , PHAROS_SYSCALL_THREAD_STACK << 3

    /*  call the supervisor function */
    ecall     

    /* if we are here then the call was made in machine mode, go directly to the function */
    c.j     pharosSThreadStack

.endfunc
