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

    char** genuri;
    int genuriSize;

    Persoana* regizor;
    Persoana** actori;
    int actoriSize;

    float rating;
} Film;

typedef struct Registru {
    Persoana** persoane;
    int persoaneSize;

    Film** filme;
    int filmeSize;
} Registru;

void adaugaFilm(Registru* registru) {
    system("cls");

    Film* film = malloc(sizeof(Film));
    printf("Care este numele filmului? ");
    scanf(" %[^\n]30s", film->nume);

    printf("Care este descrierea filmului? ");
    scanf(" %[^\n]1024s", film->descriere);

    printf("Care este limba filmului? ");
    scanf(" %20s", film->limba);

    int genuri;
    printf("Cate genuri are filmul? ");
    scanf("%d", &genuri);

    printf("Introdu %d genuri separate prin spatiu: ", genuri);
    film->genuri = malloc(sizeof(char*) * genuri);
    film->genuriSize = genuri;

    for (int i = 0; i < genuri; i++) {
        film->genuri[i] = malloc(sizeof(char) * 30);
        scanf("%30s", film->genuri[i]);
    }

    printf("Care este regizorul? (ID) ");

    int regizor;
    scanf("%d", &regizor);
    film->regizor = registru->persoane[regizor];

    printf("Cati actori are acest film? ");
    int actori;
    scanf("%d", &actori);

    film->actori = malloc(sizeof(Persoana*) * actori);
    film->actoriSize = actori;

    printf("Introdu ID-ul a %d actori separati prin spatiu: ");
    int id;
    for (int i = 0; i < actori; i++) {
        scanf("%d", &id);
        film->actori[i] = registru->persoane[id];
    }

    printf("Care este rating-ul acestui film? ");
    float rating;
    scanf("%f", &rating);

    film->rating = rating;

    Film** temp = realloc(registru->filme, (registru->filmeSize + 1) * sizeof(Film*));
    if (temp == NULL) {
        exit(-1);
    }

    registru->filme = temp;
    registru->filmeSize += 1;
    registru->filme[registru->filmeSize - 1] = film;
}

void adaugaPersoana(Registru* registru) {
    system("cls");
    printf("Cate persoane doresti sa adaugi?\n");

    int size;
    scanf("%d", &size);

    printf("\nIntrodu numele si prenumele acestei persoane\n");
    printf("Exemplu: Dubceac Mihai\n\n");

    Persoana* persoane = malloc(sizeof(Persoana) * size);

    for (int i = 0; i < size; i++) {
        scanf("%30s %30s", persoane[i].nume, persoane[i].prenume);
    }

    Persoana** temp = realloc(registru->persoane, (registru->persoaneSize + size) * sizeof(Persoana*));
    if (temp == NULL) {
        exit(-1);
    }

    registru->persoane = temp;

    int oldSize = registru->persoaneSize;
    registru->persoaneSize += size;

    for (int i = oldSize, j = 0; i < registru->persoaneSize; i++, j++) {
        registru->persoane[i] = &persoane[j];
    }
}

void adaugaPersoanaPozitie(Registru* registru) {
    system("cls");

    printf("Introdu numele si prenumele acestei persoane\n");
    printf("Exemplu: Dubceac Mihai\n\n");

    Persoana* persoana = malloc(sizeof(Persoana));
    scanf("%30s %30s", persoana->nume, persoana->prenume);

    printf("\nLa ce pozitie doresti sa adaugi aceasta persoana? ");

    unsigned int pozitie;
    scanf("%u", &pozitie);

    Persoana** temp = malloc((registru->persoaneSize + 1) * sizeof(Persoana*));
    if (temp == NULL) {
        exit(-1);
    }

    for (int i = 0; i < pozitie; i++) {
        temp[i] = registru->persoane[i];
    }

    for (int i = pozitie + 1, j = pozitie; j < registru->persoaneSize; i++, j++) {
        temp[i] = registru->persoane[j];
    }

    temp[pozitie] = persoana;
    registru->persoaneSize += 1;

    free(registru->persoane);
    registru->persoane = temp;
}

