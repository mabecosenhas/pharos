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


#include <pharos/kernel/core/multithreading.h>
#include <pharos/kernel/thread/thread.h>
#include <pharos/kernel/clock/clock.h>
#include <pharos/kernel/partition/partition.h>
#include <pharos/kernel/thread/thread.h>
#include <pharos/hal/boardApi.h>
#include <pharos/system.h>
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/kernel/thread/aperiodicthread.h>
#include <pharos/kernel/core/coreinlines.h>
#include <pharos/kernel/scheduler/schedulerinline.h>


void pharosIInitializeMultitasking(ptrCore core)
{
    /* get the highest priority ready thread */
    ptrThreadInfo highest = pharosISchedGetHighestReady(pharosICore()->runningFlatScheduler);


    /* update the running thread */
    pharosISchedUpdateRunningThread(highest);

    /* update that there is no need for context switch (we are already doing it) */
    pharosIThreadContextSwitchNeeded(FALSE);

    /* call the application initialization before start multi-threading*/
    pharosApplicationInitialization();

    /* set the WCET as not active on this core */
    pharosICore()->wcetIsActive = FALSE;

    /* if the first thread has a WCET */
    if(highest->wcet != PHAROS_WCET_UNLIMITED)
    {
        /* activate the WCET timer on this core */
        pharosICore()->wcetIsActive = TRUE;

        /* update the last dispatch instant of the heir thread */
        highest->lastDispatchTime = pharosSClockGetSinceBoot();

        /* set the clock interrupt when the WCET expires */
        pharosCpuWcetClockFire(highest->wcet);
    }

    /* set the nominal state */
    pharosICoreSetState(core , PHAROS_CORE_STATE_NOMINAL);

    /* initialize the clock to run in nominal state 
     * (calls pharosITimeTickIsr instead of pharosITimeTickIsrInitializing) */
    pharosCpuClockReset();

    /* perform a CPU context switch to the highest priority ready thread */
    pharosCpuRestoreHeir(highest);
}
