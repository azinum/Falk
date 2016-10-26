/* falk.c */
/* Author: Azinum */
/* Date: 2016-08-30 */


#include "falk.h"
#include "table.h"
#include "object.h"
#include "list.h"
#include "llist.h"
#include "io.h"

void falk_instance_init(Falk_instance* F) {
    F->parse_instance = new(Parse_instance);
    parse_instance_init(F->parse_instance);
    F->parse_instance->lex_instance = new(Lex_instance);
    lex_instance_init(F->parse_instance->lex_instance);
    F->vm_instance = new(VM_instance);
    VM_init(F->vm_instance);
    F->init = 1;
}


void falk_execute(Falk_instance* F) {
    if (F->argc == 2) {     /* open file */
        char* input = read_file(F->argv[1]);
        if (input != NULL)
            parse(F->parse_instance, input);
    } else {
        char* input = NULL;
        unsigned long size = 0;
        
        while (1) {
            printf(FALK_PROMPT);
            if (getline(&input, &size, stdin) > 0) {
                input[strlen(input)-1] = '\0';      /* remove newline */
                if (!(VM_execute(F->vm_instance, VM_EXEC_FILE, input)))
                    break;
            }
            break;
        }
    }
    
    falk_instance_free(F);
}

/*
** push number to stack
*/
int falk_push_number(Falk_instance* F, double number) {
    /* if instance of Falk is not initialized, return false */
    if (!F->init) {
        return 0;
    }
    Object obj;
    obj.type = T_NUMBER;
    obj.value.number = number;
    list_push(F->vm_instance->stack, obj);
    
    return 1;   /* success */
}

void falk_instance_free(Falk_instance* F) {
    parse_instance_free(F->parse_instance);
    lex_instance_free(F->parse_instance->lex_instance);
    VM_instance_free(F->vm_instance);
    free(F);
}

