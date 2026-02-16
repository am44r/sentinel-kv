CC = gcc
CFLAGS = -Wall -Iinclude
SRC = src/main.c src/storage.c src/hashmap.c src/server.c

all: bin/sentinel-kv

bin/sentinel-kv: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o bin/sentinel-kv

clean:
	rm -f bin/sentinel-kv