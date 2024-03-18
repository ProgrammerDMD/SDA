#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Date {
    int year, month, day;
} Date;

typedef struct Category {
    char* name;
    long double revenue;
    long double revenueMonthly[12];
} Category;

typedef struct Product {
    char* name;
    double revenue;
} Product;

typedef struct Transaction {
    unsigned int id;
    Date date;
    char* name;
    char* category;
    char* subcategory;
    double price;
    unsigned int quantity;
    char* country;
    char* city;
} Transaction;

int compare(const void* transaction1, const void* transaction2) {
    Transaction* t1 = ((Transaction*) transaction1);
    Transaction* t2= ((Transaction*) transaction2);

    if (t1->date.year > t2->date.year) return 1;
    if (t1->date.year < t2->date.year) return -1;

    return 0;
}

int compareRevenue(const void* product1, const void* product2) {
    Product* p1 = ((Product*) product1);
    Product* p2= ((Product*) product2);

    if (p1->revenue < p2->revenue) return 1;
    if (p1->revenue > p2->revenue) return -1;

    return 0;
}

int compareRevenueCategory(const void* category1, const void* category2) {
    Category* c1 = ((Category*) category1);
    Category* c2= ((Category*) category2);

    if (c1->revenue < c2->revenue) return 1;
    if (c1->revenue > c2->revenue) return -1;

    return 0;
}

void readDate(Date* date, char* source, int lastPosition) {
    char buffer[5];
    int k = 0;

    for (int i = 0, bufferIndex = 0; i < lastPosition; i++) {
        buffer[bufferIndex++] = source[i];
        if (source[i] == '-' || source[i] == ',') {
            k += 1;
            buffer[bufferIndex - 1] = '\0';
            bufferIndex = 0;

            switch (k) {
                case 1:
                    date->year = strtol(buffer, NULL, 10);
                    break;
                case 2:
                    date->month = strtol(buffer, NULL, 10);
                    break;
                case 3:
                    date->day = strtol(buffer, NULL, 10);
                    break;
            }

            memset(buffer, 0, 5);
        }
    }
}

void readId(unsigned int* id, char* source, int firstPosition, int lastPosition) {
    char buffer[10];
    for (int i = firstPosition, k = 0; i < lastPosition; i++) {
        buffer[k++] = source[i];
    }
    *id = strtoul(buffer, NULL, 10);
}

void readString(char* dest, char* source, int firstPosition, int lastPosition) {
    char buffer[128];
    int k = 0;
    for (int i = firstPosition; i < lastPosition; i++) {
        buffer[k++] = source[i];
    }
    buffer[k] = '\0';
    strcpy(dest, buffer);
}

int readLine(FILE* file, Transaction* transaction) {
    char line[512];
    char* read = fgets(line, 512, file);

    if (read == NULL) return 0;

    unsigned int length = strlen(line);

    transaction->name = malloc(sizeof(char) * 10);
    transaction->category = malloc(sizeof(char) * 20);
    transaction->subcategory = malloc(sizeof(char) * 20);
    transaction->country = malloc(sizeof(char) * 50);
    transaction->city = malloc(sizeof(char) * 50);

    char numberBuffer[20];

    for (int i = 0, count = 0, start = 0; i < length; i++) {
        if (line[i] == ',' || line[i] == '\n') {
            count += 1;
            switch (count) {
                case 1:
                    readDate(&transaction->date, line, i + 1);
                    break;
                case 2:
                    readId(&transaction->id, line, start, i);
                    break;
                case 3:
                    readString(transaction->name, line, start, i);
                    break;
                case 4:
                    readString(transaction->category, line, start, i);
                    break;
                case 5:
                    readString(transaction->subcategory, line, start, i);
                    break;
                case 6:
                    readString(numberBuffer, line, start, i);
                    transaction->price = strtod(numberBuffer, NULL);
                    break;
                case 7:
                    readString(numberBuffer, line, start, i);
                    transaction->quantity = strtoul(numberBuffer, NULL, 10);
                    break;
                case 8:
                    readString(transaction->country, line, start, i);
                    break;
                case 9:
                    readString(transaction->city, line, start, i);
                    break;
            }
            start = i + 1;
        }
    }
    return 1;
}

Product* getProduct(char* name, Product* products, int size) {
    for (int i = 0; i < size; i++) {
        if (strcmp(name, products[i].name) == 0) return &products[i];
    }
    return NULL;
}

Category* getCategory(char* name, Category* categories, int size) {
    for (int i = 0; i < size; i++) {
        if (strcmp(name, categories[i].name) == 0) return &categories[i];
    }
    return NULL;
}

