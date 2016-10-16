/* object.h */
/* author: Azinum */
/* date: 30/08/16 */

#ifndef object_h
#define object_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define new(TYPE) ((TYPE*)malloc(sizeof(TYPE)))
#define newx(TYPE, SIZE) ((TYPE*)malloc(SIZE * sizeof(TYPE)))

/*
** reference cast
*/
#define refcast(X) (&(X))

/*
** pointer to null address
*/
#define null ((void*)0x0)

/*
** allocate new memory block and throw memory error if fail
*/
#define newx_safe(NAME, TYPE, SIZE) { \
TYPE* tmp = ((TYPE*)malloc(SIZE * sizeof(TYPE))); \
if (tmp == NULL) puts("Memory failed"); \
else NAME = tmp; \
}

#define object_create(NAME, VALUE, TYPE) \
Object* NAME = new(Object); \
NAME->value.VALUE; \
NAME->type = TYPE

/*
** when using C99 we need to use __typeof__ keyword
** now rewritten so that we can use the normal typeof
*/
#ifndef typeof
#define typeof __typeof__
#endif

typedef union Value {
    char* string;
    double number;
    void* ptr;
} Value;

typedef struct Object {
    union Value value;
    unsigned char type;
} Object;

typedef struct Token {
    char* token;
    unsigned int op;
} Token;

typedef struct Offset {
    int x, y;
} Offset;

enum Instructions {
    OP_NULL = 0,
    OP_BEGIN,
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,     /* 5 */
    
    OP_EQ_ASSIGN,
    
    OP_LEFT_SHIFT,  /* << */
    OP_RIGHT_SHIFT,  /* >> */
    
    OP_END,
    
    T_NULL,     /* 10 */
    T_IDENTIFIER,
    T_NUMBER,
    T_STRING,
    T_CSTRING,
    T_SCOPE,    /* 15 */
    T_VAR,
    
    OP_GOTO,     /* goto ip. args: 1 */
    OP_JUMP,     /* goto ip - jump. args: 1 */
    OP_IF,
    OP_WHILE,    /* 20 */
    OP_CALLF,
    
    TOK_NEWLINE,  /* not an instruction / op code */
    TOK_LEFT_P,     /* left parenthesis "(" */
    TOK_RIGHT_P,    /* right parenthesis ")" */
    TOK_LEFT_CURLY_BRACKET,     /* 25 */
    TOK_RIGHT_CURLY_BRACKET,
    TOK_COMMA,
    TOK_COLON,
};

static Token tokens[] = {
    {"+", OP_ADD},
    {"-", OP_SUB},
    {"*", OP_MUL},
    {"/", OP_DIV},
    {"=", OP_EQ_ASSIGN},
    {"(", TOK_LEFT_P},
    {")", TOK_RIGHT_P},
    {"{", TOK_LEFT_CURLY_BRACKET},
    {"}", TOK_RIGHT_CURLY_BRACKET},
    {",", TOK_COMMA},
    {":", TOK_COLON},
    {"if", OP_IF},
    {"while", OP_WHILE},
    //    {"\n", TOK_NEWLINE},
};


unsigned char is_number(const char* string);

double to_number(const char* string);

void print_object(Object object);

#endif /* object_h */
