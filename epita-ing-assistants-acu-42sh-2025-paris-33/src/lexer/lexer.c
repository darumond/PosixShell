#include "lexer.h"

#include <ctype.h>

#include "../io_backend/io_backend.h"
#include "../tools/utils.h"

static token_t *create_token(void)
{
    token_t *token = calloc(1, sizeof(token_t));
    if (token == NULL)
        errx(1, "calloc failed !");

    token->value = calloc(100, sizeof(char));
    if (token->value == NULL)
        errx(1, "calloc failed !");

    return token;
}

static int is_delim(char c)
{
    if (c != ' ' && c != EOF && c != '\n' && c != ';')
        return 1;
    return 0;
}

static int is_assign_word(char *word)
{
    // if the first character is a digit
    if (!isalpha(word[0]))
        return 1;

    for (size_t i = 1; word[i] && word[i] != '='; i++)
    {
        if (!isalpha(word[i]) && !isdigit(word[i]) && word[i] != '_')
            return 1;
    }

    // We have to check if there a sign &apos;=&apos;
    for (size_t i = 0; word[i] != '\0'; i++)
    {
        if (word[i] == '=')
            return 0;
    }

    return 1;
}

static int sub_handle_type_token(token_t *new_token, int res)
{
    if (strcmp(new_token->value, "if") == 0)
    {
        new_token->type_token = IF;
        new_token->reserved_word = true;
    }
    else if (strcmp(new_token->value, "then") == 0)
    {
        new_token->type_token = THEN;
        new_token->reserved_word = true;
    }
    else if (strcmp(new_token->value, "else") == 0)
    {
        new_token->type_token = ELSE;
        new_token->reserved_word = true;
    }
    else if (strcmp(new_token->value, "for") == 0)
    {
        new_token->type_token = FOR;
        new_token->reserved_word = true;
    }
    else if (strcmp(new_token->value, "in") == 0)
    {
        new_token->type_token = IN;
        new_token->reserved_word = true;
    }
    else if (strcmp(new_token->value, "!") == 0)
    {
        new_token->type_token = NEGATION;
        new_token->reserved_word = true;
    }
    else if (strcmp(new_token->value, "elif") == 0)
    {
        new_token->type_token = ELIF;
        new_token->reserved_word = true;
    }
    else if (strcmp(new_token->value, "fi") == 0)
    {
        new_token->type_token = FI;
        new_token->reserved_word = true;
    }
    else if (strcmp(new_token->value, "while") == 0)
    {
        new_token->type_token = WHILE;
        new_token->reserved_word = true;
    }
    else
        res = -1;
    return res;
}

static void handle_type_token(token_t *new_token)
{
    int res = 0;
    res = sub_handle_type_token(new_token, res);
    if (res == 0)
        return;
    else if (strcmp(new_token->value, "until") == 0)
    {
        new_token->type_token = UNTIL;
        new_token->reserved_word = true;
    }
    else if (strcmp(new_token->value, "do") == 0)
    {
        new_token->type_token = DO;
        new_token->reserved_word = true;
    }
    else if (strcmp(new_token->value, "done") == 0)
    {
        new_token->type_token = DONE;
        new_token->reserved_word = true;
    }
    else if (is_assign_word(new_token->value) == 0)
    {
        new_token->type_token = ASSIGNMENT_WORD;
    }
    else
    {
        new_token->type_token = WORDS;
    }
}

