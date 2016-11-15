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
    return 1;
}


int parse(Parse_instance* P, char* input) {
    AST_node* node = new(AST_node);
    ast_node_init(node, node);
    
    if (!lex(P->lex_instance, input))
        return 0;
    
    node = parse_tree(P, node, 0, P->lex_instance->result.top);
    
    ast_walk_ast(node, 0);
    
    return 1;
}

AST_node* parse_tree(Parse_instance* P, AST_node* node, unsigned int from, unsigned int to) {
    if (!node || from > to) {
        return NULL;
    }
    
    Tokenlist lexed = P->lex_instance->result;
    
    for (unsigned int i = from; i < to; i++) {
        switch (lexed.value[i].type) {
                
            case TOK_LEFT_P: {
                
            }
                break;
            
            case OP_ADD:
            case OP_SUB:
            case OP_MUL:
            case OP_DIV:
            case OP_EQ:
                
            case OP_LT:
            case OP_GT:
            case OP_LEQ:
            case OP_GEQ:
            
            case OP_EQ_ASSIGN:
            case OP_ADD_ASSIGN:
            case OP_SUB_ASSIGN:
            case OP_MUL_ASSIGN:
            case OP_DIV_ASSIGN:
            
            case T_NUMBER:
            case T_STRING: {
                ast_node_push_child(node, ast_node_create_child(lexed.value[i]));
            }
                break;
                
            default:
                break;
        }
    }
    
    return node;
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

int parse_throw_error(Parse_instance* P, unsigned char error) {
    printf("(ParseError) %s\n", parse_error_info[error]);
    return 0;
}


void parse_instance_free(Parse_instance* P) {
    list_free(P->result);
    list_free(P->stack);
    free(P);
}
