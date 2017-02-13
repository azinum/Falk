/* libbase.c */
/* Author: Azinum */
/* Date: 2016-11-29 */

#include "include/falk.h"
#include <stdio.h>


static Object lprint(VM_instance* VM) {
    print_object(list_get_top(&VM->stack));
    falk_pop(VM);
    return falk_create_null(VM);
}

int fib(int n) {
    if (n < 2)
        return n;
    return fib(n - 1) + fib(n - 2);
}

static Object lfib(VM_instance* VM) {
    int ret, n;
    if (!falk_build_args(VM, "i", &n)) {
        return falk_create_null(VM);
    }
    ret = fib(n);
    return falk_create_object(VM, 'i', &ret);
}

static CLibfunction baselib[] = {
    {"print", {lprint, 1}, ""},
    {"fib", {lfib, 1}, ""},
    {NULL, {NULL, 0}, NULL}
};

extern Object Init(VM_instance* VM) {
    falk_openlib(VM, baselib);
    return falk_create_null(VM);
}
