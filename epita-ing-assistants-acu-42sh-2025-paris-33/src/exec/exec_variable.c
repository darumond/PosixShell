#include "exec_variable.h"

#include "../io_backend/io_backend.h"
#include "hash_map.h"

bool is_variable(struct ast *ast)
{
    if (ast->data.ast_simple_cmd->variable
        && strcmp(ast->data.ast_simple_cmd->variable[0], "") != 0)
        return true;
    return false;
}

int run_variable(struct ast *ast)
{
    char **var = ast->data.ast_simple_cmd->variable;
    for (int i = 0; i < len_arg(var); i++)
    {
        int j = 0;
        while (var[i][j] && var[i][j] != '=')
            j++;

        char *key = strndup(var[i], j);

        j += 1; // skip '='

        int j_copy = j;
        while (var[i][j])
            j++;
        char *value = strndup(var[i] + j_copy, j - j_copy);
        bool b = false;
        hash_map_insert(args->variable, key, value, &b);
    }
    // hash_map_dump(args->variable);
    return 1;
}
