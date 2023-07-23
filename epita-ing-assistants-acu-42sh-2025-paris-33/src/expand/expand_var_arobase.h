#ifndef EXPAND_VAR_AROBASE_H
#define EXPAND_VAR_AROBASE_H

#include <string.h>
#include <unistd.h>

#include "../io_backend/io_backend.h"
#include "../parser/ast.h"
#include "../tools/tools.h"

int handle_var_arobase_mid(expand_context_t *context, int i);
int handle_var_arobase_arg_size_1(expand_context_t *context, int i, int j,
                                  int j_arg);
int handle_var_arobase_arg_size_0(expand_context_t *context, int i, int j,
                                  int res);
int handle_var_arobase_arg_size_other(expand_context_t *context, int i, int j,
                                      int j_arg);
int handle_var_arobase(expand_context_t *context, int i, int j);

#endif /* ! EXPAND_VAR_AROBASE_H */
