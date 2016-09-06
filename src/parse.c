/* parse.c */
/* author: Azinum */
/* date: 05/09/16 */


#include "parse.h"


void parse_instance_init(Parse_instance* P) {
    P->error = PARSE_NO_ERROR;
    P->warning = 0;
    P->result = new(Tokenlist);
    list_init(P->result);
}

/*
** the shunting yard algorithm will be used in parser
** main reason is that it is simple and using a parser generator is not very optimal
*/
void parse(Parse_instance* P, char* input) {
    lex(P->lex_instance, input);
    Tokenlist* lexed = P->lex_instance->result;
    
    for (int i = 0; i < lexed->top; i++) {
        switch (lexed->value[i].op) {
            
            case OP_ADD:
                puts("op_add");
                break;
                
            case OP_SUB:
                puts("op_sub");
                break;
                
            case OP_MUL:
                puts("op_mul");
                break;
                
            case OP_DIV:
                puts("op_div");
                break;
                
            default:
                break;
        }
    }
}

void parse_instance_free(Parse_instance* P) {
    list_free(P->result);
}