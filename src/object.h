/* object.h */
/* author: Azinum */
/* date: 30/08/16 */

#ifndef object_h
#define object_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define new(TYPE) ((TYPE*)malloc(sizeof(TYPE)))

/*
** when using C99 we need to use __typeof__ keyword
** now rewritten so that we can use the normal typeof
*/
#ifndef typeof
#define typeof __typeof__
#endif

/*
** token with the most characters
** tok | size
** + :   1
** == :  2
** <<= : 3
*/
#define TOKEN_SIZE_MAX 3

typedef union Value {
    double number;
    void* ptr;
} Value;

typedef struct Object {
    union Value value;
    unsigned char type;
} Object;

typedef struct Token {
    char* token;
    int op;
} Token;

enum Instructions {
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    
    OP_EQ_ASSIGN,
    
    OP_LEFT_SHIFT,  /* << */
    OP_RIGHT_SHIFT,  /* >> */
    
    OP_IDENTIFIER,
    
    TOK_NEWLINE = 255,  /* normal instructions goes from 0-255 due to BYTE format (unsigned char) */
    TOK_LEFT_P,     /* left parenthesis "(" */
    TOK_RIGHT_P,
    TOK_LEFT_CURLY_BRACKET,
    TOK_RIGHT_CURLY_BRACKET,
    TOK_COMMA,
    TOK_PERIOD,
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
    {".", TOK_PERIOD},
    {":", TOK_COLON}
//    {"\n", TOK_NEWLINE},
};


unsigned char is_number(const char* string);

double to_number(const char* string);

#endif /* object_h */
