/* string.c */
/* Author: Azinum */
/* Date: 2016-11-09 */


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
