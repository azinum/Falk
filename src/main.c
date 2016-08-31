/* main.c */
/* author: Azinum */
/* date: 30/08/16 */

#include "falk.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char* argv[]) {
    Falk_instance* F = new(Falk_instance);
    falk_instance_init(F);
    falk_getinput(F);
    return 0;
}
