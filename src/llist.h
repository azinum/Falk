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
** add new item to linked list
*/
#define llist_push(LLIST, VALUE) { \
    typeof(*LLIST)* current##LLIST = LLIST; \
    typeof(*LLIST)* prev##LLIST = LLIST; \
    while ((current##LLIST->next != NULL)) { \
        prev##LLIST = current##LLIST; \
        current##LLIST = current##LLIST->next; \
    } \
    current##LLIST->next = new(typeof(*LLIST)); \
    current##LLIST->next->value = VALUE; \
    current##LLIST->next->next = NULL; \
    current##LLIST->next->prev = prev##LLIST; \
}


/*
** insert value at specific index on list
** TODO: add some error checking
*/
#define llist_insert(LLIST, INDEX, VALUE) { \
    int count = 0; \
    typeof(*LLIST)* current##LLIST = LLIST; \
    while ((current##LLIST->next != NULL)) { \
        current##LLIST = current##LLIST->next; \
        count++; \
        if (count == INDEX) { \
            typeof(*LLIST)* toassign = new(typeof(*LLIST)); \
            toassign->prev = current##LLIST; \
            toassign->next = current##LLIST->next; \
            current##LLIST->next = toassign; \
            toassign->value = VALUE; \
        } \
    }\
}

/*
** pop last item in list
*/
#define llist_pop(LLIST)

#endif /* llist_h */
