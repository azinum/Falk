/* libstandard.h */
/* Author: Azinum */
/* Date: 2016-10-26 */

#ifndef libstandard_h
#define libstandard_h

#include <stdio.h>
#include "vm.h"

Object falk_print(VM_instance* VM);

Object falk_time(VM_instance* VM);

Object falk_string_compile(VM_instance* VM);

void falklib_standard_help(VM_instance* VM, const char* function);

static CLibfunction falklib_standard[] = {
    {"print", falk_print, "Print stuff to console."},
    {"time", falk_time, "Get time."},
    {"strcompile", falk_string_compile, "Evaluate blocks in string."},
    {NULL, NULL, NULL}
};

#endif /* libstandard_h */
