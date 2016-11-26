/* string.c */
/* Author: Azinum */
/* Date: 2016-11-09 */

#include <math.h>

#include "string.h"
#include "object.h"
#include "list.h"

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
    if (*string == '\0') {
        free(result);
        return NULL;
    }
    
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
    return from + rand() % (from - to);
}

/*
** create random char but skip certain characters
** example:
**    string_rand_char_skip('a', 'f', "bc");
** result can only be: (a|d|e|f)
*/
char string_rand_char_skip(unsigned char from, unsigned char to, const char* skip) {
    if (string_char_in_string(skip, from) || string_char_in_string(skip, to))
        return 0;
    
    unsigned long size = strlen(skip);
    if (size == 0)  /* nothing to exclude */
        return string_rand_char(from, to);
    
    int ret = from + rand() % ((from) - (to+1));
    
    for (int i = 0; i < size; i++) {
        if (ret == skip[i]) {
            if (ret + 1 <= to) {
                ret++;
                continue;
            }
            if (ret - 1 >= from) {
                ret--;
                continue;
            }
        }
    }
    
    return ret;
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

int string_char_in_string(const char* string, char ch) {
    unsigned long size = strlen(string);
    while (size--) {
        if (string[size] == ch)
            return 1;
    }
    return 0;
}


char* string_replace(char* string, char* a, char* b) {
    String output;
    list_init(&output);
    
    unsigned long size_string, size_old, size_new, where;
    
    size_string = strlen(string);
    size_old = strlen(a);
    size_new = strlen(b);
    where = string_item_in_string_index(string, a) - 1;
    
    for (unsigned long i = 0; i < where; i++) {
        list_push(&output, string[i]);
    }
    
    for (unsigned long i = 0; i < size_new; i++) {
        list_push(&output, b[i]);
    }
    
    for (unsigned long i = where + size_old; i < size_string; i++) {
        list_push(&output, string[i]);
    }
    
    list_push(&output, '\0');   /* null terminate string */
    return output.value;
}

/*
** return true|false for if item exists in source string
*/
unsigned char string_item_in_string(char* string, char* item) {
    unsigned long lookahead = strlen(item);
    unsigned long matchc = 0;   /* how many characters are matching */
    int item_index = 0;
    
    for (unsigned long i = 0; i < strlen(string); i++) {
        if (string[i] == item[item_index]) {    /* got the first character */
            matchc++;
            for (unsigned long j = 1; j < lookahead; j++) {
                item_index++;
                if (string[i + j] == item[item_index]) {
                    matchc++;
                }
            }
            if (matchc == lookahead) {  /* we have got a match */
                return 1;
            }
        }
    }
    return 0;
}

/*
** find item in string and return that index
** if function return 0 then that means that item in string was not found
*/
unsigned long string_item_in_string_index(char* string, char* item) {
    unsigned long lookahead = strlen(item);
    unsigned long matchc = 1;
    int item_index = 0;
    
    for (unsigned long i = 0; i < strlen(string); i++) {
        if (string[i] == item[item_index]) {
            for (unsigned long j = 1; j < lookahead; j++) {
                item_index++;
                if (string[i + j] == item[item_index]) {
                    matchc++;
                }
            }
            if (matchc == lookahead) {
                return 1 + i;
            }
        }
    }
    return 0;
}
