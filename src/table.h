/* table.h */
/* author: Azinum */
/* date: 31/08/16 */


#ifndef table_h
#define table_h

#include <stdio.h>
#include "object.h"
#include "list.h"

#define table_define(NAME, TYPE) \
typedef struct NAME { \
    int key; \
    TYPE value; \
} NAME

#define table_init(TABLE) \
TABLE->size = 1; \
TABLE->top = 0; \
TABLE->items = new(HTValue)


#define table_realloc(TABLE, SIZE) \
TABLE->items = (typeof(*TABLE->items)*)realloc(TABLE->items, (TABLE->size * sizeof(typeof(*TABLE->items))) + ((SIZE + 1) * sizeof(typeof(*TABLE->items)))); \
TABLE->size += SIZE

#define table_push(TABLE, KEY, VALUE) \
table_realloc(TABLE, 1); \
TABLE->items[TABLE->top].value = new(typeof(VALUE)); \
TABLE->items[TABLE->top].value = VALUE; \
TABLE->items[TABLE->top].key = hash(KEY); \
TABLE->top++

#define table_is_safe(TABLE) (TABLE->size > TABLE->top)

/*
** key and value for hashtable
** value can be anything, need to be converted on use
** e.g:
**     *(int*)value = 5;
*/
typedef struct HTValue {
    int key;
    void* value;
} HTValue;

/*
** simple hashtable implementation
*/
typedef struct HashTable {
    unsigned int size;
    unsigned int top;
    HTValue* items;
} HashTable;


int hash(const char* key);

void* table_find(HashTable* table, char* key);

void* table_get(HashTable* table, int index);

#endif /* table_h */
