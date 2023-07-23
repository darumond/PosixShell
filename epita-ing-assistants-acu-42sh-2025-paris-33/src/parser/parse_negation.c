#include "parse_negation.h"

#include "../tools/tools.h"
#include "../tools/utils.h"

struct ast *init_ast_negation(void)
{
    // Here we allocate memory for the AST node
    struct ast *node_negation = calloc(1, sizeof(struct ast));
    check_allocate(node_negation);

    node_negation->type = AST_NEGATION;
    // Here we allocate memory for the data in the AST node
    node_negation->data.ast_negation = calloc(1, sizeof(struct treeNegation));
    check_allocate(node_negation->data.ast_negation);
    node_negation->children = calloc(1, sizeof(struct ast));
    check_allocate(node_negation->children);
    return node_negation;
}
