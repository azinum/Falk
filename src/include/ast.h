/* ast.h */
/* Author: Azinum */
/* Date: 2016-10-26 */
    
#ifndef ast_h
#define ast_h

#include <stdio.h>
#include "object.h"
#include "conf.h"

extern_c

typedef struct AST_node {
    struct AST_node* root;
    struct AST_node** children;
    unsigned int top, size;
    Token value;
} AST_node;

enum AST_node_types {
    AST_NODE_EXPR,      /* (...) */
    AST_NODE_BODY,      /* {...} */
};

static const char* ast_node_identifiers[] = {
    "expression",
    "body"
};

int ast_node_init(AST_node* node, AST_node* root);

int ast_node_push_child(AST_node* node, AST_node* child);

AST_node* ast_node_create_child(Token value);

int ast_node_realloc(AST_node* node);

int ast_node_print_node(AST_node* node);

AST_node* ast_node_get_child(AST_node* node, unsigned int index);

int ast_node_set_value(AST_node* node, Token value);

int ast_walk_ast(AST_node* node, int level);

int ast_node_swap(AST_node* node, int first, int second);

int ast_node_pop(AST_node* node);

extern_c_end

#endif /* ast_h */