int main() {
    FILE* file = fopen("sales.csv", "r");

    if (file == NULL) {
        printf("Fisierul nu exista!");
        return -1;
    }

    char text[512];
    fgets(text, 512, file);

    int maxSizeTransactions = 2,
            maxSizeProducts = 2,
            maxSizeCategories = 2,
            multiplierTransactions = 2,
            multiplierProducts = 2,
            multiplierCategories = 2,
            sizeTransactions = 0,
            sizeProducts = 0,
            sizeCategories = 0,
            minYear = INT_MAX,
            maxYear = INT_MIN;

    Transaction* transactions = malloc(sizeof(Transaction) * multiplierTransactions);
    Product* products = malloc(sizeof(Product) * multiplierProducts);
    Category* categories = malloc(sizeof(Category) * multiplierCategories);

    // Citirea produselor si definirea a doua variabile pe care le folosim pentru a crea un vector cu toti anii.
    while (readLine(file, &transactions[sizeTransactions])) {
        if (sizeTransactions == maxSizeTransactions - 1) {
            multiplierTransactions *= 1.25;
            Transaction* temp = realloc(transactions, sizeof(Transaction) * multiplierTransactions * maxSizeTransactions);

            if (temp == NULL) {
                printf("\nNu se poate aloca memorie!");
                exit(-1);
            }

            maxSizeTransactions *= multiplierTransactions;
            transactions = temp;
        }

        if (sizeProducts == maxSizeProducts - 1) {
            multiplierProducts *= 1.25;

            Product* temp = realloc(products, sizeof(Product) * multiplierProducts * maxSizeProducts);
            if (temp == NULL) {
                printf("\nNu se poate aloca memorie!");
                exit(-1);
            }

            maxSizeProducts *= multiplierProducts;
            products = temp;
        }

        if (sizeCategories == maxSizeCategories - 1) {
            multiplierCategories *= 1.25;

            Category* temp = realloc(categories, sizeof(Category) * multiplierCategories * maxSizeCategories);
            if (temp == NULL) {
                printf("\nNu se poate aloca memorie!");
                exit(-1);
            }

            maxSizeCategories *= multiplierCategories;
            categories = temp;
        }

        Transaction transaction = transactions[sizeTransactions];
        Product* product = getProduct(transaction.name, products, sizeProducts);
        Category* category = getCategory(transaction.category, categories, sizeCategories);
        if (product == NULL) {
            products[sizeProducts].name = transaction.name;
            products[sizeProducts].revenue = transaction.quantity * transaction.price;
            sizeProducts += 1;
        } else {
            product->revenue += transaction.quantity * transaction.price;
        }

        if (category == NULL) {
            categories[sizeCategories].name = transaction.category;
            categories[sizeCategories].revenue = transaction.quantity * transaction.price;

            for (int i = 0; i < 12; i++) {
                categories[sizeCategories].revenueMonthly[i] = 0;
            }
            categories[sizeCategories].revenueMonthly[transaction.date.month - 1] = categories[sizeCategories].revenue;

            sizeCategories += 1;
        } else {
            category->revenue += transaction.quantity * transaction.price;
            category->revenueMonthly[transaction.date.month - 1] += transaction.quantity * transaction.price;
        }

        if (transaction.date.year < minYear) minYear = transaction.date.year;
        if (transaction.date.year > maxYear) {
            maxYear = transaction.date.year;
        }
        sizeTransactions += 1;
    }

    fclose(file);

    // Sortarea produselor
    qsort(transactions, sizeTransactions, sizeof(Transaction), compare);
    qsort(products, sizeProducts, sizeof(Product), compareRevenue);
    qsort(categories, sizeCategories, sizeof(Category), compareRevenueCategory);


    // Cele mai vandute produse pe fiecare luna
    file = fopen("products-top.txt", "w");
    fprintf(file, "Cele mai vandute produse sunt:\n");

    for (int i = 0; i < 5 && i < sizeProducts; i++) {
        fprintf(file, "%d. \"%s\" cu venitul %.2f\n", i + 1, products[i].name, products[i].revenue);
    }

    fclose(file);

    // Categoriile cele mai vandute pe fiecare luna

    char months[12][11] = {
            "Ianuarie", "Februarie", "Martie",
            "Aprilie", "Mai", "Iunie",
            "Iulie", "August", "Septembrie",
            "Octombrie\0", "Noiembrie", "Decembrie"
    };

    file = fopen("categories-top.txt", "w");
    fprintf(file, "%-15s %-15s", "Categoria", "Venitul");
    for (int i = 0; i < 12; i++) {
        fprintf(file, "%-15s", months[i]);
    }

    fprintf(file, "\n");

    for (int i = 0; i < sizeCategories; i++) {
        fprintf(file, "%-15s %-15.2Lf", categories[i].name, categories[i].revenue);

        for (int j = 0; j < 12; j++) {
            fprintf(file, "%-15.2Lf", categories[i].revenueMonthly[j]);
        }

        fprintf(file, "\n");
    }

    fclose(file);

    unsigned int arraySize = maxYear - minYear + 1;

    // Sortarea veniturilor
    long double** venit = malloc(sizeof(double*) * arraySize);

    for (int i = 0, startIndex = 0; i < arraySize; i++) {
        long double* venitPerMonth = calloc(12, sizeof(long double));
        venit[i] = venitPerMonth;

        int year = i + minYear;

        Transaction transaction = transactions[startIndex];
        while (transaction.date.year == year) {
            venitPerMonth[transaction.date.month - 1] += transaction.price * transaction.quantity;
            startIndex += 1;
            transaction = transactions[startIndex];
        }
    }

    file = fopen("sales-sorted.txt", "w");

    fprintf(file, "%-9s", "Anul");
    for (int i = 0; i < 12; i++) {
        fprintf(file, "%-15s", months[i]);
    }

    fprintf(file, "\n");

    for (int i = 0; i < arraySize; i++) {
        fprintf(file, "%-9d", i + 1970);

        for (int j = 0; j < 12; j++) {
            fprintf(file, "%-15.2Lf", venit[i][j]);
        }

        free(venit[i]);
        fprintf(file, "\n");
    }

    free(venit);

    return 0;
}