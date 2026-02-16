CC = gcc
CFLAGS = -Wall -Iinclude

all: bin/sentinel-kv

bin/sentinel-kv: src/main.c src/storage.c
	$(CC) $(CFLAGS) src/main.c src/storage.c -o bin/sentinel-kv

clean:
	rm -f bin/sentinel-kv