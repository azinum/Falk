/* llist.h */
/* author: Azinum */
/* date: 15/09/16 */

#ifndef llist_h
#define llist_h

#include <stdio.h>

/*
** define doubly linked list
*/
#define llist_define(NAME, TYPE) \
typedef struct NAME { \
    struct NAME* next, *prev; \
    TYPE value; \
} NAME; \


/*
** initialize a doubly linked list
*/
#define llist_init(LLIST) \
LLIST->next = NULL; \
LLIST->prev = NULL; \

/*
** create a new node
*/
#define llist_create_node(NODE) \
NODE->next, NODE->prev = new(typeof(*NODE)); \

/*
** add new item to linked list
*/
#define llist_push(LLIST, VALUE) { \
    typeof(*LLIST)* newnode = new(typeof(*LLIST)); \
    typeof(*LLIST)* current = LLIST; \
    if (LLIST == NULL) { \
        LLIST = newnode; \
    } \
    while ((current->next != NULL)) { \
        current = current->next; \
    } \
    newnode->next = NULL; \
    current->next = newnode; \
    newnode->prev = current; \
    newnode->value = VALUE; \
}

/*
** insert value at specific index on list
** TODO: add some error checking
*/
#define llist_insert(LLIST, INDEX, VALUE) { \
    int count = 0; \
    typeof(*LLIST)* current##LLIST = LLIST; \
    while ((current##LLIST->next != NULL)) { \
        if (count == INDEX) { \
            typeof(*LLIST)* toassign = new(typeof(*LLIST)); \
            toassign->prev = current##LLIST; \
            toassign->next = current##LLIST->next; \
            current##LLIST->next = toassign; \
            toassign->value = VALUE; \
        } \
        current##LLIST = current##LLIST->next; \
        count++; \
    }\
}


/*
** remove a link from list at an index
** do {...} while(0); is used so that we can break out of macro scope
*/
#define llist_erase(LLIST, INDEX) { \
    do { \
        typeof(*LLIST)* current = LLIST; \
        int count = 0; \
        if (INDEX < count) break; \
        while (current->next != NULL) { \
            current = current->next; \
            if (count == INDEX) { \
                if (current->prev != NULL) { \
                    if (current->next != NULL) \
                        current->prev->next = current->next; \
                    else \
                        current->prev->next = NULL; \
                } \
                if (current->next != NULL) { \
                    if (current->prev != NULL) { \
                        current->next->prev = current->prev; \
                    } else \
                        current->next = NULL; \
                } \
                free(current); \
                current = NULL; \
                break; \
            } \
            count++; \
        } \
    } while(0); \
}

#endif /* llist_h */
