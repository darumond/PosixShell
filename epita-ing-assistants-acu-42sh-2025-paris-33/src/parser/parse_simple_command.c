#include "parse_simple_command.h"

#include <string.h>

#include "../tools/utils.h"

#define BREAK 123456789

static struct ast *init_ast_simple_command(void)
{
    struct ast *ast_simple_command = calloc(1, sizeof(struct ast));
    check_allocate(ast_simple_command);
    ast_simple_command->type = AST_SIM_CMD;
    ast_simple_command->data.ast_simple_cmd =
        calloc(1, sizeof(struct treeSimpleCommand));

    check_allocate(ast_simple_command->data.ast_simple_cmd);
    ast_simple_command->data.ast_simple_cmd->argument =
        calloc(100, sizeof(char *));
    check_allocate(ast_simple_command->data.ast_simple_cmd->argument);

    ast_simple_command->data.ast_simple_cmd->redirections =
        calloc(100, sizeof(char *));
    check_allocate(ast_simple_command->data.ast_simple_cmd->redirections);

    ast_simple_command->data.ast_simple_cmd->variable =
        calloc(100, sizeof(char *));
    check_allocate(ast_simple_command->data.ast_simple_cmd->variable);

    ast_simple_command->data.ast_simple_cmd->taille_tableau = 100;
    size_t taille_tableau = 100;
    for (size_t i = 0; i < taille_tableau; i++)
    {
        ast_simple_command->data.ast_simple_cmd->redirections[i] =
            calloc(taille_tableau, sizeof(char));
        check_allocate(
            ast_simple_command->data.ast_simple_cmd->redirections[i]);

        ast_simple_command->data.ast_simple_cmd->argument[i] =
            calloc(taille_tableau, sizeof(char));
        check_allocate(ast_simple_command->data.ast_simple_cmd->argument[i]);

        ast_simple_command->data.ast_simple_cmd->variable[i] =
            calloc(taille_tableau, sizeof(char));
        check_allocate(ast_simple_command->data.ast_simple_cmd->variable[i]);
    }
    return ast_simple_command;
}

void parse_variable(struct ast *tree_simple_command, token_t **token)
{
    int j = 0;
    while (
        (*token)->type_token
        == ASSIGNMENT_WORD) //&& strcmp((*token)->value, "cat") != 0) // On
                            // change cette conditions par is_assignement_word
    {
        if ((*token)->type_token == ENDOFFILE)
        {
            return;
        }
        if ((*token)->type_token == PIPE || (*token)->type_token == AND
            || (*token)->type_token == OR)
        {
            return;
        }

        (*tree_simple_command).data.ast_simple_cmd->variable[j] =
            my_strcpy_simple_command(
                (*tree_simple_command).data.ast_simple_cmd->variable[j],
                (*token)->value, tree_simple_command);

        if ((*token)->newline || (*token)->semicolon)
        {
            return;
        }
        my_freetoken(token);
        *token = send_token();
        *token = my_skip_space(token);

        j++;
    }
    if (!(*token)->newline && !(*token)->semicolon)
    {
        my_free_var((*tree_simple_command).data.ast_simple_cmd);
        (*tree_simple_command).data.ast_simple_cmd->variable = NULL;
    }
}

int handle_redir_token(token_t **token, struct ast *tree_simple_command)
{
    token = handle_redir(
        tree_simple_command, token,
        len_arg((*tree_simple_command).data.ast_simple_cmd->redirections));
    if ((*token)->semicolon || (*token)->newline)
        return 0;
    return 1;
}

void add_token(struct ast *tree_simple_command, token_t **token, int i)
{
    (*tree_simple_command).data.ast_simple_cmd->argument[i] =
        my_strcpy_simple_command(
            (*tree_simple_command).data.ast_simple_cmd->argument[i],
            (*token)->value, tree_simple_command);
}

int handle_token_redir_newline(token_t **token, struct ast *tree_simple_command)
{
    token = handle_redir(
        tree_simple_command, token,
        len_arg((*tree_simple_command).data.ast_simple_cmd->redirections));
    if (is_newline_semicolon(token))
        return 1;
    return 0;
}

static void skip_token(token_t **token)
{
    my_freetoken(token);
    *token = send_token();
    *token = my_skip_space(token);
}

bool is_newline_semicolon(token_t **token)
{
    if ((*token)->newline || (*token)->type_token == NEWLINE
        || (*token)->semicolon)
        return true;
    return false;
}

