/* table.c */
/* author: Azinum */
/* date: 31/08/16 */


#include "table.h"

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

void* table_find(HashTable* table, char* key) {
    unsigned long hashed = hash(key);
    HTValue h;
    for (int i = 0; i < table->top; i++) {
        h = table->items[i];
        if (h.key == hashed) {
            return h.value;
        }
    }
    return NULL;
}

/*
** we can now use table as standard list
*/
void* table_get(HashTable* table, int index) {
    if (table_is_safe(table) && index <= table->top) {
        return table->items[index].value;
    }
    return NULL;
}