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
    P->jump = 0;
    list_init(refcast(P->lexed));
    
    /*
    ** temporary way of initializing rules
    */
    parse_rules[5].rule_size = 3;
    parse_rules[5].rule = intarr_create(parse_rules[5].rule_size, IF, EXPRESSION, BODY | END);     /* rule for if */
    parse_rules[5].prio = intarr_create(3, 1, 0, 2);    /* low -> high */
    
}

PRule get_prod_rule(unsigned char a, unsigned char b) {
    PRule rule = {0, 0, 0};     /* initialize a 'null' rule */

    for (int i = 0; i < arr_size(prod_rules); i++) {
        if (prod_rules[i].a == a && prod_rules[i].b == b) {
            rule = prod_rules[i];
            break;
        }
    }

    return rule;
}

Parse_node get_parse_node(Parse_instance* P, int type) {
    Parse_node node = {0, 0, 0, NULL, NULL};
    
    for (int i = 0; i < arr_size(parse_rules); i++) {
        if (parse_rules[i].type == type) {
            node = parse_rules[i];
        }
    }
    return node;
}

void check_precedence(Parse_instance* P, Tokenlist* stack) {
    if (stack->top >= 2) {
        Parse_node op1, op2;
        op1 = get_parse_node(P, list_get_top(stack).op);
        op2 = get_parse_node(P, list_get_from_top(stack, -1).op);
        if (op1.type != 0 && op2.type != 0) {
            if (op2.prec >= op1.prec && op1.asso == ASSO_LR) {
                Token temp = list_get_top(stack);
                list_push(P->result, list_get_from_top(stack, -1));
                list_pop2(stack);
                list_pop2(stack);
                list_push(stack, temp);
                return;
            }
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

    TokenLL* list = new(TokenLL);
    llist_init(list);

    /*
    ** extra token to push @ what index (where)
    */
    struct Value2push {
        int value;     /* rule value itself */
        int where;     /* where to push the rule value token */
    };

    list_define(Value2push_list, struct Value2push);
    Value2push_list val2push;
    list_init(refcast(val2push));

    for (int i = 0; i < P->lex_instance->result.top; i++) {
        llist_push(list, list_get(refcast(P->lex_instance->result), i));
    }

    int* check;
    int pushed = 0;     /* extra tokens pushed */
    PRule rule;
    Intlist topush;
    list_init(refcast(topush));

    for (int i = 0; i < P->lex_instance->result.top; i++) {
        if (!(check = check_next(P, i, 2))) {    /* to make sure we use a valid rule */
            continue;
        }
        rule = get_prod_rule(check[0], check[1]);

        if (rule.value != 0) {  /* for when rule is not null */
            list_push(refcast(val2push), ((struct Value2push){rule.value, P->jump + 1}));
        }
    }

    /*
    ** sort list based on 'where' variable (low -> high)
    */
    struct Value2push temp;
    for (int j = 0; j < val2push.top; j++) {
        for (int i = 0; i < val2push.top; i++) {
            if (i + 1 < val2push.top) {  /* make sure we do not exceed index limit of list */
                if (list_get(refcast(val2push), i).where > list_get(refcast(val2push), i + 1).where) {
                    temp = list_get(refcast(val2push), i);
                    list_get(refcast(val2push), i) = list_get(refcast(val2push), i + 1);
                    list_get(refcast(val2push), i + 1) = temp;
                }
            }
        }
    }

    for (int i = 0; i < val2push.top; i++) {
        llist_insert(list,
            list_get(refcast(val2push), i).where + pushed,
            ((struct Token){"#", list_get(refcast(val2push), i).value}));
        pushed++;
    }
    
    puts("");
    TokenLL* it = list;
    while (it->next != NULL) {
        it = it->next;
        list_push(refcast(P->lexed), it->value);
        printf("%s ", it->value.token);
    }

    puts("");
    return 1;
}


/*
** recursive parsing
*/
int parse_expression(Parse_instance* P, unsigned int from, unsigned int to) {
    Tokenlist* stack = new(Tokenlist);
    list_init(stack);

    Token current;
    for (int i = from; i < to; i++) {
        if (i > P->lim) {   /* reached limit: parsing done */
            return 1;
        }
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
                
            case OP_CALLF:
            case T_IDENTIFIER:
            case T_NUMBER: {
                list_push(P->result, current);
            }
                break;

            case OP_IF: {
                /* ... */
                Parse_node rule = get_parse_node(P, OP_IF);
                Offset_list block_list;
                list_init(refcast(block_list));
                Int_list comp;
                list_init(refcast(comp));
                
                get_next(P, i, rule.rule_size);
                
                int* next = NULL;
                for (int j = i; j < to; j++) {
                    next = check_next(P, j, rule.rule_size);    /* check one step at a time */
                    for (int i = 0; i < rule.rule_size; i++) {
                        list_push(refcast(comp), next[i]);
                    }
                }
                
                int valid = check_validity(P, rule, comp);  /* check if syntax i correct */
                if (!valid) {   /* syntax is not valid, throw an error */
                    parse_throw_error(P, PARSE_ERR_SYNTAXERROR);
                } else {    /* syntax is valid, continue */
                    
                }
                printf("valid? %i\n", valid);
            }
                break;

            case OP_WHILE: {
                /* ... */
            }
                break;

            case TOK_LEFT_P: {
                check_next(P, i, 1);
                if (parse_expression(P, i + 1, P->jump-1)) {
                    return 1;
                }
                i = P->jump;
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

    if (!produce(P))
        return 0;
    
    P->lim = P->lexed.top;
    if (!parse_expression(P, 0, P->lexed.top))
        return 0;

    for (int i = 0; i < P->result->top; i++) {
        printf("%s ", list_get(P->result, i).token);
    }
    puts("");

    list_clear2(P->result);
    list_clear2(refcast(lex_instance->result));
    list_clear2(refcast(P->lexed));
    return 1;
}

/*
** check following sequence of code
*/
int* check_next(Parse_instance* P, int index, int steps) {
    list_define(Instruction_list, int);
    Instruction_list list;
    list_init(refcast(list));
    P->jump = 0;    /* do extra jump after call (arrange position for extra tokens) */
    int lex_top = P->lex_instance->result.top;

    int i = index;
    while (steps > 0 && i < lex_top) {    /* while there are steps left */
        int op = list_get(refcast(P->lex_instance->result), i).op;
        switch (op) {
            case OP_ADD:
            case OP_SUB:
            case OP_DIV:
            case OP_MUL:
            case OP_EQ_ASSIGN: {
                debug_printf("op ");
                list_push(refcast(list), OP);
            }
                break;

            case T_NUMBER: {
                debug_printf("number ");
                list_push(refcast(list), OPR);
            }
                break;

            case T_IDENTIFIER: {
                debug_printf("ident ");
                list_push(refcast(list), op);
            }
                break;

            case OP_IF: {
                debug_printf("if ");
                list_push(refcast(list), IF);
            }
                break;

            case OP_WHILE: {
                debug_printf("while ");
                list_push(refcast(list), WHILE);
            }
                break;
                
            /* (...) */
            case TOK_LEFT_P: {
                debug_printf("expr ");
                int delta = 1;  /* how big difference */

                while (i++ < lex_top) {
                    if (list_get(refcast(P->lex_instance->result), i).op == TOK_LEFT_P) {
                        delta++;
                    }

                    if (list_get(refcast(P->lex_instance->result), i).op == TOK_RIGHT_P) {
                        delta--;
                    }

                    if (delta == 0) {
                        P->jump = i;
                        break;
                    }
                }

                if (delta != 0) {
                    list_free(refcast(list));   /* we do not need what we do not use, free it */
                    if(!parse_throw_error(P, PARSE_ERR_BLOCK_NO_MATCH)) {
                        return null;
                    }
                    return null;
                }
                list_push(refcast(list), EXPRESSION);
            }
                break;
                
            /* {...} */
            case TOK_LEFT_CURLY_BRACKET: {
                debug_printf("body ");
                int delta = 1;
                
                while (i++ < lex_top) {
                    if (list_get(refcast(P->lex_instance->result), i).op == TOK_LEFT_CURLY_BRACKET) {
                        delta++;
                    }
                    
                    if (list_get(refcast(P->lex_instance->result), i).op == TOK_RIGHT_CURLY_BRACKET) {
                        delta--;
                    }
                    
                    if (delta == 0) {
                        P->jump = i;
                        break;
                    }
                }
                
                if (delta != 0) {
                    list_free(refcast(list));
                    if(!parse_throw_error(P, PARSE_ERR_BLOCK_NO_MATCH)) {
                        return null;
                    }
                    return null;
                }
                list_push(refcast(list), BODY);
            }
                break;
                
            case OP_END: {
                list_push(refcast(list), END);
            }
                break;
                
            default:
                break;
        }
        P->jump = i;
        steps--;
        i++;
    }

    debug_printf("\n");
    return list.value;
}

/*
** check type at this index
*/
int check_current(Parse_instance* P, int index) {
    int* check, ret;
    check = check_next(P, index, 1);
    ret = *check;
    free(check);
    return ret;
}


int check_validity(Parse_instance* P, Parse_node rule, Int_list comp) {
    for (int i = 0; i < rule.rule_size; i++) {
        if (!(list_get(refcast(comp), i) & rule.rule[i])) {
            return 0;   /* grammatical error */
        }
    }
    return 1;   /* syntax is correct */
}

/*
** this function returns blocks
*/
Offset_list get_next(Parse_instance* P, int index, int steps) {
    Offset_list blocks;
    list_init(refcast(blocks));
    /*
    ** current block type
    */
    int block;
    while (steps > 0) {
        block = check_current(P, index);
        list_push(refcast(blocks), ((Offset){index, P->jump}));
        index = P->jump + 1;
        steps--;
    }
    
    return blocks;
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

int parse_throw_error(Parse_instance* P, unsigned char error) {
    printf("(ParseError) %s\n", parse_error_info[error]);
    return 0;
}


void parse_instance_free(Parse_instance* P) {
    list_free(P->result);
    list_free(P->stack);
    free(P);
}
