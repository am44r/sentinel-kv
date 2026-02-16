#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "../include/network.h"
#include "../include/storage.h"
#include "../include/hashmap.h"

void handle_client(int client_fd, SimpleHashMap *index, FILE *db) {
    char buffer[1024] = {0};
    
    int valread = read(client_fd, buffer, 1024);
    if (valread <= 0) return;

    buffer[strcspn(buffer, "\n")] = 0;
    
    printf("Received: %s\n", buffer);

    char *command = strtok(buffer, " ");
    char *key = strtok(NULL, " ");
    char *value = strtok(NULL, " ");

    if (command && strcmp(command, "SET") == 0) {
        if (key && value) {
            long offset = storage_put(db, key, value);
            hashmap_put(index, key, offset);
            
            char *msg = "+OK\n";
            send(client_fd, msg, strlen(msg), 0);
        } else {
            char *msg = "-ERR Usage: SET key value\n";
            send(client_fd, msg, strlen(msg), 0);
        }
    } 
    else if (command && strcmp(command, "GET") == 0) {
        if (key) {
            long offset = hashmap_get(index, key);
            if (offset != -1) {
                char *found_val = storage_get(db, offset);
                
                char response[1024];
                snprintf(response, sizeof(response), "$%ld\n%s\n", strlen(found_val), found_val);
                send(client_fd, response, strlen(response), 0);
                
                free(found_val);
            } else {
                char *msg = "-ERR Key not found\n";
                send(client_fd, msg, strlen(msg), 0);
            }
        } else {
            char *msg = "-ERR Usage: GET key\n";
            send(client_fd, msg, strlen(msg), 0);
        }
    } 
    else {
        char *msg = "-ERR Unknown Command\n";
        send(client_fd, msg, strlen(msg), 0);
    }
}

void server_start(int port, SimpleHashMap *index, FILE *db) {
    int server_fd, client_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int opt = 1;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("Setsockopt failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Sentinel-KV listening on port %d...\n", port);

    while (1) {
        if ((client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("Accept failed");
            continue;
        }

        handle_client(client_fd, index, db);
        close(client_fd);
    }
}