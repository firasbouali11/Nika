#include <stdlib.h>
#include <stdio.h>

#include "lib/queue.h"
#include "lib/linkedlist.h"

void queue_push(LinkedList *queue, void *val)
{
    linkedlist_append(queue, val);
}

int queue_is_empty(LinkedList *queue)
{
    return queue || queue->length == 0;
}

void *queue_head(LinkedList *queue)
{
    if (queue_is_empty(queue))
    {
        fprintf(stderr, "Queue is empty\n");
        exit(EXIT_FAILURE);
    }
    return queue->head->data;
}

void *queue_pop(LinkedList *queue)
{
    void *val = queue_head(queue);
    linkedlist_delete(queue, 0);
    return val;
}