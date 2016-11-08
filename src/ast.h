/* ast.h */
/* Author: Azinum */
/* Date: 2016-10-26 */

#ifndef ast_h
#define ast_h

#include <stdio.h>
#include "object.h"

typedef struct AST_node {
    struct AST_node* root, *children;
    unsigned int top, size;
    Token value;
} AST_node;


enum AST_errors {
    AST_ERR_NONE,
    AST_ERR_NULL,
    AST_ERR_NO_CHILDREN,
    AST_ERR_CHILD_NOT_FOUND,
    AST_ERR_INVALID_INDEX,
};

static const char* AST_error_messages[] = {
    "",
    "Node is null",
    "Node is childless",
    "Child not found in this node",
    "Index out of range"
};

void ast_node_init(AST_node* node, AST_node* root);

void ast_node_setv(AST_node* node, Token value);

void ast_node_push_child_value(AST_node* node, AST_node* root, Token value);

void ast_node_set_child_value(AST_node* node, Token value, unsigned int index);

void ast_node_realloc(AST_node* node, unsigned int size);

AST_node* ast_node_get_child(AST_node* node, unsigned int index);

int ast_print_ast(AST_node* node, int level);

void ast_print_branch(AST_node* node);

void ast_node_print_node(AST_node* node);

void ast_node_throw_error(AST_node* node, int error, const char* function);

#endif /* ast_h */