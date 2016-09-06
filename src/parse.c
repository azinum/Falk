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

void check_precedence(Parse_instance* P) {
    
}

/*
** the shunting yard algorithm will be used in parser
** main reason is that it is simple and using a parser generator is not very optimal
*/
void parse(Parse_instance* P, char* input) {
    lex(P->lex_instance, input);
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
                
            /* can be function, push to stack */
            case T_IDENTIFIER: {

            }
                break;
            
            case T_NUMBER: {
                /* push to output */
            }
                break;
                
            case TOK_NEWLINE: {
                P->line++;
                printf("ln: %i\n", P->line);
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
        puts(P->result->value[i].token);
    }

}

void parse_throw_error(Parse_instance* P, unsigned char error) {
    P->error = error;
    printf("(ParseError) %s\n", parse_error_info[P->error]);
    P->error = PARSE_NO_ERROR;
}

void parse_instance_free(Parse_instance* P) {
    list_free(P->result);
}