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


#include <pharos/declarations.h>
#include <pharos/rtc/rtc.h>
#include <pharos/rtc/rtcinline.h>
#include <pharos/kernel/memory/memory.h>
#include <pharos/hal/cpu/conf.h>
#include <pharos/rtc/rtcinternal.h>
#include <pharos/rtm/rtm.h>


static size_t printUnsignedNum(RtcOutputCharHandler handler , char *array , size_t count ,
                               unsigned long long int num , int32_t base , int32_t maxwidth , int32_t lead);


static size_t printSignedNum(RtcOutputCharHandler handler , char *array , size_t count ,
                             long long int num , bool isNegative , int32_t base , int32_t maxwidth , int32_t lead);


static size_t printDoubleF(RtcOutputCharHandler handler , char *array , size_t countTotal , double value ,
                           int64_t widthInteger , int64_t widthComma);


static size_t printDoubleE(RtcOutputCharHandler handler , char *array , size_t countTotal , double value ,
                           int64_t widthComma , char exponentChar);


static size_t printChar(RtcOutputCharHandler handler , char *array , size_t countTotal , char c);


static size_t printDoubleErrorCases(RtcOutputCharHandler handler , char *array , size_t countTotal , double value);


static size_t printFloatSign(RtcOutputCharHandler handler , char *array , size_t countTotal , double *value);


static size_t strPrintSpecial(RtcOutputCharHandler handler , char *array , const char **fmt , size_t count ,
                              va_list *ap);



/**
 * Normalizes the number. That is, makes 0.1 < *val < 1.0 and returns the exponent
 * 
 * @param val float number to normalize
 * 
 * @return returns the exponent (base 10)
 */
static int normalize(float64_t *val);


static const char numbers[] = "0123456789ABCDEF";


char *getNumber(const char *string , int64_t *result)
{
    *result = 0;

    /* get the following number (if one exists) */
    while((*string >= '0') && (*string <= '9'))
    {
        /* update the width */
        *result *= 10;
        *result += (*string - '0');

        /* search next char */
        string++;
    }

    return (char *) string;
}


long long int readSignedNumber(int32_t lflag , va_list *ap , bool *isNegative)
{
    long long int snumber = 0;

    if(lflag == 0)
    {
        snumber = (long long int) va_arg(*ap , int);
    }
    else if(lflag == 1)
    {
        snumber = (long long int) va_arg(*ap , long int);
    }
    else
    {
        snumber = (long long int) va_arg(*ap , long long int);
    }

    if(snumber < 0)
    {
        snumber = -snumber;
        *isNegative = TRUE;
    }

    return snumber;
}


unsigned long long int readUnsignedNumber(int32_t lflag , va_list *ap)
{
    unsigned long long int unumber = 0;

    /* get the number based on the number of "ll"s placed */
    if(lflag == 0)
    {
        unumber = (unsigned long long int) va_arg(*ap , unsigned int);
    }
    else if(lflag == 1)
    {
        unumber = (unsigned long long int) va_arg(*ap , unsigned long int);
    }
    else
    {
        unumber = (unsigned long long int) va_arg(*ap , unsigned long long int);
    }

    return unumber;
}


size_t printString(RtcOutputCharHandler handler , char *array , va_list *ap , size_t count)
{
    /* aux variable to store a string */
    char *str;

    /* get the string */
    str = va_arg(*ap , char *);

    /* check the special case of printing a NULL string */
    if(str == NULL)
    {
        /* print "null" */
        count = printChar(handler , array , count , 'n');
        count = printChar(handler , array , count , 'u');
        count = printChar(handler , array , count , 'l');
        count = printChar(handler , array , count , 'l');
    }
    else
    {
        /* until the string ends */
        for(; *str != '\0'; str++)
        {
            /* print its chars */
            count = printChar(handler , array , count , *str);
        }
    }

    return count;
}


