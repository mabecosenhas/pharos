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


#include <pharos/kernel/collection/doublelinkedsimplelist.h>


INLINE void *castToObject(ptrDoubleLinkedSimpleListNode node , int16_t offset)
{
    /* cast to a byte address so we can add to the address the offset location */
    uint8_t *address = (uint8_t *) node;


    /* subtract the offset */
    /* MISRA2004 17.4: pointer arithmetic is used to calculate the correct address of the object given its node */
    address -= offset;

    /* return the object address */
    return (void *) address;
}


INLINE ptrDoubleLinkedSimpleListNode castToNode(void *object , int16_t offset)
{
    /* cast to a byte address so we can add to the address the offset location */
    uint8_t *address = (uint8_t *) object;


    /* add the offset */
    /* MISRA2004 11.4: pointer arithmetic is used to calculate the correct address of the node given the object */
    address += offset;

    /* return the node address */
    return (ptrDoubleLinkedSimpleListNode) address;
}


void doubleLinkedSimpleListAdd(void *existingElement , void *newElement , int16_t offset)
{
    /* cast the element to a node */
    ptrDoubleLinkedSimpleListNode existingNode = castToNode(existingElement , offset);

    /* cast the element to a node */
    ptrDoubleLinkedSimpleListNode newNode = castToNode(newElement , offset);

    /* get the next element (if there is a next) */
    ptrDoubleLinkedSimpleListNode next = existingNode->next;


    /* update the new element next */
    newNode->next = next;

    /* if there is a next */
    if(next != NULL)
    {
        /* then update its previous */
        next->previous = newNode;
    }

    /* update the existing node next */
    existingNode->next = newNode;

    /* set the previous pointer of the new node */
    newNode->previous = existingNode;
}


void *doubleLinkedSimpleListRemove(void *elementToRemove , int16_t offset)
{
    /* return value */
    void *result = NULL;
    
    /* cast the element to a node */
    ptrDoubleLinkedSimpleListNode node = castToNode(elementToRemove , offset);

    /* get the next node (if any) */
    ptrDoubleLinkedSimpleListNode next = node->next;

    /* get the previous node (if any) */
    ptrDoubleLinkedSimpleListNode previous = node->previous;


    /* if there is a next node */
    if(next != NULL)
    {
        /* then update its previous */
        next->previous = previous;
        
        /* then we have at least this element */
        result= castToObject(next , offset);
    }

    /* if there is a previous */
    if(previous != NULL)
    {
        /* then update its next */
        previous->next = next;
        
        /* then we have at least this element */
        result= castToObject(previous , offset);
    }

    /* reset the node */
    node->next = NULL;
    node->previous = NULL;
    
    /* return the previous, next or NULL if there are no other nodes on the list */
    return result;
}


void doubleLinkedSimpleListInit(void *element , int16_t offset)
{
    /* cast the element to a node */
    ptrDoubleLinkedSimpleListNode node = castToNode(element , offset);

    node->next = NULL;
    node->previous = NULL;
}


void *doubleLinkedSimpleListNext(void *element , int16_t offset)
{
    /* return value */
    void *result;

    /* cast the element to a node */
    ptrDoubleLinkedSimpleListNode node = castToNode(element , offset);


    /* if there is a next */
    if(node->next != NULL)
    {
        /* then cast the next to an object and return it */
        result = castToObject(node->next , offset);
    }
    else
    {
        /* there is no next */
        result = NULL;
    }

    /* return the calculated value */
    return result;
}
