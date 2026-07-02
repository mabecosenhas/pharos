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


#ifndef PHAROS_PARTITION_PARTITION_H
#define PHAROS_PARTITION_PARTITION_H


#include <pharos/hal/board.h>
#include <pharos/kernel/object/declarations.h>
#include <pharos/kernel/object/object.h>
#include <pharos/kernel/partition/declarations.h>
#include <pharos/kernel/bulk/bulk.h>
#include <pharos/kernel/resource/resource.h>
#include <pharos/kernel/object/protection.h>
#include <pharos/kernel/error/error.h>
#include <pharos/kernel/scheduler/scheduler.h>
#include <pharos/kernel/interrupt/interrupt.h>
#include <pharos/kernel/partition/ipcall.h>
#include <pharos/kernel/core/declarations.h>
#include <pharos/kernel/memory/memory.h>
#include <pharos/kernel/memory/malloc.h>
#include <pharos/kernel/interrupt/cstable.h>
#include <pharos/kernel/interrupt/interrupttable.h>
#include <pharos/kernel/interrupt/interrupt.h>
#include <pharos/kernel/thread/aperiodicthread.h>
#include <pharos/kernel/core/coreconf.h>


typedef void (*PartitionInitializeMethod)(PartitionNumber number , void *data);

/**
 * @brief partitions table of the application
 */
struct PartitionTable
{
    /**
     * Table of partitions 
     * 
     * @note application must initialize the partitions inside the table
     */
    ptrPartition partitions;

    /**
     * Number of partitions in the table 
     * 
     * @note application must initialize with the number of partitions inside the table
     */
    PartitionNumber size;
};

/**
 * @brief Partition structure
 */
struct Partition
{
    /**
     * Thread used to restart the partition.
     * 
     * When a partition restart is issued, threads of this partition will no longer be executed and instead this 
     * thread will be scheduled to be executed, which will restart the running partition.
     * 
     * Since a partition restart takes some time, it must be done in the context of a thread. In other to ensure 
     * timely response to the restart (very fast or very slow, depending on the characteristics of the partition) 
     * a separate thread (this one) is used to restart the partition. The developer can specify the priority, 
     * budget, intervals, etc.
     * 
     * If none is specify, the restart will be performed in the context of the idle thread.
     * 
     * @note application must initialize this thread (if none then it is initialized with NULL by the compiler)
     */
    ptrAperiodicThread restartThread;

    /**
     * State of the partition
     */
    PharosPartitionState state;

    /**
     * partition initialize method
     * 
     * @note application must initialize this function (if none then it is initialized with NULL by the compiler)
     */
    PartitionInitializeMethod initHandler;

    /**
     * Partition error handler routine
     * 
     * @note application must initialize this function (if none then it is initialized with NULL by the compiler)
     */
    HealthErrorHandler errorHandler;

    /**
     * Health deadline handler
     * 
     * @note application should initialize this variable to place a handler to be called when a deadline miss is 
     * detected
     */
    HealthThreadErrorHandler threadErrorHandler;

    /**
     * Partition fatal error routine
     * 
     * @note application must initialize this function (if none then it is initialized with NULL by the compiler)
     */
    HealthFatalErrorHandler fatalErrorHandler;

    /**
     * partition address where the contents of the partition will be.
     * 
     * Generic partitions should use this to encompass all the variables they own so that the applications which use this
     * generic partition can select the partition number.
     * 
     * For example, a very simple "Ethernet driver" partition could have a structure like this:
     * 
     * struct
     * {
     *     char macAddress[20]
     *     uint8_t incomingBuffer[4*1024]
     *     uint8_t outgoingBuffer[4*1024]
     *     uint32_t state
     * } Ethernet
     * 
     * 
     * and the application could define:
     * 
     * PARTITION_13_SECTION Ethernet ethernet = 
     * { 
     *    .macAddress = "213.42.2.1",
     * }
     *  
     * .
     * .
     * .
     * 
     *  Partition 13
     *  {
     *     .fatalErrorHandler = NULL
     *     .data = &ethernet
     *     .
     *     .
     *     .
     *  }
     * 
     * @note application must initialize this pointer (if none then it is initialized with NULL by the compiler)
     */
    void *data;

    /**
     * table of critical sections that this partition can execute
     * 
     * @note application should initialize this with the critical sections that this partition needs
     */
    CriticalSectionTable criticalSectionTable;

