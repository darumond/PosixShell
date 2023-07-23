#include "exec_pipe.h"

int exec_pipe(struct ast *left_son, struct ast *right_son, struct ast *root)
{
    int fds[2];
    pipe(fds);
    pid_t left = exec_fork(left_son, fds, LEFT, root);
    pid_t right = exec_fork(right_son, fds, RIGHT, root);
    close(fds[0]);
    close(fds[1]);
    int status;
    waitpid(left, &status, 0);
    waitpid(right, &status, 0);
    return WEXITSTATUS(status);
}

int run_pipe(struct ast *ast_pipe, struct ast *root)
{
    return exec_pipe(ast_pipe->children[0], ast_pipe->children[1], root);
}
