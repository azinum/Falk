/* main.c */
/* Author: Azinum */
/* Date: 2016-08-30 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "include/falk.h"
#include "include/preprocessor.h"


int main(int argc, char** argv) {
    Falk_instance* F = new(Falk_instance);
    if (!falk_instance_init(F)) {
        printf("%s\n", "Falk instance could not be created.");
        return 0;
    }
    F->argc = argc;
    F->argv = argv;
    
    falk_execute(F);
    falk_instance_free(F);
    return 1;
}
