#ifndef PARSE_WHILE_H
#define PARSE_WHILE_H

#include "../io_backend/io_backend.h"
#include "../lexer/lexer.h"
#include "../tools/tools.h"
#include "../tools/utils.h"
#include "ast.h"
#include "parse_compoundlist.h"

struct ast *init_ast_while(void);
struct ast *rule_while(token_t **token);
void fill_node_while(struct ast *node, token_t **token);

#endif /* ! PARSE_WHILE_H */
