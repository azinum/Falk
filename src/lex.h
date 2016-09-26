/* lex.h */
/* author: Azinum */
/* date: 30/08/16 */


#ifndef lex_h
#define lex_h

#include <stdio.h>
#include <string.h>

#include "list.h"

#define LEX_DEBUG 0


enum Lex_errors {
    LEX_NO_ERROR,
    LEXERR_INVALID_TOKEN,
    LEXERR_INVALID_BLOCK,   /* missing end / begin token of block */
};

typedef struct Lex_instance {
    int error, warning;  /* current error and warning */
    unsigned int line;  /* to trace our code back if error */
    Token op;   /* current operator */
} Lex_instance;

static const char* lex_error_info[] = {
    "",
    "Invalid token",
    "Block does not match"
};

/*
** allowed characters for identifiers
*/
static const char* allowed_characters = "QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm_";

Tokenlist* lex(Lex_instance* L, char* input);

void lex_instance_init(Lex_instance* L);

void lex_instance_free(Lex_instance* L);

unsigned char is_operator(char token);

unsigned char get_opcode(char token);

unsigned char is_identifier(const char* token);

void lex_throw_error(Lex_instance* L, unsigned char error);

#endif /* lex_h */
