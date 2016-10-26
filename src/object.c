/* object.c */
/* author: Azinum */
/* date: 30/08/16 */

#include <limits.h>

#include "object.h"
#include "table.h"

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
    if (strlen(string) <= 0)
        return 0;
    
    char* end;
    double number = strtod(string, &end);
    
    if (*end != '\0')
        return INT8_MIN;
    
    return number;
}

void print_object(Object object) {
begin:
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
            printf("Scope @: $%i\n", (int)object.value.ptr);
        }
            break;
        
        case T_NULL: {
            puts("null");
        }
            break;
        
        case T_VAR: {
            if (object.value.ptr != NULL) {
                object = (*(TValue*)object.value.ptr).tval;
                goto begin;
            } else {
                puts("Undefined");
            }
        }
            break;
        
        default: {
            printf("Extern data @: $%i. Type: %i.\n", (int)object.value.ptr, object.type);
        }
            break;
    }
}


int object_is_true(Object obj) {
    switch (obj.type) {
        case T_NUMBER:
            if (obj.value.number != 0)
                return 1;
            break;
            
        case T_STRING:
            if (strncmp(obj.value.string, "", strlen(obj.value.string)))
                return 1;
            break;
        default:
            break;
    }
    return 0;
}
