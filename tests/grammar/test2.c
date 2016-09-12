#include <stdlib.h>
#include <stdio.h>

#define new(TYPE) ((TYPE*)malloc(sizeof(TYPE)))
#define newx(TYPE, SIZE) ((TYPE*)malloc(SIZE * sizeof(TYPE)))

#define mem_realloc(TARGET, SIZE) (TARGET = (typeof(*TARGET)*)realloc(TARGET, SIZE))

#define arr_size(ARR) (sizeof(ARR) / sizeof(ARR[0]))

enum Flags {
    NONE        = 2 << 0,
    IF          = 2 << 1,
    WHILE       = 2 << 2,
    FUNC        = 2 << 3,
    EXPRESSION  = 2 << 4,
    BODY        = 2 << 5,
    END         = 2 << 6,
};


typedef struct Rule {
    int value;      /* rule itself */
    int* flags;     /* array of flags */
    int flagc;      /* flag count */
} Rule;

Rule rules[] = {
    {NONE, NULL, -1},
    {IF, NULL, -1},
    {WHILE, NULL, -1},
    {FUNC, NULL, -1},
};

int main() {
    int prog[] = {
        IF, EXPRESSION, BODY, END
    };

    Rule rule;
    rule.value = IF;
    rule.flagc = 2;
    rule.flags = newx(int, 3);
    rule.flags[0] = EXPRESSION;
    rule.flags[1] = BODY | END;

    for (int i = 0; i < arr_size(prog); i++) {
        switch (prog[i]) {
            case IF: {
                int value;
                for (int j = 0; j < rule.flagc; j++) {
                    value = prog[++i] & rule.flags[j];
                    if (!value || i >= arr_size(prog)) {
                        printf("Grammatical error\n");
                    }
                }
            }
                break;

            case WHILE: {
                /* check grammar for while */
            }
                break;

            /* case ... */

            default:
                break;
        }
    }

    return 0;
}
