/* object.h */
/* author: Azinum */
/* date: 30/08/16 */

#ifndef object_h
#define object_h

#include <stdio.h>

#define new(TYPE) ((TYPE*)malloc(sizeof(TYPE)))

typedef union Value {
    double number;
    void* ptr;
} Value;

typedef struct Object {
    union Value value;
    unsigned char type;
} Object;

#endif /* object_h */
