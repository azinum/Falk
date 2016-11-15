/* libstd.c */
/* Author: Azinum */
/* Date: 2016-10-26 */

#include <time.h>

#include "libstd.h"
#include "falk.h"

Object falk_libstd_time(VM_instance* VM) {
    return falk_create_number(VM, clock());
}

int falk_libstd_cfib(int n) {
    if (n < 2) {
        return n;
    }
    return falk_libstd_cfib(n - 1) + falk_libstd_cfib(n - 2);
}

Object falk_libstd_fib(VM_instance* VM) {
    int ret, n;
    
    if (!falk_build_args(VM, "i", &n))
        return falk_create_null(VM);
    
    ret = falk_libstd_cfib(n);
    
    return falk_create_object(VM, 'i', &ret);
}


Object falk_libstd_print(VM_instance* VM) {
    if (VM->stack->top > 0) {
        print_object(list_get_top(VM->stack));
        vm_stack_pop();
        return falk_create_null(VM);
    }
    VM_throw_error(VM, VM_ERR_STACK, VM_ERRC_STACK_NOT_ENOUGH_ITEMS, "@falk_print lib function");
    return falk_create_null(VM);
}

void falk_libstd_standard_help(VM_instance* VM, const char* function) {
    for (int i = 0; i < arr_size(falk_libstd); i++) {
        if (falk_libstd[i].name == function) {
            printf("%s\n%s\n", function, falk_libstd[i].doc_string);
            return;
        }
    }
    printf("%s\n", "Function was not found in this library.");
}
