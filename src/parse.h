/* parse.h */
/* author: Azinum */
/* date: 05/09/16 */


#ifndef parse_h
#define parse_h

#include <stdio.h>
#include "lex.h"

enum Parse_errors {
    PARSE_NO_ERROR
};

typedef struct Parse_instance {
    int error, warning;
} Parse_instance;


void parse_instance_init(Parse_instance* P);
void parse_instance_free(Parse_instance* P);

#endif /* parse_h */
