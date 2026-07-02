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
#include <pharos/kernel/scheduler/scheduler.h>
#include <pharos/kernel/thread/priority.h>
#include <pharos/kernel/core/core.h>
#include <pharos/kernel/core/coreconfinline.h>


/**
 * increments (by one) the index without reaching the module
 * 
 * @param index index to increment 
 * @param module maximum value (that cannot be reached)
 * 
 * @return returns the increment index within the module
 */
INLINE uint32_t incNext(uint32_t index , uint32_t module);


/**
 * Updates the running partition on the hierarchical scheduler
 * 
 * @param core running core
 * @param scheduler hierarchical scheduler
 * @param heir heir partition
 */
static void pharosIHierarchicalSchedulerUpdate(ptrCore core , ptrHierarchicalScheduler scheduler , ptrPartition heir);


INLINE uint32_t incNext(uint32_t index , uint32_t module)
{
    /* use an "if" since it is much faster than a "%" operation */

    /* check if it is bigger than the maximum */
    if(index >= module - 1)
    {
        /* return to 0 */
        index = 0;
    }
    else
    {
        /* increment the index */
        index++;
    }

    /* returned the increment index */
    return index;
}


ClockTick pharosISchedHierarchicalUpdate(ptrCore core , ptrHierarchicalScheduler scheduler , ClockTick currentTick)
{
    /* number of extra ticks to execute */
    ClockTick result;


    /* if the current tick is supposed to switch partition */
    if(currentTick == scheduler->maf.nextTick)
    {
        /* calculate the instant for the next partition scheduling */
        scheduler->maf.nextTick += scheduler->maf.duration[scheduler->maf.indexNext];

        /* update the running flat scheduler last processed tick until this tick 
         * (this tick was not processed by this scheduler) */
        core->runningFlatScheduler->lastProcessedTick = currentTick;

        /* update the running scheduler */
        pharosIHierarchicalSchedulerUpdate(core , scheduler , scheduler->maf.partitions[scheduler->maf.indexNext]);

        /* calculate the index of the next partition to be executed */
        scheduler->maf.indexNext = incNext(scheduler->maf.indexNext , scheduler->maf.size);

        /* calculate the number of ticks that this scheduler has to process to catch up */
        result = currentTick - core->runningFlatScheduler->lastProcessedTick;
    }
    else
    {
        /* only execute one tick (did not switch partitions) */
        result = 0;
    }

    /* return the number of clock ticks to process */
    return result;
}


void pharosIHierarchicalSchedulerUpdate(ptrCore core , ptrHierarchicalScheduler scheduler , ptrPartition heir)
{
    /* interrupt iterator */
    ptrPartitionIsrConf iterator;

    /* get the active scheduler*/
    ptrFlatScheduler runningFlat = core->runningFlatScheduler;

 
    /* if should accept only the running partition interrupts */
    if(scheduler->flags.acceptOnlyPartitionInterrupts == TRUE)
    {
        /* start iterating for all */
        ptrPartitionIsrConf iterator = linkedListFirst(&runningFlat->interruptsInstalled);

        /* loop through every installed interrupt on the running flat scheduler */
        while(iterator != NULL)
        {
            /* un-install the interrupt on the CPU (leave it on the flat scheduler) */
            pharosIInterruptRemoveInternal(iterator);

            /* go to next interrupt */
            iterator = linkedListNext(&runningFlat->interruptsInstalled , iterator);
        }
    }

    /* if there is no heir partition */
    if(heir == NULL)
    {
        /* that means that we have to schedule every other partition */
        core->runningFlatScheduler = &core->flatScheduler;
    }
    else
    {
        /* update the running core flat scheduler to the next partition to be executed */
        core->runningFlatScheduler = heir->scheduler;
    }

    /* update the active scheduler */
    runningFlat = core->runningFlatScheduler;

    /* if should accept only the running partition interrupts */
    if(scheduler->flags.acceptOnlyPartitionInterrupts == TRUE)
    {
        /* start iterating for all */
        iterator = linkedListFirst(&runningFlat->interruptsInstalled);

        /* loop through every installed interrupt on the heir flat scheduler */
        while(iterator != NULL)
        {
            /* install the interrupt directly on the CPU (no need to update the flat scheduler) */
            pharosIInterruptInstallInternal(iterator);

            /* go to next interrupt */
            iterator = linkedListNext(&runningFlat->interruptsInstalled , iterator);
        }
    }

    /* then a context switch is required */
    pharosIThreadContextSwitchNeeded(TRUE);
}


void pharosISchedHierarchicalInit(ptrCore core , ptrHierarchicalScheduler hierarchical)
{
    /* if there is no heir partition for the first time window */
    if(hierarchical->maf.partitions[0] == NULL)
    {
        /* that means that we have to schedule every other partition */
        core->runningFlatScheduler = &core->flatScheduler;
    }
    else
    {
        /* update the running core flat scheduler to the next partition to be executed */
        core->runningFlatScheduler = hierarchical->maf.partitions[0]->scheduler;
    }

    /* next index on the MAF to be executed */
    hierarchical->maf.indexNext = incNext(0 , hierarchical->maf.size);

    /* place the clock tick when a partition scheduling should occur */
    hierarchical->maf.nextTick = hierarchical->maf.duration[0];

    /* initialize the objects after MMU information has been set on the partitions */
    pharosIPartitionsObjectsInit(core);
}
