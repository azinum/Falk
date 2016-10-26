/* table.h */
/* Author: Azinum */
/* Date: 2016-08-31 */


#ifndef table_h
#define table_h

#include <stdio.h>
#include "object.h"
#include "list.h"

#define table_init(TABLE) \
TABLE->size = 1; \
TABLE->top = 0; \
TABLE->items = new(TValue)


#define table_is_safe(TABLE) (TABLE->size > TABLE->top)

#define table_realloc(TABLE, SIZE) \
TABLE->items = (TValue*)realloc(TABLE->items, (TABLE->size * sizeof(TValue)) + ((SIZE + 1) * sizeof(TValue))); \
TABLE->size += SIZE

#define tobject_create(NAME, VALUE, TYPE) \
Object NAME; \
NAME.value.VALUE; \
NAME.type = TYPE;

#define table_push_object(TABLE, KEY, VALUE, TYPE) { \
Object obj; \
obj.value.VALUE; \
obj.type = TYPE; \
table_push(TABLE, KEY, obj); \
}

/*
** key and value for hashtable
** Table Value
*/
typedef struct TValue {
    unsigned long key;
    struct Object tval;
} TValue;

/*
** simple hashtable implementation
*/
typedef struct HashTable {
    unsigned int size;
    unsigned int top;
    TValue* items;
} HashTable;


unsigned long hash(const char* key);

TValue* table_find(HashTable* table, char* key);

TValue* table_get(HashTable* table, int index);

unsigned char table_push(HashTable* table, char* key, Object value);

unsigned char table_replace(HashTable* table, char* key, Object value);

#endif /* table_h */
