/* io.c */
/* author: Azinum */
/* date: 9/10/16 */


#include "io.h"

char* read_file(const char* fname) {
    char* buffer = NULL;
    long ssize, rsize;   /* string, read size*/
    FILE* f = fopen(fname, "r");
    
    if (f == NULL) {
        printf("Could not open file \"%s\".\n", fname);
        return NULL;
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
