/* vm.h */
/* author: Azinum */
/* date: 19/09/16 */

#ifndef vm_h
#define vm_h

#include <stdio.h>
#include <stdarg.h>
#include "list.h"
#include "object.h"
#include "lex.h"
#include "table.h"

#define VM_DEBUG 1

list_define(Stack, Object);
list_define(Instruction_list, void*);

/*
** VM debugging here
*/
#if VM_DEBUG
#define vm_next VM_debug_print_vmi(VM, *(VM->ip + 1)); goto **(++VM->ip);
#define vm_skip(N) VM_debug_print_vmi(VM, *(VM->ip + 1)); (VM->ip += N)
#else
#define vm_next goto **(++VM->ip)
#define vm_skip(N) (VM->ip += N)
#endif

#define vm_jump(N) goto **(VM->ip += N)

#define vmcase(CASE, BODY) { \
    CASE : { BODY ; } \
    vm_next; \
}


#define op_arith(L, R, OP) \
(((L.type | R.type) == T_NUMBER) ? (L.value.number OP R.value.number) : (0))

#define num_arith(OP) \
if (VM->stack->top >= 2) { \
    list_get_from_top(VM->stack, -1).value.number = op_arith(list_get_from_top(VM->stack, -1), list_get_top(VM->stack), OP); \
    list_spop(VM->stack); \
    goto **(++VM->ip); \
}

/*
** WARNING!
** order reserved
*/
enum VM_instructions {
    VMI_EQ_ASSIGN,
    VMI_ADD,
    VMI_SUB,
    VMI_DIV,
    VMI_MUL,
    VMI_PUSHK,
    VMI_PUSHIDF,
    
    VMI_EXIT
};

static const char* VMI_info[] = {
    "VMI_EQ_ASSIGN",
    "VMI_ADD",
    "VMI_SUB",
    "VMI_DIV",
    "VMI_MUL",
    "VMI_PUSHK",
    "VMI_PUSHIDF",
    "VMI_EXIT",
};

enum VM_errors {
    VM_NO_ERROR = 0,
    VM_ERR_STACK,
    VM_ERR_ARITH,
};

enum VM_error_causes {
    VM_ERRC_NONE,
    /* stack */
    VM_ERRC_STACK_NOT_ENOUGH_ITEMS,
    VM_ERRC_STACK_OVERFLOW,
    VM_ERRC_STACK_NOT_INIT,
    /* arith */
    VM_ERRC_ARITH_INVALID_TYPES,
};

static const char* VM_error_messages[] = {
    "",
    "Stack error: ",
    "Arithmetic error: ",
};

static const char* VM_error_cause_messages[] = {
    "",
    /* messages for stack */
    "Not enough items on stack",
    "StackOverflow",
    "Stack has not been initialized yet",
    
    /* arithmetic error causes */
    "Invalid types on arithmetic operation",
};

typedef struct Scope {
    struct Scope* global;
    HashTable* variables;
} Scope;

typedef struct VM_instance {
    unsigned char init;     /* is VM initialized? */
    void** program;     /* program itself */
    void** ip;      /* pointer to an instruction */
    Instruction_list* ins;     /* all instructions */
    Stack* stack;   /* list of objects */
    Scope* global;  /* global scope */
} VM_instance;

int VM_execute(VM_instance* VM, char* input);

void VM_init(VM_instance* VM);

void VM_instance_free(VM_instance* VM);

void** ins_add_instructions(int insc, void* ins, ...);

void** to_ins(VM_instance* VM, Tokenlist* list);

void VM_throw_error(int error, int cause, const char* msg);

void VM_debug_print_vmi(VM_instance* VM, void* vmi);

#endif /* vm_h */
