/* libtest.c */
/* Author: Azinum */
/* Date: 2016-11-28 */

#include <time.h>

#include "include/falk.h"

int fib(int n) {
    if (n < 2) {
        return n;
    }
    return fib(n - 1) + fib(n - 2);
}

static Object falk_fib(VM_instance* VM) {
    int ret, n;
    if (!falk_build_args(VM, "i", &n)) {
        return falk_create_null(VM);
    }
    ret = fib(n);
    return falk_create_object(VM, 'i', &ret);
}

int what(int a, int b, int c, int d) {
    return (a + b) << (c + d);
}

static Object lib_what(VM_instance* VM) {
    int ret, a, b, c, d;
    if (!falk_build_args(VM, "iiii", &a, &b, &c, &d)) {
        return falk_create_null(VM);
    }
    ret = what(a, b, c, d);
    return falk_create_object(VM, 'i', &ret);
}

static Object print(VM_instance* VM) {
    if (VM->stack->top > 0) {
        print_object(list_get_top(VM->stack));
        vm_stack_pop();
        return falk_create_null(VM);
    }
    VM_throw_error(VM, VM_ERR_STACK, VM_ERRC_STACK_NOT_ENOUGH_ITEMS, "falk_print lib function");
    return falk_create_null(VM);
}

static struct CLibfunction mylib[] = {
    {"fib", {falk_fib, 1}, "Fib."},
    {"print", {print, 1}, "Print to console."},
    {"what", {lib_what, 4}, "What?"},
    {NULL, {NULL, 0}, NULL}
};

Object Init(VM_instance* VM) {
    falk_openlib(VM, mylib);
    return falk_create_null(VM);
}
