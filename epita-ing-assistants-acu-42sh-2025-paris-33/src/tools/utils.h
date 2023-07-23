#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer/lexer.h"
#include "parser/ast.h"

// fonctions
void my_freetoken(token_t **token);
void free_ast(struct ast *ast);
void my_free_var(struct treeSimpleCommand *tree_simple_command);
token_t *my_skip_space(token_t **token);
void my_free_tab(char **str, int size);
char **init_tab(size_t size);

#endif /* ! UTILS_H */
