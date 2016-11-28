/* falk.c */
/* Author: Azinum */
/* Date: 2016-08-30 */

#include <stdarg.h>
#include <dlfcn.h>
#include <string.h>
#include <stdio.h>

#include "include/falk.h"

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
    
    return 1;
}


void falk_execute(Falk_instance* F) {
    if (F->argc >= 2) {
        for (int i = 1; i < F->argc; i++) {
            if (F->argv[i][0] == '-') {
                switch (F->argv[i][1]) {
                    case 'c': {     /* execute compiled file */
                        VM_execute(F->vm_instance, VM_EXEC_FILE, F->argv[i + 1]);
                        i++;
                    }
                        break;
                        
                    case 'h': {     /* help */
                        falk_print_help(F);
                        goto done;
                    }
                        break;
                        
                    case 's': {     /* execute string */
                        VM_execute(F->vm_instance, VM_EXEC_INTERPRET, F->argv[i + 1]);
                        i++;
                    }
                        break;
                        
                    case 'i': {     /* interpret */
                        falk_input(F, VM_EXEC_INTERPRET);
                    }
                        break;
                        
                    case 'l': {
                        falk_loadlib(F->vm_instance, F->argv[i + 1]);
                        i++;
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
            } else {
                VM_execute(F->vm_instance, VM_EXEC_FILE, F->argv[1]);
                goto done;
            }
        }
        goto done;
    }
    
    // VM_execute(F->vm_instance, VM_EXEC_FILE, "test/compile/autoexec.fac");
    
    falk_input(F, VM_EXEC_INTERPRET);
done:
    falk_instance_free(F);
}


/*
** prints command-line args and usage
*/
void falk_print_help(Falk_instance* F) {
    printf("\n%s\n",
        "-c [script]     Execute compiled file.\n"
        "-s              Execute string.\n"
        "-i              Run in interpreted mode.\n"
        "-h              Print usage.\n"
        "-l [library]    load library.\n"
        "[script]        Execute script.\n"
        "None of above   Run interpreter.\n"
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

/*
** push any object to stack
*/
int falk_push_obj(VM_instance* VM, Object obj) {
    if (VM->stack->top >= VM->stack->size)      /* stack overflow */
        return 0;
    list_push(VM->stack, obj);
    return 1;
}

int falk_openlib(VM_instance* VM, CLibfunction lib[]) {
    int i = 0;
    while (lib[i].name != NULL) {
        falk_push_cfunction(VM, lib[i].name, lib[i].func);
        i++;
    }
    return 1;
}

int falk_loadlib(VM_instance* VM, const char* path) {
    void* handle;
    Func func;
    handle = dlopen(path, RTLD_NOW);
    if (!handle) {
        printf("Could not open library \"%s\"\n", path);
        return 0;
    }
    
    func = dlsym(handle, "Init");
    if (!func) {
        printf("Init function does not exist\n");
        return 0;
    }
    func(VM);
    printf("Loaded library \"%s\"\n", path);
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
    table_push_object(VM->global->variables, name, func = function, T_CFUNCTION);
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

Object falk_create_pointer(VM_instance* VM, void* p) {
    Object obj;
    obj.type = T_POINTER;
    obj.value.ptr = p;
    return obj;
}

/*
** formats available:
** i (int)
** d (double)
** s (char*)
** p (void*)
*/
Object falk_create_object(VM_instance* VM, char format, void* value) {
    Object obj = falk_create_null(VM);
    
    switch (format) {
        case 'i': {
            obj.type = T_NUMBER;
            obj.value.number = (double)(*(int*)value);
        }
            break;
            
        case 'd': {
            obj.type = T_NUMBER;
            obj.value.number = *(double*)value;
        }
            break;
            
        case 's': {
            obj.type = T_CSTRING;
            obj.value.string = (char*)value;
        }
            break;
            
        case 'p': {
            obj.type = T_POINTER;
            obj.value.ptr = value;
        }
            break;
        
        default:
            printf("Invalid format '%c'\n", format);
            break;
    }
    
    return obj;
}

Token falk_create_token(char* string, unsigned int type) {
    Token token;
    token.value = string;
    token.type = type;
    return token;
}


/*
** build args from Falk stack to C variables
*/
int falk_build_args(VM_instance* VM, const char* format, ...) {
    va_list ap;
    va_start(ap, format);
    int success = 1;
    
    for (int i = 0; i < strlen(format); i++) {
        switch (format[i]) {
            case 'i': {       /* integer */
                Object obj = list_get_top(VM->stack);
                vm_stack_pop();
                if (obj.type == T_NUMBER) {
                    int* arg = va_arg(ap, int*);
                    *arg = (int)obj.value.number;
                    break;
                }
                success = 0;
                goto done;
            }
                break;
                
            case 'd': {      /* double */
                Object obj = list_get_top(VM->stack);
                vm_stack_pop();
                if (obj.type == T_NUMBER) {
                    double* arg = va_arg(ap, double*);
                    *arg = obj.value.number;
                    break;
                }
                success = 0;
                goto done;
            }
                break;
                
            case 'p': {       /* pointer */
                Object obj = list_get_top(VM->stack);
                vm_stack_pop();
                
                void* arg = va_arg(ap, void*);
                if (obj.type == T_POINTER || obj.type == T_SCOPE) {
                    arg = (void*)obj.value.ptr;
                    break;
                }
                if (obj.type == T_VAR) {
                    arg = obj.value.obj;
                    break;
                }
                success = 0;
                goto done;
            }
                break;
                
            case 'c': {      /* char */
                Object obj = list_get_top(VM->stack);
                vm_stack_pop();
                if (obj.type == T_NUMBER) {
                    char* arg = va_arg(ap, char*);
                    *arg = (char)obj.value.number;
                    break;
                }
                success = 0;
                goto done;
            }
                break;
                
            case 's': {       /* cstring */
                Object obj = list_get_top(VM->stack);
                vm_stack_pop();
                if (obj.type == T_CSTRING) {
                    char* arg = va_arg(ap, char*);
                    arg = newx(char, strlen(obj.value.string) + 1);
                    strcpy(arg, obj.value.string);
                    puts(arg);
                    break;
                }
                success = 0;
                goto done;
            }
                break;
                
            default: {
                printf("Incorrect format \"%c\"\n", format[i]);
                success = 0;
            }
                break;
        }
    }
done:
    va_end(ap);
    return success;
}

void falk_instance_free(Falk_instance* F) {
//    VM_instance_free(F->vm_instance);
//    lex_instance_free(F->parse_instance->lex_instance);
//    parse_instance_free(F->parse_instance);
//    free(F);
}

