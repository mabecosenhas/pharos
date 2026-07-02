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


/**
 * This file contains the functions that the boards must implement in order for Pharos to compile
 */

#ifndef PHAROS_BOARDAPI_H
#define PHAROS_BOARDAPI_H


#include <pharos/kernel/object/declarations.h>
#include <pharos/kernel/resource/declarations.h>
#include <pharos/kernel/time/declarations.h>
#include <pharos/kernel/buffer/declarations.h>
#include <pharos/kernel/thread/declarations.h>
#include <pharos/kernel/queue/declarations.h>
#include <pharos/kernel/channel/declarations.h>
#include <pharos/kernel/event/declarations.h>
#include <pharos/kernel/interrupt/declarations.h>
#include <pharos/kernel/partition/declarations.h>
#include <pharos/kernel/bulk/declarations.h>    
#include <pharos/kernel/semaphore/declarations.h>
#include <pharos/kernel/future/declarations.h>
#include <pharos/kernel/core/declarations.h>
#include <pharos/kernel/memory/memory.h>
#include <pharos/hal/declarations.h>
#include <pharos/hal/cpu/cpu.h>
#include <pharos/hal/interrupt.h>


extern const EnvMemoryProtection pharosVKernelMemProtection;


/**
 * Initialize the memory protection given its start and end address
 * 
 * @param memProtection protection to initialize
 * @param partition partition number to initialize on the table
 */
void pharosCpuMemProtectionInit(ptrEnvMemoryProtection memProtection , PartitionNumber partition);


/**
 * Get the stack of the idle thread on the running core
 * 
 * @return returns the stack of the idle thread 
 */
uint8_t *pharosCpuThreadIdleStack(void);


/**
 * Switch the CPU mode to user mode and execute the thread entry point
 * 
 * @param partitionData partition data
 * @param event event received
 * @param thread running thread
 */
void pharosCpuExecuteUserModeEvent(void *partitionData , Event event , ptrThreadInfo thread);


/**
 * Execute the sporadic thread job with a mutex in user mode
 * 
 * @param partitionData partition data
 * @param ssId mutex number
 * @param thread running thread
 */
void pharosCpuExecuteUserModeMutex(void *partitionData , SemMutexNumber ssId , ptrThreadInfo thread);


/**
 * Execute the sporadic thread job with a light weight queue message in user mode
 * 
 * @param partitionData partition data
 * @param number light weight queue number
 * @param message message received
 * @param buffer buffer of the message
 * @param size size (bytes) of the message
 * @param thread running thread
 */
void pharosCpuExecuteUserModeLwQueue(void *partitionData , LwQueueNumber number , ptrBulk message ,
                                     uint8_t *buffer , uint32_t size , ptrThreadInfo thread);


/**
 * Execute the sporadic thread job with a heavy weight queue message in user mode
 * 
 * @param partitionData partition data
 * @param queueId hw queue id
 * @param buffer buffer of the received message
 * @param size size of the message
 * @param priority priority of the received message
 * @param number future id pointer (NULL if none)
 * @param thread pointer to the running thread
 */
void pharosCpuExecuteUserModeHwQueue(void *partitionData , HwQueueNumber queueId , uint8_t *buffer ,
                                     uint32_t size , PharosPriority priority , ptrFutureId number ,
                                     ptrThreadInfo thread);


/**
 * Execute the sporadic thread job with a channel message in user mode
 * 
 * @param partitionData partition data
 * @param channelId channel id
 * @param bulk bulk received
 * @param bulkBuffer bulk buffer received
 * @param bulkSize size of the bulk message received (number of bytes)
 * @param messageBuffer buffer message received
 * @param messageSize size of the buffer message received (number of bytes)
 * @param priority priority of the received message
 * @param number future id pointer (NULL if none)
 * @param thread pointer to the running thread
 */
void pharosCpuExecuteUserModeChannel(void *partitionData , ptrChannelBulk bulk ,
                                     ChannelId channelId , PharosPriority priority , ptrFutureId number ,
                                     ptrThreadInfo thread);


/**
 * Execute the thread in aperiodic fashion (change the stack, from supervisor to user mode, etc)
 * 
 * note: this function must not return
 * 
 * @param partitionData partition data
 * @param thread aperiodic thread to execute
 */
