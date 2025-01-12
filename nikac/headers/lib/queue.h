#pragma once

#include "linkedlist.h"

void queue_push(LinkedList *queue, void *val);

int queue_is_empty(LinkedList *queue);

void *queue_head(LinkedList *queue);

void *queue_pop(LinkedList *queue);