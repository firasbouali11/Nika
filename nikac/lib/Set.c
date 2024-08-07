#include <stdlib.h>
#include <string.h>

#include "headers/Set.h"

#define INIT_CAPACITY 100

static int hash(char *key, int capacity, int key_size)
{
    int hash = 0;
    if (key_size == 0)
        key_size = strlen(key);
    for (int i = 0; i < key_size; i++)
    {
        hash = (31 * key[i] + hash) % capacity;
    }
    return abs(hash);
}

Set *newSet()
{
    Set *set = malloc(sizeof(Set));
    set->array = malloc(INIT_CAPACITY * sizeof(SetElt *));
    for (int i = 0; i < INIT_CAPACITY; i++)
    {
        set->array[i] = NULL;
    }
    set->size = 0;
    set->capacity = INIT_CAPACITY;
    return set;
}

SetElt *newSetElt(void *value, int size)
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

void addToSet(Set *set, void *element, int elt_size)
{
    if (set->size >= set->capacity)
        resize(set);
    set->size++;
    SetElt *new = newSetElt(element, elt_size);
    int key = hash(element, set->capacity, elt_size);
    SetElt *existing = set->array[key];
    if (existing == NULL)
    {
        set->array[key] = new;
        return;
    }
    SetElt *prev = NULL;
    while (existing != NULL && memcmp(existing->data, element, elt_size) != 0)
    {
        prev = existing;
        existing = existing->next;
    }
    if(!existing)
        prev->next = new;
    else{
        free(existing->data);
        existing->data = new;
    }
}

int containsInSet(Set *set, void *element, int size)
{
    int key = hash(element, set->capacity, size);
    SetElt *x = set->array[key];
    while (x != NULL){
        if (memcmp(x->data, element, size) == 0)
            return 1;
        x = x->next;
    }
    return 0;
}

SetElt *deleteFromSet(Set *set, void *element, int elt_size)
{
    int key = hash(element, set->capacity, elt_size);
    SetElt *elt = set->array[key];
    SetElt *prev = NULL;
    while (elt != NULL && (memcmp(elt->data, element, elt_size) != 0))
    {
        prev = elt;
        elt = elt->next;
    }
    if(elt == NULL)
        return NULL;
    if(prev == NULL){
        set->array[key] = elt->next;
    }
    else prev->next = elt->next;
    set->size--;
    return elt;

}
    