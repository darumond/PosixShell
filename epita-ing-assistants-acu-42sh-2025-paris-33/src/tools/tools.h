#ifndef TOOLS_H
#define TOOLS_H

#include <err.h>
#include <string.h>
#include <unistd.h>

#include "parser/ast.h"
#include "parser/parse_simple_command.h"

typedef struct expand_context
{
    struct treeSimpleCommand *cmd;
    char **tmp;
    int single_quote_on;
    int double_quote_on;
    int x;
    int y;
    char **new_tab_cmd;
    int inside_arobase;
    int i_arg;
    char *str_copy;
    char *str;
    bool breake;
} expand_context_t;

char *my_strcpy(char *dst, char *src);
bool my_is_digit(char *str);
int len_arg(char **str);
void check_allocate(void *ptr);
char *my_strcpy_simple_command(char *dst, char *src, struct ast *my_ast);
char *int_to_str(int i);
char **init_table(void);
void copy_matrix(char **dst, char **src);
void free_table(char **list);

#endif /* ! TOOLS_H */
