/* libstandard.c */
/* Author: Azinum */
/* Date: 2016-10-26 */

#include "libstandard.h"
#include "falk.h"

#include <time.h>

Object falk_print(VM_instance* VM) {
    if (VM->stack->top > 0) {
        print_object(list_get_top(VM->stack));
        return falk_create_null(VM);
    }
    VM_throw_error(VM, VM_ERR_STACK, VM_ERRC_STACK_NOT_ENOUGH_ITEMS, "@falk_print lib function");
    return falk_create_null(VM);
}


Object falk_time(VM_instance* VM) {
    return falk_create_number(VM, clock());
}
