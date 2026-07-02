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


#ifndef PHAROS_MALLOC_H
#define PHAROS_MALLOC_H


/**
 * Allocates memory from the partition memory area
 * 
 * @param size amount of bytes to allocate
 * 
 * @return returns NULL if it was not possible to allocate the desired memory or the address to the allocated memory otherwise
 */
void *pharosMalloc(uint64_t size);


/**
 * Frees the allocated memory
 * 
 * @param pointer pointer to the allocated memory. If the pointer is different from something returned by malloc, 
 * nothing will be done and the function will return 0
 * 
 * @return returns the amount of bytes free. If 0, then nothing was done (invalid pointer)
 */
uint64_t pharosFree(void *pointer);



/* PHAROS_MALLOC_H */
#endif 
