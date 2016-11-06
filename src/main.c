/* main.c */
/* Author: Azinum */
/* Date: 2016-08-30 */

#include <stdio.h>
#include <stdlib.h>

#include "falk.h"
#include "ast.h"

int main(int argc, const char* argv[]) {
    Falk_instance* F = new(Falk_instance);
    falk_instance_init(F);
    
    AST_node* node = new(AST_node);
    ast_node_init(node, node);
    ast_node_push_child_value(node, node, falk_create_token("A", T_STRING));
    ast_node_push_child_value(node, node, falk_create_token("B", T_STRING));
    ast_node_new_branch(node->children, node);
    ast_node_push_child_value(ast_node_get_child(node, 0), node, falk_create_token("a", T_STRING));
    ast_node_push_child_value(ast_node_get_child(node, 0), node, falk_create_token("b", T_STRING));
    
    int it = 100;
    while (it-- > 0)        /* walk ast 100 times */
        ast_walk_ast(node, 0);
    
    /* Falk_instance* F = new(Falk_instance);
    F->argc = argc;
    F->argv = argv;
    falk_instance_init(F);
    falk_execute(F);
    */
    return 0;
}
