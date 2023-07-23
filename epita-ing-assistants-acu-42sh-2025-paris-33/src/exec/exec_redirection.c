#include "exec_redirection.h"

#include <stdio.h>
#include <string.h>

bool is_redirection(struct ast *ast)
{
    if (strcmp(ast->data.ast_simple_cmd->redirections[0], "") == 0)
        return false;
    return true;
}

int open_flag_redirection(char *op, char *filename)
{
    int fd;
    if (strcmp(op, ">") == 0)
    {
        fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    }
    if (strcmp(op, "<") == 0)
    {
        fd = open(filename, O_RDONLY, S_IRUSR | S_IWUSR);
    }
    if (strcmp(op, ">>") == 0)
    {
        fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
    }
    if (strcmp(op, ">&") == 0)
    {
        fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    }
    if (strcmp(op, "<&") == 0)
    {
        fd = open(filename, O_RDONLY | O_CREAT, S_IRUSR | S_IWUSR);
    }
    if (strcmp(op, ">|") == 0)
    {
        fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    }
    if (strcmp(op, "<>") == 0)
    {
        fd = open(filename, O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
    }
    return fd;
}

bool is_operation(char *str)
{
    if (strcmp(str, ">") != 0 && strcmp(str, "<") != 0 && strcmp(str, ">>") != 0
        && strcmp(str, ">&") != 0 && strcmp(str, "<&") != 0
        && strcmp(str, ">|") != 0 && strcmp(str, "<>") != 0)
    {
        return false;
    }
    return true;
}

int handle_io_number_value(char *str)
{
    if (str[0] == '>')
        return 1;
    else //(str[0] == '<')
        return 0;
}

void destroy_tab_fdup_io_number(int *tab_fdup, int *tab_io_number)
{
    free(tab_fdup);
    free(tab_io_number);
}

void handle_link_reset(struct ast **ast, struct ast **root,
                       redirection_context_t *context)
{
    if (strcmp((*ast)->data.ast_simple_cmd->argument[0], "") != 0)
        context->exit_status = run_sim_cmd(*ast, *root);

    for (int i = len_arg(context->redir) - 1; i >= 0; i--)
    {
        context->new_stdout =
            dup2(context->tab_fdup[i], context->tab_io_number[i]);
    }
    destroy_tab_fdup_io_number(context->tab_fdup, context->tab_io_number);
}

redirection_context_t *init_redirection_context(struct ast **ast)
{
    redirection_context_t *context = malloc(sizeof(redirection_context_t));
    context->redir = (*ast)->data.ast_simple_cmd->redirections;
    context->tab_fdup = calloc(len_arg(context->redir), sizeof(int));
    context->tab_io_number = calloc(len_arg(context->redir), sizeof(int));
    check_allocate(context->tab_fdup);
    check_allocate(context->tab_io_number);
    context->io_number = -1;
    context->exit_status = 0;
    context->j = 0;
    return context;
}

void destroy_and_close(redirection_context_t *context, bool close_2_time)
{
    if (close_2_time)
        close(context->fd_dup);
    close(context->fd_file);
    destroy_tab_fdup_io_number(context->tab_fdup, context->tab_io_number);
}

int handle_redirection(redirection_context_t *context, int my_errno)
{
    for (int i = 0; i < len_arg(context->redir); i++)
    {
        if (my_is_digit(context->redir[i]))
        {
            context->io_number = atoi(context->redir[i]);
            continue;
        }
        if (context->io_number == -1)
        {
            context->io_number = handle_io_number_value(context->redir[i]);
        }
        if (is_operation(context->redir[i]))
        {
            context->fd_file =
                open_flag_redirection(context->redir[i], context->redir[i + 1]);
            if (context->fd_file == -1)
            {
                if (errno == ENOENT)
                {
                    perror(context->redir[i + 1]);
                    destroy_tab_fdup_io_number(context->tab_fdup,
                                               context->tab_io_number);
                    return 1;
                }
            }
            context->fd_dup = dup(context->io_number);
            if (context->fd_dup == -1)
            {
                destroy_and_close(context, false);
                my_errno = 5; // if my_errno == 5, it means You Could not dup
                              // file for redirection!
                return my_errno;
                // errx(1, "Could not dup file for redirection!");
            }
            context->tab_io_number[context->j] = context->io_number;
            context->tab_fdup[context->j] = context->fd_dup;
            context->j++;
            context->new_stdout = dup2(context->fd_file, context->io_number);
            if (context->new_stdout == -1)
            {
                destroy_and_close(context, true);
                my_errno = 10; // if my_errno == 10, it means fdup : Could not
                               // dup stdout!
                return my_errno;
            }
            if (close(context->fd_file) == -1)
            {
                destroy_and_close(context, true);
                my_errno = 15; // if my_errno == 15, it means io_number : Could
                               // not dup stdout!
                return my_errno;
            }
        }
        context->io_number = -1;
    }
    return my_errno;
}

int run_redirection(struct ast *ast, struct ast *root)
{
    redirection_context_t *context = init_redirection_context(&ast);
    int my_errno = 0;
    my_errno = handle_redirection(context, my_errno);
    if (my_errno == 1)
        return my_errno;
    else if (my_errno == 5)
    {
        errx(1, "Could not dup file for redirection!");
    }
    else if (my_errno == 10)
    {
        errx(1, "fdup : Could not dup stdout!");
    }
    else if (my_errno == 15)
    {
        errx(1, "io_number : Could not dup stdout!");
    }
    handle_link_reset(&ast, &root, context);
    int exit_status = context->exit_status;
    free(context);
    return exit_status;
}
