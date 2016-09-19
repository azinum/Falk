/* vm.h */
/* author: Azinum */
/* date: 19/09/16 */

#ifndef vm_h
#define vm_h

#include <stdio.h>
#include <stdarg.h>
#include "list.h"
#include "object.h"

list_define(Stack, Object);

#define vmcase(CASE, BODY) { \
    CASE : { BODY ; } \
    goto **(++VM->ip); \
}

/*
** WARNING!
** order reserved
*/
enum VM_instructions {
    VMI_ADD,
    VMI_SUB,
    VMI_DIV,
    VMI_MUL,
    VMI_PUSHK,
    VMI_PUSHIDF,
    
    VMI_EXIT
};

typedef struct VM_instance {
    unsigned char init;     /* is VM initialized? */
    void** program;     /* program itself */
    void** ip;      /* pointer to an instruction */
    void** ins;     /* all instructions */
    Stack* stack;   /* list of objects */
} VM_instance;

int VM_execute(VM_instance* VM);

void VM_init(VM_instance* VM);

void VM_instance_free(VM_instance* VM);

void** ins_add_instructions(int insc, void* ins, ...);

void** to_ins(VM_instance* VM, Tokenlist* list);

#endif /* vm_h */
