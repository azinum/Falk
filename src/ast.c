/* ast.c */
/* Author: Azinum */
/* Date: 2016-10-26 */

#include "ast.h"

/*
** initialize AST node
*/
void ast_node_init(AST_node* node, AST_node* root) {
    node->top = 0;
    node->size = 0;     /* allocated memory for children */
    node->children = NULL;
    node->root = root;
}


/*
** basic value assignment to node
*/
void ast_node_setv(AST_node* node, Token value) {
    node->value = value;
}


/*
** create new child
*/
void ast_node_push_child_value(AST_node* node, AST_node* root, Token value) {
    if (node->top >= node->size) {
        ast_node_realloc(node, 1);
    }
    if (node->children == NULL) {
        ast_node_new_branch(node, root);
    }
    node->children[node->top].root = root;
    node->children[node->top++].value = value;
}


/*
** assign value to child
*/
void ast_node_set_child_value(AST_node* node, Token value, unsigned int index) {
    ast_node_get_child(node, index)->value = value;
}


void ast_node_realloc(AST_node* node, unsigned int size) {
    node->children = (AST_node*)realloc(node->children, node->size + (sizeof(AST_node) * size));
    node->size += sizeof(AST_node) * size;
}


/*
** get child from a node
*/
AST_node* ast_node_get_child(AST_node* node, unsigned int index) {
    if (node == NULL)
        return NULL;
    
    if (index >= node->size || index >= node->top)
        return NULL;
    
    return &node->children[index];
}


/*
** walk and print ast from a node
*/
void ast_walk_ast(AST_node* node, int level) {
    AST_node* child;
    
    int j = level;
    for (int i = 0; i < node->top; i++) {
        child = ast_node_get_child(node, i);
        if (child != NULL) {
            while (j-- > 0)
                printf("   ");
            printf("%s\n", child->value.token);
            if (child->children != NULL) {
                ast_walk_ast(child, level + 1);
            }
            j = level;
        }
    }
}

void ast_node_new_branch(AST_node* node, AST_node* root) {
    node->children = new(AST_node);
    node->top = 0;
    node->size = 1;
    node->root = root;
}
