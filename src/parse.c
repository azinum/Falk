/* parse.c */
/* Author: Azinum */
/* Date: 2016-09-05 */


#include "parse.h"
#include "falk.h"

int parse_instance_init(Parse_instance* P) {
    if (!P) {
        return 0;
    }
    P->error = PARSE_NO_ERROR;
    P->warning = 0;
    P->line = 0;
    P->result = new(Tokenlist);
    list_init(P->result);
    P->stack = new(Tokenlist);
    list_init(P->stack);
    P->jump = 0;
    list_init((&P->lexed));
    P->exit_on_error = 1;

    /*
    ** temporary way of initializing rules
    */
    /* if */
    parse_rules[5].rule_size = 3;
    parse_rules[5].rule = intarr_create(parse_rules[5].rule_size, OP_IF, EXPRESSION, BODY | END);     /* rule for if */
    parse_rules[5].prio = intarr_create(3, 1, 2, 0);    /* high -> low */
    /* while */
    parse_rules[6].rule_size = 3;
    parse_rules[6].rule = intarr_create(parse_rules[6].rule_size, OP_WHILE, EXPRESSION, BODY | END);
    parse_rules[6].prio = intarr_create(3, 1, 2, 0);
    return 1;
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

Parse_rule get_parse_node(Parse_instance* P, int type) {
    Parse_rule node = {0, 0, 0, NULL, NULL};

    for (int i = 0; i < arr_size(parse_rules); i++) {
        if (parse_rules[i].type == type) {
            node = parse_rules[i];
        }
    }
    return node;
}

void check_precedence(Parse_instance* P, Tokenlist* stack) {
    if (stack->top > 2) {
        Parse_rule op1, op2;
        op1 = get_parse_node(P, list_get_top(stack).type);
        op2 = get_parse_node(P, list_get_from_top(stack, -1).type);
        if (is_op(op1.type) && is_op(op2.type)) {
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
    list_init((&val2push));

    for (int i = 0; i < P->lex_instance->result.top; i++) {
        llist_push(list, list_get((&P->lex_instance->result), i));
    }

    int* check;
    int pushed = 0;     /* extra tokens pushed */
    PRule rule;
    Intlist topush;
    list_init((&topush));

    for (int i = 0; i < P->lex_instance->result.top; i++) {
        if (!(check = check_next(P, P->lex_instance->result, i, 2))) {    /* to make sure we use a valid rule */
            continue;
        }
        rule = get_prod_rule(check[0], check[1]);

        if (rule.value != 0) {  /* for when rule is not null */
            list_push((&val2push), ((struct Value2push){rule.value, P->jump + 1}));
        }
    }

    /*
    ** sort list based on 'where' variable (low -> high)
    */
    struct Value2push temp;
    for (int j = 0; j < val2push.top; j++) {
        for (int i = 0; i < val2push.top; i++) {
            if (i + 1 < val2push.top) {  /* make sure we do not exceed index limit of list */
                if (list_get((&val2push), i).where > list_get((&val2push), i + 1).where) {
                    temp = list_get((&val2push), i);
                    list_get((&val2push), i) = list_get((&val2push), i + 1);
                    list_get((&val2push), i + 1) = temp;
                }
            }
        }
    }

    for (int i = 0; i < val2push.top; i++) {
        llist_insert(list,
            list_get((&val2push), i).where + pushed,
            ((struct Token){"#", list_get((&val2push), i).value}));
        pushed++;
    }


    TokenLL* it = list;
    while (it->next != NULL) {
        it = it->next;
        list_push((&P->lexed), it->value);
    }

    return 1;
}


/*
** recursive parsing
*/
int parse_expression(Parse_instance* P, unsigned int from, unsigned int to) {
    Tokenlist* stack = new(Tokenlist);
    list_init(stack);

    Token current;
    if (from <= to) {
        for (int i = from;; i++) {
            current = list_get((&P->lexed), i);
//            printf("> %s\n", current.token);
            switch (current.type) {
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

                /*
                ** TODO: need to structure this up
                */
                case OP_WHILE:
                case OP_IF: {
                    /* ... */
                    Parse_rule rule = get_parse_node(P, current.type);
                    Offset_list block_list, block_list_unsorted;
                    list_init((&block_list));
                    list_init((&block_list_unsorted));
                    Int_list comp;
                    list_init((&comp));

                    block_list = get_next(P, P->lexed, i, rule.rule_size);
                    block_list_unsorted = block_list;

                    int* next = NULL;
                    for (int j = i; j < to; j++) {
                        next = check_next(P, P->lexed, j, rule.rule_size);    /* check one step at a time */
                        for (int i = 0; i < rule.rule_size; i++) {
                            list_push((&comp), next[i]);
                        }
                    }

                    int valid = check_validity(P, rule, comp);  /* check if syntax is correct */
                    if (!valid) {   /* syntax is not valid, throw an error */
                        parse_throw_error(P, PARSE_ERR_SYNTAXERROR);
                        if (P->exit_on_error)
                            return 0;
                    } else {    /* syntax is valid, continue */
                        /* sort for which order blocks is parsed */
                        Offset temp_block;
                        for (int j = 0; j < block_list.top; j++) {
                            for (int i = 0; i < block_list.top; i++) {
                                if (rule.prio[i] < rule.prio[i + 1]) {
                                    temp_block = list_get((&block_list), i);
                                    list_get((&block_list), i) = list_get((&block_list), i + 1);
                                    list_get((&block_list), i + 1) = temp_block;
                                }
                            }
                        }
                    }

                    for (int i = 0; i < block_list.top; i++) {
                        Offset block = list_get((&block_list), i);
                        if (check_current(P, P->lexed, block.x) == current.type) {
                            list_push(P->result, current);  /* do not parse same rule again */
                            continue;
                        }
                        printf("(%i, %i)\n", block.x, block.y);
                        
                        if (!parse_expression(P, block.x, block.y)) {
                            return 0;
                        }
                    }
                    i = list_get_top((&block_list_unsorted)).y;   /* jump to last block */
                }
                    break;

                case TOK_LEFT_P: {
                    list_push(stack, current);
                    /*
                    check_next(P, i, 1);
                    if (parse_expression(P, i + 1, P->jump-1)) {
                        return 1;
                    }
                    i = P->jump;
                    */
                }
                    break;

                case TOK_RIGHT_P: {
                    while (stack->top > 0) {
                        if (list_get_top(stack).type == TOK_LEFT_P) {
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
            
            if (!(i < to)) {
                break;
            }
        }
    }
    while (stack->top > 0) {
        list_push(P->result, list_get_top(stack));
        list_pop2(stack);
    }

    free(stack);

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
        printf("%s ", list_get(P->result, i).value);
    }
    puts("");

    list_clear2(P->result);
    list_clear2((&lex_instance->result));
    list_clear2((&P->lexed));
    return 1;
}

/*
** check following sequence of code
*/
int* check_next(Parse_instance* P, Tokenlist what, int index, int steps) {
    Int_list list;
    list_init((&list));
    P->jump = 0;    /* do extra jump after call (arrange position for extra tokens) */
    int lex_top = what.top;

    int i = index;
    while (steps > 0 && i < lex_top) {    /* while there are steps left */
        int op = list_get((&what), i).type;
        switch (op) {
            case OP_ADD:
            case OP_SUB:
            case OP_DIV:
            case OP_MUL:
            case OP_EQ_ASSIGN: {
                debug_printf("op ");
                list_push((&list), OP);
            }
                break;

            case T_NUMBER: {
                debug_printf("number ");
                list_push((&list), OPR);
            }
                break;

            case T_IDENTIFIER: {
                debug_printf("ident ");
                list_push((&list), op);
            }
                break;

            case OP_IF: {
                debug_printf("if ");
                list_push((&list), OP_IF);
            }
                break;

            case OP_WHILE: {
                debug_printf("while ");
                list_push((&list), OP_WHILE);
            }
                break;

            /* (...) & {...} */
            case TOK_LEFT_CURLY_BRACKET:
            case TOK_LEFT_P: {
                debug_printf("block ");
                int delta = 0;  /* how big difference */

                while (i < lex_top) {
                    if (list_get((&what), i).type == op) {
                        delta++;
                    }

                    if (list_get((&what), i).type == op + 1 /* matching token */) {
                        delta--;
                    }

                    if (delta == 0) {
                        P->jump = i;
                        break;
                    }
                    i++;
                }

                if (delta != 0) {
                    list_free((&list));   /* we do not need what we do not use, free it */
                    if(!parse_throw_error(P, PARSE_ERR_BLOCK_NO_MATCH)) {
                        return null;
                    }
                    return null;
                }
                
                if (op == TOK_LEFT_P)   /* ( --> expression */
                    list_push((&list), EXPRESSION);
                
                if (op == TOK_LEFT_CURLY_BRACKET)   /* { --> body */
                    list_push((&list), BODY);
            }
                break;

            case OP_END: {
                list_push((&list), END);
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
int check_current(Parse_instance* P, Tokenlist what, int index) {
    int* check, ret;
    check = check_next(P, what, index, 1);
    ret = *check;
    free(check);
    return ret;
}

/*
** basic grammar checking
*/
int check_validity(Parse_instance* P, Parse_rule rule, Int_list comp) {
    for (int i = 0; i < rule.rule_size; i++) {
        if (!(list_get((&comp), i) & rule.rule[i])) {
            return 0;   /* grammatical error */
        }
    }
    return 1;   /* syntax is correct */
}

/*
** this function returns blocks
*/
Offset_list get_next(Parse_instance* P, Tokenlist what, int index, int steps) {
    Offset_list blocks;
    list_init((&blocks));
    /*
    ** current block type
    */
    int block;
    while (steps > 0) {
        block = check_current(P, what, index);
        list_push((&blocks), ((Offset){index, P->jump}));
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
