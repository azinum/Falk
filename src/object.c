/* object.c */
/* author: Azinum */
/* date: 30/08/16 */

#include "object.h"

unsigned char is_number(const char* string) {
    if (strlen(string) <= 0)
        return 0;
    char* end;
    strtod(string, &end);

    if (*end != '\0')
        return 0;

    return 1;
}

double to_number(const char* string) {
    char* end;
    double number = strtod(string, &end);
    
    if (*end != '\0')
        return -1;
    
    return number;
}

void print_object(Object object) {
    switch (object.type) {
        case T_NUMBER: {
            printf("%.6g\n", object.value.number);
        }
            break;
            
        case T_CSTRING: {
            printf("%s\n", object.value.string);
        }
            break;
            
        case T_SCOPE: {
            printf("Scope @: %i\n", (int)object.value.ptr);
        }
            break;
            
        default: {
            printf("Unknown data @: %i\n", (int)object.value.ptr);
        }
            break;
    }
}