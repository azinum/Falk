/* falk.h */
/* Author: Azinum */
/* Date: 2016-08-30 */


#ifndef falk_h
#define falk_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "conf.h"
#include "vm.h"
#include "parse.h"
#include "string.h"
#include "object.h"

extern_c

#define FALK_PROMPT "=> "
#define MAXINPUT 512

typedef struct Falk_instance {
    Parse_instance* parse_instance;
    VM_instance* vm_instance;
    int argc;
    char** argv;
    unsigned char init;
} Falk_instance;

int falk_instance_init(Falk_instance* F);

void falk_execute(Falk_instance* F);

void falk_print_help(Falk_instance* F);

void falk_input(Falk_instance* F, int mode);

void falk_instance_free(Falk_instance* F);

extern int falk_push_number(VM_instance* VM, double number);

extern int falk_push_obj(VM_instance* VM, Object obj);

extern int falk_openlib(VM_instance* VM, CLibfunction lib[]);

extern int falk_loadlib(VM_instance* VM, const char* path);

extern void falk_pop(VM_instance* VM);

extern void falk_popx(VM_instance* VM, int x);

extern void falk_push_cfunction(VM_instance* VM, char* name, Cfunction function);

extern Object falk_create_number(VM_instance* VM, double number);

extern Object falk_create_cstring(VM_instance* VM, char* string);

extern Object falk_create_null(VM_instance* VM);

extern Object falk_create_pointer(VM_instance* VM, void* p);

extern Object falk_create_object(VM_instance* VM, char format, void* value);

Token falk_create_token(char* string, unsigned int type);

extern int falk_build_args(VM_instance* VM, const char* format, ...);

extern_c_end

#endif /* falk_h */
