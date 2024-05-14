#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Node {
    int data;
    struct Node *right;
    struct Node *left;
} Node;

Node *createNode(int data) {
    Node *node = malloc(sizeof(Node));
    node->data = data;
    node->right = NULL;
    node->left = NULL;
    return node;
}

typedef struct QNode {
    void *data;
    struct QNode *next;
} QNode;

typedef struct Queue {
    QNode *head;
    int size;
} Queue;

QNode *createQNode(void *data) {
    if (data == NULL) return NULL;

    QNode *node = malloc(sizeof(QNode));
    node->data = data;
    node->next = NULL;
    return node;
}

Queue *createQueue() {
    Queue *queue = malloc(sizeof(Queue));
    queue->head = NULL;
    queue->size = 0;
    return queue;
}

QNode *popFront(Queue *list) {
    QNode *current = list->head;
    QNode *next = current->next;
    list->head = next;
    current->next = NULL;
    list->size -= 1;
    return current;
}

void pushBack(Queue *queue, QNode *node) {
    if (node == NULL) return;

    queue->size += 1;
    if (queue->head == NULL) {
        queue->head = node;
        return;
    }

    QNode *current = queue->head;
    if (current->next == NULL) {
        current->next = node;
        return;
    }

    while (current->next != NULL) {
        current = current->next;
    }

    current->next = node;
}

void pushNode(Node *head, Node *node) {
    if (head == NULL) return;

    if (node->data >= head->data) {
        if (head->right == NULL) {
            head->right = node;
        } else {
            pushNode(head->right, node);
        }
    } else {
        if (head->left == NULL) {
            head->left = node;
        } else {
            pushNode(head->left, node);
        }
    }
}

Node *searchNode(Node *head, int *key) {
    if (head == NULL) return NULL;

    if (head->data == *key) return head;

    if (*key > head->data) {
        return searchNode(head->right, key);
    } else {
        return searchNode(head->left, key);
    }
}

void preorderSearch(Node *head) {
    if (head == NULL) return;

    printf("%d ", head->data);
    preorderSearch(head->left);
    preorderSearch(head->right);
}

void inorderSearch(Node *head) {
    if (head == NULL) return;

    inorderSearch(head->left);
    printf("%d ", head->data);
    inorderSearch(head->right);
}

void postorderSearch(Node *head) {
    if (head == NULL) return;

    postorderSearch(head->left);
    postorderSearch(head->right);
    printf("%d ", head->data);
}

void searchLargime(Node *head) {
    Queue *queue = createQueue();
    pushBack(queue, createQNode(head));

    while (queue->size > 0) {
        QNode *node = popFront(queue);
        Node *data = node->data;
        printf("%d ", data->data);

        pushBack(queue, createQNode(data->left));
        pushBack(queue, createQNode(data->right));

        node->data = NULL;
        node->next = NULL;
        free(node);
    }

    free(queue);
}

void leftRotate(Node *node) {

}

int meniu() {
    system("cls");

    printf("Selecteaza o optiune:\n");
    printf("0. Iesire din program\n");
    printf("1. Adauga n elemente in arbore\n");
    printf("2. Parcurgerea arborelui in preordine\n");
    printf("3. Parcurgerea arborelui in inordine\n");
    printf("4. Parcurgerea arborelui in postordine\n");
    printf("5. Parcurgerea arborelui in Largime (BFS)\n");
    printf("6. Balansarea arborelui\n");
    printf("7. Oglindirea arborelui\n");
    printf("8. Curatarea arborelui\n");
    printf("9. Eliberarea memoriei\n");

    int optiune;
    scanf("%d", &optiune);

    return optiune;
}

Node *createNodes(Node *head) {
    system("cls");

    printf("Cate noduri doresti sa citesti? ");
    int noduri;

    scanf("%d", &noduri);
    for (int i = 0; i < noduri; i++) {
        if (head == NULL) {
            head = createNode(rand() % 100);
            continue;
        }

        pushNode(head, createNode(rand() % 100));
    }

    printf("Nodurile au fost create!\n");
    system("pause");

    return head;
}

int getNodeHeight(Node *node) {
    if (node == NULL) return 0;
    if (node->right == 0 && node->left == 0) return 0;

    int right = getNodeHeight(node->right) + 1;
    int left = getNodeHeight(node->left) + 1;

    if (right >= left) {
        return right;
    } else {
        return left;
    }
}

int getTotalNodes(Node* root) {
    if (root == NULL) return 0;
    return 1 + getTotalNodes(root->left) + getNodeHeight(root->right);
}

void getSortedArray(Node* root, Node* nodes[], int* index) {
    if (root == NULL) return;
    getSortedArray(root->left, nodes, index);
    nodes[(*index)++] = root;
    getSortedArray(root->right, nodes, index);
}

Node* constructBalanced(Node* nodes[], int start, int end) {
    if (start > end) return NULL;
    int mid = (start + end) / 2;
    Node* root = nodes[mid];
    root->left = constructBalanced(nodes, start, mid - 1);
    root->right = constructBalanced(nodes, mid + 1, end);
    return root;
}

Node* balanceTree(Node* root) {
    int n = getTotalNodes(root);
    Node* nodes[n];
    int index = 0;
    getSortedArray(root, nodes, &index);

    return constructBalanced(nodes, 0, n - 1);
}

int main() {
    srand(time(NULL));

    int optiune;
    Node *head = NULL;

    while ((optiune = meniu())) {
        if (optiune == 1) {
            head = createNodes(head);
        }
        if (optiune == 2) {
            system("cls");
            printf("Nodurile afisate in preordine:\n");
            preorderSearch(head);
            printf("\n");
            system("pause");
        }
        if (optiune == 3) {
            system("cls");
            printf("Nodurile afisate in inordine:\n");
            inorderSearch(head);
            printf("\n");
            system("pause");
        }
        if (optiune == 4) {
            system("cls");
            printf("Nodurile afisate in postordine:\n");
            postorderSearch(head);
            printf("\n");
            system("pause");
        }
        if (optiune == 5) {
            system("cls");
            printf("Nodurile afisate in largime:\n");
            searchLargime(head);
            printf("\n");
            system("pause");
        }
        if (optiune == 6) {
            system("cls");
            printf("Balance Factor: %d", getNodeHeight(head->right) - getNodeHeight(head->left));
            system("pause");
        }
    }
    return 0;
}
