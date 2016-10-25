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

#define VM_DEBUG 0

list_define(Stack, Object);
list_define(Instruction_list, void*);

/*
** VM debugging here
*/
#if VM_DEBUG
#define vm_begin VM_debug_print_vmi(VM, *(VM->ip)); goto **(VM->ip)
#define vm_next VM_debug_print_vmi(VM, *(VM->ip + 1)); goto **(++VM->ip)
#define vm_skip(N) VM_debug_print_vmi(VM, *(VM->ip + 1)); (VM->ip += N)
#define VM_PRINT_EXTRA_INFO \
printf("Stack top: %i, vars: %i\n", VM->stack->top, VM->global->variables->top);
#else
#define vm_begin goto **(VM->ip)
#define vm_next goto **(++VM->ip)
#define vm_skip(N) (VM->ip += N)
#define VM_PRINT_EXTRA_INFO
#endif

#define vm_jump(N) goto **(VM->ip += N)

#define vmcase(CASE, BODY) { \
    CASE : { BODY ; } \
    vm_next; \
}


#define op_arith(L, R, OP) \
((op_arith_issafe(L, R) ? (L.value.number OP R.value.number) : (-1))

/*
** this macro is for type checking, to check if two objects has the same type
*/
#define op_arith_issafe(L, R) ((L.type | R.type) == T_NUMBER))

/*
** compare type between two objects
*/
#define comp_type(A, B, TYPE) ((A.type | B.type) == TYPE)

/*
** convert var to object
** if variable is detected, convert it to object
** else: do nothing with it
*/
#define obj_convert(O) ((O.type == T_VAR) ? ((*(TValue*)(O.value.ptr)).tval) : (O))

/*
** TODO: throw exception on error
*/
#define num_arith(OP, MSG) \
if (VM->stack->top >= 2) { \
    list_get_from_top(VM->stack, -1).value.number = op_arith( \
        obj_convert(list_get_from_top(VM->stack, -1)), \
        obj_convert(list_get_top(VM->stack)), \
    OP); \
    list_spop(VM->stack); \
    list_get_top(VM->stack).type = T_NUMBER; \
    vm_next; \
} else { \
    VM_throw_error(VM, VM_ERR_STACK, VM_ERRC_STACK_NOT_ENOUGH_ITEMS, MSG); \
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
    VMI_EXIT,
    VMI_GOTO
};


enum VM_exec_mode {
    VM_EXEC_INTERPRET,      /* includes lexing, parsing and final vm execution step */
    VM_EXEC_FILE,   /* deserialize file and convert to vm instructions */
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
    "VMI_GOTO"
};

enum VM_errors {
    VM_NO_ERROR = 0,
    VM_ERR_STACK,
    VM_ERR_ARITH,
    VM_INS_CONV_ERR,    /* instruction conversion error */
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
    "Stack Error: ",
    "Arithmetic Error: ",
    "Instruction Conversion Error: ",
};

static const char* VM_error_cause_messages[] = {
    "",
    /* messages for stack */
    "Not enough items on stack;",
    "Stack overflow;",
    "Stack has not been initialized yet;",

    /* arithmetic error causes */
    "Invalid types on arithmetic operation;",
};

typedef struct Scope {
    struct Scope* global;
    HashTable* variables;
} Scope;

typedef struct VM_instance {
    unsigned char init;     /* VM initialized? */
    unsigned char exit_on_error;
    void** ip;      /* pointer to an instruction */
    Instruction_list* instructions;     /* all instructions */
    Stack* stack;   /* list of objects */
    Scope* global;  /* global scope */
    Object* dummy;
} VM_instance;

int VM_execute(VM_instance* VM, int mode, char* input);

void VM_init(VM_instance* VM);

void VM_instance_free(VM_instance* VM);

void** ins_add_instructions(int insc, void* ins, ...);

void** to_ins(VM_instance* VM, Tokenlist* list);

void** string2bytecode(VM_instance* VM, char* input);

void VM_throw_error(VM_instance* VM, int error, int cause, const char* msg);

void VM_debug_print_vmi(VM_instance* VM, void* vmi);

#endif /* vm_h */
