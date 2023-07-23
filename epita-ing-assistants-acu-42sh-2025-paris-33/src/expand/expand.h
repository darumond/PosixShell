#ifndef EXPAND_H
#define EXPAND_H

#include <string.h>
#include <unistd.h>

#include "../io_backend/io_backend.h"
#include "../parser/ast.h"
#include "../tools/tools.h"
#include "expand_basic.h"
#include "expand_var.h"
#include "expand_var_arobase.h"

struct treeSimpleCommand *expand(struct treeSimpleCommand *cmd, bool *is_func);
int expand_basic(expand_context_t *context, int i, int j, bool *continu);
int expand_variable(expand_context_t *context, int i, int j, bool *continu);

#endif /* EXPAND_H ! */
