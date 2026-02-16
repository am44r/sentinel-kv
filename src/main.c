#include <stdio.h>
#include <stdlib.h>
#include "../include/storage.h"
#include "../include/hashmap.h"

int main() {
    FILE *db = fopen("data/storage.db", "rb+");
    if (!db) db = fopen("data/storage.db", "wb+");
    
    SimpleHashMap index;
    hashmap_init(&index);

    printf("--- LOADING INDEX ---\n");
    storage_load(db, &index);
    printf("Database loaded.\n");

    long offset = hashmap_get(&index, "user:1");
    if (offset != -1) {
        printf("Recovered 'user:1' from disk! Offset: %ld\n", offset);
    } else {
        printf("Could not find 'user:1'. Is this the first run?\n");
    }


    hashmap_destroy(&index);
    fclose(db);
    return 0;
}