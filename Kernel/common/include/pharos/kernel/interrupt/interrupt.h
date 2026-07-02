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


#ifndef PHAROS_KERNEL_INTERRUPT_H
#define PHAROS_KERNEL_INTERRUPT_H

#include <pharos/kernel/interrupt/declarations.h>
#include <pharos/hal/hal.h>
#include <pharos/interrupt.h>
#include <pharos/kernel/interrupt/cstable.h>
#include <pharos/kernel/interrupt/interrupttable.h>
#include <pharos/hal/board.h>
#include <pharos/kernel/collection/linkedlist.h>
#include <pharos/kernel/partition/declarations.h>
#include <pharos/kernel/scheduler/declarations.h>
#include <pharos/kernel/thread/declarations.h>


/**
 * Handler to execute before the interrupt has been processed
 * 
 * @param argument argument set in the InterruptConfiguration
 * 
 * @return returns any information to pass to the PharosInterruptPostHandler
 */
typedef uintptr_t(*PharosInterruptPreHandler)(void *argument);


/**
 * Handler to execute after the interrupt has been processed
 * 
 * @param argument argument set in the InterruptConfiguration
 * @param preHandlerReturn return value of the PharosInterruptPreHandler (or zero if no PharosInterruptPreHandler was 
 * set)
 */
typedef void (*PharosInterruptPostHandler)(void *argument , uintptr_t preHandlerReturn);


/**
 * Application handler for an interrupt
 */
typedef void (*PharosInterruptHandler)(void *argument , uintptr_t preInterruptData , PharosInterruptNumber number , const uint8_t *interruptedAddress);

struct CriticalSection
{
    /**
     * critical section handler to be executed while interrupts are disabled
     * 
     * Use extreme caution when using this handler because if the interrupts are disabled for long, the system
     * maybe unresponsive. This affects all other threads on the same core since the system will NOT do anything
     * else until this method returns.
     * 
     * Also note that, while running this method, you have full memory permissions to any area of the system and are running in supervisor mode.
     * 
     * @note application must initialize this handler
     */
    CriticalSectionHandler csHandler;
};

struct InterruptConfiguration
{
    /**
     * CPU specific interrupt configuration
     * 
     * @note application must initialize this field (if the CPU supports additional interrupt configuration)
     */
    PharosCpuInterruptConfiguration cpu;

    /**
     * number of the interrupt to catch
     * 
     * @note application must initialize this number
     */
    PharosInterruptNumber number;

    /**
     * Handler to execute before the partition interrupt routines
     * 
     * @note application must initialize this field (or leave NULL if no handler is to be executed)
     */
    PharosInterruptPreHandler preInterruptHandler;

    /**
     * Handler to execute after the partition interrupt routines
     * 
     * @note application must initialize this field (or leave NULL if no handler is to be executed)
     */
    PharosInterruptPostHandler postInterruptHandler;

    /**
     * Flag indicating if the interrupt is already installed 
     * 
     * Initialized automatically by Pharos
     */
    bool isInstalled;

    /**
     * List of PartitionIsrConf
     * 
     * List<ptrPartitionIsrConf>
     * 
     * Initialized automatically by Pharos
     */
    LinkedList partitionInterrupts;

    /**
     * Argument to pass to the clear handler (if any)
     * 
     * @note application should initialize this number
     */
    void *argument;
};

struct PartitionIsrConf
{
    /**
     * handler to be executed.
     * 
     * Use extreme caution in writing this method since it is executed inside an interrupt and only higher priority 
     * interrupts can preempt it. The system will NOT execute any thread nor lower priority interrupts until this method returns.
     * 
     * Also note that, while running this method, you have full memory permissions to any area of the system and are running in supervisor mode.
     * 
     * @note application must initialize this handler
     */
    PharosInterruptHandler isrHandler;

    /**
     * Argument to pass when the routine is invoked 
     * 
     * @note application should not initialize this field
     */
    void *argument;

