/* preprocessor.c */
/* Author: Azinum */
/* Date: 2016-11-25 */

#include <string.h>

#include "include/preprocessor.h"
#include "include/falk.h"
#include "include/string.h"

void preprocessor_init(Preprocessor* preprocessor) {
    
}

char* preprocess_string(Preprocessor* preprocessor, char* input) {
    String output;
    list_init(&output);
    
    return output.value;
}

Prep_macro* preprocessor_find_macros(Preprocessor* preprocessor, char* input) {
    Prep_macro* macros = new(Prep_macro);
    int size = 1;
    
    for (int i = 0; i < strlen(input); i++) {
        if (input[i] == *prep_tokens[PREP_MACRO_TOKEN]) {
            // printf("Macro at: %i\n", i);
        }
    }
    
    return macros;
}

void preprocessor_print_macros(Preprocessor* preprocessor) {
    for (int i = 0; i < preprocessor->macro_count; i++) {
        printf("%s\n", preprocessor->macros->name);
    }
}
