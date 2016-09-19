/* vm.c */
/* author: Azinum */
/* date: 19/09/16 */

#include "vm.h"


void VM_init(VM_instance* VM) {
    VM->init = 1;
    VM->stack = new(Stack);
    list_init(VM->stack);
    VM->ins = newx(void*, 28);
}

int VM_execute(VM_instance* VM) {
    VM->ins = ins_add_instructions(3, 0,
        &&VM_ADD,
        &&VM_SUB,
        &&VM_DIV,
        &&VM_MUL,
        &&VM_PUSHK,
        &&VM_PUSHIDF,
        &&VM_EXIT
    );
    
    Object test;
    test.value.number = 5;
    test.type = T_NUMBER;
    
    VM->program = ins_add_instructions(3, 0,
                                       &&VM_PUSHK, &test, &&VM_EXIT);
    
    VM->ip = VM->program;
    
    goto **VM->ip;
    
    vmcase(VM_PUSHK,
        puts("PushK");
        list_push(VM->stack, *(Object*)*(VM->ip + 1));
        printf("%i\n", (int)list_get_top(VM->stack).value.number);
        VM->ip++;
    );
    vmcase(VM_PUSHIDF,
        puts("PushIDF");
    );
    vmcase(VM_ADD,
        puts("Add");
    );
    vmcase(VM_SUB,
       puts("Sub");
    );
    vmcase(VM_DIV,
       puts("Div");
    );
    vmcase(VM_MUL,
        puts("Mul");
    );
    vmcase(VM_EXIT,
       puts("Exit");
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
                result[rtop++] = VM->ins[VMI_PUSHK];
                result[rtop++] = new(Object);
                ((Object*)result[rtop])->type = T_NUMBER;
                ((Object*)result[rtop])->value.number = to_number(current.token);
            }
                break;
        }
    }
    
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