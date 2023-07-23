#include "expand_basic.h"

#include <stdlib.h>
#include <string.h>

int handle_quote_on(expand_context_t *context, int i, int j)
{
    int continu = 0;
    if (context->cmd->argument[i][j] == '\'' && context->double_quote_on == 0)
    {
        if (context->single_quote_on == 0)
            context->single_quote_on = 1;
        else
            context->single_quote_on = 0;
        continu = 1;
        return continu;
    }
    if (context->cmd->argument[i][j] == '\"' && context->single_quote_on == 0)
    {
        if (context->double_quote_on == 1)
            context->double_quote_on = 0;
        else
            context->double_quote_on = 1;
        continu = 1;
        return continu;
    }
    return continu;
}

int handle_slash(expand_context_t *context, int i, int j, bool *continu)
{
    if (context->double_quote_on == 0 && context->cmd->argument[i][j] == '\\'
        && context->single_quote_on == 0)
    {
        j += 1;
        if (context->cmd->argument[i][j] != '\n')
        {
            context->str_copy[(context->x)] = context->cmd->argument[i][j];
            context->x++;
        }
        *continu = true;
        return j;
    }
    return j;
}

int handle_slash_dollar_quote_newline(expand_context_t *context, int i, int j,
                                      bool *continu)
{
    if (context->double_quote_on == 0 && context->single_quote_on == 0
        && context->cmd->argument[i][j] == '\\'
        && context->cmd->argument[i][j + 1] == '\\')
    {
        context->str_copy[context->x] = '\\';
        j++;
        context->x++;
        *continu = true;
        return j;
    }
    else if (context->double_quote_on == 1
             && context->cmd->argument[i][j] == '\\'
             && (context->cmd->argument[i][j + 1] == '$'
                 || context->cmd->argument[i][j + 1] == '`'
                 || context->cmd->argument[i][j + 1] == '\"'
                 || context->cmd->argument[i][j + 1] == '\\'))
    {
        j++;
        context->str_copy[context->x] = context->cmd->argument[i][j];
        context->x++;
        *continu = true;
        return j;
    }
    else if (context->double_quote_on == 1
             && context->cmd->argument[i][j] == '\\'
             && context->cmd->argument[i][j + 1] == '\n')
    {
        j++;
        *continu = true;
        return j;
    }
    else
    {
        if (context->double_quote_on == 0 && context->single_quote_on == 0
            && context->cmd->argument[i][j] == '$')
        {
            j--;
            *continu = true;
            return j;
        }
        context->str_copy[context->x] = context->cmd->argument[i][j];
        context->x++;
    }
    return j;
}
