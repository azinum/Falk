/* parse.h */
/* author: Azinum */
/* date: 05/09/16 */


#ifndef parse_h
#define parse_h

#include <stdio.h>
#include "lex.h"
#include "table.h"

#define PARSE_DEBUG 0

enum Associativities {
    ASSO_LR = 5,
    ASSO_RL,
    ASSO_NONE
};

enum Parse_errors {
    PARSE_NO_ERROR,
    PERR_BLOCK_NO_MATCH,     /* block does not match */
};

typedef struct Operator {
    unsigned char op;   /* what operator? OP_ADD, OP_SUB e.t.c */
    unsigned char asso;     /* associativity */
    unsigned char prec;     /* operator precedence */
} Operator;

static Operator operators[] = {
    /* standard operators */
    {OP_MUL, ASSO_LR, 20},
    {OP_DIV, ASSO_LR, 20},
    {OP_SUB, ASSO_LR, 19},
    {OP_ADD, ASSO_LR, 19},
    {OP_EQ_ASSIGN, ASSO_RL, 1},
    /* other */
    {T_IDENTIFIER, ASSO_NONE, -1},
    {TOK_LEFT_P, ASSO_NONE, -1},
    {TOK_RIGHT_P, ASSO_NONE, -1},
    {-1, -1, -1}
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
    Tokenlist* stack;   /* we use the stack for keeping operators and keywords */
} Parse_instance;


void parse_instance_init(Parse_instance* P);

int parse(Parse_instance* P, char* input);

void parse_throw_error(Parse_instance* P, unsigned char error);

void parse_instance_free(Parse_instance* P);

void check_precedence(Parse_instance* P);

void add_operators(Parse_instance* P);

Operator get_operator(unsigned char op);

#endif /* parse_h */
