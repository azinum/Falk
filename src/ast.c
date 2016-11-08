/* ast.c */
/* Author: Azinum */
/* Date: 2016-10-26 */

#include "ast.h"

/*
** initialize AST node
*/
void ast_node_init(AST_node* node, AST_node* root) {
    if (!node) {
        ast_node_throw_error(node, AST_ERR_NULL, "ast_node_init");
        return;
    }
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
        ast_node_throw_error(node, AST_ERR_NULL, "ast_node_push_child_value");
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
    AST_node* child = ast_node_get_child(node, index);
    if (child)
        child->value = value;
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
        ast_node_throw_error(node, AST_ERR_NULL, "ast_node_get_child");
        return NULL;
    }
    
    if (index >= node->size || index >= node->top) {
        ast_node_throw_error(node, AST_ERR_INVALID_INDEX, "ast_node_get_child");
        return NULL;
    }
    
    if (!&node->children[index]) {
        ast_node_throw_error(node, AST_ERR_CHILD_NOT_FOUND, "ast_node_get_child");
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
int ast_print_ast(AST_node* node, int level) {
    if (!node) {
        ast_node_throw_error(node, AST_ERR_NULL, "ast_print_ast");
        return 0;
    }
    
    AST_node* it;
    int j = level;
    
    for (int i = 0; i < node->top; i++) {
        it = ast_node_get_child(node, i);
        
        while (j--)
            printf("---");
        
        printf("%s\n", it->value.value);
        if (it->children) {
            if (!ast_print_ast(it, level + 1))
                continue;
        }
        j = level;
    }
    
    return 1;
}

/*
** print children of node
*/
void ast_print_branch(AST_node* node) {
    if (!node) {
        ast_node_throw_error(node, AST_ERR_NULL, "ast_print_branch");
        return;
    }
    
    if (!node->children) {
        ast_node_throw_error(node, AST_ERR_NO_CHILDREN, "ast_print_branch");
        return;
    }
    
    for (int i = 0; i < node->top; i++) {
        printf("%s\n", node->children[i].value.value);
    }
}


/*
** print node value, throw error if node is null
*/
void ast_node_print_node(AST_node* node) {
    if (!node) {
        ast_node_throw_error(node, AST_ERR_NULL, "ast_node_print_node");
        return;
    }
    printf("%s\n", node->value.value);
}


/*
** print error message
** what error
** at which node (addr) and function (name)
*/
void ast_node_throw_error(AST_node* node, int error, const char* function) {
    printf("AST Error: %s. @Node: %p, @function: %s.\n", AST_error_messages[error], node, function);
}
