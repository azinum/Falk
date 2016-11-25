/* preprocessor.c */
/* Author: Azinum */
/* Date: 2016-11-25 */

#include "preprocessor.h"
#include "falk.h"

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
    return macros;
}

void preprocessor_print_macros(Preprocessor* preprocessor) {
    for (int i = 0; i < preprocessor->macro_count; i++) {
        printf("%s\n", preprocessor->macros->name);
    }
}
