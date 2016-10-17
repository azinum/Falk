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

#if PARSE_DEBUG
#define debug_printf(TOPRINT, ...) printf(TOPRINT, ##__VA_ARGS__)
#else
#define debug_printf(TOPRINT, ...)
#endif

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
    OP          = 1 << 8,
    OPR         = 1 << 9,
    ANY = NONE | IF | ELSE | WHILE | FUNC | EXPRESSION | BODY | END
};

/*
** grammar flag info
*/
static const char* gf_info[] = {
    "none",
    "if",
    "else",
    "while",
    "function",
    "expression",
    "body",
    "end",
    "op",
    "operand",
    "any"
};

typedef struct Operator {
    unsigned char op;   /* what operator? OP_ADD, OP_SUB e.t.c */
    unsigned char asso;     /* associativity */
    unsigned char prec;     /* operator precedence */
} Operator;

/*
** rule for creating extra tokens
*/
typedef struct PRule {
    unsigned char a, b, value;    /* a + b = value */
} PRule;

static PRule prod_rules[] = {
    {T_IDENTIFIER, EXPRESSION, OP_CALLF},
    {EXPRESSION, EXPRESSION, OP_CALLF},
    {0, 0, 0}
};

typedef struct Parse_node {
    int type,   /* if, while, +, - */
        asso,   /* x(x) or (x)x */
        prec,
        *rule,  /* array of what can follow this type */
        *prio;  /* priority in which order a rule is parsed */
} Parse_node;

static Parse_node parse_rules[] = {
    {OP_MUL, ASSO_LR, 10, NULL, NULL},
    {OP_MUL, ASSO_LR, 10, NULL, NULL},
    {OP_ADD, ASSO_LR, 5, NULL, NULL},
    {OP_SUB, ASSO_LR, 5, NULL, NULL},
    {OP_EQ_ASSIGN, ASSO_RL, 1, NULL, NULL},
    {OP_IF, ASSO_NONE, 0, NULL, NULL},
    {OP_WHILE, ASSO_NONE, 0, NULL, NULL}
};

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
    int error, warning, line;
    Tokenlist* result;
    Lex_instance* lex_instance;
    Tokenlist* stack;   /* we use the stack for keeping operators and keywords */
    int jump;   /* block size when do look ahead */
} Parse_instance;

void parse_instance_init(Parse_instance* P);

int parse_throw_error(Parse_instance* P, unsigned char error);

void parse_instance_free(Parse_instance* P);

Operator get_operator(unsigned char op);

PRule get_prod_rule(unsigned char a, unsigned char b);

int* intarr_create(int flagc, ...);

int parse(Parse_instance* P, char* input);

int produce(Parse_instance* P);

int parse_expression(Parse_instance* P, int from, int to);

void check_precedence(Parse_instance* P, Tokenlist* stack);

unsigned char* check_next(Parse_instance* P, int index, int steps);

void gf_info_print(unsigned char flag);

#endif /* parse_h */
