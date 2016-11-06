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


void ast_node_init(AST_node* node, AST_node* root);

void ast_node_setv(AST_node* node, Token value);

void ast_node_push_child_value(AST_node* node, AST_node* root, Token value);

void ast_node_set_child_value(AST_node* node, Token value, unsigned int index);

void ast_node_realloc(AST_node* node, unsigned int size);

AST_node* ast_node_get_child(AST_node* node, unsigned int index);

void ast_walk_ast(AST_node* node, int level);

void ast_node_new_branch(AST_node* node, AST_node* root);

#endif /* ast_h */
