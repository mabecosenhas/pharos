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


#ifndef RTCSTRING_H
#define RTCSTRING_H

#include <pharos/rtc/rtc.h>


/**
 * Appends the src string to the dest string. The copy ends when src is '\0' or at most n bytes have been copied
 * 
 * This function is similar to C Standard Library "strncat" function, but returns a pointer to the address where the
 * strings where concatenated, instead of returning the destination address. Making it very similar to stpncpy but
 * without the overhead of written '\0' chars in the end of dst. If n is too big, this would take a waste of time.
 * 
 * This function can be used very easily to concatenate several strings. Take the example:
 * 
 * char string[100] = "My string";
 * 
 * string = rtcStringConcatN(string , "123" , 3);
 * string = rtcStringConcatN(string , "---" , 3);
 * string = rtcStringConcatN(string , "abc" , 3);
 * string = rtcStringConcatN(string , "456" , 3);
 * 
 * The result string will be "My string123---abc456".
 * 
 * @note The destination address MUST contain the space enough to copy n bytes, that is, the size of the destination 
 * array must be rtcStrlen(destination) + n + 1. The last "+1" corresponds to the termination '\0' of the string.
 * 
 * @param dest destination string
 * @param src source string
 * @param n maximum number of bytes to copy (plus one, corresponding to the termination '\0')
 * 
 * @return returns a pointer to where the src was copied into dest
 */
char *rtcStringConcatN(char *dest , const char *src , size_t n);


/**
 * Search for the specified char on the array
 * 
 * This function is similar to C Standard Library "strchr" function, but receives a "char" instead of an "int"
 * 
 * @note The array MUST contain the termination ('\0') char otherwise the method will continue to look for in the array.
 * 
 * @param array array to search for
 * @param c char to look for
 * 
 * @return returns the pointer to the location in the array where the first occurrence of "c" was found. Returns NULL
 * if the array does not contain the c char
 */
char *rtcStringSearch(const char *array , char c);


/**
 * Search for the specified char on the array, in reverse order.
 * 
 * This function is similar to C Standard Library "strrchr" function, but receives a "char" instead of an "int"
 * 
 * @note The array MUST contain the termination ('\0') char otherwise the method will continue to look for in the array.
 * 
 * @param array array to search for
 * @param c char to look for
 * 
 * @return returns the pointer to the location in the array where the first occurrence of "c" was found (in 
 * reverse order). Returns NULL if the array does not contain the c char
 */
char *rtcStringSearchReverse(const char *array , char c);


/**
 * Compares the two specified strings, with a maximum number of bytes to compare
 * 
 * This function is similar to C Standard Library "strncmp" function, but returns an int32_t instead of an int.
 * Comparison functions are changed to all return int32_t to make them portable across all systems.
 * 
 * @param s1 string one
 * @param s2 string two
 * @param n maximum number of bytes to compare
 * @return returns 0 if the strings are equal (until '\0'), smaller than zero if s1 < s2 and larger than zero if s1 > s2
 */
int32_t rtcStringCompareN(const char *s1 , const char *s2 , size_t n);


/**
 * Compares the two specified strings without a specified maximum amount of bytes to compare with.
 * Note that if the strings don't have a termination char (i.e. '\0'), then this function will continue to look in the 
 * memory from the next '\0' until they are different. This could cause an invalid memory exception.
 * 
 * This function is similar to C Standard Library "strcmp" function, but returns an int32_t instead of an int.
 * Comparison functions are changed to all return intptr_t to make them portable across all systems.
 * 
 * @param s1 string one
 * @param s2 string two
 *
 * @return returns 0 if the strings are equal (until '\0'), smaller than zero if s1 < s2 and larger than zero if s1 > s2
 */
intptr_t rtcStringCompare(const char *s1 , const char *s2);


/**
 * Searches each character in s for a match in accept. Returns the index of the first character in s that is not present 
 * in the accept string.
 * 
 * This function is similar to C Standard Library "strspn" function.
 * 
 * @param s string to look for
 * @param accept string with list of accepted chars
 * 
 * @return returns the index in s of the first character that is not present in the accept string
 */
size_t rtcStringSearchAccept(const char *s , const char *accept);


/**
 * Searches each character in s for a match in reject. Returns the index of the first character in s that is present 
 * in the reject string.
 * 
 * This function is similar to C Standard Library "strcspn" function.
 * 
 * @param s string to look for
 * @param reject string with list of rejected chars
 * 
 * @return returns the index in s of the first character that is present in the reject string
 */
size_t rtcStringSearchReject(const char *s , const char *reject);


/**
 * Calculates the length of the string (until it finds the '\0' char)
 * 
 * This function is similar to C Standard Library "strlen" function.
 * 
 * @note if the string does not contain the termination char '\0' then it can possibly end up searching memory areas 
 * that are not valid. It is recommended to use rtcStrnlen instead of this function.
 * 
 * @param s string to search
 * 
 * @return returns the number of non-termination chars ('\0') in string 
 */
size_t rtcStringLength(const char *s);


/**
 * Calculates the length of the string (until it finds the '\0' char) or max bytes have been reached.
 * 
 * This function is similar to C Standard Library "strlen" function.
 * 
 * @note if the string does not contain the termination char '\0' then it can possibly end up searching memory areas 
 * that are not valid. It is recommended to use rtcStrnlen instead of this function.
 * 
 * @param s string to search
 * @param max maximum number of bytes to search for
 * 
 * @return returns the number of non-termination chars ('\0') in string  or "max" if the string is larger than "max"
 */
INLINE size_t rtcStringLengthN(const char *s , size_t max);


/**
 * Same as printk but copies to the array instead of the outputChar function
 * 
 * This function is similar to C Standard Library "sprintf" function.
 *
 * @note: the destination string ("str") MUST have enough space to copy all of the content.
 * 
 * @param str destination string
 * @param format string to print
 * @param ... remaining arguments
 * 
 * @return returns the number of characters placed in the str
 */
size_t rtcStringPrint(char *str , const char *format , ...);


/**
 * Extract token from string. Note that the original string ("s") is changed.
 * 
 * This function is similar to C Standard Library "strtok_r" function, but with fewer parameters and easier to use.
 * 
 * The following examples shows how to use it:
 * 
 * char stringAux[] = "My string to test the tokenizer with. It contains spaces, dots, commas, etc....
 * s = stringAux;
 * 
 * token = rtcStringTokenizer(&s , " .,");
 * while(token != NULL)
 * {
 *    printk("Found <%s>\r\n" , token);
 *    token = rtcStringTokenizer(&s , " .,");
 * }
 * 
 * @param s address of the string. Used to store the state of the tokenizer.
 * @param delim string that separates the tokens
 * 
 * @return returns the pointer in s for the next token
 */
char *rtcStringTokenizer(char **s , const char *delim);



#endif /* RTCSTRING_H */
