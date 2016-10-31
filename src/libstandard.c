/* libstandard.c */
/* Author: Azinum */
/* Date: 2016-10-26 */

#include "libstandard.h"
#include "falk.h"

#include <time.h>

int falk_print(VM_instance* VM) {
    falk_pop(VM);   /* pop argc */
    if (VM->stack->top > 0) {
        print_object(list_get_top(VM->stack));
        return 1;
    }
    VM_throw_error(VM, VM_ERR_STACK, VM_ERRC_STACK_NOT_ENOUGH_ITEMS, "@falk_print lib function");
    return 0;
}


int falk_time(VM_instance* VM) {
    falk_pop(VM);
    falk_push_number(VM, clock());
    return 1;
}
