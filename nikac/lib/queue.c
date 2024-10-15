#include <stdlib.h>
#include <stdio.h>
#include "lib/queue.h"


Queue * queue_new(){
    Queue * queue = malloc(sizeof(Queue));
    queue->list = linkedlist_new();
    return queue;
}

void queue_push(Queue * queue, void* val){
    linkedlist_append(queue->list, val);
}

int queue_is_empty(Queue * queue){
    return queue->list->length == 0;
}

void* queue_head(Queue * queue){
    if(queue_is_empty(queue)){
        fprintf(stderr, "Queue is empty\n");
        exit(EXIT_FAILURE);
    }
    return queue->list->head->data;
}

void* queue_pop(Queue * queue){
    LinkedList * list = queue->list;
    void* val = queue_head(queue);
    linkedlist_delete(list, 0);
    return val;
}