    /**
     * number of the interrupt in the table of partition interrupts 
     * 
     * @note application must initialize this handler
     */
    uint32_t number;

    /**
     * partition that installed this interrupt configuration
     * 
     * Initialized automatically by Pharos
     */
    ptrPartition partition;

    /**
     * node on the linked list of interrupt handlers to execute with the same interrupt number
     * 
     * Initialized automatically by Pharos
     */
    LinkedListNode node;

    /**
     * Cache for the interrupt number.
     * 
     * Initialized automatically by Pharos
     */
    PharosInterruptNumber interruptNumber;

    /**
     * Cache for the interrupt configuration 
     * 
     * Initialized automatically by Pharos
     */
    ptrInterruptConfiguration interruptConf;

    /**
     * Node on the linked list of the scheduler list of installed interrupts
     */
    LinkedListNode nodeInstalled;

    struct
    {
        /**
         * Flag indicating if the handler is installed
         * 
         * If set initially to TRUE, then the interrupt handler is installed at system startup
         * 
         * @note application must initialize this handler
         */
        uint32_t isInstalledStartup : 1;

        /**
         * Current state of installed
         */
        uint32_t isInstalled : 1;

        /**
         * Saved value of the "isInstalled" flag when switching partitions (in hierarchical scheduling)
         */
        uint32_t savedIsInstalled : 1;
    } flags;
};

struct PartitionIsrConfTable
{
    /**
     * pointer to a table of interrupts that the partition can manage
     * 
     * @note application must initialize this variable
     */
    ptrPartitionIsrConf configurations;

    /**
     * number of interrupt configurations on the table
     * 
     * @note application must initialize this variable
     */
    uint32_t size;
};


/**
 * determine if the CPU is running on an interrupt context or thread context
 * 
 * @return returns TRUE if the CPU is running inside an interrupt and FALSE otherwise
 */
INLINE bool pharosIInterruptIsRunning(void);


/**
 * Initialize the interrupts on a specified partition
 * 
 * @param partition partition which contains the interrupt handlers 
 * @param table table of interrupts of the partition
 */
void pharosIInterruptPartitionInitialize(ptrPartition partition , const ptrPartitionIsrConfTable table);


/**
 * Initialize an internal interrupt 
 * 
 * @param conf interrupt configuration
 * @param partConf partition interrupt configuration
 */
void pharosIInterruptInstallInit(ptrInterruptConfiguration conf , ptrPartitionIsrConf partConf);


/**
 * Initialize the interrupts on the specified core
 * 
 * @param core core to initialize the interrupts in
 */
void pharosIInterruptInit(ptrCore core);


/**
 * Removes all interrupts installed by a specified partition 
 * 
 * @param partition partition to remove the interrupts 
 * @param table table of interrupts of the partition
 */
void pharosIInterruptRemoveAll(ptrPartition partition , const ptrPartitionIsrConfTable table);


/**
 * Execute the interrupt handler only if the partition is in the nominal state
 * 
 * @constraint it is assumed that interrupts are enabled when this function is called 
 * @constraint it is assumed that the interrupt pre-interrupt handler has been called
 * (possibly clearing the interrupt before this function is called with interrupts enabled)
 * 
 * @param interruptedAddress address of the interrupt to execute
 * @param interrupt interrupt number
 * @param iterator partition ISR configuration
 */
void pharosIInterruptExecute(ptrInterruptConfiguration conf , uintptr_t preInterruptHandlerReturn ,
                             const uint8_t *interruptedAddress);


/**
 * Interrupt handler that is called when an invalid memory access is detected. 
 * Stops the running thread, forcing a thread dispatch to occur.
 * 
 * @param interruptedInstruction address of the instruction that triggered the error 
 * @param memAddress memory address that triggered the error
 * 
 * @return returns TRUE if the mem access is valid and FALSE otherwise
 */
bool pharosIInvalidMemAccess(uint8_t *interruptedInstruction , uint8_t *memAddress);


