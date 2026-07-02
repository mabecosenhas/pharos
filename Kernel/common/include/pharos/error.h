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


#ifndef PHAROS_API_ERROR_H
#define PHAROS_API_ERROR_H


#include <pharos/declarations.h>
#include <pharos/kernel/object/declarations.h>
#include <pharos/kernel/thread/declarations.h>

typedef enum
{
    /**
     * WCET reached error
     * 
     * Occurs when a thread defines a WCET and it tries to execute for more time
     */
    PHAROS_THREAD_ERROR_WCET = 0 ,

    /**
     * Periodic thread period elapsed
     * 
     * Occurs when a periodic thread ends a job and tries to reschedule for the next period, but the the next period 
     * instant has already elapsed. Pharos will loop until it finds the next activation that is valid and raises an error
     * until it finds one.
     */
    PHAROS_THREAD_ERROR_PERIOD_MISS = 1 ,

    /**
     * Thread deadline miss error
     */
    PHAROS_THREAD_ERROR_DEADLINE_MISS = 2 ,

    /**
     * Thread attempted to access an invalid memory
     */
    PHAROS_THREAD_ERROR_INVALID_MEM_ACCESS = 3 ,

    /**
     * Thread is using an invalid stack. Note that the partition has access to the stack memory area,
     * but the thread is using the stack at an region where it should not. Not all CPUs can detect this error.
     */
    PHAROS_THREAD_ERROR_INVALID_STACK_ACCESS = 4 ,
} PharosThreadError;


/**
 * Handler for errors of a thread
 * 
 * @param threadId id of the thread that triggered the error
 * @param error error that was caused by the thread
 * @param inst address of the instruction of the thread that caused the error (if the error is PHAROS_THREAD_ERROR_INVALID_MEM_ACCESS) 
 * @param address contains the address that the thread attempted to get (if the error is PHAROS_THREAD_ERROR_INVALID_MEM_ACCESS)
 */
typedef void (*HealthThreadErrorHandler)(ThreadId threadId , PharosThreadError error , const uint8_t *inst , const uint8_t *address);

typedef void (*HealthErrorHandler)(PharosError error , const uint8_t *interruptedInstruction);

typedef void (*HealthFatalErrorHandler)(PharosFatalError error , const char_t *file , const char_t *func , uint32_t line);

/**
 * Announce a fatal error occurred. 
 * 
 * Note: Application can use the GCC macros: __FILE__, __func__ ,  __LINE__. This way GCC will automatically place 
 * the correct value
 * 
 * @param error error that occurred
 * @param file file where the error was detected (can use the GCC __FILE__ macro to automatically place the file name)
 * @param func function where the error was detected (can use the GCC __func__ macro to automatically place the function name)
 * @param line line number of the file where the error was detected (can use the GCC __LINE__ macro to automatically place the line number)
 */
void pharosFatalErrorAnnounce(PharosFatalError error , const char_t *file , const char_t *func , uint32_t line);


/**
 * Announce a non-fatal error occurred
 * 
 * @param error error that occurred
 */
void pharosErrorAnnounce(PharosError error , const uint8_t *interruptedInstruction);


/**
 * Handler of general errors (not specific to an interrupt). The application should implement this function to be 
 * able to handle general errors.
 * 
 * @param error error that occurred
 * @param interruptedInstruction address of the instruction that triggered the error (NULL if not relevant)
 */
void pharosErrorHandler(PharosError error , const uint8_t *interruptedInstruction);


/**
 * Handler of fatal errors. The application should implement this function to be able to handle fatal errors.
 * Caution, this method is invoked in supervisor mode.
 * 
 * @param error error that occurred
 * @param file file where the error was detected
 * @param func function where the error was detected 
 * @param line line number of the file where the error was detected 
 * 
 * @return returns TRUE if the error is indeed a fatal error and the system should shutdown or FALSE if not valid.
 * If TRUE, all partitions fatal error handlers will be invoked and aftwards the system will shutdown. If FALSE, the 
 * method returns and nothing will be done.
 */
bool pharosFatalErrorHandler(PharosFatalError error , const char_t *file , const char_t *func , uint32_t line);


#endif /* ERROR_H */
