/* object.h */
/* author: Azinum */
/* date: 30/08/16 */

#ifndef object_h
#define object_h

#include <stdio.h>
#include <stdlib.h>

#define new(TYPE) ((TYPE*)malloc(sizeof(TYPE)))

/*
** when using C99 we need to use __typeof__ keyword
** now rewritten so that we can use the normal typeof
*/
#ifndef typeof
#define typeof __typeof__
#endif

typedef union Value {
    double number;
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
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    
    OP_IDENTIFIER,
    
};

unsigned char is_number(const char* string);

double to_number(const char* string);

#endif /* object_h */
