/* lex.c */
/* author: Azinum */
/* date: 30/08/16 */


#include "lex.h"

/*
** initialize any Lex_instance
*/
void lex_instance_init(Lex_instance* L) {
    L->error = LEX_NO_ERROR;
    L->warning = 0;
    L->line = 0;
    L->result = new(Tokenlist);
    list_init(L->result);
}


/*
** main function for lexer
** here we convert our input string to a list of tokens
** every token has a string type and a integer byte (unsigned char)
** the string is for knowing how the token looks like, the number is describing the token
** get description from enum: Instructions, at "object.h"
*/
void lex(Lex_instance* L, char* input) {
    Tokenlist* splitted = new(Tokenlist);
    list_init(splitted);
    
    String* item = new(String);
    list_init(item);
    char temp_token;
    
    for (int i = 0; i < (int)strlen(input) - 1; i++) {
        temp_token = input[i];
        
        if (temp_token == ' ') {
            Token to_push;
            to_push.token = item->value;
            list_push(splitted, to_push);
            list_clear(item);
        } else {
            list_push(item, temp_token);
        }
    }
    
    /* push last item to output */
    list_push(splitted, (Token){item->value});
    
    for (int i = 0; i < splitted->size; i++) {
        printf("%s \n", splitted->value[i].token);
    }
    
    list_free(splitted);
    list_free(item);
}


/*
** free a lex instance
*/
void lex_instance_free(Lex_instance* L) {
    list_free(L->result);
    free(L);    /* free whole instance */
}

