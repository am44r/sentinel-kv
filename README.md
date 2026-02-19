# SENTINEL-KV
### A Low-Level, Persistent Key-Value Store in Pure C

## OVERVIEW
Sentinel-KV is a minimalist, networked key-value store built from scratch. No external dependencies, no bloated frameworks—just raw C accessing memory and disk. It implements a persistent Log-Structured storage engine backed by an in-memory hash index.

**Author:** Amaar Sana
**License:** MIT

## ARCHITECTURE

### 1. The Storage Engine (Log-Structured)
We don't do random writes here. That's slow. Sentinel-KV uses an **append-only** storage model (`storage.c`).
* **Writes (`SET`):** Data is appended to the end of `data/storage.db`. We write a binary `EntryHeader` (timestamp, key length, value length) followed by the raw key and value bytes.
* **Reads (`GET`):** O(1) disk seek. The in-memory index tells us exactly where the data lives (offset) in the file. We jump, read the header, and pull the value.
* **Persistence:** On boot, the server replays the entire `storage.db` file to rebuild the hashmap in RAM. Crash safety included.

### 2. The Index (Hashmap)
* **Algorithm:** DJB2 hash function (the classic `hash * 33 + c`) for string hashing.
* **Collision Resolution:** Linear Probing. If a slot is taken, we walk the array until we find a spot. Simple, cache-friendly.
* **Capacity:** Currently hardcoded to 1024 slots (`TABLE_SIZE`) because `realloc` is for people who don't know their memory constraints ahead of time.

### 3. The Network (TCP Server)
* Uses standard BSD sockets (`<arpa/inet.h>`).
* Single-threaded, blocking I/O (for now). It handles one client, processes the command, and waits for the next.
* **Protocol:** Custom plain-text space-delimited protocol.

## BUILD & RUN

**Prerequisites:**
* GCC
* Make
* A POSIX-compliant OS (Linux/macOS)

**Compile:**
```bash
make
# Compiles src/main.c src/storage.c src/hashmap.c src/server.c -> bin/sentinel-kv
```

## Usage
Run using netcat
```bash
$ nc localhost 6379
```
**Commands**
Store data (SET)
```bash
SET user1 hello
```
Delete data (DELETE)
```bash
DELETE user1
```

Update data (SET)
```bash
SET user1 hello
```

**Example**
```bash
$ nc localhost 6379

SET user_id 42069
+OK

GET user_id
$5
42069

DELETE user_id
+OK
```

## Data layout
All entries in this program are stored as such:
```c
typedef struct {
    uint32_t timestamp;  // 4 bytes
    uint32_t key_length; // 4 bytes
    uint32_t val_length; // 4 bytes
} EntryHeader;
```
## Run with Docker
If you have Docker installed, you can build and run Sentinel-KV without installing GCC:

1. Build: `docker build -t sentinel-kv .`
2. Run: `docker run -p 6379:6379 -v $(pwd)/data:/app/data sentinel-kv`
