#include "parse_until.h"

struct ast *init_ast_until(void)
{
    // Here we allocate memory for the AST node
    struct ast *node_until = calloc(1, sizeof(struct ast));
    check_allocate(node_until);

    node_until->type = AST_UNTIL;
    // Here we allocate memory for the data in the AST node
    node_until->data.ast_until = calloc(1, sizeof(struct treeUntil));
    check_allocate(node_until->data.ast_until);
    node_until->children = calloc(2, sizeof(struct ast));
    check_allocate(node_until->children);
    return node_until;
}

struct ast *rule_until(token_t **token)
{
    if ((*token)->type_token == UNTIL)
    {
        // We create here the If node
        struct ast *ast_until = init_ast_until();
        fill_node_until(ast_until, token);
        if (((*token)->type_token == DONE && (*token)->newline)
            || ((*token)->type_token == DONE && (*token)->semicolon))
        {
            return ast_until;
        }
        if ((*token)->type_token == DONE)
        {
            my_freetoken(token);
            *token = send_token();
        }
        return ast_until;
    }
    return NULL;
}

void fill_node_until(struct ast *node, token_t **token)
{
    // condition
    my_freetoken(token);
    *token = send_token();
    node->data.ast_until->condition = init_ast_compoundlist(token);
    node->children[node->nb_children] = node->data.ast_until->condition;
    node->nb_children++;

    // body then
    if ((*token)->type_token == DO)
    {
        node->data.ast_until->tree_do = init_ast_compoundlist(token);
        node->children[node->nb_children] = node->data.ast_until->tree_do;
        node->nb_children++;
    }
    else
    {
        args->exit_status = 2;
        errx(2, "unexpected token");
    }
}
