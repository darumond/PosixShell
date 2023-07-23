#ifndef PARSE_SIMPLE_COMMAND_H
#define PARSE_SIMPLE_COMMAND_H

#include "../exec/exec_redirection.h"
#include "../tools/tools.h"
#include "ast.h"
#include "lexer/lexer.h"
#include "parse_redirection.h"

int fill_simple_command(struct ast *tree_simple_command, token_t **token);
struct ast *parse_simple_command(token_t **token);
bool is_newline_semicolon(token_t **token);
bool is_pipe_or_and(token_t **token);
bool is_quote_word(token_t **token, int is_echo);
int handle_token_redir_newline(token_t **token,
                               struct ast *tree_simple_command);
// void add_token(struct ast *tree_simple_command, token_t **token, int i);
int handle_redir_token(token_t **token, struct ast *tree_simple_command);

#endif /* ! PARSE_SIMPLE_COMMAND_H */
