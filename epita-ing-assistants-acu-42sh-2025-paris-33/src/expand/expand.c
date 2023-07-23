#include "expand.h"

#include <stdlib.h>
#include <string.h>

expand_context_t *init_expand_context(struct treeSimpleCommand *cmd)
{
    expand_context_t *context = calloc(1, sizeof(expand_context_t));
    context->cmd = cmd;
    context->new_tab_cmd = init_tab(100);
    context->tmp = context->cmd->argument;
    return context;
}

void expand_fill_new_tab(expand_context_t *context)
{
    if (context->inside_arobase == 0)
    {
        context->new_tab_cmd[context->i_arg] =
            my_strcpy(context->new_tab_cmd[context->i_arg], context->str_copy);
        free(context->str_copy);
        free(context->str);
        context->i_arg++;
    }
    else
    {
        if (args->size_arg >= 1)
        {
            char *argvlast =
                strcat(args->argument[args->size_arg - 1], context->str_copy);
            context->new_tab_cmd[context->i_arg] =
                my_strcpy(context->new_tab_cmd[context->i_arg], argvlast);
            context->i_arg++;
        }
        free(context->str_copy);
        free(context->str);
        context->inside_arobase = 0;
    }
}

int expand_variable(expand_context_t *context, int i, int j, bool *continu)
{
    j = handle_var_oldpwd(context, i, j);

    j = handle_var_star(context, i, j);

    j = handle_var_interrogation(context, i, j);

    if (handle_var_arobase_mid(context, i) == 1)
    {
        context->breake = true;
        return j;
    }

    if (handle_var_arobase(context, i, j) == 1)
    {
        context->breake = true;
        return j;
    }

    j = handle_var_diez(context, i, j);

    j = handle_var_dollar(context, i, j);

    j = handle_var_uid(context, i, j);

    j = handle_var_argument(context, i, j, continu);
    if (*continu == true)
    {
        *continu = true;
        return j;
    }

    j = handle_var_basic(context, i, j, continu);
    if (*continu == true)
    {
        return j;
    }
    return j;
}

int expand_basic(expand_context_t *context, int i, int j, bool *continu)
{
    if (handle_quote_on(context, i, j) == 1)
    {
        *continu = true;
        return j;
    }

    j = handle_slash(context, i, j, continu);
    if (*continu == true)
    {
        return j;
    }

    j = handle_slash_dollar_quote_newline(context, i, j, continu);
    if (*continu == true)
    {
        return j;
    }
    return j;
}

struct treeSimpleCommand *expand(struct treeSimpleCommand *cmd, bool *is_func)
{
    if (hash_map_is_present_func(args->function, cmd->argument[0]))
    {
        *is_func = true;
        args->key_func = cmd->argument[0];
        return cmd;
    }
    expand_context_t *context = init_expand_context(cmd);

    for (int i = 0; context->cmd->argument[i]
         && strcmp(context->cmd->argument[i], "") != 0;
         i++)
    {
        context->str_copy = calloc(10000, sizeof(char));
        context->str = calloc(100, sizeof(char));
        context->x = 0;
        context->y = 0;

        for (int j = 0; context->cmd->argument[i][j] != '\0'; j++)
        {
            bool continu = false;
            context->breake = false;
            j = expand_variable(context, i, j, &continu);
            if (context->breake == true)
                break;
            if (continu == true)
                continue;

            continu = false; // On reset continu;

            // expand quote, slash, dollar
            j = expand_basic(context, i, j, &continu);
            if (continu == true)
                continue;
        }
        if (strcmp(context->str_copy, "") != 0)
            expand_fill_new_tab(context);
        else
        {
            free(context->str_copy);
            free(context->str);
        }
    }

    context->cmd->argument = context->new_tab_cmd;
    cmd->argument = context->cmd->argument;
    my_free_tab(context->tmp, 100);
    free(context);
    return cmd;
}