static token_t *token_return(token_t *new_token, char c)
{
    if (new_token->value[0] == '&')
    {
        new_token->type_token = AND;
        return new_token;
    }
    if (strcmp(new_token->value, "|") == 0
        || strcmp(new_token->value, "||") == 0)
    {
        if (new_token->value[1] == '|')
            new_token->type_token = OR;
        else
            new_token->type_token = PIPE;
        return new_token;
    }

    if (c == ';')
    {
        new_token->semicolon = true;
        new_token->type_token = WORDS;
        handle_type_token(new_token);
        return new_token;
    }
    if (c == '\'')
    {
        new_token->type_token = BETWEEN_QUOTES;
        return new_token;
    }
    if (strcmp(new_token->value, "") == 0 && c == '\n')
    {
        new_token->type_token = NEWLINE;
        new_token->newline = true;
        return new_token;
    }
    if (strcmp(new_token->value, "") != 0)
    {
        if (c == '\n')
        {
            new_token->newline = true;
            handle_type_token(new_token);
            return new_token;
        }
        handle_type_token(new_token);
        return new_token;
    }
    if (c == EOF)
    {
        new_token->value[0] = c;
        new_token->type_token = ENDOFFILE;
        return new_token;
    }

    new_token->type_token = SPACE;
    return new_token;
}
// si une single quote est detecter , met tout dans le char dst jusqu'a la
// prochaine single quote
// return le nb de fois qu'on a ecrit dans le token pour les prochains
// remplissages
static int absorb_single_quote(token_t *token, int i)
{
    char c;
    token->value[i] = '\'';
    i++;
    c = get_new_char();
    while (c != '\'' && c != EOF)
    {
        if (i > 15)
        {
            size_t taille_initiale = strlen(token->value);
            token->value = realloc(token->value, i + 20);
            memset(token->value + taille_initiale, 0, i + 20 - taille_initiale);
        }
        token->value[i] = c;
        i++;
        c = get_new_char();
    }
    if (c == EOF)
        err(2, "erreur");

    token->value[i] = c;
    return i + 1;
}
// si une double quote est detecter , met tout dans le char dst jusqu'a la
// prochaine double quote
// return le nb de fois qu'on a ecrit dans le token pour les prochains
// remplissages
static int absorb_double_quote(token_t *token, int i)
{
    char c;
    token->value[i] = '\"';
    i++;
    c = get_new_char();
    while (c != '\"' && c != EOF)
    {
        if (i > 15)
        {
            size_t taille_initiale = strlen(token->value);
            token->value = realloc(token->value, i + 20);
            memset(token->value + taille_initiale, 0, i + 20 - taille_initiale);
        }
        if (c == '\\')
        {
            token->value[i] = c;
            i++;
            c = get_new_char();
            token->value[i] = c;
            i++;
            c = get_new_char();
        }
        else
        {
            token->value[i] = c;
            i++;
            c = get_new_char();
        }
    }
    if (c == EOF)
        err(2, "erreur");
    token->value[i] = c;
    return i + 1;
}

static token_t *handle_and(token_t *new_token, char c)
{
    new_token->value[0] = c;
    char t = get_new_char();
    if (t != '&')
        exit(127);
    new_token->value[1] = t;
    return token_return(new_token, c);
}

static token_t *handle_pipe_or(token_t *new_token, char c)
{
    new_token->value[0] = c;
    char t = get_new_char();
    if (t != '|')
        unget_new_char(t);
    else
        new_token->value[1] = t;
    return token_return(new_token, c);
}

static token_t *handle_arrow_right(token_t *new_token, char c, int i)
{
    new_token->value[i] = c;
    new_token->type_token = REDIRECTION;
    i++;
    c = get_new_char();
    if (c == '>' || c == '&' || c == '|')
    {
        new_token->value[i] = c;
        c = get_new_char();
    }
    else
        unget_new_char(c);
    if (c == '\n')
        new_token->newline = true;
    if (c == ';')
        new_token->semicolon = true;
    return new_token;
}

static token_t *handle_parenthesis(token_t *new_token, char c)
{
    new_token->value[0] = c;
    new_token->type_token = PARENTHESIS;
    if (c == ')')
        return new_token;
    c = get_new_char();
    if (c != ')')
        err(2, "erreur");
    unget_new_char(c);
    return new_token;
}

static token_t *handle_left_arrow(token_t *new_token, char c, int i)
{
    new_token->value[i] = c;
    new_token->type_token = REDIRECTION;
    i++;
    c = get_new_char();

    if (c == '>' || c == '&')
    {
        new_token->value[i] = c;
        c = get_new_char();
    }
    else
        unget_new_char(c);
    if (c == '\n')
        new_token->newline = true;
    if (c == ';')
        new_token->semicolon = true;
    return new_token;
}

