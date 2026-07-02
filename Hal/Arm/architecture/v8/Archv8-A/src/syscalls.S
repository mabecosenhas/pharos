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


.func outputChar
outputChar:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_DEBUG_PRINT_CHAR

    /* return to caller */
    ret

.endfunc


/**
 * PharosWaitResult pharosWait(ClockTick clockTicks)
 */
.func pharosWait
pharosWait:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_WAIT

    /* return to caller */
    ret

.endfunc


/**
 * PharosBulkGetResult pharosBulkGet(BulkQueueNumber number , ptrBulk *bulkDestination , ptrBuffer buffer)
 */
.func pharosBulkGet
pharosBulkGet:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_BULK_GET

    /* return to caller */
    ret

.endfunc


/**
 * PharosBulkGiveResult pharosBulkGive(ptrBulk bulk)
 */
.func pharosBulkGive
pharosBulkGive:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_BULK_GIVE

    /* return to caller */
    ret

.endfunc


.func pharosShutdown
pharosShutdown:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_SHUTDOWN

    /* return to caller */
    ret

.endfunc


/**
 * PharosChannelBulkQueueGetResult pharosChannelBulkQueueGet(ChannelBulkQueueNumber number , ptrChannelBulkKernel *bulk , ptrBuffer buffer);
 */
.func pharosChannelBulkQueueGet
pharosChannelBulkQueueGet:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_CHANNEL_BULK_QUEUE_GET

    /* return to caller */
    ret

.endfunc


/**
 * PharosChannelBulkFreeResult pharosChannelBulkFree(ChannelNumber number);
 */
.func pharosChannelBulkFree
pharosChannelBulkFree:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_CHANNEL_FREE_BULK

    /* return to caller */
    ret

.endfunc


/**
 * PharosChannelReceiveResult pharosChannelReceiveLocal(ChannelNumber number , ptrChannelBulkKernel *bulk , 
 *                                                      ptrBuffer bulkMessage , ptrBuffer dataMessage , 
 *                                                      ptrPharosPriority priority , Timeout timeout);
 */
.func pharosChannelReceiveLocal
pharosChannelReceiveLocal:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_CHANNEL_LOCAL_RECEIVE

    /* return to caller */
    ret

.endfunc


/**
 * pharosChannelReceiveR pharosChannelReceive(ptrChannelId id , ptrChannelBulkKernel *bulk , ptrBuffer bulkMessage ,
 *                                            ptrBuffer dataMessage , ptrPharosPriority priority , ptrFutureId future ,
 *                                            Timeout timeout);
 */
.func pharosChannelReceive
pharosChannelReceive:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_CHANNEL_RECEIVE

    /* return to caller */
    ret

.endfunc


/**
 * PharosChannelSendResult pharosChannelSend(ptrChannelId id , ptrChannelBulkKernel bulk , uint32_t offset , uint32_t size , PharosPriority priority);
 */
.func pharosChannelSend
pharosChannelSend:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_CHANNEL_SEND

    /* return to caller */
    ret

.endfunc
    

/**
 * PharosClockGetResult pharosClockGetDate(volatile ptrDate date);
 */
.func pharosClockGetDate
pharosClockGetDate:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_CLOCK_GET_DATE

    /* return to caller */
    ret

.endfunc
    


/**
 * PharosClockSetResult pharosClockSetDate(ptrDate date);
 */
.func pharosClockSetDate
pharosClockSetDate:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_CLOCK_SET_DATE

    /* return to caller */
    ret

.endfunc


.func pharosClockGetTicks
pharosClockGetTicks:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_CLOCK_GET_TICKS

    /* return to caller */
    ret

.endfunc

/**
 * uint64_t pharosClockGetSinceBoot();
 */
.func pharosClockGetSinceBoot
pharosClockGetSinceBoot:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_CLOCK_GET_SINCE_BOOT

    /* return to caller */
    ret

.endfunc
    
/**
 * void pharosFatalErrorAnnounce(PharosFatalError error , const char *file , const char *func , uint32_t line);
 */
.func pharosFatalErrorAnnounce
pharosFatalErrorAnnounce:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_FATAL_ERROR_ANNOUNCE

    /* return to caller */
    ret

.endfunc
    

/**
 * void pharosErrorAnnounce(PharosError error , uint8_t *interruptedInstruction);
 */
.func pharosErrorAnnounce
pharosErrorAnnounce:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_ERROR_ANNOUNCE

    /* return to caller */
    ret

.endfunc
    

/**
 * PharosEventSendResult pharosEventSend(ThreadId thread, Event event);
 */
.func pharosEventSend
pharosEventSend:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_EVENT_SEND

    /* return to caller */
    ret

.endfunc
    

/**
 * PharosEventReceiveResult pharosEventReceive(Timeout timeout, ptrEvent event);
 */
.func pharosEventReceive
pharosEventReceive:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_EVENT_RECEIVE

    /* return to caller */
    ret

.endfunc
    

