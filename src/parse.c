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
    P->rules = new(RuleList);
    P->resultll = new(TokenLL);
    llist_init(P->resultll);
    list_init(P->rules);
    list_push(P->rules, ((Rule){intarr_create(4, IF, EXPRESSION, BODY | END, ELSE | END), 4}));
    list_push(P->rules, ((Rule){intarr_create(3, WHILE, EXPRESSION, BODY | END), 3}));
    list_push(P->rules, ((Rule){intarr_create(3, FUNC, EXPRESSION, BODY | END), 3}));
    
    list_init(refcast(P->lexed));
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


void check_precedence(Parse_instance* P, Tokenlist* stack) {
    if (stack->top >= 2) {
        Operator op1, op2;
        op1 = get_operator(list_get_top(stack).op);
        op2 = get_operator(list_get_from_top(stack, -1).op);
        
        if (op2.op == TOK_LEFT_P || op1.op == TOK_LEFT_P)
            return;
        
        if (op2.prec >= op1.prec && op1.asso == ASSO_LR) {
            Token top = list_get_top(stack);
            list_push(P->result, list_get_from_top(stack, -1));
            list_pop2(stack);
            list_pop2(stack);
            list_push(stack, top);
            return;
        }
    }
}

/*
** recursive parsing
*/
void parse_expression(Parse_instance* P, int from, int to) {
    Tokenlist* stack = new(Tokenlist);
    list_init(stack);
    
    Token current;
    for (int i = from; i < to; i++) {
        current = list_get(refcast(P->lexed), i);
        
        switch (current.op) {
            case OP_ADD:
            case OP_SUB:
            case OP_MUL:
            case OP_DIV:
            case OP_EQ_ASSIGN: {
                list_push(stack, current);
                check_precedence(P, stack);
            }
                break;
                
            case T_IDENTIFIER:
            case T_NUMBER: {
                list_push(P->result, current);
            }
                break;
            
            case I_IF: {
                puts("I_IF");
            }
                break;
                
            case I_WHILE: {
                puts("I_WHILE");
            }
                break;
                
            /* expression */
            case TOK_LEFT_P: {
                int current_index = i;
                int next = current_index;
                while (next++ < to) {
                    if (list_get(refcast(P->lexed), i).op == TOK_RIGHT_P) {
                        break;
                    }
                }
                parse_expression(P, i + 1, i + next);
                i += next;
            }
                break;
                
            case TOK_RIGHT_P: {
                while (stack->top > 0) {
                    if (list_get_top(stack).op == TOK_LEFT_P) {
                        list_pop2(stack);
                        break;
                    }
                    list_push(P->result, list_get_top(stack));
                    list_pop2(stack);
                }
            }
                break;
            
            default:
                break;
        }
    }
    
    while (stack->top > 0) {
        list_push(P->result, list_get_top(stack));
        list_pop2(stack);
    }
}

void parse(Parse_instance* P, char* input) {
    Lex_instance* lex_instance = new(Lex_instance);
    lex_instance_init(lex_instance);
    P->lex_instance = lex_instance;
    
    lex(lex_instance, input);
    
    P->lexed = lex_instance->result;
    
    parse_expression(P, 0, P->lexed.top);
    
    for (int i = 0; i < P->result->top; i++) {
        printf("%s ", list_get(P->result, i).token);
    }
    puts("");
    
    list_clear2(P->result);
}


Offset* check_next(Parse_instance* P, int steps) {
    Offset* offset = newx(Offset, steps + 1);
    
    return offset;
}

unsigned char is_op(int op) {
    if (op > OP_BEGIN && op < OP_END)
        return 1;
    return 0;
}

int* intarr_create(int flagc, ...) {
    int* result = newx(int, flagc);
    va_list args;
    va_start(args, flagc);
    
    for (int i = 0; i < flagc; i++)
        result[i] = va_arg(args, int);
    
    return result;
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
