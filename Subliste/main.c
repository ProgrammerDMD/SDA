#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

typedef struct List {
    Node* head;
    int max;
    int size;
} List;

Node* createNode(int data) {
    Node* node = malloc(sizeof(Node));
    node->data = data;
    node->next = NULL;
    return node;
}

List* createList() {
    List* list = malloc(sizeof(List));
    list->head = NULL;
    list->max = 0;
    list->size = 0;
    return list;
}

void push_front(List* list, Node* node) {
    list->size += 1;
    if (list->head == NULL) {
        list->head = node;
        return;
    }

    Node* head = list->head;
    list->head = node;
    list->head->next = head;

    if (node->data > list->max) list->max = node->data;
}

void print_list(List* list, int n) {
    if (list->head == NULL) return;

    if (list->size < 2) {
        return;
    }

    printf("Divizorul este %d: ", n);

    Node* node = list->head;
    while (node != NULL) {
        printf("%d ", node->data);
        node = node->next;
    }
    printf("\n");
}

List** sublists(List* list) {
    List** lists = malloc(sizeof(List*) * (list->max + 1));
    for (int i = 0; i <= list->max; i++) {
        Node* node = list->head;

        lists[i] = createList();
        List* sublist = lists[i];

        if (i == 0 || i == 1) {
            while (node != NULL) {
                if (node->data == i) {
                    push_front(sublist, createNode(node->data));
                }
                node = node->next;
            }
            print_list(sublist, i);
            continue;
        }

        while (node != NULL) {
            if ((node->data % i == 0) && !(node->data == 0 || node->data == 1)) {
                push_front(sublist, createNode(node->data));
            }
            node = node->next;
        }

        print_list(sublist, i);
    }

    return lists;
}

int main() {
    int n;
    printf("Cate numere citesti? ");
    scanf("%d", &n);
    printf("Introdu numere: ");

    List* list = createList();
    for (int i = 0; i < n; i++) {
        int numar;
        scanf("%d", &numar);
        push_front(list, createNode(numar));
    }

    sublists(list);
    system("pause");

    return 0;
}