/**
 * PharosLwQueueSendResult pharosLwQueueSend(LightWeightQueueNumber id , ptrBulk message , uint32_t size);
 */
.func pharosLwQueueSend
pharosLwQueueSend:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_LW_QUEUE_SEND

    /* return to caller */
    ret

.endfunc
    

/**
 * PharosLwQueueReceiveResult pharosLwQueueReceive(LightWeightQueueNumber id , ptrBulk *message , ptrBuffer buffer);
 */
.func pharosLwQueueReceive
pharosLwQueueReceive:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_LW_QUEUE_RECEIVE

    /* return to caller */
    ret

.endfunc
    

/**
 * PharosLwQueueReceiveTimeoutResult pharosLwQueueReceiveTimeout(LightWeightQueueNumber id , ptrBulk *message , ptrBuffer buffer , Timeout timeout);
 */
.func pharosLwQueueReceiveTimeout
pharosLwQueueReceiveTimeout:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_LW_QUEUE_RECEIVE_TIMEOUT

    /* return to caller */
    ret

.endfunc


/**
 * PharosHwQueueSendR pharosHwQueueSend(PartitionNumber partition , HwQueueNumber number , const uint8_t *buffer , uint32_t size , PharosPriority priority , ptrFutureId future, Timeout timeout);
 */
.func pharosHwQueueSend
pharosHwQueueSend:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_HW_QUEUE_SEND

    /* return to caller */
    ret

.endfunc


/**
 * PharosHwReceiveResult pharosHwQueueReceive(HwQueueNumber number , uint8_t *buffer , uint32_t *size , ptrPharosPriority priority , Timeout timeout);
 */
.func pharosHwQueueReceive
pharosHwQueueReceive:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_HW_QUEUE_RECEIVE

    /* return to caller */
    ret

.endfunc


/**
 * PharosResourceAccessResult pharosResourceAccess(ptrResourceId id , uint32_t input , uint32_t *output , Timeout timeout);
 */
.func pharosResourceAccess
pharosResourceAccess:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_RESOURCE_ACCESS

    /* return to caller */
    ret

.endfunc



/**
 * PharosSemCeilObtainResult pharosSemCeilObtain(SemaphoreCeilingNumber number, Timeout timeout);
 */
.func pharosSemCeilObtain
pharosSemCeilObtain:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_SEMAPHORE_CEILING_OBTAIN

    /* return to caller */
    ret

.endfunc


/**
 * PharosSemCeilReleaseResult pharosSemCeilRelease(SemaphoreCeilingNumber id);
 */
.func pharosSemCeilRelease
pharosSemCeilRelease:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_SEMAPHORE_CEILING_RELEASE

    /* return to caller */
    ret

.endfunc


/**
 * PharosSemMutexObtainResult pharosSemMutexObtain(SemaphoreMutexNumber id);
 */
.func pharosSemMutexObtain
pharosSemMutexObtain:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_SEMAPHORE_MUTEX_OBTAIN

    /* return to caller */
    ret

.endfunc


/**
 * PharosSemMutexObtainTimeoutResult pharosSemMutexObtainTimeout(SemaphoreMutexNumber id, Timeout timeout);
 */
.func pharosSemMutexObtainTimeout
pharosSemMutexObtainTimeout:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_SEMAPHORE_MUTEX_OBTAIN_TIMEOUT

    /* return to caller */
    ret

.endfunc


/**
 * PharosSemMutexReleaseResult pharosSemMutexRelease(SemaphoreMutexNumber id);
 */
.func pharosSemMutexRelease
pharosSemMutexRelease:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_SEMAPHORE_MUTEX_RELEASE

    /* return to caller */
    ret

.endfunc


/**
 * PharosSemCountObtainResult pharosSemCountObtain(SemaphoreCountingNumber id);
 */
.func pharosSemCountObtain
pharosSemCountObtain:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_SEMAPHORE_COUNTING_OBTAIN

    /* return to caller */
    ret

.endfunc


/**
 * PharosSemCountObtainTimeoutResult pharosSemCountObtainTimeout(SemaphoreCountingNumber id, Timeout timeout);
 */
.func pharosSemCountObtainTimeout
pharosSemCountObtainTimeout:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_SEMAPHORE_COUNTING_OBTAIN_TIMEOUT

    /* return to caller */
    ret

.endfunc


/**
 * PharosSemCountReleaseResult pharosSemCountRelease(SemaphoreCountingNumber id);
 */
.func pharosSemCountRelease
pharosSemCountRelease:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_SEMAPHORE_COUNTING_RELEASE

    /* return to caller */
    ret

.endfunc


/**
 * PharosThreadSuspendResult pharosThreadSuspend(ThreadId id);
 */
.func pharosThreadSuspend
pharosThreadSuspend:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_THREAD_SUSPEND

    /* return to caller */
    ret

.endfunc


/**
 * PharosThreadSuspendSelfResult pharosThreadSuspendSelf();
 */
.func pharosThreadSuspendSelf
pharosThreadSuspendSelf:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_THREAD_SUSPEND_SELF

    /* return to caller */
    ret

