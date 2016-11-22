/* io.c */
/* Author: Azinum */
/* Date: 2016-10-09 */


#include "io.h"

char* file_read(const char* fname) {
    char* buffer = NULL;
    long ssize, rsize;   /* string, read size*/
    FILE* f = fopen(fname, "r");
    
    if (f == NULL) {
        printf("Could not open file \"%s\".\n", fname);
        fclose(f);
        return buffer;
    }
    
    fseek(f, 0, SEEK_END);
    
    ssize = ftell(f);
    
    rewind(f);
    
    buffer = newx(char, ssize + 1);
    
    rsize = fread(buffer, sizeof(char), ssize, f);
    
    buffer[ssize] = '\0';
    
    if (ssize != rsize) {
        free(buffer);
        buffer = NULL;
    }
    
    fclose(f);
    
    return buffer;
}


int file_exist(const char* fname) {
    FILE* f = fopen(fname, "r");
    if (f) {
        fclose(f);
        return 1;
    }
    return 0;
}

/*
** print what, x number of times
*/
void printx(const char* what, unsigned int count) {
    while (count--) {
        printf("%s", what);
    }
}
