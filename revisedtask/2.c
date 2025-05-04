#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

// Structure to pass sorting parameters to threads
typedef struct {
    int *array;
    int start;
    int end;
} SortParams;

// Thread function to perform bubble sort on a portion of the array
void* sort_thread(void *arg) {
    SortParams *params = (SortParams *)arg;
    int *array = params->array;
    int start = params->start;
    int end = params->end;

    for (int i = start; i < end; i++) {
        for (int j = start; j < end - 1; j++) {
            if (array[j] > array[j + 1]) {
                int temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }

    return NULL;
}

// Function to merge two sorted subarrays: array[start..mid-1] and array[mid..end-1]
void merge(int *array, int start, int mid, int end) {
    int left_size = mid - start;
    int right_size = end - mid;

    // Allocate memory for temporary arrays
    int *left = malloc(left_size * sizeof(int));
    int *right = malloc(right_size * sizeof(int));

    // Copy data to temporary arrays
    for (int i = 0; i < left_size; i++)
        left[i] = array[start + i];
    for (int j = 0; j < right_size; j++)
        right[j] = array[mid + j];

    // Merge temporary arrays back into original
    int i = 0, j = 0, k = start;
    while (i < left_size && j < right_size) {
        if (left[i] <= right[j]) {
            array[k++] = left[i++];
        } else {
            array[k++] = right[j++];
        }
    }

    // Copy any remaining elements
    while (i < left_size) array[k++] = left[i++];
    while (j < right_size) array[k++] = right[j++];

    free(left);
    free(right);
}

// Utility function to print an array
void print_array(const int *array, int size) {
    for (int i = 0; i < size; i++)
        printf("%d ", array[i]);
    printf("\n");
}

int main() {
    const int ARRAY_SIZE = 20;
    int array[ARRAY_SIZE];

    // Seed random number generator and populate array
    srand(time(NULL));
    for (int i = 0; i < ARRAY_SIZE; i++)
        array[i] = rand() % 100;

    printf("Original array:\n");
    print_array(array, ARRAY_SIZE);

    // Define thread identifiers and sorting parameters
    pthread_t thread1, thread2;
    SortParams params1 = { array, 0, ARRAY_SIZE / 2 };
    SortParams params2 = { array, ARRAY_SIZE / 2, ARRAY_SIZE };

    // Create threads to sort each half
    pthread_create(&thread1, NULL, sort_thread, &params1);
    pthread_create(&thread2, NULL, sort_thread, &params2);

    // Wait for sorting threads to complete
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("\nAfter sorting each half:\n");
    print_array(array, ARRAY_SIZE);

    // Merge sorted halves into a single sorted array
    merge(array, 0, ARRAY_SIZE / 2, ARRAY_SIZE);

    printf("\nFully sorted array:\n");
    print_array(array, ARRAY_SIZE);

    return 0;
}
