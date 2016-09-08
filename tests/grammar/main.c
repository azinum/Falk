/* main.c - grammar tests */
/* author: Azinum */
/* date: 07/09/16 */

#include <stdlib.h>
#include <stdio.h>

/*
** some grammar testing
*/

/*
** some sort of mask may be used
*/
enum Things {
    ADD = 2 << 0,
    SUB = 2 << 1,
    MUL = 2 << 2,
    DIV = 2 << 3,
    IDF = 2 << 4,
    NUM = 2 << 5,
};

typedef struct Rule {
    int from, to, next;
} Rule;

Rule rules[] = {
    {ADD, DIV, IDF},
    {ADD, DIV, NUM}
};

unsigned char next_is_legal(int current, int next) {
    Rule rule;
    for (int i = 0; i < arr_size(rules); i++) {     /* find rule */
        rule = rules[i];

        if (next == rule.next) {    /* found the next "token" */
            if (current >= rule.from && current <= rule.to) {   /* is next legal? */
                return 1;
            }
        }

    }
    return 0;
}

int main() {
    printf("%i", next_is_legal(DIV, IDF));
    return 0;
}
