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


#ifndef PHAROS_SIMPLELIST_H
#define PHAROS_SIMPLELIST_H


#include <pharos/kernel/collection/declarations.h>

/**
 * Simple list node
 */
struct SimpleListNode
{
    ptrSimpleListNode next;
};

/**
 * Simple list
 */
struct SimpleList
{
    /**
     * first node on the list
     */
    ptrSimpleListNode first;

    /**
     * last node
     */
    ptrSimpleListNode last;

    /**
     * number of elements of the list
     */
    uint32_t size;

    /**
     * offset used to cast to the object [byte]
     */
    int16_t offset;
};






/**
 * Append an object to the first position of the list
 * 
 * @param list list where to append the object
 * @param object object to append to the list
 */
void simpleListAppendFirst(ptrSimpleList list , void *object);


/**
 * Append the set of objects to the last position of the list, assuming that the object has correct pointers to next
 * This takes O(1) complexity
 * 
 * @param list list where to append the object
 * @param size number of objects in the list
 * @param firstObject first object to append to the list
 * @param lastObject last object to append to the list
 */
void simpleListAppendAll(ptrSimpleList list , uint32_t size , void *firstObject , void *lastObject);


/**
 * Append an object to the last position of the list
 * 
 * @param list list where to append the object
 * @param object object to append to the list
 */
void simpleListAppendLast(ptrSimpleList list , void *object);


/**
 * gets the first object of a list
 * 
 * @param list list where to get the first object
 * @return returns the first object on the list or NULL if there is none
 */
void *simpleListGetFirst(ptrSimpleList list);

/**
 * gets the last object of a list
 * 
 * @param list list where to get the first object
 * @return returns the last object on the list or NULL if there is none
 */
void *simpleListGetLast(ptrSimpleList list);


/**
 * get the next object of an object
 */
void *simpleListGetNext(ptrSimpleList list , void *object);


/**
 * get the specified element at the given index
 */
void *simpleListGetAt(ptrSimpleList list , uint32_t index);


/**
 * removes a specified object of a list
 */
bool simpleListRemove(ptrSimpleList list , void *object);


/**
 * remove the first object of a list
 */
void *simpleListRemoveFirst(ptrSimpleList list);


/**
 * Initialize the simple list 
 * 
 * @param list list to initialize
 * @param offset offset (in bytes) of the node on the object
 */
INLINE void simpleListInitialize(const ptrSimpleList list , int16_t offset)
{
    /* initialize with no first (list is empty) */
    list->first = NULL;

    /* initialize with no last (list is empty) */
    list->last = NULL;

    /* initialize with no size (list is empty) */
    list->size = 0U;

    /* initialize the offset */
    list->offset = offset;
}


/**
 * Clears the simple list (removes all objects from the list)
 * 
 * @param list list to clear
 */
INLINE void simpleListClear(ptrSimpleList list)
{
    /* initialize with no first (list is empty) */
    list->first = NULL;

    /* initialize with no last (list is empty) */
    list->last = NULL;

    /* initialize with no size (list is empty) */
    list->size = 0U;
}


/**
 * determine the number of elements of a list
 * 
 * @param list simple list
 * @return returns the number of elements on the list
 */
INLINE uint32_t simpleListSize(ptrSimpleList list)
{
    /* return the list size */
    return list->size;
}


/**
 * determines if the list is empty
 * 
 * @param list list to analyze
 * 
 * @return returns TRUE if the list is empty and FALSE otherwise
 */
INLINE bool simpleListIsEmpty(ptrSimpleList list)
{
    /* return TRUE if the size is 0 and FALSE otherwise */
    return list->size == 0U ? TRUE : FALSE;
}
#endif /* SIMPLELIST_H */
