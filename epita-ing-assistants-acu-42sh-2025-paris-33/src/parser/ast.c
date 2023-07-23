#include "ast.h"

#include "../exec/exec.h"
#include "../tools/utils.h"
#include "parse_for.h"
#include "parse_func.h"
#include "parse_if.h"
#include "parse_list.h"
#include "parse_simple_command.h"
#include "parse_until.h"
#include "parse_while.h"

int parse_element(token_t **token)
{
    if ((*token)->type_token == WORDS)
    {
        return 1;
    }
    return 0;
}

struct ast *parse_shell_command(token_t **token)
{
    struct ast *node;
    if (strcmp((*token)->value, "(") == 0)
    {
        return NULL;
    }
    if (strcmp((*token)->value, "{") == 0)
    {
        node = init_ast_compoundlist(token);
        if (strcmp((*token)->value, "}") == 0 && (*token)->newline)
        {
            return node;
        }
        if (strcmp((*token)->value, "}") == 0 && (*token)->semicolon)
        {
            return node;
        }
        if (strcmp((*token)->value, "}") == 0)
        {
            my_freetoken(token);
            *token = send_token();
        }
        return node;
    }

    node = rule_if(token);
    if (!node)
    {
        node = rule_while(token);
        if (!node)
        {
            node = rule_until(token);
            if (!node)
            {
                node = rule_for(token);
            }
        }
    }
    if (!node)
    {
        args->exit_status = 2;
        errx(2, "ERREUR SHELL COMMAND");
    }

    return node;
}

struct ast *parse_command(token_t **token)
{
    struct ast *node = parse_simple_command(token);

    // In the case if its not a simple command
    if (node == NULL)
    {
        node = parse_shell_command(token);
        if (node == NULL)
            node = rule_func(token);
        return node;
    }

    else
    {
        return node;
    }
}

struct ast *parse_input(void)
{
    token_t *token = send_token();
    while (token->type_token == NEWLINE)
    {
        my_freetoken(&token);
        token = send_token();
    }
    if (token->type_token == ENDOFFILE)
    {
        my_freetoken(&token);
        return NULL;
    }
    while (strcmp(token->value, "}") == 0)
    {
        my_freetoken(&token);
        token = send_token();
    }
    struct ast *root = calloc(1, sizeof(struct ast));
    root = parse_list(root, &token);
    my_freetoken(&token);
    return root;
}