PHAROS_OPTIMIZE_O0 size_t strPrintSpecial(RtcOutputCharHandler handler , char *array , const char **fmt ,
                                          size_t count , va_list *ap)
{
    /* @note: on arm-v8a-a53-rasp3-qemu-release this function fails the test 750 if the attribute -O0 is removed
     * (GCC 8.2.0). Reason still to figure out...maybe related to variable arguments */

    /* long flag */
    int32_t lflag = 0;

    /* lead is '0' or ' ' */
    int32_t lead = ' ';

    /* width of the integer part of a double */
    int64_t widthInteger = 0;

    /* width of the fractional part of a double */
    int64_t widthComma = 6;

    /* variable to store the unsigned number */
    unsigned long long int unumber;

    /* variable to store the signed number */
    long long int snumber;

    /* flag that indicates if the number is negative */
    bool isNegative = FALSE;


    /* if starting with a lead of zeros (e.g. %04d of 98 is "0098") */
    if(**fmt == '0')
    {
        /* store that state */
        lead = '0';

        /* search next char */
        (*fmt)++;
    }

    /* get the width (if one exists) */
    *fmt = getNumber(*fmt , &widthInteger);

    /* search for a dot */
    if(**fmt == '.')
    {
        /* search next char */
        (*fmt)++;

        /* get the following number */
        *fmt = getNumber(*fmt , &widthComma);
    }


    /* if the number is a 64 bit variable */
    if(**fmt == 'l')
    {
        /* update the long flag */
        lflag = 1;

        /* search next char */
        (*fmt)++;

        /* if found a second "l" */
        if(**fmt == 'l')
        {
            /* update the long flag */
            lflag = 2;

            /* search next char */
            (*fmt)++;
        }
    }

    /* check the special character */
    switch(**fmt)
    {
        /* use unsigned base 8 */
    case 'o':
    case 'O':

        /* read the unsigned number */
        unumber = readUnsignedNumber(lflag , ap);
        count = printUnsignedNum(handler , array , count , unumber , 8 , widthInteger , lead);
        
        /* finished processing the special char */
        break;

        /* use signed base 10 */
    case 'i':
    case 'I':
    case 'd':
    case 'D':

        /* read the signed number */
        snumber = readSignedNumber(lflag , ap , &isNegative);
        
        /* print the signed number */
        count = printSignedNum(handler , array , count , snumber , isNegative , 10 , widthInteger , lead);
        
        /* finished processing the special char */
        break;

        /* use unsigned base 10 */
    case 'u':
    case 'U':

        /* read the unsigned number */
        unumber = readUnsignedNumber(lflag , ap);
        
        /* print the unsigned number */
        count = printUnsignedNum(handler , array , count , unumber , 10 , widthInteger , lead);
        
        /* finished processing the special char */
        break;

        /* use unsigned base 16 */
    case 'x':

        /* read the unsigned number */
        unumber = readUnsignedNumber(lflag , ap);
        
        /* print the unsigned number */
        count = printUnsignedNum(handler , array , count , unumber , 16 , widthInteger , lead);
        
        /* finished processing the special char */
        break;

        /* use unsigned base 16 */
    case 'p':

        /* read the unsigned number */
        unumber = readUnsignedNumber(lflag , ap);
        
        /* print the unsigned number with base 16 */
        count = printUnsignedNum(handler , array , count , unumber , 16 , widthInteger , lead);
        
        /* finished processing the special char */
        break;

        /* use string */
    case 's':

        /* print the string */
        count = printString(handler , array , ap , count);

        /* finished processing the special char */
        break;

        /* use char */
    case 'c':
        count = printChar(handler , array , count , va_arg(*ap , int));

        /* finished processing the special char */
        break;

        /* use double */
    case 'f':
        
        /* print the double number "normally" */
        count = printDoubleF(handler , array , count , va_arg(*ap , double) , widthInteger , widthComma);
        
        /* finished processing the special char */
        break;

        /* use double with exponent marker 'e' */
    case 'e':

        /* print the double number with an exponent marker 'e' */
        count = printDoubleE(handler , array , count , va_arg(*ap , double) , widthComma , 'e');
        
        /* finished processing the special char */
        break;

        /* use double with exponent marker 'E' */
    case 'E':

        /* print the double number with an exponent marker 'E' */
        count = printDoubleE(handler , array , count , va_arg(*ap , double) , widthComma , 'E');
        
        /* finished processing the special char */
        break;

        /* use char (could be, for example, another '%' in which case we just print it) */
    default:
        count = printChar(handler , array , count , **fmt);

        /* finished processing the special char */
        break;
        
    } /* end of switch*/


    /* return the array index */
    return count;
}


size_t rtcPrintkInternal(RtcOutputCharHandler handler , char *array , const char *fmt , va_list ap)
{
    /* number of bytes written so far */
    size_t count = 0;


    /* for every char */
    for(; *fmt != '\0'; fmt++)
    {
        /* search for special character '%' */
        if(*fmt == '%')
        {
            /* increment the iterator */
            fmt++;

            /* print the number/string/whatever */
            count = strPrintSpecial(handler , array , &fmt , count , &ap);
        }
        else
        {
            /* print the normal char */
            count = printChar(handler , array , count , *fmt);
        }
    }

    /* also print the '\0' (at least to the array) */
    count = printChar(handler , array , count , *fmt);

    /* return the number of chars written */
    return count;
}


