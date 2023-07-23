#ifndef PARSE_IF_H
#define PARSE_IF_H

#include "../lexer/lexer.h"
#include "../tools/tools.h"
#include "../tools/utils.h"
#include "ast.h"
#include "parse_compoundlist.h"

struct ast *init_ast_if(void);
struct ast *rule_if(token_t **token);
void fill_node_if(struct ast *node, token_t **token);

#endif /* ! PARSE_IF_H */
