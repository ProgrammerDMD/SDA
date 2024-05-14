#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void bubbleSort(int* array, int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (array[j] > array[j + 1]) {
                swap(&array[j], &array[j + 1]);
            }
        }
    }
}

void insertionSort(int* array, int size) {
    int key, j;
    for (int i = 1; i < size; i++) {
        key = array[i];
        j = i - 1;

        while (j >= 0 && array[j] > key) {
            array[j + 1] = array[j];
            j--;
        }
        array[j + 1] = key;
    }
}

void showElements(int* array, int size) {
    for (int i = 0; i < 50 && i < size; i++) {
        printf("%d ", array[i]);
    }

    printf("\n");
}

int binarySearch(int* array, int size, int* data) {
    int low = 0;
    int high = size - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;

        if (array[mid] == *data) {
            return mid;
        } else if (array[mid] < *data) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    return -1;
}

int linearSearch(int* array, int size, int* data) {
    for (int i = 0; i < size; i++) {
        if (array[i] == *data) return i;
    }
    return -1;
}

void benchmark(int size, int data) {
    clock_t start, end;

    int* array1 = NULL, *array2 = NULL;
    array1 = malloc(sizeof(int) * size);
    array2 = malloc(sizeof(int) * size);

    for (int i = 0; i < size; i++) {
        array1[i] = array2[i] = rand() % 1000000 + 1;
    }

    printf("Elementele primului tablou: ");
    showElements(array1, size);

    printf("\nElementele pentru al doilea tablou: ");
    showElements(array2, size);

    // Bubble Sort
    printf("\nSe executa sortarea Bubble Sort pentru %d elemente\n", size);
    start = clock();
    bubbleSort(array1, size);
    end = clock();
    printf("Bubble Sort a fost executat in %lf secunde\n", (double) (end - start) / CLOCKS_PER_SEC);

    // Insertion Sort
    printf("\nSe executa sortarea Insertion Sort pentru %d elemente\n", size);
    start = clock();
    insertionSort(array2, size);
    end = clock();
    printf("Insertion Sort a fost executat in %lf secunde\n", (double) (end - start) / CLOCKS_PER_SEC);

    printf("\nElementele primului tablou: ");
    showElements(array1, size);

    printf("\nElementele pentru al doilea tablou: ");
    showElements(array2, size);

    // Binary Search
    printf("\nCautare binara de 10.000 ori\n");
    start = clock();

    for (int i = 0; i < 10000; i++) {
        binarySearch(array1, size, &data);
    }
    end = clock();
    printf("Binary Search a fost executat in %lf secunde\n", (double) (end - start) / CLOCKS_PER_SEC);


    // Linear Search
    printf("\nCautare lineara de 10.000 ori\n");
    start = clock();

    for (int i = 0; i < 10000; i++) {
        linearSearch(array1, size, &data);
    }
    end = clock();
    printf("Linear Search a fost executat in %lf secunde\n", (double) (end - start) / CLOCKS_PER_SEC);


    // Curatarea memoriei
    free(array1);
    free(array2);
}

void addArrays(int* a, int* b, int* c, int* maxDigits) {
    int carry = 0;
    for (int i = *maxDigits - 1; i >= 0; i--) {
        int sum = a[i] + b[i] + carry;
        c[i] = sum % 10;
        carry = sum / 10;
    }
}

int* fibonacci(int size, int maxDigits) {
    int* first = (int*) calloc(maxDigits, sizeof(int));
    int* second = (int*) calloc(maxDigits, sizeof(int));
    int* result = (int*) calloc(maxDigits, sizeof(int));

    first[maxDigits - 1] = 1;
    second[maxDigits - 1] = 1;

    if (size <= 2)
        return first;

    for (int i = 3; i <= size; ++i) {
        addArrays(first, second, result, &maxDigits);

        for (int j = 0; j < maxDigits; ++j) {
            first[j] = second[j];
            second[j] = result[j];
        }
    }

    free(first);
    free(second);

    return result;
}

int main() {
    srand(time(NULL));

    printf("Cate numere doresti sa citesti? ");
    int N;
    scanf("%d", &N);
    printf("Ce numar doresti sa cauti? ");

    int data;
    scanf("%d", &data);

    benchmark(N, data);
    system("pause");
    system("cls");

    printf("Al catalea numar Fibonacci doresti sa-l citesti? ");
    scanf("%d", &N);

    clock_t start, end;
    start = clock();
    int* fib = fibonacci(N, 20000);

    printf("Numarul Fibonacci la pozitia %d este ", N);
    int startPrinting = 0;
    for (int i = 0; i < 20000; i++) {
        if (fib[i] != 0) {
            startPrinting = 1;
        }
        if (startPrinting) printf("%d", fib[i]);
    }
    end = clock();

    printf("\nNumarul a fost cautat in %lf secunde\n", (double) (end - start) / CLOCKS_PER_SEC);
    system("pause");

    return 0;
}
