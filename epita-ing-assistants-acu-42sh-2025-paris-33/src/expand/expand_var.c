#include "expand_var.h"

#include <stdlib.h>
#include <string.h>

int handle_var_star(expand_context_t *context, int i, int j)
{
    if (context->double_quote_on == 0 && context->single_quote_on == 0
        && strcmp(context->cmd->argument[i], "$*") == 0)
    {
        j += 2; // On skip $*
        for (int count_i = 0; count_i < args->size_arg; count_i++)
        {
            for (int count_j = 0; args->argument[count_i][count_j]; count_j++)
            {
                context->str_copy[context->x] =
                    args->argument[count_i][count_j];
                context->x++;
            }
            if (count_i < args->size_arg - 1)
                context->str_copy[context->x] = ' ';
            context->x++;
        }
    }

    if (context->double_quote_on == 0 && context->single_quote_on == 0
        && context->cmd->argument[i][j] == '$'
        && context->cmd->argument[i][j + 1] == '*')
    {
        j += 2; // On skip $*

        if (args->size_arg > 0)
        {
            for (int count_i = 0; count_i < args->size_arg; count_i++)
            {
                for (int count_j = 0; args->argument[count_i][count_j];
                     count_j++)
                {
                    context->str_copy[context->x] =
                        args->argument[count_i][count_j];
                    context->x++;
                }
                if (count_i < args->size_arg - 1)
                {
                    context->str_copy[context->x] = ' ';
                    context->x++;
                }
            }
        }
    }
    return j;
}

int handle_var_interrogation(expand_context_t *context, int i, int j)
{
    if (context->double_quote_on == 0 && context->single_quote_on == 0
        && context->cmd->argument[i][j] == '$'
        && context->cmd->argument[i][j + 1] == '?')
    {
        j += 2; // On skip $?
        char *exit_status = int_to_str(args->exit_status);
        if (strlen(exit_status) >= 1)
        {
            for (size_t p = 0; p < strlen(exit_status); p++)
            {
                context->str_copy[context->x] = exit_status[p];
                context->x++;
            }
        }
        else
        {
            context->str_copy[context->x] = args->exit_status + '0';
            context->x++;
        }
        free(exit_status);
    }
    return j;
}

int handle_var_diez(expand_context_t *context, int i, int j)
{
    if (context->double_quote_on == 0 && context->single_quote_on == 0
        && context->cmd->argument[i][j] == '$'
        && context->cmd->argument[i][j + 1] == '#')
    {
        j += 2; // skip le $#
        char *buff;
        if (len_arg(args->argument) == 0)
        {
            buff = int_to_str(0);
            context->str_copy[context->x] = '0';
            context->x++;
            free(buff);
        }
        else
        {
            buff = int_to_str(len_arg(args->argument));
            int z = 0;
            while (buff[z] != '\0')
            {
                context->str_copy[context->x] = buff[z];
                z++;
                context->x++;
            }
            free(buff);
        }
    }
    return j;
}

int handle_var_dollar(expand_context_t *context, int i, int j)
{
    if (context->double_quote_on == 0 && context->single_quote_on == 0
        && context->cmd->argument[i][j] == '$'
        && context->cmd->argument[i][j + 1] == '$')
    {
        j += 2; // skip $$
        int pid = getpid();
        char *process_pid = int_to_str(pid);

        for (size_t i = 0; process_pid[i] != '\0'; i++)
        {
            context->str_copy[context->x] = process_pid[i];
            context->x++;
        }
        free(process_pid);
    }
    return j;
}

int handle_var_uid(expand_context_t *context, int i, int j)
{
    if (context->double_quote_on == 0 && context->single_quote_on == 0
        && context->cmd->argument[i][j] == '$')
    {
        if (strstr(context->cmd->argument[i] + j, "UID"))
        {
            j++;
            if (context->cmd->argument[i][j] == '{')
                j++;

            int uid = getgid();
            char *group_id = int_to_str(uid);

            for (size_t i = 0; group_id[i] != '\0'; i++)
            {
                context->str_copy[context->x] = group_id[i];
                context->x++;
                j++;
            }
            free(group_id);
        }
    }
    return j;
}

