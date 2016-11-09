/* string.h */
/* Author: Azinum */
/* Date: 2016-11-09 */


#ifndef string_h
#define string_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char* string_random(unsigned int length);

char* string_random_range(unsigned int length, int min, int max);

#endif /* string_h */