.endfunc


/**
 * PharosThreadResumeResult pharosThreadResume(ThreadId id);
 */
.func pharosThreadResume
pharosThreadResume:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_THREAD_RESUME

    /* return to caller */
    ret

.endfunc


/**
 * PharosTimerStartResult pharosTimerStart(TimerNumber number , uint64_t argument);
 */
.func pharosTimerStart
pharosTimerStart:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_TIMER_START

    /* return to caller */
    ret

.endfunc

   
/**
 * PharosTimerStop pharosTimerStop(TimerNumber timerId);
 */
.func pharosTimerStop
pharosTimerStop:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_TIMER_STOP

    /* return to caller */
    ret

.endfunc


/**
 * PharosInterruptCatchResult pharosInterruptInstall(uint32_t index);
 */
.func pharosInterruptInstall
pharosInterruptInstall:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_INTERRUPT_INSTALL

    /* return to caller */
    ret

.endfunc


/**
 * PharosInterruptCatchResult pharosInterruptRemove(uint32_t index);
 */
.func pharosInterruptRemove
pharosInterruptRemove:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_INTERRUPT_REMOVE

    /* return to caller */
    ret

.endfunc


/**
 * PharosCriticalSectionExecuteResult pharosCriticalSectionExecute(uint32_t index , uint32_t arg1 , uint32_t arg2 , uint32_t arg3 , uint32_t arg4);
 */
.func pharosCriticalSectionExecute
pharosCriticalSectionExecute:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_CRITICAL_SECTION_EXECUTE

    /* return to caller */
    ret

.endfunc


.func pharosFutureGet
pharosFutureGet:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_FUTURE_GET

    /* return to caller */
    ret

.endfunc


.func pharosFutureFree
pharosFutureFree:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_FUTURE_FREE

    /* return to caller */
    ret

.endfunc


.func pharosFutureWait
pharosFutureWait:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_FUTURE_WAIT

    /* return to caller */
    ret

.endfunc


.func pharosFutureReady
pharosFutureReady:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_FUTURE_READY

    /* return to caller */
    ret

.endfunc


.func pharosFutureQueueAdd
pharosFutureQueueAdd:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_FUTURE_QUEUE_ADD

    /* return to caller */
    ret

.endfunc


.func pharosFutureQueueRemove
pharosFutureQueueRemove:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_FUTURE_QUEUE_REMOVE

    /* return to caller */
    ret

.endfunc


.func pharosFutureQueueWait
pharosFutureQueueWait:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_FUTURE_QUEUE_WAIT

    /* return to caller */
    ret

.endfunc


.func pharosMalloc
pharosMalloc:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_MALLOC

    /* return to caller */
    ret

.endfunc


.func pharosFree
pharosFree:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_FREE

    /* return to caller */
    ret

.endfunc


.func pharosThreadAperiodicCreate
pharosThreadAperiodicCreate:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_THREAD_APERIODIC_CREATE

    /* return to caller */
    ret

.endfunc


.func pharosThreadPeriodicCreate
pharosThreadPeriodicCreate:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_THREAD_PERIODIC_CREATE

    /* return to caller */
    ret

.endfunc


.func pharosThreadSporadicCreate
pharosThreadSporadicCreate:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_THREAD_SPORADIC_CREATE

    /* return to caller */
    ret

.endfunc


.func pharosThreadStart
pharosThreadStart:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_THREAD_START

    /* return to caller */
    ret

.endfunc


.func pharosThreadStartDelay
pharosThreadStartDelay:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_THREAD_START_DELAY

    /* return to caller */
    ret

.endfunc


.func pharosThreadGetIdSelf
pharosThreadGetIdSelf:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_THREAD_GET_ID_SELF

    /* return to caller */
    ret

.endfunc


.func pharosThreadGetId
pharosThreadGetId:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_THREAD_GET_ID

    /* return to caller */
    ret

.endfunc


.func pharosThreadGetStatus
pharosThreadGetStatus:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_THREAD_GET_STATUS

    /* return to caller */
    ret

.endfunc


.func pharosThreadSetPriority
pharosThreadSetPriority:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_THREAD_SET_PRIORITY

    /* return to caller */
    ret

.endfunc


.func pharosThreadStop
pharosThreadStop:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_THREAD_STOP

    /* return to caller */
    ret

.endfunc


.func pharosThreadStopSelf
pharosThreadStopSelf:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_THREAD_STOP_SELF

    /* return to caller */
    ret

.endfunc


.func pharosThreadRestart
pharosThreadRestart:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_THREAD_RESTART

    /* return to caller */
    ret

.endfunc


.func pharosPartitionRestart
pharosPartitionRestart:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_PARTITION_RESTART

    /* return to caller */
    ret

.endfunc
 

.func pharosThreadStack
pharosThreadStack:

    /*  call the supervisor function */
    svc     PHAROS_SYSCALL_THREAD_STACK

    /* return to caller */
    ret

.endfunc
 
