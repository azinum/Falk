/* falk.c */
/* author: Azinum */
/* date: 30/08/16 */


#include "falk.h"

void falk_instance_init(Falk_instance* F) {
    F->lex_instance = new(Lex_instance);
}

