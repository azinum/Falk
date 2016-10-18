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

list_define(Offset_list, Offset);

list_define(Int_list, int);

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
    PARSE_ERR_BLOCK_NO_MATCH,     /* block does not match */
    PARSE_ERR_SYNTAXERROR,
};

static const char* parse_error_info[] = {
    "",     /* no error */
    "Block does not match",
    "SyntaxError"
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
        prec,   /* operator priority */
        *rule,  /* array of what can follow this type */
        *prio,  /* priority in which order a rule is parsed */
        rule_size;  /* how many items in rule */
} Parse_node;

/*
** these are initialized at parse_instance_init
*/
static Parse_node parse_rules[] = {
/*  type,   asso,   prec,   rule,   prio,   rule size   */
    {OP_MUL, ASSO_LR, 10, NULL, NULL, 0},
    {OP_MUL, ASSO_LR, 10, NULL, NULL, 0},
    {OP_ADD, ASSO_LR, 5, NULL, NULL, 0},
    {OP_SUB, ASSO_LR, 5, NULL, NULL, 0},
    {OP_EQ_ASSIGN, ASSO_RL, 1, NULL, NULL, 0},
    {OP_IF, ASSO_NONE, 0, NULL, NULL, 0},
    {OP_WHILE, ASSO_NONE, 0, NULL, NULL, 0}
};

typedef struct Parse_instance {
    int error, warning, line;
    Tokenlist* result;
    Lex_instance* lex_instance;
    Tokenlist lexed;    /* manipulated lex result */
    Tokenlist* stack;   /* stack for dealing operators */
    int jump;   /* block size when do look ahead */
    int lim;    /* do not parse beyond this */
} Parse_instance;

void parse_instance_init(Parse_instance* P);

int parse_throw_error(Parse_instance* P, unsigned char error);

void parse_instance_free(Parse_instance* P);

PRule get_prod_rule(unsigned char a, unsigned char b);

Parse_node get_parse_node(Parse_instance* P, int type);

int* intarr_create(int flagc, ...);

int parse(Parse_instance* P, char* input);

int produce(Parse_instance* P);

int parse_expression(Parse_instance* P, unsigned int from, unsigned int to);

void check_precedence(Parse_instance* P, Tokenlist* stack);

int* check_next(Parse_instance* P, int index, int steps);

int check_current(Parse_instance* P, int index);

int check_validity(Parse_instance* P, Parse_node rule, Int_list comp);

Offset_list get_next(Parse_instance* P, int index, int steps);

void gf_info_print(unsigned char flag);

#endif /* parse_h */
