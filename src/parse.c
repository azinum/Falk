/* parse.c */
/* author: Azinum */
/* date: 05/09/16 */


#include "parse.h"


void parse_instance_init(Parse_instance* P) {
    P->error = PARSE_NO_ERROR;
    P->warning = 0;
}

void parse(Parse_instance* P, char* input) {
    lex(P->lex_instance, input);
}

void parse_instance_free(Parse_instance* P) {
    
}