#include "parse_pipeline.h"

struct ast *init_ast_pipe(void)
{
    // Here we allocate memory for the AST node
    struct ast *node_pipe = calloc(1, sizeof(struct ast));
    check_allocate(node_pipe);

    node_pipe->type = AST_PIPE;
    // Here we allocate memory for the data in the AST node
    node_pipe->data.ast_pipe = calloc(1, sizeof(struct treePipe));
    check_allocate(node_pipe->data.ast_pipe);
    node_pipe->children = calloc(2, sizeof(struct ast));
    check_allocate(node_pipe->children);
    return node_pipe;
}

struct ast *return_node1(struct ast *node_negation, struct ast *node1)
{
    node_negation->data.ast_negation->my_tree = node1;
    node_negation->nb_children++;
    node_negation->children[0] = node1;
    return node_negation;
}

struct ast *return_pipe(struct ast *node_negation, struct ast *ast_pipe)
{
    node_negation->data.ast_negation->my_tree = ast_pipe;
    node_negation->nb_children++;
    node_negation->children[0] = ast_pipe;
    return node_negation;
}

struct ast *parse_pipeline(token_t **token)
{
    struct ast *node_negation = NULL;
    if ((*token)->type_token == NEGATION)
    {
        node_negation = init_ast_negation();
        my_freetoken(token);
        *token = send_token();
    }
    struct ast *node1 = parse_command(token);
    struct ast *last_pipe_node = NULL;
    int exist_pipe = 0;
    struct ast *ast_pipe = NULL;
    while ((*token)->type_token == PIPE)
    {
        exist_pipe = 1;
        ast_pipe = init_ast_pipe(); // 2 FILS MAX
        if (last_pipe_node != NULL)
        {
            ast_pipe->data.ast_pipe->commande_left = last_pipe_node;
            ast_pipe->children[0] = last_pipe_node;
            ast_pipe->nb_children++;
        }
        else
        {
            ast_pipe->data.ast_pipe->commande_left = node1;
            ast_pipe->children[0] = node1;
            ast_pipe->nb_children++;
        }
        // mettre le node command dans pipe
        my_freetoken(token);
        *token = send_token();
        while ((*token)->type_token == NEWLINE)
        {
            my_freetoken(token);
            *(token) = send_token();
        }
        struct ast *node2 = parse_command(token);
        ast_pipe->data.ast_pipe->commande_right = node2;
        ast_pipe->children[1] = node2;
        ast_pipe->nb_children++;
        last_pipe_node = ast_pipe;
    }
    if (exist_pipe == 0 && node_negation == NULL)
        return node1;
    if (exist_pipe == 0 && node_negation != NULL)
    {
        return return_node1(node_negation, node1);
    }
    if (node_negation != NULL)
    {
        return return_pipe(node_negation, ast_pipe);
    }
    return ast_pipe;
}
