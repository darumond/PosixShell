#include "parse_func.h"

struct ast *init_ast_func(void)
{
    // Here we allocate memory func the AST node
    struct ast *node_func = calloc(1, sizeof(struct ast));
    check_allocate(node_func);

    node_func->type = AST_FUNC;
    // Here we allocate memory func the data in the AST node
    node_func->data.ast_func = calloc(1, sizeof(struct treeFunc));
    check_allocate(node_func->data.ast_func);
    node_func->children = calloc(1, sizeof(struct ast));
    check_allocate(node_func->children);
    return node_func;
}

void fill_node_func(struct ast *node, token_t **token)
{
    // condition
    struct ast *shell_command;
    shell_command = parse_shell_command(token);
    node->children[node->nb_children] = shell_command;
    node->nb_children++;
}

struct ast *rule_func(token_t **token)
{
    if ((*token)->type_token == PARENTHESIS)
    {
        struct ast *ast_func = init_ast_func();
        ast_func->data.ast_func->name = args->save_token;
        my_freetoken(token);
        *token = send_token();
        if ((*token)->type_token == PARENTHESIS)
        {
            args->mul_function++;
            my_freetoken(token);
            *token = send_token();
            while ((*token)->type_token == NEWLINE)
            {
                my_freetoken(token);
                *token = send_token();
            }
            fill_node_func(ast_func, token);
            if (args->mul_function > 1)
            {
                my_freetoken(token);
                *token = send_token();
                args->mul_function = 0;
            }

            return ast_func;
        }
        errx(2, "erreur");
    }
    return NULL;
}
