#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdint.h>
#include <stddef.h>

#define TABLE_SIZE 1024

typedef struct {
    char *key;
    long offset;
    int is_occupied;
} HashEntry;

typedef struct {
    HashEntry entries[TABLE_SIZE];
} SimpleHashMap;

void hashmap_init(SimpleHashMap *map);
void hashmap_put(SimpleHashMap *map, const char *key, long offset);
long hashmap_get(SimpleHashMap *map, const char *key);
void hashmap_destroy(SimpleHashMap *map);

#endif