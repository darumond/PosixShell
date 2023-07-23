#include "parse_and_or.h"

struct ast *init_ast_and_or(token_t **token)
{
    // Here we allocate memory for the AST node
    struct ast *node_and_or = calloc(1, sizeof(struct ast));
    check_allocate(node_and_or);
    if ((*token)->type_token == AND)
    {
        node_and_or->type = AST_AND;
    }
    if ((*token)->type_token == OR)
    {
        node_and_or->type = AST_OR;
    }
    // Here we allocate memory for the data in the AST node
    node_and_or->data.ast_pipe = calloc(1, sizeof(struct treePipe));
    check_allocate(node_and_or->data.ast_pipe);
    node_and_or->children = calloc(2, sizeof(struct ast));
    check_allocate(node_and_or->children);
    return node_and_or;
}

struct ast *parse_and_or(token_t **token)
{
    struct ast *node = parse_pipeline(token);
    struct ast *last_node = NULL;
    int exist_and_or = 0;
    struct ast *node_and_or = NULL;
    while ((*token)->type_token == AND || (*token)->type_token == OR)
    {
        exist_and_or = 1;
        node_and_or = init_ast_and_or(token);
        if (last_node != NULL)
        {
            node_and_or->data.ast_and_or->commande_left = last_node;
            node_and_or->children[0] = last_node;
            node_and_or->nb_children++;
        }
        else
        {
            node_and_or->data.ast_and_or->commande_left = node;
            node_and_or->children[0] = node;
            node_and_or->nb_children++;
        }
        // mettre le node command dans pipe
        my_freetoken(token);
        *token = send_token();
        while ((*token)->type_token == NEWLINE)
        {
            my_freetoken(token);
            *(token) = send_token();
        }
        if ((*token)->semicolon)
        {
            args->exit_status = 2;
            errx(2, "Unexpected semicolon");
        }
        struct ast *node2 = parse_pipeline(token);
        node_and_or->data.ast_and_or->commande_right = node2;
        node_and_or->children[1] = node2;
        node_and_or->nb_children++;
        last_node = node_and_or;
    }
    if (!exist_and_or)
    {
        return node;
    }
    else
    {
        return node_and_or;
    }
}
