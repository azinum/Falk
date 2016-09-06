/* parse.h */
/* author: Azinum */
/* date: 05/09/16 */


#ifndef parse_h
#define parse_h

#include <stdio.h>
#include "lex.h"

enum Associativities {
    ASSO_LEFT_RIGHT,
    ASSO_RIGHT_LEFT,
    ASSO_NONE
};

enum Parse_errors {
    PARSE_NO_ERROR,
    PERR_BLOCK_NO_MATCH,     /* block does not match */
};

typedef struct Operator {
    const char* token;  /* token for this operator (+, -, *, /) */
    unsigned char op;   /* what operator? OP_ADD, OP_SUB e.t.c */
    unsigned char asso;     /* associativity */
    unsigned char prec;     /* operator precedence */
} Operator;

static Operator operators[] = {
    {"*", OP_MUL, ASSO_LEFT_RIGHT, 20},
    {"/", OP_DIV, ASSO_LEFT_RIGHT, 20},
    {"-", OP_SUB, ASSO_LEFT_RIGHT, 19},
    {"+", OP_ADD, ASSO_LEFT_RIGHT, 19},
    {"=", OP_EQ_ASSIGN, ASSO_RIGHT_LEFT, 1},
};

static const char* parse_error_info[] = {
    "", /* no error */
    "Block does not match"
};


typedef struct Parse_instance {
    int error, warning;
    int line;
    Tokenlist* result;
    Lex_instance* lex_instance;
    Tokenlist* stack;
} Parse_instance;


void parse_instance_init(Parse_instance* P);

void parse(Parse_instance* P, char* input);

void parse_throw_error(Parse_instance* P, unsigned char error);

void parse_instance_free(Parse_instance* P);

void check_precedence(Parse_instance* P);

#endif /* parse_h */
