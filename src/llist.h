/* llist.h */
/* Author: Azinum */
/* Date: 2016-09-15 */

#ifndef llist_h
#define llist_h

#include <stdio.h>


#define llist_new(NAME, LL) \
LL* NAME = new(LL); \
llist_init(NAME)

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
    do { \
        int count = 0; \
        typeof(*LLIST)* current = LLIST; \
        while ((current->next != NULL)) { \
            if (count == INDEX) { \
                typeof(*LLIST)* toassign = new(typeof(*LLIST)); \
                toassign->prev = current; \
                toassign->next = current->next; \
                current->next = toassign; \
                toassign->value = VALUE; \
                break; \
            } \
            current = current->next; \
            count++; \
        } \
        if (current->next == NULL) { \
            current->next = new(typeof(*LLIST)); \
            current->next->value = VALUE; \
            current->next->next = NULL; \
            break; \
        } \
    } while (0); \
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


/*
** remove last item in list
*/
#define llist_pop(LLIST) { \
    typeof(*LLIST)* current = LLIST; \
    while (1) { \
        if (current->next == NULL) { \
            current->prev->next = NULL; \
            free(current); \
            current = NULL; \
            break; \
        } \
        current = current->next; \
    } \
}





#endif /* llist_h */
