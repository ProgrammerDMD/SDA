#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <malloc.h>

int test = 0;

void* foo(void* args){
    printf("test");

    while (1) {
        test++;
        sleep(1);
    }

    // Return reference to global variable:
    pthread_exit(NULL);
}

typedef struct Node {
    int data;
    struct Node* next;
} Node;

typedef struct LinkedList {
    Node* head;
    Node* tail;
    Node* current;
} LinkedList;

typedef struct Queue {
    Node* head;
    Node* tail;
    int size;
} Queue;

Queue* createQueue() {
    Queue* queue = malloc(sizeof(Queue));
    queue->head = NULL;
    queue->tail = NULL;
    queue->size = 0;
    return queue;
}

void q_push_back(Queue* queue, Node* node) {
    queue->size += 1;
    if (queue->head == NULL) {
        queue->head = node;
        return;
    }

    if (queue->tail == NULL) {
        queue->tail = node;
        queue->head->next = node;
        return;
    }

    queue->tail->next = node;
    queue->tail = node;
}

Node* q_pop_front(Queue* queue) {
    if (queue->head == NULL) return NULL;

    queue->size -= 1;

    Node* node = queue->head;
    if (node->next == queue->tail) {
        queue->head = queue->tail;
        queue->tail = NULL;
    } else if (node->next != NULL) {
        queue->head = node->next;
    }

    node->next = NULL;
    return node;
}

Node* createNode(int data) {
    Node* node = malloc(sizeof(Node));
    node->data = data;
    node->next = NULL;
    return node;
}

LinkedList* createList(Node* node) {
    LinkedList* list = malloc(sizeof(LinkedList));

    list->head = node;
    list->head->next = node;
    list->tail = NULL;
    list->current = list->head;

    return list;
}

int data;

void ll_push_front(LinkedList* list, Node* node) {
    Node* oldHead = list->head;
    if (list->head->next == list->head) {
        list->head = node;
        node->next = oldHead;
        list->tail = oldHead;
        return;
    }

    list->head = node;
    node->next = oldHead;
    list->tail->next = node;
}

int main(){
    pthread_t id;
    pthread_create(&id, NULL, foo, NULL);

    Queue* queue = createQueue();
    q_push_back(queue, createNode(10));
    q_push_back(queue, createNode(5));
    q_push_back(queue, createNode(3));

    Node* node = q_pop_front(queue);
    while (node) {
        printf("%d ", node->data);
        node = q_pop_front(queue);
    }

    system("pause");

    return 0;
}