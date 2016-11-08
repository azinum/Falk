/* test.c */
/* Author: Azinum */
/* Date: 2016-11-08*/

#include "falk.h"
#include "ast.h"

int main() {
    Falk_instance* F = new(Falk_instance);
    falk_instance_init(F);
    
    AST_node* node = new(AST_node);
    ast_node_init(node, node);
    ast_node_init(node->children, node);
    ast_node_push_child_value(node, node, falk_create_token("A", T_STRING));
    ast_node_push_child_value(node, node, falk_create_token("B", T_STRING));
    ast_node_push_child_value(node->children, node, falk_create_token("a", T_STRING));
    ast_node_push_child_value(node->children, node, falk_create_token("a", T_STRING));
    
    ast_print_ast(node, 0);
}
