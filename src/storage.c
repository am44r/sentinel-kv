#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../include/storage.h"
#include <stdlib.h>


long storage_put(FILE *fp, const char *key, const char *value)
{
    EntryHeader header;
    header.timestamp = (uint32_t)time(NULL);
    header.key_length = strlen(key);
    header.val_length = strlen(value);

    fseek(fp, 0, SEEK_END);

    long offset = ftell(fp);

    fwrite(&header, sizeof(EntryHeader), 1, fp);

    fwrite(key, sizeof(char), header.key_length, fp);

    fwrite(value, sizeof(char),header.val_length, fp);

    fflush(fp);

    return offset;
}


char* storage_get(FILE *fp, long offset)
{
    EntryHeader header;

    fseek(fp, offset, SEEK_SET);

    fread(&header, sizeof(EntryHeader), 1, fp);

    char *value = malloc(header.val_length + 1);
    if(!value) return NULL;

    fseek(fp, header.key_length, SEEK_CUR);

    fread(value, sizeof(char), header.val_length, fp);

    value[header.val_length] = '\0';

    return value;
}
