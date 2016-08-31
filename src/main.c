/* main.c */
/* author: Azinum */
/* date: 30/08/16 */

#include "falk.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char* argv[]) {
    Lex_instance* L = new(Lex_instance);
    lex_instance_init(L);
    
    lex_instance_free(L);
    return 0;
}
