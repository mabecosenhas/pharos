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


#ifndef PHAROS_CORECONF_H
#define PHAROS_CORECONF_H

#include <pharos/kernel/thread/declarations.h>
#include <pharos/kernel/time/declarations.h>

struct PharosThreadCore
{
    /* no need for multi-core information in a single-core CPU */
};

struct PharosChannelBulkCore
{
    /* no need for multi-core information in a single-core CPU */
};

struct PharosHwQueueMessageCore
{
    /* no need for multi-core information in a single-core CPU */
};

struct PharosFutureCore
{
    /* no need for multi-core information in a single-core CPU */
};

struct PharosPartitionCore
{
    /* no need for multi-core information in a single-core CPU */
};

struct PharosCoreConfiguration
{
    /* no need for multi-core information in a single-core CPU */
};

struct PharosFlatSchedulerCore
{
    /* no need for multi-core information in a single-core CPU */
};


/**
 * Pointer to the running thread
 */
extern volatile ptrThreadInfo pharosVRunningThread;


/**
 * Flag indicating if context switch is necessary
 */
extern volatile uint32_t pharosVNeedsThreadContextSwitch;


/**
 * Interrupt nesting level
 */
extern uint32_t pharosVInterruptNestingLevel;


/**
 * Thread dispatch level
 */
extern volatile uint32_t pharosVDispatchLevel;


/**
 * number of clock ticks since the system was reboot
 *
 * Protection: against interrupts
 */
extern volatile ClockTick pharosVClockTicks;


/* PHAROS_CORECONF_H */
#endif 
