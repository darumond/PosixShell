#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define _POSIX_C_SOURCE 200809L

#include "../lexer/lexer.h"
#include "parse_negation.h"
struct treeInput
{
    struct ast *tree_list;
};
struct treeList
{
    struct ast *tree_andor;
};
struct treeFor
{
    char *variable;
    char **word_list;
    struct ast *tree_do;
};
struct treeFunc
{
    char *name;
    struct ast *shell_command;
};
struct treeAndOr
{
    struct ast *commande_left;
    struct ast *commande_right;
};
struct treePipe
{
    struct ast *commande_left;
    struct ast *commande_right;
};
struct condition
{
    struct ast **tree_compoundlist;
};

struct treeThen
{
    struct ast *command;
};
struct treeElse
{
    struct ast *command;
};
struct treeIf
{
    struct ast *condition;
    struct ast *tree_then;
    struct ast *tree_else;
};
struct treeCompoundlist
{
    struct ast **command;
};

struct treeSimpleCommand
{
    char **argument;
    char **redirections;
    char **variable;
    size_t taille_tableau;
};

struct treeShellCommand
{
    struct treeIF *treeif;
};

struct treeWhile
{
    struct ast *condition;
    struct ast *tree_do;
};
struct treeUntil
{
    struct ast *condition;
    struct ast *tree_do;
};

struct treeNegation
{
    struct ast *my_tree;
};

enum ast_type
{
    AST_PIPE,
    AST_COMPOUND,
    AST_IF,
    AST_LIST,
    AST_SIM_CMD,
    AST_NEGATION,
    AST_WHILE,
    AST_UNTIL,
    AST_AND,
    AST_OR,
    AST_FUNC,
    AST_FOR
};
union ast_union
{
    struct treeFunc *ast_func;
    struct treePipe *ast_pipe;
    struct treeCompoundlist *ast_compound;
    struct treeList *ast_list;
    struct treeIf *ast_if;
    struct treeSimpleCommand *ast_simple_cmd;
    struct treeNegation *ast_negation;
    struct treeUntil *ast_until;
    struct treeWhile *ast_while;
    struct treeAndOr *ast_and_or;
    struct treeFor *ast_for;
};

struct ast
{
    enum ast_type type;
    union ast_union data;
    size_t nb_children;
    struct ast **children;
};

int parse_element(token_t **token);
struct ast *parse_shell_command(token_t **token);
struct ast *parse_command(token_t **token);
struct ast *parse_input(void);

#endif /* ! AST_H */
