# Falk C Modules


``` C

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


static struct CLibfunction mylib[] = {
    {"fib", {falk_fib, 1}, "Fib."},
    {NULL, NULL, NULL}
};

Object Init(VM_instance* VM) {
    falk_openlib(VM, mylib);
    return falk_create_null(VM);
}

```

Every module needs to have a function that initializes all functions in library. That function is called ``` Init ``` and requires Falk VM as first and only parameter. ``` mylib ``` structure is where to store all functions in this library. The first parameter in structure is function name. Next is pointer to that function and how many arguments required. The last argument is a doc string for the user.


## How to compile module?



#### Mac OSX

``` gcc -bundle -undefined dynamic_lookup -fPIC -o my_module.so -I/path/to/include my_module.c ```


#### Linux

```
gcc -o my_module.o my_module.c -c -fPIC
gcc -shared -fPIC -o my_module.so my_module.o -I/path/to/include
```
