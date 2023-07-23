#include "exec.h"

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../builtin/builtin.h"
#include "../expand/expand.h"
#include "../tools/tools.h"
#include "../tools/utils.h"
#include "exec_and_or.h"
#include "exec_function.h"
#include "exec_header.h"
#include "exec_pipe.h"
#include "exec_variable.h"
#include "hash_map.h"

#define BREAK 100000001
#define CONTINUE -100000001

int handle_echo_unset_export_cd_dot(struct ast *ast, int res)
{
    res = -1;
    if (strcmp(ast->data.ast_simple_cmd->argument[0], "echo") == 0
        && strcmp(ast->data.ast_simple_cmd->redirections[0], "") == 0)
    {
        my_echo(ast->data.ast_simple_cmd);
        return 0;
    }
    if (strcmp(ast->data.ast_simple_cmd->argument[0], "unset") == 0)
    {
        return my_unset(ast->data.ast_simple_cmd);
    }
    if (strcmp(ast->data.ast_simple_cmd->argument[0], "true") == 0)
    {
        return 0;
    }
    if (strcmp(ast->data.ast_simple_cmd->argument[0], "false") == 0)
    {
        return 1;
    }

    if (strcmp(ast->data.ast_simple_cmd->argument[0], "export") == 0)
    {
        my_export(ast->data.ast_simple_cmd);
        return 0;
    }

    if (strcmp(ast->data.ast_simple_cmd->argument[0], "cd") == 0)
        return my_cd(ast->data.ast_simple_cmd);

    if (strcmp(ast->data.ast_simple_cmd->argument[0], ".") == 0)
    {
        FILE *fd = args->fp;
        if (ast->data.ast_simple_cmd->argument[1])
        {
            res = my_dot(ast->data.ast_simple_cmd->argument[1], fd);
        }
        return res;
    }
    return res;
}

int handle_exit_continue_break_empty(struct ast *ast, struct ast *root,
                                     int my_errno)
{
    if (strcmp(ast->data.ast_simple_cmd->argument[0], "break") == 0)
    {
        if (ast->data.ast_simple_cmd->argument[1] == NULL
            || strcmp(ast->data.ast_simple_cmd->argument[1], "") == 0)
        {
            return BREAK;
        }
        int n = atoi(ast->data.ast_simple_cmd->argument[1]);
        if (n > 0)
        {
            return BREAK + n;
        }
        my_errno = 5;
        return my_errno; // if my_errno == 5, it means argument is not positive
        // errx(1, "argument is not positive");
    }
    if (strcmp(ast->data.ast_simple_cmd->argument[0], "continue") == 0)
    {
        if (ast->data.ast_simple_cmd->argument[1] == NULL
            || strcmp(ast->data.ast_simple_cmd->argument[1], "") == 0)
        {
            return CONTINUE;
        }
        int n = atoi(ast->data.ast_simple_cmd->argument[1]);
        if (n > 0)
        {
            return CONTINUE - n;
        }
        my_errno = 5; // if my_errno == 5, it means argument is not positive
        return my_errno;
        // errx(1, "argument is not positive");
    }
    if (strcmp(ast->data.ast_simple_cmd->argument[0], "exit") == 0)
    {
        close_file();
        if (ast->data.ast_simple_cmd->argument[1] == NULL
            || strcmp(ast->data.ast_simple_cmd->argument[1], "") == 0)
        {
            free_ast(root);
            exit(0);
        }
        else
        {
            int exit_status = atoi(ast->data.ast_simple_cmd->argument[1]);
            free_ast(root);
            exit(exit_status);
        }
    }
    if (strcmp(ast->data.ast_simple_cmd->argument[0], "") == 0)
    {
        my_errno = 10; // if my_errno == 10, it means EMPTY
        return my_errno;
        // errx(2, "EMPTY");
    }
    return my_errno;
}

int run_sim_cmd(struct ast *ast, struct ast *root)
{
    int res = 0;
    bool is_func = false;

    ast->data.ast_simple_cmd = expand(ast->data.ast_simple_cmd, &is_func);
    if (is_func)
    {
        res = ast_exec(
            hash_map_get_func(args->function, args->key_func)->children[0],
            root);
        return res;
    }
    res = handle_echo_unset_export_cd_dot(ast, res);
    if (res == -1)
        res = 0;
    else
        return res;

    int my_errno = 0;
    my_errno = handle_exit_continue_break_empty(ast, root, my_errno);
    if (my_errno != 0)
    {
        if (my_errno == 5)
            errx(1, "argument is not positive");
        if (my_errno == 10)
            errx(2, "EMPTY");
        else
            return my_errno;
    }
    int i = len_arg(ast->data.ast_simple_cmd->argument);
    free(ast->data.ast_simple_cmd->argument[i]);
    ast->data.ast_simple_cmd->argument[i] = NULL;

    int pid = fork();

    // We are in the children
    if (pid == 0)
    {
        int res = execvp(ast->data.ast_simple_cmd->argument[0],
                         ast->data.ast_simple_cmd->argument);
        if (res < 0)
            errx(127, "Command not found!");
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
        return WEXITSTATUS(status); //! WEXITSTATUS
    }
    return res;
}

