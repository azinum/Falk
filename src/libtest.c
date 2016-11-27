/* libtest.c */
/* Author: Azinum */
/* Date: 2016-11-27 */


#include "libtest.h"

Object libtest_test(VM_instance* VM) {
    puts("Hello from test");
    return falk_create_null(VM);
}

Object libtest_test0(VM_instance* VM) {
    puts("Hello from test0");
    return falk_create_null(VM);
}

Object Init(VM_instance* VM) {
    falk_openlib(VM, libtest);
    return falk_create_null(VM);
}
