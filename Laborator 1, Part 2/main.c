#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Persoana {
    char nume[30];
    char prenume[30];
} Persoana;

typedef struct Film {
    char nume[30];
    char descriere[1024];
    char limba[20];

    Persoana regizor;
    int actoriSize;

    float rating;

    Persoana actori[10];
} Film;

void adaugaFilm(Persoana persoane[], Film filme[], int n) {
    for (int i = 0; i < n; i++) {
        system("cls");
        printf("Care este numele filmului? ");
        scanf(" %[^\n]30s", filme[i].nume);

        printf("Care este descrierea filmului? ");
        scanf(" %[^\n]1024s", filme[i].descriere);

        printf("Care este limba filmului? ");
        scanf(" %20s", filme[i].limba);

        printf("Care este regizorul? (ID) ");

        int regizor;
        scanf("%d", &regizor);
        filme[i].regizor = persoane[regizor];

        printf("Cati actori are acest film? ");
        scanf("%d", &filme[i].actoriSize);

        printf("Introdu ID-ul a %d actori separati prin spatiu: ", filme[i].actoriSize > 10 ? 10 : filme[i].actoriSize);
        for (int j = 0; j < filme[i].actoriSize && j < 10; j++) {
            int id;
            scanf("%d", &id);
            filme[i].actori[j] = persoane[id];
        }

        printf("Care este rating-ul acestui film? ");
        scanf("%f", &filme[i].rating);

        system("pause");
    }
}

void adaugaPersoana(Persoana persoane[], int n) {
    system("cls");

    printf("\nIntrodu numele si prenumele acestei persoane\n");
    printf("Exemplu: Dubceac Mihai\n\n");

    for (int i = 0; i < n; i++) {
        scanf("%30s %30s", persoane[i].nume, persoane[i].prenume);
    }
}

void afisarePersoane(Persoana persoane[], int n) {
    system("cls");

    printf("\n");
    printf("Persoanele stocate in registru sunt:\n");
    for (int i = 0; i < n; i++) {
        printf("%d. %s %s\n", i, persoane[i].nume, persoane[i].prenume);
    }
    printf("\n");
}

void afisareFilme(Film filme[], int n) {
    system("cls");
    printf("\n");
    printf("Filmele stocate in registru sunt:\n");
    for (int i = 0; i < n; i++) {
        Film film = filme[i];

        printf("ID %d:\n", i);
        printf("%s\n", film.nume);
        printf("Descriere: %s\n", film.descriere);
        printf("Limba: %s\n", film.limba);
        printf("Regizor: %s %s\n", film.regizor.nume, film.regizor.prenume);
        printf("Actori: ");

        for (int j = 0; j < film.actoriSize; j++) {
            if (j + 1 == film.actoriSize) {
                printf("%s %s", film.actori[j].nume, film.actori[j].prenume);
                break;
            }
            printf("%s %s, ", film.actori[j].nume, film.actori[j].prenume);
        }

        printf("\nRating: %.2f", film.rating);
        printf("\n\n");
    }
}

void cautaPersoana(Persoana persoane[], int n) {
    system("cls");
    printf("Care este numele si prenumele acestei persoane? ");

    char nume[30];
    char prenume[30];
    scanf("%s %s", nume, prenume);

    for (int i = 0; i < n; i++) {
        Persoana persoana = persoane[i];
        if (strcmp(nume, persoana.nume) == 0 && strcmp(prenume, persoana.prenume) == 0) {
            printf("Pozitia acestei persoane este %d\n", i);
            system("pause");
            return;
        }
    }

    printf("Aceasta persoana nu exista in registru.\n");
    system("pause");
}

int meniu() {
    system("cls");

    printf("Selecteaza o optiune:\n");
    printf("0. Iesire din program\n");
    printf("1. Adauga una sau mai multe persoane\n");
    printf("2. Adauga un film\n");
    printf("3. Afisare persoane\n");
    printf("4. Afisare filme\n");
    printf("5. Cauta o persoana dupa nume\n");

    int optiune;
    scanf("%d", &optiune);

    return optiune;
}

int main() {

    int numarPersoane, numarFilme;
    printf("Cate persoane trebuiesc sa fie stocate? ");
    scanf("%d", &numarPersoane);

    printf("Cate filme trebuiesc sa fie stocate? ");
    scanf("%d", &numarFilme);

    Persoana persoane[numarPersoane];
    Film filme[numarFilme];

    int optiune;
    while ((optiune = meniu())) {
        if (optiune == 1) {
            adaugaPersoana(persoane, numarPersoane);
        }
        if (optiune == 2) {
            adaugaFilm(persoane, filme, numarFilme);
        }
        if (optiune == 3) {
            afisarePersoane(persoane, numarPersoane);
            system("pause");
        }
        if (optiune == 4) {
            afisareFilme(filme, numarFilme);
            system("pause");
        }
        if (optiune == 5) {
            cautaPersoana(persoane, numarPersoane);
        }
    }

    return 0;
}