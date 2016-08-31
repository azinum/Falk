/* falk.c */
/* author: Azinum */
/* date: 30/08/16 */


#include "falk.h"

void falk_instance_init(Falk_instance* F) {
    F->lex_instance = new(Lex_instance);
    lex_instance_init(F->lex_instance);
}


void falk_getinput(Falk_instance* F) {
    String* input = new(String);
    
    while (1) {
        printf(FALK_PROMPT);
        if (getline(&input->value, &input->size, stdin) != -1) {;
            printf("%s\n", input->value);
        } else
            break;
    }
    
    list_free(input);
    lex_instance_free(F->lex_instance);
}