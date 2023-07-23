#include "exec_if.h"

#define BREAK 100000001
#define CONTINUE -100000001

int run_compound_list(struct ast *ast, struct ast *root)
{
    size_t nb_children = ast->nb_children;
    int res = -1;
    for (size_t i = 0; i < nb_children; i++)
    {
        if (ast->children[i]->type == AST_FUNC)
        {
            continue;
        }
        res = ast_exec(ast->children[i], root);
        if (res >= BREAK || res <= CONTINUE)
        {
            break;
        }
    }
    return res;
}

int run_if(struct ast *ast, struct ast *root)
{
    int res = run_compound_list(ast->children[0], root);
    if (res == 0)
    {
        res = run_compound_list(ast->children[1], root);
    }
    else
    {
        if (ast->children[2])
        {
            if (ast->children[2]->type == AST_IF)
            {
                res = run_if(ast->children[2], root);
            }
            else
            {
                res = run_compound_list(ast->children[2], root);
            }
        }
        else
        {
            return 0;
        }
    }
    return res;
}
