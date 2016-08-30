/* main.c */
/* author: Azinum */
/* date: 30/08/16 */

#include "falk.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char* argv[]) {
    String* string = new(String);
    list_init(string, char);
    for (int i = 0; i < 24; i++) {
        list_push(string, char, 'A');
        printf("%s\n", string->value);
    }
    return 0;
}
