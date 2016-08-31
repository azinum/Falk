/* lex.c */
/* author: Azinum */
/* date: 30/08/16 */


#include "lex.h"

/*
** initialize any Lex_instance
*/
void lex_instance_init(Lex_instance* L) {
    L->error = LEX_NO_ERROR;
    L->warning = 0;
    L->line = 0;
    L->result = new(Lexed);
    list_init(L->result);
}

/*
** main function for lexer
** here we convert our input string to a list of tokens
** every token has a string type and a integer byte (unsigned char)
** the string is for knowing how the token looks like, the number is describing the token
*/
void lex(Lex_instance* L, String* input) {
    
}