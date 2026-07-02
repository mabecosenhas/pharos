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


#ifndef PHAROS_PHAROS_SYSCALL_H
#define PHAROS_PHAROS_SYSCALL_H

/**
 * System calls available for all partitions/drivers.
 */
#define PHAROS_SYSCALL_RESTORE_TLB                                  0
#define PHAROS_SYSCALL_BULK_GET                                     1
#define PHAROS_SYSCALL_BULK_GIVE                                    2
#define PHAROS_SYSCALL_CHANNEL_SEND                                 3
#define PHAROS_SYSCALL_CHANNEL_LOCAL_RECEIVE                        4
#define PHAROS_SYSCALL_CHANNEL_BULK_QUEUE_GET                       5
#define PHAROS_SYSCALL_CHANNEL_BULK_BUFFER                          6
#define PHAROS_SYSCALL_CHANNEL_FREE_BULK                            7
#define PHAROS_SYSCALL_CLOCK_GET_DATE                               8
#define PHAROS_SYSCALL_CLOCK_SET_DATE                               9
#define PHAROS_SYSCALL_CLOCK_GET_TICKS                              10
#define PHAROS_SYSCALL_CLOCK_GET_SINCE_BOOT                         11
#define PHAROS_SYSCALL_FATAL_ERROR_ANNOUNCE                         12
#define PHAROS_SYSCALL_ERROR_ANNOUNCE                               13
#define PHAROS_SYSCALL_EVENT_SEND                                   14
#define PHAROS_SYSCALL_EVENT_RECEIVE                                15
#define PHAROS_SYSCALL_LW_QUEUE_SEND                                16
#define PHAROS_SYSCALL_LW_QUEUE_RECEIVE                             17
#define PHAROS_SYSCALL_LW_QUEUE_RECEIVE_TIMEOUT                     18
#define PHAROS_SYSCALL_HW_QUEUE_SEND                                19
#define PHAROS_SYSCALL_HW_QUEUE_RECEIVE                             20
#define PHAROS_SYSCALL_RESOURCE_ACCESS                              21
#define PHAROS_SYSCALL_SEMAPHORE_CEILING_OBTAIN                     22
#define PHAROS_SYSCALL_SEMAPHORE_CEILING_RELEASE                    23
#define PHAROS_SYSCALL_SEMAPHORE_MUTEX_OBTAIN                       24
#define PHAROS_SYSCALL_SEMAPHORE_MUTEX_OBTAIN_TIMEOUT               25
#define PHAROS_SYSCALL_SEMAPHORE_MUTEX_RELEASE                      26
#define PHAROS_SYSCALL_SEMAPHORE_COUNTING_OBTAIN                    27
#define PHAROS_SYSCALL_SEMAPHORE_COUNTING_OBTAIN_TIMEOUT            28
#define PHAROS_SYSCALL_SEMAPHORE_COUNTING_RELEASE                   29
#define PHAROS_SYSCALL_SHUTDOWN                                     30
#define PHAROS_SYSCALL_WAIT                                         31
#define PHAROS_SYSCALL_THREAD_SUSPEND                               32
#define PHAROS_SYSCALL_THREAD_SUSPEND_SELF                          33
#define PHAROS_SYSCALL_THREAD_RESUME                                34
#define PHAROS_SYSCALL_TIMER_START                                  35
#define PHAROS_SYSCALL_TIMER_STOP                                   36
#define PHAROS_SYSCALL_RESTART                                      37
#define PHAROS_SYSCALL_INTERRUPT_INSTALL                            38
#define PHAROS_SYSCALL_INTERRUPT_REMOVE                             39
#define PHAROS_SYSCALL_CRITICAL_SECTION_EXECUTE                     40
#define PHAROS_SYSCALL_IP_CALL                                      41
#define PHAROS_SYSCALL_DEBUG_PRINT_CHAR                             42
#define PHAROS_SYSCALL_FUTURE_GET                                   43
#define PHAROS_SYSCALL_FUTURE_FREE                                  44
#define PHAROS_SYSCALL_FUTURE_WAIT                                  45
#define PHAROS_SYSCALL_FUTURE_READY                                 46
#define PHAROS_SYSCALL_FUTURE_QUEUE_ADD                             47
#define PHAROS_SYSCALL_FUTURE_QUEUE_REMOVE                          48
#define PHAROS_SYSCALL_FUTURE_QUEUE_WAIT                            49
#define PHAROS_SYSCALL_MALLOC                                       50
#define PHAROS_SYSCALL_FREE                                         51
#define PHAROS_SYSCALL_THREAD_APERIODIC_CREATE                      52
#define PHAROS_SYSCALL_THREAD_PERIODIC_CREATE                       53
#define PHAROS_SYSCALL_THREAD_SPORADIC_CREATE                       54
#define PHAROS_SYSCALL_THREAD_START                                 55
#define PHAROS_SYSCALL_THREAD_START_DELAY                           56
#define PHAROS_SYSCALL_THREAD_GET_ID_SELF                           57
#define PHAROS_SYSCALL_THREAD_GET_ID                                58
#define PHAROS_SYSCALL_THREAD_GET_STATUS                            59
#define PHAROS_SYSCALL_THREAD_SET_PRIORITY                          60
#define PHAROS_SYSCALL_THREAD_STOP                                  61
#define PHAROS_SYSCALL_THREAD_STOP_SELF                             62
#define PHAROS_SYSCALL_THREAD_RESTART                               63
#define PHAROS_SYSCALL_PARTITION_RESTART                            64
#define PHAROS_SYSCALL_THREAD_STACK                                 65
#define PHAROS_SYSCALL_CHANNEL_RECEIVE                              66
#define PHAROS_SYSCALL_API_LAST_CALL                                PHAROS_SYSCALL_CHANNEL_RECEIVE   


