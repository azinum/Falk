/* libtest.h */
/* Author: Azinum */
/* Date: 2016-11-27 */

#ifndef libtest_h
#define libtest_h

#include <stdio.h>
#include "falk.h"

Object libtest_test(VM_instance* VM);

Object libtest_test0(VM_instance* VM);

Object Init(VM_instance* VM);

static struct CLibfunction libtest[] = {
    {"test", libtest_test, "Test function"},
    {"test0", libtest_test0, "Test function 0"},
    {NULL, NULL, NULL}
};


#endif /* libtest_h */
