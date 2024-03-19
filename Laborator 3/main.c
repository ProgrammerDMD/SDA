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

Node* createNode() {
    int data;
    printf("Care este marimea acestui nod? ");
    scanf("%d", &data);

    Node* node = malloc(sizeof(Node));
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    return node;
}

Node* addNode(Node* head) {
    if (head->right == NULL) {
        Node* node = createNode();
        head->right = node;
        return node;
    }

    if (head->left == NULL) {
        Node* node = createNode();
        head->left = node;
        return node;
    }

    return addNode(head->right);
}

void readNode(List* list) {
    system("cls");
    printf("Cate elemente doresti sa citesti? ");

    int n;
    scanf("%d", &n);

    if (n >= 1 && list->head == NULL) {
        n -= 1;
        Node* node = createNode();
        list->head = node;
    }

    for (int i = 0; i < n; i++) {
        addNode(list->head);
    }
}

void displayNodes(Node* head) {
    if (head == NULL) return;

    printf("%d ", head->data);
    if (head->right != NULL) {
        displayNodes(head->right);
    }

    if (head->left != NULL) {
        displayNodes(head->left);
    }
}

int meniu() {
    system("cls");

    printf("Selecteaza o optiune:\n");
    printf("0. Iesire din program\n");
    printf("1. Citeste de la tastatura unul sau mai multe elemente\n");
    printf("2. Afiseaza elementele arborelui\n");

    int optiune;
    scanf("%d", &optiune);

    return optiune;
}

int main() {

    List* list = malloc(sizeof(List));
    list->head = NULL;

    int optiune;
    while ((optiune = meniu())) {
        if (optiune == 1) {
            readNode(list);
        }
        if (optiune == 2) {
            system("cls");
            displayNodes(list->head);
            printf("\n");
            system("pause");
        }
    }

    return 0;
}
