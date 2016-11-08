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

    AST_node* it = node;
    int i = 10000000;
    while (i--) {
        ast_node_push_child_value(it, node, falk_create_token("a", T_STRING));
        ast_node_init(it->children, node);
        it = it->children;
    }
}
