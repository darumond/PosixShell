#ifndef EXEC_REDIRECTION_H
#define EXEC_REDIRECTION_H

#include <errno.h>
#include <fcntl.h>

#include "../tools/tools.h"
#include "exec.h"
#include "exec_redirection.h"

typedef struct redirection_context
{
    char **redir;
    int *tab_fdup;
    int *tab_io_number;
    int fd_dup;
    int io_number;
    int fd_file;
    int new_stdout;
    int exit_status;
    int j;
} redirection_context_t;

bool is_redirection(struct ast *ast);
bool is_operation(char *str);
int run_redirection(struct ast *ast, struct ast *root);

#endif /* ! EXEC_REDIRECTION_H */
