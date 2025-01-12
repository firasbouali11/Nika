#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib/linkedlist.h"

LinkedList *linkedlist_new()
{
    LinkedList *list = malloc(sizeof(LinkedList));
    list->head = NULL;
    list->tail = NULL;
    list->length = 0;
    return list;
}

void linkedlist_free(LinkedList* list) {
    Node* curr = list->head;
    while(curr){
        Node* next = curr->next;
        free(curr);
        curr = next;
    }
    free(list);
}

void linkedlist_append(LinkedList *list, void *data)
{
    Node *node = malloc(sizeof(Node));
    node->data = data;
    node->next = NULL;
    list->length++;
    if (list->head == NULL)
    {
        list->head = node;
        list->tail = node;
        return;
    }
    list->tail->next = node;
    list->tail = node;
}

void linkedlist_prepend(LinkedList *list, void *data)
{
    Node *node = malloc(sizeof(Node));
    Node *head = list->head;
    node->data = data;
    node->next = head;
    if (head == NULL)
    {
        list->tail = node;
    }
    list->head = node;
    list->length++;
}

void linkedlist_insert(LinkedList *list, int pos, void *data)
{
    if (pos < 0 || pos > list->length)
    {
        fprintf(stderr, "Position out of boundries");
        exit(EXIT_FAILURE);
    }
    if (pos == 0)
    {
        linkedlist_prepend(list, data);
    }
    else if (pos == list->length)
    {
        linkedlist_append(list, data);
    }
    else
    {
        Node *head = list->head;
        int i = 0;
        while (head->next != NULL && i < pos - 1)
        {
            head = head->next;
            i++;
        }
        Node *node = malloc(sizeof(Node));
        node->data = data;
        Node *temp = head->next;
        head->next = node;
        node->next = temp;
        list->length++;
    }
}

void *linkedlist_delete(LinkedList *list, int pos)
{
    Node *current = list->head;
    if (pos == 0)
    {
        list->head = current->next;
        void *data = current->data;
        free(current);
        list->length--;
        return data;
    }

    int i = 0;
    while (current->next != NULL && i < pos - 1)
    {
        current = current->next;
        i++;
    }

    Node *temp = current->next;
    current->next = temp->next;
    void *data = temp->data;
    free(temp);
    list->length--;
    return data;
}

int linkedlist_search(LinkedList *list, void *t, int size)
{
    int i = 0;
    Node *head = list->head;
    while (head != NULL)
    {
        if (memcmp(head->data, t, size) == 0)
            return i;
        head = head->next;
        i++;
    }
    return -1;
}

void linkedlist_reverse(LinkedList *list)
{
    Node *curr = list->head;
    Node *next = NULL, *prev = NULL;
    while (curr != NULL)
    {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    list->head = prev;
}