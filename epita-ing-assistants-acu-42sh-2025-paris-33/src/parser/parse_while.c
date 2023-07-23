#include "parse_while.h"

struct ast *init_ast_while(void)
{
    // Here we allocate memory for the AST node
    struct ast *node_while = calloc(1, sizeof(struct ast));
    check_allocate(node_while);

    node_while->type = AST_WHILE;
    // Here we allocate memory for the data in the AST node
    node_while->data.ast_while = calloc(1, sizeof(struct treeWhile));
    check_allocate(node_while->data.ast_while);
    node_while->children = calloc(2, sizeof(struct ast));
    check_allocate(node_while->children);
    return node_while;
}

struct ast *rule_while(token_t **token)
{
    if ((*token)->type_token == WHILE)
    {
        // We create here the If node
        struct ast *ast_while = init_ast_while();
        fill_node_while(ast_while, token);
        if (((*token)->type_token == DONE && (*token)->newline)
            || ((*token)->type_token == DONE && (*token)->semicolon))
        {
            return ast_while;
        }
        if ((*token)->type_token == DONE)
        {
            my_freetoken(token);
            *token = send_token();
        }
        return ast_while;
    }
    return NULL;
}

void fill_node_while(struct ast *node, token_t **token)
{
    // condition
    my_freetoken(token);
    *token = send_token();
    node->data.ast_while->condition = init_ast_compoundlist(token);
    node->children[node->nb_children] = node->data.ast_while->condition;
    node->nb_children++;

    // body then
    if ((*token)->type_token == DO)
    {
        node->data.ast_while->tree_do = init_ast_compoundlist(token);
        node->children[node->nb_children] = node->data.ast_while->tree_do;
        node->nb_children++;
    }
    else
    {
        args->exit_status = 2;
        errx(2, "EXPECTED TOKEN DO");
    }
}
