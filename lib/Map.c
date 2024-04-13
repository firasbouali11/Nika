#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "headers/Map.h"

#define INIT_CAPACITY 100
#define THRESHOLD 0.75

static int hash(void *key, int capacity)
{
    int hash = 0;
    char *char_key = (char *)key;
    for (int i = 0; char_key[i] != '\0'; i++)
    {
        hash = (31 * hash + char_key[i]) % capacity;
    }
    return abs(hash);
}

Map *newMap()
{
    Map *map = malloc(sizeof(Map));
    map->array = malloc(INIT_CAPACITY * sizeof(Pair *));
    for (int i = 0; i < INIT_CAPACITY; i++)
        map->array[i] = NULL;
    map->capacity = INIT_CAPACITY;
    map->size = 0;
    return map;
}

Pair *newPair(void *key, void *value)
{
    Pair *node = malloc(sizeof(Pair));
    node->first = key;
    node->second = value;
    // node->first = malloc(key_size);
    // node->second = malloc(value_size);
    // memcpy(node->first, key, key_size);
    // memcpy(node->second, value, value_size);
    node->next = NULL;
    return node;
}

static void resize(Map *map)
{
    int new_capacity = map->capacity * 2;
    Pair **new_array = malloc(new_capacity * sizeof(Pair *));
    for (int i = 0; i < new_capacity; i++)
        new_array[i] = NULL;
    for (int i = 0; i < map->capacity; i++)
    {
        Pair *node = map->array[i];
        if (node != NULL)
        {
            int index = hash(node->first, new_capacity);
            new_array[index] = node;
        }
    }
    free(map->array);
    map->array = new_array;
    map->capacity = new_capacity;
}

void addToMap(Map *map, void *key, void *value, int key_size)
{
    if (((double)map->size / map->capacity) >= THRESHOLD)
        resize(map);
    int index = hash(key, map->capacity);
    Pair *existing_node = map->array[index];
    map->size++;
    Pair *node = newPair(key, value);
    if(!existing_node){
        map->array[index] = node; 
        return;
    }
    while (existing_node)
    {
        if (memcmp(existing_node->first, key, key_size) == 0)
        {
            free(existing_node->second);
            existing_node->second = value;
            return;
        }
        if (!existing_node->next)
        {
            existing_node->next = node;
            return;
        }
        existing_node = existing_node->next;
    }
    
}

void *getFromMap(Map *map, void *key, int size)
{
    int index = hash(key, map->capacity);
    Pair *node = map->array[index];
    while (node != NULL)
    {
        if (memcmp(node->first, key, size) == 0)
            return node->second;
        node = node->next;
    }
    return NULL;
}

Pair* deleteFromMap(Map *map, void *key, int size)
{
    int index = hash(key, map->capacity);
    Pair *current = (Pair *)map->array[index];
    Pair *prev = NULL;
    while (current != NULL && (memcmp(current->first, key, size) != 0))
    {
        prev = current;
        current = current->next;
    }
    if (current == NULL)
        return NULL;
    
    if(prev == NULL) map->array[index] = NULL;
    else prev->next = current->next;
    map->size--;
    return current;
}