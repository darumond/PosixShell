#ifndef EXEC_FUNCTION_H
#define EXEC_FUNCTION_H

#include <ctype.h>
#include <err.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../builtin/builtin.h"
#include "../parser/ast.h"
#include "../parser/parse_compoundlist.h"
#include "../parser/parse_for.h"
#include "../parser/parse_func.h"
#include "../parser/parse_if.h"
#include "../parser/parse_until.h"
#include "../parser/parse_while.h"
#include "../tools/tools.h"
#include "exec_header.h"
#include "hash_map_func.h"

void run_func(struct ast *node);

#endif /* ! EXEC_FUNCTION_H */
