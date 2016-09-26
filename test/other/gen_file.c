/* gen_file.c - test for generating files */
/* author: Azinum */
/* date: 26/09/16 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int file_write_file(const char* fname, char* write) {
    FILE* f = fopen(fname, "a");
    if (!f) {
        printf("Error opening file '%s'.", fname);
        return 0;
    }
    fwrite(write, strlen(write), 1, f);
    fclose(f);
    return 1;
}

double to_number(const char* string) {
    char* end;
    double number = strtod(string, &end);

    if (*end != '\0')
        return -1;

    return number;
}

int main() {
    const char* file = "number_gen.txt";
    FILE* f = fopen(file, "w");     /* erase file contents / create new empty file */
    fclose(f);
    char* buffer = (char*)malloc(10 * sizeof(char));
    file_write_file(file, "# Generated file: string to number test.\n");
    for (int i = 0; i <= 120; i++) {
        sprintf(buffer, "%i", i);
        buffer[strlen(buffer)] = '\n';
        file_write_file(file, buffer);
    }
    free(buffer);
    return 1;
}
