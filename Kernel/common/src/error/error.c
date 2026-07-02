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


#include <pharos/kernel/error/error.h>
#include <pharos/hal/hal.h>
#include <pharos/hal/board.h>
#include <pharos/kernel/partition/partition.h>
#include <pharos/kernel/core/core.h>
#include <pharos/kernel/core/coreinlines.h>
#include <pharos/kernel/partition/partition.h>
#include <pharos/kernel/core/coreconfinline.h>


void pharosSFatalErrorAnnounce(PharosFatalError error , const char_t *file , const char_t *func , uint32_t line)
{
    /* partition number iterator */
    PartitionNumber i;

    /* interrupt level */
    PharosInterruptLevel interrupt;

    /* partition pointer iterator */
    ptrPartition p;


    /* if the error is recognized as a fatal error */
    if(pharosFatalErrorHandler(error , file , func , line) == TRUE)
    {
        /* goes through every partition and invoke the fatal error */
        for(i = 0U; i < pharosICorePartitionTable()->size; i++)
        {
            p = &pharosICorePartitionTable()->partitions[i];

            /* if there is a fatal error handler */
            if(p->fatalErrorHandler != NULL)
            {
                /* disable interrupts */
                pharosCpuDisableInterrupts(&interrupt);

                /* call handler */
                p->fatalErrorHandler(error , file , func , line);

                /* re-enable interrupts*/
                pharosCpuEnableInterrupts(&interrupt);
            }
        }

        /* and shutdown the system */
        pharosSShutdown();
    }
    /* else, the announce error is NOT handled as a fatal error */
}


void pharosSErrorAnnounce(PharosError error , const uint8_t *interruptedInstruction)
{
    /* get the running partition */
    ptrPartition runningPartition = pharosIPartitionGetRunning();


    /* if there is no running partition */
    if(runningPartition == NULL)
    {
        /* invoke the application general error handler */
        pharosErrorHandler(error , interruptedInstruction);
    }
        /* if there is a running partition */
    else
    {
        /* if the partition has an error handler */
        if(runningPartition->errorHandler != NULL)
        {
            /* call handler */
            runningPartition->errorHandler(error , interruptedInstruction);
        }
    }
}


void pharosSThreadErrorAnnounce(ptrThreadInfo t , PharosThreadError e , const uint8_t *inst , const uint8_t *address)
{
    /* get the thread partition */
    ptrPartition p = t->partition;


    /* if there is an error handler */
    if(p->threadErrorHandler != NULL)
    {
        /* then use it to handle the error */
        p->threadErrorHandler(t->threadId , e , inst , address);
    }
}
