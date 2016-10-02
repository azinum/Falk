/* vm.c */
/* author: Azinum */
/* date: 19/09/16 */

#include "vm.h"


void VM_init(VM_instance* VM) {
    VM->init = 0;
    VM->stack = new(Stack);
    list_init(VM->stack);
    VM->ins = new(Instruction_list);
    list_init(VM->ins);
    VM->dummy = new(Object);
    VM->dummy->type = T_NULL;
    VM->exit_on_error = 1;
    
    VM->global = new(Scope);
    VM->global->global = VM->global;
    VM->global->variables = new(HashTable);
    table_init(VM->global->variables);
    
    table_push_object(VM->global->variables, "global", ptr = VM->global, T_SCOPE);
    table_push_object(VM->global->variables, "null", ptr = NULL, T_NULL);
    table_push_object(VM->global->variables, "undefined", ptr = NULL, T_NULL);
    table_push_object(VM->global->variables, "pi", number = 3.14159265359, T_NUMBER);
    table_push_object(VM->global->variables, "vm", ptr = &VM, -1);
}

int VM_execute(VM_instance* VM, char* input) {
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
    
    if (!VM->init) {
        list_push(VM->ins, &&VM_EQ_ASSIGN);
        list_push(VM->ins, &&VM_ADD);
        list_push(VM->ins, &&VM_SUB);
        list_push(VM->ins, &&VM_DIV);
        list_push(VM->ins, &&VM_MUL);
        list_push(VM->ins, &&VM_PUSHK);
        list_push(VM->ins, &&VM_PUSHIDF);
        list_push(VM->ins, &&VM_EXIT);
        VM->init = 1;
    }
    
    VM->ip = to_ins(VM, lexresult);
    
    if (!VM->ip)
        return 0;
    
    vm_begin;
    
    /*
    ** left hand side of assignment MUST be variable
    ** if not true, throw an error
    */
    vmcase(VM_EQ_ASSIGN, {
        if (VM->stack->top >= 2) {
            if (list_get_from_top(VM->stack, -1).type == T_VAR) {
                /* if we do not do obj_convert then, we assign by reference / pointer */
                /* it is a feature to be added later */
                (*(TValue*)(list_get_from_top(VM->stack, -1).value.ptr)).tval = obj_convert(list_get_top(VM->stack));
            } else {
                puts("Err");
            }
        } else {
            VM_throw_error(VM, VM_ERR_STACK, VM_ERRC_STACK_NOT_ENOUGH_ITEMS, "@VM_EQ_ASSIGN");
        }
    });
    vmcase(VM_PUSHK, {
        list_push(VM->stack, *(Object*)(*(VM->ip + 1)));
        if (list_get_top(VM->stack).type != T_NUMBER) {
            VM_throw_error(VM, VM_ERR_ARITH, VM_ERRC_ARITH_INVALID_TYPES, "@VM_PUSHK");
        }
        vm_skip(1);
    });
    vmcase(VM_PUSHIDF, {
        Object* next = (Object*)(*((VM->ip + 1)));
        char* name = next->value.string;
        if (table_find(VM->global->variables, name) != NULL) {
            /*
            ** variable exist
            ** optimize: create opcode (VM_PUSHP, addr)
            */
            tobject_create(obj, ptr = table_find(VM->global->variables, name), T_VAR);
            list_push(VM->stack, obj);
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
    vmcase(VM_ADD, {
        /* 
        ** if we can do arithmetic operation,
        ** jump to next instruction, else: throw error message
        */
        num_arith(+, "@VM_ADD");
    });
    vmcase(VM_SUB, {
        num_arith(-, "@VM_SUB");
    });
    vmcase(VM_DIV, {
        num_arith(/, "@VM_DIV");
    });
    vmcase(VM_MUL, {
        num_arith(*, "@VM_MUl");
    });
    vmcase(VM_EXIT, {
        VM_PRINT_EXTRA_INFO;
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
                list_push(refcast(ilist), list_get(VM->ins, VMI_PUSHK));
                list_push(refcast(ilist), new(Object));
                (*(Object*)list_get_top(refcast(ilist))).type = T_NUMBER;
                (*(Object*)list_get_top(refcast(ilist))).value.number = to_number(current.token);
            }
                break;
                
           case T_IDENTIFIER: {
               list_push(refcast(ilist), list_get(VM->ins, VMI_PUSHIDF));
               list_push(refcast(ilist), new(Object));
               (*(Object*)list_get_top(refcast(ilist))) = (Object){((union Value){}.string = current.token), T_IDENTIFIER};
            }
                break;
                
            case OP_EQ_ASSIGN:
                list_push(refcast(ilist), list_get(VM->ins, VMI_EQ_ASSIGN));
                break;
                
            case OP_ADD:
                list_push(refcast(ilist), list_get(VM->ins, VMI_ADD));
                break;
                
            case OP_SUB:
                list_push(refcast(ilist), list_get(VM->ins, VMI_SUB));
                break;
                
            case OP_DIV:
                list_push(refcast(ilist), list_get(VM->ins, VMI_DIV));
                break;
                
            case OP_MUL:
                list_push(refcast(ilist), list_get(VM->ins, VMI_MUL));
                break;
                
            default:
                break;
        }
    }
    
    list_push(refcast(ilist), list_get(VM->ins, VMI_EXIT));     /* must not forget to exit program */
    
    result = newx(void*, ilist.size);
    for (int i = 0; i < ilist.top; i++)
        result[i] = list_get(refcast(ilist), i);
    
    return result;
}


void VM_throw_error(VM_instance* VM, int error, int cause, const char* msg) {
    printf("%s%s %s\n", VM_error_messages[error], VM_error_cause_messages[cause], msg);
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
    for (int i = 0; i < VM->ins->top; i++) {
        if (list_get(VM->ins, i) == vmi) {
            /* found */
            puts(VMI_info[i]);
            return;
        }
    }
    puts("VMI undefined");
}

void VM_instance_free(VM_instance* VM) {
    list_free(VM->stack);
    free(VM);
}