#include "hash_map.h"

size_t hash(char *key)
{
    size_t i = 0;
    size_t hash = 0;

    for (i = 0; key[i] != '\0'; ++i)
        hash += key[i];
    hash += i;

    return hash;
}

hash_map_t *hash_map_init(size_t size)
{
    hash_map_t *new = malloc(sizeof(hash_map_t));
    if (new == NULL)
        return NULL;
    new->size = size;
    new->data = malloc(sizeof(struct variable) * new->size);
    if (new->data == NULL)
        return NULL;
    for (size_t i = 0; i < new->size; i++)
        new->data[i] = NULL;
    return new;
}

bool hash_map_insert(hash_map_t *hash_map, char *key, char *value,
                     bool *updated)
{
    if (hash_map == NULL || hash_map->size == 0)
        return false;
    hash_map_t *map = hash_map;
    size_t taille_map = 0;
    while (taille_map < map->size)
    {
        struct variable *pair = map->data[taille_map];
        if (pair && strcmp(pair->key, key) == 0)
        {
            if (pair->key)
            {
                free(pair->key);
            }
            if (pair->value)
            {
                free(pair->value);
            }

            pair->key = key;
            pair->value = value;
            *updated = true;
            return updated;
        }
        else
            taille_map++;
    }
    size_t res_hash = hash(key) % map->size;
    struct variable *newpair = malloc(sizeof(struct variable));
    if (newpair == NULL)
        return false;
    newpair->key = key;
    newpair->value = value;
    newpair->next = NULL;
    if (map->data[res_hash] != NULL)
    {
        struct variable *tmp = map->data[res_hash];
        map->data[res_hash] = newpair;
        newpair->next = tmp;
    }
    else
        map->data[res_hash] = newpair;
    *updated = false;
    return updated;
}

void hash_map_free(hash_map_t *hash_map)
{
    if (hash_map == NULL)
        return;
    hash_map_t *map = hash_map;
    size_t i = 0;
    while (i < map->size)
    {
        struct variable *pair = map->data[i];
        if (pair == NULL)
        {
            i++;
            continue;
        }
        while (pair->next)
        {
            struct variable *save = pair->next;
            if (pair->key)
                free(pair->key);
            if (pair->value)
                free(pair->value);
            free(pair);
            pair = save;
        }
        if (pair->key)
            free(pair->key);
        if (pair->value)
            free(pair->value);
        if (pair)
        {
            free(pair);
        }
        i++;
    }
    free(map->data);
    free(map);
}

bool hash_map_is_present(const hash_map_t *hash_map, char *key)
{
    if (hash_map == NULL || hash_map->size == 0)
        return false;
    size_t index = hash(key) % hash_map->size;
    if (!hash_map->data[index])
        return false;
    struct variable *pair = hash_map->data[index];
    while (pair)
    {
        if (strcmp(pair->key, key) == 0)
            return true;
        pair = pair->next;
    }
    return false;
}

char *hash_map_get(const hash_map_t *hash_map, char *key)
{
    if (hash_map == NULL || hash_map->size == 0)
        return NULL;
    size_t index = hash(key) % hash_map->size;
    if (!hash_map->data[index])
        return NULL;
    struct variable *pair = hash_map->data[index];
    while (pair)
    {
        if (strcmp(pair->key, key) == 0)
            return pair->value;
        pair = pair->next;
    }
    return NULL;
}

bool hash_map_remove(hash_map_t *hash_map, char *key)
{
    if (hash_map == NULL)
        return false;
    hash_map_t *map = hash_map;
    size_t i = 0;
    int next = 0;
    while (i < map->size)
    {
        struct variable *pair = map->data[i];
        if (pair)
        {
            if (strcmp(pair->key, key) == 0)
            {
                struct variable *save = pair->next;
                free(pair->value);
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
                        struct variable *save = pair->next->next;
                        free(pair->next->key);
                        free(pair->next->value);
                        free(pair->next);
                        pair->next = save;
                        return true;
                    }
                    pair = pair->next;
                }
            if ((next = 1) && (strcmp(pair->key, key) == 0))
            {
                free(pair->key);
                free(pair->value);
                free(pair);
                return true;
            }
        }
        i++;
    }
    return false;
}

void hash_map_dump(hash_map_t *hash_map)
{
    hash_map_t *map = hash_map;
    size_t i = 0;
    printf("------------HASH_MAP OF HEADER------------\n");
    while (i < map->size)
    {
        if (map->data[i])
        {
            printf("%s: ", map->data[i]->key);
            printf("%s", map->data[i]->value);
            struct variable *tmp = map->data[i]->next;

            while (tmp)
            {
                printf(", ");
                printf("%s: ", tmp->key);
                printf("%s", tmp->value);
                tmp = tmp->next;
            }
            printf("\n");
        }
        i++;
    }
}
