#ifndef NETWORK_H
#define NETWORK_H

#include <stdio.h>
#include "hashmap.h"

void server_start(int port, SimpleHashMap *index, FILE *db);

#endif