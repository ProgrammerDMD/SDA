#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Film {
    char nume[30];
    char descriere[1024];
    char limba[20];

    char** genuri;
    int genuriSize;

    float rating;

    struct Film* next;
} Film;

typedef struct List {
    Film* head;
    Film* last;
    int size;
} List;

List* createList() {
    List* list = malloc(sizeof(List));
    list->head = NULL;
    list->last = NULL;
    list->size = 0;
    return list;
}

void showNodes(List* list) {
    system("cls");

    printf("Lista filmelor este: \n");

    int i = 0;
    for (Film* node = list->head; node != NULL; node = node->next) {
        printf("ID %d:\n", i++);
        printf("%s\n", node->nume);
        printf("Descriere: %s\n", node->descriere);
        printf("Limba: %s\n", node->limba);
        printf("Genuri: ");

        for (int j = 0; j < node->genuriSize; j++) {
            if (j + 1 == node->genuriSize) {
                printf("%s", node->genuri[j]);
                break;
            }
            printf("%s, ", node->genuri[j]);
        }
        printf("\nRating: %.2f", node->rating);
        printf("\n\n");
    }
}

Film* createNode() {
    system("cls");
    Film* film = malloc(sizeof(Film));
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

void freeNode(Film* node) {
    for (int i = 0; i < node->genuriSize; i++) {
        free(node->genuri[i]);
    }

    free(node->genuri);
    node->next = NULL;
    free(node);
}

void deleteNodePosition(List* list) {
    system("cls");
    int position;
    printf("De la ce pozitie doresti sa stergi adaugi filmul? ");
    scanf("%d", &position);

    Film* node = list->head;
    Film* previous = NULL;

    list->size -= 1;

    for (int i = 0; i < position && i < list->size; i++) {
        previous = node;
        node = node->next;
    }

    if (previous == NULL) {
        list->head = node->next;
        freeNode(node);
        return;
    }

    if (node == list->last) {
        list->last = previous;
        previous->next = NULL;
        return;
    }

    previous->next = node->next;
    freeNode(node);
}

void addNodeToPosition(List* list, Film* film) {
    int position;
    printf("La ce pozitie doresti sa adaugi filmul? ");
    scanf("%d", &position);

    Film* node = list->head;
    Film* previous = NULL;

    list->size += 1;

    for (int i = 0; i < position && i < list->size; i++) {
        previous = node;
        node = node->next;
    }

    if (previous == NULL) {
        list->head = film;
        film->next = node;
        return;
    }

    if (node == list->last) {
        list->last = film;
        node->next = list->last;
        return;
    }

    previous->next = film;
    film->next = node;
}

void addNodeToLast(List* list) {
    Film* film = createNode();
    list->size += 1;
    if (list->head == NULL) {
        list->head = film;
        return;
    }

    Film* head = list->head;
    if (head->next == NULL) {
        head->next = film;
        list->last = film;
        return;
    }

    list->last->next = film;
    list->last = film;
}

void addNodeToHead(List* list) {
    Film* film = createNode();
    list->size += 1;
    if (list->head == NULL) {
        list->head = film;
        return;
    }

    Film* oldHead = list->head;
    film->next = oldHead;
    list->head = film;

    if (oldHead->next == NULL && list->last == NULL) list->last = oldHead;
}

int getNodePosition(List* list) {
    system("cls");
    printf("Care este numele acestui film? ");

    char nume[30];
    scanf(" %[^\n]30s", nume);

    int i = 0;
    for (Film* node = list->head; node != NULL; node = node->next) {
        if (strcmp(node->nume, nume) == 0) {
            return i;
        }
        i++;
    }
    return -1;
}

void swap(Film* film1, Film* film2) {
    Film* temp = malloc(sizeof(Film));

    // Stocheaza toate datele obiectului film2 intr-o un temporary node.
    strcpy(temp->nume, film1->nume);
    strcpy(temp->descriere, film1->descriere);
    strcpy(temp->limba, film1->limba);

    temp->genuri = malloc(sizeof(char*) * film1->genuriSize);

    for (int i = 0; i < film1->genuriSize; i++) {
        temp->genuri[i] = malloc(sizeof(char) * 30);
        strcpy(temp->genuri[i], film1->genuri[i]);
    }

    temp->genuriSize = film1->genuriSize;
    temp->rating = film1->rating;

    // Copiaza elementele obiectului film2 in film1.
    strcpy(film1->nume, film2->nume);
    strcpy(film1->descriere, film2->descriere);
    strcpy(film1->limba, film2->limba);

    for (int i = 0; i < film1->genuriSize; i++) {
        free(film1->genuri[i]);
    }

    film1->genuri = realloc(film1->genuri, sizeof(char*) * film2->genuriSize);
    for (int i = 0; i < film2->genuriSize; i++) {
        film1->genuri[i] = malloc(sizeof(char) * 30);
        strcpy(film1->genuri[i], film2->genuri[i]);
    }
    film1->genuriSize = film2->genuriSize;
    film1->rating = film2->rating;

    // Copiaza elementele obiectului temp in film2.
    strcpy(film2->nume, temp->nume);
    strcpy(film2->descriere, temp->descriere);
    strcpy(film2->limba, temp->limba);

    for (int i = 0; i < film2->genuriSize; i++) {
        free(film2->genuri[i]);
    }

    film2->genuri = realloc(film2->genuri, sizeof(char*) * temp->genuriSize);
    for (int i = 0; i < temp->genuriSize; i++) {
        film2->genuri[i] = malloc(sizeof(char) * 30);
        strcpy(film2->genuri[i], temp->genuri[i]);
    }
    film2->genuriSize = temp->genuriSize;
    film2->rating = temp->rating;

    // Elibereaza obiectul temp.
    freeNode(temp);
}

void sortList(List* list) {
    Film* node = list->head;
    while (node->next != NULL) {
        if (node->rating < node->next->rating) {
            swap(node, node->next);
            node = list->head;
            continue;
        }
        node = node->next;
    }
}

void reverseList(List* list) {
    if (list->head != NULL) {
        Film* previous = NULL;
        Film* current = list->head;
        Film* next = NULL;

        while (current != NULL) {
            next = current->next;
            current->next = previous;
            previous = current;

            if (previous->next == NULL) {
                list->last = previous;
            }

            current = next;
        }

        list->head = previous;
    }
}

void emptyList(List* list) {
    Film* current = list->head;
    Film* previous = NULL;

    while (current != NULL) {
        previous = current;
        current = current->next;
        freeNode(previous);
        list->size -= 1;
    }

    list->head = NULL;
    list->last = NULL;
}

int meniu() {
    system("cls");

    printf("Selecteaza o optiune:\n");
    printf("0. Iesire din program\n");
    printf("1. Adauga un film la sfarsit de lista\n");
    printf("2. Adauga un film la inceput de lista\n");
    printf("3. Afiseaza toate filmele din lista\n");
    printf("4. Cauta un film in lista\n");
    printf("5. Sorteaza lista\n");
    printf("6. Insereaza un element la o pozitie\n");
    printf("7. Sterge un element de la o pozitie\n");
    printf("8. Inverseaza lista\n");
    printf("9. Elibereaza lista\n");

    int optiune;
    scanf("%d", &optiune);

    return optiune;
}

int main() {
    int optiune;

    List* list = createList();

    while ((optiune = meniu())) {
        if (optiune == 1) {
            addNodeToLast(list);
        }
        if (optiune == 2) {
            addNodeToHead(list);
        }
        if (optiune == 3) {
            showNodes(list);
            system("pause");
        }
        if (optiune == 4) {
            int i = getNodePosition(list);
            printf("Pozitia acestui film este %d\n", i);
            system("pause");
        }
        if (optiune == 5) {
            sortList(list);
        }
        if (optiune == 6) {
            addNodeToPosition(list, createNode());
        }
        if (optiune == 7) {
            deleteNodePosition(list);
        }
        if (optiune == 8) {
            reverseList(list);
        }
        if (optiune == 9) {
            emptyList(list);
        }
    }
    return 0;
}