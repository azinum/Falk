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
                while (P->stack->top != 0) {
                    list_pop2(P->stack);
                    if (list_get_top(P->stack).op == TOK_LEFT_P)
                        break;
                }
                if (list_get_top(P->stack).op != TOK_LEFT_P) {
                    parse_throw_error(P, PERR_BLOCK_NO_MATCH);
                    break;
                }
            }
                break;
                
            case T_IDENTIFIER: {
                /* can be function, push to stack */
                list_push(P->stack, current);
            }
                break;
            
            case T_NUMBER: {
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