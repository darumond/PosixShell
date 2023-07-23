#ifndef PARSE_UNTIL_H
#define PARSE_UNTIL_H

#include "../io_backend/io_backend.h"
#include "../lexer/lexer.h"
#include "../tools/tools.h"
#include "../tools/utils.h"
#include "ast.h"
#include "parse_compoundlist.h"

struct ast *init_ast_until(void);
struct ast *rule_until(token_t **token);
void fill_node_until(struct ast *node, token_t **token);

#endif /* ! PARSE_UNTIL_H */