static token_t *sub_handle_character(char c, token_t *new_token, int i)
{
    if (c == '(' || c == ')')
    {
        unget_new_char(c);
        return token_return(new_token, c);
    }
    if (c == '&')
    {
        new_token->value[i] = c;
        i++;
        c = get_new_char();
        if (c == '>' || c == '<')
        {
            if (c == '>')
                return handle_arrow_right(new_token, c, i);

            if (c == '<')
                return handle_left_arrow(new_token, c, i);
        }
        unget_new_char(c);
        return handle_and(new_token, c);
    }

    if (c == '|')
    {
        return handle_pipe_or(new_token, c);
    }

    if (c == '>')
    {
        return handle_arrow_right(new_token, c, i);
    }
    if (c == '<')
    {
        return handle_left_arrow(new_token, c, i);
    }
    return NULL;
}

static token_t *handle_character(char c, token_t *new_token, int i) // 54 lignes
{
    while (is_delim(c) == 1)
    {
        if (c == '\\')
        {
            new_token->value[i] = c;
            i++;
            c = get_new_char();
            if (c == '\n')
            {
                c = get_new_char();
                continue;
            }
            else
            {
                new_token->value[i] = c;
                i++;
                c = get_new_char();
                continue;
            }
        }
        if (c == '\'') // take everything in single quote
        {
            i = absorb_single_quote(new_token, i);
            c = get_new_char();
            continue;
        }
        if (c == '\"')
        {
            i = absorb_double_quote(new_token, i);
            c = get_new_char();
            continue;
        }
        token_t *tmp = sub_handle_character(c, new_token, i);
        if (tmp != NULL)
            return tmp;
        if (i > 15)
        {
            size_t taille_initiale = strlen(new_token->value);
            new_token->value = realloc(new_token->value, i + 20);
            memset(new_token->value + taille_initiale, 0,
                   i + 20 - taille_initiale);
        }
        new_token->value[i] = c;
        i++;
        c = get_new_char();
        if (c == '|')
        {
            unget_new_char(c);
            return token_return(new_token, c);
        }
    }
    return token_return(new_token, c);
}

static bool my_isdigit(char c)
{
    if (c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5'
        || c == '6' || c == '7' || c == '8' || c == '9')
        return true;
    return false;
}

static token_t *sub_send_token(token_t *new_token, int i, char c)
{
    if (my_isdigit(c))
    {
        new_token->value[i] = c;
        i++;
        c = get_new_char();
        if (c == '>' || c == '<')
        {
            new_token->type_token = REDIRECTION;
            unget_new_char(c);
            return new_token;
        }
        if (c == ' ')
        {
            new_token->type_token = WORDS;
            unget_new_char(c);
            return new_token;
        }

        while (is_delim(c) == 1)
        {
            new_token->value[i] = c;
            i++;
            c = get_new_char();
            if (i > 18)
            {
                size_t taille_initiale = strlen(new_token->value);
                new_token->value = realloc(new_token->value, i + 20);
                memset(new_token->value + taille_initiale, 0,
                       i + 20 - taille_initiale);
            }
        }
        new_token->type_token = WORDS;
        unget_new_char(c);
        return new_token;
    }
    if (c == '(' || c == ')')
    {
        return handle_parenthesis(new_token, c);
    }
    if (is_delim(c) == 0)
    {
        return token_return(new_token, c);
    }
    return NULL;
}

// This function will return token by token to the parser
token_t *send_token(void)
{
    token_t *new_token = create_token();
    int i = 0;
    char c = get_new_char(); // We retrieve the first character from the stream
    // char first_digit = c;
    // int inside_loop = 0;
    while (1)
    {
        if (c == ' ' && i != 0)
            return token_return(new_token, c);
        while (c == ' ') // skip space
            c = get_new_char();

        if (c == '#')
        {
            while (c != '\n' && c != EOF)
            {
                c = get_new_char();
            }
        }
        if (c == '\'') // take everything in single quote
        {
            i = absorb_single_quote(new_token, i);
            c = get_new_char();
            continue;
        }
        if (c == '\"')
        {
            i = absorb_double_quote(new_token, i);
            c = get_new_char();
            continue;
        }
        token_t *tmp = sub_send_token(new_token, i, c);
        if (tmp != NULL)
            return tmp;
        else
        {
            return handle_character(c, new_token, i);
        }
    }
}
