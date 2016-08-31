/* list.h */
/* author: Azinum */
/* date: 30/08/16 */

#ifndef list_h
#define list_h

#include "object.h"

#define arr_size(ARR) (sizeof(ARR) / sizeof(ARR[0]))

/*
** define a new list
*/
#define list_define(NAME, TYPE) \
typedef struct NAME { \
    TYPE* value; \
    unsigned int size; \
    unsigned int top; \
} NAME

/*
** initialize list
*/
#define list_init(LIST) \
LIST->value = new(typeof(*LIST->value)); \
LIST->size = 1; \
LIST->top = 0

/*
** for increasing list size if we need to
*/
#define list_need_space(LIST) (LIST->top >= LIST->size)

/*
** push item to list
*/
#define list_push(LIST, VALUE) \
if (list_need_space(LIST)) {\
    LIST->value = (typeof(VALUE)*)realloc(LIST->value, sizeof(VALUE) * (++LIST->size)); \
} \
LIST->value[LIST->top++] = VALUE

#define list_free(LIST) \
free(LIST->value); \
free(LIST);


#define string_push(STRING, VALUE) \
for (int i = 0; i < arr_size(VALUE) - 1; i++) {\
    list_push(STRING, VALUE[i]); \
}

/*
** string is a type of list
** list of chars
*/
list_define(String, char);

list_define(Lexed, Token);

#endif /* list_h */
