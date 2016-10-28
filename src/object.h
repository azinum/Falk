/* object.h */
/* Author: Azinum */
/* Date: 2016-08-30 */

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

/*
** convert var to object
** if variable is detected, convert it to object
** else: do nothing with it
*/
#define obj_convert(O) (O.type == T_VAR ? ((*(TValue*)(O.value.ptr)).value) : O)


#define obj2TValue(O) (*(TValue*)(O.value.ptr))

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

typedef int (*Cfunction)();

typedef struct CLibfunction {
    char* name;
    Cfunction func;
    const char* doc_string;
} CLibfunction;

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
    
    OP_GOTO,     /* goto ip. args: 1 */
    OP_JUMP,     /* goto ip - jump. args: 1 */
    OP_IF,
    OP_WHILE,    /* 13 */
    OP_CALLF,
    OP_SCOPE_BEGIN,
    OP_SCOPE_END,
    OP_EXIT,
    OP_PUSHK,
    OP_PUSHI,
    OP_POP,     /* 20 */
    OP_LT,      /* < */
    OP_GT,      /* > */
    OP_EQ,      /* == */
    OP_LEQ,     /* <= */
    OP_GEQ,     /* >= */
    OP_PUSHP,   /* push pointer */
    OP_ADD_ASSIGN,
    OP_SUB_ASSIGN,
    OP_MUL_ASSIGN,
    OP_DIV_ASSIGN,
    /* ... */
    OP_SET = OP_DIV_ASSIGN + 2,     /* SET a, b (direct assign) */
    OP_LOAD,
    OP_STORE,
    
    T_NULL,
    T_IDENTIFIER,
    T_NUMBER,
    T_STRING,   /* 30 */
    T_CSTRING,
    T_SCOPE,
    T_VAR,
    T_CFUNCTION,    /* 34 */
    
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
    {";", OP_END},
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

int object_is_true(Object obj);

#endif /* object_h */
