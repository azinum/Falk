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


unsigned char is_operator(char token) {
    char* cmp = new(char);
    *cmp = token;
    for (int i = 0; i < arr_size(tokens); i++) {
        if (!strcmp(cmp, tokens[i].token)) {
            free(cmp);
            return 1;
        }
    }
    free(cmp);
    return 0;
}

/*
** main function for lexer
** here we convert our input string to a list of tokens
** every token has a string type and a integer byte (unsigned char)
** the string is for knowing how the token looks like, the number is describing the token
** get description from enum: Instructions, at "object.h"
*/
void lex(Lex_instance* L, char* input) {
    if (strlen(input) <= 1)
        return;
    String* item = new(String);
    list_init(item);
    
    char temp_token;
    
    for (int i = 0; i < (int)strlen(input); i++) {
        temp_token = input[i];
        
        if (is_operator(temp_token)) {
            list_push(L->result, (Token){item->value});
            list_clear(item);
            list_push(item, temp_token);
            list_push(L->result, (Token){item->value});
            list_clear(item);
            continue;
        }
        
        if (temp_token == ' ') {
            list_push(L->result, ((Token){item->value}));
            list_clear(item);
        } else {
            list_push(item, temp_token);
        }
    }

    /* push last item to output */
    list_push(L->result, (Token){item->value});
    
    for (int i = 0; i < L->result->top; i++)
        printf("%s ", L->result->value[i].token);
    printf("\n");
    
//    list_free(item);
}


/*
** free a lex instance
*/
void lex_instance_free(Lex_instance* L) {
    list_free(L->result);
    free(L);    /* free whole instance */
}

