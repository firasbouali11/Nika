#pragma once

#include "linkedlist.h"

typedef struct
{
    LinkedList *list;
} Queue;

Queue *queue_new();

void queue_push(Queue *queue, void *val);

int queue_is_empty(Queue *queue);

void *queue_head(Queue *queue);

void *queue_pop(Queue *queue);