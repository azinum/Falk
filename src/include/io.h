/* io.h */
/* Author: Azinum */
/* Date: 2016-10-09 */


#ifndef io_h
#define io_h

#include <stdio.h>
#include "object.h"
#include "falk.h"

char* file_read(const char* fname);

int file_exist(const char* fname);

void printx(const char* what, unsigned int count);

#endif /* io_h */
