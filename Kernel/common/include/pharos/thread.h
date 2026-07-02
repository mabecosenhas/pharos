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


#ifndef PHAROS_API_THREAD_H
#define PHAROS_API_THREAD_H

#include <pharos/declarations.h>
#include <pharos/kernel/thread/sporadicthread.h>
#include <pharos/kernel/thread/periodicthread.h>
#include <pharos/kernel/thread/aperiodicthread.h>
#include <pharos/kernel/partition/partition.h>
#include <pharos/kernel/thread/threadinline.h>


/**
 * Causes the calling thread to wait for the specified amount of clock ticks.
 *
 * @param clockTicks number of clock ticks to wait
 */
PharosWaitR pharosWait(ClockTick clockTicks);


/**
 * Suspends the thread with the specified id
 * 
 * @param id id of the thread to suspend
 * @param rightNow if TRUE, then attempts to suspend the thread right now and returns an error if it cannot. If set to 
 * FALSE will return (and thread will not be suspended if an error occurs)
 * 
 * @return returns the result of the suspend action
 */
PharosThreadSuspendR pharosThreadSuspend(ptrThreadId id , bool rightNow);


/**
 * Suspends the calling thread
 * 
 * @param rightNow if TRUE, then attempts to suspend the thread right now and returns an error if it cannot. If set to 
 * FALSE will return
 * 
 * @return returns the result of suspending itself
 */
PharosThreadSuspendSelfR pharosThreadSuspendSelf(bool rightNow);


/**
 * Resumes a thread
 * @param id id of the thread to resume
 * 
 * @return returns the result of the resume action
 */
PharosThreadResumeR pharosThreadResume(ptrThreadId id);


/**
 * Creates a thread id for an aperiodic thread based on its number and on the running partition
 * 
 * @param number number of the thread (index on the thread table of the running partition)
 * @param id address where to place the id
 */
INLINE void pharosThreadIdInitAperiodic(const ptrThreadId id , ThreadNumber number);


/**
 * Creates a thread id for an periodic thread based on its number and on the running partition
 * 
 * @param number number of the thread (index on the thread table of the running partition)
 * @param id address where to place the id
 */
INLINE void pharosThreadIdInitPeriodic(const ptrThreadId id , ThreadNumber number);


/**
 * Creates a thread id for a sporadic thread based on its number and on the running partition
 * 
 * @param number number of the thread (index on the thread table of the running partition)
 * @param id address where to place the id
 */
INLINE void pharosThreadIdInitSporadic(const ptrThreadId id , ThreadNumber number);


/**
 * Create an aperiodic thread 
 * 
 * @param properties aperiodic thread properties
 * 
 * @return returns success or the corresponding error
 */
PharosThreadAperiodicCreateR pharosThreadAperiodicCreate(const ptrPharosThreadAperiodicProperties properties , ptrThreadId id);


/**
 * Create an periodic thread 
 * 
 * @param properties periodic thread properties
 * 
 * @return returns success or the corresponding error
 */
PharosThreadPeriodicCreateR pharosThreadPeriodicCreate(const ptrPharosThreadPeriodicProperties properties , ptrThreadId id);


/**
 * Create an sporadic thread 
 * 
 * @param properties sporadic thread properties
 * 
 * @return returns success or the corresponding error
 */
PharosThreadSporadicCreateR pharosThreadSporadicCreate(const ptrPharosThreadSporadicProperties properties , ptrThreadId id);


/**
 * Starts a thread 
 * 
 * @param id id of the thread to start
 * 
 * @return returns success or the corresponding error
 */
PharosThreadStartR pharosThreadStart(ptrThreadId id);


/**
 * Starts a thread with a specified delay
 * 
 * @param id id of the thread to start
 * @param delay number of clock ticks from which to start the start
 * 
 * @return returns success or the corresponding error
 */
PharosThreadStartDelayR pharosThreadStartDelay(ptrThreadId id , ClockTick delay);


/**
 * Gets the name of the running thread
 * 
 * @param id address where to place the thread id
 * 
 * @return returns success or the error
 */
PharosThreadGetIdSelfR pharosThreadGetIdSelf(ptrThreadId id);


/**
 * Get the thread id associated with the thread name
 * 
 * @param name name of the thread to look for
 * @param id address where to place the thread id
 * 
 * @return returns success or the error
 */
PharosThreadGetIdR pharosThreadGetId(const char *name , ptrThreadId id);


/**
 * Get the thread status associated to the thread id
 * 
 * @param id id of the thread to get the status from
 * @param status address where to place the status 
 * 
 * @return returns success or the error 
 */
PharosThreadGetStatusR pharosThreadGetStatus(ptrThreadId id , ptrThreadStatus status);


/**
 * Set the thread priority of the specified thread id
 * 
 * @param id id of the thread to update the priority
 * @param newPriority new priority to set. Must not be higher than the maximum allowed priority
 * 
 * @return returns success or the error
 */
PharosThreadPrioritySetR pharosThreadSetPriority(ptrThreadId id , ThreadPriority newPriority);


/**
 * Restarts the thread 
 * 
 * @param id id of the thread to restart 
 * 
 * @return returns success or the error
 */
PharosThreadRestartR pharosThreadRestart(ptrThreadId id);


/**
 * Stops the running thread. It can be started again.
 * 
 * @param rightNow if TRUE, then attempts to stop the thread right now and returns an error if it cannot. If set to 
 * FALSE will return (and thread will not be stopped if an error occurs)
 * 
 * @return returns success or the error
 */
PharosThreadStopSelfR pharosThreadStopSelf(bool rightNow);


/**
 * Stops the specified thread. The thread can be started again.
 * 
 * @param id id of the thread to stop
 * @param rightNow if TRUE, then attempts to stop the thread right now and returns an error if it cannot 
 * (and thread will not be stopped if an error occurs)
 * 
 * @return returns success or the error
 */
PharosThreadStopR pharosThreadStop(ptrThreadId id , bool rightNow);


/**
 * Determines the stack usage of the specified thread
 * 
 * @param id thread to calculate the stack usage of
 * @param exactMethod TRUE value to calculate the stack usage with an exact (and slow) method, FALSE value to 
 *        calculate an approximate (actual value can be bigger) stack usage using a faster method O(log(N)).
 * @param userStackUsage pointer where to place the user stack usage (in bytes)
 * @param kernelStackUsage pointer where to place the kernel stack usage (in bytes)
 * @param sharedStackUsage pointer where to place the shared stack usage (in bytes)
 * 
 * @return returns success or the error
 */
PharosThreadStackR pharosThreadStack(ptrThreadId id ,
                                     bool exactMethod ,
                                     uintptr_t *userStackUsage ,
                                     uintptr_t *kernelStackUsage ,
                                     uintptr_t *sharedStackUsage);

#endif /* API_H */