size_t printSignedNum(RtcOutputCharHandler handler , char *array , size_t countTotal , long long int positive ,
                      bool isNegative , int32_t base ,
                      int32_t maxwidth , int32_t lead)
{
    int64_t n;
    int32_t count = 1;
    int32_t digit;
    uint64_t baseIterator = 1;
    int32_t countLeading = maxwidth;

    /* check the sign */
    if(isNegative == TRUE)
    {
        /* decrease the number of leading spaces */
        if(countLeading != 0)
        {
            countLeading--;
        }
    }

    /* first count the number of chars that it takes to write the num */
    n = positive / base;
    while(n > 0)
    {
        count++;
        n = n / base;
        baseIterator *= base;
    }

    /* decrease the number of leading spaces by the number of digits required to print the number */
    if(countLeading != 0)
    {
        countLeading -= count;
    }

    /* if the start with leading zeros */
    if((lead == '0') && (isNegative == TRUE))
    {
        /* print "-" before the leading */
        countTotal = printChar(handler , array , countTotal , '-');
    }

    /* write the lead spaces (' ' or '0') */
    for(n = 0; n < countLeading; n++)
    {
        /* print the lead char */
        countTotal = printChar(handler , array , countTotal , lead);
    }

    /* if starting with leading spaces */
    if((lead == ' ') && (isNegative == TRUE))
    {
        /* print "-" after the leading */
        countTotal = printChar(handler , array , countTotal , '-');
    }

    /* loop for all the digits required to be printed */
    while(count > 0)
    {
        /* if not the last count */
        if(count != 1)
        {
            /* calculate the digit (base iterator is != 0) */
            digit = positive / (baseIterator);
        }
        else
        {
            /* calculate the digit (base iterator is == 0) */
            digit = positive;
        }

        /* print the digit */
        countTotal = printChar(handler , array , countTotal , numbers[digit]);

        /* decrease the number to print next */
        positive -= digit * baseIterator;

        /* decrease the base */
        baseIterator /= base;

        /* decrease the number of digits left to print */
        count--;
    }

    /* return the total number of chars written */
    return countTotal;
}


size_t printUnsignedNum(RtcOutputCharHandler handler , char *array , size_t countTotal , unsigned long long int num ,
                        int32_t base , int32_t maxwidth , int32_t lead)
{
    int64_t n;
    int32_t count = 1;
    int32_t digit;
    uint64_t baseIterator = 1;
    int32_t countLeading = maxwidth;


    /* first count the number of chars that it takes to write the num */
    n = num / base;
    while(n > 0)
    {
        count++;
        n = n / base;
        baseIterator *= base;
    }

    /* decrease the number of leading spaces by the number of digits required to print the number */
    if(countLeading != 0)
    {
        countLeading -= count;
    }

    /* write the lead spaces (' ' or '0') */
    for(n = 0; n < countLeading; n++)
    {
        /* print the lead char */
        countTotal = printChar(handler , array , countTotal , lead);
    }

    /* loop for all the digits required to be printed */
    while(count > 0)
    {
        /* if not the last count */
        if(count != 1)
        {
            /* calculate the digit (base iterator is != 0) */
            digit = num / (baseIterator);
        }
        else
        {
            /* calculate the digit (base iterator is == 0) */
            digit = num;
        }

        /* print the digit */
        countTotal = printChar(handler , array , countTotal , numbers[digit]);

        /* decrease the number to print next */
        num -= digit * baseIterator;

        /* decrease the base */
        baseIterator /= base;

        /* decrease the number of digits left to print */
        count--;
    }

    /* return the total number of chars written */
    return countTotal;
}


static size_t printChar(RtcOutputCharHandler handler , char *array , size_t countTotal , char c)
{
    /* if there is a handler to print */
    if(handler != NULL)
    {
        /* print it to the handler */
        handler(c);
    }

    /* if there is an array to print */
    if(array != NULL)
    {
        /* print to the array */
        array [countTotal] = c;
    }

    /* return the total number of chars printed */
    return countTotal + 1;
}


size_t printDoubleErrorCases(RtcOutputCharHandler handler , char *array , size_t countTotal , double value)
{
    /* if value is NaN */
    if(rtmIsNanf64(value) != 0)
    {
        /* write NaN */
        countTotal = printChar(handler , array , countTotal , 'N');
        countTotal = printChar(handler , array , countTotal , 'a');
        countTotal = printChar(handler , array , countTotal , 'N');
    }

    /* if value is +Inf or -Inf */
    if(rtmIsInfinitef64(value) != 0)
    {
        /* check the sign */
        if(value < 0)
        {
            /* write '-' */
            countTotal = printChar(handler , array , countTotal , '-');
        }
        else
        {
            /* write '+' */
            countTotal = printChar(handler , array , countTotal , '+');
        }

        /* write Inf */
        countTotal = printChar(handler , array , countTotal , 'I');
        countTotal = printChar(handler , array , countTotal , 'n');
        countTotal = printChar(handler , array , countTotal , 'f');
    }

    /* return the original count */
    return countTotal;
}


