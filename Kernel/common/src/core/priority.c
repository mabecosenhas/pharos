/****************************************************************************
 * Pharos - A Real-Time Secure Operating System                             *
 * Copyright 2019 Pedro Macara and Filipe Monteiro                          *
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

#include <pharos/declarations.h>


intptr_t pharosPriorityComparator(const ptrPharosPriority pa , const ptrPharosPriority pb)
{
    /* return value */
    intptr_t result;


    /* check if a < b */
    if((*pa) < (*pb))
    {
        /* return negative */
        result = -1;
    }
        /* check if a > b */
    else if((*pa)>(*pb))
    {
        /* return positive  */
        return 1;
    }
        /* else, then a == b */
    else
    {
        /* return 0 */
        result = 0;
    }

    /* return positive, negative or 0 if a > b, a < b or a == b (respectively) */
    return result;
}
