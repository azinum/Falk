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


char string_rand_char(unsigned char from, unsigned char to) {
    return from + rand() % to;
}

char* string_transform(char* string, int flag) {
    unsigned long size = strlen(string);
    if (size < 1)
        return NULL;
    char* output = newx(char, size);
    output[size] = '\0';    /* null-terminate string */
    
    switch (flag) {
        case STR_TRANSF_LOWERCASE: {
            for (int i = 0; string[i] != '\0'; i++) {
                char ch = string[i];
                if (ch >= 'A' && ch <= 'Z') {
                    output[i] = ch ^ 0x20;
                    continue;
                }
                output[i] = ch;
            }
        }
            break;
            
        case STR_TRANSF_UPPERCASE: {
            for (int i = 0; string[i] != '\0'; i++) {
                char ch = string[i];
                if (ch >= 'a' && ch <= 'z') {
                    output[i] = ch ^ 0x20;
                    continue;
                }
                output[i] = ch;
            }
        }
            break;
            
        case STR_TRANSF_CAPITALIZE: {
            char ch;
            for (int i = 0; string[i] != '\0'; i++) {
                ch = string[i];
                output[i] = ch;
                if (ch == ' ') {
                    ch = string[++i];
                    if (ch >= 'a' && ch <= 'z') {
                        output[i] = ch ^ 0x20;
                    }
                }
            }
            ch = string[0];
            if (ch >= 'a' && ch <= 'z') {
                output[0] = ch ^ 0x20;
            }
        }
            break;
        
        default:
            break;
    }
    
    return output;
}
