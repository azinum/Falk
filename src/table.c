/* table.c */
/* author: Azinum */
/* date: 31/08/16 */


#include "table.h"

/*
** string to number hash
*/
unsigned long hash(const char* key) {
    unsigned long hashvalue = 5381;
    
    int i = 0;
    
    while (i < (int)strlen(key)) {
        hashvalue <<= 8;
        hashvalue += (int)key[i];
        i++;
    }
    
    return hashvalue;
}

/*
** find item in table, return NULL if not found
*/
HTValue* table_find(HashTable* table, char* key) {
    unsigned long hashed = hash(key);
    HTValue* h;
    for (int i = 0; i < table->top; i++) {
        h = &table->items[i];
        if (h->key == hashed) {
            return h;
        }
    }
    return NULL;
}

/*
** we can now use table as standard list
*/
HTValue* table_get(HashTable* table, int index) {
    if (table_is_safe(table) && index <= table->top) {
        return &table->items[index];
    }
    return NULL;
}

/*
** replace value from key
** will return 0 if no key match
*/
unsigned char table_replace(HashTable* table, char* key, void* value) {
    HTValue* item;
    if ((item = table_find(table, key)) != NULL) {  /* find item in table */
        /* key is matching */
        item->value = value;
        return 1;
    }
    return 0;
}
