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


#include <pharos/kernel/collection/simplelist.h>
#include <pharos/hal/hal.h>


/**
 * Cast the node to the object using the list offset information
 * 
 * @param list simple list
 * @param node node to cast
 * 
 * @return returns the object that contains the node
 */
INLINE void *castToObject(ptrSimpleList list , ptrSimpleListNode node);


/**
 * Cast the object to the node using the list offset information
 * 
 * @param list linked list
 * @param object object to cast
 * 
 * @return returns the node inside the object
 */
INLINE ptrSimpleListNode castToNode(ptrSimpleList list , void *object);


/**
 * removes a specified object of a list
 */
bool simpleListRemove(ptrSimpleList list , void *object)
{
    /* return value */
    bool result;

    /* start on the first item on the list */
    ptrSimpleListNode iterator = list->first;

    /* cast the object to its node */
    ptrSimpleListNode node = castToNode(list , object);


    /* search for the node */
    if(iterator == node)
    {
        /* get the second item on the list */
        ptrSimpleListNode next = node->next;

        /* update the first position of the list */
        list->first = next;

        /* if there was a second position in the list (which is now the first) */
        if(next == NULL)
        {
            /* if there are no elements update the last item */
            list->last = NULL;
        }

        /* decrement the number of items on the list */
        list->size--;

        /* and return that we found the item */
        result = TRUE;
    }
    else
    {
        /* initialize the search with not found */
        result = FALSE;

        /* loop through the whole list */
        while(iterator != NULL)
        {
            /* if the next node is the one where are looking for */
            if(iterator->next == node)
            {
                /* found it */
                iterator->next = node->next;

                /* if it was the last item */
                if(node->next == NULL)
                {
                    /* update the last item */
                    list->last = iterator;
                }

                /* decrement the list size */
                list->size--;

                /* found it */
                result = TRUE;

                /* stop searching */
                break;
            }
            else
            {
                /* continue to next node */
                iterator = iterator->next;
            }
        }
    }

    /* return whether the item was found or not */
    return result;
}


INLINE void *castToObject(ptrSimpleList list , ptrSimpleListNode node)
{
    /* cast to a byte address so we can add to the address the offset location */
    uint8_t *address = (uint8_t *) node;

    /* subtract the offset */
    address -= list->offset;

    return (void *) address;
}


INLINE ptrSimpleListNode castToNode(ptrSimpleList list , void *object)
{
    /* cast to a byte address so we can add to the address the offset location */
    uint8_t *address = (uint8_t *) object;

    /* add the offset */
    address += list->offset;

    return (ptrSimpleListNode) (void *) address;
}


void simpleListAppendFirst(ptrSimpleList list , void *object)
{
    /* cast the object to the node */
    ptrSimpleListNode node = castToNode(list , object);

    /* get the current first and store it */
    ptrSimpleListNode oldFirst = list->first;


    /* if there was a first */
    if(oldFirst == NULL)
    {
        /* update the last item on the list if the list is empty */
        list->last = node;
    }

    /* put the node as the first node on the list */
    list->first = node;

    /* put the old first node as the next of the new first node */
    node->next = oldFirst;

    /* update the size */
    list->size++;
}


void simpleListAppendAll(ptrSimpleList list , uint32_t size , void *firstObject , void *lastObject)
{
    /* cast the object to the first node */
    ptrSimpleListNode firstNode = castToNode(list , firstObject);

    /* cast the object to the last node */
    ptrSimpleListNode lastNode = castToNode(list , lastObject);

    /* get the current first and store it */
    ptrSimpleListNode oldFirst = list->first;


    /* if there is no element on the list */
    if(oldFirst == NULL)
    {
        /* then update the first element */
        list->first = firstNode;
    }

    /* if there is a last element on the list */
    if(list->last != NULL)
    {
        /* update its next pointer */
        list->last->next = firstNode;
    }

    /* update the last item on the list */
    list->last = lastNode;

    /* update the size */
    list->size += size;
}


void simpleListAppendLast(ptrSimpleList list , void *object)
{
    /* get the node of the object */
    ptrSimpleListNode node = castToNode(list , object);

    /* get the first element of the list */
    ptrSimpleListNode oldFirst = list->first;

    /* if there is no element on the list */
    if(oldFirst == NULL)
    {
        /* then update the first element */
        list->first = node;
    }

    /* if there is a last element on the list */
    if(list->last != NULL)
    {
        /* update its next pointer */
        list->last->next = node;
    }

    /* update the last item on the list */
    list->last = node;

    /* set the last node next pointer to NULL (its the last item) */
    node->next = NULL;

    /* update the size */
    list->size++;
}


void *simpleListGetFirst(ptrSimpleList list)
{
    /* return value */
    void *result;


    /* if the list is empty */
    if(list->first == NULL)
    {
        /* return null */
        result = NULL;
    }
        /* if the list is non-empty */
    else
    {
        /* get the first node object */
        result = castToObject(list , list->first);
    }

    /* return the first node object or NULL if the list is empty */
    return result;
}


void *simpleListGetLast(ptrSimpleList list)
{
    /* return value */
    void *result;


    /* if the list is empty */
    if(list->last == NULL)
    {
        /* return null */
        result = NULL;
    }
        /* if the list is non-empty */
    else
    {
        /* get the last node object */
        result = castToObject(list , list->last);
    }

    /* return the last node object or NULL if the list is empty */
    return result;
}


void *simpleListGetNext(ptrSimpleList list , void *object)
{
    /* return value of the method */
    void *result;

    /* cast the object to the node */
    ptrSimpleListNode node = castToNode(list , object);

    /* get the next node */
    ptrSimpleListNode next = node->next;


    /* if the node is NULL */
    if(next == NULL)
    {
        /* no next */
        result = NULL;
    }
    else
    {
        /* cast it to the object */
        result = castToObject(list , next);
    }

    /* return the next object or NULL if none */
    return result;
}


void *simpleListGetAt(ptrSimpleList list , uint32_t index)
{
    /* result of the method */
    void *result = NULL;

    /* counter of the index */
    uint32_t i = 0U;

    /* iterator on the list*/
    ptrSimpleListNode iterator = list->first;


    /* if the list has the index */
    if(list->size > index)
    {
        /* loop until the end of the list */
        while(iterator != NULL)
        {
            /* and if found the index */
            if(i == index)
            {
                /* return the object at this index */
                result = castToObject(list , iterator);

                /* stop searching */
                break;
            }

            /* increment the counter */
            i++;

            /* go to next element on the list */
            iterator = iterator->next;
        }
    }

    /* should never come here */
    return result;
}


/**
 * removes the first object of a list
 */
void *simpleListRemoveFirst(ptrSimpleList list)
{
    /* pointer to the first object */
    void * result;

    /* get the first node */
    ptrSimpleListNode aux = list->first;


    /* if the first node exists */
    if(aux != NULL)
    {
        /* then the 2nd node */
        ptrSimpleListNode next = aux->next;

        /* will be the new first node */
        list->first = next;

        /* if there is no 2nd node */
        if(next == NULL)
        {
            /* then update the list last */
            list->last = NULL;
        }
        
        /* decrease the list size */
        list->size--;

        /* return the first element */
        result = castToObject(list , aux);
    }
    else
    {
        /* there are no elements on the list to remove */
        result = NULL;
    }

    return result;
}
