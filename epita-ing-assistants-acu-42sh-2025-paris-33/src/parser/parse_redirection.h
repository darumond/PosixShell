#ifndef PARSE_REDIRECTION_H
#define PARSE_REDIRECTION_H

#include "../io_backend/io_backend.h"
#include "../lexer/lexer.h"
#include "../tools/tools.h"
#include "../tools/utils.h"
#include "ast.h"

token_t **handle_redir(struct ast *tree_simple_command, token_t **token,
                       int len);

#endif /* ! PARSE_REDIRECTION_H */