int run_while(struct ast *ast, struct ast *root)
{
    int res = 0;
    while (run_compound_list(ast->children[0], root) == 0)
    {
        res = run_compound_list(ast->children[1], root);
        if (res >= BREAK)
        {
            break;
        }
        if (res <= CONTINUE)
        {
            continue;
        }
    }
    if (res == BREAK || res == CONTINUE)
    {
        return 0;
    }
    if (res > BREAK)
    {
        return res - 1;
    }
    if (res < CONTINUE)
    {
        return res + 1;
    }
    return res;
}

int run_until(struct ast *ast, struct ast *root)
{
    int res = 0;
    while ((res = run_compound_list(ast->children[0], root)) != 0)
    {
        if (res >= BREAK)
        {
            break;
        }
        if (res <= CONTINUE)
        {
            continue;
        }
        res = run_compound_list(ast->children[1], root);
        if (res <= CONTINUE)
        {
            continue;
        }
        if (res >= BREAK)
        {
            break;
        }
    }
    if (res == BREAK || res == CONTINUE)
    {
        return 0;
    }
    if (res > BREAK)
    {
        return res - 1;
    }

    return res;
}

int run_shell_cmd(struct ast *ast, struct ast *root)
{
    int res = 0;
    if (ast->type == AST_IF)
    {
        res = run_if(ast, root);
    }
    if (ast->type == AST_WHILE)
    {
        if (res < CONTINUE)
        {
            return res + 1;
        }
        res = run_while(ast, root);
    }
    if (ast->type == AST_UNTIL)
    {
        res = run_until(ast, root);
    }
    return res;
}

int run_negation(struct ast *ast, struct ast *root)
{
    int res = 0;

    res = ast_exec(ast->children[0], root);
    if (res == 0)
    {
        res = 1;
    }
    else
    {
        res = 0;
    }
    return res;
}

int run_for(struct ast *ast, struct ast *root)
{
    int res = 0;
    if (!ast->data.ast_for->word_list)
    {
        return 0;
    }

    char **var = ast->data.ast_for->word_list;
    char **list = init_table();
    copy_matrix(
        list,
        ast->data.ast_for->tree_do->children[0]->data.ast_simple_cmd->argument);
    for (int i = 0; i < len_arg(var); i++)
    {
        char *variable = ast->data.ast_for->variable;
        char *key = strndup(variable, strlen(variable));
        char *words = var[i];
        char *value = strndup(words, strlen(words));
        bool b = false;
        hash_map_insert(args->variable, key, value, &b);
        if (i != 0)
            copy_matrix(ast->data.ast_for->tree_do->children[0]
                            ->data.ast_simple_cmd->argument,
                        list);
        res = run_compound_list(ast->data.ast_for->tree_do, root);
        if (res >= BREAK)
        {
            break;
        }
        if (res <= CONTINUE)
        {
            continue;
        }
    }
    free_table(list);
    if (res == BREAK || res == CONTINUE)
    {
        return 0;
    }
    if (res > BREAK)
    {
        return res - 1;
    }
    if (res < CONTINUE)
    {
        return res + 1;
    }
    return res;
}

pid_t exec_fork(struct ast *node, int fds[2], enum pipe side, struct ast *root)
{
    pid_t pid = fork();
    if (pid != 0)
        return pid;

    enum pipe other_side = LEFT;

    if (side == LEFT)
        other_side = RIGHT;

    if (close(fds[side]) == -1)
        errx(1, "Fail to close side");

    dup2(fds[1 - side], side == LEFT ? STDOUT_FILENO : STDIN_FILENO);
    close(fds[other_side]);
    if (ast_exec(node, root) != 0)
    {
        exit(1);
    }
    exit(0);
}

int ast_exec(struct ast *ast, struct ast *root)
{
    int res = 0;
    if (!ast)
    {
        return 2;
    }
    if (ast->type == AST_NEGATION)
    {
        res = run_negation(ast, root);
        return res;
    }
    if (ast->type == AST_SIM_CMD)
    {
        if (is_redirection(ast))
            res = run_redirection(ast, root);
        else if (is_variable(ast))
            res = run_variable(ast);
        else
            res = run_sim_cmd(ast, root);
        return res;
    }
    if (ast->type == AST_PIPE)
    {
        res = run_pipe(ast, root);
        return res;
    }
    if (ast->type == AST_AND)
    {
        res = run_and(ast, root);
        return res;
    }
    if (ast->type == AST_OR)
    {
        res = run_or(ast, root);
        return res;
    }
    if (ast->type == AST_FOR)
    {
        res = run_for(ast, root);
        return res;
    }
    if (ast->type == AST_COMPOUND)
    {
        res = run_compound_list(ast, root);
        return res;
    }
    if (ast->type == AST_FUNC)
    {
        run_func(ast);
        return 0;
    }

    res = run_shell_cmd(ast, root);
    return res;
}
