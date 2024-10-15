#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/list.h"

#define INITAL_CAPACITY 100
#define EMPTY_LIST_ERROR "Can not pop from an empty list\n"
#define OUT_OF_BOUND_RROR "Index out of bound\n"

List * list_new(){
    List* list = malloc(sizeof(List)); 
    list->array = malloc(INITAL_CAPACITY * sizeof(void*));
    list->size = 0;
    list->capacity = INITAL_CAPACITY;
    return list;
}

static void resize(List* list){
    list->capacity *= 2;
    list->array = realloc(list->array, list->capacity * sizeof(void*));
}

void list_add(List* list, void* val){
    if(list->size >= list->capacity)
        resize(list);
    list->array[list->size] = val;
    list->size++;
}

void* list_get(List* list, int pos){
    if(pos < 0 || pos >= list->size)
        return NULL;
    return list->array[pos];
}

void* list_pop(List* list){
    if(list->array == NULL || list->size == 0){
        fprintf(stderr, EMPTY_LIST_ERROR);
        exit(EXIT_FAILURE);
    }
    list->size--;
    return list->array[list->size];
}

void* list_delete(List* list, int pos){
    if(pos < 0 || pos >= list->size){
        fprintf(stderr, OUT_OF_BOUND_RROR);
        exit(EXIT_FAILURE);
    }
    for (int i = pos; i < list->size-1; i++)
        list->array[i] = list->array[i+1];
    list->size--;
    return list->array[list->size];
}

void list_insert(List* list, int pos, void* val){
    if(list->size >= list->capacity)
        resize(list);
    for (int i = list->size - 1; i >= pos; i--)
        list->array[i + 1] = list->array[i];
    list->array[pos] = val;
    list->size++;
}