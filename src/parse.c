/* parse.c */
/* author: Azinum */
/* date: 05/09/16 */


#include "parse.h"
#include "falk.h"

void parse_instance_init(Parse_instance* P) {
    P->error = PARSE_NO_ERROR;
    P->warning = 0;
    P->line = 0;
    P->result = new(Tokenlist);
    list_init(P->result);
    P->stack = new(Tokenlist);
    list_init(P->stack);
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
** production of extra tokens
** in case we need extra tokens after two specific tokens or blocks
** example
**
** when calling a function, a callf instruction needs to be produced
**
** myfunction(args) -> myfunction(args) callf
*/
int produce(Parse_instance* P) {
    list_define(Intlist, int);
    Intlist prod_stack;     /* to store temporary types (operator, expression, body) */
    list_init(refcast(prod_stack));
    
    TokenLL* list = new(TokenLL);
    llist_init(list);
    int prod_pushed = 0;    /* how many extra tokens pushed */
    
    for (int i = 0; i < P->lexed.top; i++) {
        llist_push(list, list_get(refcast(P->lexed), i));
    }
    
    /*
    ** TODO: create a function for this
    ** int* get_next(self, index, steps);
    **
    ** might use a helper function
    ** int check_match(tokenlist, index, token_a, token_b)
    ** will return true / false if match was successful
    ** maybe use of another helper function that returns size of a block (for skipping tokens easily)
    **
    */
    unsigned char* check = check_next(P, 0, 2);
    
    for (int i = 0; i < 2; i++)
        printf("%i ", check[i]);
    puts("\n========");
    
    TokenLL* it = list;
    while (it->next != NULL) {
        it = it->next;
        printf("%s ", it->value.token);
    }
    puts("");
    
    return 1;
}


/*
** recursive parsing
*/
int parse_expression(Parse_instance* P, int from, int to) {
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
            
            case OP_IF: {
                puts("I_IF");
            }
                break;
                
            case OP_WHILE: {
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
                parse_expression(P, i + 1, i + next+1);
                i += next;
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
    
    return 1;
}

int parse(Parse_instance* P, char* input) {
    Lex_instance* lex_instance = new(Lex_instance);
    lex_instance_init(lex_instance);
    P->lex_instance = lex_instance;
    
    lex(lex_instance, input);
    
    P->lexed = lex_instance->result;
    
    if (!produce(P))
        return 0;
    
    return 1;
    
    if (!parse_expression(P, 0, P->lexed.top))
        return 0;
    
    for (int i = 0; i < P->result->top; i++) {
        printf("%s ", list_get(P->result, i).token);
    }
    puts("");
    
    list_clear2(P->result);
    list_clear2((&(lex_instance->result)));
    return 1;
}

/*
** check following sequence of code
*/
unsigned char* check_next(Parse_instance* P, int index, int steps) {
    list_define(Instr_list, unsigned char);
    Instr_list list;
    list_init(refcast(list));
    
    while (steps > 0) {    /* while there are steps left */
        switch (list_get(refcast(P->lex_instance->result), index).op) {
            case OP_ADD:
            case OP_SUB:
            case OP_DIV:
            case OP_MUL:
            case OP_EQ_ASSIGN: {
                puts("+|-|/|*|=");
                list_push(refcast(list), list_get(refcast(P->lex_instance->result), index).op);
            }
                break;
                
            case T_NUMBER: {
                puts("number");
                list_push(refcast(list), list_get(refcast(P->lex_instance->result), index).op);
            }
                break;
                
            case T_IDENTIFIER: {
                puts("ident");
                list_push(refcast(list), list_get(refcast(P->lex_instance->result), index).op);
            }
                break;
                
            case OP_IF: {
                puts("if");
                list_push(refcast(list), IF);
            }
                break;
            
            case OP_WHILE: {
                puts("while");
                list_push(refcast(list), WHILE);
            }
                break;
                
            case TOK_LEFT_P: {
                puts("expr");
                int delta = 0;  /* how big difference */
                
                while (index < P->lex_instance->result.top) {
                    if (list_get(refcast(P->lex_instance->result), index).op == TOK_LEFT_P)
                        delta++;

                    if (list_get(refcast(P->lex_instance->result), index).op == TOK_RIGHT_P)
                        delta--;
                    
                    if (delta == 0) {
                        break;
                    }

                    index++;
                }
                
                if (delta != 0) {
                    parse_throw_error(P, PERR_BLOCK_NO_MATCH);
                    return null;
                }
                
                list_push(refcast(list), EXPRESSION);
            }
                break;
                
            default:
                list_push(refcast(list), OP_NULL);
                break;
        }
        steps--;
        index++;
    }
    
    return list.value;
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


void gf_info_print(unsigned char flag) {
    printf("%s\n", gf_info[flag]);
}

void parse_throw_error(Parse_instance* P, unsigned char error) {
    printf("(ParseError) %s\n", parse_error_info[error]);
}


void parse_instance_free(Parse_instance* P) {
    list_free(P->result);
    list_free(P->stack);
    free(P);
}
