#include <stdio.h>
#include <stdlib.h>
#include "../include/storage.h"
#include "../include/hashmap.h"
#include "../include/network.h"

int main() {
    FILE *db = fopen("data/storage.db", "rb+");
    if (!db) db = fopen("data/storage.db", "wb+");
    
    SimpleHashMap index;
    hashmap_init(&index);

    printf("--- LOADING INDEX ---\n");
    storage_load(db, &index);
    printf("Database loaded.\n");

    server_start(6379, &index, db);

    hashmap_destroy(&index);
    fclose(db);
    return 0;
}