void pharosCpuExecuteUserModeAper(void *partitionData , ptrThreadInfo thread) __attribute__((noreturn));


/**
 * Execute the thread in periodic fashion (change the stack, from supervisor to user mode, etc)
 * 
 * note: this function returns (each time this function is invoked means that one periodic job is executed)
 * 
 * @param partitionData partition data
 * @param thread periodic thread to execute
 */
void pharosCpuExecuteUserModePer(void *partitionData , ptrThreadInfo thread);


/**
 * Switch the CPU mode to supervisor mode
 */
CpuModeChangeContext pharosCpuSwitchSupervisorMode(CpuModeChangeContext context);


/**
 * access the resource access method in the context of the shared stack
 * 
 * @param data partition data
 * @param input input to access the resource with
 * @param method method to execute
 * @param context MMU context in which to execute the resource access method
 * @param sharedStack stack in which to execute the resource access method
 * 
 * @return returns the access method result
 */
uint64_t pharosCpuResourceExecuteAccess(void *data , uint64_t input , ResourceAccessMethod method ,
                                        ptrPartitionCpuMmuContext context , StackPointer sharedStack , ptrPartition resourceOwner);


/**
 * execute the resource filter in user mode and with the calling shared stack
 * 
 * @param data partition data
 * @param id resource number to execute
 * @param callerId environment in which the filter is executed
 * @param input input to the resource
 * @param lastExecuted last clock tick where the filter was executed
 * @param filter filter function to execute
 * @param context context to where to switch the MMU to
 * @param sharedStack stack pointer in which to execute the filter
 * 
 * @return returns the filter result
 */
PharosResourceFilterR pharosCpuResourceExecuteFilter(void *data , ResourceNumber id , uint64_t input ,
                                                     PartitionNumber callerId , ResourceFilterMethod filter ,
                                                     ptrPartitionCpuMmuContext context , StackPointer sharedStack , ptrPartition resourceOwner);


/**
 * execute the hw queue filter in user mode and with the calling shared stack
 * 
 * @param data partition data
 * @param id heavy weight queue id
 * @param sender environment in which the filter is executed
 * @param priority priority with which the caller wants to send the message
 * @param lastSent last clock tick where the filter was executed
 * @param messageSize number of bytes of the message
 * @param filter filter method to be executed 
 * @param context context to where to switch the MMU to
 * @param sharedStack stack pointer in which to execute the filter
 * 
 * @return returns the filter result
 */
PharosHwQueueFilterR pharosCpuHwQueueExecuteFilter(void *data , HwQueueNumber id , PartitionNumber sender ,
                                                   PharosPriority priority , uint32_t messageSize ,
                                                   HeavyWeightFilterMethod filter ,
                                                   ptrPartitionCpuMmuContext context , StackPointer sharedStack , ptrPartition hwQueueOwner);


/**
 * execute the channel filter in user mode and with the calling thread shared stack
 * 
 * @param data data to send to the filter
 * @param id channel id
 * @param sender  environment in which the filter is executed
 * @param priority priority with which the caller wants to send the message
 * @param lastSent last clock tick where the filter was executed
 * @param bulk bulk to be sent
 * @param filter filter method to be executed 
 * @param context context to where to switch the MMU to
 * @param sharedStack stack pointer in which to execute the filter
 * 
 * @return returns the filter result
 */
PharosChannelFilterSendR pharosCpuChannelSendExecuteFilter(void *data , ChannelNumber id , PartitionNumber sender ,
                                                           PharosPriority priority , ptrChannelBulk bulk ,
                                                           ChannelFilterSendMethod filter , ptrPartitionCpuMmuContext context ,
                                                           StackPointer sharedStack , ptrPartition channelOwner);


PharosChannelFilterReceiveR pharosCpuChannelReceiveExecuteFilter(void *data , PartitionNumber receiverId ,
                                                                 ChannelId id , ChannelFilterReceiveMethod filter ,
                                                                 ptrPartitionCpuMmuContext context ,
                                                                 StackPointer sharedStack , ptrPartition channelOwner);
