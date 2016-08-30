/* object.h */
/* author: Azinum */
/* date: 30/08/16 */

#ifndef object_h
#define object_h

#include <stdio.h>

#define new(TYPE) ((TYPE*)malloc(sizeof(TYPE)))

typedef union Value {
    double number;
    const char* string;
    void* ptr;
} Value;

typedef struct Object {
    union Value value;
    unsigned char type;
} Object;

typedef struct Token {
    const char* token;
    unsigned char op;
} Token;

enum Instructions {
    OP_ADD = 10,
    OP_SUB,
};


#endif /* object_h */
