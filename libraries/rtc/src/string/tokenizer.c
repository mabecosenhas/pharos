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


#include <pharos/rtc/rtc.h>
#include <pharos/rtc/rtcinline.h>
#include <pharos/kernel/memory/memory.h>
#include <pharos/hal/cpu/conf.h>

/**
 * Find the first char in "s" that is inside "delim"
 * 
 * @param s string to search in
 * @param delim list of chars to search for
 * 
 * @return returns the pointer to the first occurrence of a char in "delim" inside "s" or NULL if none is found
 */
static char *findFirstNonDelimeter(char *s , const char *delim);


char *findFirstNonDelimeter(char *s , const char *delim)
{
    const char *d;

    /* flag to indicate if we found a delim for each char */
    int flag;

    /* find the next char that is not a delimeter */
    for(; *s != '\0'; s++)
    {
        /* mark the flag as false */
        flag = 0;

        /* is *s equal to any of the chars in the delimeter? */
        for(d = delim; *d != '\0'; d++)
        {
            if(*s == *d)
            {
                flag = 1;
                break;
            }
        }

        /* if we could not find a delim for the current *s char */
        if(flag == 0)
        {
            /* then return the current position of s (first position without  */
            return s;
        }
    }

    return NULL;
}


char *rtcStringTokenizer(char **s , const char *delim)
{
    /* return value of the method (if found) */
    char *result = NULL;

    /* iterator for a delimeter */
    char *iterator;

    /* delimeter iterator */
    const char *d;


    /* if no string to search or no delimeter */
    if((s != NULL) && (*s != NULL) && (delim != NULL) && (*delim != '\0'))
    {
        /* find the first non delimeter char in s */
        result = findFirstNonDelimeter(*s , delim);

        /* if found a delimeter */
        if(result != NULL)
        {
            /* loop the following chars */
            for(iterator = result + 1; *iterator != '\0'; iterator++)
            {
                /* search to find the next delimeter */
                for(d = delim; *d != '\0'; d++)
                {
                    if(*iterator == *d)
                    {
                        /* found a delimeter */

                        /* stop the string */
                        *iterator = '\0';

                        /* update last pointer */
                        *s = iterator + 1;

                        /* return the last found result */
                        return result;
                    }
                }
            }

            /* if reached here it means that no delimeter was found */
            *s = NULL;
        }
    }

    /* empty string, delimeter, or could not find a char that is not a delimeter */
    return result;

}
