#include <stdio.h>
#include <stdlib.h>

typedef struct Film {
    int id;
    char nume[30];
    char descriere[1024];
    char limba[20];

    char** genuri;
    int genuriSize;

    float rating;

    struct Film* next;
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

    film->next = NULL;
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

void displayNodes(Node* head) {
    if (head == NULL) return;

    showFilmInfo(head->data);
    displayNodes(head->right);
    displayNodes(head->left);
}

int getTreeDepth(Node* node) {
    int sum = 0;
    if (node == NULL) return sum;

    int right = getTreeDepth(node->right);
    int left = getTreeDepth(node->left);

    if (right >= left) {
        sum += right;
    } else {
        sum += left;
    }

    return sum + 1;
}

int meniu() {
    system("cls");

    printf("Selecteaza o optiune:\n");
    printf("0. Iesire din program\n");
    printf("1. Citeste de la tastatura unul sau mai multe elemente\n");
    printf("2. Afiseaza elementele arborelui\n");
    printf("3. Afiseaza adancimea arborelui\n");

    int optiune;
    scanf("%d", &optiune);

    return optiune;
}

int main() {

    List* list = malloc(sizeof(List));
    list->head = NULL;
    list->size = 0;

    int optiune;
    while ((optiune = meniu())) {
        if (optiune == 1) {
            readNode(list);
        }
        if (optiune == 2) {
            system("cls");
            displayNodes(list->head);
            system("pause");
        }
        if (optiune == 3) {
            system("cls");
            printf("Adancimea arboreului este: %d\n", getTreeDepth(list->head));
            system("pause");
        }
    }

    return 0;
}
