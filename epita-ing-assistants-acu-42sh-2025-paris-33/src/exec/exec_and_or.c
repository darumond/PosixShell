#include "exec_and_or.h"

int run_and(struct ast *ast, struct ast *root)
{
    int res = 0;

    if ((res = ast_exec(ast->children[0], root)) == 1)
    {
        return res;
    }
    else
    {
        res = ast_exec(ast->children[1], root);
        return res;
    }
}

int run_or(struct ast *ast, struct ast *root)
{
    int res = 0;

    if ((res = ast_exec(ast->children[0], root)) == 0)
    {
        return res;
    }
    else
    {
        res = ast_exec(ast->children[1], root);
        return res;
    }
}
