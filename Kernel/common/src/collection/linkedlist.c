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


/*
 * LinkedList.c
 */

#include <pharos/kernel/collection/linkedlist.h>


/**
 * remove a node from the list it is on by updating its previous and next nodes
 */
static void removeNode(ptrLinkedList list , ptrLinkedListNode node);


/**
 * Cast the node to the object using the list offset information
 * 
 * @param list linked list
 * @param node node to cast
 * 
 * @return returns the object that contains the node
 */
INLINE void *castToObject(ptrLinkedList list , ptrLinkedListNode node);


/**
 * Cast the object to the node using the list offset information
 * 
 * @param list linked list
 * @param object object to cast
 * 
 * @return returns the node inside the object
 */
INLINE ptrLinkedListNode castToNode(ptrLinkedList list , void *object);


void linkedListInitialize(const ptrLinkedList list , int16_t offset)
{
    /* no first element */
    list->first = NULL;

    /* no last element */
    list->last = NULL;

    /* no elements so size is 0 */
    list->size = 0U;

    /* set the node offset inside the objects */
    list->offset = offset;
}


INLINE void *castToObject(ptrLinkedList list , ptrLinkedListNode node)
{
    /* cast to a byte address so we can add to the address the offset location */
    uint8_t *address = (uint8_t *) node;

    
    /* subtract the offset */
    /* MISRA2004 17.4: pointer arithmetic is used to calculate the correct address of the object given its node */
    address -= list->offset;

    /* return the object address */
    return (void *) address;
}


INLINE ptrLinkedListNode castToNode(ptrLinkedList list , void *object)
{
    /* cast to a byte address so we can add to the address the offset location */
    uint8_t *address = (uint8_t *) object;

    
    /* add the offset */
    /* MISRA2004 11.4: pointer arithmetic is used to calculate the correct address of the node given the object */
    address += list->offset;

    /* return the node address */
    return (ptrLinkedListNode) address;
}


void linkedListAppendLast(ptrLinkedList list , void *object)
{
    /* save the current last */
    ptrLinkedListNode aux = list->last;

    /* calculate the node */
    ptrLinkedListNode node = castToNode(list , object);


    /* if the list has no first */
    if(list->first == NULL)
    {
        /* then this node will become the first */
        list->first = node;
    }

    /* update the last */
    list->last = node;

    /* update the new node pointers */
    node->previous = aux;
    node->next = NULL;

    /* update the previously last node pointers */
    if(aux != NULL)
    {
        aux->next = node;
    }

    /* update the size */
    list->size++;
}


void linkedListAppendFirst(ptrLinkedList list , void *object)
{
    /* get the current first node */
    ptrLinkedListNode aux = list->first;

    /* cast the new object to add to its node */
    ptrLinkedListNode node = castToNode(list , object);

    /* update the new first pointer */
    list->first = node;

    /* first item has no previous */
    node->previous = NULL;

    /* first item next is the previous first item */
    node->next = aux;

    /* update the previously first node pointers */
    if(aux != NULL)
    {
        /* update the previous first node */
        aux->previous = node;
    }

    /* update the size */
    list->size++;
}


bool linkedListInsert(ptrLinkedList list , void *object , uint32_t index)
{
    /* result of the method */
    bool result;

    /* index on the list */
    uint32_t i;

    /* cast the object to a node */
    ptrLinkedListNode node = castToNode(list , object);

    /* iterator though the list */
    ptrLinkedListNode iterator = list->first;

    /* auxiliary variable used to insert the node */
    ptrLinkedListNode aux;


    /* find the node to insert */
    for(i = 0U; i < index; i++)
    {
        /* if there is no item at the position "i" */
        if(iterator == NULL)
        {
            /* could not find desired position */
            result = FALSE;

            /* could not find the desired position */
            break;
        }

        /* check the next item */
        iterator = iterator->next;
    }

    /* if could find the given position */
    if(result == TRUE)
    {
        /* found position, update it */
        aux = iterator->next;
        node->next = aux;
        node->previous = iterator;

        iterator->next = node;

        if(aux != NULL)
        {
            aux->previous = node;
        }

        /* update the size */
        list->size++;

        /* return that could added the item on the list */
        result = TRUE;
    }
    else
    {
        /* could not add the item on the list */
        result = FALSE;
    }

    /* return that could (or not) add the item to the list */
    return result;
}


void *linkedListGetFirst(ptrLinkedList list)
{
    /* return value */
    void *result;


    /* if the list is empty */
    if(list->first == NULL)
    {
        /* return NULL */
        result = NULL;
    }
        /* if the list has at least a first element */
    else
    {
        /* cast the first node to the object */
        result = castToObject(list , list->first);
    }

    /* return the first element object or NULL if the list is empty */
    return result;
}


void linkedListClear(const ptrLinkedList list)
{
    /* reset the first pointer */
    list->first = NULL;

    /* reset the last pointer */
    list->last = NULL;

    /* reset the size */
    list->size = 0U;
}


void * linkedListGet(ptrLinkedList list , uint32_t index)
{
    /* return value */
    void *result;

    /* index on the list */
    uint32_t i;

    /* get the first node */
    ptrLinkedListNode iterator = list->first;


    /* find the node to insert */
    for(i = 0U; i < index; i++)
    {
        /* if there is no iterator at position i */
        if(iterator == NULL)
        {
            /* could not find desired position */
            result = NULL;

            /* stop searching */
            break;
        }

        /* get next node */
        iterator = iterator->next;
    }

    /* if found item at the desired index */
    if(result != NULL)
    {
        /* cast the node to an object */
        result = castToObject(list , iterator);
    }

    /* return the object or NULL if not found */
    return result;
}


