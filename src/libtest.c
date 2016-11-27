/* libtest.c */
/* Author: Azinum */
/* Date: 2016-11-27 */


#include "libtest.h"

Object libtest_test(VM_instance* VM) {
    Object obj;
    return obj;
}

Object Init(VM_instance* VM) {
    puts("Initialized libtest");
    return falk_create_null(VM);
}
