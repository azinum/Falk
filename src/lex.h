/* lex.h */
/* author: Azinum */
/* date: 30/08/16 */


#ifndef lex_h
#define lex_h

#include <stdio.h>
#include "list.h"

enum Lex_errors {
    LEX_NO_ERROR,
    LEXERR_INVALID_TOKEN,
    LEXERR_INVALID_BLOCK,   /* missing end / begin token of block */
};

typedef struct Lex_instance {
    int error, warning;  /* current error and warning */
    unsigned int line;  /* to trace our code back if error */
    Lexed* result;  /* final lexed product */
} Lex_instance;


void lex(Lex_instance* L, String* input);

void lex_instance_init(Lex_instance* L);

void lex_instance_free(Lex_instance* L);

#endif /* lex_h */
