/* parse.h */
/* author: Azinum */
/* date: 05/09/16 */


#ifndef parse_h
#define parse_h

#include <stdio.h>
#include <stdarg.h>
#include "lex.h"
#include "table.h"
#include "llist.h"

#define PARSE_DEBUG 0

/*
** define a doubly linked list with token type
*/
llist_define(TokenLL, Token);

/*
** every operator has their own associativity
*/
enum Associativities {
    ASSO_LR = 5,
    ASSO_RL,
    ASSO_NONE
};

enum Parse_errors {
    PARSE_NO_ERROR,
    PERR_BLOCK_NO_MATCH,     /* block does not match */
};

/*
** flags for grammar checking
*/
enum Gflags {
    NONE        = 1 << 0,
    IF          = 1 << 1,
    ELSE        = 1 << 2,
    WHILE       = 1 << 3,
    FUNC        = 1 << 4,
    EXPRESSION  = 1 << 5,
    BODY        = 1 << 6,
    END         = 1 << 7,
    ANY = NONE | IF | ELSE | WHILE | FUNC | EXPRESSION | BODY | END
};

typedef struct Rule {
    int* flags;     /* array of flags (first flag is the rule itself) */
    int fc;     /* flag count */
} Rule;

list_define(RuleList, Rule);

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
    RuleList* rules;
    TokenLL* resultll;  /* result as linked list */
} Parse_instance;

void parse_instance_init(Parse_instance* P);

int parse(Parse_instance* P, char* input);

void parse_throw_error(Parse_instance* P, unsigned char error);

void parse_instance_free(Parse_instance* P);

void check_precedence(Parse_instance* P);

Operator get_operator(unsigned char op);

int* create_flagset(int flagc, ...);

int parse_grammar(Parse_instance* P, Tokenlist* to_parse);

#endif /* parse_h */
