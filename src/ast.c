/* ast.c */
/* Author: Azinum */
/* Date: 2016-10-26 */

#include "ast.h"

/*
** initialize AST node
*/
void ast_node_init(AST_node* node, AST_node* root) {
    node->top = 0;
    node->size = 1;     /* allocated memory for children */
    node->children = new(AST_node);
    node->children->children = NULL;
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
    if (!node) {
        ast_node_throw_error(node, AST_ERR_NULL);
        return;
    }
    
    if (node->top >= node->size) {
        ast_node_realloc(node, 1);
    }
    
    if (node->children == NULL) {
        ast_node_init(node, root);
    }
    
    node->children[node->top].root = root;
    node->children[node->top++].value = value;
    
    /* NULL terminate last child */
    AST_node* tmp = &node->children[node->top];
    tmp = NULL;
    
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
    if (!node) {
        ast_node_throw_error(node, AST_ERR_NULL);
        return NULL;
    }
    
    if (index >= node->size || index >= node->top) {
        ast_node_throw_error(node, AST_ERR_INVALID_INDEX);
        return NULL;
    }
    
    if (!&node->children[index]) {
        ast_node_throw_error(node, AST_ERR_CHILD_NOT_FOUND);
        return NULL;
    }
    
    return &node->children[index];
}

/*
**
** Root
** |
** A---B    # (1) print A node. (3) continue on to B node
** |
** a---b---c    # (2) go to child node.
**
*/
void ast_print_ast(AST_node* node, int level) {
    if (!node) {
        ast_node_throw_error(node, AST_ERR_NULL);
        return;
    }
}


void ast_print_branch(AST_node* node) {
    if (!node) {
        ast_node_throw_error(node, AST_ERR_NULL);
        return;
    }
    
    if (!node->children) {
        ast_node_throw_error(node, AST_ERR_NO_CHILDREN);
        return;
    }
    
    for (int i = 0; i < node->top; i++) {
        printf("%s\n", node->children[i].value.token);
    }
}


void ast_node_print_node(AST_node* node) {
    if (!node) {
        ast_node_throw_error(node, AST_ERR_NULL);
        return;
    }
    printf("%s\n", node->value.token);
}


void ast_node_throw_error(AST_node* node, int error) {
    printf("AST Error: %s. @Node: %p.\n", AST_error_messages[error], node);
}
