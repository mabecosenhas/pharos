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


#include <pharos/bulk.h>
#include <pharos/channel.h>
#include <pharos/clock.h>
#include <pharos/error.h>
#include <pharos/event.h>
#include <pharos/queue.h>
#include <pharos/resource.h>
#include <pharos/semaphore.h>
#include <pharos/thread.h>
#include <pharos/timer.h>
#include <pharos/future.h>
#include <pharos/kernel/core/syscall.h>
#include <pharos/kernel/core/core.h>
#include <pharos/hal/boardApi.h>
#include <pharos/kernel/thread/aperiodicthread.h>
#include <pharos/kernel/channel/channelbulk.h>
#include <pharos/kernel/channel/channelbulkkernel.h>


const void * pharosVSystemCalls[] = {

    /* 0 */
    NULL ,

    /* 1 */
    pharosSBulkGet ,

    /* 2 */
    pharosSBulkGive ,

    /* 3 */
    pharosSChannelSend ,

    /* 4 */
    pharosSChannelReceiveLocal ,

    /* 5 */
    pharosSChannelBulkQueueGet ,

    /* 6 */
    NULL ,

    /* 7 */
    pharosSChannelBulkFree ,

    /* 8 */
    pharosSClockGetDate ,

    /* 9 */
    pharosSClockSetDate ,

    /* 10 */
    pharosSClockGetTicks ,

    /* 11 */
    pharosSClockGetSinceBoot ,

    /* 12 */
    pharosSFatalErrorAnnounce ,

    /* 13 */
    pharosSErrorAnnounce ,

    /* 14 */
    pharosSEventSend ,

    /* 15 */
    pharosSEventReceive ,

    /* 16 */
    pharosSLwQueueSend ,

    /* 17 */
    pharosSLwQueueReceive ,

    /* 18 */
    pharosSLwQueueReceiveTimeout ,

    /* 19 */
    pharosSHwQueueSend ,

    /* 20 */
    pharosSHwQueueReceive ,

    /* 21 */
    pharosSResourceAccess ,

    /* 22 */
    pharosSSemCeilObtain ,

    /* 23 */
    pharosSSemCeilRelease ,

    /* 24 */
    pharosSSemMutexObtain ,

    /* 25 */
    pharosSSemMutexObtainTimeout ,

    /* 26 */
    pharosSSemMutexRelease ,

    /* 27 */
    pharosSSemCountObtain ,

    /* 28 */
    pharosSSemCountObtainTimeout ,

    /* 29 */
    pharosSSemCountRelease ,

    /* 30 */
    pharosSShutdown ,

    /* 31 */
    pharosSWait ,

    /* 32 */
    pharosSThreadSuspend ,

    /* 33 */
    pharosSThreadSuspendSelf ,

    /* 34 */
    pharosSThreadResume ,

    /* 35 */
    pharosSTimerStart ,

    /* 36 */
    pharosSTimerStop ,

    /* 37 */
    pharosSRestart ,

    /* 38 */
    pharosSInterruptInstall ,

    /* 39 */
    pharosSInterruptRemove ,

    /* 40 */
    pharosSCriticalSectionExecute ,

    /* 41 */
    pharosSIpCall ,

    /* 42 */
    outputSChar ,

    /* 43 */
    pharosSFutureGet ,

    /* 44 */
    pharosSFutureFree ,

    /* 45 */
    pharosSFutureWait ,

    /* 46 */
    pharosSFutureReady ,

    /* 47 */
    pharosSFutureQueueAdd ,

    /* 48 */
    pharosSFutureQueueRemove ,

    /* 49 */
    pharosSFutureQueueWait ,

    /* 50 */
    pharosSMalloc ,

    /* 51 */
    pharosFreeS ,

    /* 52 */
    pharosSThreadAperiodicCreate ,

    /* 53 */
    pharosSThreadPeriodicCreate ,

    /* 54 */
    pharosSThreadSporadicCreate ,

    /* 55 */
    pharosSThreadStart ,

    /* 56 */
    pharosSThreadStartDelay ,

    /* 57 */
    pharosSThreadGetIdSelf ,

    /* 58 */
    pharosSThreadGetId ,

    /* 59 */
    pharosSThreadGetStatus ,

    /* 60 */
    pharosSThreadSetPriority ,

    /* 61 */
    pharosSThreadStop ,

    /* 62 */
    pharosSThreadStopSelf ,

    /* 63 */
    pharosSThreadRestart ,

    /* 64 */
    pharosSPartitionRestart ,

    /* 65 */
    pharosSThreadStack ,

    /* 66 */
    pharosSChannelReceive ,

    /* 67 */
    pharosISporadicThreadEnded ,

    /* 68 */
    pharosIPeriodicThreadEnded ,

    /* 69 */
    pharosIAperiodicThreadEnded ,

    /* 70 */
    pharosCpuResourceFilterEnded ,

    /* 71 */
    pharosCpuResourceAccessEnded ,

    /* 72 */
    pharosCpuHwQueueFilterEnded ,

    /* 73 */
    pharosCpuChannelFilterSendEnded ,

    /* 74 */
    pharosCpuChannelFilterReceiveEnded ,

    /* 75 */
    pharosCpuCriticalSectionEnded ,

    /* 76 */
    pharosCpuIpCallFilterEnded ,

    /* 77 */
    pharosCpuIpCallIoEnded ,

    /* 78 */
    pharosCpuIpCallOEnded ,

};
