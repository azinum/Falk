/* lex.h */
/* author: Azinum */
/* date: 30/08/16 */


#ifndef lex_h
#define lex_h

#include <stdio.h>
#include "list.h"

typedef struct Lex_instance {
    int error;  /* current error */
} Lex_instance;


void lex(Lex_instance* L, String* input);

#endif /* lex_h */
