/* libstd.h */
/* Author: Azinum */
/* Date: 2016-10-26 */

#ifndef libstandard_h
#define libstandard_h

#include <stdio.h>
#include "vm.h"

Object falk_libstd_print(VM_instance* VM);

Object falk_libstd_time(VM_instance* VM);

int cfib(int n);

Object falk_libstd_fib(VM_instance* VM);

void falk_libstd_standard_help(VM_instance* VM, const char* function);

static CLibfunction falk_libstd[] = {
    {"print", falk_libstd_print, "Print stuff to console."},
    {"time", falk_libstd_time, "Get time."},
    {"fib", falk_libstd_fib, "Calculate the fibonacci number."},
    {NULL, NULL, NULL}
};

#endif /* libstandard_h */
