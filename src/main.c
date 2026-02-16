#include <stdio.h>
#include <stdlib.h>
#include "../include/storage.h"

int main() {
    FILE *db = fopen("data/storage.db", "ab+");
    if (!db) {
        perror("Failed to open DB");
        return 1;
    }

    printf("--- WRITING ---\n");
    long offset1 = storage_put(db, "user:1", "wintermute");
    printf("Saved 'user:1' at offset %ld\n", offset1);

    long offset2 = storage_put(db, "user:2", "neuromancer");
    printf("Saved 'user:2' at offset %ld\n", offset2);

    printf("\n--- READING ---\n");
    
    char *val1 = storage_get(db, offset1);
    char *val2 = storage_get(db, offset2);

    printf("Key 'user:1' -> Value: %s\n", val1);
    printf("Key 'user:2' -> Value: %s\n", val2);

    free(val1);
    free(val2);
    fclose(db);
    
    return 0;
}