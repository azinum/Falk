/* list.h */
/* author: Azinum */
/* date: 30/08/16 */

#ifndef list_h
#define list_h

/*
** define a new list
*/
#define list_define(NAME, TYPE) \
typedef struct NAME { \
    TYPE* value; \
    unsigned int size; \
    unsigned int top; \
} NAME

#endif /* list_h */
