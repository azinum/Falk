#include "falk.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char* argv[]) {
    llist_define(Test, int);
    Test* test = new(Test);
    llist_init(test);
    llist_push(test, 0);
    llist_push(test, 1);
    llist_push(test, 2);
    llist_push(test, 3);
    llist_push(test, 4);
    llist_push(test, 5);
    llist_erase(test, 0);
    llist_erase(test, 0);
    llist_erase(test, 0);

    Test* it = test;
    while ((it = it->next) != NULL) {
        printf("%i\n", it->value);
    }

    Test* current = test;
    int i = 0;
    while ((current->next != NULL)) {
        if (i == 2) {
            printf("prev: %i. this: %i next: %i\n", current->prev->value, current->value, current->next->value);
            break;
        }
        current = current->next;
        i++;
    }
    return 0;
}
