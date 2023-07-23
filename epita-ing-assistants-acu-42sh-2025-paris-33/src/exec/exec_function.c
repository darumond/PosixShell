#include "exec_function.h"

void run_func(struct ast *node)
{
    char *key = strdup(node->data.ast_func->name);
    if (node->children[0]->type == AST_COMPOUND)
    {
        size_t nb_children = node->children[0]->nb_children;
        for (size_t i = 0; i < nb_children; i++)
        {
            if (node->children[0]->children[i]->type == AST_FUNC)
            {
                run_func(node->children[0]->children[i]);
            }
        }
    }

    struct ast *value = node;
    bool func_bool = false;
    hash_map_insert_func(args->function, key, value, &func_bool);
    if (func_bool == true)
    {
        args->reference_counting++;
    }
}
