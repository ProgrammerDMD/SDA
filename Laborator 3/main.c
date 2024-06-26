#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Film {
    int id;
    char nume[30];
    char descriere[1024];
    char limba[20];

    char** genuri;
    int genuriSize;

    float rating;
} Film;

typedef struct Node {
    Film* data;
    struct Node* left;
    struct Node* right;
} Node;

typedef struct List {
    Node* head;
    int size;
} List;

Film* createFilm(int id) {
    system("cls");

    Film* film = malloc(sizeof(Film));
    film->id = id;

    printf("Care este numele filmului? ");
    scanf(" %[^\n]30s", film->nume);

    printf("Care este descrierea filmului? ");
    scanf(" %[^\n]1024s", film->descriere);

    printf("Care este limba filmului? ");
    scanf(" %20s", film->limba);

    printf("Cate genuri are filmul? ");
    scanf("%d", &film->genuriSize);

    printf("Introdu %d genuri separate prin spatiu: ", film->genuriSize);
    film->genuri = malloc(sizeof(char*) * film->genuriSize);

    for (int i = 0; i < film->genuriSize; i++) {
        film->genuri[i] = malloc(sizeof(char) * 30);
        scanf("%30s", film->genuri[i]);
    }

    printf("Care este rating-ul acestui film? ");
    scanf("%f", &film->rating);

    return film;
}

Node* createNode(int id) {
    Node* node = malloc(sizeof(Node));
    node->data = createFilm(id);
    node->left = NULL;
    node->right = NULL;
    return node;
}

Node* addNode(List* list, Node* head) {
    if (head->right == NULL) {
        Node* node = createNode(list->size);
        head->right = node;
        list->size += 1;
        return node;
    }

    if (head->left == NULL) {
        Node* node = createNode(list->size);
        head->left = node;
        list->size += 1;
        return node;
    }

    return addNode(list, head->right);
}

void readNode(List* list) {
    system("cls");
    printf("Cate elemente doresti sa citesti? ");

    int n;
    scanf("%d", &n);

    if (n >= 1 && list->head == NULL) {
        n -= 1;

        Node* node = createNode(list->size);
        list->head = node;
        list->size += 1;
    }

    for (int i = 0; i < n; i++) {
        addNode(list, list->head);
    }
}

void showFilmInfo(Film* film) {
    printf("ID %d:\n", film->id);
    printf("%s\n", film->nume);
    printf("Descriere: %s\n", film->descriere);
    printf("Limba: %s\n", film->limba);
    printf("Genuri: ");

    for (int j = 0; j < film->genuriSize; j++) {
        if (j + 1 == film->genuriSize) {
            printf("%s", film->genuri[j]);
            break;
        }
        printf("%s, ", film->genuri[j]);
    }

    printf("\nRating: %.2f", film->rating);
    printf("\n\n");
}

Node* searchNode(Node* head, char nume[30]) {
    if (head == NULL) return NULL;
    if (strcmp(head->data->nume, nume) == 0) return head;

    Node* right = searchNode(head->right, nume);
    Node* left = searchNode(head->left, nume);

    if (right != NULL) return right;
    if (left != NULL) return left;

    return NULL;
}

void displayNodes(Node* head) {
    if (head == NULL) return;

    showFilmInfo(head->data);
    displayNodes(head->left);
    displayNodes(head->right);
}

int getTreeDepth(Node* node, int sum) {
    if (node == NULL) return sum;

    int right = getTreeDepth(node->right, sum + 1);
    int left = getTreeDepth(node->left, sum + 1);

    if (right >= left) {
        return right;
    } else {
        return left;
    }
}

int getNodeDepth(char* nume, Node* root, int sum) {
    if (root == NULL) return -1;
    if (strcmp(root->data->nume, nume) == 0) return sum;

    int right = getNodeDepth(nume, root->right, sum + 1);
    int left = getNodeDepth(nume, root->left, sum + 1);

    if (right != -1) return right;
    if (left != -1) return left;

    return -1;
}

int getNodeHeight(Node* node) {
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

void showLeaves(Node* node) {
    if (node != NULL && node->right == NULL && node->left == NULL) {
        showFilmInfo(node->data);
    } else if (node != NULL) {
        showLeaves(node->left);
        showLeaves(node->right);
    }
}

void deleteTree(List* list, Node* node) {
    if (node == NULL) return;
    deleteTree(list, node->right);
    deleteTree(list, node->left);

    Film* film = node->data;
    for (int i = 0; i < film->genuriSize; i++) {
        free(film->genuri[i]);
    }

    free(film->genuri);
    free(node->data);
    free(node);
    list->size -= 1;
}

int meniu() {
    system("cls");

    printf("Selecteaza o optiune:\n");
    printf("0. Iesire din program\n");
    printf("1. Citeste de la tastatura unul sau mai multe filme\n");
    printf("2. Afiseaza elementele arborelui\n");
    printf("3. Afiseaza adancimea arborelui\n");
    printf("4. Cauta un film dupa nume\n");
    printf("5. Afiseaza adancimea unui nod\n");
    printf("6. Afiseaza inaltimea unui nod\n");
    printf("7. Afiseaza frunzele arborelui\n");
    printf("8. Sterge arborele\n");

    int optiune;
    scanf("%d", &optiune);

    return optiune;
}

int main() {

    List* list = malloc(sizeof(List));
    list->head = NULL;
    list->size = 0;

    int optiune;
    char search[30];
    while ((optiune = meniu())) {
        if (optiune == 1) {
            readNode(list);
        }

        if (optiune == 2) {
            system("cls");
            printf("Lista filmelor este:\n");
            displayNodes(list->head);
            system("pause");
        }

        if (optiune == 3) {
            system("cls");
            printf("Adancimea arboreului este: %d\n", getTreeDepth(list->head, 0));
            system("pause");
        }

        if (optiune == 4) {
            system("cls");

            printf("Care este numele filmului? ");
            scanf(" %[^\n]s", search);

            Node* node = searchNode(list->head, search);
            if (node == NULL) {
                printf("Acest film nu exista!\n");
            } else {
                showFilmInfo(node->data);
            }
            system("pause");
        }

        if (optiune == 5) {
            system("cls");

            printf("Care este numele filmului? ");
            scanf(" %[^\n]s", search);

            int depth = getNodeDepth(search, list->head, 0);
            if (depth == -1) {
                printf("Acest film nu exista!\n");
            } else {
                printf("Adancimea acestui nod este: %d\n", depth);
            }
            system("pause");
        }

        if (optiune == 6) {
            system("cls");

            printf("Care este numele filmului? ");
            scanf(" %[^\n]s", search);

            Node* node = searchNode(list->head, search);
            if (node == NULL) {
                printf("Acest film nu exista!\n");
            } else {
                printf("Inaltimea acestui nod este: %d\n", getNodeHeight(node));
            }

            system("pause");
        }

        if (optiune == 7) {
            system("cls");

            printf("Frunzele arborelui sunt:\n");
            showLeaves(list->head);

            system("pause");
        }

        if (optiune == 8) {
            system("cls");

            deleteTree(list, list->head);
            list->head = NULL;

            printf("Arborele a fost eliberat!\n");

            system("pause");
        }
    }

    return 0;
}
