#ifndef PARSE_COMPOUNDLIST_H
#define PARSE_COMPOUNDLIST_H

#include "../lexer/lexer.h"
#include "../tools/utils.h"
#include "ast.h"
#include "parse_and_or.h"

typedef struct comp_context
{
    int do_comp;
    int in_brackets;
} comp_context_t;

struct ast *init_ast_compoundlist(token_t **token);

#endif /* ! PARSE_COMPOUNDLIST_H */