    /**
     * table of interrupts that this partition can manage
     * 
     * @note application should initialize this with the table of interrupts handlers that this partition requires
     */
    PartitionIsrConfTable interruptConfigurationTable;

    /**
     * table of I/O memory areas
     * 
     * @note application should initialize this with the I/O memory permissions
     */
    IoMemAreaTable ioTable;

    /**
     * table of Inter-partition calls
     * 
     * @note application should initialize this with the table of inter-partition calls that the partition supports
     */
    IpCallTable ipCallTable;

    /**
     * @brief objects of the partition
     * 
     * @note application must initialize this table
     */
    ObjectTable objects;

    /**
     * @brief Information of the memory protection for the partition
     * 
     * @note application should not initialize this flag
     */
    EnvMemoryProtection memProtection;

    /**
     * @brief CPU context of the driver
     * 
     * @note application should not initialize this flag
     */
    PartitionCpuMmuContext context;

    /**
     * @brief id of the running partition
     * 
     * @note application should not initialize this flag
     */
    PartitionNumber id;

    /**
     * Dynamic memory of the partition inside the user space. Used for malloc/free operations or to get the 
     * user stack space in case of thread creation
     * 
     * @note application should initialize this variable if dynamic creation of new threads is required 
     * (this corresponds to the user stack given to the threads) or simple "pharosMalloc" dynamic memory is used
     */
    PharosDynamicMemory dynamicMemory;

    /**
     * Dynamic memory for the partition inside the kernel space.
     * Used for thread/semaphore/queues creation
     * 
     * @note application should initialize this variable if dynamic creation of new threads/semaphores/etc is required 
     * (this corresponds to the shared stack given to the threads)
     */
    PharosDynamicMemory kernelDynamicMemory;

    /**
     * Dynamic memory for the partition inside the shared stack space.
     * Used for thread creation
     * 
     * @note application should initialize this variable if dynamic creation of new threads is required 
     * (this corresponds to the shared stack given to the threads)
     */
    PharosDynamicMemory sharedStackDynamicMemory;

    /**
     * Scheduler associated to the partition
     * 
     * @note application should initialize this variable with:
     *  - NULL if flat scheduling is required to schedule the partition on the core where it is running
     *  - address of a unique FlatScheduler instance for this partition if hierarchical scheduling is required
     */
    ptrFlatScheduler scheduler;

    /**
     * RMP information
     * 
     * @note application should not initialize this flag
     */
    PharosPartitionCore rmp;

    /**
     * Tree map of threads sorted by name
     * 
     * @note application should not initialize this flag
     */
    TreeMap threadsByName;

    /**
     * Map of aperiodic threads sorted by id
     * 
     * @note application should not initialize this flag
     */
    TreeMap aperiodicById;

    /**
     * Map of periodic threads sorted by id
     * 
     * @note application should not initialize this flag
     */
    TreeMap periodicById;

    /**
     * Map of sporadic threads sorted by id
     * 
     * @note application should not initialize this flag
     */
    TreeMap sporadicById;

    /**
     * Last aperiodic thread id number created
     * 
     * @note application should not initialize this flag
     */
    ThreadNumber aperiodicIterator;

    /**
     * Last periodic thread id number created
     * 
     * @note application should not initialize this flag
     */
    ThreadNumber periodicIterator;

    /**
     * Last sporadic thread id number created
     * 
     * @note application should not initialize this flag
     */
    ThreadNumber sporadicIterator;

    /**
     * Sum of all the threads (of this partition) inter partition nest level
     * 
     * @note application should not initialize this field
     */
    uint32_t interPartitionNestLevel;

    /**
     * List of external threads that are in the context of this partition
     * 
     * @constraint the running core can only update the corresponding index on this table
     * 
     * @note application should not initialize this field
     */
    LinkedList externalThreads[PHAROS_NUMBER_CORES];

    /**
     * List of channel bulks currently held by the partition. Useful when restarting the partition (has to free all
     * bulks held)
     * 
     * @constraint only the core that owns the partition must change this list
     * 
     * @note application should not initialize this field
     */
    LinkedList externalChannelBulksHold;

    /**
     * List of external futures hold
     * 
     * @constraint only the core that owns the partition must change this list
     * 
     * @note application should not initialize this field
     */
    LinkedList externalFuturesHold;

