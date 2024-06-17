#include <stdlib.h>
#include <stdio.h>
#include "headers/Queue.h"


Queue * newQueue(){
    Queue * queue = malloc(sizeof(Queue));
    queue->list = newLinkedList();
    return queue;
}

void pushQueue(Queue * queue, void* val, int size){
    appendToLinkedList(queue->list, val, size);
}

int queueEmpty(Queue * queue){
    return queue->list->length == 0;
}

void* headQueue(Queue * queue){
    if(queueEmpty(queue)){
        fprintf(stderr, "Queue is empty\n");
        exit(EXIT_FAILURE);
    }
    return queue->list->head->data;
}

void* popQueue(Queue * queue){
    LinkedList * list = queue->list;
    void* val = headQueue(queue);
    deleteFromLinkedList(list, 0);
    return val;
}