/* vm.c */
/* Author: Azinum */
/* Date: 2016-09-19 */

#include "vm.h"
#include "falk.h"
#include "io.h"
#include "libstandard.h"
#include <time.h>

void VM_init(VM_instance* VM) {
    VM->init = 0;
    VM->stack = new(Stack);
    list_init(VM->stack);
    VM->instructions = new(Instruction_list);
    list_init(VM->instructions);
    VM->dummy = new(Object);
    VM->dummy->type = T_NULL;
    VM->exit_on_error = 1;
    VM->ip = 0;
    
    VM->global = new(Scope);
    VM->global->global = VM->global;
    VM->global->variables = new(HashTable);
    table_init(VM->global->variables);
    
    table_push_object(VM->global->variables, "global", ptr = VM->global, T_SCOPE);
    table_push_object(VM->global->variables, "null", ptr = NULL, T_NULL);
    table_push_object(VM->global->variables, "undefined", ptr = NULL, T_NULL);
    table_push_object(VM->global->variables, "pi", number = 3.14159265359, T_NUMBER);
    table_push_object(VM->global->variables, "a", number = 7, T_NUMBER);
    table_push_object(VM->global->variables, "vm", ptr = &VM, -1);
}

int VM_execute(VM_instance* VM, int mode, char* input) {
    if (!VM->init) {
        list_push(VM->instructions, &&VM_EQ_ASSIGN);
        list_push(VM->instructions, &&VM_ADD);
        list_push(VM->instructions, &&VM_SUB);
        list_push(VM->instructions, &&VM_DIV);
        list_push(VM->instructions, &&VM_MUL);
        list_push(VM->instructions, &&VM_PUSHK);
        list_push(VM->instructions, &&VM_PUSHI);
        list_push(VM->instructions, &&VM_EXIT);
        list_push(VM->instructions, &&VM_GOTO);
        list_push(VM->instructions, &&VM_IF);
        list_push(VM->instructions, &&VM_POP);
        list_push(VM->instructions, &&VM_LT);
        list_push(VM->instructions, &&VM_GT);
        list_push(VM->instructions, &&VM_EQ);
        list_push(VM->instructions, &&VM_LEQ);
        list_push(VM->instructions, &&VM_GEQ);
        list_push(VM->instructions, &&VM_PUSHP);
        list_push(VM->instructions, &&VM_CALLF);
        list_push(VM->instructions, &&VM_ADD_ASSIGN);
        list_push(VM->instructions, &&VM_SUB_ASSIGN);
        list_push(VM->instructions, &&VM_MUL_ASSIGN);
        list_push(VM->instructions, &&VM_DIV_ASSIGN);
        list_push(VM->instructions, &&VM_LOAD);
        list_push(VM->instructions, &&VM_STORE);
        VM->init = 1;
    }

    switch (mode) {
        case VM_EXEC_INTERPRET: {
            Lex_instance* lex_instance = new(Lex_instance);
            lex_instance_init(lex_instance);
            
            if (!lex(lex_instance, input)) {
                return 0;
            }
            
            Tokenlist* lexresult = new(Tokenlist);
            list_init(lexresult);
            *lexresult = lex_instance->result;
            
            if (!lexresult) {
                return 0;
            }
            
            VM->program = to_ins(VM, lexresult);
        }
            break;
            
        case VM_EXEC_FILE: {
            char* read = read_file(input);
            if (read != NULL) {
                VM->program = VM_string2bytecode(VM, read);
                break;
            }
            return 0;
        }
            
        default:
            VM_throw_error(VM, 0, 0, "Invalid exec mode");
            break;
    }
    
    if (!VM->program)
        return 0;

    vm_begin;

    /*
    ** left hand side of assignment MUST be variable
    ** if not true, throw an error
    */
    vmcase(VM_EQ_ASSIGN, {
        if (VM->stack->top >= 2) {
            /* if we do not do obj_convert then, we assign by reference / pointer */
            /* it is a feature to be added later */
            obj2TValue(list_get_from_top(VM->stack, -1)).value = obj_convert(list_get_top(VM->stack));
            list_spop(VM->stack);
            vm_next;
        }
        VM_throw_error(VM, VM_ERR_STACK, VM_ERRC_STACK_NOT_ENOUGH_ITEMS, "@VM_EQ_ASSIGN");
    });
    
    /* load reg */
    vmcase(VM_LOAD, {
        Object reg = (*(Object*)vm_getip(VM->ip + 1));
        if (reg.type != T_NUMBER) {
            VM_throw_error(VM, VM_ERR_REGISTER, VM_ERRC_REGISTER_INVALID_TYPE, "@VM_LOAD");
        }
        list_push(VM->stack, VM->registers[(int)reg.value.number]);
        vm_jump(2);
    });
    
    /* store reg, value */
    vmcase(VM_STORE, {
        Object reg = (*(Object*)vm_getip(VM->ip + 1));
        Object value = (*(Object*)vm_getip(VM->ip + 2));
        if (reg.type != T_NUMBER) {
            VM_throw_error(VM, VM_ERR_REGISTER, VM_ERRC_REGISTER_INVALID_TYPE, "@VM_STORE");
        }
        
        switch (value.type) {
            case T_IDENTIFIER: {
                if (table_find(VM->global->variables, value.value.string) != NULL) {
                    tobject_create(obj, ptr = table_find(VM->global->variables, value.value.string), T_VAR);
                    VM->registers[(int)reg.value.number] = obj;
                } else {
                    Object null_object;
                    null_object.type = T_NULL;
                    VM->registers[(int)reg.value.number] = null_object;
                }
            }
                break;
                
            default:
                VM->registers[(int)reg.value.number] = value;
                break;
        }
    
        vm_jump(3);
    });
    
    vmcase(VM_ADD_ASSIGN, {
        num_assign(+=, "@VM_ADD_ASSIGN");
    });
    
    vmcase(VM_SUB_ASSIGN, {
        num_assign(-=, "@VM_SUB_ASSIGN");
    });
    
    vmcase(VM_MUL_ASSIGN, {
        num_assign(*=, "@VM_MUL_ASSIGN");
    });
    
    vmcase(VM_DIV_ASSIGN, {
        num_assign(/=, "@VM_DIV_ASSIGN");
    });
    
    vmcase(VM_PUSHK, {
        list_push(VM->stack, *((Object*)VM->program[VM->ip + 1]));
        /* if (list_get_top(VM->stack).type != T_NUMBER) {
            VM_throw_error(VM, VM_ERR_STACK, VM_ERRC_ARITH_INVALID_TYPES, "@VM_PUSHK");
        } */
        vm_skip(1);
    });
    
    vmcase(VM_PUSHP, {
        list_push(VM->stack, *((Object*)VM->program[VM->ip + 1]));
        vm_skip(1);
    });
    
    vmcase(VM_PUSHI, {
        Object* next = (Object*)(((VM->program[VM->ip + 1])));
        char* name = next->value.string;
        
        if (table_find(VM->global->variables, name) != NULL) {
            /*
            ** variable exist
            ** optimize: create opcode (VM_PUSHP, addr)
            */
            tobject_create(obj, ptr = table_find(VM->global->variables, name), T_VAR);
            list_push(VM->stack, obj);
            VM->program[VM->ip] = list_get(VM->instructions, VMI_PUSHP);
            VM->program[VM->ip + 1] = &obj;
            
        } else {
            /*
            ** variable does not exist
            ** create variable
            ** optimize code
            */
            table_push_object(VM->global->variables, name, ptr = NULL, T_NULL);
            tobject_create(obj, ptr = table_find(VM->global->variables, name), T_VAR);
            list_push(VM->stack, obj);
        }
        vm_skip(1);
    });
    
    /*
    ** if we can do arithmetic operation,
    ** jump to next instruction, else: throw error message
    */
    vmcase(VM_ADD, {
        num_arith(+, "@VM_ADD");
    });
    
    vmcase(VM_SUB, {
        num_arith(-, "@VM_SUB");
    });
    
    vmcase(VM_DIV, {
        num_arith(/, "@VM_DIV");
    });
    
    vmcase(VM_MUL, {
        num_arith(*, "@VM_MUL");
    });
    
    vmcase(VM_LT, {
        num_arith(<, "@VM_LT");
    });
    
    vmcase(VM_GT, {
        num_arith(>, "@VM_GT");
    });
    
    vmcase(VM_EQ, {
        num_arith(==, "@VM_EQ");
    });
    
    vmcase(VM_LEQ, {
        num_arith(<=, "@VM_LEQ");
    });

    vmcase(VM_GEQ, {
        num_arith(>=, "@VM_GEQ");
    });
    
    vmcase(VM_GOTO, {
        VM->ip = (int)((Object*)VM->program[VM->ip + 1])->value.number-1;
        vm_next;
    });
    
    vmcase(VM_POP, {
        if (VM->stack->top > 0) {
            list_spop(VM->stack);
            vm_next;
        }
        VM_throw_error(VM, VM_ERR_STACK, VM_ERRC_STACK_NOT_ENOUGH_ITEMS, "@VM_POP");
    });
    
    vmcase(VM_CALLF, {
        if (VM->stack->top <= 0) {
            VM_throw_error(VM, VM_ERR_STACK, VM_ERRC_STACK_NOT_ENOUGH_ITEMS, "@VM_CALLF");
            return 0;
        }
        Object func;
        func = obj_convert(list_get_top(VM->stack));
        if (func.type == T_CFUNCTION) {
            list_spop(VM->stack);
            int res = ((*(Cfunction)func.value.ptr)(VM));
            for (int i = 0; i < res && VM->stack->top > 0; i++) {
                list_spop(VM->stack);
            }
            vm_next;
        }
        VM_throw_error(VM, VM_ERR_CALL, VM_ERRC_NOT_A_FUNC, "@VM_CALLF");
        return 0;
    });
    
    vmcase(VM_IF, {
        if (VM->stack->top > 0) {   /* stack can not be empty */
            if (object_is_true(list_get_top(VM->stack))) {      /* if (true) {...} */
                list_spop(VM->stack);   /* pop top */
                vm_skip(1);
            } else {
                /*
                ** do a jump if statement is false
                ** if (false) {...}
                **          jump  ^
                */
                list_spop(VM->stack);
                VM->ip = (int)((Object*)VM->program[VM->ip + 1])->value.number - 1;
            }
        } else {
            VM_throw_error(VM, VM_ERR_STACK, VM_ERRC_STACK_NOT_ENOUGH_ITEMS, "@VM_IF");
        }
    });
    
    vmcase(VM_EXIT, {
        if (VM->stack->top > 0) {
            print_object(list_get_top(VM->stack));
            list_clear2(VM->stack);
        }
        return 1;
    });

    return 0;
}


