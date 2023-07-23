#include "tools.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "parser/parse_simple_command.h"

char *my_strcpy_simple_command(char *dst, char *src, struct ast *my_ast)
{
    size_t taille_tableau = my_ast->data.ast_simple_cmd->taille_tableau;
    size_t i = 0;
    for (; src[i] != '\0'; i++)
    {
        if (i == taille_tableau - 2)
        {
            dst = realloc(dst, taille_tableau + 100);
            taille_tableau += 100;
        }
        dst[i] = src[i];
    }
    dst[i] = '\0';
    return dst;
}
char *my_strcpy(char *dst, char *src)
{
    size_t i = 0;
    for (; src[i] != '\0'; i++)
    {
        if (i > 90)
        {
            size_t taille_initiale = strlen(dst);
            dst = realloc(dst, i + 10);
            memset(dst + taille_initiale, 0, i + 10 - taille_initiale);
        }
        dst[i] = src[i];
    }
    dst[i] = '\0';
    return dst;
}

int len_arg(char **str)
{
    int i = 0;
    if (!str)
        return 0;
    while (str[i] && strcmp(str[i], "") != 0)
        i++;
    return i;
}

void check_allocate(void *ptr)
{
    if (ptr == NULL)
        errx(1, "Allocation of memory failed !");
}

bool my_is_digit(char *str)
{
    if (strcmp(str, "1") != 0 && strcmp(str, "2") != 0 && strcmp(str, "3") != 0
        && strcmp(str, "4") != 0 && strcmp(str, "5") != 0
        && strcmp(str, "6") != 0 && strcmp(str, "7") != 0
        && strcmp(str, "8") != 0 && strcmp(str, "9") != 0)
    {
        return false;
    }
    return true;
}

char *int_to_str(int i)
{
    char *str;
    int length = 0;
    int temp = i;

    while (temp != 0)
    {
        length++;
        temp /= 10;
    }

    str = (char *)malloc((length + 1) * sizeof(char));
    if (str == NULL)
    {
        printf("Erreur d'allocation mémoire");
        exit(1);
    }

    str[length] = '\0';
    while (length--)
    {
        str[length] = (i % 10) + '0';
        i /= 10;
    }
    return str;
}
char **init_table(void)
{
    char **list = calloc(100, sizeof(char *));

    for (size_t i = 0; i < 100; i++)
    {
        list[i] = calloc(100, sizeof(char));
    }
    return list;
}
void copy_matrix(char **dst, char **src)
{
    size_t i = 0;
    while (src[i] != NULL && strcmp(src[i], "") != 0)
    {
        my_strcpy(dst[i], src[i]);
        i++;
    }
}
void free_table(char **list)
{
    for (size_t i = 0; i < 100; i++)
    {
        free(list[i]);
    }
    free(list);
}
