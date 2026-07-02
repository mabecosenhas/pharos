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


#include <pharos/hal/cpu/cpu.h>
#include <pharos/kernel/core/syscall.h>


const uint32_t pharosVCpuSyscallStackSize[PHAROS_SYSCALL_API_LAST_CALL + 1U] = {
    [PHAROS_SYSCALL_RESTORE_TLB] = 0U ,
    [PHAROS_SYSCALL_BULK_GET] = 0U ,
    [PHAROS_SYSCALL_BULK_GIVE] = 0U ,
    [PHAROS_SYSCALL_CHANNEL_SEND] = 0U ,
    [PHAROS_SYSCALL_CHANNEL_LOCAL_RECEIVE] = 0U ,
    [PHAROS_SYSCALL_CHANNEL_RECEIVE] = 0U ,
    [PHAROS_SYSCALL_CHANNEL_BULK_QUEUE_GET] = 0U ,
    [PHAROS_SYSCALL_CHANNEL_BULK_BUFFER] = 0U ,
    [PHAROS_SYSCALL_CHANNEL_FREE_BULK] = 0U ,
    [PHAROS_SYSCALL_CLOCK_GET_DATE] = 0U ,
    [PHAROS_SYSCALL_CLOCK_SET_DATE] = 0U ,
    [PHAROS_SYSCALL_CLOCK_GET_TICKS] = 0U ,
    [PHAROS_SYSCALL_CLOCK_GET_SINCE_BOOT] = 0U ,
    [PHAROS_SYSCALL_FATAL_ERROR_ANNOUNCE] = 0U ,
    [PHAROS_SYSCALL_ERROR_ANNOUNCE] = 0U ,
    [PHAROS_SYSCALL_EVENT_SEND] = 0U ,
    [PHAROS_SYSCALL_EVENT_RECEIVE] = 0U ,
    [PHAROS_SYSCALL_LW_QUEUE_SEND] = 0U ,
    [PHAROS_SYSCALL_LW_QUEUE_RECEIVE] = 0U ,
    [PHAROS_SYSCALL_LW_QUEUE_RECEIVE_TIMEOUT] = 0U ,
    [PHAROS_SYSCALL_HW_QUEUE_SEND] = 0U ,
    [PHAROS_SYSCALL_HW_QUEUE_RECEIVE] = 0U ,
    [PHAROS_SYSCALL_RESOURCE_ACCESS] = 0U ,
    [PHAROS_SYSCALL_SEMAPHORE_CEILING_OBTAIN] = 0U ,
    [PHAROS_SYSCALL_SEMAPHORE_CEILING_RELEASE] = 0U ,
    [PHAROS_SYSCALL_SEMAPHORE_MUTEX_OBTAIN] = 0U ,
    [PHAROS_SYSCALL_SEMAPHORE_MUTEX_OBTAIN_TIMEOUT] = 0U ,
    [PHAROS_SYSCALL_SEMAPHORE_MUTEX_RELEASE] = 0U ,
    [PHAROS_SYSCALL_SEMAPHORE_COUNTING_OBTAIN] = 0U ,
    [PHAROS_SYSCALL_SEMAPHORE_COUNTING_OBTAIN_TIMEOUT] = 0U ,
    [PHAROS_SYSCALL_SEMAPHORE_COUNTING_RELEASE] = 0U ,
    [PHAROS_SYSCALL_SHUTDOWN] = 0U ,
    [PHAROS_SYSCALL_WAIT] = 0U ,
    [PHAROS_SYSCALL_THREAD_SUSPEND] = 0U ,
    [PHAROS_SYSCALL_THREAD_SUSPEND_SELF] = 0U ,
    [PHAROS_SYSCALL_THREAD_RESUME] = 0U ,
    [PHAROS_SYSCALL_TIMER_START] = 0U ,
    [PHAROS_SYSCALL_TIMER_STOP] = 0U ,
    [PHAROS_SYSCALL_RESTART] = 0U ,
    [PHAROS_SYSCALL_INTERRUPT_INSTALL] = 0U ,
    [PHAROS_SYSCALL_INTERRUPT_REMOVE] = 0U ,
    [PHAROS_SYSCALL_CRITICAL_SECTION_EXECUTE] = 0U ,
    [PHAROS_SYSCALL_IP_CALL] = 0U ,
    [PHAROS_SYSCALL_DEBUG_PRINT_CHAR] = 0U ,
    [PHAROS_SYSCALL_FUTURE_GET] = 0U ,
    [PHAROS_SYSCALL_FUTURE_FREE] = 0U ,
    [PHAROS_SYSCALL_FUTURE_WAIT] = 0U ,
    [PHAROS_SYSCALL_FUTURE_READY] = 0U ,
    [PHAROS_SYSCALL_FUTURE_QUEUE_ADD] = 0U ,
    [PHAROS_SYSCALL_FUTURE_QUEUE_REMOVE] = 0U ,
    [PHAROS_SYSCALL_FUTURE_QUEUE_WAIT] = 0U ,
};
