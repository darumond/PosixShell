#ifndef IO_BACKEND_H
#define IO_BACKEND_H
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../exec/hash_map.h"
#include "../exec/hash_map_func.h"

typedef struct args
{
    char **argument;
    int size_arg;
    hash_map_t *variable;
    int exit_status;
    FILE *fp;
    char *save_token;
    hash_map_func_t *function;
    char **argv;
    int argc;
    struct ast **node_list_func;
    int reference_counting;
    char *key_func;
    int mul_function;

} args_t;

extern args_t *args;

// fonctions
void file_convert(int argc, char *argv[]);
char get_new_char(void);
char unget_new_char(char c);
void close_file(void);

#endif /* ! IO_BACKEND_H */
