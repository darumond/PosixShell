#ifndef EXEC_VARIABLE_H
#define EXEC_VARIABLE_H

#include <ctype.h>
#include <err.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../parser/ast.h"
#include "../tools/tools.h"
#include "hash_map.h"

// variable
bool is_variable(struct ast *ast);
int run_variable(struct ast *ast);

#endif /* ! EXEC_VARIABLE_H */
