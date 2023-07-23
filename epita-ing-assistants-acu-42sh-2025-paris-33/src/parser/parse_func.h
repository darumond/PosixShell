#ifndef PARSE_FUNC_H
#define PARSE_FUNC_H

#include "../io_backend/io_backend.h"
#include "../lexer/lexer.h"
#include "../tools/tools.h"
#include "../tools/utils.h"
#include "ast.h"
#include "parse_compoundlist.h"

struct ast *init_ast_func(void);
struct ast *rule_func(token_t **token);
void fill_node_func(struct ast *node, token_t **token);

#endif /* ! PARSE_FUNC_H */
