#include "exec/exec.h"
#include "exec/exec_header.h"
#include "exec/hash_map_func.h"
#include "io_backend/io_backend.h"
#include "lexer/lexer.h"
#include "parser/ast.h"
#include "tools/utils.h"

struct args *args = NULL;

int main(int argc, char *argv[])
{
    args = calloc(1, sizeof(args_t));
    args->argv = argv;
    args->argc = argc;
    file_convert(argc, argv);
    args->mul_function = 0;
    struct ast *new = NULL;
    int res;

    args->variable = hash_map_init(10);
    args->function = hash_map_init_func(10);

    // ajout de la variable PATH
    bool b = false;
    hash_map_insert(args->variable, "PATH", getenv("PATH"), &b);
    args->node_list_func = calloc(10, sizeof(struct ast *));
    while ((new = parse_input()) != NULL)
    {
        res = ast_exec(new->children[0], new);
        args->exit_status = res;
        if (new->children[0]->type != AST_FUNC)
            free_ast(new);
        // else if(args->reference_counting > 0)
        // {
        //     args->reference_counting--;
        // }
        else
        {
            *args->node_list_func = new;
            args->node_list_func++;
        }
        args->mul_function = 0;
    }

    // free_ast(new);

    free(new);
    while (*args->node_list_func)
    {
        free_ast(*args->node_list_func);
        args->node_list_func++;
    }
    // hash_map_free_func(args->function);
    if (res >= 100000001 || res <= -100000001)
    {
        return 0;
    }
    return res;
}
