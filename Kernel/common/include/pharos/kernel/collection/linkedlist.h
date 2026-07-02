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


#ifndef PHAROS_LINKEDLIST_H
#define PHAROS_LINKEDLIST_H


#include <pharos/kernel/collection/declarations.h>

/**
 * Definition of structures
 */

/**
 * Simple linked list
 */
struct LinkedList
{
    /**
     * head node of the list
     */
    ptrLinkedListNode first;

    /**
     * last node of the list (used to append at the end very fast)
     */
    ptrLinkedListNode last;

    /**
     * number of elements of the list
     */
    uint32_t size;

    /**
     * offset of the node inside the structure
     */
    int16_t offset;
};

/**
 * node of the list
 */
struct LinkedListNode
{
    /**
     * next node
     */
    ptrLinkedListNode next;

    /**
     * previous node
     */
    ptrLinkedListNode previous;
};


/**
 * initialize a linked list
 */
void linkedListInitialize(const ptrLinkedList list , int16_t offset);


/**
 * Removes all elements from the list (resets the size to 0)
 * 
 * Note: does not make any modifications to the nodes on the list.
 * 
 * @param list
 */
void linkedListClear(const ptrLinkedList list);


/**
 * append a new object to the list
 */
void linkedListAppendLast(ptrLinkedList list , void *object);


/**
 * apppend an object to the first position of the list
 */
void linkedListAppendFirst(ptrLinkedList list , void *object);


/**
 * insert an object in the specified position of the list
 *
 * @return returns true if the insertion was possible and false otherwise
 */
bool linkedListInsert(ptrLinkedList list , void *object , uint32_t index);


/**
 * get the first element of the list
 */
void *linkedListGetFirst(ptrLinkedList list);


/**
 * gets the object of a list with the specified index
 */
void *linkedListGet(ptrLinkedList list , uint32_t index);


/**
 * get the next object on the list
 */
void * linkedListNext(ptrLinkedList list , void *object);


/**
 * get the previous object on the list
 */
void * linkedListPrevious(ptrLinkedList list , void *object);


/**
 * determines if the object on the list has a next node
 */
bool linkedListHasNext(ptrLinkedList list , void *object);


/**
 * determines if the object on the list has a previous node
 */
bool linkedListHasPrevious(ptrLinkedList list , void *object);


/**
 * remove a node with the specified position of the list
 */
void *linkedListRemove(const ptrLinkedList list , uint16_t index);


/**
 * remove a node from the list it is inserted into
 */
void linkedListRemoveObject(ptrLinkedList list , void *object);


/**
 * Switches the object from one list to the other
 * 
 * @param source list where the object currently is
 * @param destination list where the object will be placed to
 * @param object object to switch
 */
void linkedListSwitchLists(ptrLinkedList source , ptrLinkedList destination , void *object);


/**
 * remove the first node of the linked list. Assumes there is at least one element on the list.
 * 
 * @param list linked list
 * 
 * @return returns a pointer to the first object on the list (which was removed) or NULL if the list is empty
 */
void *linkedListRemoveFirst(ptrLinkedList list);


/**
 * get the last element of the list (NULL if none)
 */
void *linkedListLast(ptrLinkedList list);


/**
 * get the first element of the list (NULL if none)
 */
void *linkedListFirst(ptrLinkedList list);


/**
 * Determine if the object is inside the list 
 * 
 * @constraint the object if inside any list, must be inside the specified list
 * 
 * @param list list to detect
 * @param object object to check
 * 
 * @return returns TRUE if the object is inside the list and FALSE otherwise
 */
bool linkedListIsInside(const ptrLinkedList list , void *object);


/**
 * determine the list size
 * 
 * @param list list to check
 *
 * @return returns the size (number of elements) of the list
 */
INLINE uint32_t linkedListSize(const ptrLinkedList list)
{
    /* return the size */
    return list->size;
}


/**
 * determine if the list is empty
 * 
 * @param list list to check
 * 
 * @return returns TRUE if the list is empty and FALSE otherwise
 */
INLINE bool linkedListIsEmpty(ptrLinkedList list)
{
    /* check if the size is 0 and return TRUE if it is */
    return list->size == 0 ? TRUE : FALSE;
}


#endif /* LINKEDLIST_H */
