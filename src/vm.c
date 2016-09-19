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
}

int VM_execute(VM_instance* VM, char* input) {
    Lex_instance* lex_instance = new(Lex_instance);
    lex_instance_init(lex_instance);
    
    list_push(VM->ins, &&VM_ADD);
    list_push(VM->ins, &&VM_SUB);
    list_push(VM->ins, &&VM_DIV);
    list_push(VM->ins, &&VM_MUL);
    list_push(VM->ins, &&VM_PUSHK);
    list_push(VM->ins, &&VM_PUSHIDF);
    list_push(VM->ins, &&VM_EXIT);
    
    if (!lex(lex_instance, input))
        return 0;
    
    VM->ip = new(void*);
    
    VM->ip = to_ins(VM, lex_instance->result);
    
    goto **VM->ip;
    
    vmcase(VM_PUSHK,
        list_push(VM->stack, *(Object*)*(VM->ip + 1));
        VM->ip++;
    );
    vmcase(VM_PUSHIDF,
        
    );
    vmcase(VM_ADD,
        if (VM->stack->top > 2) {
            list_get_from_top(VM->stack, -1).value.number = op_arith(list_get_from_top(VM->stack, -1), list_get_top(VM->stack), +);
            list_spop(VM->stack);
        }
    );
    vmcase(VM_SUB,
       
    );
    vmcase(VM_DIV,
       
    );
    vmcase(VM_MUL,
        
    );
    vmcase(VM_EXIT,
        printf("%i\n", (int)list_get_top(VM->stack).value.number);
       puts("EXIT");
       return 1;
    );
    
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
                
            case OP_ADD: {
                result[rtop++] = list_get(VM->ins, VMI_ADD);
            }
                break;
                
            default:
                break;
        }
    }
    
    result[rtop] = list_get(VM->ins, VMI_EXIT);
    return result;
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
}