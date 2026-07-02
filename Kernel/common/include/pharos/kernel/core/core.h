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


#ifndef PHAROS_PHAROS_CORE_H
#define PHAROS_PHAROS_CORE_H


#include <pharos/kernel/core/declarations.h>
#include <pharos/kernel/partition/partition.h>
#include <pharos/kernel/timer/timer.h>
#include <pharos/hal/cpu/cpu.h>
#include <pharos/kernel/core/coreconf.h>
#include <pharos/kernel/interrupt/interrupttable.h>


/**
 * Compact the size of the I/O registers
 */
#pragma pack(push , __SIZEOF_POINTER__)

struct Core
{
    /**
     * Core configuration
     * 
     * @note application should not initialize this field
     */
    PharosCoreConfiguration rmp;

    /**
     * Configuration of the CPU core.
     * Specific to each CPU
     * 
     * @note application should initialize this field (if needed)
     */
    CpuCoreConfiguration cpu;

    /**
     * Interrupt table configuration for the CPU core
     * 
     * @note application should initialize this field
     */
    InterruptTable interrupts;

    /**
     * Partitions that run inside the CPU core.
     * 
     * @note application should initialize this field
     */
    PartitionTable partitions;

    /**
     * Configuration of the number of microseconds per each clock tick of this CPU core
     * This cannot be higher than 999.999 microseconds, that is, a clock tick cannot be higher than one second 
     * 
     * This is so to be able to quickly update the time of day without having to perform divisions inside the 
     * clock interrupt routine
     * 
     * @note in some CPUs (e.g. ARM Cortex-M family) the timer resolution is not big enough to even reach 1 second. For example,
     * the systick in ARMv7-M family only has a 24-bit counter with a 1 microsecond resolution. To avoid this limitation, Pharos
     * internally supports multiple ticks to a "real" clock tick. For example, if the desired clock tick is 500 ms, then Pharos will place
     * a clock tick of 50 ms and only when 10 "hardware clock ticks" occur will the "application clock tick" be updated.
     * 
     * @note application should initialize this field with the number of microseconds per clock tick
     */
    uint32_t microsecondsPerClockTick;

    /**
     * Scheduler definition for the running core
     * 
     * @note application should initialize this value with:
     *  - NULL for a flat scheduler
     *  - the pointer to the hierarchical scheduler if hierarchical scheduling is required
     */
    ptrScheduler scheduler;

    /**
     * Default flat scheduler. Used only if hierarchical scheduling is not required
     * 
     * @note application should not initialize this field
     */
    FlatScheduler flatScheduler;

    /**
     * The flat scheduler running at each instant on the running core
     * 
     * @note application should initialize this field if hierarchical scheduling is required
     */
    ptrFlatScheduler runningFlatScheduler;

    /**
     * Core number
     * 
     * @note application should not initialize this field
     */
    CoreNumber number;

    /**
     * Flag indicating if the wcet timer is active for the core
     * 
     * @note application should not initialize this field
     */
    bool wcetIsActive;

    /**
     * Threshold above which dynamic threads are not allowed to be dynamically created
     * 
     * @note application should initialize this field
     */
    ThreadPriority maxDynamicPriority;

    /**
     * Running timer partition
     * 
     * @note application should not initialize this field
     */
    ptrPartition runningTimerPartition;

    /**
     * Running partition (used when the system is initializing)
     * 
     * @note application should not initialize this field
     */
    ptrPartition runningPartition;

    /**
     * Running interrupt handler partition
     * 
     * @note application should not initialize this field
     */
    ptrPartition pharosVRunningInterruptEnv;

    /**
     * Table of application ISRs per interrupt number per core
     * 
     * @note application should not initialize this field
     */
    ptrInterruptConfiguration pharosVIsrTable[PHAROS_NUMBER_INTERRUPTS];

    /**
     * State in which the core is in
     * 
     * @note application should not initialize this field
     */
    PharosCoreState state;

    /* Make sure that each Core has a size multiple of the cache line to ensure that the structure is not accessed by 
     * a core different from the running core to avoid expensive cache communication */
} __attribute__((aligned(PHAROS_CPU_CACHE_LINE_SIZE)));

struct CoreTable
{
    /**
     * Table of Cores
     */
    ptrCore cores;

    /**
     * number of cores
     */
    uint32_t size;

    /**
     * Function that will be called (if not NULL) to determine if the running partition has enough permissions to 
     * restart the restarting partition.
     * If NULL, only a partition can restart itself. 
     * Also, a partition is always able to restart itself (this method will not be invoked in this case).
     */
    PartitionRestartGrantHandler restartGrantHandler;
};

/**
 * restore the previous pack definition
 */
#pragma pack(pop)

/**
 * Table of cores.
 * 
 * @note Application needs to define this variable and place inside the full configuration of the system
 */
extern CoreTable pharosVCoreTable;


/**
 * Table of function calls to the syscalls 
 */
extern const void * pharosVSystemCalls[];


/**
 * Initialize the running core
 */
void pharosCoreConfInitialize(ptrPharosCoreConfiguration conf);


/**
 * Get the scheduler on the running core
 * 
 * @return returns the scheduler on the running core
 */
INLINE ptrScheduler pharosIScheduler(void);


/**
 * Internal function to shutdown the CPU in supervisor mode
 */
PharosShutdownR pharosSShutdown(void);


/**
 * Internal function to restart the CPU in supervisor mode
 */
PharosRestartR pharosSRestart(void);


/**
 * Fill the specified address with zeros
 * 
 * @param start address where to fill with zeros
 * @param size number of 8-bytes to initialize
 */
void pharosIZeros64(uint64_t *start , uintptr_t size);


/**
 * Copy the number of specified 8-byte data from the source to the destination
 * 
 * @param dst destination address
 * @param src source address
 * @param size number of 8-bytes to transmit
 */
void pharosICopy64(uint64_t *dst , const uint64_t *src , const uintptr_t size);


/**
 * Calculate the ceil of log2 of a specified number (only the integer part)
 * 
 * if 2^(log2(n)) == n then log2(n) is equal to ceil log2(n).
 * 
 * For example:
 *   if n = 100100b then log2 will give 5 and log2ceil will give 6
 *   if n = 100000b then log2 will give 5 and log2ceil will give 5
 * 
 * @param n number to calculate the ceil log2 of
 * 
 * @return returns the integer part of the log2 of n
 */
uint32_t pharosUint32Log2Ceil(uint32_t n);


/**
 * Determines if the specified core is in the nominal state
 * 
 * @param core core to check
 * 
 * @return returns TRUE if the core is in the nominal state
 */
INLINE bool pharosICoreIsInNominalState(ptrCore core);


/**
 * Places the core in the specified state
 * 
 * @param core core to change
 * @param state state to place
 */
INLINE void pharosICoreSetState(ptrCore core , PharosCoreState state);


/**
 * Calculate the log2 of a specified number (only the integer part)
 * 
 * @constraint n must NOT be zero
 * 
 * @param n number to calculate the log2 of
 * 
 * @return returns the integer part of the log2 of n
 */
INLINE uint32_t pharosUint32Log2(uint32_t n)
{
    /* use the builtin function to get the highest leading 0 bit, and with it get the highest leading 1 bit */
    return 31U - __builtin_clz(n);
}


#endif /* CORE_H */