bool is_pipe_or_and(token_t **token)
{
    if ((*token)->type_token == PIPE || (*token)->type_token == AND
        || (*token)->type_token == OR)
        return true;
    return false;
}

bool is_quote_word(token_t **token, int is_echo)
{
    if (parse_element(token) == 1 || is_echo
        || (*token)->type_token == BETWEEN_QUOTES
        || (*token)->type_token == ASSIGNMENT_WORD)
        return true;
    return false;
}

static int sub1_cmd(token_t **token)
{
    if ((*token)->type_token == PARENTHESIS)
    {
        return 1;
    }
    else
    {
        free(args->save_token);
        return 0;
    }
}

static int sub2_cmd(struct ast *tree_simple_command, token_t **token)
{
    if ((*token)->type_token == REDIRECTION)
    {
        if (handle_token_redir_newline(token, tree_simple_command) == 1
            && ((*token)->type_token != WORDS))
            return 0;
    }
    if ((*token)->type_token == ENDOFFILE)
        return 0;

    if ((*token)->type_token == WORDS)
    {
        if (len_arg(tree_simple_command->data.ast_simple_cmd->redirections) > 0)
        {
            size_t index =
                len_arg(tree_simple_command->data.ast_simple_cmd->redirections)
                - 1;
            if (strcmp((*token)->value,
                       tree_simple_command->data.ast_simple_cmd
                           ->redirections[index])
                == 0)
                return 0;
        }
    }
    return -1;
}

static int sub3_cmd(struct ast *tree_simple_command, token_t **token)
{
    if ((*token)->type_token == REDIRECTION)
    {
        if (handle_redir_token(token, tree_simple_command) == 0)
        {
            char **str = tree_simple_command->data.ast_simple_cmd->redirections;
            if (is_operation(str[len_arg(str) - 1])
                || (strcmp((*token)->value, str[len_arg(str) - 1]) == 0))
                return 0;
        }
    }
    return -1;
}
static int sub4_cmd(token_t **token, int *i, int *eof,
                    struct ast *tree_simple_command)
{
    int p = *i;
    if ((*token)->type_token == ENDOFFILE)
    {
        *eof = 0;
        return BREAK;
    }
    if ((*token)->type_token == REDIRECTION)
    {
        *eof = 1;
        return BREAK;
    }

    if (is_pipe_or_and(token))
    {
        return 0;
    }

    add_token(tree_simple_command, token, p);

    if ((*token)->newline || (*token)->semicolon)
    {
        return 0;
    }
    skip_token(token);
    p++;
    *i = p;
    return -1;
}

int fill_simple_command(struct ast *tree_simple_command, token_t **token)
{
    // we check here if it's a assign_word for variable
    if ((*token)->type_token == ASSIGNMENT_WORD)
    {
        parse_variable(tree_simple_command, token);
    }

    int res1 = sub2_cmd(tree_simple_command, token);
    if (res1 != -1)
        return res1;
    int i = 0;
    add_token(tree_simple_command, token, i);

    if (is_newline_semicolon(token))
    {
        return 0;
    }
    int is_echo = 0;
    if (strcmp((*token)->value, "echo") == 0)
    {
        is_echo = 1;
    }

    args->save_token = calloc(100, 1);
    my_strcpy(args->save_token, (*token)->value);
    skip_token(token);
    int res = sub1_cmd(token);
    if (res != 0)
        return res;
    int eof = 0;
    i++;
    while (1)
    {
        while (is_quote_word(token, is_echo))
        {
            int res2 = sub4_cmd(token, &i, &eof, tree_simple_command);
            if (res2 != -1)
            {
                if (res2 == BREAK)
                {
                    break;
                }
                return res2;
            }
        }
        int res3 = sub3_cmd(tree_simple_command, token);
        if (res3 != -1)
            return res;
        if (eof == 1)
            continue;
        break;
    }
    return 0;
}

struct ast *parse_simple_command(token_t **token)
{
    if (strcmp((*token)->value, "{") == 0)
    {
        return NULL;
    }
    *token = my_skip_space(token);
    if (parse_element(token) == 1 || (*token)->type_token == ASSIGNMENT_WORD
        || (*token)->type_token == REDIRECTION)
    {
        struct ast *tree_simple_command =
            init_ast_simple_command(); // We retrieve here a node of type
                                       // simplComm

        if (fill_simple_command(tree_simple_command, token) == 1)
        {
            free_ast(tree_simple_command);
            return NULL;
        }
        return tree_simple_command;
    }
    return NULL;
}
