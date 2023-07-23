#include "parse_if.h"

void fill_node_if(struct ast *node, token_t **token)
{
    // condition
    my_freetoken(token);
    *token = send_token();
    node->data.ast_if->condition = init_ast_compoundlist(token);
    node->children[node->nb_children] = node->data.ast_if->condition;
    node->nb_children++;

    // body then
    if ((*token)->type_token == THEN)
    {
        my_freetoken(token);
        *token = send_token();
        node->data.ast_if->tree_then = init_ast_compoundlist(token);
        node->children[node->nb_children] = node->data.ast_if->tree_then;
        node->nb_children++;
    }

    // body else
    if ((*token)->type_token == ELSE)
    {
        my_freetoken(token);
        *token = send_token();
        node->data.ast_if->tree_else = init_ast_compoundlist(token);
        node->children[node->nb_children] = node->data.ast_if->tree_else;
        node->nb_children++;
    }

    else if ((*token)->type_token == ELIF)
    {
        node->data.ast_if->tree_else = rule_if(token);
        node->children[node->nb_children] = node->data.ast_if->tree_else;
        node->nb_children++;
    }
}

struct ast *rule_if(token_t **token)
{
    if ((*token)->type_token == IF || (*token)->type_token == ELIF)
    {
        // We create here the If node
        struct ast *ast_if = init_ast_if();
        fill_node_if(ast_if, token);
        if (((*token)->type_token == FI && (*token)->newline)
            || ((*token)->type_token == FI && (*token)->semicolon))
        {
            return ast_if;
        }
        if ((*token)->type_token == FI)
        {
            my_freetoken(token);
            *token = send_token();
        }
        return ast_if;
    }
    return NULL;
}

struct ast *init_ast_if(void)
{
    // Here we allocate memory for the AST node
    struct ast *node_if = calloc(1, sizeof(struct ast));
    check_allocate(node_if);

    node_if->type = AST_IF;
    // Here we allocate memory for the data in the AST node
    node_if->data.ast_if = calloc(1, sizeof(struct treeIf));
    check_allocate(node_if->data.ast_if);
    node_if->children = calloc(3, sizeof(struct ast));
    check_allocate(node_if->children);

    return node_if;
}
