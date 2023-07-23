#ifndef EXEC_AND_OR_H
#define EXEC_AND_OR_H

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

int run_and(struct ast *ast, struct ast *root);
int run_or(struct ast *ast, struct ast *root);

#endif /* ! EXEC_AND_OR_H */
