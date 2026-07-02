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


#ifndef PHAROS_KERNEL_API_ERROR_H
#define PHAROS_KERNEL_API_ERROR_H


#include <pharos/error.h>


void pharosSFatalErrorAnnounce(PharosFatalError error , const char_t *file , const char_t *func , uint32_t line);


void pharosSErrorAnnounce(PharosError error , const uint8_t *interruptedInstruction);


/**
 * Announce that a thread error occurred and call the appropriate handler (if one exists)
 * 
 * @param t thread that triggered the error
 * @param e error that was caused by the thread
 * @param inst address of the instruction of the thread that caused the error (if the error is PHAROS_THREAD_ERROR_INVALID_MEM_ACCESS) 
 * @param address contains the address that the thread attempted to get (if the error is PHAROS_THREAD_ERROR_INVALID_MEM_ACCESS)
 * @param p partition that owns the thread
 */
void pharosSThreadErrorAnnounce(ptrThreadInfo t , PharosThreadError e , const uint8_t *inst , const uint8_t *address);

#endif /* ERROR_H */