size_t printDoubleE(RtcOutputCharHandler handler , char *array , size_t countTotal , double value ,
                    int64_t widthComma , char exponentChar)
{
    int32_t exponent = 0;
    int places = 0;
    size_t mycount;
    static const char numbers[] = "01234567899";
    int32_t aux;

    mycount = printDoubleErrorCases(handler , array , countTotal , value);

    if(mycount != countTotal)
    {
        return mycount;
    }

    /* print the sign */
    countTotal = printFloatSign(handler , array , countTotal , &value);

    /* normalize the value (between 0.1 and 1) */
    exponent = normalize(&value);

    aux = value * 10.0;
    if(aux > 9)
    {
        value /= 10.0;
        exponent++;
        aux = value * 10.0;
    }

    /* write the integer part */
    countTotal = printChar(handler , array , countTotal , numbers[aux]);

    /* write the integer/fractional divider (in our case we use the '.', but in some places it can use the ',' */
    countTotal = printChar(handler , array , countTotal , '.');

    /* write the fractional part */

    value = value * 10.0 - (int) (value * 10.0);

    /* decrease the exponent */
    exponent--;

    while(places < widthComma)
    {
        int digit = value * 10.0;
        countTotal = printChar(handler , array , countTotal , numbers[digit]);

        value = value * 10.0 - digit;
        places++;
    }

    /* write the exponent char ('e' or 'E') */
    countTotal = printChar(handler , array , countTotal , exponentChar);


    if(exponent > 0)
    {
        countTotal = printChar(handler , array , countTotal , '+');
    }

    else
    {
        countTotal = printChar(handler , array , countTotal , '-');
        exponent = -exponent;
    }

    /* write the exponent with at least two digits, leading with 0 */
    countTotal = printSignedNum(handler , array , countTotal , exponent , FALSE , 10 , 2 , '0');


    return countTotal;
}


size_t printFloatSign(RtcOutputCharHandler handler , char *array , size_t countTotal , double *value)
{
    if(*value < 0.0)
    {
        countTotal = printChar(handler , array , countTotal , '-');
        *value = -(*value);
    }

    return countTotal;
}


size_t printDoubleF(RtcOutputCharHandler handler , char *array , size_t countTotal , double value ,
                    int64_t widthInteger , int64_t widthComma)
{
    int exponent = 0;
    int places = 0;
    size_t mycount;

    mycount = printDoubleErrorCases(handler , array , countTotal , value);

    if(mycount != countTotal)
    {
        return mycount;
    }

    /* print the sign */
    countTotal = printFloatSign(handler , array , countTotal , &value);

    /* normalize the value (between 0.1 and 1) */
    exponent = normalize(&value);

    /* write the integer part */
    while(widthInteger > 0 || (exponent > 0))
    {
        int digit = value * 10;
        countTotal = printChar(handler , array , countTotal , '0' + digit);

        value = value * 10 - digit;
        places++;
        exponent--;
        widthInteger--;
    }

    /* if did not wrote anything on the integer part */
    if(places == 0)
    {
        /* then at least write a 0 */
        countTotal = printChar(handler , array , countTotal , '0');
        places++;
    }

    /* write the dot */
    countTotal = printChar(handler , array , countTotal , '.');

    /* reset the number of places (to count the number of decimal digits written) */
    places = 0;

    while((exponent < 0) && (places < widthComma))
    {
        countTotal = printChar(handler , array , countTotal , '0');

        exponent++;
        places++;
    }

    while(places < widthComma)
    {
        int digit = value * 10.0;
        countTotal = printChar(handler , array , countTotal , '0' + digit);

        value = value * 10.0 - digit;
        places++;
    }

    return countTotal;
}


static int normalize(double *val)
{
    /* result */
    int exponent = 0;

    /* copy of the value */
    double value = *val;


    /* if the val is really just 0.0 */
    if(value == 0.0)
    {
        /* then do nothing */
        return 0;
    }

    /* divide value by 10 until it is smaller than one */
    while(value >= 1.0)
    {
        /* divide by 10 */
        value /= 10.0;

        /* increase the exponent */
        exponent++;
    }

    /* multiply by 10 until it is larger than 0.1 */
    while(value < 0.1)
    {
        /* multiply by 10 */
        value *= 10.0;

        /* decrease the exponent */
        exponent--;
    }

    /* at this point value is between 0.1 and 1 and the exponent has the value 
     * by which multiplied it gives the original value */

    /* store the value again */
    *val = value;

    /* return the exponent */
    return exponent;
}
