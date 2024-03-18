#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* left;
    struct Node* right;
} Node;

typedef struct List {
    Node* head;
} List;

Node* createNode(int data) {
    Node* node = malloc(sizeof(Node));
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    return node;
}

Node* addNode(Node* head, int data) {
    if (head->right == NULL) {
        Node* node = createNode(data);
        head->right = createNode(data);
        return node;
    }

    if (head->left == NULL) {
        Node* node = createNode(data);
        head->left = createNode(data);
        return node;
    }

    return addNode(head->right, data);
}

void displayNodes(Node* head) {
    printf("%d ", head->data);
    if (head->right != NULL) {
        displayNodes(head->right);
    }

    if (head->left != NULL) {
        displayNodes(head->left);
    }
}

int main() {

    List* list = malloc(sizeof(List));

    Node* head = createNode(0);
    list->head = head;

    addNode(head, 1);
    addNode(head, 2);
    addNode(head, 3);
    addNode(head, 4);
    addNode(head, 5);

    displayNodes(head);

    return 0;
}
