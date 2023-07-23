#ifndef PARSE_LIST_H
#define PARSE_LIST_H

#include "../lexer/lexer.h"
#include "../tools/tools.h"
#include "../tools/utils.h"
#include "ast.h"
#include "parse_and_or.h"

struct ast *init_ast_list(struct ast *node);
struct ast *parse_list(struct ast *node, token_t **token);

#endif /* ! PARSE_LIST_H */
