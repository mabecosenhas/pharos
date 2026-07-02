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


#include <pharos/kernel/core/mpsc.h>
#include <pharos/hal/boardApi.h>


void pharosIMpscQueueInitialize(ptrPharosMpscQueue queue)
{
    /* initialize the head of the queue to point to the stub */
    queue->head = &queue->stub;

    /* initialize the tail of the queue to point to the stub */
    queue->tail = &queue->stub;

    /* initialize the stub to have no next element */
    queue->stub.next = NULL;
}


ptrPharosMpscNode pharosIMpscQueueGetMessage(ptrPharosMpscQueue queue , ptrPharosMpscNode node)
{
    /* change the active element. This is important because of the way the algorithm works.
     * For example, you only have one message on your HW queue. That means that the MPSC queue could end up with only
     * that MPSC node on the queue. Since the queue with one element is empty, and if you place the HW message queue 
     * again in the queue it would overload the next pointer. So we have to use another node: the queue stub */
    if(queue->tail == node)
    {
        /* then use the queue stub to insert the message */
        node = &queue->stub;
    }

    /* return the node */
    return node;
}


void pharosIMpscQueuePush(ptrPharosMpscQueue queue , ptrPharosMpscNode node)
{
    /* interrupt level */
    PharosInterruptLevel interrupt;

    /* previous node that was the head of the queue */
    ptrPharosMpscNode previous;


    /* reset the node next pointer (node will be at the end of the queue) */
    node->next = NULL;

    /* disable interrupts so we can make sure to finish the exchange immediately so that another core waiting to the 
     * exchange will wait a minimum amount of time. This is required in some CPUs (like ARM with its 
     * ldaxr and stlxr instructions or RISC-V) that have more than one assembly instruction per __atomic_exchange_n */
    pharosCpuDisableInterrupts(&interrupt);

    /* exchange the head of the queue to the node (head points to the last node on the queue) */
    previous = __atomic_exchange_n(&queue->head , node , __ATOMIC_ACQ_REL);

    /* update the previous head (last item on the queue) to point to new last item */
    previous->next = node;

    /* re-enable interrupts */
    pharosCpuEnableInterrupts(&interrupt);
}


ptrPharosMpscNode pharosIMpscQueuePop(ptrPharosMpscQueue queue)
{
    /* tail of the MPSC queue */
    ptrPharosMpscNode tail;

    /* next pointer */
    ptrPharosMpscNode next;

    /* interrupt level */
    PharosInterruptLevel interrupt;


    /* disable interrupts so we are sure that this is single-consumer */
    pharosCpuDisableInterrupts(&interrupt);

    /* get the current tail (first item placed on the queue) */
    tail = queue->tail;

    /* get the second node on the list */
    next = __atomic_load_n(&tail->next , __ATOMIC_ACQUIRE);

    /* if there is no second node that means that only the stub is there */
    if(next != NULL)
    {
        /* move to the next */
        queue->tail = next;
    }

    /* re-enable interrupts */
    pharosCpuEnableInterrupts(&interrupt);

    /* return the first item on the list (FIFO) or NULL if the queue is empty */
    return next;
}