/*
** convert input of tokens to an array of instructions
*/
void** to_ins(VM_instance* VM, Tokenlist* list) {
    Instruction_list ilist;
    list_init(refcast(ilist));
    void** result;

    Token current;
    for (int i = 0; i < list->top; i++) {
        current = list_get(list, i);

        switch (current.op) {
            case T_NUMBER: {
                /* pushk, value */
                list_push(refcast(ilist), list_get(VM->instructions, VMI_PUSHK));
                list_push(refcast(ilist), new(Object));
                (*(Object*)list_get_top(refcast(ilist))).type = T_NUMBER;
                (*(Object*)list_get_top(refcast(ilist))).value.number = to_number(current.token);
            }
                break;

           case T_IDENTIFIER: {
               list_push(refcast(ilist), list_get(VM->instructions, VMI_PUSHI));
               list_push(refcast(ilist), new(Object));
               (*(Object*)list_get_top(refcast(ilist))) = (Object){((union Value){}.string = current.token), T_IDENTIFIER};
            }
                break;

            case OP_EQ_ASSIGN:
                list_push(refcast(ilist), list_get(VM->instructions, VMI_EQ_ASSIGN));
                break;

            case OP_ADD:
                list_push(refcast(ilist), list_get(VM->instructions, VMI_ADD));
                break;

            case OP_SUB:
                list_push(refcast(ilist), list_get(VM->instructions, VMI_SUB));
                break;

            case OP_DIV:
                list_push(refcast(ilist), list_get(VM->instructions, VMI_DIV));
                break;

            case OP_MUL:
                list_push(refcast(ilist), list_get(VM->instructions, VMI_MUL));
                break;

            default:
                break;
        }
    }

    list_push(refcast(ilist), list_get(VM->instructions, VMI_EXIT));     /* must not forget to exit program */

    result = newx(void*, ilist.size);
    for (int i = 0; i < ilist.top; i++)
        result[i] = list_get(refcast(ilist), i);

    return result;
}

