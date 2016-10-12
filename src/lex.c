/* lex.c */
/* author: Azinum */
/* date: 30/08/16 */


#include "lex.h"
#include "falk.h"

/*
** initialize any Lex_instance
*/
void lex_instance_init(Lex_instance* L) {
    L->error = LEX_NO_ERROR;
    L->warning = 0;
    L->line = 1;
    list_init(refcast(L->result));
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

unsigned char is_keyword(Token token) {
    for (int i = 0; i < arr_size(tokens); i++) {
        if (!strcmp(token.token, tokens[i].token)) {
            return 1;
        }
    }
    return 0;
}

unsigned char get_keyword(Token token) {
    for (int i = 0; i < arr_size(tokens); i++) {
        if (!strcmp(token.token, tokens[i].token)) {
            return tokens[i].op;
        }
    }
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
int lex(Lex_instance* L, char* input) {
    if (strlen(input) < 1)
        return 0;
    String item;
    list_init(refcast(item));
    
    char temp_token;
    int inputlim = (int)strlen(input);
    
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
        return 0;
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
                /*
                ** if op then
                **   push current item
                **   erase value of current item
                **   push op to result
                **   break
                ** else
                **   ...
                */
                if (is_operator(temp_token)) {
                    char* copy;
                    string_copy(copy, item.value);
                    list_push(refcast(L->result), ((Token){copy}));
                    list_clear(refcast(item));
                    copy = newx(char, 2);
                    copy[0] = temp_token;
                    copy[1] = '\0';
                    list_push(refcast(L->result), ((Token){copy}));
                    break;
                }
                
                if (temp_token == ' ') {
                    char* copy;
                    string_copy(copy, (refcast(item)->value));
                    list_push(refcast(L->result), ((Token){copy}));
                    list_clear(refcast(item));
                    break;
                }
                list_push(refcast(item), temp_token);
            }
                break;
        }
    }
    
    /* push last item to output */
    char* copy;
    string_copy(copy, (refcast(item)->value));
    list_push(refcast(L->result), (Token){copy});
    
    list_val_free(refcast(item));
    
    for (int i = 0; i < refcast(L->result)->top; i++) {
        /* find token instruction/type */
        Token* current = &refcast(L->result)->value[i];
        
        if (is_keyword(*current)) {
            current->op = get_keyword(*current);
            continue;
        }
        
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
    for (int i = 0; i < refcast(L->result)->top; i++) {
        current = refcast(L->result)->value[i];
        printf("(%s, %i)\n", current.token, current.op);
    }
#endif
    
    /*
    ** hack to remove all null characters (must fix the real problem)
    */
    Tokenlist lexmod;
    list_init(refcast(lexmod));
    
    for (int i = 0; i < L->result.top; i++) {
        if (list_get(refcast(L->result), i).op != 0) {
            list_push(refcast(lexmod), list_get(refcast(L->result), i));
        }
    }
    
    L->result = lexmod;
    
    return 1;
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