void afisarePersoane(Registru* registru) {
    system("cls");

    printf("\n");
    printf("Persoanele stocate in registru sunt:\n");
    for (int i = 0; i < registru->persoaneSize; i++) {
        Persoana persoana = *registru->persoane[i];
        printf("%d. %s %s\n", i, persoana.nume, persoana.prenume);
    }
    printf("\n");
}

void afisareFilme(Registru* registru) {
    system("cls");
    printf("\n");
    printf("Filmele stocate in registru sunt:\n");
    for (int i = 0; i < registru->filmeSize; i++) {
        Film film = *registru->filme[i];

        printf("ID %d:\n", i);
        printf("%s\n", film.nume);
        printf("Descriere: %s\n", film.descriere);
        printf("Limba: %s\n", film.limba);
        printf("Genuri: ");

        for (int j = 0; j < film.genuriSize; j++) {
            if (j + 1 == film.genuriSize) {
                printf("%s", film.genuri[j]);
                break;
            }
            printf("%s, ", film.genuri[j]);
        }
        printf("\n");

        printf("Regizor: %s %s\n", film.regizor->nume, film.regizor->prenume);
        printf("Actori: ");

        for (int j = 0; j < film.actoriSize; j++) {
            if (j + 1 == film.actoriSize) {
                printf("%s %s", film.actori[j]->nume, film.actori[j]->prenume);
                break;
            }
            printf("%s %s, ", film.actori[j]->nume, film.actori[j]->prenume);
        }

        printf("\nRating: %.2f", film.rating);
        printf("\n\n");
    }
}

void cautaPersoana(Registru* registru) {
    system("cls");
    printf("Care este numele si prenumele acestei persoane? ");

    char nume[30];
    char prenume[30];
    scanf("%s %s", nume, prenume);

    for (int i = 0; i < registru->persoaneSize; i++) {
        Persoana persoana = *(registru->persoane[i]);
        if (strcmp(nume, persoana.nume) == 0 && strcmp(prenume, persoana.prenume) == 0) {
            printf("Pozitia acestei persoane este %d\n", i);
            system("pause");
            return;
        }
    }

    printf("Aceasta persoana nu exista in registru.\n");
    system("pause");
}

void sortezaFilmeDupaRating(Registru* registru) {
    for (int i = 0; i < registru->filmeSize - 1; i++) {
        Film* film1 = registru->filme[i];
        Film* film2 = registru->filme[i + 1];
        if (film2->rating > film1->rating) {
            registru->filme[i] = film2;
            registru->filme[i + 1] = film1;
        }
    }
}

void stergeFilm(Registru* registru) {
    system("cls");
    afisareFilme(registru);
    printf("Indica pozitia filmului pe care doresti sa-l stergi: ");

    unsigned int pozitie;
    scanf("%u", &pozitie);

    Film** temp = calloc(registru->filmeSize - 1, sizeof(Film*));

    for (int i = 0; i < pozitie; i++) {
        temp[i] = registru->filme[i];
    }

    for (int i = pozitie, j = pozitie + 1; j < registru->filmeSize; i++, j++) {
        temp[i] = registru->filme[j];
    }

    free(registru->filme[pozitie]);
    free(registru->filme);

    registru->filme = temp;
    registru->filmeSize -= 1;
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
    printf("6. Sorteaza filmele dupa rating\n");
    printf("7. Adauga o persoana la o anumita pozitie\n");
    printf("8. Sterge un film\n");

    int optiune;
    scanf("%d", &optiune);

    return optiune;
}

int main() {

    Registru* registru = malloc(sizeof(Registru));
    registru->persoane = NULL;
    registru->filme = NULL;
    registru->persoaneSize = 0;
    registru->filmeSize = 0;

    int optiune;
    while ((optiune = meniu())) {
        if (optiune == 1) {
            adaugaPersoana(registru);
        }
        if (optiune == 2) {
            adaugaFilm(registru);
        }
        if (optiune == 3) {
            afisarePersoane(registru);
            system("pause");
        }
        if (optiune == 4) {
            afisareFilme(registru);
            system("pause");
        }
        if (optiune == 5) {
            cautaPersoana(registru);
        }
        if (optiune == 6) {
            sortezaFilmeDupaRating(registru);
        }
        if (optiune == 7) {
            adaugaPersoanaPozitie(registru);
        }
        if (optiune == 8) {
            stergeFilm(registru);
        }
    }

    return 0;
}