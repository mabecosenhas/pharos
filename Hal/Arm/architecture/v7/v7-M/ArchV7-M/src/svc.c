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


#include <pharos/hal/v7m/nvic.h>
#include <pharos/hal/v7m/conf.h>
#include <pharos/hal/v7m/scb.h>
#include <pharos/hal/v7m/misc.h>
#include <pharos/hal/cpu/context.h>
#include <pharos/kernel/core/core.h>
#include <pharos/kernel/core/syscall.h>
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/kernel/core/coreinlines.h>
#include <pharos/kernel/thread/dispatcher.h>
#include <pharos/buffer.h>



typedef uint32_t(*SyscallEnded)(uint8_t *address , CpuModeChangeContext context);


void pharosCpuPendableHandler(void)
{
    /* current stack pointer */
    ptrPharosCpuExceptionFrame ef;

    /* running thread */
    ptrThreadInfo running = pharosISchedGetRunningThread();


    /* check if the current thread is on the process of doing a dispatch*/
    if(running->cpuContext.pendableDone == FALSE)
    {
        /* it is not, but now will */
        running->cpuContext.pendableDone = TRUE;

        /* get the previous stack */
        ef = pharosCpuPspGet();

        /* save the previous stack */
        running->cpuContext.previousPendableStack = (uint32_t) ef;

        /* check if we were running (before the exception) inside the kernel */
        if(running->cpuContext.insideKernel == FALSE)
        {
            /* switch to the kernel stack */
            ef = (ptrPharosCpuExceptionFrame) running->cpuContext.currentKernelStack;
        }

        /* reserve space for the exception frame */
        ef--;

        /* add privileges to thread mode (when we return) */
        pharosCpuControlAddPriv();

        /* leave to the function pharosCpuArmv7mDispatch when this exception ends */
        ef->register_pc = (uint16_t *) ((uint32_t) pharosCpuArmv7mDispatch & ~((uint32_t) 1));

        /* maintain the thumb mode when this exception leaves */
        ef->register_xpsr = 0x01000000U;

        /* when the interrupt leaves, switch to the new stack */
        pharosCpuPspSet((uint32_t) ef);
    }
    /* else, the thread was already doing a dispatch so we won't do anything else and let it finish 
     * otherwise it could happen that the thread stack is always decreasing (e.g. a very fast clock tick) and
     * will soon run out of stack */

    /* clear the pendable interrupt */
    PHAROS_CPU_SCB->interruptControl = PHAROS_CPU_SCB_ICSR_PENDSVCLEAR;

    /* when returning from this function, the CPU will go to the pharosCpuArmv7mDispatch function, 
     * with privileges and in kernel stack */
}


