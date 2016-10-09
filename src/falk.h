/* falk.h */
/* author: Azinum */
/* date: 30/08/16 */


#ifndef falk_h
#define falk_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vm.h"
#include "parse.h"

#define FALK_PROMPT "=> "
#define MAXINPUT 512

typedef struct Falk_instance {
    Parse_instance* parse_instance;
    VM_instance* vm_instance;
    int argc;
    const char** argv;
} Falk_instance;

void falk_instance_init(Falk_instance* F);

void falk_execute(Falk_instance* F);

void falk_instance_free(Falk_instance* F);

#endif /* falk_h */
