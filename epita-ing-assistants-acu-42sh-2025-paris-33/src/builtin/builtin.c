#include "builtin.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void handle_option_e(char *str)
{
    int i;
    for (i = 0; str[i] != '\0'; i++)
    {
        if (str[i + 1] && (str[i] == '\\' && str[i + 1] == 'n'))
        {
            printf("\n");
            i++;
        }
        else if (str[i + 1] && (str[i] == '\\' && str[i + 1] == 't'))
        {
            printf("\t");
            i++;
        }
        // gestion de l'echapement (non tester)
        else if (str[i + 1] && (str[i] == '\\' && str[i + 1] == '\\'))
        {
            printf("\\");
            i++;
        }
        else
        {
            printf("%c", str[i]);
        }
    }
}

int my_unset(struct treeSimpleCommand *command)
{
    int v = 0;
    int f = 0;
    int nboption = 0;
    for (int i = 1; strcmp(command->argument[i], "") != 0
         || strcmp(command->argument[i + 1], "") != 0;
         i++)
    {
        if (strcmp(command->argument[i], "-f") == 0)
        {
            f = 1;
            nboption++;
        }
        if (strcmp(command->argument[i], "-v") == 0)
        {
            v = 1;
            nboption++;
        }
        if (nboption > 1)
            err(1, "error");
    }
    int j = nboption + 1;
    for (; strcmp(command->argument[j], "") != 0
         || strcmp(command->argument[j + 1], "") != 0;
         j++)
    {
        if (f == 1)
        {
            hash_map_remove_func(args->function, command->argument[j]);
        }
        if (v == 1 || (f == 0))
        {
            hash_map_remove(args->variable, command->argument[j]);
        }
    }
    return 0;
}

int compte_occurence(char *chaine, char caractere)
{
    int compteur = 0;
    for (size_t i = 0; i < strlen(chaine); i++)
    {
        {
            if (chaine[i] == caractere)
                compteur++;
        }
    }
    return compteur;
}
static int my_dot_handle_else(struct ast *new, int res, char *new_path,
                              FILE *fd)
{
    while ((new = parse_input()) != NULL)
    {
        res = ast_exec(new->children[0], new);
        args->exit_status = res;
        free_ast(new);
    }
    free(new_path);
    close_file();
    args->fp = fd;
    return res;
}

static int my_dot_handle_return(struct ast *new, int res, char *file, FILE *fd)
{
    args->fp = fopen(file, "r");
    if (args->fp == NULL)
    {
        perror("fopen failed");
        exit(EXIT_FAILURE);
    }
    while ((new = parse_input()) != NULL)
    {
        res = ast_exec(new->children[0], new);
        args->exit_status = res;
        free_ast(new);
    }
    close_file();
    args->fp = fd;
    return res;
}

int my_dot(char *file, FILE *fd)
{
    int res = 0;
    struct ast *new = NULL;
    if (file[0] != '.' && file[1] != '\\')
    {
        char *my_path = hash_map_get(args->variable, "PATH");
        char *new_path = calloc(100, 1);
        int times = compte_occurence(my_path, ':');
        int i = 0;
        int j = 0;
        while (times > 0)
        {
            while (my_path[i] != ':')
            {
                new_path[i] = my_path[j];
                i++;
                j++;
            }
            args->fp = fopen(new_path, "r");
            if (args->fp == NULL && times == 0)
            {
                perror("fopen failed");
                free(new_path);
                err(1, "error");
            }
            if (args->fp == NULL && times != 0)
            {
                times--;
                memset(new_path, 0, 100);
            }
            else
            {
                return my_dot_handle_else(new, res, new_path, fd);
            }
        }
    }
    return my_dot_handle_return(new, res, file, fd);
}
void my_echo(struct treeSimpleCommand *command)
{
    int do_newline = 1;
    int do_escape = 0;
    int nb_option = 1; // 1 pour echo premiÃ¨re commande

    // Dans quelle methode on est ?
    for (int i = 1; strcmp(command->argument[i], "") != 0; i++)
    {
        if (strcmp(command->argument[i], "-E") == 0)
        {
            do_escape = 0;
            nb_option++;
        }
        if (strcmp(command->argument[i], "-n") == 0)
        {
            do_newline = 0;
            nb_option++;
        }
        if (strcmp(command->argument[i], "-e") == 0)
        {
            do_escape = 1;
            nb_option++;
        }
    }

    if (do_escape == 0) // -E
    {
        for (int i = nb_option; strcmp(command->argument[i], "") != 0; i++)
        {
            printf("%s", command->argument[i]);
            if (strcmp(command->argument[i + 1], "") != 0)
                printf(" ");
        }
        if (do_newline == 1) // pas de -n
            printf("\n");
    }
    if (do_escape == 1)
    {
        for (int i = nb_option; strcmp(command->argument[i], "") != 0; i++)
        {
            handle_option_e(command->argument[i]);
            if (strcmp(command->argument[i + 1], "") != 0)
                printf(" ");
        }
        if (do_newline == 1) // pas de -n
            printf("\n");
    }
}

