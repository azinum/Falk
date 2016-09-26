/* falk.c */
/* author: Azinum */
/* date: 30/08/16 */


#include "falk.h"
#include "table.h"

void falk_instance_init(Falk_instance* F) {
    F->parse_instance = new(Parse_instance);
    parse_instance_init(F->parse_instance);
    F->parse_instance->lex_instance = new(Lex_instance);
    lex_instance_init(F->parse_instance->lex_instance);
    F->vm_instance = new(VM_instance);
    VM_init(F->vm_instance);
}


void falk_execute(Falk_instance* F) {
    if (F->argc == 2) {     /* open file */
        char* input = read_file(F->argv[1]);
        if (input != NULL)
            parse(F->parse_instance, input);
    } else {
        char* input = NULL;
        unsigned long size = 0;
        
        while (1) {
            printf(FALK_PROMPT);
            if (getline(&input, &size, stdin) > -1) {
                /* parse(F->parse_instance, input); */
                if (!VM_execute(F->vm_instance, input))
                    break;
            } else
                break;
        }
    }
    
    falk_instance_free(F);
}

char* read_file(const char* fname) {
    char* buffer = NULL;
    long ssize, rsize;   /* string, read size*/
    FILE* f = fopen(fname, "r");
    
    if (!f) {
        printf("Could not open file \"%s\".\n", fname);
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    
    ssize = ftell(f);
    
    rewind(f);
    
    buffer = newx(char, ssize + 1);
    
    rsize = fread(buffer, sizeof(char), ssize, f);
    
    buffer[ssize] = '\0';
    
    if (ssize != rsize) {
        free(buffer);
        buffer = NULL;
    }
    
    fclose(f);

    
    return buffer;
}

void falk_instance_free(Falk_instance* F) {
    parse_instance_free(F->parse_instance);
    lex_instance_free(F->parse_instance->lex_instance);
    VM_instance_free(F->vm_instance);
    free(F);
}

