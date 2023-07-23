#ifndef EXPAND_VAR_H
#define EXPAND_VAR_H

#include <string.h>
#include <unistd.h>

#include "../io_backend/io_backend.h"
#include "../parser/ast.h"
#include "../tools/tools.h"
#include "expand.h"

int handle_var_oldpwd(expand_context_t *context, int i, int j);
int handle_var_argument(expand_context_t *context, int i, int j, bool *continu);
int handle_var_uid(expand_context_t *context, int i, int j);
int handle_var_dollar(expand_context_t *context, int i, int j);
int handle_var_diez(expand_context_t *context, int i, int j);
int handle_var_interrogation(expand_context_t *context, int i, int j);
int handle_var_star(expand_context_t *context, int i, int j);
int handle_var_basic_braquet(expand_context_t *context, int i, int j,
                             bool *continu);
int handle_var_basic(expand_context_t *context, int i, int j, bool *continu);

#endif /* ! EXPAND_VAR_H */
