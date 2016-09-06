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

unsigned char get_opcode(char token) {
    Token cmp;
    cmp.token = new(char);
    *cmp.token = token;
    for (int i = 0; i < arr_size(tokens); i++) {
        if (!strcmp(cmp.token, tokens[i].token)) {
            return tokens[i].op;
        }
    }
    
    return 0;
}

/*
** check if token is valid identifier
*/
unsigned char is_identifier(char* token) {
    int tokenlen = (int)strlen(token);
    int allowedlen = (int)strlen(allowed_characters);
    /* if invalid then return 0 */
    for (int i = 0; i < allowedlen; i++) {
        for (int j = 0; j < tokenlen; j++) {
            if (token[i] != allowed_characters[j] && j >= tokenlen - 1)
                return 0;
        }
    }
    
    return 1;
}

void push_item(Lex_instance* L, Token item) {
    list_push(L->result, item);
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
    int inputlim = (int)strlen(input);
    
    for (int i = 0; i < inputlim; i++) {
        temp_token = input[i];
        switch (temp_token) {
            case '#': {     /* comment */
                while (i++ < inputlim || input[i] == '\n');
            }
                break;
        
            case '\n': {
                L->line++;
            }
                break;
                
            default: {
                if (is_operator(temp_token)) {
                    char* tmp = new(char);
                    strcpy(tmp, item->value);
                    list_push(L->result, ((Token){tmp}));
                    string_clear(item);
                    list_push(item, temp_token);
                    tmp = new(char);
                    *tmp = temp_token;
                    list_push(L->result, ((Token){tmp, get_opcode(temp_token)}));
                    string_clear(item);
                    continue;
                }
                
                if (temp_token == ' ') {
                    char* tmp = new(char);
                    strcpy(tmp, item->value);
                    list_push(L->result, ((Token){tmp}));
                    string_clear(item);
                } else {
                    list_push(item, temp_token);
                }
            }
                break;
        }
    }
    
    char* tmp = new(char);
    strcpy(tmp, item->value);
    /* push last item to output */
    list_push(L->result, (Token){tmp});
    
    list_free(item);
    
    for (int i = 0; i < L->result->top; i++) {
        /* find token instruction/type */
        Token* current = &L->result->value[i];
        if (is_identifier(current->token)) {
            current->op = T_IDENTIFIER;
            continue;
        }
        if (is_number(current->token)) {
            current->op = T_NUMBER;
            continue;
        }
    }
    
#if LEX_DEBUG
    Token current;
    for (int i = 0; i < L->result->top; i++) {
        current = L->result->value[i];
        if (strcmp(current.token, "\0"))    /* don't print null character */
            puts(current.token);
    }
#endif
}


/*
** free a lex instance
*/
void lex_instance_free(Lex_instance* L) {
    list_free(L->result);
    free(L);    /* free whole instance */
}

