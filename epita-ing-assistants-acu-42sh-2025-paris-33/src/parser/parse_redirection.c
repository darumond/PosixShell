#include "parse_redirection.h"

token_t **handle_redir(struct ast *tree_simple_command, token_t **token,
                       int len)
{
    size_t i = len;
    while ((*token)->type_token != ENDOFFILE)
    {
        my_strcpy(tree_simple_command->data.ast_simple_cmd->redirections[i],
                  (*token)->value);

        if ((*token)->type_token == WORDS)
        {
            if ((*token)->newline || (*token)->semicolon)
                return token;
            my_freetoken(token);
            *token = send_token();
            if ((*token)->type_token == WORDS)
                return token;
            else
            {
                *token = my_skip_space(token);
                i++;
                continue;
            }
        }

        if ((*token)->type_token == REDIRECTION
            && ((*token)->newline || (*token)->semicolon))
        {
            args->exit_status = 2;
            if ((*token)->newline)
                errx(2, "syntax error near unexpected token '%s' ", "newline");
            errx(2, "syntax error near unexpected token '%s' ", ";");
        }

        if ((*token)->newline || (*token)->semicolon
            || (*token)->type_token == PIPE)
        {
            break;
        }

        my_freetoken(token);
        *token = send_token();
        *token = my_skip_space(token);
        i++;
        if ((*token)->type_token == ENDOFFILE)
        {
            args->exit_status = 2;
            errx(2, "Unexpected end of file");
        }
    }
    return token;
}
