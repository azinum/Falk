/* vm.c */
/* author: Azinum */
/* date: 19/09/16 */

#include "vm.h"


void VM_init(VM_instance* VM) {
    VM->init = 1;
    VM->stack = new(Stack);
    list_init(VM->stack);
    VM->ins = new(Instruction_list);
    list_init(VM->ins);
    VM->program = newx(void*, 28);
    
    VM->global = new(Scope);
    VM->global->global = VM->global;
    VM->global->variables = new(HashTable);
    table_init(VM->global->variables);
    
    table_push_object(VM->global->variables, "global", ptr = VM->global, T_SCOPE);
    table_push_object(VM->global->variables, "null", ptr = NULL, T_NULL);
    table_push_object(VM->global->variables, "pi", number = 3.14159265359, T_NUMBER);
    table_push_object(VM->global->variables, "vm", ptr = &VM, -1);
}

int VM_execute(VM_instance* VM, char* input) {
    Lex_instance* lex_instance = new(Lex_instance);
    lex_instance_init(lex_instance);
    
    list_push(VM->ins, &&VM_EQ_ASSIGN);
    list_push(VM->ins, &&VM_ADD);
    list_push(VM->ins, &&VM_SUB);
    list_push(VM->ins, &&VM_DIV);
    list_push(VM->ins, &&VM_MUL);
    list_push(VM->ins, &&VM_PUSHK);
    list_push(VM->ins, &&VM_PUSHIDF);
    list_push(VM->ins, &&VM_EXIT);
    
    if (!lex(lex_instance, input)) {
        return 0;
    }
    
    VM->ip = to_ins(VM, lex_instance->result);
    
    if (!VM->ip)
        return 0;
    
    goto **VM->ip;
    
    vmcase(VM_EQ_ASSIGN, {
        /*
        ** left hand side of assignment MUST be variable
        ** if not true, throw an error
        */
    });
    vmcase(VM_PUSHK, {
        list_push(VM->stack, *(Object*)*(VM->ip + 1));
        VM->ip++;
    });
    vmcase(VM_PUSHIDF, {
        char* name = ((Object*)*((VM->ip + 1)))->value.string;
        if (table_find(VM->global->variables, name) != NULL) {
            /*
            ** variable exist
            ** optimize: create opcode (VM_PUSHP, addr)
            */
            tobject_create(obj, ptr = table_find(VM->global->variables, name), T_VAR);
            list_push(VM->stack, obj);
            printf("Found variable (%s).\n", name);
        } else {
            /*
            ** variable does not exist
            ** create variable
            ** optimize code
            */
            table_push_object(VM->global->variables, name, ptr = NULL, T_NULL);
        }
        VM->ip++;
    });
    vmcase(VM_ADD, {
        /* 
        ** if we can do arithmetic operation,
        ** jump to next instruction, else: throw error message
        */
        num_arith(+);
        VM_throw_error(VM_ERR_STACK, VM_ERRC_STACK_NOT_ENOUGH_ITEMS, "@VM_ADD");
    });
    vmcase(VM_SUB, {
        num_arith(-);
        VM_throw_error(VM_ERR_STACK, VM_ERRC_STACK_NOT_ENOUGH_ITEMS, "@VM_SUB");
    });
    vmcase(VM_DIV, {
        num_arith(/);
        VM_throw_error(VM_ERR_STACK, VM_ERRC_STACK_NOT_ENOUGH_ITEMS, "@VM_DIV");
    });
    vmcase(VM_MUL, {
        num_arith(*);
        VM_throw_error(VM_ERR_STACK, VM_ERRC_STACK_NOT_ENOUGH_ITEMS, "@VM_MUL");
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
    void** result = newx(void*, list->top);
    int rtop = 0;
    
    Token current;
    for (int i = 0; i < list->top; i++) {
        current = list_get(list, i);
        
        switch (current.op) {
            case T_NUMBER: {
                result[rtop++] = list_get(VM->ins, VMI_PUSHK);
                Object* number = new(Object);
                number->type = T_NUMBER;
                number->value.number = to_number(current.token);
                result[rtop++] = number;
            }
                break;
                
            case T_IDENTIFIER: {
                result[rtop++] = list_get(VM->ins, VMI_PUSHIDF);
                Object* idf = new(Object);
                idf->type = T_IDENTIFIER;
                idf->value.string = current.token;
                result[rtop++] = idf;
            }
                break;
                
            case OP_ADD:
                result[rtop++] = list_get(VM->ins, VMI_ADD);
                break;
                
            case OP_SUB:
                result[rtop++] = list_get(VM->ins, VMI_SUB);
                break;
                
            case OP_DIV:
                result[rtop++] = list_get(VM->ins, VMI_DIV);
                break;
                
            case OP_MUL:
                result[rtop++] = list_get(VM->ins, VMI_MUL);
                break;
                
            default:
                break;
        }
    }
    
    result[rtop] = list_get(VM->ins, VMI_EXIT);
    return result;
}


void VM_throw_error(int error, int cause, const char* msg) {
    printf("%s%s; %s\n", VM_error_messages[error], VM_error_cause_messages[cause], msg);
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


void VM_instance_free(VM_instance* VM) {
    list_free(VM->stack);
    free(VM);
}