/**
 * Executes the specified handler in user mode and on the specified MMU context and with interrupts disabled
 * 
 * @param data partition data
 * @param arg1 argument 1 for the handler
 * @param arg2 argument 2 for the handler
 * @param arg3 argument 3 for the handler
 * @param arg4 argument 4 for the handler
 * @param context MMU context 
 * @param handler handler to execute 
 * 
 * @return output of the method
 */
uint32_t pharosCpuCriticalSectionExecute(void *data , uint32_t arg1 , uint32_t arg2 , uint32_t arg3 ,
                                         uint32_t arg4 , ptrPartitionCpuMmuContext context ,
                                         CriticalSectionHandler handler);


/**
 * Syscall internal function when the resource access has ended executing
 * 
 * @param address that triggered the syscall. If not correct, this function will not do anything.
 * @param context current context at the time the syscall was made
 * 
 * @return returns the context after the syscall has ended. If the address is correct, will switch to
 * supervisor mode
 */
CpuModeChangeContext pharosCpuResourceAccessEnded(uint8_t *address , CpuModeChangeContext context);


/**
 * Syscall internal function when the resource filter has ended executing
 * 
 * @param address that triggered the syscall. If not correct, this function will not do anything.
 * @param context current context at the time the syscall was made
 * 
 * @return returns the context after the syscall has ended. If the address is correct, will switch to 
 * supervisor mode
 */
CpuModeChangeContext pharosCpuResourceFilterEnded(uint8_t *address , CpuModeChangeContext context);


/**
 * Syscall internal function when the heavy weight queue filter has ended executing
 * 
 * @param address that triggered the syscall. If not correct, this function will not do anything.
 * @param context current context at the time the syscall was made
 * 
 * @return returns the context after the syscall has ended. If the address is correct, will switch to 
 * supervisor mode
 */
CpuModeChangeContext pharosCpuHwQueueFilterEnded(uint8_t *address , CpuModeChangeContext context);


/**
 * Syscall internal function when the channel send filter has ended executing
 * 
 * @param address that triggered the syscall. If not correct, this function will not do anything.
 * @param context current context at the time the syscall was made
 * 
 * @return returns the context after the syscall has ended. If the address is correct, will switch to 
 * supervisor mode
 */
CpuModeChangeContext pharosCpuChannelFilterSendEnded(uint8_t *address , CpuModeChangeContext context);


/**
 * Syscall internal function when the channel receive filter has ended executing
 * 
 * @param address that triggered the syscall. If not correct, this function will not do anything.
 * @param context current context at the time the syscall was made
 * 
 * @return returns the context after the syscall has ended. If the address is correct, will switch to 
 * supervisor mode
 */
CpuModeChangeContext pharosCpuChannelFilterReceiveEnded(uint8_t *address , CpuModeChangeContext context);


/**
 * Syscall internal function when the critical section has ended executing
 * 
 * @param address that triggered the syscall. If not correct, this function will not do anything.
 * @param context current context at the time the syscall was made
 * 
 * @return returns the context after the syscall has ended. If the address is correct, will switch to supervisor mode
 */
CpuModeChangeContext pharosCpuCriticalSectionEnded(uint8_t *address , CpuModeChangeContext context);


/**
 * Syscall internal function when the IP-call filter ended executing
 * 
 * @param address that triggered the syscall. If not correct, this function will not do anything.
 * @param context current context at the time the syscall was made
 * 
 * @return returns the context after the syscall has ended. If the address is correct, will switch to supervisor mode
 */
CpuModeChangeContext pharosCpuIpCallFilterEnded(uint8_t *address , CpuModeChangeContext context);


/**
 * Syscall internal function when the IP-call input/output call ended executing
 * 
 * @param address that triggered the syscall. If not correct, this function will not do anything.
 * @param context current context at the time the syscall was made
 * 
 * @return returns the context after the syscall has ended. If the address is correct, will switch to supervisor mode
 */
CpuModeChangeContext pharosCpuIpCallIoEnded(uint8_t *address , CpuModeChangeContext context);


