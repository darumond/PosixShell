#include "utils.h"

#include "../io_backend/io_backend.h"
#include "lexer/lexer.h"

// void print_table_char(char **arguments)
// {
//     for (size_t i = 0; strcmp(arguments[i], "") != 0; i++)
//     {
//         printf("\"");
//         for (size_t j = 0; arguments[i][j] != '\0'; j++)
//         {
//             printf("%c", arguments[i][j]);
//             if (arguments[i][j + 1] == '\0')
//                 printf("\" ");
//         }

//         if (strcmp(arguments[i + 1], "") == 0)
//         {
//             printf("}");
//         }
//     }
// }

// void pretty_print(struct ast *ast)
// {
//     size_t nb_child = ast->nb_children;
//     for (size_t i = 0; i < nb_child; i++)
//     {
//         switch (ast->children[i]->type)
//         {
//         case AST_IF:
//             printf("if ");
//             pretty_print(ast->children[i]);
//             break;
//         case AST_COMPOUND:
//             if (i > 0 && i % 2 == 0)
//                 printf("else ");
//             if (i > 0 && i % 2 != 0)
//                 printf("then ");
//             pretty_print(ast->children[i]);
//             break;
//         case AST_SIM_CMD:
//             printf("{argument --> ");
//             print_table_char(ast->children[i]->data.ast_simple_cmd->argument);
//             printf("{redirections --> ");
//             print_table_char(
//                 ast->children[i]->data.ast_simple_cmd->redirections);
//             printf("{variable --> ");
//             print_table_char(ast->children[i]->data.ast_simple_cmd->variable);
//             printf("\n");
//         default:
//             break;
//         }
//     }
// }

void my_freetoken(token_t **token)
{
    free((*token)->value);
    free(*token);
}

static void my_free_simp_cmd(struct treeSimpleCommand *tree_simple_command)
{
    for (size_t i = 0; i < 100; i++)
    {
        if (tree_simple_command->argument)
            free(tree_simple_command->argument[i]);
        if (tree_simple_command->redirections)
            free(tree_simple_command->redirections[i]);
        if (tree_simple_command->variable)
            free(tree_simple_command->variable[i]);
    }
    free(tree_simple_command->argument);
    free(tree_simple_command->redirections);
    free(tree_simple_command->variable);
    free(tree_simple_command);
}

void my_free_var(struct treeSimpleCommand *tree_simple_command)
{
    for (size_t i = 0; i < 100; i++)
    {
        free(tree_simple_command->variable[i]);
    }
    free(tree_simple_command->variable);
}

static void my_free_for(struct treeFor *ast)
{
    for (size_t i = 0; i < 100; i++)
    {
        free(ast->word_list[i]);
    }
    free(ast->word_list);
    free(ast->variable);
    free(ast);
}

static void free_handle_type(struct ast *ast)
{
    if (ast->type == AST_COMPOUND)
        free(ast->data.ast_compound);
    if (ast->type == AST_IF)
        free(ast->data.ast_if);
    if (ast->type == AST_LIST)
        free(ast->data.ast_list);
    if (ast->type == AST_SIM_CMD)
        my_free_simp_cmd(ast->data.ast_simple_cmd);
    if (ast->type == AST_PIPE)
        free(ast->data.ast_list);
    if (ast->type == AST_NEGATION)
        free(ast->data.ast_negation);
    if (ast->type == AST_WHILE)
        free(ast->data.ast_while);
    if (ast->type == AST_UNTIL)
        free(ast->data.ast_until);
    if (ast->type == AST_AND || ast->type == AST_OR)
        free(ast->data.ast_and_or);
    if (ast->type == AST_FOR)
        my_free_for(ast->data.ast_for);
    if (ast->type == AST_FUNC)
    {
        free(ast->data.ast_func->name);
        free(ast->data.ast_func);
    }
}

void free_ast(struct ast *ast)
{
    for (size_t i = 0; i < ast->nb_children; i++)
    {
        free_ast(ast->children[i]);
    }
    free_handle_type(ast);
    free(ast->children);
    free(ast);
}

token_t *my_skip_space(token_t **token)
{
    while ((*token)->type_token == SPACE)
    {
        my_freetoken(token);
        *token = send_token();
    }
    return *token;
}

char **init_tab(size_t size)
{
    // On initialise un tableau
    char **new_tab_cmd = calloc(size, sizeof(char *));
    // int mybreak = 0;
    for (size_t i = 0; i < size; i++)
    {
        new_tab_cmd[i] = calloc(size, sizeof(char));
    }
    return new_tab_cmd;
}

void my_free_tab(char **str, int size)
{
    for (int i = 0; i < size; i++)
    {
        free(str[i]);
    }
    free(str);
}
