#include "parse_compoundlist.h"

static void my_skip_token2(token_t **token)
{
    my_freetoken(token);
    *token = send_token();
    *token = my_skip_space(token);
}

void compoundlist_2(struct ast *ast_compd, token_t **token,
                    comp_context_t *context, struct ast *node)
{
    while ((*token)->reserved_word == 0 || context->do_comp
           || context->in_brackets)
    {
        node = parse_and_or(token);
        if (node != NULL)
            ast_compd->children =
                realloc(ast_compd->children,
                        (ast_compd->nb_children + 1) * sizeof(struct ast));

        ast_compd->children[ast_compd->nb_children] = node;
        if (node != NULL)
            ast_compd->nb_children++;
        if ((*token)->semicolon)
        {
            my_skip_token2(token);
        }
        if ((*token)->value[0] == '}')
        {
            break;
        }
        if ((*token)->type_token == DONE && context->in_brackets == 0)
        {
            break;
        }
        if ((*token)->type_token == NEWLINE || (*token)->newline)
        {
            my_skip_token2(token);
        }
        while ((*token)->type_token == NEWLINE)
        {
            my_skip_token2(token);
        }
        if ((*token)->type_token == DONE || (*token)->value[0] == '}'
            || (*token)->type_token == ENDOFFILE)
        {
            break;
        }
    }
}

struct ast *init_ast_compoundlist(token_t **token)
{
    comp_context_t *context = malloc(sizeof(comp_context_t));
    context->do_comp = 0;
    context->in_brackets = 0;
    if ((*token)->type_token == DO)
    {
        context->do_comp = 1;
        my_skip_token2(token);
        while (strcmp((*token)->value, "") == 0)
            my_skip_token2(token);
    }
    if (strcmp((*token)->value, "{") == 0)
    {
        context->in_brackets = 1;
        my_skip_token2(token);
    }
    while ((*token)->type_token == NEWLINE)
    {
        my_skip_token2(token);
    }
    struct ast *ast_compd = calloc(1, sizeof(struct ast));
    ast_compd->type = AST_COMPOUND;
    ast_compd->data.ast_compound = calloc(1, sizeof(struct treeCompoundlist));
    ast_compd->children = calloc(1, sizeof(struct ast *));
    struct ast *node = parse_and_or(token);
    ast_compd->children[ast_compd->nb_children] = node;
    if (node != NULL)
        ast_compd->nb_children++;
    if (strcmp((*token)->value, "}") == 0)
    {
        free(context);
        return ast_compd;
    }
    if ((*token)->semicolon || (*token)->newline)
    {
        my_skip_token2(token);
    }
    while ((*token)->type_token == NEWLINE)
    {
        my_skip_token2(token);
    }
    if ((*token)->type_token == DONE)
    {
        context->do_comp = 0;
    }
    if (strcmp((*token)->value, "}") == 0)
    {
        free(context);
        return ast_compd;
    }
    compoundlist_2(ast_compd, token, context, node);
    free(context);
    return ast_compd;
}