/*
** load string from a compiled file (or raw string input) and this function will return
** an array of instructions
*/
void** VM_string2bytecode(VM_instance* VM, char* input) {
    Instruction_list result;
    list_init(refcast(result));
    
    int limit = (int)strlen(input);
    for (int i = 0; i < limit; i++) {
        switch (input[i] - 65) {    /* - 65 offset for making the code human readable */
            case OP_EQ_ASSIGN:
                list_push(refcast(result),  list_get(VM->instructions, VMI_EQ_ASSIGN));
                break;
            
            case OP_ADD:
                list_push(refcast(result),  list_get(VM->instructions, VMI_ADD));
                break;
            
            case OP_SUB:
                list_push(refcast(result),  list_get(VM->instructions, VMI_SUB));
                break;
            
            case OP_MUL:
                list_push(refcast(result),  list_get(VM->instructions, VMI_MUL));
                break;
            
            case OP_DIV:
                list_push(refcast(result),  list_get(VM->instructions, VMI_DIV));
                break;
            
            case OP_EXIT:
                list_push(refcast(result),  list_get(VM->instructions, VMI_EXIT));
                break;
            
            case OP_PUSHK:
                list_push(refcast(result),  list_get(VM->instructions, VMI_PUSHK));
                break;
                
            case OP_PUSHP:
                list_push(refcast(result),  list_get(VM->instructions, VMI_PUSHP));
                break;
                
            case OP_GOTO:
                list_push(refcast(result),  list_get(VM->instructions, VMI_GOTO));
                break;
                
            case OP_IF:
                list_push(refcast(result),  list_get(VM->instructions, VMI_IF));
                break;
                
            case OP_PUSHI:
                list_push(refcast(result),  list_get(VM->instructions, VMI_PUSHI));
                break;
                
            case OP_LT:
                list_push(refcast(result), list_get(VM->instructions, VMI_LT));
                break;
                
            case OP_GT:
                list_push(refcast(result), list_get(VM->instructions, VMI_GT));
                break;
                
            case OP_EQ:
                list_push(refcast(result), list_get(VM->instructions, VMI_EQ));
                break;
                
            case OP_LEQ:
                list_push(refcast(result), list_get(VM->instructions, VMI_LEQ));
                break;
                
            case OP_GEQ:
                list_push(refcast(result), list_get(VM->instructions, VMI_GEQ));
                break;
            
            case OP_POP:
                list_push(refcast(result), list_get(VM->instructions, VMI_POP));
                break;
                
            case OP_CALLF:
                list_push(refcast(result), list_get(VM->instructions, VMI_CALLF));
                break;
            
            case OP_ADD_ASSIGN:
                list_push(refcast(result), list_get(VM->instructions, VMI_ADD_ASSIGN));
                break;
                
            case OP_SUB_ASSIGN:
                list_push(refcast(result), list_get(VM->instructions, VMI_SUB_ASSIGN));
                break;
                
            case OP_MUL_ASSIGN:
                list_push(refcast(result), list_get(VM->instructions, VMI_MUL_ASSIGN));
                break;
                
            case OP_DIV_ASSIGN:
                list_push(refcast(result), list_get(VM->instructions, VMI_DIV_ASSIGN));
                break;
                
            case OP_LOAD:
                list_push(refcast(result), list_get(VM->instructions, VMI_LOAD));
                break;
                
            case OP_STORE:
                list_push(refcast(result), list_get(VM->instructions, VMI_STORE));
                break;
            
            case '#' - 65: {
                /*
                ** skip till comment end (newline, #)
                ** # comment
                ** # this is also a comment #
                */
                while (i++ < limit) {
                    if (input[i] == '\n' || input[i] == '#')
                        break;
                }
            }
                break;
            
            case '%' - 65: {    /* fetch number: %number% */
                String temp;
                list_init(refcast(temp));
                Offset block;
                block.x = i + 1;
                while (i++ < limit) {
                    if (input[i] == '%') {
                        block.y = i;
                        break;
                    }
                }
                
                for (int i = block.x; i < block.y; i++) {
                    list_push(refcast(temp), input[i]);
                }
                
                list_push(refcast(temp), '\0');
                if (!is_number(temp.value)) {
                    VM_throw_error(VM, 0, 0, "Not a number");
                    return null;
                }
                
                object_create(number, number = to_number(temp.value), T_NUMBER);
                
                list_push(refcast(result), (void*)number);
                
                string_clear(refcast(temp));
            }
                break;
                
            case '\"' - 65: {   /* string: "string" */
                
            }
                break;
            
            case '?' - 65: {    /* identifier: ?test? */
                String temp;
                list_init(refcast(temp));
                Offset block;
                block.x = i + 1;
                while (i++ < limit) {
                    if (input[i] == '?') {
                        block.y = i;
                        break;
                    }
                }
                
                for (int i = block.x; i < block.y; i++) {
                    list_push(refcast(temp), input[i]);
                }
                
                list_push(refcast(temp), '\0');
                
                if (!is_identifier(temp.value)) {
                    VM_throw_error(VM, 0, 0, "Not a valid identifier");
                    return null;
                }
                
                object_create(identifier, string = temp.value, T_IDENTIFIER);
                
                list_push(refcast(result), identifier);
                
                temp.top = 0;
            }
                break;
            
            default:
                break;
        }
    }
    
    list_push(refcast(result), list_get(VM->instructions, VMI_EXIT));
    return result.value;
}

void VM_throw_error(VM_instance* VM, int error, int cause, const char* msg) {
    printf("%s%s%s\n", VM_error_messages[error], VM_error_cause_messages[cause], msg);
    if (VM->exit_on_error) {
        /*
        ** TODO: free everything
        */
        VM_instance_free(VM);
        exit(0);
    }
}

void** ins_add_instructions(int insc, void* ins, ...) {
    void** result = newx(void*, insc + 1);

    va_list args;
    va_start(args, ins);

    for (int i = 0; i < insc; i++)
        result[i] = va_arg(args, void*);

    va_end(args);

    return result;
}


/*
** find and print Virtual Machine instruction
*/
void VM_debug_print_vmi(VM_instance* VM, void* vmi) {
    for (int i = 0; i < VM->instructions->top; i++) {
        if (list_get(VM->instructions, i) == vmi) {
            /* found */
            puts(VMI_info[i]);
            return;
        }
    }
    puts("VMI undefined");
}

/*
** print all items in stack
*/
void VM_print_stack(VM_instance* VM) {
    for (int i = 0; i < VM->stack->top; i++) {
        print_object(list_get(VM->stack, i));
    }
}

void VM_instance_free(VM_instance* VM) {
    list_free(VM->stack);
    free(VM);
}
