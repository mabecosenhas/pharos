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


#include <pharos/kernel/thread/thread.h>
#include <pharos/hal/boardApi.h>
#include <pharos/kernel/interrupt/interrupt.h>
#include <pharos/kernel/object/protection.h>
#include <pharos/kernel/object/object.h>
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/kernel/clock/clock.h>
#include <pharos/kernel/thread/rtthread.h>
#include <pharos/kernel/thread/dispatcher.h>
#include <pharos/kernel/scheduler/scheduler.h>
#include <pharos/kernel/partition/partition.h>
#include <pharos/kernel/channel/channel.h>


/**
 * Checks if the running partition has enough permissions to restart the specified partition
 * 
 * @param runningPartition running partition 
 * @param restartingPartition partition to restart 
 * 
 * @return returns TRUE if has permissions and FALSE otherwise
 */
static bool hasPermissions(ptrPartition runningPartition , PartitionNumber restartingPartition);


PharosPartitionRestartR pharosSPartitionRestart(PartitionNumber partition)
{
    /* result of this method */
    PharosPartitionRestartR result;

    /* get the running partition */
    ptrPartition runningPartition = pharosIPartitionGetRunning();

    /* get the partition to restart */
    ptrPartition p = pharosIPartitionGet(partition);


    /* if the partition to restart does not exist */
    if(p == NULL)
    {
        /* return invalid partition */
        result = PHAROS_PARTITION_RESTART_INVALID;
    }
        /* if the running partition does not have enough permissions to restart */
    else if(hasPermissions(runningPartition , partition) == FALSE)
    {
        /* then return not allowed */
        result = PHAROS_PARTITION_RESTART_NOT_ALLOWED;
    }
        /* else, we can proceed with the partition restart */
    else
    {
        /* send a signal to restart the running partition */
        pharosIPartitionRestartSignal(p);

        /* return success (only returns if restarting a partition different from the running partition) */
        result = PHAROS_PARTITION_RESTART_SUCCESS;
    }

    /* return success or the error */
    return result;
}


void pharosIPartitionRestartPending(void)
{
    /* get the partitions of the running core */
    ptrCore core = pharosICore();

    /* partition iterator */
    PartitionNumber iterator;

    /* partition pointer */
    ptrPartition p;


    /* for each partition of the running core */
    for(iterator = 0; iterator < core->partitions.size; iterator++)
    {
        /* get the partition */
        p = &core->partitions.partitions[iterator];

        /* if need to restart it */
        if(p->partitionFlags.readyRestart == TRUE)
        {
            /* then restart it */
            pharosIPartitionRestartLocal(p);
        }
    }
}


void pharosIPartitionRestartSignalOnPartitionCore(ptrPartition partition)
{
    /* interrupt level */
    PharosInterruptLevel level;


    /* disable interrupts so no one can change the list of free bulks */
    pharosCpuDisableInterrupts(&level);

    /* if the state is not already restarting */
    if(partition->state != PHAROS_PARTITION_STATE_RESTARTING)
    {
        /* change the partition state to restart */
        partition->state = PHAROS_PARTITION_STATE_RESTARTING;

        /* increment the restart number */
        partition->restartNumber++;

        /* halt any thread of this partition and 
         * stop threads from other partitions to accessing this partition (channels, hw messages, etc) */
        partition->partitionFlags.isInitialized = FALSE;

        /* re-enable interrupts */
        pharosCpuEnableInterrupts(&level);

        /* check if the partition can be restarted and signals, to the idle thread or restart thread, if it can */
        pharosIPartitionRestartCheck(partition);

        /* make sure a re-scheduling is made (the running thread could belong to the restarting partition) */
        pharosIThreadContextSwitchNeeded(TRUE);

        /* dispatch to other threads */
        pharosIDispatchThread();
    }
    else
    {
        /* re-enable interrupts */
        pharosCpuEnableInterrupts(&level);

        /* check if the partition can be restarted and signals it if it can */
        pharosIPartitionRestartCheck(partition);
    }
}


void pharosIPartitionRestartLocal(ptrPartition p)
{
    /* remove all the installed interrupts */
    pharosIInterruptRemoveAll(p , &p->interruptConfigurationTable);

    /* make ready any external futures that the partition is currently holding 
     * (before the channel bulks otherwise the futures of the bulks will not be reset) */
    pharosIPartitionFuturesReset(p);

    /* free all bulks currently held by the partition */
    pharosIPartitionChannelBulksReset(p);

    /* remove all threads from the scheduler ready list */
    pharosIPartitionResetThreads(p);

    /* reset all timers */
    pharosIPartitionTimersReset(p);

    /* re-initialize the future table of the partition being restarted */
    pharosIFutureTableReset(p , &p->objects.futureTable);

    /* lastly initialize the partition */
    pharosIPartitionReinit(p);
}


void pharosIPartitionRestartThreadBody(void)
{
    /* get the running thread */
    ptrThreadInfo runningThread = pharosISchedGetRunningThread();

    /* get the running partition */
    ptrPartition runningPartition = runningThread->partition;


    /* loop */
    while(TRUE)
    {
        /* signal to restart the partition */
        pharosIPartitionRestartLocal(runningPartition);

        /* remove this thread from the ready list (will not do anything if the runningThread is the idle thread) */
        pharosISchedRemoveReadyThread(runningThread);

        /* dispatch to another thread */
        pharosIDispatchThread();
    }
}


bool hasPermissions(ptrPartition runningPartition , PartitionNumber restartingPartition)
{
    /* result value of this method */
    bool result = FALSE;


    /* if restarting itself */
    if(runningPartition->id == restartingPartition)
    {
        /* then allowed it */
        result = TRUE;
    }

    /* if has a filter to allow/deny restarts */
    if(pharosVCoreTable.restartGrantHandler != NULL)
    {
        /* then call it (this can override the previous result) */
        result = pharosVCoreTable.restartGrantHandler(runningPartition->id , restartingPartition);
    }
    /* otherwise, does not have permission (already initialized the result to FALSE) */

    /* return TRUE if has permissions and FALSE otherwise */
    return result;
}


void pharosIPartitionRestartCheck(ptrPartition partition)
{
    /* if the partition has no threads in inter-partition calls (IP-calls or resources) */
    if(partition->interPartitionNestLevel == 0U)
    {
        /* if there is a restart thread for the partition */
        if(partition->restartThread != NULL)
        {
            /* activate the partition restart thread */
            pharosISchedAddReadyThread(&partition->restartThread->info);
        }
        else
        {
            /* then we can restart (the pharosIPartitionRestartPending will check this flag) */
            partition->partitionFlags.readyRestart = TRUE;
        }

        /* make sure a re-scheduling is made (the running thread could belong to the restarting partition) */
        pharosIThreadContextSwitchNeeded(TRUE);

        /* dispatch to other threads */
        pharosIDispatchThread();
    }
    /* else, the partition still has threads that are inside other partitions calls (filters/IP calls) */
}
