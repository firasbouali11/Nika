#pragma once

typedef struct SetElt{
    void* data;
    int size;
    struct SetElt* next;
} SetElt;

typedef struct
{
    SetElt **array;
    int size;
    int capacity;
} Set;

static int hash(char *key, int capacity, int key_size);

Set *newSet();

void addToSet(Set *set, void *element, int elt_size);

int containsInSet(Set *set, void *element, int size);

SetElt *deleteFromSet(Set *set, void *element, int elt_size);