/**
 * Syscall internal function when the IP-call output call ended executing
 * 
 * @param address that triggered the syscall. If not correct, this function will not do anything.
 * @param context current context at the time the syscall was made
 * 
 * @return returns the context after the syscall has ended. If the address is correct, will switch to supervisor mode
 */
CpuModeChangeContext pharosCpuIpCallOEnded(uint8_t *address , CpuModeChangeContext context);


/**
 * Initialize a thread context CPU
 * 
 * @param context context to initialize
 * @param entryPoint thread entry point
 * @param stack stack pointer of the thread
 * @param stackSize stack size
 */
void pharosCpuThreadContextInit(ptrPartition partition , ptrThreadInfo thread , ptrEnvMemoryProtection memProtection);


/**
 * Re-initialize the thread CPU context 
 * 
 * @param thread thread to re-initialize
 * @param userEntryPoint user entry point
 * @param kernelEntryPoint kernel entry point
 * @param applicationStack application stack
 * @param kernelStack kernel stack
 * @param sharedStack shared stack
 */
void pharosCpuThreadContextReInit(ptrThreadInfo thread);


/**
 * Initialize the memory management unit of the CPU
 * (is called after the partitions and drivers have been initialized)
 */
void pharosCpuMmuInitialize(void);


/**
 * Initialize the partition with the specific CPU information (if needed)
 * 
 * @param partition partition to initialize
 */
void pharosCpuPartitionInitialize(ptrPartition partition);


/**
 * initialize the clock
 */
void pharosCpuClockInitialize(void);


/**
 * initialize the clock to run in nominal state
 * 
 * @constraint interrupts must be disabled when calling this function
 */
void pharosCpuClockReset(void);


/**
 * set a WCET interrupt in the specified amount of microseconds.
 * 
 * @param delta microseconds in which to generate the WCET interrupt
 */
void pharosCpuWcetClockFire(uint32_t delta);


/**
 * Disable the WCET clock interrupt
 */
void pharosCpuWcetClockDisable(void);


/**
 * Enable the interrupts (disregarding any previous interrupt state)
 */
void pharosCpuInterruptNestedEnable(void);


/**
 * Disable the interrupts (disregarding any previous interrupt state)
 */
void pharosCpuInterruptNestedDisable(void);


/**
 * Force the enable of interrupts
 */
void pharosCpuInterruptForceEnable(void);


/**
 * Force the disable of interrupts
 */
void pharosCpuInterruptForceDisable(void);


/**
 * disable interrupts. Increase interrupt level
 * 
 * @param number previous interrupt level
 */
INLINE void pharosCpuDisableInterrupts(ptrPharosInterruptLevel number);


/**
 * enable interrupts. Decreases interrupt level
 * 
 * @param level interrupt level to set
 */
INLINE void pharosCpuEnableInterrupts(ptrPharosInterruptLevel number);


/**
 * Temporarily enable interrupts to previous interrupt level and leave restore the same interrupt level again
 * From a functional point of view this is the same as invoking the enable and disable interrupt functions. 
 * However, it can be performed with fewer clock cycles (depending on the implementation)
 * 
 * @param number previous interrupt level
 */
void pharosCpuTempEnableInterrupts(ptrPharosInterruptLevel number);


/**
 * Initialize the partition MMU information 
 * 
 * @param context
 * @param memProtection
 */
void pharosCpuPartitionMmuInit(ptrPartitionCpuMmuContext context , ptrEnvMemoryProtection memProtection);


/**
 * performs the context switch to the first thread
 * 
 * @note: this function will not return to the caller
 * 
 * @param firstThread first thread CPU context
 */
void pharosCpuRestoreHeir(ptrThreadInfo firstThread) __attribute__((noreturn));


/**
 * Shuts down the CPU
 * 
 * method does not return
 */
void pharosCpuShutdown(void) __attribute__((noreturn));


/**
 * Restarts the CPU
 * 
 * method does not return
 */
void pharosCpuRestart(void) __attribute__((noreturn));


/**
 * Gets the correct stack pointer given the start of the stack and its size.
 * This method is useful to abstract the Pharos kernel from whether the stack grows up or down on each CPU
 * 
 * @param originalPointer start address of the stack
 * @param stackSize stack size
 * 
 * @return returns the correct stack pointer that should be used on this CPU
 */
