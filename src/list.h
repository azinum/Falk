/* list.h */
/* author: Azinum */
/* date: 30/08/16 */

#ifndef list_h
#define list_h

#include "object.h"

/*
** get array size / length
** e.g
** int test[] = {
**     23, 43, 239, 2
** };
** printf("array 'test' contain %i elements.\n", (int)arr_size(test));
*/
#define arr_size(ARR) (sizeof(ARR) / sizeof(ARR[0]))

/*
** simple realloc with specific size
*/
#define mem_realloc(TARGET, SIZE) \
TARGET = (typeof(*TARGET)*)realloc(TARGET, SIZE);

/*
** define a new list with any type you want
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
** smart reallocation macro for increase and decrease size of list
*/
#define list_realloc(TARGET, SIZE) \
TARGET->value = (typeof(*TARGET->value)*)realloc(TARGET->value, (TARGET->size * sizeof(typeof(*TARGET->value))) + ((SIZE + 1) * sizeof(typeof(*TARGET->value)))); \
TARGET->size += SIZE


#define list_clear(LIST) \
while (LIST->top > 0) { \
    list_pop(LIST); \
}

/*
** for increasing list size if we need to
*/
#define list_need_space(LIST) (LIST->top >= LIST->size - 1)

/*
** safe to push item to list?
*/
#define list_is_safe(LIST) (LIST->size > LIST->top)

/*
** quick assign value to top of list
*/
#define list_set_top(LIST, VALUE) LIST->value[LIST->top] = VALUE

/*
** assign value to an index of list
*/
#define list_set(LIST, INDEX, VALUE) \
if (LIST->top > INDEX) LIST->value[INDEX] = VALUE

#define list_set_from_top(LIST, DELTA, VALUE) \
list_set(LIST, LIST->top + DELTA, VALUE)

#define list_get_top(LIST) (LIST->value[LIST->top - 1])

#define list_get(LIST, INDEX) (LIST->value[INDEX])

#define list_get_from_top(LIST, DELTA) (LIST->value[(LIST->top - 1) + DELTA])

/*
** push item to list
*/
#define list_push(LIST, VALUE) \
if (list_need_space(LIST)) { \
    list_realloc(LIST, 1); \
} \
LIST->value[LIST->top++] = VALUE

/*
** free not only items in list, but the list itself too
*/
#define list_free(LIST) \
free(LIST->value); \
free(LIST);

/*
** remove last item in list
*/
#define list_pop(LIST) \
LIST->value[--LIST->top] = 0; \
list_realloc(LIST, -1)


#define list_pop2(LIST) \
LIST->top--; \
list_realloc(LIST, -1)

/*
** spop => stack pop
** list_spop is an optimized / fast version of list_pop
** what it does is, when popping an element, not giving any memory to OS but decrementing top index of stack
*/
#define list_spop(LIST) \
--LIST->top; \

/*
** instead of pushing one character at a time, we can now push a full string onto our target string
*/
#define string_push(STRING, VALUE) \
for (int i = 0; i < arr_size(VALUE) - 1; i++) { \
    list_push(STRING, VALUE[i]); \
}

#define string_pop(STRING) \
STRING->value[--STRING->top] = '\0'; \
list_realloc(STRING, -1)

#define string_print(STRING) \
puts(STRING->value)

/*
** remove every character in string and free memory @list_realloc
*/
#define string_clear(STRING) \
while (STRING->size > 1) { \
    string_pop(STRING); \
}

/*
** "normal" strings (char*)
** need to allocate memory for new string
*/
#define string_copy_from_null(TARGET, STRING) \
mem_realloc(TARGET, strlen(STRING) + 1); \
strcpy(TARGET, STRING)

/*
** string is a type of list
** list of chars
*/
list_define(String, char);

list_define(Tokenlist, Token);

#endif /* list_h */
