/* parse.h */
/* Author: Azinum */
/* Date: 2016-09-05 */


#ifndef parse_h
#define parse_h

#include <stdio.h>
#include <stdarg.h>

#include "lex.h"
#include "table.h"
#include "llist.h"
#include "ast.h"

extern_c

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


typedef struct Grammar_rule {
    const char* key, *action;
} Grammar_rule;



enum Parser_commands {
    PARSERC_NONE,
    PARSERC_SWAP,
    PARSERC_PUSH,
    PARSERC_REMOVE,
};

static const char parsercommands_keys[] = {
    'n',    /* do nothing (goto next token) */
    's',    /* swap A<=>B */
    'p',    /* push (instruction) */
    'r',    /* remove current token */
    'u',    /* push token as it is */
    '$',    /* parser variable $(ip)*/
};


static Grammar_rule grammar_rules[] = {
    {"word:", "p func"},
    {NULL, NULL}
};

typedef struct Parse_instance {
    int error, warning, line;
    unsigned char exit_on_error;
    Tokenlist* result;
    Lex_instance* lex_instance;
    Tokenlist lexed;    /* manipulated lex result */
    Tokenlist* stack;   /* stack for dealing with operators */
    int jump;   /* block size when do look ahead */
    int lim;    /* do not parse beyond this */
    HashTable variables;
} Parse_instance;

int parse_instance_init(Parse_instance* P);

int parse_throw_error(Parse_instance* P, unsigned char error);

void parse_instance_free(Parse_instance* P);

int* intarr_create(int flagc, ...);

int parse(Parse_instance* P, char* input);

AST_node* parse_tree(Parse_instance* P, AST_node* node, unsigned int from, unsigned int to);

int parse_asm(Parse_instance* P, char* input);

unsigned char is_op(int op);

extern_c_end

#endif /* parse_h */
