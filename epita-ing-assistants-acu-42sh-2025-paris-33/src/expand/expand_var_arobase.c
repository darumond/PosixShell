#include <stdlib.h>
#include <string.h>

#include "expand_var.h"

int handle_var_arobase_mid(expand_context_t *context, int i)
{
    int breake = 0;
    if (context->double_quote_on == 0 && context->single_quote_on == 0
        && strcmp(context->cmd->argument[i], "$@") == 0)
    {
        // we loop over the array args of arguments
        for (int j = 0; j < args->size_arg; j++)
        {
            context->new_tab_cmd[context->i_arg] = my_strcpy(
                context->new_tab_cmd[context->i_arg], args->argument[j]);
            context->i_arg++;
        }
        breake = 1;
        return breake;
    }
    return breake;
}

int handle_var_arobase_arg_size_1(expand_context_t *context, int i, int j,
                                  int j_arg)
{
    int breake = 0;
    context->inside_arobase = 1;

    if (args->size_arg == 1)
    {
        if (context->cmd->argument[i][0] != '$'
            || context->cmd->argument[i][1]
                != '@') // Si le $@ est collé à droite
        {
            context->inside_arobase = 0;
            if (context->cmd->argument[i][j]
                != '\0') // Si le $@ est collé au milieu
            {
                strcat(context->str_copy, args->argument[0]);
                context->x++;
                for (size_t count = 0;
                     count < strlen(context->cmd->argument[i]) - 2; count++)
                {
                    context->str_copy[context->x] =
                        context->cmd->argument[i][j];
                    j++;
                    context->x++;
                    count++;
                }
                breake = 1;
                return breake;
            }

            char *argv_tmp = strcat(context->str_copy, args->argument[j_arg]);
            j_arg++;
            context->str_copy = argv_tmp;
            breake = 1;
            return breake;
        }
        else // Si le $@ est collé à gauche
        {
            char *argv_tmp =
                strcat(args->argument[j_arg], context->cmd->argument[i] + 2);
            j_arg++;
            char *tmp = context->str_copy;
            context->str_copy = strndup(argv_tmp, strlen(argv_tmp));
            free(tmp);
            context->inside_arobase = 0;
            breake = 1;
            return breake;
        }
    }
    return breake;
}

int handle_var_arobase_arg_size_0(expand_context_t *context, int i, int j,
                                  int res)
{
    if (args->size_arg == 0)
    {
        if (context->cmd->argument[i][0] != '$'
            || context->cmd->argument[i][1]
                != '@') // Si le $@ est collé à droite
        {
            context->inside_arobase = 0;
            if (context->cmd->argument[i][j]
                != '\0') // Si le $@ est collé au milieu
            {
                for (size_t count = 0;
                     count < strlen(context->cmd->argument[i]) - 2; count++)
                {
                    context->str_copy[context->x] =
                        context->cmd->argument[i][j];
                    j++;
                    context->x++;
                    count++;
                }
                res = 1;
                return res; // break
            }
            res = 1;
            return res; // break
        }
    }
    return res;
}

int handle_var_arobase_arg_size_other(expand_context_t *context, int i, int j,
                                      int j_arg)
{
    int res = 0;
    res = handle_var_arobase_arg_size_0(context, i, j, res);
    if (res != 0)
        return 1; // break
    else // Si le $@ est collé à gauche
    {
        char *tmp = context->str_copy;
        context->str_copy = strndup(context->cmd->argument[i] + 2,
                                    strlen(context->cmd->argument[i] + 2));
        free(tmp);
        context->inside_arobase = 0;
        return 1;
    }

    if (args->size_arg > 1)
    {
        strcat(context->str_copy, args->argument[j_arg]);
        j_arg++;
        context->new_tab_cmd[context->i_arg] =
            my_strcpy(context->new_tab_cmd[context->i_arg], context->str_copy);
        context->i_arg++;
    }

    // we loop over the array args of arguments
    for (; j_arg < args->size_arg - 1; j_arg++)
    {
        context->new_tab_cmd[context->i_arg] = my_strcpy(
            context->new_tab_cmd[context->i_arg], args->argument[j_arg]);
        context->i_arg++;
    }

    // On gere le dernier arg;
    int j_copy = j;
    while (context->cmd->argument[i][j])
        j++;
    char *argvlast = strndup(context->cmd->argument[i] + j_copy, j - j_copy);
    char *tmp = context->str_copy;
    context->str_copy = argvlast;
    free(tmp);
    return 1; // break
}

int handle_var_arobase(expand_context_t *context, int i, int j)
{
    int breake = 0;
    if (context->double_quote_on == 0 && context->single_quote_on == 0
        && context->cmd->argument[i][j] == '$'
        && context->cmd->argument[i][j + 1] == '@')
    {
        j += 2; // On skip $@
        int j_arg = 0;

        if (handle_var_arobase_arg_size_1(context, i, j, j_arg) == 1)
        {
            breake = 1;
            return breake;
        }

        if (handle_var_arobase_arg_size_other(context, i, j, j_arg) == 1)
        {
            breake = 1;
            return breake;
        }
    }
    return breake;
}