/**
 * Announce that the specified is using an invalid stack (perhaps overloading the stack)
 * 
 * @param running thread that caused the stack error
 * @param interruptedInstruction address that caused the error (if available)
 * @param memAddress memory address that was tried to be accessed (if available)
 */
void pharosIAnnounceInvalidStackError(ptrThreadInfo running , uint8_t *interruptedInstruction , uint8_t *memAddress);


/**
 * Announce an invalid memory error 
 * 
 * @param running thread that caused the invalid memory error
 * @param interruptedInstruction address that caused the error (if available)
 * @param memAddress memory address that was tried to be accessed (if available)
 */
void pharosIAnnounceInvalidMemoryError(ptrThreadInfo running , uint8_t *interruptedInstruction , uint8_t *memAddress);


/**
 * Interrupt handler that is called when an illegal instruction is attempted to be executed. 
 * Stops the running thread, forcing a thread dispatch to occur.
 * 
 * @param number interrupt number that triggered the interrupt
 */
void pharosIIllegalInstruction(uint8_t *interruptedInstruction);


/**
 * Interrupt handler that is called when an a memory access is performed to an unaligned address. 
 * Stops the running thread, forcing a thread dispatch to occur.
 * 
 * @param number interrupt number that triggered the interrupt
 */
void pharosIInterruptMemNotAligned(uint8_t *interruptedInstruction);


/**
 * Internal syscall.
 * 
 * Catch an interrupt given its index on the running partition
 * 
 * @param index index of the interrupt to install on the running partition InterruptTable
 * @param argument argument to pass to the interrupt routine
 * 
 * @return returns success or the error when attempting to install the interrupt
 */
PharosInterruptInstallR pharosSInterruptInstall(uint32_t index , void *argument);


/**
 * Internal syscall
 * 
 * Remove an interrupt given its index on the running partition
 * 
 * @param index index of the interrupt to remove on the running partition InterruptTable
 * 
 * @return returns success or the error when attempting to remove the interrupt
 */
PharosInterruptRemoveR pharosSInterruptRemove(uint32_t index);


/**
 * Internal syscall.
 * 
 * Executes the specified critical section given its index on the running partition
 * 
 * @param index index of the critical section to execute on the running partition CriticalSectionTable
 * @param arg1 argument 1 of the critical section function
 * @param arg2 argument 2 of the critical section function
 * @param arg3 argument 3 of the critical section function
 * @param arg4 argument 4 of the critical section function
 * @return returns success or error when attempting to execute the critical section
 */
PharosCsExecuteR pharosSCriticalSectionExecute(uint32_t index , uint32_t arg1 , uint32_t arg2 , uint32_t arg3 , uint32_t arg4 , uint32_t *output);


/**
 * Installs an internal Pharos interrupt (e.g. Clock tick interrupt)
 * 
 * @param conf interrupt core configuration
 */
void pharosIInterruptInstallInternal(ptrPartitionIsrConf partConf);


/**
 * Removes the interrupt of Pharos (used in hierarchical scheduling to remove the running shceduler interrupts)
 * 
 * @param partConf partition interrupt configuration
 * @param conf interrupt configuration
 */
void pharosIInterruptRemoveInternal(ptrPartitionIsrConf partConf);


/**
 * Get the environment that owns the running interrupt handler 
 * 
 * @return returns the pointer to a structure that defines the running environment
 */
INLINE ptrPartition pharosIInterruptRunningEnvGet(void);


/**
 * Set the environment that owns the running interrupt handler 
 * 
 * @param partition set the partition running on the current interrupt handler
 */
INLINE void pharosIInterruptRunningEnvSet(ptrPartition partition);


/**
 * Get the interrupt configuration given the interrupt vector number
 * 
 * @param vector interrupt vector number
 * 
 * @return returns the interrupt configuration (NULL if none is defined)
 */
INLINE ptrInterruptConfiguration pharosIInterruptConfigurationGet(PharosInterruptNumber vector);


#endif /* PHAROS_KERNEL_INTERRUPT_H */