/**
 * These syscalls are supposed to be called by Pharos and have one argument (the return address) 
 * If the argument is not correct, they don't do anything. The argument is set by the syscall itself. There is no direct method that invokes these syscalls
 */
#define PHAROS_SYSCALL_SPORADIC_THREAD_ENDED                     (PHAROS_SYSCALL_API_LAST_CALL + 1) /* 67 */
#define PHAROS_SYSCALL_PERIODIC_THREAD_ENDED                     (PHAROS_SYSCALL_API_LAST_CALL + 2) /* 68 */
#define PHAROS_SYSCALL_APERIODIC_THREAD_ENDED                    (PHAROS_SYSCALL_API_LAST_CALL + 3) /* 69 */
#define PHAROS_SYSCALL_INTERNAL_LAST_CALL                        PHAROS_SYSCALL_APERIODIC_THREAD_ENDED             

/**
 * These syscalls are supposed to be called by Pharos and one argument (the return address). 
 * They check the return address and if incorrect don't do anything.
 * There is no direct method that invokes these syscalls
 */
#define PHAROS_SYSCALL_SUPERVISOR_RESOURCE_EXECUTE_FILTER_ENDED         (PHAROS_SYSCALL_API_LAST_CALL +  4) /* 70 */
#define PHAROS_SYSCALL_SUPERVISOR_RESOURCE_EXECUTE_ACCESS_ENDED         (PHAROS_SYSCALL_API_LAST_CALL +  5) /* 71 */
#define PHAROS_SYSCALL_SUPERVISOR_HW_QUEUE_EXECUTE_FILTER_ENDED         (PHAROS_SYSCALL_API_LAST_CALL +  6) /* 72 */
#define PHAROS_SYSCALL_SUPERVISOR_CHANNEL_EXECUTE_FILTER_SEND_ENDED     (PHAROS_SYSCALL_API_LAST_CALL +  7) /* 73 */    
#define PHAROS_SYSCALL_SUPERVISOR_CHANNEL_EXECUTE_FILTER_RECEIVE_ENDED  (PHAROS_SYSCALL_API_LAST_CALL +  8) /* 74 */    
#define PHAROS_SYSCALL_SUPERVISOR_CRITICAL_SECTION_EXECUTE_END          (PHAROS_SYSCALL_API_LAST_CALL +  9) /* 75 */ 
#define PHAROS_SYSCALL_SUPERVISOR_IPC_FILTER_ENDED                      (PHAROS_SYSCALL_API_LAST_CALL + 10) /* 76 */ 
#define PHAROS_SYSCALL_SUPERVISOR_IPC_FUNCTION_IO_ENDED                 (PHAROS_SYSCALL_API_LAST_CALL + 11) /* 77 */ 
#define PHAROS_SYSCALL_SUPERVISOR_IPC_FUNCTION_O_ENDED                  (PHAROS_SYSCALL_API_LAST_CALL + 12) /* 78 */ 

#define PHAROS_SYSCALL_LAST_CALL                                        PHAROS_SYSCALL_SUPERVISOR_IPC_FUNCTION_O_ENDED


#endif /* SYSCALL_H */
