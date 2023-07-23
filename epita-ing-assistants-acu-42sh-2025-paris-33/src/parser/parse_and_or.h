#ifndef PARSE_AND_OR_H
#define PARSE_AND_OR_H

#include "../io_backend/io_backend.h"
#include "../lexer/lexer.h"
#include "../tools/tools.h"
#include "../tools/utils.h"
#include "ast.h"
#include "parse_pipeline.h"

struct ast *init_ast_and_or(token_t **token);
struct ast *parse_and_or(token_t **token);

#endif /* ! PARSE_AND_OR_H */
