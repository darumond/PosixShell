#ifndef EXEC_PIPE_H
#define EXEC_PIPE_H

#include <ctype.h>
#include <err.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../builtin/builtin.h"
#include "../parser/ast.h"
#include "../tools/tools.h"
#include "exec_header.h"

int exec_pipe(struct ast *left_son, struct ast *right_son, struct ast *root);
int run_pipe(struct ast *ast_pipe, struct ast *root);

#endif /* ! EXEC_PIPE_H */
