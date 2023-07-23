#ifndef BUILTIN_H
#define BUILTIN_H

#include <errno.h>

#include "../exec/exec_header.h"
#include "../parser/ast.h"
#include "../tools/tools.h"

// fonctions
void my_echo(struct treeSimpleCommand *command);
int my_dot(char *file, FILE *fd);
int my_unset(struct treeSimpleCommand *command);
int my_export(struct treeSimpleCommand *command);
void update_old_pwd();
void update_pwd();
int my_cd(struct treeSimpleCommand *cmd);

#endif /* ! BUILTIN_H */
