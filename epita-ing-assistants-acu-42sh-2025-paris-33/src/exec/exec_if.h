#ifndef EXEC_IF_H
#define EXEC_IF_H

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

int run_if(struct ast *ast, struct ast *root);
int run_compound_list(struct ast *ast, struct ast *root);

#endif /* ! EXEC_IF_H */
