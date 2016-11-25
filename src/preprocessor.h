/* preprocessor.h */
/* Author: Azinum */
/* Date: 2016-11-25 */

#ifndef preprocessor_h
#define preprocessor_h

#include <stdio.h>
#include "table.h"

/*
 Example:
 
 define test(a, b)
    push a
    push b
    add
 end_define
*/


/*
Preprocessor steps:
  1. Find all macros from input string and save them somewhere
  2. Create a new string
  3. Find and replace all macro-functions to the new string
  4: Done
*/

enum Prep_types {
    PREP_MACRO_BEGIN,
    PREP_MACRO_END,
    PREP_MACRO_ARGS_BEGIN,
    PREP_MACRO_ARGS_END,
    PREP_MACRO_ARG_SEPERATOR,
};

static const char* Prep_tokens[] = {
    "define",
    "end_define",
    "(",
    ")",
    ","
};

typedef struct Prep_macro {
    const char* name;
    const char** args;
    unsigned int argc;
} Prep_macro;


typedef struct Preprocessor {
    Prep_macro* macros;
    int macro_count;
} Preprocessor;


void preprocessor_init(Preprocessor* preprocessor);

char* preprocess_string(Preprocessor* preprocessor, char* input);

Prep_macro* preprocessor_find_macros(Preprocessor* preprocessor, char* input);

void preprocessor_print_macros(Preprocessor* preprocessor);

#endif /* preprocessor_h */
