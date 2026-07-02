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


#ifndef PHAROS_SEMAPHORE_DECLARATIONS_H
#define PHAROS_SEMAPHORE_DECLARATIONS_H


#include <pharos/hal/hal.h>
#include <pharos/kernel/object/declarations.h>

typedef struct SemaphoreCeiling SemaphoreCeiling , *ptrSemaphoreCeiling;

typedef struct SemaphoreMutex SemaphoreMutex , *ptrSemaphoreMutex;

typedef struct SemaphoreCounting SemaphoreCounting , *ptrSemaphoreCounting;

typedef struct SemaphoreCeilingTable SemaphoreCeilingTable , *ptrSemaphoreTable;

typedef struct SemaphoreMutexTable SemaphoreMutexTable , *ptrSemaphoreMutexTable;

typedef struct SemaphoreCountingTable SemaphoreCountingTable , *ptrSemaphoreCountingTable;



typedef ObjectNumber SemCeilNumber;
typedef ObjectNumber SemMutexNumber;
typedef ObjectNumber SemCountNumber;

typedef struct ThreadChain ThreadChain , *ptrThreadChain;

#endif /* DECLARATIONS_H */
