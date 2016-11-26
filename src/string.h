/* string.h */
/* Author: Azinum */
/* Date: 2016-11-09 */


#ifndef string_h
#define string_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

enum String_transform_flags {
    STR_TRANSF_UPPERCASE,
    STR_TRANSF_LOWERCASE,
    STR_TRANSF_CAPITALIZE
};

char* string_random(unsigned int length);

char* string_random_range(unsigned int length, int min, int max);

char* string_to_upper(char* string);

char* string_to_lower(char* string);

char* string_int2string(int number);

char string_rand_char(unsigned char from, unsigned char to);

char string_rand_char_skip(unsigned char from, unsigned char to, const char* skip);

char* string_transform(char* string, int flag);

int string_char_in_string(const char* string, char ch);

char* string_replace(char* source_string, char* a, char* b);

unsigned char string_item_in_string(char* source_string, char* item);

unsigned long string_get_item_in_string(char* source_string, char* item);

#endif /* string_h */
