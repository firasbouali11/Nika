#include <stdlib.h>
#include <string.h>

#include "lib/set.h"

#define INIT_CAPACITY 100
#define THRESHOLD 0.8

static int hash(void *key, int capacity, int key_size)
{
    long long hash = 0;
    char *char_key = (char *)key;
    for (int i = 0; i < key_size; i++)
    {
        hash = (31 * hash + char_key[i]) % capacity;
    }
    if (hash < 0)
        hash += capacity;
    return (int)hash;
}

Set *set_new()
{
    Set *set = malloc(sizeof(Set));
    set->array = malloc(INIT_CAPACITY * sizeof(SetElt *));
    for (int i = 0; i < INIT_CAPACITY; i++)
        set->array[i] = NULL;
    set->size = 0;
    set->capacity = INIT_CAPACITY;
    return set;
}

static SetElt *set_elt_new(void *value, int size)
{
    SetElt *elt = malloc(sizeof(SetElt));
    elt->data = value;
    elt->size = size;
    elt->next = NULL;
    return elt;
}

static void resize(Set *set)
{
    int new_capacity = 2 * set->capacity;
    SetElt **new_array = malloc(new_capacity * sizeof(SetElt *));
    // rehash elements
    for (int i = 0; i < new_capacity; i++)
    {
        new_array[i] = NULL;
    }
    
    for (int i = 0; i < set->capacity; i++)
    {
        SetElt *s = set->array[i];
        if (s != NULL)
        {
            int key = hash(s->data, new_capacity, s->size);
            new_array[key] = s;
        }
    }
    free(set->array);
    set->array = new_array;
    set->capacity = new_capacity;
}

void set_add(Set *set, void *element, int elt_size)
{
    if ((float)set->size / set->capacity >= THRESHOLD)
        resize(set);
    set->size++;
    SetElt *new = set_elt_new(element, elt_size);
    int key = hash(element, set->capacity, elt_size);
    SetElt *existing = set->array[key];
    if (!existing)
    {
        set->array[key] = new;
        return;
    }
    while (existing)
    {
        if (memcmp(existing->data, element, elt_size) == 0)
        {
            set->size--;
            return;
        }
        if (!existing->next)
        {
            existing->next = new;
            return;
        }
        existing = existing->next;
    }
    return;
}

int set_contains(Set *set, void *element, int size)
{
    int key = hash(element, set->capacity, size);
    SetElt *x = set->array[key];
    while (x)
    {
        if (memcmp(x->data, element, size) == 0)
            return 1;
        x = x->next;
    }
    return 0;
}

SetElt *set_delete(Set *set, void *element, int elt_size)
{
    int key = hash(element, set->capacity, elt_size);
    SetElt *elt = set->array[key];
    SetElt *prev = NULL;
    while (elt && (memcmp(elt->data, element, elt_size) != 0))
    {
        prev = elt;
        elt = elt->next;
    }
    if (elt == NULL)
        return NULL;
    if (prev == NULL)
        set->array[key] = elt->next;
    else
        prev->next = elt->next;
    set->size--;
    return elt;
}
