/* vm.c */
/* Author: Azinum */
/* Date: 2016-09-19 */

#include <time.h>
#include <string.h>

#include "vm.h"
#include "falk.h"
#include "io.h"
#include "libstd.h"

int VM_init(VM_instance* VM) {
    if (!VM) {
        return 0;
    }
    VM->init = 0;   /* initialize VM instructions */
    /* custom stack init { */
    VM->stack_size = 28;
    VM->stack = new(Stack);
    VM->stack->value = newx(Object, VM->stack_size);
    VM->stack->top = 0;
    VM->stack->size = VM->stack_size;
    /* } */
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

    VM->obj_null.type = T_NULL;
    VM->obj_null.value.number = 0;

    table_push_object(VM->global->variables, "global", ptr = VM->global, T_SCOPE);
    table_push_object(VM->global->variables, "null", ptr = NULL, T_NULL);
    table_push_object(VM->global->variables, "undefined", ptr = NULL, T_NULL);
    table_push_object(VM->global->variables, "pi", number = 3.14159265359, T_NUMBER);
    table_push_object(VM->global->variables, "vm", ptr = &VM, T_POINTER);
    table_push_object(VM->global->variables, "test", number = 32, T_NUMBER);
    
    return 1;
}

int VM_execute(VM_instance* VM, int mode, char* input) {
    double start = clock();

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
        list_push(VM->instructions, &&VM_GOTO_LABEL);
        list_push(VM->instructions, &&VM_LABEL_DEFINE);
        VM->init = 1;
    }

    switch (mode) {
        case VM_EXEC_INTERPRET: {
            VM->program = VM_asm2bytecode(VM, input);
        }
            break;

        case VM_EXEC_FILE: {
            char* read = file_read(input);
            if (read != NULL) {
                VM->program = VM_asm2bytecode(VM, read);
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

    VM->ip = 0;

    vm_begin;

    /*
    ** left hand side of assignment MUST be variable
    ** if not true, throw an error
    */
    vmcase(VM_EQ_ASSIGN, {
        if (VM->stack->top > 1) {
            Object left = list_get_from_top(VM->stack, -1);
            const Object right = list_get_top(VM->stack);
            
            switch (right.type) {
                case T_NUMBER: {
                    left.value.obj->value.number = right.value.number;
                }
                    break;
                    
                case T_CSTRING: {
                    char* value = right.value.string;
                    unsigned long len = strlen(value);
                    left.value.string = newx(char, len);
                    strcpy(left.value.string, value);
                }
                    break;
                
                default: {
                    puts("Invalid variable assignment");
                }
                    break;
            }
            vm_stack_pop();
            vm_next;
        }
        VM_throw_error(VM, VM_ERR_STACK, VM_ERRC_STACK_NOT_ENOUGH_ITEMS, "eq_assign");
    });

    vmcase(VM_ADD_ASSIGN, {
        num_assign(+=, "add_assign");
    });

    vmcase(VM_SUB_ASSIGN, {
        num_assign(-=, "sub_assign");
    });

    vmcase(VM_MUL_ASSIGN, {
        num_assign(*=, "mul_assign");
    });

    vmcase(VM_DIV_ASSIGN, {
        num_assign(/=, "div_assign");
    });

    vmcase(VM_PUSHK, {
        vm_stack_push(*((Object*)VM->program[++VM->ip]), "pushk");
    });

    vmcase(VM_PUSHP, {
        vm_stack_push(*((Object*)VM->program[++VM->ip]), "pushp");
    });

    vmcase(VM_PUSHI, {
        Object* next = (Object*)(((VM->program[VM->ip + 1])));
        char* name = next->value.string;
        
        TValue* var = table_find(VM->global->variables, name);
        
        if (var->value.type != T_NULL) {
            /*
            ** variable exist
            ** optimize: create opcode (VM_PUSHP, addr)
            */
            tobject_create(obj, obj = &var->value, T_VAR);
            vm_stack_push(obj, "pushi");
            VM->program[VM->ip++] = list_get(VM->instructions, VMI_PUSHP);
            VM->program[VM->ip++] = &obj;
            vm_begin;
        }
        /*
        ** variable does not exist
        ** create variable
        ** optimize code
        */
        table_push_object(VM->global->variables, name, ptr = NULL, T_NULL);
        tobject_create(obj, obj = &table_find(VM->global->variables, name)->value, T_VAR);
        vm_stack_push(obj, "pushi");

        vm_jump(2);
    });

    /*
    ** if we can do arithmetic operation,
    ** jump to next instruction, else: throw error message
    */
    vmcase(VM_ADD, {
        num_arith(+, "add");
    });

    vmcase(VM_SUB, {
        num_arith(-, "sub");
    });

    vmcase(VM_DIV, {
        num_arith(/, "div");
    });

    vmcase(VM_MUL, {
        num_arith(*, "mul");
    });

    vmcase(VM_LT, {
        num_arith(<, "lt");
    });

    vmcase(VM_GT, {
        num_arith(>, "gt");
    });

    vmcase(VM_EQ, {
        num_arith(==, "eq");
    });

    vmcase(VM_LEQ, {
        num_arith(<=, "leq");
    });

    vmcase(VM_GEQ, {
        num_arith(>=, "geq");
    });

    vmcase(VM_GOTO, {
        VM->ip = (int)((Object*)VM->program[VM->ip + 1])->value.number - 1;
    });
    
    vmcase(VM_GOTO_LABEL, {
        Object obj = *(Object*)VM->program[VM->ip + 1];
        
        if (obj.type == T_VAR) {
            obj = obj_convert(obj);
            if (object_is_number(obj)) {
                if (obj.value.number <= VM->program_size) {
                    VM->ip = (int)obj.value.number;
                    vm_begin;
                }
                VM_throw_error(VM, VM_ERR_LABEL, VM_ERRC_NUMBER_TOO_BIG, "goto_label");
            }
            VM_throw_error(VM, VM_ERR_LABEL, VM_ERRC_NOT_A_NUMBER, "goto_label");
        }
        if (obj.type == T_IDENTIFIER) {
            Object var = variable_find(VM, obj.value.string);
            if (var.type != T_NULL) {
                tobject_create(obj, obj = &var, T_VAR);
                (*(Object*)VM->program[VM->ip + 1]) = obj;
                if (object_is_number(var)) {
                    if (var.value.number <= VM->program_size) {
                        VM->ip = (int)var.value.number;
                        vm_begin;
                    }
                    VM_throw_error(VM, VM_ERR_LABEL, VM_ERRC_NUMBER_TOO_BIG, "goto_label");
                }
                VM_throw_error(VM, VM_ERR_LABEL, VM_ERRC_NOT_A_NUMBER, "goto_label");
            }
            VM_throw_error(VM, VM_ERR_LABEL, VM_ERRC_INVALID_LABEL, "goto_label");
        }
        VM_throw_error(VM, VM_ERR_LABEL, VM_ERRC_INVALID_LABEL, "goto_label");
    });
    
    /*
    ** define a label
    **
    ** pushi test label
    **   # BODY
    ** pushi test goto2
    */
    vmcase(VM_LABEL_DEFINE, {
        if (VM->stack->top > 0) {
            Object obj = list_get_top(VM->stack);
            if (obj.type == T_VAR) {
                obj_convert2(obj)->type = T_NUMBER;
                obj_convert2(obj)->value.number = VM->ip + 1;
                vm_stack_pop();
            }
            vm_next;
        }
        VM_throw_error(VM, VM_ERR_STACK, VM_ERRC_STACK_NOT_ENOUGH_ITEMS, "label_define");
    });
    
    vmcase(VM_POP, {
        if (VM->stack->top > 0) {
            vm_stack_pop();
            vm_next;
        }
        VM_throw_error(VM, VM_ERR_STACK, VM_ERRC_STACK_NOT_ENOUGH_ITEMS, "pop");
    });

    /*
    ** call C/Falk function
    */
    vmcase(VM_CALLF, {
        /*
        ** number of items on stack must be 2 or more (function and argc)
        */
        if (VM->stack->top > 1) {
            Object func;
            Object argc;

            func = obj_convert(list_get_from_top(VM->stack, -1));
            argc = list_get_top(VM->stack);

            if (func.type == T_CFUNCTION && argc.type == T_NUMBER) {
                vm_stack_pop();     /* pop function */
                vm_stack_pop();     /* pop argc */
                /* leftovers are all the args */
                Object rvalue = func.value.func(VM);
                /* pop all args (Cfunctions may not increment or decrement stack, it's okay to modify items on stack though) */
                /* for (int i = 0; i < (int)argc.value.number; i++) {
                    vm_stack_pop();
                } */
                vm_stack_push(rvalue, "callf");     /* push rvalue to stack */
                vm_next;
            }
        }
    });

    vmcase(VM_IF, {
        if (VM->stack->top > 0) {   /* stack can not be empty */
            if (object_is_true(list_get_top(VM->stack))) {      /* if (true) {...} */
                vm_stack_pop();   /* pop top */
                vm_jump(2);
            }
            /*
            ** do a jump if statement is false
            ** if (false) {...}
            **          jump  ^
            */
            vm_stack_pop();
            VM->ip = (int)((Object*)VM->program[VM->ip + 1])->value.number;
            if (VM->ip > VM->program_size) {
                VM->ip = VM->program_size - 1;
            }
            vm_begin;
        }
        VM_throw_error(VM, VM_ERR_STACK, VM_ERRC_STACK_NOT_ENOUGH_ITEMS, "if");
    });

    vmcase(VM_EXIT, {
        if (VM->stack->top > 0) {
            print_object(list_get_top(VM->stack));
            list_sclear(VM->stack);
        }
        printf("%.6g ms\n", (double)(clock() - start) / 1000.0f);
        return 1;
    });

    return 0;
}


Object variable_find(VM_instance* VM, char* name) {
    if (table_find(VM->global->variables, name)) {
        TValue* var = table_find(VM->global->variables, name);
        return var->value;
    }
    return falk_create_null(VM);
}

/*
** convert input of tokens to an array of instructions
*/
void** VM_list2instructions(VM_instance* VM, Tokenlist* list) {
    Instruction_list ilist;
    list_init((&ilist));
    void** result;

    Token current;
    for (int i = 0; i < list->top; i++) {
        current = list_get(list, i);

        switch (current.type) {
            case T_NUMBER: {
                /* pushk, value */
                list_push((&ilist), list_get(VM->instructions, VMI_PUSHK));
                list_push((&ilist), new(Object));
                (*(Object*)list_get_top((&ilist))).type = T_NUMBER;
                (*(Object*)list_get_top((&ilist))).value.number = to_number(current.value);
            }
                break;

           case T_IDENTIFIER: {
               list_push((&ilist), list_get(VM->instructions, VMI_PUSHI));
               list_push((&ilist), new(Object));
               (*(Object*)list_get_top((&ilist))) = (Object){{((union Value){}.string = current.value)}, T_IDENTIFIER};
            }
                break;

            case OP_EQ_ASSIGN:
                list_push((&ilist), list_get(VM->instructions, VMI_EQ_ASSIGN));
                break;

            case OP_ADD:
                list_push((&ilist), list_get(VM->instructions, VMI_ADD));
                break;

            case OP_SUB:
                list_push((&ilist), list_get(VM->instructions, VMI_SUB));
                break;

            case OP_DIV:
                list_push((&ilist), list_get(VM->instructions, VMI_DIV));
                break;

            case OP_MUL:
                list_push((&ilist), list_get(VM->instructions, VMI_MUL));
                break;

            default:
                break;
        }
    }

    list_push((&ilist), list_get(VM->instructions, VMI_EXIT));     /* must not forget to exit program */

    result = newx(void*, ilist.size);
    for (int i = 0; i < ilist.top; i++)
        result[i] = list_get((&ilist), i);

    return result;
}

/*
** load string from a compiled file (or raw string input) and this function will return
** an array of instructions
*/
void** VM_string2bytecode(VM_instance* VM, char* input) {
    Instruction_list result;
    list_init((&result));

    int limit = (int)strlen(input);
    for (int i = 0; i < limit; i++) {
        switch (input[i] - 65) {    /* - 65 offset for making the code human readable */
            case OP_EQ_ASSIGN:
                list_push((&result),  list_get(VM->instructions, VMI_EQ_ASSIGN));
                break;

            case OP_ADD:
                list_push((&result),  list_get(VM->instructions, VMI_ADD));
                break;

            case OP_SUB:
                list_push((&result),  list_get(VM->instructions, VMI_SUB));
                break;

            case OP_MUL:
                list_push((&result),  list_get(VM->instructions, VMI_MUL));
                break;

            case OP_DIV:
                list_push((&result),  list_get(VM->instructions, VMI_DIV));
                break;

            case OP_EXIT:
                list_push((&result),  list_get(VM->instructions, VMI_EXIT));
                break;

            case OP_PUSHK:
                list_push((&result),  list_get(VM->instructions, VMI_PUSHK));
                break;

            case OP_PUSHP:
                list_push((&result),  list_get(VM->instructions, VMI_PUSHP));
                break;

            case OP_GOTO:
                list_push((&result),  list_get(VM->instructions, VMI_GOTO));
                break;

            case OP_IF:
                list_push((&result),  list_get(VM->instructions, VMI_IF));
                break;

            case OP_PUSHI:
                list_push((&result),  list_get(VM->instructions, VMI_PUSHI));
                break;

            case OP_LT:
                list_push((&result), list_get(VM->instructions, VMI_LT));
                break;

            case OP_GT:
                list_push((&result), list_get(VM->instructions, VMI_GT));
                break;

            case OP_EQ:
                list_push((&result), list_get(VM->instructions, VMI_EQ));
                break;

            case OP_LEQ:
                list_push((&result), list_get(VM->instructions, VMI_LEQ));
                break;

            case OP_GEQ:
                list_push((&result), list_get(VM->instructions, VMI_GEQ));
                break;

            case OP_POP:
                list_push((&result), list_get(VM->instructions, VMI_POP));
                break;

            case OP_CALLF:
                list_push((&result), list_get(VM->instructions, VMI_CALLF));
                break;

            case OP_ADD_ASSIGN:
                list_push((&result), list_get(VM->instructions, VMI_ADD_ASSIGN));
                break;

            case OP_SUB_ASSIGN:
                list_push((&result), list_get(VM->instructions, VMI_SUB_ASSIGN));
                break;

            case OP_MUL_ASSIGN:
                list_push((&result), list_get(VM->instructions, VMI_MUL_ASSIGN));
                break;

            case OP_DIV_ASSIGN:
                list_push((&result), list_get(VM->instructions, VMI_DIV_ASSIGN));
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
                list_init((&temp));
                Offset block;
                block.x = i + 1;
                block.y = block.x;
                while (i++ < limit) {
                    if (input[i] == '%') {
                        block.y = i;
                        break;
                    }
                }

                for (int i = block.x; i < block.y; i++) {
                    list_push((&temp), input[i]);
                }

                string_push((&temp), "");
                if (!is_number(temp.value)) {
                    VM_throw_error(VM, 0, 0, "Not a number");
                    return null;
                }

                object_create(number, number = to_number(temp.value), T_NUMBER);

                list_push((&result), (void*)number);

                string_clear((&temp));
            }
                break;

            case '\"' - 65: {   /* string: "string" */
                String temp;
                list_init((&temp));
                Offset block;
                block.x = i + 1;
                while (i++ < limit) {
                    if (input[i] == '\"') {
                        block.y = i;
                        break;
                    }
                }

                for (int i = block.x; i < block.y; i++) {
                    list_push((&temp), input[i]);
                }

                list_push((&temp), '\0');

                object_create(string, string = temp.value, T_CSTRING);

                list_push((&result), string);

                temp.top = 0;
            }
                break;

            case '?' - 65: {    /* identifier: ?test? */
                String temp;
                list_init((&temp));
                Offset block;
                block.x = i + 1;
                while (i++ < limit) {
                    if (input[i] == '?') {
                        block.y = i;
                        break;
                    }
                }

                for (int i = block.x; i < block.y; i++) {
                    list_push((&temp), input[i]);
                }

                list_push((&temp), '\0');

                if (!is_identifier(temp.value)) {
                    VM_throw_error(VM, 0, 0, "Not a valid identifier");
                    return null;
                }

                object_create(identifier, string = temp.value, T_IDENTIFIER);

                list_push((&result), identifier);

                temp.top = 0;
            }
                break;

            default:
                break;
        }
    }

    list_push((&result), list_get(VM->instructions, VMI_EXIT));
    return result.value;
}

void** VM_asm2bytecode(VM_instance* VM, char* input) {
    Instruction_list result;
    list_init((&result));

    Lex_instance* lex_instance = new(Lex_instance);
    lex_instance_init(lex_instance);
    lex(lex_instance, input);

    for (int i = 0; i < lex_instance->result.top; i++) {
        Token current = list_get((&lex_instance->result), i);
        switch (current.type) {
            case T_CSTRING: {
                object_create(obj, string = current.value, T_CSTRING);
                list_push((&result), (void*)obj);
            }
                break;

            case T_NUMBER: {
                object_create(obj, number = to_number(current.value), T_NUMBER);
                list_push((&result), (void*)obj);
            }
                break;

            case T_IDENTIFIER: {
                Token temp = current;
                for (int i = 0; i < arr_size(vm_asm_keywords); i++) {
                    if (!strcmp(current.value, vm_asm_keywords[i].value)) {
                        temp = vm_asm_keywords[i];
                        break;
                    }
                }

                if (temp.type == T_IDENTIFIER) {
                    object_create(obj, string = temp.value, T_IDENTIFIER);
                    list_push((&result), (void*)obj);
                } else {
                    list_push((&result),  list_get(VM->instructions, temp.type));
                }
            }
                break;

            default:
                break;
        }
    }

    list_push((&result), list_get(VM->instructions, VMI_EXIT));
    VM->program_size = result.top;
    
    return result.value;
}

void VM_throw_error(VM_instance* VM, int error, int cause, const char* msg) {
    printf("%s%s@%s.\n",
           VM_error_messages[error],
           VM_error_cause_messages[cause],
           msg);
    if (VM->exit_on_error) {
        /*
        ** TODO: free everything
        */
        VM_instance_free(VM);
        exit(0);
    }
}

void** VM_ins_add_instructions(int insc, void* ins, ...) {
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