int my_export(struct treeSimpleCommand *command)
{
    bool b = false;
    int times = compte_occurence(command->argument[1], '=');
    char *my_key = calloc(100, 1);
    char *my_value = calloc(100, 1);
    size_t j = 0;
    size_t k = 0;
    if (times == 1)
    {
        while (command->argument[1][j] != '=')
        {
            my_key[j] = command->argument[1][j];
            j++;
        }
        j++;
        while (command->argument[1][j] != '\0')
        {
            my_value[k] = command->argument[1][j];
            j++;
            k++;
        }
        setenv(my_key, my_value, 1);
        hash_map_insert(args->variable, my_key, my_value, &b);
    }
    else
    {
        if (hash_map_is_present(args->variable, command->argument[1]))
        {
            char *value = hash_map_get(args->variable, command->argument[1]);
            setenv(command->argument[1], value, 1);
        }
        else
        {
            setenv(command->argument[1], "", 1);
        }
    }
    return 0;
}

void update_old_pwd()
{
    char *old_pwd = NULL;
    old_pwd = getcwd(old_pwd, 1024);
    if (old_pwd == NULL)
        errx(1, "could not determined the current working directory!");

    if (setenv("OLDPWD", old_pwd, 1) == -1)
        errx(1, "could not set the value of OLDPWD");
    free(old_pwd);
}

void update_pwd()
{
    char *new_pwd = NULL;
    new_pwd = getcwd(
        new_pwd, 1024); // this function will allocate memory for buffer new_pwd
    if (new_pwd == NULL)
        errx(1, "could not determined the current working directory!");

    if (setenv("PWD", new_pwd, 1) == -1)
        errx(1, "could not set the value of PWD");
    free(new_pwd);
}
int my_cd(struct treeSimpleCommand *cmd)
{
    if (len_arg(cmd->argument) > 2)
        errx(1, "cd: too many arguments");

    char *path = NULL;

    if (len_arg(cmd->argument) == 1) // in the case where no argument
                                     // is passed to the cd command
    {
        char *home = getenv("HOME");
        if (home == NULL)
            errx(1, "cd: HOME not set");
        update_old_pwd();
        if (chdir(home) == -1) // here we execute the cd command for the calling
                               // process in our case our 42sh executable
            errx(1, "can't change the current directory!");
        update_pwd();
    }

    if (strcmp(cmd->argument[1], "-") == 0)
    {
        char *old_pwd = getenv("OLDPWD");
        if (old_pwd == NULL)
            errx(1, "cd: OLDPWD not set");
        update_old_pwd();
        if (chdir(old_pwd) == -1)
            errx(1, "can't change the current directory!");

        update_pwd();
        printf("%s\n", old_pwd);
    }

    else if (len_arg(cmd->argument) == 2)
    {
        path = calloc(strlen(cmd->argument[1]) + 1, sizeof(char));
        path = my_strcpy(path, cmd->argument[1]);
        update_old_pwd();

        if (chdir(path) == -1)
        {
            free(path);
            if (errno == ENOENT)
                errx(1, "cd: %s: No such file or directory!", cmd->argument[1]);
            if (errno == ENOTDIR)
                errx(1, "cd: %s: one component in the path is not a directory!",
                     cmd->argument[1]);
        }

        update_pwd();
        free(path);
    }
    return 0;
}
