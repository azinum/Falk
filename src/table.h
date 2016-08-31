/* table.h */
/* author: Azinum */
/* date: 31/08/16 */


#ifndef table_h
#define table_h

#include <stdio.h>
#include "object.h"

#define table_define(NAME, TYPE) \
typedef struct NAME { \
    int key; \
    TYPE value; \
} NAME

int hash(const char* key);

#endif /* table_h */
