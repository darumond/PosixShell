#include "parse_list.h"

struct ast *init_ast_list(struct ast *node)
{
    node->type = AST_LIST;
    node->data.ast_list = calloc(1, sizeof(struct treeList));
    check_allocate(node->data.ast_list);
    node->children = calloc(1, sizeof(struct ast *));
    check_allocate(node->children);

    return node;
}

struct ast *parse_list(struct ast *node, token_t **token)
{
    if ((*token)->type_token == ENDOFFILE)
    {
        return NULL;
    }
    node = init_ast_list(node);
    node->children[node->nb_children] = parse_and_or(token);
    if (node->children[node->nb_children] != NULL)
        node->nb_children++;

    while ((*token)->type_token != ENDOFFILE && (*token)->newline == 0
           && (*token)->type_token != NEWLINE && !(*token)->semicolon)
    {
        node->children = realloc(
            node->children, (node->nb_children + 1) * sizeof(struct ast *));
        node->children[node->nb_children] = parse_and_or(token);

        if (node->children[node->nb_children] != NULL)
            node->nb_children++;
    }
    return node;
}
