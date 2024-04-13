#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headers/LinkedList.h"


LinkedList* newLinkedList() {
    LinkedList* list = malloc(sizeof(LinkedList));
    list->head = NULL;
    list->tail = NULL;
    list-> length = 0;
    return list;
}

void appendToLinkedList(LinkedList* list, void* data, int size) {
    Node* node = malloc(sizeof(Node));
    node->data = malloc(size); 
    memcpy(node->data, data, size);
    node->next = NULL;
    list->length += 1;
    if(list->head == NULL){
        list->head = node;
        list->tail = node;
        return;
    }
    list->tail->next = node;
    list->tail = node;
}

void prependToLinkedList(LinkedList* list, void* data, int size) {
    Node* node = malloc(sizeof(Node));
    Node * head = list->head;
    node->data = malloc(size); 
    memcpy(node->data, data, size);
    node->next = head;
    if(head == NULL) {
        list->tail = node;
    }
    list->head = node;
    list->length++;
}

void insertToLinkedList(LinkedList* list, int pos, void* data, int size) {
    if(pos < 0 || pos > list->length){
        printf("Position out of boundries");
        return;
    }
    if(pos == 0) {
        prependToLinkedList(list, data, size);
    } else if(pos == list->length){
        appendToLinkedList(list, data, size);
    } else {
        Node * head = list->head;
        int i = 0;
        while(head->next != NULL && i < pos - 1) {
            head = head->next;
            i++;
        }
        Node* node = malloc(sizeof(Node));
        node->data = malloc(size); 
        memcpy(node->data, data, size);
        Node* temp = head->next; 
        head->next = node;
        node->next = temp;
        list->length++;
    } 
}

void deleteFromLinkedList(LinkedList* list, int pos) {
    Node* current = list->head;
    if (pos == 0) {
        list->head = current->next;
        free(current->data);
        free(current);
        list->length--;
        return;
    }
    
    int i = 0;
    while (current->next != NULL && i < pos - 1) {
        current = current->next;
        i++;
    }
    
    Node* temp = current->next;
    current->next = temp->next;
    free(temp->data);
    free(temp);
    list->length--;
}

int searchInLinkedList(LinkedList* list, void* t, int size) {
    int i = 0;
    Node* head = list->head;
    while(head != NULL) {
        if(memcmp(head->data, t, size) == 0) {
            return i;
        }
        head = head->next;
        i++;
    }
    return -1;
}

// when values are of type int 
void printLinkedList(LinkedList * list) {
    Node* node = list->head;
    while(node != NULL) {
        printf("%d -> ", *(int*)node->data);
        node = node->next;
    }
    printf("NIL\n");
}

void destroyLinkedList(LinkedList* list) {
    Node* node = list->head;
    while(node != NULL) {
        Node* temp = node->next;
        free(node->data);
        free(node);
        node = temp;
    }
    free(list);
}