    /**
     * Partition restart number. Useful for futures to see if they have the correct version.
     * If not, that means that the partition has since restarted.
     * 
     * @note application should not initialize this field
     */
    PartitionRestartNumber restartNumber;

    /**
     * List of dynamically created threads
     * 
     * @note application should not initialize this field
     */
    SimpleList createdThreads;

    struct
    {
        /**
         * @brief indicates if the partition is to be initialized at startup (FALSE) or should be initialized later 
         * on (TRUE). If the flag is set to TRUE, then the partition will only be initialized while the idle thread
         * is running.
         * 
         * This could be very useful for allowing some partition to start in a much quicker way instead of having to
         * wait for all partitions to initialize.
         * 
         * @note application must initialize this boolean flag
         */
        uint32_t isLazyLoad : 1;

        /**
         * Flag that indicates if the partition has been initialized
         * 
         * @note application should not initialize this flag
         */
        uint32_t isInitialized : 1;

        /**
         * Indicates if the partition threads run in supervisor or user mode. While normally partitions should run
         * only in user mode, in debug mode it might be useful to run in supervisor mode to have access to all the 
         * memory, CPU registers, etc.
         * 
         * @note application must initialize this boolean flag
         */
        uint32_t isSupervisor : 1;

        /**
         * Indicates if the partition has enough permissions to reset the CPU. A faulty low criticality 
         * partition could be issuing resets without permission and this flag prevents that
         * 
         * @note application must initialize this boolean flag
         */
        uint32_t canResetCpu : 1;

        /**
         * Indicates if the partition has enough permissions to restarts the CPU. A faulty low criticality 
         * partition could be issuing restarts without permission and this flag prevents that
         * 
         * @note application must initialize this boolean flag
         */
        uint32_t canRestartCpu : 1;

        /**
         * Indicates if the partition is ready to restart
         * 
         * @note application should not initialize this flag
         */
        uint32_t readyRestart : 1;

    } partitionFlags;

    /**
     * List of free futures that the partition application can use
     * 
     * @note application does not have to initialize this variable
     */
    SimpleList freeFutures;

    /**
     * pointer core that owns this partition
     * 
     * @note application does not have to initialize this variable
     */
    ptrCore core;

    /**
     * Additional information that might be required for a CPU
     * For example, CPUs with MPU (instead of MMU) need to know which channel bulks are hold by the partition
     * 
     * @note application does not have to initialize this variable
     */
    PharosCpuPartitionInfo cpuInfo;
};


/**
 * Initialize the partitions of the specified core 
 */
void pharosIPartitionsInitialize(ptrCore core);


/**
 * Initialize the minimal information of the partitions of the core so that inter-partition calls (e.g. resources, 
 * hw queue, channels) will give an error if the partition has not been initialized yet but these calls don't 
 * "explode" the system due to data not being correctly initialized
 * 
 * @param core core to initialize
 * @param coreNumber number of the core
 */
void pharosIPartitionsInitializeMinimal(ptrCore core , CoreNumber coreNumber);


/**
 * Initialize the partition 
 * 
 * @param p partition to initialize 
 */
void pharosIPartitionInit(ptrPartition p);


/**
 * Re-initialize (immediately) a partition 
 * 
 * @param p partition to re-initialize
 */
void pharosIPartitionReinit(ptrPartition p);


/**
 * Initialize the memory areas of a specific partition
 * 
 * @param number partition number
 */
void pharosIPartitionDataInitialize(PartitionNumber number);


/**
 * Initialize the kernel data (data and bss sections)
 */
void pharosIKernelDataInitialize(void);


/**
 * Initialize the IP-call data section
 */
void pharosIIpCallDataInitialize(void);


/**
 * Initialize the shared data section
 */
void pharosISharedDataInitialize(void);


/**
 * Initialize the kernel bulks section
 */
void pharosIKernelBulksInitialize(void);


/**
 * Initialize the all the partitions objects (semaphores, threads, etc)
 */
void pharosIPartitionsObjectsInit(ptrCore core);


/**
 * Initialize all the partitions with lazy load
 */
void pharosIPartitionInitLazyLoad(void);


