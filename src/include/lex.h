/* lex.h */
/* Author: Azinum */
/* Date: 2016-08-30 */


#ifndef lex_h
#define lex_h

#include <stdio.h>
#include <string.h>

#include "conf.h"
#include "list.h"

#define LEX_DEBUG 0

extern_c

enum Lex_errors {
    LEX_NO_ERROR,
    LEXERR_INVALID_TOKEN,
    LEXERR_INVALID_BLOCK,   /* missing end / begin token of block */
};

typedef struct Lex_instance {
    int error, warning;  /* current error and warning */
    unsigned int line;  /* to trace our code back if error */
    Token op;   /* current operator */
    Tokenlist result;
} Lex_instance;

static const char* lex_error_info[] = {
    "",
    "Invalid token",
    "Block does not match"
};

/*
** allowed characters for identifiers
*/
static const char* allowed_characters = "QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm_0123456789";

/*
** allowed characters after the first letter of identifier
*/
static const char* allowed_characters_after = "0123456789";

int lex(Lex_instance* L, char* input);

int lex_instance_init(Lex_instance* L);

void lex_instance_free(Lex_instance* L);

unsigned char is_operator(char token);

unsigned char is_keyword(Token token);

unsigned char get_keyword(Token token);

unsigned char get_opcode(char token);

unsigned char is_identifier(const char* token);

void lex_throw_error(Lex_instance* L, unsigned char error);

extern_c_end

#endif /* lex_h */
