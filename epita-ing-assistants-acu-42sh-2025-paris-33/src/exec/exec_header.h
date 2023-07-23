#ifndef EXEC_HEADER_H
#define EXEC_HEADER_H

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "../parser/ast.h"
#include "../tools/utils.h"

enum pipe
{
    LEFT,
    RIGHT
};
// fonctions

pid_t exec_fork(struct ast *node, int fds[2], enum pipe side, struct ast *root);
int ast_exec(struct ast *ast, struct ast *root);
int run_compound_list(struct ast *ast, struct ast *root);

#endif /* ! EXEC_HEADER_H */