StackPointer pharosCpuInitializeStackPointer(StackPointer originalPointer , uint32_t stackSize);


/**
 * Initialize the shared stack information
 * 
 * @param sharedStack shared stack to initialize
 */
void pharosCpuSharedStackInitialize(ptrStackShared sharedStack);


/**
 * Adds the shared stack memory space to the specified context
 * 
 * @param sharedStack
 * @param context MMU context where to add the permissions
 */
void pharosCpuAddSharedStack(ptrThreadInfo thread , ptrPartitionCpuMmuContext context);


/**
 * Removes the shared stack back memory space from the specified context
 * 
 * @param sharedStack
 * @param context MMU context where to restore the stack
 */
void pharosCpuRemoveSharedStack(ptrThreadInfo thread , ptrPartitionCpuMmuContext context);


/**
 * Updates the thread running partition (used in filters, ip-calls, resource access method)
 * 
 * @param thread thread to update
 * @param newPartition new running partition of the thread 
 * @param context new MMU/MPU context of the thread
 */
void pharosCpuThreadRunningPartitionUpdate(ptrThreadInfo thread , ptrPartition newPartition , ptrPartitionCpuMmuContext context);


/**
 * Updates the thread running partition to its original (used in filters, ip-calls, resource access method)
 * 
 * @param thread thread to update
 * @param original previous partition context the thread was in
 * @param context current context of the thread running partition
 */
void pharosCpuThreadRunningPartitionRestore(ptrThreadInfo thread , ptrPartition original , ptrPartitionCpuMmuContext context);


/**
 * Adds the channel bulk to the running partition 
 * 
 * @constraint interrupts must be disabled when calling this function
 * 
 * @param queue queue where the bulk is from
 * @param bulk bulk to add
 * @param partition partition from which to give the permissions to access the bulk to
 * 
 * @return returns the size of each bulk (in bytes) or 0 if insufficient MMU tables are available
 */
uint32_t pharosCpuChannelBulkAdd(ptrChannelBulkQueue queue , ptrChannelBulkKernel bulk , ptrPartition partition);


/**
 * Removes the channel from the specified partition
 * 
 * @constraint interrupts must be disabled when calling this function
 * 
 * @param bulk bulk to remove
 * @param partition partition from which to remove the permissions to access the bulk from
 */
void pharosCpuChannelBulkRemove(ptrChannelBulkKernel bulk , ptrPartition partition);


/**
 * Initialize a channel bulk queue
 * 
 * @param queue queue to initialize
 * 
 * @return returns the pointer to the first byte on the queue
 */
uint8_t *pharosCpuChannelBulkQueueInit(ptrChannelBulkQueue queue , uint32_t *bulkSize);


/**
 * catch an interrupt given its configuration
 * 
 * @param conf interrupt configuration
 */
void pharosCpuInterruptInstall(ptrInterruptConfiguration conf);


/**
 * Unmask the interrupt
 * 
 * @param conf interrupt configuration
 */
void pharosCpuInterruptUnmask(ptrInterruptConfiguration conf);


/**
 * Mask the interrupt
 * 
 * @param conf interrupt configuration
 */
void pharosCpuInterruptMask(ptrInterruptConfiguration conf);


/**
 * Enable the nested interrupts
 */
void pharosCpuInterruptNestedEnable(void);


/**
 * remove an interrupt given its configuration
 * 
 * @param partConf partition interrupt configuration 
 * @param conf interrupt configuration
 */
void pharosCpuInterruptRemove(ptrInterruptConfiguration conf);

/**
 * Determine if the specified address is a valid I/O mem address
 * 
 * Note that the memAddress must be aligned to four bytes, that is, we cannot single out a single byte and have 
 * to give permissions to multiple of 4 bytes.
 * That is because the address that the application is trying to write to is read as an address of 4 bytes and
 *  NOT as a single byte
 */
bool pharosCpuIoMemAddressIsValid(const ptrPartition partition , uint8_t *memAddress);


/**
 * Executes the instruction address at the specified address
 */
void pharosCpuExecuteMemAddress(ptrThreadCpuContext context , uint8_t *interruptedInstruction);


