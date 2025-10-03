#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <locale.h>

/* Shell Sort */
void shell(int* items, int count) {
    int i, j, gap, k;
    int x;
    int a[5] = { 9, 5, 3, 2, 1 };

    for (k = 0; k < 5; k++) {
        gap = a[k];
        for (i = gap; i < count; ++i) {
            x = items[i];
            for (j = i - gap; (j >= 0) && (x < items[j]); j = j - gap)
                items[j + gap] = items[j];
            items[j + gap] = x;
        }
    }
}

/* Quick Sort (собственный) */
void qs(int* items, int left, int right) {
    if (items == NULL) return;
    if (left >= right) return;

    int i = left;
    int j = right;
    int x = items[(left + right) / 2]; // опорный элемент

    while (i <= j) {
        while (i <= right && items[i] < x) i++;
        while (j >= left && items[j] > x) j--;

        if (i <= j) {
            int tmp = items[i];
            items[i] = items[j];
            items[j] = tmp;
            i++;
            j--;
        }
    }

    if (left < j) qs(items, left, j);
    if (i < right) qs(items, i, right);
}

/* Генераторы массивов */
void generate_random(int arr[], int n) {
    for (int i = 0; i < n; i++) arr[i] = rand() % 10000;
}

void generate_ascending(int arr[], int n) {
    for (int i = 0; i < n; i++) arr[i] = i;
}

void generate_descending(int arr[], int n) {
    for (int i = 0; i < n; i++) arr[i] = n - 1 - i;
}

void generate_mixed(int arr[], int n) {
    int half = n / 2;
    for (int i = 0; i < half; i++) arr[i] = i;
    for (int i = half; i < n; i++) arr[i] = (n - 1) - (i - half);
}

/* Замер времени сортировок */
void measure_time(void (*sort_func)(int*, int), int arr[], int n, const char* algo_name, const char* data_type) {
    int* temp = (int*)malloc(n * sizeof(int));
    memcpy(temp, arr, n * sizeof(int));

    clock_t start = clock();
    sort_func(temp, n);
    clock_t end = clock();

    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("%-15s %-25s: %.6f сек\n", algo_name, data_type, time_taken);

    free(temp);
}

void measure_qs_time(int arr[], int n, const char* data_type) {
    int* temp = (int*)malloc(n * sizeof(int));
    memcpy(temp, arr, n * sizeof(int));

    clock_t start = clock();
    qs(temp, 0, n - 1);
    clock_t end = clock();

    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("%-15s %-25s: %.6f сек\n", "Quick Sort", data_type, time_taken);

    free(temp);
}

/* qsort из stdlib */
int cmp_int(const void* a, const void* b) {
    int x = *(const int*)a;
    int y = *(const int*)b;
    return (x > y) - (x < y);
}

void measure_std_qsort_time(int arr[], int n, const char* data_type) {
    int* temp = (int*)malloc(n * sizeof(int));
    memcpy(temp, arr, n * sizeof(int));

    clock_t start = clock();
    qsort(temp, n, sizeof(int), cmp_int);
    clock_t end = clock();

    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("%-15s %-25s: %.6f сек\n", "stdlib qsort", data_type, time_taken);

    free(temp);
}

int main() {
    setlocale(LC_ALL, "");
    const int n = 10000;
    int* arr = (int*)malloc(n * sizeof(int));

    srand((unsigned)time(NULL));

    int test_cases = 4;
    void (*generators[])(int[], int) = {
        generate_random, generate_ascending,
        generate_descending, generate_mixed
    };

    const char* data_types[] = {
        "Случайные данные", "Возрастающая",
        "Убывающая", "Смешанная"
    };

    for (int i = 0; i < test_cases; i++) {
        generators[i](arr, n);
        printf("\n=== %s ===\n", data_types[i]);

        measure_time(shell, arr, n, "Shell Sort", data_types[i]);
        measure_qs_time(arr, n, data_types[i]);
        measure_std_qsort_time(arr, n, data_types[i]);
    }

    free(arr);
    return 0;
}