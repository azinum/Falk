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