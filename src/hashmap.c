#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../include/hashmap.h"

// DJB2 -> string to large number
unsigned long hash(const char *str)
{
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
    {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

void hashmap_init(SimpleHashMap *map)
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        map->entries[i].key = NULL;
        map->entries[i].offset = 0;
        map->entries[i].is_occupied = 0;
    }
}

void hashmap_put(SimpleHashMap *map, const char *key, long offset)
{
    unsigned long index = hash(key) % TABLE_SIZE;
    unsigned long original_index = index;

    while (map->entries[index].is_occupied)
    {
        if (strcmp(map->entries[index].key, key) == 0)
        {
            map->entries[index].offset = offset;
            return;
        }
        index = (index + 1) % TABLE_SIZE;
        
        if (index == original_index) 
        {
            printf("Error: Hash Map is full!\n");
            return;
        }
    }
    map->entries[index].key = strdup(key); 
    map->entries[index].offset = offset;
    map->entries[index].is_occupied = 1;  
}


long hashmap_get(SimpleHashMap *map, const char *key) {
    unsigned long index = hash(key) % TABLE_SIZE;
    unsigned long original_index = index;

    while (map->entries[index].is_occupied) {
        if (strcmp(map->entries[index].key, key) == 0) {
            return map->entries[index].offset;
        }
        index = (index + 1) % TABLE_SIZE;
        if (index == original_index) break;
    }

    return -1;
}


void hashmap_destroy(SimpleHashMap *map) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (map->entries[i].key != NULL) {
            free(map->entries[i].key);
        }
    }
}