#ifndef LEXER_H
#define LEXER_H

#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

enum type_token
{
    DEFAULT = 0,
    IF,
    THEN,
    ELIF,
    ELSE,
    FI,
    ENDOFFILE,
    SEMICOLON,
    NEWLINE,
    APOSTROPHE,
    SPACE,
    WORDS,
    BETWEEN_QUOTES,
    REDIRECTION,
    WHILE,
    DONE,
    DO,
    UNTIL,
    PIPE,
    NEGATION,
    OR,
    AND,
    FOR,
    IN,
    ASSIGNMENT_WORD,
    BRACKETS,
    PARENTHESIS
};

typedef struct token
{
    char *value;
    enum type_token type_token;
    bool semicolon;
    bool newline;
    bool reserved_word;
    bool parenthesis; // to know if there is a parenthesis at the right of the
                      // token
} token_t;

// token_t *create_token(void);
token_t *send_token(void);
// int is_assign_word(char *word);
// token_t *handle_parenthesis(token_t *new_token, char c);

#endif /* ! LEXER_H */
