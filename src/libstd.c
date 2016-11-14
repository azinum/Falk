/* libstd.c */
/* Author: Azinum */
/* Date: 2016-10-26 */

#include "libstd.h"
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

Object falk_string_compile(VM_instance* VM) {
    return falk_create_cstring(VM, "");
}

void falklib_standard_help(VM_instance* VM, const char* function) {
    
    for (int i = 0; i < arr_size(falklib_standard); i++) {
        if (falklib_standard[i].name == function) {
            printf("%s\n%s\n", function, falklib_standard[i].doc_string);
            return;
        }
    }
    printf("%s\n", "Function was not found in this library.");
}
