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


#ifndef RTCIO_H
#define RTCIO_H


typedef void (*RtcOutputCharHandler)(char c);



/**
 * prints to the output, using function "handler".
 * 
 * See printk so see the format of the function.
 * 
 * @param handler function to print each byte
 * @param fmt arguments (see printk function)
 * @param ... list of arguments (see printk function)
 */
void rtcPrint(RtcOutputCharHandler handler , const char *fmt , ...) __attribute__((format(printf , 2 , 3)));


/**
 * Same as rtcPrintk but uses the Pharos outputChar function.
 * 
 * This function is very similar to printf. Consider the following example:
 * 
 * printk("Writting some numbers to uart 0x%lx %f %.9e" , 0x1122334455667788UL , 12.312 , 1.33333e-32);
 * 
 * The result will be "Writting some numbers to uart 0x1122334455667788 12.311999 1.333330000e-32".
 * 
 * @note not all printf "special chars" (like "%g") are supported. But for debugging the current implementation is 
 * useful. If you have more requirements ask Pharos team, otherwise it will not be updated (any time soon at least).
 * 
 * @param fmt string to print
 * @param ... remaining arguments
 */
void printk(const char *fmt , ...) __attribute__((format(printf , 1 , 2)));


/**
 * Same as printk. This function follows the naming convention of the RTC library, whereas printk does not 
 * (for commodity)
 * 
 * @param fmt string to print
 * @param ... remaining arguments
 */
void rtcPrink(const char *fmt , ...) __attribute__((format(printf , 1 , 2)));


#endif /* RTCIO_H */
