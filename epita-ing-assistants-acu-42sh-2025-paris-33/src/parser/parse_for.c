#include "parse_for.h"

struct ast *init_ast_for(void)
{
    // Here we allocate memory for the AST node
    struct ast *node_for = calloc(1, sizeof(struct ast));
    check_allocate(node_for);

    node_for->type = AST_FOR;
    // Here we allocate memory for the data in the AST node
    node_for->data.ast_for = calloc(1, sizeof(struct treeFor));
    check_allocate(node_for->data.ast_for);
    node_for->data.ast_for->variable = calloc(100, sizeof(char));
    node_for->data.ast_for->word_list = calloc(100, sizeof(char *));
    for (size_t i = 0; i < 100; i++)
    {
        node_for->data.ast_for->word_list[i] = calloc(100, sizeof(char));
        check_allocate(node_for->data.ast_for->word_list[i]);
    }
    node_for->children = calloc(3, sizeof(struct ast));
    check_allocate(node_for->children);

    return node_for;
}

struct ast *rule_for(token_t **token)
{
    if ((*token)->type_token == FOR)
    {
        // We create here the If node
        struct ast *ast_for = init_ast_for();
        fill_node_for(ast_for, token);
        if (((*token)->type_token == DONE && (*token)->newline)
            || ((*token)->type_token == DONE && (*token)->semicolon))
        {
            return ast_for;
        }
        if ((*token)->type_token == DONE)
        {
            my_freetoken(token);
            *token = send_token();
        }
        return ast_for;
    }
    return NULL;
}

void skip_token2(token_t **token)
{
    my_freetoken(token);
    *token = send_token();
}

int handle_token_in(token_t **token, int seperator, int i, struct ast *node)
{
    if ((*token)->semicolon || (*token)->newline)
    {
        seperator = 1;
    }
    skip_token2(token);
    while ((*token)->type_token != DO && (*token)->type_token != ENDOFFILE)
    {
        my_strcpy(node->data.ast_for->word_list[i], (*token)->value);
        i++;
        skip_token2(token);
        if ((*token)->type_token == NEWLINE || (*token)->newline
            || (*token)->semicolon)
        {
            seperator = 1;
        }
    }
    return seperator;
}

void handle_token_do(token_t **token, struct ast *node)
{
    node->data.ast_for->tree_do = init_ast_compoundlist(token);
    node->children[node->nb_children] = node->data.ast_for->tree_do;
    node->nb_children++;
}

void fill_node_for(struct ast *node, token_t **token)
{
    // condition
    skip_token2(token);
    my_strcpy(node->data.ast_for->variable, (*token)->value);
    int seperator = 0;
    if ((*token)->newline || (*token)->semicolon)
    {
        seperator = 1;
    }
    skip_token2(token);
    int i = 0;
    while ((*token)->type_token == NEWLINE)
        skip_token2(token);
    if ((*token)->type_token == IN)
    {
        seperator = handle_token_in(token, seperator, i, node);
    }
    while (((*token)->type_token == NEWLINE || (*token)->newline)
           && (*token)->type_token != DO && (*token)->type_token != ENDOFFILE)
    {
        skip_token2(token);
        seperator = 1;
    }
    if (seperator == 0)
    {
        args->exit_status = 2;
        errx(2, "NO SEPERATOR");
    }

    // body then
    if ((*token)->type_token == DO)
    {
        handle_token_do(token, node);
    }
    else
    {
        args->exit_status = 2;
        errx(2, "NO DO");
    }
}
