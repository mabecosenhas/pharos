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


#include <pharos/kernel/channel/channel.h>
#include <pharos/kernel/channel/channelbulkkernel.h>
#include <pharos/channel.h>
#include <pharos/kernel/scheduler/scheduler.h>
#include <pharos/kernel/thread/dispatcher.h>
#include <pharos/kernel/thread/sporadicthread.h>
#include <pharos/kernel/object/object.h>
#include <pharos/kernel/channel/channelbulk.h>
#include <pharos/kernel/object/protection.h>
#include <pharos/kernel/interrupt/interrupt.h>
#include <pharos/hal/boardApi.h>
#include <pharos/kernel/partition/partition.h>
#include <pharos/kernel/core/coreinlines.h>
#include <pharos/kernel/core/coreconfinline.h>


void pharosIChannelsInitializeMinimal(ptrPartition p)
{
    /* channel iterator */
    ChannelNumber i;

    /* get the table */
    ptrChannelTable table = &p->objects.channelTable;


    /* for each channel */
    for(i = 0U; i < table->size; i++)
    {
        /* get the channel */
        ptrChannel channel = &table->channels[i];

        /* initialize the owner */
        channel->owner = p;

        /* set the channel partition number */
        channel->id.partition = p->id;

        /* set the channel number */
        channel->id.channel = i;

        /* initialize the tree map */
        treeMapInitialize(&channel->messages , (ComparatorMethod) pharosPriorityComparator , OFFSETOF(ChannelBulkKernel , treeNode));

        /* initialize the map of blocked threads on the channel (needed to be initialized here since sporadic threads 
         * from other cores could try to wait for the channel) */
        pharosIThreadQueueInitialize(&channel->blockedThreads , channel->flags.isFifo);

        /* channel is set to not initialized */
        channel->flags.isInitialized = FALSE;
    }
}


void pharosIChannelsInitialize(ptrPartition p , const ptrChannelTable table)
{
    /* channel iterator */
    ChannelNumber i;


    /* this method could be invoked by a partition restart. In this case we have to re-initialize the blockedThreads 
     * map by removing all threads, leaving only the sporadic threads of other cores */

    /* for each channel */
    for(i = 0U; i < table->size; i++)
    {
        /* get the channel */
        ptrChannel channel = &table->channels[i];

        /* init minimum priority */
        channel->minimumPriority = PHAROS_CHANNEL_PRIORITY_MINIMUM_ALL;

        /* remove all threads from the queue except for sporadic threads from other partitions */
        pharosIThreadQueueRemoveAllThreads(&channel->blockedThreads);

        /* channel is now initialized */
        channel->flags.isInitialized = TRUE;
    }
}


ptrChannel pharosIChannelGet(ptrChannelId id)
{
    /* result of the method */
    ptrChannel result;

    /* get the desired partition */
    ptrPartition p = pharosIPartitionGet(id->partition);


    /* check if the partition exists */
    if(p != NULL)
    {
        /* get the table of channels of the channel environment */
        ptrChannelTable table = &p->objects.channelTable;

        /* if the channel number is too high */
        if((table == NULL) || (id->channel >= table->size))
        {
            /* return invalid */
            result = NULL;
        }
            /* if the number is OK */
        else
        {
            /* return the pointer to the queue */
            result = &table->channels[id->channel];
        }
    }
        /* partition is invalid */
    else
    {
        /* return invalid */
        result = NULL;
    }

    /* return the result */
    return result;
}


ptrChannel pharosIChannelGetLocal(ChannelNumber number)
{
    /* result of the method */
    ptrChannel result;

    /* get the table of channels in the current environment */
    ptrChannelTable table = &pharosIPartitionRunObjectTable()->channelTable;


    /* if the channel number is too high */
    if(number >= table->size)
    {
        /* return invalid */
        result = NULL;
    }
        /* if the number is OK */
    else
    {
        /* return the pointer to the channel */
        result = &table->channels[number];
    }

    /* return the result */
    return result;
}


ptrChannelBulkKernel pharosIChannelGetHighestPriorityBulk(ptrChannel channel , ptrPharosPriority priority)
{
    /* highest priority message on the queue */
    ptrChannelBulkKernel result;

    /* aux bulk */
    ptrChannelBulkKernel aux;


    /* remove the highest priority of the messages (as the lowest value) */
    result = treeMapRemoveFirst(&channel->messages);

    /* if there is another bulk with the same priority */
    if(result->nextPriority != NULL)
    {
        /* get the next element */
        aux = result->nextPriority;

        /* copy the last pointer */
        aux->lastPriority = result->lastPriority;

        /* place the next one again in the map */
        treeMapPut(&channel->messages , &aux->priority , aux);
    }

    /* if the user requested to know the priority of the message */
    if(priority != NULL)
    {
        /* then set it */
        *priority = result->priority;
    }

    /* return the highest priority bulk */
    return result;
}


void pharosIChannelBulkPut(ptrChannel channel , ptrChannelBulkKernel bulk)
{
    /* aux bulk */
    ptrChannelBulkKernel aux;


    /* search a bulk with the same priority */
    aux = treeMapGet(&channel->messages , &bulk->priority);

    /* the bulk will be added to the end of the list */
    bulk->nextPriority = NULL;

    /* if there is another bulk with the same priority */
    if(aux != NULL)
    {
        /* if there are already two (or more) elements on the list */
        if(aux->lastPriority != NULL)
        {
            /* update the last one to point to the new bulk */
            aux->lastPriority->nextPriority = bulk;
        }
            /* list has only one element */
        else
        {
            /* update the next */
            aux->nextPriority = bulk;
        }

        /* update the last pointer */
        aux->lastPriority = bulk;
    }
    else
    {
        /* place the bulk on the list */
        treeMapPut(&channel->messages , &bulk->priority , bulk);

        /* there are no other bulks with the same priority */
        bulk->lastPriority = NULL;
    }
}
