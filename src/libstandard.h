/* libstandard.h */
/* Author: Azinum */
/* Date: 2016-10-26 */

#ifndef libstandard_h
#define libstandard_h

#include <stdio.h>
#include "vm.h"

int falk_print(VM_instance* VM);

int falk_time(VM_instance* VM);

static CLibfunction falklib_standard[] = {
    {"print", falk_print, "Print stuff to console"},
    {"time", falk_time, "Get time"},
    {NULL, NULL, NULL}
};

#endif /* libstandard_h */
