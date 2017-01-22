/*
** main.c - uFalk | micro Falk Virtual Machine
** Author: Azinum
** Date: 2017-01-21
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define alloc_new(type) (type*)malloc(sizeof(type))
#define alloc_new_x(type, size) (type*)malloc(sizeof(type) * size)
#define realloc_x(var, size) var = realloc(var, sizeof(var) * size)

#define arr_size(arr) ((sizeof(arr)) / (sizeof(arr[0])))

#define stack_push(value) vm->stack[++vm->top] = *(Object*)&code[ip]
#define stack_pop() --vm->top
#define stack_get_top() (vm->stack[vm->top])

#define op_arith(op) vm->stack[--vm->top].value.number op stack_get_top().value.number

enum Instructions {
    I_EXIT = 1,
    I_PUSH,
    I_POP,
    I_GOTO,
    I_IF,
    
    I_ADD,
    I_SUB,
    I_MUL,
    I_DIV,
    
    /* Object types */
    T_NUMBER,
    T_IDENTIFIER,
};

const char valid_characters[]       = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM_";
const char valid_characters_after[] = "1234567890";
const char delim_tokens[] = "!?%&|^~(){}[]+-*/=\n\t; ";


typedef struct Object {
    union {
        double number;
        struct Object* next;
    } value;
    unsigned char type;
} Object;

typedef struct Token {
    char* value;
    unsigned char type;
} Token;

typedef struct UVM_state {
    unsigned short top;
    struct Object stack[32];
    unsigned int lex_size;
} UVM_state;

struct Token tokens[] = {
    {"exit", I_EXIT},
    {"push", I_PUSH},
    {"goto", I_GOTO},
    {"?", I_IF},
    {"pop", I_POP},
    {"add", I_ADD},
    {"sub", I_SUB},
    {"mul", I_MUL},
    {"div", I_DIV},
};

int UVM_init(UVM_state* vm) {
    vm->top = 0;
    vm->lex_size = 0;
    return 1;
}

void UVM_print_top(UVM_state* vm) {
    Object obj = stack_get_top();
    switch (obj.type) {
        case T_NUMBER:
            printf("%.7g\n", obj.value.number);
            break;
        
        default:
            break;
    }
}

unsigned char is_delim_token(UVM_state* vm, char* str, unsigned int i) {
    char ch = str[i];
    unsigned int end = 0;
    do {
        if (ch == delim_tokens[end])
            return 1;
    } while (delim_tokens[end++] != '\0');
    return 0;
}

/*
** Delimit everything with either space or operator (see delim_tokens), do error-checking later.
** "h+b lwdq8 12" => ["h", "+", "b", "lwdq8", "12"]
*/
char** UVM_lex(UVM_state* vm, char* str) {
    unsigned long size = strlen(str);
    char** rvalue, temp[size];
    rvalue = alloc_new(char*);
    
    unsigned int i = 0;     /* index for temp string */
    unsigned int rvi = 0;   /* return value index */
    unsigned int it = 0;    /* iterate input string */
    do {
        if (is_delim_token(vm, str, it)) {
            temp[i] = '\0';     /* null-terminate tmp string */
            i = 0;
            
            realloc_x(rvalue, rvi + 2);
            rvalue[rvi] = alloc_new_x(char, i);
            strcpy(rvalue[rvi++], temp);
            rvalue[rvi] = alloc_new(char);
            *rvalue[rvi] = str[it];
            rvalue[rvi++][1] = '\0';

            for (int j = 0; j < size; j++) {
                temp[j] = 0;
            }
            it++;
        }
        temp[i++] = str[it];
    } while (it++ < size);
   
    vm->lex_size = rvi;
    
    return rvalue;
}

unsigned char is_keyword(char* str) {
    for (int i = 0; i < arr_size(tokens); i++) {
        if (!strcmp(str, tokens[i].value)) {
            return tokens[i].type;
        }
    }
    return 0;
}

unsigned char is_number(const char* string) {
    if (strlen(string) <= 0)
        return 0;
    
    char* end;
    strtod(string, &end);
    
    if (*end != '\0')
        return 0;
    
    return 1;
}

double to_number(const char* string) {
    if (strlen(string) <= 0)
        return 0;
    
    char* end;
    double number = strtod(string, &end);
    
    if (*end != '\0')
        return -1;
    
    return number;
}

unsigned char* UVM_parse(UVM_state* vm, char** input) {
    unsigned int grow = 512;    /* how many bytes to grow each reallocation */
    unsigned int size = grow;
    unsigned char* bytecode = alloc_new_x(unsigned char, size);
    
    unsigned int i,     /* index of input */
        it,             /* iterator for bytecode list */
        instruction;    /* current instruction */
    i = instruction = it = 0;
    do {
        if (is_number(input[i])) {
            Object obj;
            obj.value.number = to_number(input[i]);
            obj.type = T_NUMBER;
            *(Object*)&bytecode[it] = obj;
            it += sizeof(Object);
        }
        if ((instruction = is_keyword(input[i]))) {
            if (it + 1 > size) {
                realloc_x(bytecode, (size += grow));
            }
            bytecode[it++] = instruction;
        }
    } while (++i < vm->lex_size);
    free(input);    /* won't need it anymore */
    return bytecode;
}

int UVM_exec(UVM_state* vm, char* str) {
    unsigned char* code = UVM_parse(vm, UVM_lex(vm, str));
    unsigned int ip = 0;
    while (1) {
        switch (code[ip++]) {
            case I_PUSH: {
                stack_push(*(Object*)&code[ip]);
                ip += sizeof(Object);
            }
                break;
                
            case I_POP:
                stack_pop();
                break;
                
            case I_GOTO: {
                Object obj = (*(Object*)&code[ip]);
                if (obj.type == T_NUMBER) {
                    ip = obj.value.number + 1;
                }
            }
                break;
                
            /*
            ** x => stack top
            ** x ? a, b
            **      ^ if true goto a
            **         ^ if false goto b
            */
            case I_IF: {
                if (vm->top >= 1) {
                    const Object top = stack_get_top();
                    if (top.type == T_NUMBER) {
                        if (top.value.number != 0) {
                            ip = (*(Object*)&code[ip]).value.number + 1;
                            break;
                        }
                        ip = (*(Object*)&code[ip + sizeof(Object)]).value.number + 1;
                        break;
                    }
                }
            }
                break;
                
            case I_ADD:
                op_arith(+=);
                break;
                
            case I_SUB:
                op_arith(-=);
                break;
            
            case I_MUL:
                op_arith(*=);
                break;
            
            case I_DIV:
                op_arith(/=);
                break;
                
            case I_EXIT:
                UVM_print_top(vm);
                return 2;
                break;
            
            default:
                break;
        }
    }
    return 1;
}

int UVM_free(UVM_state* vm) {
    free(vm);
    return 1;
}

int main(int argc, const char * argv[]) {
    UVM_state* vm = alloc_new(UVM_state);
    UVM_init(vm);
    UVM_exec(vm,
             "push 3;"
             "push 3;"
             "mul;"
             "exit;"
             );
    UVM_free(vm);
    return 0;
}