void pharosCpuSyscallHandler(void)
{
    /* number of the syscall invoked */
    uint8_t syscallNumber;

    /* current stack */
    ptrPharosCpuExceptionFrame ef;

    /* new stack pointer */
    uint8_t *newStack;

    /* new stack */
    ptrPharosCpuExceptionFrame newEf;

    /* running thread */
    ptrThreadInfo running = pharosISchedGetRunningThread();


    /* disable dispatching inside this routine */
    pharosVDispatchLevel++;

    /* get the thread mode stack pointer */
    ef = pharosCpuPspGet();

    /* calculate the system call number */
    syscallNumber = (*(ef->register_pc - 1)) & 0xFF;

    /* check if a return from direct I/O */
    if(syscallNumber == PHAROS_SYSCALL_RESTORE_TLB)
    {
        /* get the address we that triggered the fault */
        uint32_t returnAddress = (*((uint32_t *) ef->register_pc));


        /* place the next instruction to be executed */
        ef->register_pc = (uint16_t *) (returnAddress & ~((uint32_t) 1));

        /* not inside the kernel anymore */
        running->cpuContext.insideKernel = FALSE;

        /* check if the running partition is running in supervisor mode */
        if(pharosIPartitionHasSupervisor() == FALSE)
        {
            /* if not, then remove privileges to thread mode. When we return, we will run in user mode */
            pharosCpuControlRemovePriv();
        }
    }
        /* check if it is a "standard" system call */
    else if(syscallNumber <= PHAROS_SYSCALL_API_LAST_CALL)
    {
        /* first check if the thread was overloading the stack. The running stack MUST be inside the application stack.
         * This is NOT a memory error, if it was then it would call the pharosMpuHandler. The running partition has 
         * permissions to access the "ef" address, but the "ef" address is not inside the stack, so it could be corrupting
         * the stack of other threads inside the same partition (or other global variables of the partition) */
        if(pharosIStackIsAddressInsideRunningStack(running , (uint8_t *) ef) == FALSE)
        {
            /* it was not, raise the error */
            pharosIAnnounceInvalidStackError(running , (uint8_t *) ef->register_pc , (uint8_t *) ef);
        }
        else
        {
            /* set that the running thread is now inside the kernel */
            running->cpuContext.insideKernel = TRUE;

            /* save the current user stack */
            running->cpuContext.previousSvcStack = (uint32_t) ef;

            /* switch to the kernel stack */
            newStack = running->cpuContext.currentKernelStack;

            /* check that we switching to the thread kernel stack, not to the application stack */
            PHAROS_ASSERT(pharosIStackIsAddressInside(&running->applicationStack , newStack) == FALSE ,
                          "Running in thread %p (thread number = %d , partition %d) inside user stack %p-%p and new stack = %p. Was executing address %p" ,
                          running , running->threadId.number , running->partition->id ,
                          running->applicationStack.stack ,
                          running->applicationStack.stack + running->applicationStack.size ,
                          newStack , ef->register_pc);

            /* reserve space for the stack needed for the syscall */
            newStack = newStack - pharosVCpuSyscallStackSize[syscallNumber];

            /* copy the stack to the kernel stack */
            memcpy(newStack , ef + 1 , pharosVCpuSyscallStackSize[syscallNumber]);

            /* reserve space for the exception frame */
            newEf = (ptrPharosCpuExceptionFrame) (newStack - sizeof (PharosCpuExceptionFrame));

            /* check that we switching to the thread kernel stack, not to the application stack */
            PHAROS_ASSERT(pharosIStackIsAddressInside(&running->applicationStack , (uint8_t *) newEf) == FALSE ,
                          "Running in thread %p (thread number = %d , partition %d) inside user stack %p-%p and new stack = %p. Was executing address %p" ,
                          running , running->threadId.number , running->partition->id ,
                          running->applicationStack.stack ,
                          running->applicationStack.stack + running->applicationStack.size ,
                          newStack , ef->register_pc);

            /* copy the argument registers (r0 to r3) to the new stack so when we leave the exception they will be restored */
            newEf->register_r0 = ef->register_r0;
            newEf->register_r1 = ef->register_r1;
            newEf->register_r2 = ef->register_r2;
            newEf->register_r3 = ef->register_r3;

            /* when we leave the syscallHandler, we will go to the correct syscall function */
            newEf->register_pc = (uint16_t *) ((uint32_t) pharosCpuArmv7mSyscall & ~((uint32_t) 1));

            /* maintain the thumb mode */
            newEf->register_xpsr = 0x01000000U;

            /* place in r12 the pointer to the function to be called */
            newEf->register_r12 = (uint32_t) pharosVSystemCalls[syscallNumber];

            /* when the interrupt leaves, switch to the kernel stack */
            pharosCpuPspSet((uint32_t) newEf);

            /* check that we switching to the thread kernel stack, not to the application stack */
            PHAROS_ASSERT(pharosIStackIsAddressInside(&running->kernelStack , (uint8_t *) newEf) == TRUE ,
                          "Running in thread %p executing syscall %d (thread number = %d , partition %d) inside user stack %p-%p and new ef = %p. Was executing address %p" ,
                          running , syscallNumber , running->threadId.number , running->partition->id ,
                          running->applicationStack.stack ,
                          running->applicationStack.stack + running->applicationStack.size ,
                          newStack , ef->register_pc);

            /* add privileges to thread mode. When we return, we will run inside the  kernel */
            pharosCpuControlAddPriv();
        }
        /* return to the syscall function with kernel stack and with privileges */
    }
        /* check if it is an "end" system call */
    else if(syscallNumber <= PHAROS_SYSCALL_LAST_CALL)
    {
        /* check if the call was made from the correct address */
        if(((SyscallEnded) pharosVSystemCalls[syscallNumber])((uint8_t *) (ef->register_pc) , pharosCpuControlGet()) == 0)
        {
            /* call ended, so we are now running inside kernel mode */
            running->cpuContext.insideKernel = TRUE;

            /* add privileges to thread mode. When we return, we will run inside the  kernel */
            pharosCpuControlAddPriv();

            /* and don't do anything else */
        }
        else
        {
            /* it was not, raise the error */
            pharosErrorAnnounce(PHAROS_ERR_INVALID_SYSCALL , (uint8_t *) ef->register_pc);

            /* and don't do anything else */
        }
    }
        /* check if it is a return from dispatch (issued by a pendable exception) */
    else if(syscallNumber == 0xFEU)
    {
        /* syscall is 0xFE, a return from pharosCpuArmv7mDispatch */

        /* check if syscall 0xfe was made from inside the kernel */
        if(pharosCpuControlIsUnpriv() == FALSE)
        {
            ptrPharosCpuExceptionFrame e = (ptrPharosCpuExceptionFrame) running->cpuContext.previousPendableStack;

            /* maintain the thumb mode */
            e->register_xpsr |= 0x01000000U;

            /* reset the pendable flag */
            running->cpuContext.pendableDone = FALSE;

            /* when we leave the exception, restore the original stack */
            pharosCpuPspSet(running->cpuContext.previousPendableStack);

            /* remove privileges to thread mode. When we return, we will run in user mode */
            if((running->cpuContext.insideKernel == FALSE) && (pharosIPartitionHasSupervisor() == FALSE))
            {
                pharosCpuControlRemovePriv();
            }
        }
        /* else, syscall 0xfe was invoked outside the kernel, reject it by doing nothing */
    }
        /* check if is a return from a "standard" syscall */
    else if(syscallNumber == 0xFFU)
    {
        /* syscall is 0xFF, a return from a syscall */

        /* check if syscall 0xff was made from inside the kernel */
        if(pharosCpuControlIsUnpriv() == FALSE)
        {
            /* get the pointer to the new stack (which is the original stack before the system call has been performed) */
            newEf = (ptrPharosCpuExceptionFrame) running->cpuContext.previousSvcStack;

            /* check that the thread is going to run on the application stack */
            PHAROS_ASSERT((pharosIStackIsAddressInside(&running->applicationStack , (uint8_t *) newEf) == TRUE) ||
                          ((running->sharedAccessStack != NULL) && (pharosIStackIsAddressInside(&running->sharedAccessStack->stack , (uint8_t *) newEf) == TRUE)) ,
                          "Running in thread %p (thread number = %d , partition %d) and should be (but is not) inside user stack %p-%p and ef = %p. Kernel stack is %p-%p. Was executing address %p" ,
                          running , running->threadId.number , running->partition->id ,
                          running->applicationStack.stack ,
                          running->applicationStack.stack + running->applicationStack.size ,
                          newEf ,
                          running->kernelStack.stack ,
                          running->kernelStack.stack + running->kernelStack.size ,
                          ef->register_pc);

            /* check that we running on the thread kernel stack, not to the application stack */
            PHAROS_ASSERT(pharosIStackIsAddressInside(&running->applicationStack , (uint8_t *) ef) == FALSE ,
                          "Running in thread %p (thread number = %d , partition %d) inside user stack %p-%p and new stack = %p. Was executing address %p" ,
                          running , running->threadId.number , running->partition->id ,
                          running->applicationStack.stack ,
                          running->applicationStack.stack + running->applicationStack.size ,
                          ef , ef->register_pc);

            /* place in the r0 the return value */
            newEf->register_r0 = ef->register_r0;

            /* place in the r1 the return value */
            newEf->register_r1 = ef->register_r1;

            /* maintain the thumb mode */
            newEf->register_xpsr |= 0x01000000U;

            /* has left the kernel */
            running->cpuContext.insideKernel = FALSE;

            /* check if the running partition is running in supervisor mode */
            if(pharosIPartitionHasSupervisor() == FALSE)
            {
                /* if not, then remove privileges to thread mode. When we return, we will run in user mode */
                pharosCpuControlRemovePriv();
            }

            /* when we leave the exception, restore the original stack */
            pharosCpuPspSet(running->cpuContext.previousSvcStack);
        }
        /* else, syscall 0xff was invoked outside the kernel, reject it by doing nothing */
    }
        /* else, is an invalid system call */
    else
    {
        /* invalid syscall, raise the error */
        pharosErrorAnnounce(PHAROS_ERR_INVALID_SYSCALL , (uint8_t *) ef->register_pc);

        /* and don't do anything else */
    }

    /* enable dispatching again */
    pharosVDispatchLevel--;

    /* if thread dispatch is enabled, running the outermost interrupt and a thread context switch is needed */
    if((pharosVDispatchLevel == 0U) && (pharosVInterruptNestingLevel == 0U) && (pharosINeedsThreadContextSwitch() == TRUE))
    {
        /* then generate a pendable interrupt (it will perform the context switch) */
        PHAROS_CPU_SCB->interruptControl = PHAROS_CPU_SCB_ICSR_PENDSVSET;
    }
}
