#pragma once

#include "List.h"

typedef struct Pair {
    void* first;
    void* second;
    struct Pair *next;
} Pair;

typedef struct {
    Pair** array;
    int size;
    int capacity;
} Map;

Map* newMap();

Pair* newPair(void* key, void* value);

void addToMap(Map* map, void* key, void* value, int key_size);

void* getFromMap(Map* map, void* key, int size);

Pair* deleteFromMap(Map* map, void* key, int size);