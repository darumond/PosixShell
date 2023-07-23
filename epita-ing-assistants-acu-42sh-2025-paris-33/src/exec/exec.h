#ifndef EXEC_H
#define EXEC_H
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "../parser/ast.h"
#include "../tools/utils.h"
#include "exec_redirection.h"
#include "exec_variable.h"

// fonctions
int run_if(struct ast *ast, struct ast *root);
int run_sim_cmd(struct ast *ast, struct ast *root);
int right_arrow(struct treeSimpleCommand *simple_cmd);
int left_arrow(struct treeSimpleCommand *simple_cmd);

#endif /* ! EXEC_H */
