#include <stdio.h>

void swap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void insertionSort(int array[], int length)
{
    int temp, j;
    for (int i = 1; i < length; i++) {
        temp = array[i];
        j = i - 1;

        while (j >= 0 && array[j] > temp) {
            array[j + 1] = array[j];
            j = j - 1;
        }
        array[j + 1] = temp;
    }
}

void selectionSort(int arr[], int length)
{
    int min;

    for (int i = 0; i < length - 1; i++)
    {
        min = i;
        for (int j = i + 1; j < length; j++)
            if (arr[j] < arr[min])
                min = j;

        if(min != i)
            swap(&arr[min], &arr[i]);
    }
}

void mergeSortedArrays(int array[], int left, int middle, int right) {
    int leftLength = middle - left + 1;
    int rightLength = right - middle;

    int leftSorted[leftLength];
    int rightSorted[rightLength];

    for (int i = 0; i < leftLength; i++) {
        leftSorted[i] = array[left + i];
    }

    for (int i = 0; i < rightLength; i++) {
        rightSorted[i] = array[middle + 1 + i];
    }

    int i = 0, j = 0, k = left;
    while (i < leftLength && j < rightLength) {
        if (leftSorted[i] > rightSorted[j]) {
            array[k] = leftSorted[i];
            i++;
        } else {
            array[k] = rightSorted[j];
            j++;
        }
        k++;
    }

    while (i < leftLength) {
        array[k] = leftSorted[i];
        i++;
        k++;
    }

    while (j < rightLength) {
        array[k] = rightSorted[j];
        j++;
        k++;
    }
}

void mergeSortRecursion(int array[], int left, int right) {
    if (right > left) {
        int middle = left + (right - left) / 2;
        mergeSortRecursion(array, left, middle);
        mergeSortRecursion(array, middle + 1, right);
        mergeSortedArrays(array, left, middle, right);
    }
}

void mergeSort(int array[], int length) {
    mergeSortRecursion(array, 0, length - 1);
}

int main() {
    int array[] = {10, 7, 15, 1, 7, 2};
    insertionSort(array, 6);

    for (int i = 0; i < 6; i++) {
        printf("%d ", array[i]);
    }

    return 0;
}
