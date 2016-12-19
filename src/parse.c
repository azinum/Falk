/* parse.c */
/* Author: Azinum */
/* Date: 2016-09-05 */


#include "include/parse.h"
#include "include/falk.h"

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
    /* AST_node* node = new(AST_node);
    ast_node_init(node, node); */
    
    if (!lex(P->lex_instance, input))
        return 0;
    
    if (!parse_asm(P, input))
        return 0;
    
    /* node = parse_tree(P, node, 0, P->lex_instance->result.top);
    ast_walk_ast(node, 0); */
    
    return 1;
}

AST_node* parse_tree(Parse_instance* P, AST_node* node, unsigned int from, unsigned int to) {
    if (!node || !P || (from > to)) {
        return NULL;
    }
    
    Tokenlist lexed = P->lex_instance->result;
    int branchc = 0;    /* branch count (how many branches on this node) */
    
    for (unsigned int i = from; i < to; i++) {
        switch (lexed.value[i].type) {
            
            case TOK_LEFT_CURLY_BRACKET:
            case TOK_LEFT_P: {
                int type = lexed.value[i].type;
                int begin = i + 1;
                int end = begin;
                int offset = 1;
                
                if (type == TOK_LEFT_P) {
                    ast_node_push_child(node, ast_node_create_child((Token){"expr", AST_NODE_EXPR}));
                }
                if (type == TOK_LEFT_CURLY_BRACKET) {
                    ast_node_push_child(node, ast_node_create_child((Token){"body", AST_NODE_BODY}));
                }
                
                while (++i < to) {
                    if (lexed.value[i].type == type)
                        offset++;
                    
                    if (lexed.value[i].type == type + 1)    /* matching token => type + 1 */
                        offset--;
                    
                    if (offset == 0) {
                        end = i;
                        break;
                    }
                }
                
                AST_node* temp = ast_node_get_child(node, branchc);
                
                if (temp) {
                    if (!node->children) {
                        node->children = new(AST_node*);
                    }
                    parse_tree(P, temp, begin, end);
                }
            }
                break;
                
            default:
                ast_node_push_child(node, ast_node_create_child(lexed.value[i]));
                branchc++;
                break;
        }
    }
    
    return node;
}

int parse_asm(Parse_instance* P, char* input) {
    
    return 1;
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
