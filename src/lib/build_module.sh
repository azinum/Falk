#
# How to compile modules
#

# Mac OSX:
# gcc -bundle -undefined dynamic_lookup -fPIC -o module.so -I/path/to/include module.c

# Linux:
# 1) gcc -o module.o module.c -c -fPIC
# 2) gcc -shared -fPIC -o module.so module.o -Iinclude/


# Windows:
# ...
