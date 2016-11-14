/* libstd.c */
/* Author: Azinum */
/* Date: 2016-10-26 */

#include "libstd.h"
#include "falk.h"

#include <time.h>

Object falk_libstd_print(VM_instance* VM) {
    if (VM->stack->top > 0) {
        print_object(list_get_top(VM->stack));
        return falk_create_null(VM);
    }
    VM_throw_error(VM, VM_ERR_STACK, VM_ERRC_STACK_NOT_ENOUGH_ITEMS, "@falk_print lib function");
    return falk_create_null(VM);
}


Object falk_libstd_time(VM_instance* VM) {
    double time = clock();
    printf("%.7g\n", time);
    return falk_create_number(VM, clock());
}

Object falk_libstd_string_compile(VM_instance* VM) {
    String result;
    list_init((&result));
    
    Object top;
    if (VM->stack->top > 0) {
        top = list_get_top(VM->stack);
        if (object_is_cstring(top)) {
            unsigned long size = strlen(top.value.string);
            for (unsigned long i = 0; i < size; i++) {
                char current = top.value.string[i];
                switch (current) {
                    case '!': {
                        String temp;
                        list_init((&temp));
                        if (top.value.string[++i] == '{') {
                            while (i < size) {
                                if (top.value.string[++i] == '}')
                                    break;
                                list_push((&temp), top.value.string[i]);
                            }
                            string_push((&temp), "");   /* null terminate string */
                            char* varname = object2string(obj_convert(variable_find(VM, temp.value)));
                            if (*varname != '\0')
                                string_push2((&result), varname, strlen(varname));
                        }
                    }
                        break;
                        
                    default: {
                        list_push((&result), current);
                    }
                        break;
                }
            }
        }
    }
    
    string_push((&result), "");
    return falk_create_cstring(VM, result.value);
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
