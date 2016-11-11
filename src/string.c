/* string.c */
/* Author: Azinum */
/* Date: 2016-11-09 */

#include <math.h>

#include "string.h"
#include "object.h"

/*
** random string with readable characters
*/
char* string_random(unsigned int length) {
    if (length == 0)
        return NULL;
    
    char* string = newx(char, length + 1);
    
    for (int i = 0; i < length; i++) {
        string[i] = 126 - (rand() % 65);
    }
    
    string[length] = '\0';
    
    return string;
}

/*
** create a string within a certain range of characters
*/
char* string_random_range(unsigned int length, int min, int max) {
    if (length == 0)
        return NULL;
    
    char* string = newx(char, length);
    
    for (int i = 0; i < length; i++) {
        string[i] = min + (rand() % ((max + 1) - min));
    }
    
    string[length] = '\0';
    
    return string;
}

char* string_to_upper(char* string) {
    char* result = newx(char, strlen(string));
    if (*string == '\0')
        return NULL;
    
    for (int i = 0; string[i] != '\0'; i++) {
        char ch = string[i];
        if (ch >= 'a' && ch <= 'z') {
            result[i] = ch ^ 0x20;
            continue;
        }
        result[i] = ch;
    }
    return result;
}

char* string_to_lower(char* string) {
    char* result = newx(char, strlen(string));
    if (*string == '\0')
        return NULL;
    
    for (int i = 0; string[i] != '\0'; i++) {
        char ch = string[i];
        if (ch >= 'A' && ch <= 'Z') {
            result[i] = ch ^ 0x20;
            continue;
        }
        result[i] = ch;
    }
    return result;
}





char* string_int2string(int number) {
    int size = (int)floor(log(number));
    char* output = newx(char, size);
    output[size] = '\0';
    snprintf(output, size, "%i", number);
    return output;
}