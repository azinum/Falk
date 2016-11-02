/* falk.c */
/* Author: Azinum */
/* Date: 2016-08-30 */


#include "falk.h"
#include "table.h"
#include "object.h"
#include "list.h"
#include "llist.h"
#include "io.h"
#include "libstandard.h"

void falk_instance_init(Falk_instance* F) {
    F->parse_instance = new(Parse_instance);
    parse_instance_init(F->parse_instance);
    F->parse_instance->lex_instance = new(Lex_instance);
    lex_instance_init(F->parse_instance->lex_instance);
    F->vm_instance = new(VM_instance);
    VM_init(F->vm_instance);
    F->init = 1;
    
    falk_openlib(F->vm_instance, falklib_standard);
}


void falk_execute(Falk_instance* F) {
    if (F->argc >= 2) {
        for (int i = 1; i < F->argc; i++) {
            switch (F->argv[i][0]) {
                case '-': {
                    switch (F->argv[i][1]) {
                        case 'c': {     /* execute compiled file */
                            VM_execute(F->vm_instance, VM_EXEC_FILE, (char*)F->argv[i + 1]);
                            goto done;
                        }
                            break;
                            
                        case 'h': {     /* help */
                            
                            goto done;
                        }
                            break;
                            
                        default: {    /* invalid option */
                            printf("%s \"%c\"\n", "Invalid option", F->argv[i][1]);
                            goto done;
                        }
                            break;
                    }
                }
                    break;
                    
                default: {
                    /* script */
                    char* read = file_read(F->argv[1]);
                    VM_execute(F->vm_instance, VM_EXEC_FILE, read);
                    free(read);
                    goto done;
                }
                    break;
            }
        }
    }
    
    VM_execute(F->vm_instance, VM_EXEC_FILE, "test/compile/autoexec.fac");
    
    char* input = NULL;
    unsigned long size = 0;
    
    while (1) {
        printf(FALK_PROMPT);
        if (getline(&input, &size, stdin) > 0) {
            if (!(VM_execute(F->vm_instance, VM_EXEC_INTERPRET, input)))
                break;
        }
    }
    
done:
    falk_instance_free(F);
}

/*
** push number to stack
*/
int falk_push_number(VM_instance* VM, double number) {
    Object obj;
    obj.type = T_NUMBER;
    obj.value.number = number;
    list_push(VM->stack, obj);
    
    return 1;   /* success */
}

int falk_openlib(VM_instance* VM, CLibfunction lib[]) {
    int i = 0;
    while (lib[i].func != NULL) {
        falk_push_cfunction(VM, lib[i].name, lib[i].func);
        i++;
    }
    return 1;
}

void falk_pop(VM_instance* VM) {
    list_spop(VM->stack);
}

/*
** pop stack x times
*/
void falk_popx(VM_instance* VM, int x) {
    while (x-- > 0) {
        list_spop(VM->stack);
    }
}

void falk_push_cfunction(VM_instance* VM, char* name, Cfunction function) {
    table_push_object(VM->global->variables, name, ptr = function, T_CFUNCTION);
}

void falk_instance_free(Falk_instance* F) {
    parse_instance_free(F->parse_instance);
    lex_instance_free(F->parse_instance->lex_instance);
    VM_instance_free(F->vm_instance);
    free(F);
}

