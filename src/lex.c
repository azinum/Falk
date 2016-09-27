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
    L->line = 1;
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
unsigned char is_identifier(const char* token) {
    int tokenlen = (int)strlen(token);
    if (tokenlen <= 0)
        return 0;
    int allowedlen = (int)strlen(allowed_characters);
    int validc = 0;     /* how many characters in string is valid? */
    /* if invalid then return 0 */
    for (int i = 0; i < tokenlen; i++) {
        for (int j = 0; j < allowedlen; j++) {
            if (token[i] == allowed_characters[j])
                validc++;
        }
    }
    
    for (int i = 0; i < (int)strlen(allowed_characters_after); i++) {
        if (token[0] == allowed_characters_after[i])
            return 0;
    }
    
    if (validc == tokenlen)     /* number of valid characters must be same as input string length */
        return 1;
    
    return 0;
}


/*
** main function for lexer
** here we convert our input string to a list of tokens
** every token has a string type and a integer byte (unsigned char)
** the string is for knowing how the token looks like, the number is describing the token
** get description from enum: Instructions, at "object.h"
*/
Tokenlist* lex(Lex_instance* L, char* input) {
    if (strlen(input) < 1)
        return NULL;
    String* item = new(String);
    list_init(item);
    
    char temp_token;
    int inputlim = (int)strlen(input);
    
    Tokenlist* result;  /* final lexed product */
    result = new(Tokenlist);
    list_init(result);
    
    /*
    ** bracket count (how many extra brackets there are)
    ** if counter is not equal to 0, (same amount of brackets) then we got a mismatch
    */
    int parenc, curlybc, normalbc;
    parenc = 0;
    curlybc = 0;
    normalbc = 0;
    
    for (int i = 0; i < inputlim; i++) {
        switch (input[i]) {
            case '(': {
                parenc++;
            }
                break;
                
            case ')': {
                parenc--;
            }
                break;
                
            case '{': {
                curlybc++;
            }
                break;
                
            case '}': {
                curlybc--;
            }
                break;
                
            case '[': {
                normalbc++;
            }
                break;
                
            case ']': {
                normalbc--;
            }
                break;
                
            default:
                break;
        }
    }
    
    if (parenc != 0 || curlybc != 0 || normalbc != 0) {
        lex_throw_error(L, LEXERR_INVALID_BLOCK);
        return NULL;
    }
    
    for (int i = 0; i < inputlim; i++) {
        temp_token = input[i];
        
        if (parenc < 0 || curlybc  < 0 || normalbc < 0)
            lex_throw_error(L, LEXERR_INVALID_BLOCK);
        
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
                    string_copy(tmp, item->value);
                    list_push(result, ((Token){tmp}));
                    string_clear(item);
                    list_push(item, temp_token);
                    tmp = new(char);
                    *tmp = temp_token;
                    list_push(result, ((Token){tmp, get_opcode(temp_token)}));
                    string_clear(item);
                    continue;
                }
                
                if (temp_token == ' ') {
                    char* tmp = new(char);
                    string_copy(tmp, item->value);
                    list_push(result, ((Token){tmp}));
                    string_clear(item);
                } else {
                    list_push(item, temp_token);
                }
            }
                break;
        }
    }
    
    char* tmp;
    string_copy(tmp, item->value);
    
    /* push last item to output */
    list_push(result, (Token){tmp});
    
    list_free(item);
    
    for (int i = 0; i < result->top; i++) {
        /* find token instruction/type */
        Token* current = &result->value[i];
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
    for (int i = 0; i < result->top; i++) {
        current = result->value[i];
        if (strcmp(current.token, "\0"))    /* don't print null character */
            printf("%s, %i\n", current.token, current.op);
    }
#endif
    return result;
}

void lex_throw_error(Lex_instance* L, unsigned char error) {
    printf("(LexError) %s.\n", lex_error_info[error]);
    L->error = LEX_NO_ERROR;
}

/*
** free a lex instance
*/
void lex_instance_free(Lex_instance* L) {
    free(L);    /* free whole instance */
}

