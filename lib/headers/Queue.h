#include "LinkedList.h"

typedef struct {
    LinkedList * list;
} Queue;


Queue * newQueue();

void pushQueue(Queue * queue, void* val, int size);

int queueEmpty(Queue * queue);

void* headQueue(Queue * queue);

void* popQueue(Queue * queue);