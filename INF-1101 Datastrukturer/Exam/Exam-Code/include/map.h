/**
 * @file map.h
 * @author Steffen Viken Valvaag
 * @brief Defines a combination of chained and open hash table.
 * @version 0.1
 */
#ifndef MAP_H
#define MAP_H

#include "common.h"
#include "list.h"


/**
 * @brief The type of map.
 */
struct map;
typedef struct map map_t;


/**
 * @brief The type of map item.
 */
typedef struct map_item map_item_t;
struct map_item
{
    unsigned int hashval;
    void *key;
    void *value;
};


/**
 * @brief Creates a new empty hash table.
 * 
 * @param cmpfunc 
 * @param hashfunc 
 * @return map_t* 
 */
map_t *map_create(cmpfunc_t cmpfunc, hashfunc_t hashfunc);


/**
 * @brief Destroys the given map. Subsequently accessing the map will lead to undefined behavior.
 * 
 * @param map The map to destroy.
 */
void map_destroy(map_t *map);


/**
 * @brief Maps the given key to the given value.
 * @warning If the given key exists in the map, its value will be overwritten.
 * 
 * @param map The map to insert the new entry into.
 * @param key The key to map.
 * @param value The value to map the key to.
 */
void map_put(map_t *map, char *key, void *value);


/**
 * @brief Returns the table the given key maps to.
 * 
 * @param map The map to get the key from.
 * @param key The key to get the value for.
 * @return list_iter_t* An iterator that contains the values that the key is mapped to. Empty list if entry does not exist.
 */
list_iter_t *map_get_table(map_t *map, char *key);


/**
 * @brief The djb2 hash algorithm.
 * 
 * @param bytes The bytes to hash.
 * @return unsigned long The hash.
 * 
 * Hash algorithm devised by Dan Bernstein.
 */
unsigned long djb2(void *bytes);


#endif
