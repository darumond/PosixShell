#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <ctype.h>
#include <err.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct variable
{
    char *key;
    char *value;
    struct variable *next;
};

typedef struct hash_map_t
{
    struct variable **data;
    size_t size;
} hash_map_t;

// variable
hash_map_t *hash_map_init(size_t size);
void hash_map_free(hash_map_t *hash_map);
void hash_map_dump(hash_map_t *hash_map);

// hashmap fonction (tools)
size_t hash(char *str);
bool hash_map_insert(hash_map_t *hash_map, char *key, char *value,
                     bool *updated);
char *hash_map_get(const hash_map_t *hash_map, char *key);
bool hash_map_remove(hash_map_t *hash_map, char *key);
bool hash_map_is_present(const hash_map_t *hash_map, char *key);

#endif /* ! HASH_MAP_H */
