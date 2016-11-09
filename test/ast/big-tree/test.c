/* test.c */
/* Author: Azinum */
/* Date: 2016-11-09 */

#include <stdio.h>
#include <stdlib.h>

#include "falk.h"
#include "ast.h"


int main(int argc, const char* argv[]) {
    Falk_instance* F = new(Falk_instance);
    if (!falk_instance_init(F)) {
        printf("%s\n", "Falk instance could not be created.");
        return 0;
    }

    F->argc = argc;
    F->argv = argv;

    AST_node* node = new(AST_node);
    do {
        if (!ast_node_init(node, node, node))
            break;

        if (!ast_node_init(node->children, node, node))
            break;

        if (!ast_node_init(node->children->children, node, node))
            break;

        if (!ast_node_init(node->children->children->children, node, node))
            break;

        if (!ast_node_push_child_value(node, node, falk_create_token(string_random_range(5, 'a', 'z'), T_STRING)))
            break;
        if (!ast_node_push_child_value(node, node, falk_create_token(string_random_range(5, 'a', 'z'), T_STRING)))
            break;
        if (!ast_node_push_child_value(node->children, node, falk_create_token(string_random_range(5, 'a', 'z'), T_STRING)))
            break;
        if (!ast_node_push_child_value(node->children, node, falk_create_token(string_random_range(5, 'a', 'z'), T_STRING)))
            break;
        if (!ast_node_push_child_value(&node->children[1], node, falk_create_token(string_random_range(5, 'a', 'z'), T_STRING)))
            break;
        if (!ast_node_push_child_value(&node->children[1], node, falk_create_token(string_random_range(5, 'a', 'z'), T_STRING)))
            break;
        if (!ast_node_push_child_value(&node->children[2], node, falk_create_token(string_random_range(5, 'a', 'z'), T_STRING)))
            break;
        if (!ast_node_push_child_value(node->children->children, node, falk_create_token(string_random_range(5, 'a', 'z'), T_STRING)))
            break;
        if (!ast_node_push_child_value(node->children->children, node, falk_create_token(string_random_range(5, 'a', 'z'), T_STRING)))
            break;
        if (!ast_node_push_child_value(&node->children->children[1], node, falk_create_token(string_random_range(5, 'a', 'z'), T_STRING)))
            break;
        if (!ast_node_push_child_value(node, node, falk_create_token(string_random_range(5, 'a', 'z'), T_STRING)))
            break;
        if (!ast_node_push_child_value(node, node, falk_create_token(string_random_range(5, 'a', 'z'), T_STRING)))
            break;

        ast_print_ast(node, 0);

    } while (0);

    falk_instance_free(F);
    return 1;
}