/**
 * get the partition with the specified id
 * 
 * @param id index of the partition on the partitions table
 * 
 * @return returns the pointer to the partition or NULL if invalid
 */
ptrPartition pharosIPartitionGet(PartitionNumber id);


/**
 * get the partition with the specified id if the partition is on the running core (if not returns NULL)
 * 
 * @param id index of the partition on the partitions table
 * 
 * @return returns the pointer to the partition or NULL if invalid
 */
INLINE ptrPartition pharosIPartitionGetOnRunCore(PartitionNumber id);


/**
 * Get the number of partitions configured by the application
 * 
 * @return returns the number of partitions configured by the application
 */
uint32_t pharosIPartitionGetNumber(void);


/**
 * Synchronizes all cores to reach this function. Only leaves when all cores have reached this function
 */
void pharosICoreSynchronize(void);


/**
 * Gets the running partition
 * 
 * @return returns the running partition
 */
ptrPartition pharosIPartitionGetRunning(void);


/**
 * Check if the partition has been initialized 
 * 
 * @param partition partition to check
 * 
 * @return returns TRUE if the partition has been initialized and FALSE otherwise 
 */
bool pharosIPartitionIsInitialized(ptrPartition partition);


/**
 * Add the aperiodic thread to the partition
 * 
 * @param partition partition where to add
 * @param thread thread to add
 */
void partitionAddAperiodicThread(ptrPartition partition , ptrAperiodicThread thread);


/**
 * Add the periodic thread to the partition
 * 
 * @param partition partition where to add
 * @param thread thread to add
 */
void partitionAddPeriodicThread(ptrPartition partition , ptrPeriodicThread thread);


/**
 * Add the sporadic thread to the partition
 * 
 * @param partition partition where to add
 * @param thread thread to add
 */
void partitionAddSporadicThread(ptrPartition partition , ptrSporadicThread thread);


/**
 * Syscall to restart the running partition
 * 
 * @param partition partition to restart
 * 
 * @return returns success or an error
 */
PharosPartitionRestartR pharosSPartitionRestart(PartitionNumber partition);


/**
 * Restart any partition that is pending a restart
 */
void pharosIPartitionRestartPending(void);


/**
 * Attempt to restart the partition on any core
 * 
 * @param partition partition to restart
 */
void pharosIPartitionRestartSignal(ptrPartition partition);


/**
 * Attempt to restart the partition on the partition core
 * 
 * @constraint must be executed on the partition core 
 * 
 * @param partition partition to restart
 */
void pharosIPartitionRestartSignalOnPartitionCore(ptrPartition partition);


/**
 * Check the conditions for the partition to restart
 * 
 * @constraint must be called from the same core as the owner of the partition 
 * 
 * @param partition partition to restart
 */
void pharosIPartitionRestartCheck(ptrPartition partition);


/**
 * Restart the partition on the local core
 * 
 * @param p partition to restart
 */
void pharosIPartitionRestartLocal(ptrPartition p);


/**
 * Body of the partition restart thread body
 */
void pharosIPartitionRestartThreadBody(void);


/**
 * Restore the original partition that the threads was on 
 * 
 * @param thread thread to restore
 * @param original original partition
 */
void pharosIThreadInterPartitionRestore(ptrThreadInfo thread , ptrPartition original , bool *wasInside ,
                                        ptrPharosCpuInterPartitionContext originalContext);


/**
 * Stop all external threads that are running inside the context of the partition. That is, threads that are inside the
 * filters or IP-calls of the partition.
 * 
 * @param partition partition to look at
 */
void pharosIPartitionExternalThreadsStop(ptrPartition partition);

/**
 * Stop all external threads that are running inside the context of the partition in the running core.
 * That is, threads that are inside the filters or IP-calls of the partition.
 * 
 * @param partition partition to look at
 */
void pharosIPartitionExternalThreadsStopOnPartitionCore(CoreNumber runningCore , ptrPartition partition);


/**
 * Resets all threads of the partition
 * 
 * @param partition partition to look at
 */
void pharosIPartitionResetThreads(ptrPartition partition);


/**
 * Check that the partition is in the ready state (could have a delay start or be restarting)
 * 
 * @param partition partition to check
 * 
 * @return returns TRUE if the partition is ready to be executed
 */
INLINE bool pharosIPartitionIsStarted(ptrPartition partition);

#endif /* PARTITION_H */
