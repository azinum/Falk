/* falk.h */
/* Author: Azinum */
/* Date: 2016-08-30 */


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
    unsigned char init;
} Falk_instance;

void falk_instance_init(Falk_instance* F);

void falk_execute(Falk_instance* F);

void falk_instance_free(Falk_instance* F);

int falk_push_number(VM_instance* VM, double number);

int falk_openlib(VM_instance* VM, CLibfunction lib[]);

void falk_pop(VM_instance* VM);

void falk_popx(VM_instance* VM, int x);

void falk_push_cfunction(VM_instance* VM, char* name, Cfunction function);

#endif /* falk_h */