void *linkedListNext(ptrLinkedList list , void *object)
{
    /* return value */
    void *result;

    /* cast the object to its node */
    ptrLinkedListNode node = castToNode(list , object);


    /* if the next does not exist */
    if(node->next == NULL)
    {
        /* result is NULL (cannot cast otherwise would give != NULL) */
        result = NULL;
    }
        /* if the next exists */
    else
    {
        /* can safely cast to object */
        result = castToObject(list , node->next);
    }

    /* return the object or NULL if none exists */
    return result;
}


void *linkedListPrevious(ptrLinkedList list , void *object)
{
    /* return value */
    void *result;

    /* cast the object to its node */
    ptrLinkedListNode node = castToNode(list , object);


    /* if the previous does not exist */
    if(node->previous == NULL)
    {
        /* result is NULL (cannot cast otherwise would give != NULL) */
        result = NULL;
    }
        /* if the previous exists */
    else
    {
        /* can safely cast to object */
        result = castToObject(list , node->previous);
    }
    return result;
}


bool linkedListHasNext(ptrLinkedList list , void *object)
{
    /* cast the object to its node */
    ptrLinkedListNode node = castToNode(list , object);

    /* return TRUE if the next is not null */
    return node->next == NULL ? FALSE : TRUE;
}


bool linkedListHasPrevious(ptrLinkedList list , void *object)
{
    /* cast the object to its node */
    ptrLinkedListNode node = castToNode(list , object);

    /* return TRUE if the previous is not null */
    return node->previous == NULL ? FALSE : TRUE;
}


void *linkedListRemove(const ptrLinkedList list , uint16_t index)
{
    /* iterator on the list */
    uint16_t i;

    /* get first item on the list */
    ptrLinkedListNode iterator = list->first;

    /* find the node to remove */
    for(i = 0U; i < index; i++)
    {
        if(iterator == NULL)
        {
            /* could not find desired position */
            return NULL;
        }

        /* get next node */
        iterator = iterator->next;
    }

    /* remove the node */
    removeNode(list , iterator);

    /* return the object */
    return castToObject(list , iterator);
}


void *linkedListRemoveFirst(ptrLinkedList list)
{
    /* object removed */
    void *result;

    /* get the first node */
    ptrLinkedListNode first = list->first;


    /* check if there is an object to remove */
    if(first != NULL)
    {
        /* remove the first node */
        removeNode(list , first);

        /* cast the first node to the object */
        result = castToObject(list , first);
    }
    else
    {
        /* there is no object to remove */
        result = NULL;
    }

    /* return the object removed (or NULL if none) */
    return result;
}


void linkedListRemoveObject(ptrLinkedList list , void *object)
{
    /* get the node */
    ptrLinkedListNode node = castToNode(list , object);

    /* remove the node */
    removeNode(list , node);
}


void linkedListSwitchLists(ptrLinkedList source , ptrLinkedList destination , void *object)
{
    /* get the node */
    ptrLinkedListNode node = castToNode(source , object);

    /* get the current first node */
    ptrLinkedListNode aux = destination->first;


    /* remove the node */
    removeNode(source , node);

    /* update the new first pointer */
    destination->first = node;

    /* first item has no previous */
    node->previous = NULL;

    /* first item next is the previous first item */
    node->next = aux;

    /* update the previously first node pointers */
    if(aux != NULL)
    {
        /* update the previous first node */
        aux->previous = node;
    }

    /* update the size */
    destination->size++;
}


void *linkedListLast(ptrLinkedList list)
{
    /* result of the method */
    void *result;


    /* if the list last does not exist */
    if(list->last == NULL)
    {
        /* then return invalid */
        result = NULL;
    }
    else
    {
        /* else get the cast to the object */
        result = castToObject(list , list->last);
    }

    /* and return the result */
    return result;
}


void *linkedListFirst(ptrLinkedList list)
{
    /* result of the method */
    void *result;


    /* if the list first does not exist */
    if(list->first == NULL)
    {
        /* then return invalid */
        result = NULL;
    }
    else
    {
        /* else get the cast to the object */
        result = castToObject(list , list->first);
    }

    /* and return the result */
    return result;
}


bool linkedListIsInside(const ptrLinkedList list , void *object)
{
    /* result of the method */
    bool result;

    /* get the node */
    ptrLinkedListNode node = castToNode(list , object);


    /* if the list is not empty */
    if(linkedListIsEmpty(list) == TRUE)
    {
        /* if the list is empty for sure the object is not inside it */
        result = FALSE;
    }
    else
    {
        /* if the node has not previous nor next */
        if((node->next == NULL) && (node->previous == NULL))
        {
            /* then the list is not empty but the object is not inserted */
            result = FALSE;
        }
        else
        {
            /* the object is inserted in the list somewhere */
            result = TRUE;
        }
    }

    /* return TRUE if the node is inside the list and FALSE otherwise */
    return result;
}


void removeNode(ptrLinkedList list , ptrLinkedListNode node)
{
    ptrLinkedListNode next , previous;

    next = node->next;
    previous = node->previous;

    /* if there is a previous */
    if(previous != NULL)
    {
        /* update the previous next */
        previous->next = next;
    }

    /* if there is a next */
    if(next != NULL)
    {
        /* update the next previous */
        next->previous = previous;
    }

    /* if the node to be removed is the list first node */
    if(list->first == node)
    {
        /* update the first node */
        list->first = node->next;
    }

    /* if the node to be removed is the list last node */
    if(list->last == node)
    {
        /* update the list last node */
        list->last = node->previous;
    }

    /* reset the node removed */
    node->next = NULL;
    node->previous = NULL;

    /* update the size (assumes the node is on the list) */
    list->size--;
}
