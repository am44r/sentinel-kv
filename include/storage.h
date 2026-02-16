#ifndef STORAGE_H
#define STORAGE_H

#include <stdint.h>
#include <stdio.h>

typedef struct {
    uint32_t timestamp;
    uint32_t key_length;
    uint32_t val_length;
} EntryHeader;

long storage_put(FILE *fp, const char *key, const char *value);
char* storage_get(FILE *fp, long offset);

#endif