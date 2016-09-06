/* main.c */
/* author: Azinum */
/* date: 30/08/16 */

#include "falk.h"
#include "table.h"
#include <stdio.h>
#include <stdlib.h>


int main(int argc, const char* argv[]) {
    int number = 7;
    HashTable* test = new(HashTable);
    table_init(test);
    table_push(test, "Hello", &number);
    void* toprint;
    if ((toprint = table_find(test, "Hello"))) {
        printf("%i\n", *(int*)toprint);
    }
    /*
    Falk_instance* F = new(Falk_instance);
    F->argc = argc;
    F->argv = argv;
    falk_instance_init(F);
    falk_getinput(F);
*/
    return 0;
}