/**
 * Initialize the IpCall preamble 
 * 
 * @param preamble
 * @param syscall
 * @param function
 */
void pharosCpuIpCallPreambleInit(const ptrIpCall call , ptrIpCallMinimum minimum , uint32_t callNumber ,
                                 PartitionNumber partition);


/**
 * Idle thread job
 * 
 * This function is run at supervisor level and with full memory permissions (just like the kernel)
 * 
 * @note: application can override this function since Pharos only creates a default implementation 
 * (that does nothing). Since the function is run at supervisor level and with full memory permissions, special care
 * should be taken when overriding this function in order not to break the system.
 */
void pharosCpuIdleJobBody(void);


/**
 * Executes the filter function under a different stack and a different MMU context
 * 
 * @param partition partition attempting to execute the inter-partition call 
 * @param input input parameter
 * @param stack stack in which the filter is to be executed
 * @param context MMU context in which to execute the filter
 * @param filterFunction filter function to execute
 * 
 * @return returns the result of the filter function
 */
PharosIpCallR pharosCpuExecuteIpCallFilter(void *partitionData , PartitionNumber partition , uint8_t *input ,
                                           uint8_t *stack , ptrPartitionCpuMmuContext context , void *filterFunction , ptrPartition ipcallOwner);


/**
 * Executes the inter-partition call with input and output parameters in the context of the specified stack and MMU
 * 
 * @param input input parameter to pass to the function
 * @param output output parameter to pass to the function 
 * @param stack  stack in which the function is to be executed
 * @param context MMU context in which to execute the function
 * @param functionPointer function to execute
 * 
 * @return returns the result of the function
 */
PharosIpCallR pharosCpuExecuteIpCallIo(void *partitionData , uint8_t *input , uint8_t *output , uint8_t *stack ,
                                       ptrPartitionCpuMmuContext context , void *functionPointer ,
                                       ptrPartition caller , ptrPartition ipcallOwner);


/**
 * Executes the inter-partition call with only output parameters in the context of the specified stack and MMU.
 * This is the same as the function pharosCpuExecuteIpCallIo but without input parameter
 * 
 * @param input input parameter to pass to the function
 * @param output output parameter to pass to the function 
 * @param stack  stack in which the function is to be executed
 * @param context MMU context in which to execute the function
 * @param functionPointer function to execute
 * 
 * @return returns the result of the function
 */
PharosIpCallR pharosCpuExecuteIpCallO(void *partitionData , uint8_t *output , uint8_t *stack ,
                                      ptrPartitionCpuMmuContext context , void *functionPointer ,
                                      ptrPartition caller , ptrPartition ipcallOwner);


/**
 * Shuts down the CPU
 */
void pharosCpuShutdown(void) __attribute__((noreturn));


/**
 * initialize the CPU
 * 
 * @param core core to initialize
 */
void pharosCpuCoreInitialize(ptrCore core);


/**
 * initialize the clock
 */
void pharosCpuClockInitialize();


/**
 * performs a context switch between thread "current" and thread "heir"
 * 
 * @param current current thread that will relinquish the CPU
 * @param heir next thread to occupy the CPU
 */
void pharosCpuContextSwitch(ptrThreadInfo current , ptrThreadInfo heir);


/**
 * Signal to a thread that the inter-partition call (filter, resource access, inter-partition call) has ended.
 * The thread should return to the specified inter-partition context and return FALSE
 * 
 * @param thread thread to signal
 * @param context inter-partition context and return TRUE
 */
void pharosCpuThreadFinishCall(ptrThreadInfo thread , ptrPharosCpuInterPartitionContext context);


/**
 * Prints a char to the debug UART 
 * 
 * @param c char to print
 */
void outputSChar(char_t c);


/**
 * Check if the specified address is inside the interrupt stack 
 * 
 * @param interruptStackStart address where to place the start address of the interrupt stack (for the running core)
 * @param interrruptStackEnd address where to place the end address of the interrupt stack (for the running core)
 */
void pharosCpuInterruptStackGet(uint8_t **interruptStackStart , uint8_t **interrruptStackEnd);


#endif /* BOARDAPI_H */
