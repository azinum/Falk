/* parse.c */
/* author: Azinum */
/* date: 05/09/16 */


#include "parse.h"


void parse_instance_init(Parse_instance* P) {
    P->error = PARSE_NO_ERROR;
    P->warning = 0;
    P->line = 0;
    P->result = new(Tokenlist);
    list_init(P->result);
    P->stack = new(Tokenlist);
    list_init(P->stack);
}


Operator get_operator(unsigned char op) {
    Operator ret = {};
    
    for (int i = 0; i < arr_size(operators); i++) {
        ret = operators[i];
        if (ret.op == op) {
            return ret;
        }
    }
    
    return ret;
}


void check_precedence(Parse_instance* P) {
    if (P->stack->top >= 2) {
        Operator op0 = get_operator(list_get_top(P->stack).op);
        Operator op1 = get_operator(list_get_from_top(P->stack, -1).op);
        
        if (op1.op == TOK_LEFT_P)   /* do not read left parathese */
            return;
        
        if (op1.prec >= op0.prec && op0.asso == ASSO_LR) {
            list_push(P->result, list_get_from_top(P->stack, -1));
            list_pop2(P->stack);
            list_pop2(P->stack);
            list_push(P->stack, list_get_top(P->stack));
            return;
        }
    
    }
}


/*
** the shunting yard algorithm will be used in parser
** main reason is that it is simple and using a parser generator is not very optimal, full control is what we want
*/
int parse(Parse_instance* P, char* input) {
    if(!lex(P->lex_instance, input))
        return 0;
    Tokenlist* lexed = P->lex_instance->result;
    
    Token current;
    for (int i = 0; i < lexed->top; i++) {
        current = lexed->value[i];
        
        switch (current.op) {
            case OP_ADD:
            case OP_SUB:
            case OP_MUL:
            case OP_DIV:
            case OP_EQ_ASSIGN: {
                list_push(P->stack, current);
                check_precedence(P);
            }
                break;
                
            case TOK_LEFT_P: {
                list_push(P->stack, current);
            }
                break;
                
            case TOK_RIGHT_P: {
                /* While stack is not empty and left parenthese is not detected,
                ** push top value of stack and then pop top value.
                */
                while (P->stack->top != 0) {
                    if (list_get_top(P->stack).op == TOK_LEFT_P) {
                        list_pop2(P->stack);    /* pop left parenthese */
                        break;
                    }
                    list_push(P->result, list_get_top(P->stack));
                    list_pop2(P->stack);
                }
            }
                break;
                
            case T_IDENTIFIER: {
                list_push(P->result, current);
            }
                break;
            
            case T_NUMBER: {
                /* push to output */
                list_push(P->result, current);
            }
                break;
                
            case TOK_NEWLINE: {
                P->line++;
            }
                break;
            
            default: {
                /* error? */
            }
                break;
        }
    }
    
    /*
    ** push rest of stack (only temporary, when parser detect end of expression,
    ** it will push all operators on stack till we detect another end or similar to that)
    */
    while (P->stack->size > 1) {
        list_push(P->result, list_get_top(P->stack));
        list_pop2(P->stack);
    }
    
    for (int i = 0; i < P->result->top; i++) {
        printf("> %s\n", P->result->value[i].token);
    }
    
    for (int i = 0; i < P->lex_instance->result->top; i++) {
        list_pop2(P->lex_instance->result);
    }
    
    return 1;
}


void parse_throw_error(Parse_instance* P, unsigned char error) {
    P->error = error;
    printf("(ParseError) %s\n", parse_error_info[P->error]);
    P->error = PARSE_NO_ERROR;
}


void parse_instance_free(Parse_instance* P) {
    list_free(P->result);
    list_free(P->stack);
    free(P);
}