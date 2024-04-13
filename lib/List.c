#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headers/List.h"

#define INITAL_CAPACITY 100

List * newList(){
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

void addToList(List* list, void* val, int size){
    if(list->size >= list->capacity){
        resize(list);
    }
    void * new_val = malloc(size);
    memcpy(new_val, val, size);
    list->array[list->size] = new_val;
    list->size++;
}

void* getFromList(List* list, int pos){
    if(pos < 0 || pos >= list->size)
        return NULL;
    return list->array[pos];
}

void popFromList(List* list){
    if(list->array == NULL || list->size == 0){
        printf("Can not pop from an empty list");
        exit(1);
    }
    free(list->array[list->size-1]);
    list->size--;
}

void deleteFromList(List* list, int pos){
    if(pos < 0 || pos >= list->size){
        printf("Index out of bound");
        exit(1);
    }
    free(list->array[pos]);
    for (int i = pos; i < list->size-1; i++)
    {
        list->array[i] = list->array[i+1];
    }
    list->size--;
}

void insertToList(List* list, int pos, void* val, int size){
    if(list->size >= list->capacity){
        resize(list);
    }
    for (int i = list->size - 1; i >= pos; i--) {
        list->array[i + 1] = list->array[i];
    }
    void * new_val = malloc(size);
    list->array[pos] = new_val;
    list->size++;
}

void freeList(List * list){
    for (int i = 0; i < list->size; i++){
        free(list->array[i]);
    }
    free(list->array);
    free(list);
}

static void merge(int **array, int l, int mid, int h) {
    int i = l;
    int j = mid + 1;
    int k = 0;
    int res[h - l + 1];
    while (i <= mid && j <= h) {
        if (*array[i] < *array[j]) {
            res[k] = *array[i];
            i++;
        } else {
            res[k] = *array[j];
            j++;
        }
        k++;
    }
    while (i <= mid) {
        res[k] = *array[i];
        i++;
        k++;
    }
    while (j <= h) {
        res[k] = *array[j];
        j++;
        k++;
    }
    for (int i = l; i <= h; i++)
        *array[i] = res[i - l];
}

void mergeSort(int **array, int l, int h) {
    if (l < h) {
        int mid = (l + h) / 2;
        mergeSort(array, l, mid);
        mergeSort(array, mid + 1, h);
        merge(array, l, mid, h);
    }
}