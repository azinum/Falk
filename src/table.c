/* table.c */
/* author: Azinum */
/* date: 31/08/16 */


#include "table.h"

int hash(const char* key) {
    int hashvalue = 0;
    
    int i = 0;
    
    while (i++ < (int)strlen(key)) {
        hashvalue <<= 8;
        hashvalue += key[i];
    }
    
    return hashvalue;
}

void* table_find(HashTable* table, char* key) {
    int hashed = hash(key);
    HTValue h;
    for (int i = 0; i < table->top; i++) {
        h = table->items[i];
        if (h.key == hashed) {
            return h.value;
        }
    }
    return NULL;
}