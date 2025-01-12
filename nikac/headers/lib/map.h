#pragma once

typedef struct Pair
{
    void *first;
    void *second;
    struct Pair *next;
} Pair;

typedef struct {
    void * value;
    unsigned int size;
} Key;

typedef struct
{
    Pair **array;
    int size;
    int capacity;
} Map;

Map *map_new();

Pair *pair_new(void *key, void *value);

void *map_add(Map *map, void *key, void *value, int key_size);

void *map_get(Map *map, void *key, int size);

Pair *map_delete(Map *map, void *key, int size);