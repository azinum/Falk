gcc -c -Wall -Werror -fpic test.c
gcc -shared -o libtest.so test.o
