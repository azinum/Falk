#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>

typedef int (*Function)(int, ...);

int main() {
    void* handle;
    Function func;
    handle = dlopen("test.so", RTLD_LAZY);
    if (handle) {
        func = dlsym(handle, "add");
        printf("%i\n", func(4, 7));
    }
    return 1;
}
