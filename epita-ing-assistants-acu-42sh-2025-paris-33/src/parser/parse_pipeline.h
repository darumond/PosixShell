#ifndef PARSE_PIPELINE_H
#define PARSE_PIPELINE_H

#include "../lexer/lexer.h"
#include "../tools/tools.h"
#include "../tools/utils.h"
#include "ast.h"

struct ast *init_ast_pipe(void);
struct ast *parse_pipeline(token_t **token);
struct ast *return_node1(struct ast *node_negation, struct ast *node1);
struct ast *return_pipe(struct ast *node_negation, struct ast *ast_pipe);

#endif /* ! PARSE_PIPELINE_H */
