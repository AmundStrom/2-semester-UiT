/* 
 * Authors: 
 * Steffen Viken Valvaag <steffenv@cs.uit.no> 
 * Magnus Stenhaug <magnus.stenhaug@uit.no> 
 * Erlend Helland Graff <erlend.h.graff@uit.no> 
 */

#include <stdlib.h>
#include <ctype.h>

#include "map.h"
#include "list.h"
#include "printing.h"

#define INITIAL_MAP_SIZE 50


struct map
{
    cmpfunc_t cmp;
    hashfunc_t hash;
    list_t **table;
    int cap;
    int size;
};


static map_item_t *new_map_item(unsigned int hashval, void *key, void *value)
{
    map_item_t *new = (map_item_t *)calloc(1, sizeof(map_item_t));
    if (new == NULL)
    {
        fprintf(stderr, "Could not allocate new item\n");
        return NULL;
    }
    new->key = key;
    new->value = value;
    new->hashval = hashval;

    return new;
}


static int map_resize(map_t *map)
{
    int oldsize, newsize;
    list_t **old, **new;
    map_item_t *current;

    newsize = map->cap * 2;
    new = (list_t **)calloc(newsize, sizeof(list_t *));
    if (new == NULL)
    {
        fprintf(stderr, "Could not resize hash table\n");
        goto error;
    }

    old = map->table;
    oldsize = map->cap;

    map->table = new;
    map->cap = newsize;
    map->size = 0;

    for (int i = 0; i < oldsize; i++)
    {
        if (old[i] != NULL)
        {
            while (list_size(old[i]) > 0)
            {
                current = list_popfirst(old[i]);
                map_put(map, current->key, current->value);
                free(current);
            }
            list_destroy(old[i]);
        }
    }
    free(old);

    return 0;

 error:
    return -1;
}


map_t *map_create(cmpfunc_t cmp, hashfunc_t hash)
{
    map_t *map = (map_t *)calloc(1, sizeof(map_t));
    if (map == NULL)
    {
        fprintf(stderr, "Could not allocate space for map\n");
        goto error;
    }
    map->cmp = cmp;
    map->hash = hash;
    map->cap = INITIAL_MAP_SIZE;
    map->size = 0;
    map->table = (list_t **)calloc(map->cap, sizeof(list_t *));
    if (map->table == NULL)
    {
        fprintf(stderr, "Could not allocate space for hash table\n");
        goto error;
    }

    return map;

 error:
    if (map != NULL)
    {
        free(map);
    }

    return NULL;
}


void map_put(map_t *map, char *key, void *value)
{
    int idx;
    unsigned int hashval = 0;
    map_item_t *new, *current = NULL;

    if (map->size >= map->cap)
    {
        map_resize(map);
    }
    
    hashval = map->hash(key);
    idx = hashval % map->cap;

    new = new_map_item(hashval, key, value);
    if (new == NULL)
    {
        goto error;
    }

    while(1)
    {
        // If there already exists a list on the given index
        if (map->table[idx] != NULL)
        {
            list_iter_t *iter = list_createiter(map->table[idx]);
            current = list_next(iter);

            // Check if the current key is the same as the new key
            if(compare_strings(current->key, key) == 0)
            {
                list_destroyiter(iter);
                break;
            }
            list_destroyiter(iter);
        } 
        // If there doesn't exist a list on the given index
        else 
        {
            map->table[idx] = list_create(map->cmp);
            if (map->table[idx] == NULL)
            {
                goto error;
            }
            map->size++;
            break;
        }
        idx++;
        idx %= map->cap;
    }

    list_addlast(map->table[idx], new);

    return;

 error:
    if (new != NULL)
    {
        free(new);
    }
    ERROR_PRINT("Could not add to hasmap\n");

    return;
}


list_iter_t *map_get_table(map_t *map, char *key)
{
    int idx;
    unsigned int hashval = 0;
    list_iter_t *iter;
    map_item_t *current;

    if(key == NULL)
    {
        goto error;
    }

    hashval = map->hash(key);
    idx = hashval % map->cap;

    if (map->table[idx] == NULL)
    {
        goto error;
    }

    while(1)
    {
        iter = list_createiter(map->table[idx]);
        current = list_next(iter);

        // Check if the current key is the same as the given key
        if(compare_strings(current->key, key) == 0)
        {
            list_destroyiter(iter);
            break;
        }
        list_destroyiter(iter);

        idx++;
        idx %= map->cap;
        if (map->table[idx] == NULL)
        {
            goto error;
        }
    }

    iter = list_createiter(map->table[idx]);

    return iter;

 error:
    return NULL;
}

void map_destroy(map_t *map)
{
    int idx;
    list_iter_t *iter;
    map_item_t *current;

    for (idx = 0; idx < map->cap; idx++)
    {
        if (map->table[idx] == NULL)
        {
            continue;
        }
        iter = list_createiter(map->table[idx]);
        while ((current = list_next(iter)) != NULL)
        {
            free(current);
        }
        list_destroyiter(iter);
        list_destroy(map->table[idx]);
    }

    free(map->table);
    free(map);
}

unsigned long djb2(void *bytes)
{
    unsigned long hash = 5381;
    unsigned char *str = (unsigned char *)bytes;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + tolower(c)); /* hash * 33 + c */

    return hash;
}