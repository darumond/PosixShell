#ifndef EXPAND_BASIC_H
#define EXPAND_BASIC_H

#include <string.h>
#include <unistd.h>

#include "../io_backend/io_backend.h"
#include "../parser/ast.h"
#include "../tools/tools.h"

int handle_quote_on(expand_context_t *context, int i, int j);
int handle_slash(expand_context_t *context, int i, int j, bool *continu);
int handle_slash_dollar_quote_newline(expand_context_t *context, int i, int j,
                                      bool *continu);

#endif /* ! EXPAND_BASIC_H */
