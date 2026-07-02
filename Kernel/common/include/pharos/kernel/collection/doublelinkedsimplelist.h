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


#ifndef PHAROS_KERNEL_COLLECTION_DOUBLELINKEDSIMPLELIST_H
#define PHAROS_KERNEL_COLLECTION_DOUBLELINKEDSIMPLELIST_H


#include <pharos/kernel/collection/declarations.h>

/**
 * Double simple linked list without a header, just has nodes
 */
struct DoubleLinkedSimpleListNode
{
    /**
     * head node of the list
     */
    ptrDoubleLinkedSimpleListNode next;

    /**
     * last node of the list (used to append at the end very fast)
     */
    ptrDoubleLinkedSimpleListNode previous;
};


/**
 * Adds an object to a double linked simple list
 * 
 * @param existingElement already existing element in the list
 * @param newElement new element to add
 * @param offset offset of the DoubleLinkedSimpleListNode in the object
 */
void doubleLinkedSimpleListAdd(void *existingElement , void *newElement , int16_t offset);


/**
 * Remove an element from the simple list 
 * 
 * @param elementToRemove
 * @param offset offset of the DoubleLinkedSimpleListNode in the object
 * 
 * @return returns an existing element to the list or NULL if the list is now empty
 */
void *doubleLinkedSimpleListRemove(void *elementToRemove , int16_t offset);


/**
 * Initializes an element of the list
 * 
 * @param element element to initialize 
 * @param offset offset of the DoubleLinkedSimpleListNode in the object
 */
void doubleLinkedSimpleListInit(void *element , int16_t offset);


/**
 * Iterate to the next element on the list
 * 
 * @param element existing element
 * @param offset offset of the DoubleLinkedSimpleListNode in the object
 * 
 * @return returns the next element on the list or NULL if there is none
 */
void *doubleLinkedSimpleListNext(void *element , int16_t offset);


#endif /* PHAROS_KERNEL_COLLECTION_DOUBLELINKEDSIMPLELIST_H */
