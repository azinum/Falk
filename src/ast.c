/* ast.c */
/* Author: Azinum */
/* Date: 2016-10-26 */

#include "ast.h"
#include "io.h"

int ast_node_init(AST_node* node, AST_node* root) {
    if (!node) {
        node = new(AST_node);
    }
    node->top = 0;
    node->size = 0;
    node->children = NULL;
    node->root = root;
    node->value = falk_create_token("", T_NULL);
    return 1;
}


int ast_node_push_child(AST_node* node, AST_node* child) {
    if (!node) {
        return 0;
    }
    
    if (!node->children) {
        node->children = new(AST_node*);
    }
    
    if (node->top >= node->size) {
        ast_node_realloc(node);
    }
    
    node->children[node->top++] = child;
    return 1;
}


AST_node* ast_node_create_child(Token value) {
    AST_node* ret = new(AST_node);
    ast_node_init(ret, NULL);
    ret->value = value;
    return ret;
}


/*
** allocate block for 1 extra child node
*/
int ast_node_realloc(AST_node* node) {
    if (!node) {
        return 0;
    }
    if (!node->children) {
        return 0;
    }
    
    node->children = (AST_node**)realloc(node->children, sizeof(AST_node*) * (1 + node->size));
    node->children[node->top] = new(AST_node);
    ast_node_init(node->children[node->top], node);
    node->size++;
    return 1;
}


/*
** print only this node
*/
int ast_node_print_node(AST_node* node) {
    if (!node) {
        return 0;
    }
    printf("%s\n", node->value.value);
    return 1;
}


AST_node* ast_node_get_child(AST_node* node, unsigned int index) {
    if (!node) {
        return NULL;
    }
    if (node->top < index) {
        return NULL;
    }
    if (!node->children) {
        return NULL;
    }
    
    return node->children[index];
}


int ast_node_set_value(AST_node* node, Token value) {
    if (!node) {
        return 0;
    }
    node->value = value;
    return 1;
}


/*
** fully walk through tree and print it
*/
int ast_walk_ast(AST_node* node, int level) {
    if (!node) {
        return 0;
    }
    
    for (int i = 0; i < node->top; i++) {
        printx("--", level);
        ast_node_print_node(node->children[i]);
        ast_walk_ast(node->children[i], level + 1);
    }
    
    return 1;
}


/*
** swap 2 child nodes
*/
int ast_node_swap(AST_node* node, int first, int second) {
    if (!node) {
        return 0;
    }
    if (!node->children) {
        return 0;
    }
    if (first > node->top || second > node->top) {
        return 0;
    }
    
    AST_node* temp = node->children[first];
    node->children[first] = node->children[second];
    node->children[second] = temp;
    
    return 1;
}


/*
** pop last child on node
*/
int ast_node_pop(AST_node* node) {
    if (!node) {
        return 0;
    }
    if (!node->children) {
        return 0;
    }
    
    return 1;
}
