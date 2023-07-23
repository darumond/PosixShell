#include "hash_map_func.h"

size_t hash_func(char *key)
{
    size_t i = 0;
    size_t hash = 0;

    for (i = 0; key[i] != '\0'; ++i)
        hash += key[i];
    hash += i;

    return hash;
}

hash_map_func_t *hash_map_init_func(size_t size)
{
    hash_map_func_t *new = calloc(1, sizeof(hash_map_func_t));
    if (new == NULL)
        return NULL;
    new->size = size;
    new->data = malloc(sizeof(struct function) * new->size);
    if (new->data == NULL)
        return NULL;
    for (size_t i = 0; i < new->size; i++)
        new->data[i] = NULL;
    return new;
}

bool hash_map_insert_func(hash_map_func_t *hash_map, char *key,
                          struct ast *value, bool *updated)
{
    if (hash_map == NULL || hash_map->size == 0)
        return false;
    hash_map_func_t *map = hash_map;
    size_t taille_map = 0;
    while (taille_map < map->size)
    {
        struct function *pair = map->data[taille_map];
        if (pair && strcmp(pair->key, key) == 0)
        {
            if (pair->key)
            {
                free(pair->key);
            }
            if (pair->value)
            {
                free_ast(pair->value);
            }

            pair->key = key;
            pair->value = value;
            *updated = true;
            return updated;
        }
        else
            taille_map++;
    }
    size_t res_hash = hash_func(key) % map->size;
    struct function *newpair = calloc(1, sizeof(struct function));
    if (newpair == NULL)
        return false;
    newpair->key = key;
    newpair->value = value;
    newpair->next = NULL;
    if (map->data[res_hash] != NULL)
    {
        struct function *tmp = map->data[res_hash];
        map->data[res_hash] = newpair;
        newpair->next = tmp;
    }
    else
        map->data[res_hash] = newpair;
    *updated = false;
    return updated;
}

void hash_map_free_func(hash_map_func_t *hash_map)
{
    if (hash_map == NULL)
        return;
    hash_map_func_t *map = hash_map;
    size_t i = 0;
    while (i < map->size)
    {
        struct function *pair = map->data[i];
        if (pair == NULL)
        {
            i++;
            continue;
        }
        while (pair->next)
        {
            struct function *save = pair->next;
            if (pair->key)
                free(pair->key);
            if (pair->value)
                free_ast(pair->value);
            free(pair);
            pair = save;
        }
        if (pair->key)
            free(pair->key);
        if (pair->value)
            free_ast(pair->value);
        if (pair)
        {
            free(pair);
        }
        i++;
    }
    free(map->data);
    free(map);
}

bool hash_map_is_present_func(const hash_map_func_t *hash_map, char *key)
{
    if (hash_map == NULL || hash_map->size == 0)
        return false;
    size_t index = hash_func(key) % hash_map->size;
    if (!hash_map->data[index])
        return false;
    struct function *pair = hash_map->data[index];
    while (pair)
    {
        if (strcmp(pair->key, key) == 0)
            return true;
        pair = pair->next;
    }
    return false;
}

struct ast *hash_map_get_func(const hash_map_func_t *hash_map, char *key)
{
    if (hash_map == NULL || hash_map->size == 0)
        return NULL;
    size_t index = hash_func(key) % hash_map->size;
    if (!hash_map->data[index])
        return NULL;
    struct function *pair = hash_map->data[index];
    while (pair)
    {
        if (strcmp(pair->key, key) == 0)
            return pair->value;
        pair = pair->next;
    }
    return NULL;
}

bool hash_map_remove_func(hash_map_func_t *hash_map, char *key)
{
    if (hash_map == NULL)
        return false;
    hash_map_func_t *map = hash_map;
    size_t i = 0;
    int next = 0;
    while (i < map->size)
    {
        struct function *pair = map->data[i];
        if (pair)
        {
            if (strcmp(pair->key, key) == 0)
            {
                struct function *save = pair->next;
                free_ast(pair->value);
                free(pair->key);
                free(pair);
                map->data[i] = save;
                return true;
            }
            else

                while (pair->next)
                {
                    next = 1;
                    if (strcmp(pair->next->key, key) == 0)
                    {
                        struct function *save = pair->next->next;
                        free(pair->next->key);
                        free_ast(pair->next->value);
                        free(pair->next);
                        pair->next = save;
                        return true;
                    }
                    pair = pair->next;
                }
            if ((next = 1) && (strcmp(pair->key, key) == 0))
            {
                free(pair->key);
                free_ast(pair->value);
                free(pair);
                return true;
            }
        }
        i++;
    }
    return false;
}
