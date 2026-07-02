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


#ifndef PHAROS_API_SEMAPHORE_H
#define PHAROS_API_SEMAPHORE_H


#include <pharos/declarations.h>
#include <pharos/kernel/semaphore/ceiling.h>
#include <pharos/kernel/semaphore/mutex.h>
#include <pharos/kernel/semaphore/counting.h>
#include <pharos/kernel/time/declarations.h>
#include <pharos/kernel/object/declarations.h>


/**
 * Obtain a semaphore with priority ceiling. A thread trying to get a semaphore with priority ceiling 
 * will always get it since the thread a thread holding the semaphore cannot be switched to another 
 * thread of lower priority since all blocking calls (other than obtaining other semaphores with 
 * priority ceiling) are denied
 * 
 * @param number ceiling semaphore number on the running partition
 * 
 * @return returns the result of the obtain procedure
 */
PharosSemCeilObtainR pharosSemCeilObtain(SemCeilNumber number , Timeout timeout);


/**
 * release a ceiling semaphore
 * 
 * @param id ceiling semaphore id
 *  
 * @return returns the result of the release procedure
 */
PharosSemCeilReleaseR pharosSemCeilRelease(SemCeilNumber id);


/**
 * Obtain a mutex semaphore without a timeout.
 * 
 * Since this is a NOT blocking call, it can be called by interrupts.
 * 
 * @param id mutex semaphore id
 * 
 * @return returns the result of the obtain procedure
 */
PharosSemMutexObtainR pharosSemMutexObtain(SemMutexNumber id);


/**
 * Obtain a mutex semaphore with a timeout.
 * 
 * Since this is a potentially blocking call, it will return an error if periodic or sporadic threads invoke it.
 * 
 * @param id mutex semaphore id
 * @param timeout timeout information. If set to PHAROS_TIMEOUT_ZERO (0x0) the call will not block and may return unavailable if the semaphore is hold by another thread. If set to PHAROS_TIMEOUT_INFINITE (0xffffffff) will bock forever
 * 
 * @return returns the result of the obtain procedure
 */
PharosSemMutexObtainTimeoutR pharosSemMutexObtainTimeout(SemMutexNumber id , Timeout timeout);


/**
 * release a mutex semaphore 
 * 
 * @param id mutex semaphore id
 * 
 * @return returns the result of the release procedure
 */
PharosSemMutexReleaseR pharosSemMutexRelease(SemMutexNumber id);


/**
 * Obtain a counting semaphore. If the counting semaphore internal count is 0 does not block and returns the corresponding error
 * 
 * @param id counting semaphore id
 * 
 * @return returns the result of the obtain procedure
 */
PharosSemCountObtainR pharosSemCountObtain(SemCountNumber id);


/**
 * Obtains a counting semaphore and waits if the semaphore is not available.
 * @param id counting semaphore id
 * @param timeout timeout information. If set to PHAROS_TIMEOUT_ZERO (0x0) the call will not block and may return unavailable if the semaphore is hold by another thread. If set to PHAROS_TIMEOUT_INFINITE (0xffffffff) will bock forever
 * 
 * @return returns the result of the obtain procedure
 */
PharosSemCountObtainTimeoutR pharosSemCountObtainTimeout(SemCountNumber id , Timeout timeout);


/**
 * release a counting semaphore 
 * 
 * @param id counting semaphore id
 * 
 * @return returns the result of the release procedure
 */
PharosSemCountReleaseR pharosSemCountRelease(SemCountNumber id);

#endif /* SEMAPHORE_H */
