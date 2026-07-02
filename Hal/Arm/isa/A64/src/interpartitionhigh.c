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


#include <pharos/kernel/thread/thread.h>
#include <pharos/hal/interpartition.h>

extern uint8_t *pharosCpuInterParititionContextExit;


void pharosCpuThreadFinishCall(ptrThreadInfo thread , ptrPharosCpuInterPartitionContext context)
{
    /* restore the thread running partition to its original partition */
    thread->runningPartition = thread->partition;

    /* copy the context to the base context. When the thread executes again it will go to the context saved */
    thread->cpuContext.baseContext = context->ipBaseContext;

    /* save the original x30 (return address) */
    thread->cpuContext.x30Ipc = thread->cpuContext.baseContext.x30;

    /* switch the return address to the exit */
    thread->cpuContext.baseContext.x30 = (uintptr_t) & pharosCpuInterParititionContextExit;
}
