/* falk.c */
/* author: Azinum */
/* date: 30/08/16 */


#include "falk.h"

void falk_instance_init(Falk_instance* F) {
    F->lex_instance = new(Lex_instance);
    lex_instance_init(F->lex_instance);
    F->parse_instance = new(Parse_instance);
    parse_instance_init(F->parse_instance);
}


void falk_getinput(Falk_instance* F) {
    char* input = NULL;
    unsigned long size = 0;
    
    while (1) {
        printf(FALK_PROMPT);
        if (getline(&input, &size, stdin) > -1) {
            input[strlen(input)] = 0;   /* remove newline */
            lex(F->lex_instance, input);
        } else
            break;
    }
    
    lex_instance_free(F->lex_instance);
    parse_instance_free(F->parse_instance);
}