int handle_var_argument(expand_context_t *context, int i, int j, bool *continu)
{
    if (context->double_quote_on == 0 && context->single_quote_on == 0
        && context->cmd->argument[i][j] == '$'
        && (isdigit(context->cmd->argument[i][j + 1])
            || isdigit(context->cmd->argument[i][j + 2])))
    {
        if (context->cmd->argument[i][j + 1] == '{')
            j++;

        j++;
        int j_copy = j;
        while (context->cmd->argument[i][j]
               && (isdigit(context->cmd->argument[i][j])))
            j++;

        if (context->cmd->argument[i][j] == '}')
            j++; // need to skip the '}'

        char *number = strndup(context->cmd->argument[i] + j_copy, j - j_copy);
        if (atoi(number) > args->size_arg)
        {
            j--;
            free(number);
            *continu = true;
            // context->breake = true;
            return j;
        }
        char *tmp1 = number;
        char *value_var = args->argument[atoi(number) - 1];

        if (value_var)
        {
            int z = 0;
            while (value_var[z] != '\0')
            {
                context->str_copy[context->x] = value_var[z];
                z++;
                context->x++;
            }
        }
        free(tmp1);
    }
    return j;
}

int handle_var_basic_braquet(expand_context_t *context, int i, int j,
                             bool *continu)
{
    j++;
    int j_copy = j;
    while (context->cmd->argument[i][j] && context->cmd->argument[i][j] != '}')
        j++;

    char *key = strndup(context->cmd->argument[i] + j_copy, j - j_copy);
    char *tmp1 = key;
    char *value_var = hash_map_get(args->variable, key);

    if (hash_map_is_present(args->variable, key))
    {
        int z = 0;
        while (value_var[z] != '\0')
        {
            context->str_copy[context->x] = value_var[z];
            z++;
            context->x++;
        }
        free(tmp1);
        *continu = true;
        return j; // CONTINUE
    }
    free(tmp1);
    return j;
}

int handle_var_basic(expand_context_t *context, int i, int j, bool *continu)
{
    if (context->double_quote_on == 0 && context->single_quote_on == 0
        && context->cmd->argument[i][j] == '$'
        && context->cmd->argument[i][j + 1] != '\0'
        && context->cmd->argument[i][j + 1] != ' ')
    {
        j++;
        if (context->cmd->argument[i][j] == '{'
            && context->cmd->argument[i][0] != '\''
            && context->cmd->argument[i][0] != '\"')
        {
            j = handle_var_basic_braquet(context, i, j, continu);
        }
        else
        {
            int j_copy = j;
            while (context->cmd->argument[i][j]
                   && context->cmd->argument[i][j] != ' '
                   && context->cmd->argument[i][j] != '}'
                   && context->cmd->argument[i][j] != '$')
                j++;

            char *key = strndup(context->cmd->argument[i] + j_copy, j - j_copy);
            char *tmp1 = key;
            char *value_var = hash_map_get(args->variable, key);

            if (hash_map_is_present(args->variable, key))
            {
                if (value_var[0] == '\'' || value_var[0] == '"')
                {
                    my_strcpy(context->cmd->argument[i], value_var);
                    *continu = false;
                    free(tmp1);
                    j = 0;
                    return j;
                }
                int z = 0;
                while (value_var[z] != '\0')
                {
                    context->str_copy[context->x] = value_var[z];
                    z++;
                    context->x++;
                }
                free(tmp1);
                j--; // jcomprends pas pq mais ca marche mtn
                *continu = true;

                return j; // CONTINUE
            }
            free(tmp1);
        }
    }
    return j;
}

int handle_var_oldpwd(expand_context_t *context, int i, int j) // 37 lignes
{
    if (context->double_quote_on == 0 && context->single_quote_on == 0
        && context->cmd->argument[i][j] == '$')
    {
        if (strstr(context->cmd->argument[i], "OLDPWD") != NULL)
        {
            j++;
            if (context->cmd->argument[i][j] == '{')
                j++;

            char *env_var = getenv("OLDPWD");
            while (strcmp(context->str, "OLDPWD") != 0)
            {
                context->str[context->y] = context->cmd->argument[i][j];
                context->y++;
                j++;
            }

            if (context->cmd->argument[i][j] == '}')
                j++;

            if (strcmp(context->str, "OLDPWD") == 0)
            {
                context->str[context->y] = '\0';
                for (size_t i = 0; env_var[i] != '\0'; i++)
                {
                    context->str_copy[context->x] = env_var[i];
                    context->x++;
                }
            }
        }

        if (strstr(context->cmd->argument[i], "PWD") != NULL
            && context->str[0] != 'O')
        {
            j++;
            if (context->cmd->argument[i][j] == '{')
                j++;

            char *env_var = getenv("PWD");
            while (strcmp(context->str, "PWD") != 0)
            {
                context->str[context->y] = context->cmd->argument[i][j];
                context->y++;
                j++;
            }

            if (context->cmd->argument[i][j] == '}')
                j++;

            if (strcmp(context->str, "PWD") == 0)
            {
                context->str[context->y] = '\0';
                for (size_t i = 0; env_var[i] != '\0'; i++)
                {
                    context->str_copy[context->x] = env_var[i];
                    context->x++;
                }
            }
        }
    }
    return j;
}
