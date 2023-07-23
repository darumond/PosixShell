#ifndef PARSE_FOR_H
#define PARSE_FOR_H

#include "../io_backend/io_backend.h"
#include "../lexer/lexer.h"
#include "../tools/tools.h"
#include "../tools/utils.h"
#include "ast.h"
#include "parse_compoundlist.h"

struct ast *init_ast_for(void);
struct ast *rule_for(token_t **token);
void fill_node_for(struct ast *node, token_t **token);

#endif /* ! PARSE_FOR_H */
