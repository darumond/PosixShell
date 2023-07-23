#include "io_backend.h"

#include <stdio.h>

#include "../tools/tools.h"

void file_convert(int argc, char *argv[])
{
    // printf("%s    %s\n",argv[0],argv[1]);

    if (argc >= 3 && strcmp(argv[1], "-c") == 0)
    {
        args->fp = fmemopen(argv[2], strlen(argv[2]), "r");
        if (args->fp == NULL)
        {
            perror("fmemopen failed");
            exit(EXIT_FAILURE);
        }
        return;
    }

    if (argc == 1)
    {
        args->fp = fdopen(STDIN_FILENO, "r");
        if (args->fp == NULL)
        {
            perror("fdopen failed");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        args->fp = fopen(argv[1], "r");
        if (args->fp == NULL)
        {
            perror("fopen failed");
            exit(EXIT_FAILURE);
        }
        args->argument = argv + 2;
        args->size_arg = argc - 2;
        // int var = 5;
        // printf("%s\n", args[2]);
        // printf("%d\n", var);
        // ici tu met a jour ta structure d'argv
    }
}

char get_new_char(void)
{
    if (args->fp == NULL)
    {
        exit(2);
    }

    return fgetc(args->fp);
}

char unget_new_char(char c)
{
    if (args->fp == NULL)
    {
        exit(2);
    }
    return ungetc(c, args->fp);
}

void close_file(void)
{
    if (args->fp != NULL)
    {
        fclose(args->fp);
    }
}
