/* libbase.c */
/* Author: Azinum */
/* Date: 2016-11-29 */

#include "include/falk.h"

static Object print(VM_instance* VM) {
    print_object(list_get_top(&VM->stack));
    falk_pop(VM);
    return falk_create_null(VM);
}

static CLibfunction baselib[] = {
    {"print", {print, 1}, ""},
    {NULL, {NULL, 0}, NULL}
};

extern Object Init(VM_instance* VM) {
    falk_openlib(VM, baselib);
    return falk_create_null(VM);
}
