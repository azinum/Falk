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

int falk_instance_init(Falk_instance* F) {
    if (!F) {
        return 0;
    }
    
    F->parse_instance = new(Parse_instance);
    if (!parse_instance_init(F->parse_instance)) {
        printf("%s\n", "Parse instance could not be created.");
        return 0;
    }
    
    
    F->parse_instance->lex_instance = new(Lex_instance);
    if (!lex_instance_init(F->parse_instance->lex_instance)) {
        printf("%s\n", "Lex instance could not be created.");
        return 0;
    }
    
    F->vm_instance = new(VM_instance);
    if (!VM_init(F->vm_instance)) {
        printf("%s\n", "VM could not be created.");
        return 0;
    }
    
    F->init = 1;
    srand((unsigned int)time(NULL));
    
    falk_openlib(F->vm_instance, falklib_standard);
    
    return 1;
}


void falk_execute(Falk_instance* F) {
    if (F->argc >= 2) {
        for (int i = 1; i < F->argc; i++) {
            switch (F->argv[i][0]) {
                case '-': {
                    switch (F->argv[i][1]) {
                        case 'c': {     /* execute compiled file */
                            VM_execute(F->vm_instance, VM_EXEC_FILE, (char*)F->argv[i + 1]);
                            i++;
                        }
                            break;
                            
                        case 'h': {     /* help */
                            falk_print_help(F);
                            goto done;
                        }
                            break;
                            
                        case 's': {     /* execute string */
                            VM_execute(F->vm_instance, VM_EXEC_INTERPRET, (char*)F->argv[i + 1]);
                            i++;
                        }
                            break;
                            
                        case 'i': {     /* interpret */
                            falk_input(F, VM_EXEC_INTERPRET);
                        }
                            break;
                            
                        default: {    /* invalid option */
                            printf("Invalid option \"-%c\". %s\n",
                                F->argv[i][1],
                                "-h for a list of options.");
                            i++;
                        }
                            break;
                    }
                }
                    break;
                    
                default: {
                    /* script */
                    VM_execute(F->vm_instance, VM_EXEC_FILE, (char*)F->argv[1]);
                    goto done;
                }
                    break;
            }
        }
        goto done;
    }
    
    VM_execute(F->vm_instance, VM_EXEC_FILE, "test/compile/autoexec.fac");
    
    falk_input(F, VM_EXEC_INTERPRET);
done:
    falk_instance_free(F);
}


/*
** prints command-line args and usage
*/
void falk_print_help(Falk_instance* F) {
    printf("\n%s\n",
        "-c [script]   Execute compiled file.\n"
        "-s            Execute string.\n"
        "-i            Run in interpreted mode.\n"
        "-h            Print usage.\n"
        "[script]      Execute script.\n"
        "0 args        Run interpreter.\n"
    );
}

/*
** get input and execute in what exec mode
*/
void falk_input(Falk_instance* F, int mode) {
    char* input = NULL;
    unsigned long size = 0;
    
    while (1) {
        printf(FALK_PROMPT);
        if (getline(&input, &size, stdin) > 0) {
            if (!(VM_execute(F->vm_instance, mode, input)))
                break;
        }
    }
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

/*
** falk_create returns a Falk object
*/
Object falk_create_number(VM_instance* VM, double number) {
    Object obj;
    obj.type = T_NUMBER;
    obj.value.number = number;
    return obj;
}

Object falk_create_cstring(VM_instance* VM, char* string) {
    Object obj;
    obj.type = T_CSTRING;
    obj.value.string = string;
    return obj;
}

Object falk_create_null(VM_instance* VM) {
    return VM->obj_null;
}

Token falk_create_token(char* string, unsigned int type) {
    Token token;
    token.value = string;
    token.type = type;
    return token;
}


void falk_instance_free(Falk_instance* F) {
    parse_instance_free(F->parse_instance);
    lex_instance_free(F->parse_instance->lex_instance);
    VM_instance_free(F->vm_instance);
    free(F);
}

