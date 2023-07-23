#ifndef HASH_MAP_FUNC_H
#define HASH_MAP_FUNC_H

#include <ctype.h>
#include <err.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../parser/ast.h"
#include "../tools/utils.h"

struct function
{
    char *key;
    struct ast *value;
    struct function *next;
};

typedef struct hash_map_func_t
{
    struct function **data;
    size_t size;
} hash_map_func_t;

// variable
hash_map_func_t *hash_map_init_func(size_t size);
void hash_map_free_func(hash_map_func_t *hash_map);
// void hash_map_dump_func(hash_map_func_t *hash_map);

// hashmap fonction (tools)
size_t hash_func(char *str);
bool hash_map_insert_func(hash_map_func_t *hash_map, char *key,
                          struct ast *value, bool *updated);
struct ast *hash_map_get_func(const hash_map_func_t *hash_map, char *key);
bool hash_map_remove_func(hash_map_func_t *hash_map, char *key);
bool hash_map_is_present_func(const hash_map_func_t *hash_map, char *key);

#endif /* ! HASH_MAP_FUNC_H */
